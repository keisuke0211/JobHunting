//◇======================◇
//｜　　　　　　　  　　　｜
//｜　ランキングの処理    ｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include "ranking_score.h"
#include "time.h"
//マクロ定義
#define NUM_PLASE		(3)	//ランキング桁数
#define NUM_RANKING		(6)	//ランキングの数
#define NUM_RANKING1	(5)	//ランキングの数
#define MAX_RANKING		(5)	//ランキング準拠順位５
LPDIRECT3DTEXTURE9 g_pTextureRanking[NUM_RANKING] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;
Ranking g_aRanking[NUM_RANKING];
int g_nRanking;
int g_aData[NUM_RANKING];

//===================================================
//ランキングの初期化処理
//===================================================
void InitRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;
	int nCntRanking;

	//デバイスの取得
	pDevice = GetDevice();

	D3DXCreateTextureFromFile(pDevice,"data/texture/number001.png",&g_pTextureRanking[0]);
	D3DXCreateTextureFromFile(pDevice,"data/texture/number002.png",&g_pTextureRanking[1]);
	D3DXCreateTextureFromFile(pDevice,"data/texture/number003.png",&g_pTextureRanking[2]);
	D3DXCreateTextureFromFile(pDevice,"data/texture/number004.png",&g_pTextureRanking[3]);
	D3DXCreateTextureFromFile(pDevice,"data/texture/number004.png",&g_pTextureRanking[4]);
	D3DXCreateTextureFromFile(pDevice,"data/texture/number005.png",&g_pTextureRanking[5]);


	g_aData[0] = 5000;
	g_aData[1] = 4950;
	g_aData[2] = 4900;
	g_aData[3] = 4800;
	g_aData[4] = 2000;
	g_aData[5] = 10;

	for (nCntRanking = 0; nCntRanking < NUM_PLASE; nCntRanking++)
	{
		g_aRanking[nCntRanking].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRanking[nCntRanking].bUse = false;
		g_nRanking = 0;

	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLASE * NUM_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < NUM_PLASE * NUM_RANKING; nCntRanking++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f	+ nCntRanking * 34.0f, 180.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(30.0f + nCntRanking * 34.0f, 180.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f	+ nCntRanking * 34.0f, 240.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(30.0f + nCntRanking * 34.0f, 240.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(255.0f, 255.0f, 255.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(255.0f, 255.0f, 255.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(255.0f, 255.0f, 255.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(255.0f, 255.0f, 255.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロック
	g_pVtxBuffRanking->Unlock();
}
//================================================
//ランキング終了処理
//=================================================
void UninitRankingScore(void)
{
	int nRanking;

	for (nRanking = 0; nRanking < NUM_RANKING; nRanking++)
	{
		if (g_pTextureRanking[nRanking] != NULL)
		{
			g_pTextureRanking[nRanking]->Release();
			g_pTextureRanking[nRanking] = NULL;
		}
	}
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}
//=========================================================
//ランキング更新処理
//=========================================================
void UpdateRankingScore(void)
{
	int g_nRankUpdate = -1;
	int Score;
	int nCount1;
	int nCount2;
	int nMax;
	bool bRankUpdate = false;
	Score = GetInitTime() - GetTime();

	FILE *pFile;	//ファイルポインタを宣言

	// ファイルを開く
	pFile = fopen("data\\ranking.bin", "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ファイルからデータ数を書き込む
		fread(&g_aData[0], sizeof(int), 5, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		g_aData[0] = 150;
		g_aData[1] = 180;
		g_aData[2] = 200;
		g_aData[3] = 220;
		g_aData[4] = 230;
		g_aData[5] = 250;
	}

	if (Score < g_aData[NUM_RANKING - 1])
	{//最も小さい値より挿入する値が大きかった時

	 //最も小さい値に挿入する値を代入する
		g_aData[NUM_RANKING - 1] = Score;

		g_nRankUpdate = NUM_RANKING - 1;
	}

	//ソートの処理
	for (nCount1 = 0; nCount1 < NUM_RANKING - 1; nCount1++)
	{
		//並び替えをする
		for (nCount2 = nCount1 + 1; nCount2 < NUM_RANKING; nCount2++)
		{
			if (g_aData[nCount1] > g_aData[nCount2])
			{//比較してる数が

			 //現在の比較している数を保存
				nMax = g_aData[nCount1];

				//最大値に置き換える
				g_aData[nCount1] = g_aData[nCount2];

				//変更した数値を元に戻す
				g_aData[nCount2] = nMax;
			}
		}
	}

	// ファイルを開く
	pFile = fopen("data\\ranking.bin", "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ファイルからデータ数を書き込む
		fwrite(&g_aData[0], sizeof(int), 5, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	
	SetRanking(D3DXVECTOR3(490.0f, 180.0f, 0.0f), g_aData[0]);
	SetRanking(D3DXVECTOR3(490.0f, 265.0f, 0.0f), g_aData[1]);
	SetRanking(D3DXVECTOR3(490.0f, 350.0f, 0.0f), g_aData[2]);
	SetRanking(D3DXVECTOR3(490.0f, 435.0f, 0.0f), g_aData[3]);
	SetRanking(D3DXVECTOR3(490.0f, 515.0f, 0.0f), g_aData[4]);
	SetRanking(D3DXVECTOR3(490.0f, 615.0f, 0.0f), Score);
}
//======================================================
//ランキング描画処理
//======================================================
void DrawRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntRanking;
	int nCnt;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < NUM_RANKING; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureRanking[nCnt]);

		for (nCntRanking = 0; nCntRanking < NUM_PLASE * NUM_RANKING; nCntRanking++)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCnt * NUM_PLASE + nCntRanking )*4, 2);
		}
	}
}
//==================================================
//ランキングセット処理
//==================================================
void SetRanking(D3DXVECTOR3 pos, int nRanking)
{
	int aTexU[NUM_PLASE];
	int nCntRanking;
	int nCntNumber;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < NUM_RANKING; nCntRanking++)
	{
		if (g_aRanking[nCntRanking].bUse == false)
		{
			g_nRanking = nRanking;

			aTexU[0] = g_nRanking % 1000 / 100;
			aTexU[1] = g_nRanking % 100 / 10;
			aTexU[2] = g_nRanking % 10 / 1;

			for (nCntNumber = 0; nCntNumber < NUM_PLASE; nCntNumber++, pVtx += 4)
			{
				g_aRanking[nCntNumber].pos = pos;

				pVtx[0].pos = D3DXVECTOR3(g_aRanking[nCntNumber].pos.x - 24.0f + (nCntNumber * 85.0f), g_aRanking[nCntNumber].pos.y - 35.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aRanking[nCntNumber].pos.x + 24.0f + (nCntNumber * 85.0f), g_aRanking[nCntNumber].pos.y - 35.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aRanking[nCntNumber].pos.x - 24.0f + (nCntNumber * 85.0f), g_aRanking[nCntNumber].pos.y + 35.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aRanking[nCntNumber].pos.x + 24.0f + (nCntNumber * 85.0f), g_aRanking[nCntNumber].pos.y + 35.0f, 0.0f);

				pVtx[0].tex = D3DXVECTOR2(0.01f + (aTexU[nCntNumber] * 0.1f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntNumber] * 0.1f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.01f + (aTexU[nCntNumber] * 0.1f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntNumber] * 0.1f), 1.0f);
			}

			g_aRanking[nCntRanking].bUse = true;

			break;
		}
		else
		{
			pVtx += 4 * NUM_PLASE;
		}
	}

	//頂点バッファをロック
	g_pVtxBuffRanking->Unlock();
}

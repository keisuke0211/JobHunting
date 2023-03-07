//==========================================
//
//ランキング表示プログラム[ranking.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"

//マクロ
#define RANK_CHARPART_NUM			(4)		//文字部分で使用するポリゴン数
#define RANK_NUMPART_NUM			(8)		//数字部分で使用するポリゴン数
#define RANK_1COLUMN_USEPOLY		(12)	//スコア桁数
#define RANK_SCORETEX_SIZE_WIDTH	(512)	//スコアテクスチャ幅
#define RANK_SCORETEX_SIZE_HEIGHT	(512)	//スコアテクスチャ高さ
#define RANK_SCORETEX_NUM_WIDTH		(8)		//スコアテクスチャパターン幅
#define RANK_SCORETEX_NUM_HEIGHT	(8)		//スコアテクスチャパターン高さ
#define RANK_FLASH_TIME				(20)	//点滅時間（フレーム）

//グローバル
LPDIRECT3DTEXTURE9 g_pTextureRanking;		//背景部テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffRanking;	//文字の頂点バッファポインタ（中心)
Ranking g_aRanking[MAX_RANK];				//ランキング構造体
int g_nNowScore;							//プレイ結果
int g_nRankUpdate = -1;						//ランキング更新箇所
int g_nTimerRanking;						//画面遷移カウンター
int g_nCounterFlash;						//点滅カウンター

//========================
//初期化処理
//========================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスのポインタ
	int nCntRankingNum, nCntRankingStr;			//カウンタ
	int aTexUV[RANK_1COLUMN_USEPOLY];			//文字格納
	const int nStrNumRank[MAX_RANK][RANK_CHARPART_NUM] = {
		{ 1 + SCORE_PATT_OFFSET ,PATT_NUM_S ,PATT_NUM_T ,PATT_NUM_SPACE },
		{ 2 + SCORE_PATT_OFFSET ,PATT_NUM_N ,PATT_NUM_D ,PATT_NUM_SPACE },
		{ 3 + SCORE_PATT_OFFSET ,PATT_NUM_R ,PATT_NUM_D ,PATT_NUM_SPACE },
		{ 4 + SCORE_PATT_OFFSET ,PATT_NUM_T ,PATT_NUM_H ,PATT_NUM_SPACE },
		{ 5 + SCORE_PATT_OFFSET ,PATT_NUM_T ,PATT_NUM_H ,PATT_NUM_SPACE }
	};
	const int nStrNumYou[RANK_CHARPART_NUM] = { PATT_NUM_Y,PATT_NUM_O,PATT_NUM_U,PATT_NUM_SPACE };

	D3DXVECTOR3 posNowScore;

	pDevice = GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\txt_font_001.PNG",
		&g_pTextureRanking);

	//頂点バッファの生成（スコア）
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RANK_1COLUMN_USEPOLY * (MAX_RANK + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffRanking,
		NULL);

	//位置設定
	posNowScore = D3DXVECTOR3(256.0f, 120.0f, 0.0f);
	g_aRanking[0].pos = D3DXVECTOR3(256.0f, 200.0f, 0.0f);
	g_aRanking[1].pos = D3DXVECTOR3(256.0f, 264.0f, 0.0f);
	g_aRanking[2].pos = D3DXVECTOR3(256.0f, 328.0f, 0.0f);
	g_aRanking[3].pos = D3DXVECTOR3(256.0f, 392.0f, 0.0f);
	g_aRanking[4].pos = D3DXVECTOR3(256.0f, 456.0f, 0.0f);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//順位頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	//プレイスコア
	//文字
	for (nCntRankingStr = 0; nCntRankingStr < RANK_CHARPART_NUM; nCntRankingStr++)
	{
		aTexUV[nCntRankingStr] = nStrNumYou[nCntRankingStr];
	}

	//数字
	for (nCntRankingStr = 0; nCntRankingStr < RANK_NUMPART_NUM; nCntRankingStr++)
	{
		aTexUV[nCntRankingStr + RANK_CHARPART_NUM] = g_nNowScore % (int)pow(10, (RANK_NUMPART_NUM - nCntRankingStr)) / (int)pow(10, RANK_NUMPART_NUM - 1 - nCntRankingStr) + SCORE_PATT_OFFSET;
	}

	//情報設定
	for (nCntRankingStr = 0; nCntRankingStr < RANK_1COLUMN_USEPOLY; nCntRankingStr++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(posNowScore.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * nCntRankingStr,
			posNowScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(posNowScore.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * (nCntRankingStr + 1),
			posNowScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(posNowScore.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * nCntRankingStr,
			posNowScore.y + RANK_SCORETEX_SIZE_HEIGHT / RANK_SCORETEX_NUM_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(posNowScore.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * (nCntRankingStr + 1),
			posNowScore.y + RANK_SCORETEX_SIZE_HEIGHT / RANK_SCORETEX_NUM_HEIGHT, 0.0f);

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_WIDTH,
			(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_HEIGHT);
		pVtx[1].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_WIDTH,
			(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_HEIGHT);
		pVtx[2].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_WIDTH,
			(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_WIDTH,
			(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_HEIGHT);
	}

	for (nCntRankingNum = 0; nCntRankingNum < MAX_RANK; nCntRankingNum++)
	{
		//位置設定
		//文字
		for (nCntRankingStr = 0; nCntRankingStr < RANK_CHARPART_NUM; nCntRankingStr++)
		{
			aTexUV[nCntRankingStr] = nStrNumRank[nCntRankingNum][nCntRankingStr];
		}

		//数字
		for (nCntRankingStr = 0; nCntRankingStr < RANK_NUMPART_NUM; nCntRankingStr++)
		{
			aTexUV[nCntRankingStr + RANK_CHARPART_NUM] = g_aRanking[nCntRankingNum].nScore % (int)pow(10, (RANK_NUMPART_NUM - nCntRankingStr)) / (int)pow(10, RANK_NUMPART_NUM - 1 - nCntRankingStr) + SCORE_PATT_OFFSET;
		}

		for (nCntRankingStr = 0; nCntRankingStr < RANK_1COLUMN_USEPOLY; nCntRankingStr++, pVtx += 4)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aRanking[nCntRankingNum].pos.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * nCntRankingStr,
				g_aRanking[nCntRankingNum].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRanking[nCntRankingNum].pos.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * (nCntRankingStr + 1),
				g_aRanking[nCntRankingNum].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRanking[nCntRankingNum].pos.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * nCntRankingStr,
				g_aRanking[nCntRankingNum].pos.y + RANK_SCORETEX_SIZE_HEIGHT / RANK_SCORETEX_NUM_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRanking[nCntRankingNum].pos.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * (nCntRankingStr + 1),
				g_aRanking[nCntRankingNum].pos.y + RANK_SCORETEX_SIZE_HEIGHT / RANK_SCORETEX_NUM_HEIGHT, 0.0f);

			//座標変換用係数設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			if (nCntRankingNum == g_nRankUpdate)
			{
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.94f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.94f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.94f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.94f, 0.0f, 1.0f);
			}
			else
			{
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_WIDTH,
				(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_HEIGHT);
			pVtx[1].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_WIDTH,
				(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_HEIGHT);
			pVtx[2].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_WIDTH,
				(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_HEIGHT);
			pVtx[3].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_WIDTH,
				(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_HEIGHT);
		}
		g_aRanking[nCntRankingNum].bDisp = true;
	}

	//頂点バッファをアンロック
	g_pVtxbuffRanking->Unlock();

	//点滅時間初期化
	g_nCounterFlash = 0;
}

//========================
//終了処理
//========================
void UninitRanking(void)
{
	//テクスチャの破棄
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxbuffRanking != NULL)
	{
		g_pVtxbuffRanking->Release();
		g_pVtxbuffRanking = NULL;
	}
}

//========================
//更新処理
//========================
void UpdateRanking(void)
{
	int nCntRank;		//カウンタ
	VERTEX_2D *pVtx;	//設定用ポインタ

	g_nCounterFlash++;

	//順位頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	if (g_nRankUpdate != -1 && g_nCounterFlash >= RANK_FLASH_TIME)
	{
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			if (nCntRank == g_nRankUpdate)
			{
				g_aRanking[nCntRank].bDisp = g_aRanking[nCntRank].bDisp ? false : true;
				g_nCounterFlash = 0;
			}
		}
	}

	//頂点バッファをアンロック
	g_pVtxbuffRanking->Unlock();
}

//========================
//描画処理
//========================
void DrawRanking(void)
{
	int nCntRankingNum, nCntRankingScore;			//カウンタ
	LPDIRECT3DDEVICE9 pDevice;						//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//順位頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffRanking, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//プレイ結果のスコア表示
	for (nCntRankingScore = 0; nCntRankingScore < RANK_1COLUMN_USEPOLY; nCntRankingScore++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTextureRanking);

		//描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRankingScore * 4, 2);
	}

	//ランキング表示
	for (nCntRankingNum = 0; nCntRankingNum < MAX_RANK; nCntRankingNum++)
	{
		if (g_aRanking[nCntRankingNum].bDisp)
		{
			for (nCntRankingScore = 0; nCntRankingScore < RANK_1COLUMN_USEPOLY; nCntRankingScore++)
			{
				//テクスチャ設定
				pDevice->SetTexture(0, g_pTextureRanking);

				//描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntRankingNum + 1) * RANK_1COLUMN_USEPOLY + nCntRankingScore) * 4, 2);
			}
		}
	}
}

//========================
//リセット処理
//========================
void ResetRanking(void)
{
	g_aRanking[0].nScore = 5000;
	g_aRanking[1].nScore = 3000;
	g_aRanking[2].nScore = 2000;
	g_aRanking[3].nScore = 1000;
	g_aRanking[4].nScore = 600;
}

//========================
//設定処理
//========================
void SetRanking(int nScore)
{
	int nCntRank;	//カウンタ
	int nTempScore;	//スコア仮置き場

	g_nNowScore = nScore;

	if (g_aRanking[MAX_RANK - 1].nScore < nScore)
	{//少なくとも5位以上に入る
		g_aRanking[MAX_RANK - 1].nScore = nScore;
		g_nRankUpdate = MAX_RANK - 1;

		//ソート
		for (nCntRank = MAX_RANK - 1; nCntRank > 0; nCntRank--)
		{
			if (g_aRanking[nCntRank].nScore > g_aRanking[nCntRank - 1].nScore)
			{
				nTempScore = g_aRanking[nCntRank - 1].nScore;
				g_aRanking[nCntRank - 1].nScore = g_aRanking[nCntRank].nScore;
				g_aRanking[nCntRank].nScore = nTempScore;
				g_nRankUpdate--;
			}
			else
			{
				break;
			}
		}
	}
}
//◇====================◇
//｜　　　　　　　　　　｜
//｜　 　背景の処理　　 ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "bg.h"

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_BG (3)      //背景の数

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBg = NULL; //頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureBg[NUM_BG] = {};//テクスチャ(3枚分)へのポインタ
float g_aTexV[NUM_BG];						 //テクスチャ座標の開始位置(V値)

int directionMode;
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【背景・処理】｜―――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇==============================◇
//｜　　　 背景の初期化処理　　 　｜
//◇==============================◇
void InitBg(void)
{
	int nCntBG;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Plains.png",
		&g_pTextureBg[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\cloud001.png",
		&g_pTextureBg[1]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\cloud002.png",
		&g_pTextureBg[2]);

	//テクスチャ座標の開始位置(V値)の初期化
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aTexV[nCntBG] = 0.0f;
	}
	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	directionMode = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBg,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBg->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		if (nCntBG == 0)
		{
			//　　　　　　　　　　　　 x       y     z
			pVtx[0].pos = D3DXVECTOR3(000.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1000.0f, 0.0f, 0.0f); //右回りで設定する
			pVtx[2].pos = D3DXVECTOR3(000.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1000.0f, 720.0f, 0.0f); //左回りで設定する
		}
		else if (nCntBG == 1)
		{
			pVtx[0].pos = D3DXVECTOR3(000.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1000.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(000.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1000.0f, 720.0f, 0.0f);
		}
		else if (nCntBG == 2)
		{
			pVtx[0].pos = D3DXVECTOR3(000.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1000.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(000.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1000.0f, 720.0f, 0.0f);
		}
		//◇――――――――――――◇
		//｜　　　　rhwの設定　　　 ｜
		//◇――――――――――――◇
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//◇――――――――――――◇
		//｜　　　 頂点カラー 　　　｜
		//◇――――――――――――◇
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇

		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG] + 1.0f);

		pVtx += 4; //頂点データのポインタを4つ分進める
	}
	//頂点バッファのアンロック
	g_pVtxbuffBg->Unlock();
}
//◇==============================◇
//｜　　  　背景の終了処理　　 　 ｜
//◇==============================◇
void UninitBg(void)
{
	int nCntBG;


	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		if (g_pTextureBg[nCntBG] != NULL)
		{
			g_pTextureBg[nCntBG]->Release();
			g_pTextureBg[nCntBG] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffBg != NULL)
	{
		g_pVtxbuffBg->Release();
		g_pVtxbuffBg = NULL;
	}
}
//◇==============================◇
//｜　　　  背景の更新処理　 　　 ｜
//◇==============================◇
void UpdateBg(void)
{
	int nCntBG;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBg->Lock(0, 0, (void **)&pVtx, 0);

		for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
		{
			//テクスチャ座標の開始位置(V値)の更新
			g_aTexV[0] += -0.0009f;
			g_aTexV[1] += -0.0005f;
			g_aTexV[2] += -0.0005f;

			//テクスチャ座標の更新
			pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG]);
			pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG]);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG] + 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG] + 1.0f);

			pVtx += 4;
		}
	//頂点バッファのアンロック
	g_pVtxbuffBg->Unlock();
}
//◇==============================◇
//｜　　 　 背景の描画処理 　　　 ｜
//◇==============================◇
void DrawBg(void)
{
	int nCntBG;

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffBg, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pDevice->SetTexture(0, g_pTextureBg[nCntBG]);
		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,        //プリミティブの種類
		//	2,                                               //プリミティブ(ポリゴン)数
		//	&g_aVertex[0],                                   //頂点情報の先頭アドレス
		//	sizeof(VERTEX_2D));                              //頂点情報構造体のサイズ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 3);
	}
}

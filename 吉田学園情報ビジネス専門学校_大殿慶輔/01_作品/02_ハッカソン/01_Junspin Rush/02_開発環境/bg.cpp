/*==========================================================================================================================

ゲーム背景の処理[bg.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "main.h"
#include "bg.h"
#include "input.h"

//マクロ定義
#define BG_HEIGHT			(930.0f)		//背景表示の高さ
#define BG_WIDTH			(2048.0f)		//背景表示の幅
#define BG_TEX_MOUNTAIN		"data/TEXTURE/bg_mountain_000.PNG"	

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureBg = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBg = NULL;	//頂点バッファへのポインタ
float g_BgTexV;										//背景スクロール用変数

//背景の初期化処理
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_TEX_MOUNTAIN, &g_pTextureBg);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL);

	float g_aTexV = 0.0f;		//背景スクロール用変数

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void* *)&pVtx, 0);
	
	//背景(山)の頂点情報の設定
	{
		//頂点座標の設定
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, BG_HEIGHT - 1024.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1024.0f * 5, BG_HEIGHT - 1024.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1024.0f * 5, BG_HEIGHT, 0.0f);
		}

		//rhwの設定
		{
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
		}

		//頂点カラーの設定
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//テクスチャの座標
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(5.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(5.0f, 1.0f);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffBg->Unlock();
}

//背景の終了処理
void UninitBg(void)
{
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//背景の更新処理
void UpdateBg(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void* *)&pVtx, 0);

	if (GetMode() == MODE_GAME_00)
	{// ゲームの時、
		//テクスチャ座標の開始位置の更新
		g_BgTexV += 0.0002f;
	}

	//テクスチャの座標
	pVtx[0].tex.x = g_BgTexV;
	pVtx[1].tex.x = g_BgTexV + 5.0f;
	pVtx[2].tex.x = g_BgTexV;
	pVtx[3].tex.x = g_BgTexV + 5.0f;

	//頂点バッファをロックする
	g_pVtxBuffBg->Unlock();
}

//背景の描画処理
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg);
	
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		
}
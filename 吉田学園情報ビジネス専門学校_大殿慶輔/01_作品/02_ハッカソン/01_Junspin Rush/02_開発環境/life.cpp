/*-----------------------------------------------------------------------------------------------------

ライフ処理[life.cpp]
Author:平澤詩苑

-------------------------------------------------------------------------------------------------------*/
#include "life.h"
#include "input.h"
#include "player.h"

#define MAX_HEART			(3)					//ライフの表示する数　= プレイヤーの最大ライフ
#define TEX_LIFE			"data/TEXTURE/ui_life_000.PNG"	//ライフの画像名
#define TEX_LIFE_WIDTH		(0.25f)				//テクスチャ座標の幅
#define HEART_WIDTH			(64.0f)				//ハートのサイズや、ハート同士の間隔

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureLife = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffLife = NULL;	//頂点バッファへのポインタ
int g_nLife = MAX_HEART;			//ライフ

//ライフの初期化処理
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_LIFE, &g_pTextureLife);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HEART, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLife, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_HEART; nCntLife++, pVtx += 4)
	{
		//頂点座標の設定
		{
			pVtx[0].pos = D3DXVECTOR3(20.0f + nCntLife * HEART_WIDTH, 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(84.0f + nCntLife * HEART_WIDTH, 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(20.0f + nCntLife * HEART_WIDTH, 94.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(84.0f + nCntLife * HEART_WIDTH, 94.0f, 0.0f);
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
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffLife->Unlock();
}

//ライフの終了処理
void UninitLife(void)
{
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//ライフの更新処理
void UpdateLife(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void* *)&pVtx, 0);

	//体力取得
	g_nLife = GetPlayer()->nLife;

	for (int nCntLife = 0; nCntLife < MAX_HEART; nCntLife++, pVtx += 4)
	{
		//カウンターより体力が多い
		if (nCntLife < g_nLife)
		{
			//新鮮なハートを表示
			{
				pVtx[0].tex.x = 0.0f;
				pVtx[1].tex.x = 0.5f;
				pVtx[2].tex.x = 0.0f;
				pVtx[3].tex.x = 0.5f;
			}
		}

		//枯渇しているハートを表示
		else
		{
			//頂点座標の設定
			{
				pVtx[0].tex.x = 0.5f;
				pVtx[1].tex.x = 1.0f;
				pVtx[2].tex.x = 0.5f;
				pVtx[3].tex.x = 1.0f;
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffLife->Unlock();
}

//ライフの描画処理
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntGround = 0; nCntGround < MAX_HEART; nCntGround++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGround * 4, 2);
	}
}
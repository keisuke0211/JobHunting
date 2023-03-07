/*==========================================================================================================================

地面処理[ground.cpp]
Author:平澤詩苑

==========================================================================================================================*/
#include "ground.h"

//マクロ定義
#define BG_TEX_GROUND		"data/TEXTURE/obj_soil_000.PNG"		//地面のテクスチャ
#define MAX_GROUND			(21)								//地面の数
#define GROUND_POS_Y		(696.0f)							//地面を配置する高さ
#define GROUND_LENGTH		(64.0f)								//地面の長さ
#define GROUND_WIDTH		(64.0f)								//隣同士の地面の中心の幅

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffGround = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureGround = NULL;	//テクスチャへのポインタ
float g_GroundMoveX;									//地面を横に動かす
D3DXVECTOR3 g_Groundpos;

//地面の初期化処理
void InitGround(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, BG_TEX_GROUND, &g_pTextureGround);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GROUND, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGround, NULL);

	//横に動かす変数を初期化
	g_GroundMoveX = 1.0f;

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGround->Lock(0, 0, (void* *)&pVtx, 0);

	//頂点座標の設定
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f,		SCREEN_HEIGHT - GROUND_LENGTH, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f,	SCREEN_HEIGHT - GROUND_LENGTH, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,		SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f,	SCREEN_HEIGHT, 0.0f);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f , 0.0f);
		pVtx[1].tex = D3DXVECTOR2(10.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f , 1.0f);
		pVtx[3].tex = D3DXVECTOR2(10.0f, 1.0f);
	}

	//頂点バッファをロックする
	g_pVtxBuffGround->Unlock();
}

//地面の終了処理
void UninitGround(void)
{
	if (g_pTextureGround != NULL)
	{
		g_pTextureGround->Release();
		g_pTextureGround = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGround != NULL)
	{
		g_pVtxBuffGround->Release();
		g_pVtxBuffGround = NULL;
	}
}

//地面の更新処理
void UpdateGround(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGround->Lock(0, 0, (void* *)&pVtx, 0);

	g_GroundMoveX += 0.02f;

	//頂点座標の設定
	if (GetMode() == MODE_GAME_00)
	{// ゲームの時、
		pVtx[0].tex.x = g_GroundMoveX;
		pVtx[1].tex.x = g_GroundMoveX + 10.0f;
		pVtx[2].tex.x = g_GroundMoveX;
		pVtx[3].tex.x = g_GroundMoveX + 10.0f;
	}

	//頂点バッファをロックする
	g_pVtxBuffGround->Unlock();
}

//地面の描画処理
void DrawGround(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGround, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGround);

	for (int nCntGround = 0; nCntGround < MAX_GROUND; nCntGround++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGround * 4, 2);
	}
}
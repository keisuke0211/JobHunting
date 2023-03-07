//========================================
// 
// OBJ:城[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_castle_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"
#include "sound.h"
#include "obj_castle_00.h"	// OBJ:城	[00]

//****************************************
// マクロ定義
//****************************************
// OBJ:城[00] のテクスチャの相対パス
#define OBJ_CASTLE_00_TEXTURE_PATH	"data\\TEXTURE\\BackGround\\castle.png"
#define OBJ_CASTLE_00_WIDTH			(240)
#define OBJ_CASTLE_00_HEIGHT		(240)
#define OBJ_CASTLE_00_POS			D3DXVECTOR3(160.0f,520.0f,0.0f)
#define OBJ_CASTLE_00_DAMAGE_TIME	(40)
#define OBJ_CASTLE_00_VIB_MAX		(20)

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:城[00] のパラメーター初期化処理
void InitParameterObj_castle_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_castle_00	// テクスチャへのポインタ
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObj_castle_00	// 頂点バッファへのポインタ
						= NULL;
Obj_castle_00		g_obj_castle_00;			// OBJ:城[00] の部品毎の情報

//========== *** OBJ:城[00] の情報を取得 ***
Obj_castle_00 *GetObj_castle_00(void) 
{
	return &g_obj_castle_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_castle_00関数 - OBJ:城[00] のパラメーター初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterObj_castle_00(void)
{
	Obj_castle_00	*pObj	// OBJ:城[00] の情報のポインタ
					= &g_obj_castle_00;

	pObj->nCounterDamage = 0;	// 到着カウンター
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitObj_castle_00関数 - OBJ:城[00] の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitObj_castle_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ
	Obj_castle_00	*pObj		// OBJ:城[00] の部品毎の情報のポインタ
						= &g_obj_castle_00;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, OBJ_CASTLE_00_TEXTURE_PATH, &g_aTextureObj_castle_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffObj_castle_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffObj_castle_00->Lock(0, 0, (void**)&pVtx, 0);

	// パラメーターの初期化処理
	InitParameterObj_castle_00();

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		OBJ_CASTLE_00_POS,
		INITD3DXVECTOR3,
		false,
		OBJ_CASTLE_00_WIDTH,
		OBJ_CASTLE_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, INITCOLOR);

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffObj_castle_00->Unlock();
}

//========================================
// UninitObj_castle_00関数 - OBJ:城[00] の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitObj_castle_00(void)
{
	// テクスチャの破棄
	if (g_aTextureObj_castle_00 != NULL)
	{
		g_aTextureObj_castle_00->Release();
		g_aTextureObj_castle_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffObj_castle_00 != NULL) 
	{
		g_pVtxBuffObj_castle_00->Release();
		g_pVtxBuffObj_castle_00 = NULL;
	}
}

//========================================
// UpdateObj_castle_00関数 - OBJ:城[00] の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateObj_castle_00(void)
{
	VERTEX_2D				*pVtx;	// 頂点情報へのポインタ
	Obj_castle_00		*pObj	// OBJ:城[00] の部品毎の情報のポインタ
							= &g_obj_castle_00;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffObj_castle_00->Lock(0, 0, (void**)&pVtx, 0);

	if (pObj->nCounterDamage > 0) 
	{
		pObj->nCounterDamage--;
	}

	float fDamageRate = (float)pObj->nCounterDamage / (float)OBJ_CASTLE_00_DAMAGE_TIME;

	// 頂点座標を設定
	{
		int nVib = (OBJ_CASTLE_00_VIB_MAX * fDamageRate) + 1;	// 振れ幅

		SetVertexPos2D(pVtx,
			OBJ_CASTLE_00_POS + D3DXVECTOR3((-rand() % nVib) + (rand() % nVib),0.0f,0.0f),
			INITD3DXVECTOR3,
			false,
			OBJ_CASTLE_00_WIDTH,
			OBJ_CASTLE_00_HEIGHT,
			ANGLE_TYPE_FIXED);
	}
	// 頂点カラーの設定
	{
		SetVertexColor2D(pVtx,
		{
			255,
			(int)(255 * (1.0f - fDamageRate)),
			(int)(255 * (1.0f - fDamageRate)),
			255,
		});
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffObj_castle_00->Unlock();
}

//========================================
// DrawObj_castle_00関数 - OBJ:城[00] の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawObj_castle_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffObj_castle_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureObj_castle_00);

	// OBJ:城[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetDamageObj_castle_00関数 - OBJ:城[00] の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetDamageObj_castle_00(void) 
{
	Obj_castle_00	*pObj	// OBJ:城[00] の部品毎の情報のポインタ
					= &g_obj_castle_00;

	pObj->nCounterDamage = OBJ_CASTLE_00_DAMAGE_TIME;
	PlaySound(6);
}
//========================================
// 
// フレーム[02] の処理
// Author:西村 吏功
// 
//========================================
//  *** ui_frame_02.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "ui_frame_02.h"

//****************************************
// マクロ定義
//****************************************

// フレーム[02] の出現にかかる時間
// フレーム[02] の消えるのにかかる時間
// フレーム[02] の透明度の最大値
#define UI_FRAME_02_IN_POP_TIME		(10)
#define UI_FRAME_02_IN_CLEAR_TIME	(10)
#define UI_FRAME_02_ALPHA_MAX		(1.0f)

//****************************************
// 列挙型の定義
//****************************************

// フレーム[02] の部品番号
typedef enum
{
	UI_FRAME_02_PARTS_FRAME = 0,	// フレーム
	UI_FRAME_02_PARTS_MAX,
}UI_FRAME_02_PARTS;

//****************************************
// 構造体の定義
//****************************************

// フレーム[02] のポリゴン毎の情報構造体
typedef struct
{
	char			aTexturePath	// テクスチャファイルの相対パス
					[TXT_MAX];		// 
	int				nPtnMaxX;		// パターンの上限(横)
	int				nPtnMaxY;		// パターンの上限(縦)
	int				nPtnMin;		// パターンNo.の下限
	int				nPtnMax;		// パターンNo.の上限
	int				nAnimTime;		// アニメーションにかかる時間
	float			fWidth;			// 幅
	float			fHeight;		// 高さ
	int				nPolygonNum;	// 必要ポリゴン数
	D3DXVECTOR3		relativePos;	// 相対位置

	ScaleDiameter	scale;			// 大きさにかかる倍率
	int				nPtn;			// パターンNo.
	int				nCounterAnim;	// アニメーションのカウンター
}Ui_frame_02Parts;

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterUi_frame_02(void);

// 部品(フレーム)の更新処理
void UpdateUi_frame_02Parts_keyboard(void);

// 状態処理
void StateProcessUi_frame_02(void);

// 必要ポリゴン数を調べる
int CheckPolygonNumUi_frame_02(int nParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_frame_02	// テクスチャへのポインタ
						[UI_FRAME_02_PARTS_MAX]	//
						= {};					//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_frame_02	// 頂点バッファへのポインタ
						= NULL;					//
Ui_frame_02				g_ui_frame_02;			// フレーム[02] の情報

// フレーム[02] の部品毎の情報
Ui_frame_02Parts g_aUi_frame_02Parts[UI_FRAME_02_PARTS_MAX] =
{
	{ "data\\TEXTURE\\Jumspin Rush タイトル画面.png"   , 1 , 1 , 0 , 0 , -1 , PIXEL * 1555  * 0.2f, PIXEL * 1066 * 0.2f, 1 , D3DXVECTOR3(SCREEN_WIDTH * 0.5f, PIXEL * 60, 0.0f) },
};

//========== *** フレーム[02] 情報を取得 ***
Ui_frame_02 *GetUi_frame_02(void) 
{
	return &g_ui_frame_02;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateUi_frame_02Parts_keyboard関数 - 部品(フレーム)の更新処理 -
//========================================
void UpdateUi_frame_02Parts_keyboard(void)
{
	Ui_frame_02Parts	*pParts						// 
						= &g_aUi_frame_02Parts		//
						[UI_FRAME_02_PARTS_FRAME];	// 部品の情報のポインタ
	VERTEX_2D			*pVtx;						// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_frame_02->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報へのポインタをその部品の開始点までずらす
	pVtx += (4 * CheckPolygonNumUi_frame_02(UI_FRAME_02_PARTS_FRAME));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// 頂点座標を設定
		SetVertexPosition(pVtx,
			g_ui_frame_02.pos + g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].fWidth
			* g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale.fWidth,
			g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].fHeight
			* g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale.fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtn,
				g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtnMin,
				g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtnMax,
				&g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nCounterAnim,
				g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtnMaxX,
			g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtnMaxY,
			NULL);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_frame_02.fAlpha) });
	}
}

//========================================
// StateProcess関数 - 状態処理 -
//========================================
void StateProcessUi_frame_02(void)
{
	Ui_frame_02	*pUi				// 
				= &g_ui_frame_02;	// フレーム[02] の情報のポインタ 

	switch (pUi->state)
	{
		//========== *** 出現 ***
	case UI_FRAME_02_STATE_POP:
	{

	}
		break;
		//========== *** 出現中 ***
	case UI_FRAME_02_STATE_IN_POP:
	{
		// 状態カウンターを加算
		pUi->nCounterState++;

		// 透明度と高さの倍率を更新
		pUi->fAlpha =
		g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale.fHeight =
			UI_FRAME_02_ALPHA_MAX * ((float)pUi->nCounterState / (float)UI_FRAME_02_IN_POP_TIME);

		if (pUi->nCounterState >= UI_FRAME_02_IN_POP_TIME)
		{// 状態カウンターが出現にかかる時間に達した時、
			// 出現状態にする
			SetStateUi_frame_02(UI_FRAME_02_STATE_POP);

			// 透明度を更新
			pUi->fAlpha = UI_FRAME_02_ALPHA_MAX;
		}
	}
		break;
		//========== *** 消えている ***
	case UI_FRAME_02_STATE_CLEAR:
	{

	}
		break;
		//========== *** 消え中 ***
	case UI_FRAME_02_STATE_IN_CLEAR:
	{
		// 状態カウンターを加算
		pUi->nCounterState++;

		// 透明度を更新
		pUi->fAlpha = 
		g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale.fHeight = 
			UI_FRAME_02_ALPHA_MAX * (1.0f - ((float)pUi->nCounterState / (float)UI_FRAME_02_IN_CLEAR_TIME));

		if (pUi->nCounterState >= UI_FRAME_02_IN_CLEAR_TIME)
		{// 状態カウンターが消えるのにかかる時間に達した時、
			// 消えている状態にする
			SetStateUi_frame_02(UI_FRAME_02_STATE_CLEAR);

			// ゲームの状態を通常に設定
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}
	}
		break;
	}
}

//========================================
// CheckPolygonNum関数 - 必要ポリゴン数を調べる -
//========================================
int CheckPolygonNumUi_frame_02(int nParts) 
{
	int nPolygonNum	// 
		= 0;		// 必要ポリゴン数

	// ポリゴン毎の必要ポリゴン数を加算していく
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_frame_02Parts[nCntPolygon].nPolygonNum;
	}

	// 必要ポリゴン数を返す
	return nPolygonNum;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_frame_02関数 - パラメーターの初期化処理 -
//========================================
void InitParameterUi_frame_02(void)
{
	g_ui_frame_02.state			= UI_FRAME_02_STATE_CLEAR;	// 状態
	g_ui_frame_02.nCounterState	= 0;						// 状態カウンター
	g_ui_frame_02.fAlpha		= 0.0f;						// 全体の透明度

	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++) 
	{
		g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale = { 1.0f,1.0f };	// 大きさの倍率
	}
}

//========================================
// InitUi_frame_02関数 - フレーム[02] の初期化処理 -
//========================================
void InitUi_frame_02(void)
{
	// パラメーターの初期化処理
	InitParameterUi_frame_02();

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_frame_02(UI_FRAME_02_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_frame_02,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_frame_02->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aUi_frame_02Parts[nCntParts].aTexturePath, &g_aTextureUi_frame_02[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_frame_02Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// 頂点座標を設定
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_frame_02Parts[nCntParts].fWidth,
				g_aUi_frame_02Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhwの設定
			SetRHW(pVtx);

			// 頂点カラーの設定
			SetVertexColor(pVtx, { 255,255,255,255 });

			// テクスチャ座標の設定
			SetTexturePosition(pVtx, 
				0, 
				g_aUi_frame_02Parts[nCntParts].nPtnMaxX, 
				g_aUi_frame_02Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffUi_frame_02->Unlock();
}

//========================================
// UninitUi_frame_02関数 - フレーム[02] の終了処理 -
//========================================
void UninitUi_frame_02(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_frame_02[nCntParts] != NULL)
		{
			g_aTextureUi_frame_02[nCntParts]->Release();
			g_aTextureUi_frame_02[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_frame_02 != NULL) 
	{
		g_pVtxBuffUi_frame_02->Release();
		g_pVtxBuffUi_frame_02 = NULL;
	}
}

//========================================
// UpdateUi_frame_02関数 - フレーム[02] の更新処理 -
//========================================
void UpdateUi_frame_02(void) 
{
	Ui_frame_02	*pUi				//
				= &g_ui_frame_02;	// フレーム[02] の情報のポインタ

	// 状態処理
	StateProcessUi_frame_02();

	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++) 
	{
		// 部品番号に応じた更新処理
		switch (nCntParts)
		{
			//========== *** フレームの説明 ***
		case UI_FRAME_02_PARTS_FRAME:
		{
			// 部品(フレームの説明)の更新処理
			UpdateUi_frame_02Parts_keyboard();
		}
			break;
		}
	}
}

//========================================
// DrawUi_frame_02関数 - フレーム[02] の描画処理 -
//========================================
void DrawUi_frame_02(void)
{
	Ui_frame_02	*pUi				//
				= &g_ui_frame_02;	// フレーム[02] の情報のポインタ

	if (pUi->state == UI_FRAME_02_STATE_CLEAR)
	{// 消えている状態の時、
		// 処理を終了する
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_frame_02, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// ポリゴン番号

	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureUi_frame_02[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_frame_02Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// フレーム[02] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState関数 - フレーム[02] の状態設定処理 -
//========================================
void SetStateUi_frame_02(UI_FRAME_02_STATE state) 
{
	Ui_frame_02	*pUi				//
				= &g_ui_frame_02;	// フレーム[02] の情報のポインタ 

	switch (state)
	{
		//========== *** 出現 ***
	case UI_FRAME_02_STATE_POP:
	{

	}
		break;
		//========== *** 出現中 ***
	case UI_FRAME_02_STATE_IN_POP:
	{

	}
		break;
		//========== *** 消えている ***
	case UI_FRAME_02_STATE_CLEAR:
	{

	}
		break;
		//========== *** 消え中 ***
	case UI_FRAME_02_STATE_IN_CLEAR:
	{

	}
		break;
	}

	// 状態を代入
	pUi->state = state;

	// 状態カウンターを初期化
	pUi->nCounterState = 0;
}

//========================================
// SetUi_frame_02関数 - フレーム[02] の設定処理 -
//========================================
void SetUi_frame_02(D3DXVECTOR3 pos)
{
	Ui_frame_02	*pUi				//
				= &g_ui_frame_02;	// フレーム[02] の情報のポインタ 

	if (pUi->state != UI_FRAME_02_STATE_CLEAR) 
	{// 消えている状態でない時、
		// 処理を終了する
		return;
	}

	// 位置を代入
	pUi->pos = pos;

	// 出現中の状態にする
	SetStateUi_frame_02(UI_FRAME_02_STATE_IN_POP);
}
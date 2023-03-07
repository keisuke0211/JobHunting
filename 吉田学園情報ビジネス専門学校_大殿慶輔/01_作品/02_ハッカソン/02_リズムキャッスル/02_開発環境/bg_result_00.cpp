//========================================
// 
// BG:リザルト[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** bg_result_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "physics.h"
#include "polygon2D.h"
#include "bg_result_00.h"	// BG:リザルト	[00]

//****************************************
// マクロ定義
//****************************************
// BG:リザルト[00] のデータファイルの相対パス
#define BG_RESULT_00_DATA_FILE_PATH	"data\\GAMEDATA\\BACKGROUND\\BG_RESULT_00_DATA.txt"

// BG:リザルト[00] の部品の最大数
#define BG_RESULT_00_PARTS_MAX	(4)

//****************************************
// 構造体の定義
//****************************************
// BG:リザルト[00] の管理情報
typedef struct 
{
	int			nPartNum;	// 部品数
}Bg_result_00Control;

// BG:リザルト[00] の部品毎の情報
typedef struct
{
	char		aTexturePath	// テクスチャの相対パス
				[TXT_MAX];
	D3DXVECTOR3	move;			// 移動量
	Scale		cutScale;		// 切り取り倍率
	D3DXVECTOR3	pos;			// 位置
}Bg_result_00Parts;

//****************************************
// プロトタイプ宣言
//****************************************
// BG:リザルト[00] のパラメーター初期化処理
void InitParameterBg_result_00(Bg_result_00Parts *pParts);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureBg_result_00	// テクスチャへのポインタ
						[BG_RESULT_00_PARTS_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_result_00	// 頂点バッファへのポインタ
						= NULL;
Bg_result_00Control		g_bg_result_00Control;	// BG:リザルト[00] の管理情報
Bg_result_00Parts		g_aBg_result_00Parts		// BG:リザルト[00] の部品毎の情報
						[BG_RESULT_00_PARTS_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_result_00関数 - BG:リザルト[00] のパラメーター初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterBg_result_00(Bg_result_00Parts *pParts)
{
	pParts->pos = INITD3DXVECTOR3;	// 位置
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadBg_result_00関数 - BG:リザルト[00] の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadBg_result_00(void) 
{
	FILE				*pFile;		// ファイルポインタ
	char				aDataSearch	// データ検索用
						[TXT_MAX];
	Bg_result_00Control	*pCtl		// BG:リザルト[00] の管理情報のポインタ
						= &g_bg_result_00Control;
	Bg_result_00Parts	*pParts		// BG:リザルト[00] の部品毎の情報のポインタ
						= g_aBg_result_00Parts;

	// 部品数を初期化
	pCtl->nPartNum = 0;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(BG_RESULT_00_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if		(!strcmp(aDataSearch, "END"))		{ fclose(pFile); break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "PARTSSET"))
		{// 部品の設定情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// 検索

				if		(!strcmp(aDataSearch, "PARTSSET_END"))	{ break; }	// 読み込みを終了
				else if (!strcmp(aDataSearch, "PARTS"))
				{// 部品毎の情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	// 検索

						if (!strcmp(aDataSearch, "PARTS_END")) {
							pParts++;			// ポインタを進める
							pCtl->nPartNum++;	// 部品数を加算
							break;
						}
						else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))	{ fscanf(pFile, "%s", &pParts->aTexturePath); }	// ファイル名
						else if (!strcmp(aDataSearch, "CUT_SCALE:"))	{ LoadScale(pFile, &pParts->cutScale); }		// 切り取り倍率
						else if (!strcmp(aDataSearch, "MOVE:"))			{ LoadVector(pFile, &pParts->move); }			// 移動量
					}
				}
			}
		}
	}
}

//========================================
// InitBg_result_00関数 - BG:リザルト[00] の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitBg_result_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ
	Bg_result_00Control	*pCtl		// BG:リザルト[00] の管理情報のポインタ
						= &g_bg_result_00Control;
	Bg_result_00Parts	*pParts		// BG:リザルト[00] の部品毎の情報のポインタ
						= g_aBg_result_00Parts;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntParts = 0; nCntParts < pCtl->nPartNum; nCntParts++)
	{
		D3DXCreateTextureFromFile(pDevice, pParts[nCntParts].aTexturePath, &g_aTextureBg_result_00[nCntParts]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * pCtl->nPartNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_result_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_result_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < pCtl->nPartNum; nCntParts++, pVtx += 4)
	{
		// パラメーターの初期化処理
		InitParameterBg_result_00(pParts);

		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			ANGLE_TYPE_FIXED);

		// rhwの設定
		SetRHW2D(pVtx);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, INITCOLOR);

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx, 0, 1, 1, NULL);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBg_result_00->Unlock();
}

//========================================
// UninitBg_result_00関数 - BG:リザルト[00] の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitBg_result_00(void)
{
	// テクスチャの破棄
	for (int nCntParts = 0; nCntParts < BG_RESULT_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureBg_result_00[nCntParts] != NULL)
		{
			g_aTextureBg_result_00[nCntParts]->Release();
			g_aTextureBg_result_00[nCntParts] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBg_result_00 != NULL) 
	{
		g_pVtxBuffBg_result_00->Release();
		g_pVtxBuffBg_result_00 = NULL;
	}
}

//========================================
// UpdateBg_result_00関数 - BG:リザルト[00] の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateBg_result_00(void)
{
	VERTEX_2D			*pVtx;	// 頂点情報へのポインタ
	Bg_result_00Control	*pCtl	// BG:リザルト[00] の管理情報のポインタ
						= &g_bg_result_00Control;
	Bg_result_00Parts	*pParts	// BG:リザルト[00] の部品毎の情報のポインタ
						= g_aBg_result_00Parts;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg_result_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < pCtl->nPartNum; nCntParts++, pVtx += 4, pParts++)
	{
		// テクスチャ座標の設定
		SetTexturePos2DCut(pVtx, pParts->cutScale, pParts->pos);

		// 位置を更新
		pParts->pos += pParts->move;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBg_result_00->Unlock();
}

//========================================
// DrawBg_result_00関数 - BG:リザルト[00] の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawBg_result_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	Bg_result_00Control	*pCtl		// BG:リザルト[00] の管理情報のポインタ
						= &g_bg_result_00Control;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg_result_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < pCtl->nPartNum; nCntParts++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureBg_result_00[nCntParts]);

		// BG:リザルト[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParts, 2);
	}
}
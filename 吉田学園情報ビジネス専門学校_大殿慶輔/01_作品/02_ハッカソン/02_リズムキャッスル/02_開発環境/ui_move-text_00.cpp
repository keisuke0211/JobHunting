//========================================
// 
// UI:移動テキスト[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_move-text_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_move-text_00.h"	// UI:移動テキスト	[00]

//****************************************
// マクロ定義
//****************************************
// UI:移動テキスト[00] の最大数
#define UI_MOVETEXT_00_MAX	(64)

//****************************************
// 構造体の定義
//****************************************
typedef struct 
{
	bool bUse;
	char aString[TXT_MAX];
	int nFont;
	Color col;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	int nLife;
	int nLifeMax;
}Ui_moveText_00;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:移動テキスト[00] のパラメーター初期化処理
void InitParameterUi_moveText_00(Ui_moveText_00 *pUi);

//****************************************
// グローバル宣言
//****************************************
Ui_moveText_00	g_aUi_moveText_00
				[UI_MOVETEXT_00_MAX];	// UI:移動テキスト[00] の部品毎の情報

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_moveText_00関数 - UI:移動テキスト[00] のパラメーター初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterUi_moveText_00(Ui_moveText_00 *pUi)
{
	pUi->bUse = false;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_moveText_00関数 - UI:移動テキスト[00] の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitUi_moveText_00(void)
{
	Ui_moveText_00		*pUi		// UI:移動テキスト[00] の部品毎の情報のポインタ
						= g_aUi_moveText_00;

	for (int nCntUi = 0; nCntUi < UI_MOVETEXT_00_MAX; nCntUi++, pUi++)
	{
		// パラメーターの初期化処理
		InitParameterUi_moveText_00(pUi);
	}
}

//========================================
// UninitUi_moveText_00関数 - UI:移動テキスト[00] の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitUi_moveText_00(void)
{
	
}

//========================================
// UpdateUi_moveText_00関数 - UI:移動テキスト[00] の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateUi_moveText_00(void)
{
	Ui_moveText_00	*pUi	// UI:移動テキスト[00] の部品毎の情報のポインタ
					= g_aUi_moveText_00;

	for (int nCntUi = 0; nCntUi < UI_MOVETEXT_00_MAX; nCntUi++, pUi++)
	{
		if (!pUi->bUse) 
		{
			continue;
		}
		else if (--pUi->nLife <= 0) 
		{
			pUi->bUse = false;
			continue;
		}

		pUi->pos += pUi->move*((float)pUi->nLife / (float)pUi->nLifeMax);

		Color setCol = pUi->col;
		setCol.a *= ((float)pUi->nLife / (float)pUi->nLifeMax);

		// テキスト(2D)の設定処理
		SetText2D(
			pUi->aString,
			pUi->nFont,
			DISPLAY_CENTER,
			pUi->pos,
			setCol,
			1.0f, 1.0f);
	}
}

//========================================
// SetUi_moveText_00関数 - UI:移動テキスト[00] の設定処理 -
//========================================
void SetUi_moveText_00(char *pString, int nFont, D3DXVECTOR3 pos, D3DXVECTOR3 move, Color col, int nLife)
{
	Ui_moveText_00	*pUi	// UI:移動テキスト[00] の部品毎の情報のポインタ
		= g_aUi_moveText_00;

	for (int nCntUi = 0; nCntUi < UI_MOVETEXT_00_MAX; nCntUi++, pUi++)
	{
		if (pUi->bUse) 
		{
			continue;
		}

		sprintf(pUi->aString, pString);
		pUi->bUse = true;
		pUi->col = col;
		pUi->pos = pos;
		pUi->move = move;
		pUi->nFont = nFont;
		pUi->nLife =
		pUi->nLifeMax = nLife;

		break;
	}
}
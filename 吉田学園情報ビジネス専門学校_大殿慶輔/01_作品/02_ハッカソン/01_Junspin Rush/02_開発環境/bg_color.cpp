//========================================
// 
// 背景色の処理
// Author:西村 吏功
// 
//========================================
//  *** bg_color.cpp ***
//========================================
#include "main.h"
#include "bg_color.h"

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameterBg_color(void);

//****************************************
// グローバル宣言
//****************************************
Bg_color g_bg_color;	// 背景色の情報

//========== *** 背景色の情報を取得 ***
Bg_color *GetBg_color(void) 
{
	return &g_bg_color;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_color - パラメーターの初期化処理 -
//========================================
void InitParameterBg_color(void) 
{
	g_bg_color.col					= BG_START_COLOR;	// 色
	g_bg_color.oldColor				= { 0, 0, 0, 0 };	// 古い色
	g_bg_color.newColor				= { 0, 0, 0, 0 };	// 新しい色
	g_bg_color.bChangeColor			= false;			// 変色フラグ
	g_bg_color.nCounterChangeColor	= 0;				// 変色カウンター
	g_bg_color.nTimeChangeColor		= 0;				// 変色にかかる時間
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitBg_color関数 - 背景色の初期化処理 -
//========================================
void InitBg_color(void)
{
	// パラメーターの初期化処理
	InitParameterBg_color();
}

//========================================
// UninitBg_color関数 - 背景色の終了処理 -
//========================================
void UninitBg_color(void)
{
	
}

//========================================
// UpdateBg_color関数 - 背景色の更新処理 -
//========================================
void UpdateBg_color(void)
{
	Bg_color	*pBg			// 
				= &g_bg_color;	// 背景色の情報

	if (pBg->bChangeColor) 
	{// 変色フラグが真の時、
		// 変色カウンターを加算
		pBg->nCounterChangeColor++;

		// 古い色の成分を変色カウンターに応じた割合分設定
		pBg->col.nR = pBg->oldColor.nR * (1.0f - ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor));
		pBg->col.nG = pBg->oldColor.nG * (1.0f - ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor));
		pBg->col.nB = pBg->oldColor.nB * (1.0f - ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor));

		// 新しい色の成分を変色カウンターに応じた割合分加算
		pBg->col.nR += pBg->newColor.nR * ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor);
		pBg->col.nG += pBg->newColor.nG * ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor);
		pBg->col.nB += pBg->newColor.nB * ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor);

		if (pBg->nCounterChangeColor >= pBg->nTimeChangeColor) 
		{// 変色カウンターが変色にかかる時間に達した時、
			// 変色フラグを偽にする
			pBg->bChangeColor = false;

			// 色を新しい色に変える
			pBg->col = pBg->newColor;
		}
	}
}

//========================================
// SetBg_color関数 - 背景色の設定処理 -
//========================================
void SetBg_color(Color newColor, int nTime)
{
	Bg_color	*pBg			// 
				= &g_bg_color;	// 背景色の情報

	if (nTime > 0) 
	{// 変色にかかる時間が0を上回っていた時、
		// 古い色と新しい色を設定
		pBg->oldColor = pBg->col;
		pBg->newColor = newColor;

		// 変色カウンターを初期化
		pBg->nCounterChangeColor = 0;

		// 変色にかかる時間を代入
		pBg->nTimeChangeColor = nTime;

		// 変色フラグを真にする
		pBg->bChangeColor = true;
	}
	else 
	{// 変色にかかる時間が0以下であった時、
		// 色を新しい色に変える
		pBg->col = newColor;
	}
}
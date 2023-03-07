//========================================
// 
// テキストのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** text.h ***
//========================================
#ifndef _TEXT_H_	// このマクロ定義がされていなかった時
#define _TEXT_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// マクロ定義
//****************************************
// テキスト(2D)の最大数
#define TEXT2D_MAX	(128)

// テキスト(3D)の最大数
#define TEXT3D_MAX	(128)

// 表示形式
typedef enum
{
	DISPLAY_CENTER,	// 中央揃え
	DISPLAY_LEFT,	// 左詰め
	DISPLAY_RIGHT,	// 右詰め
}DISPLAY;

//****************************************
// プロトタイプ宣言
//****************************************
// フォントの読み込み処理
void LoadFont(void);

// テキストの初期化処理
// テキストの終了処理
// テキストの描画処理
void InitText(void);
void UninitText(void);
void DrawText_(void);

// テキスト(2D)の設定処理
void SetText2D(char *pString, int nFont, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight);

// テキスト(3D)の設定処理
void SetText3D(char *pString, int nFont, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard);

#endif
//========================================
// 
// 色の処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** color.h ***
//========================================
#ifndef _COLOR_H_	// このマクロ定義がされていなかった時
#define _COLOR_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include <stdio.h>

//****************************************
// プロトタイプ宣言
//****************************************
// 色の読み込み処理
void LoadColor(FILE *pFile, Color *pCol);

// 色に明るさを反映
Color BrightnessToColor(Color col, float fBrightness);

// Color形式の色をD3DXCOLOR形式に変換
// Color形式の色をD3DCOLOR形式に変換
D3DXCOLOR ColorToD3DXCOLOR(Color col);
D3DCOLOR ColorToD3DCOLOR(Color col);

// Color形式の色の値を割合で返す
// Color形式の色を加算
// Color形式の色を混ぜる
Color ColorRate(Color col, float fRate);
Color ColorAdd(Color col1, Color col2);
Color ColorMix(Color col1, Color col2);

#endif
//========================================
// 
// 色の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** color.cpp ***
//========================================
#include "main.h"
#include "color.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadColor関数 - 色の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadColor(FILE *pFile, Color *pCol)
{
	fscanf(pFile, "%d", &pCol->r);
	fscanf(pFile, "%d", &pCol->g);
	fscanf(pFile, "%d", &pCol->b);
	fscanf(pFile, "%d", &pCol->a);
}

//========================================
// BrightnessToColor関数 - 色に明るさを反映 -
// Author:RIKU NISHIMURA
//========================================
Color BrightnessToColor(Color col, float fBrightness) 
{
	col.r *= fBrightness;
	col.g *= fBrightness;
	col.b *= fBrightness;

	return col;
}

//========================================
// ColorToD3DXCOLOR関数 - Color形式の色をD3DXCOLOR形式に変換 -
// Author:RIKU NISHIMURA
//========================================
D3DXCOLOR ColorToD3DXCOLOR(Color col)
{
	return D3DXCOLOR(
		(float)col.r / (float)255,
		(float)col.g / (float)255,
		(float)col.b / (float)255,
		(float)col.a / (float)255
		);
}

//========================================
// ColorToD3DCOLOR関数 - Color形式の色をD3DCOLOR形式に変換 -
// Author:RIKU NISHIMURA
//========================================
D3DCOLOR ColorToD3DCOLOR(Color col) 
{
	return D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// ColorRate関数 - Color形式の色の値を割合で返す -
// Author:RIKU NISHIMURA
//========================================
Color ColorRate(Color col, float fRate) 
{
	col.r *= fRate;
	col.g *= fRate;
	col.b *= fRate;
	col.a *= fRate;
	return col;
}

//========================================
// ColorAdd関数 - Color形式の色を加算 -
// Author:RIKU NISHIMURA
//========================================
Color ColorAdd(Color col1, Color col2) 
{
	col1.r += col2.r;
	col1.g += col2.g;
	col1.b += col2.b;
	col1.a += col2.a;
	return col1;
}

//========================================
// ColorMix関数 - Color形式の色を混ぜる -
// Author:RIKU NISHIMURA
//========================================
Color ColorMix(Color col1, Color col2)
{
	col1.r *= ((float)col2.r / (float)255);
	col1.g *= ((float)col2.g / (float)255);
	col1.b *= ((float)col2.b / (float)255);
	col1.a *= ((float)col2.a / (float)255);
	return col1;
}
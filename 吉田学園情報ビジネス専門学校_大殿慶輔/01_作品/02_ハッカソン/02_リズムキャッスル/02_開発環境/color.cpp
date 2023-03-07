//========================================
// 
// �F�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** color.cpp ***
//========================================
#include "main.h"
#include "color.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadColor�֐� - �F�̓ǂݍ��ݏ��� -
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
// BrightnessToColor�֐� - �F�ɖ��邳�𔽉f -
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
// ColorToD3DXCOLOR�֐� - Color�`���̐F��D3DXCOLOR�`���ɕϊ� -
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
// ColorToD3DCOLOR�֐� - Color�`���̐F��D3DCOLOR�`���ɕϊ� -
// Author:RIKU NISHIMURA
//========================================
D3DCOLOR ColorToD3DCOLOR(Color col) 
{
	return D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// ColorRate�֐� - Color�`���̐F�̒l�������ŕԂ� -
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
// ColorAdd�֐� - Color�`���̐F�����Z -
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
// ColorMix�֐� - Color�`���̐F�������� -
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
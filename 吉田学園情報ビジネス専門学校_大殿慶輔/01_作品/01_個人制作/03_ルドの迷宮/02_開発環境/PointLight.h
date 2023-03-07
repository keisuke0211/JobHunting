//****************************************
//	PointLight.h
//	Author:大殿慶輔
//****************************************
#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include"main.h"
//****************************************
// 構造体の定義
//****************************************
// 光の情報
typedef struct
{
	// 位置関連
	D3DXVECTOR3	rot;	// 向き
	D3DXCOLOR	col;	// 色
}PointLight;
//****************************************
// マクロ定義
//****************************************
#define NUM_POINTLIGHT			(1)		//ライトの数
//****************************************
//プロトタイプ宣言
//****************************************
void InitPointLight(void);
void UninitPointLight(void);
void UpdatePointLight(void);
#endif
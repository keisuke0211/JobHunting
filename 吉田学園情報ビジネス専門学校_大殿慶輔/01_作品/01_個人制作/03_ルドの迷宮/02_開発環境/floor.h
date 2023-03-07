//****************************************
//	floar.h
//	Author:大殿慶輔
//****************************************
#ifndef _FLOOR_H_
#define _FLOOR_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define MAX_FLOOR (100)			//床の最大数
//****************************************
// 構造体の定義
//****************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3	vecLine;	//境界線ベクトル
	D3DXVECTOR3	vecToPos;	//境界線ベクトル

	int nCype;				//計算方法
	int nType;				//画像種類
	int nSplitX;			//横の分割数
	int nSplitY;			//縦の分割数

	float fWidth;			//横幅
	float fHight;			//高さ
	bool bUse;				//使用しているかどうか
}Floor;
//****************************************
// プロトタイプ宣言
//****************************************
void InitFloor(void);
void UninitFloor(void);
void UpdateFloor(void);
void DrawFloor(void);
void LoadFloor(void);
Floor *GetFloor(void);

#endif
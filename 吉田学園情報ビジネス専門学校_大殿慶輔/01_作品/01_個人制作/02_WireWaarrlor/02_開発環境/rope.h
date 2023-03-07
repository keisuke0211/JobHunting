//◇====================◇
//｜　　　　　　　　　　｜
//｜　rope.h            ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _ROPE_H_  //このマクロ定義がされてなかったら
#define _ROPE_H_  //２重インクルード防止のマクロを定義する

#include"main.h"
//****************************************
// ルアーの構造体の定義
//****************************************

typedef struct
{
	D3DXVECTOR3 posA;	//現在の位置
	D3DXVECTOR3 posB;	//現在の位置
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;    //向き

	float Length;
	float Angle;

	bool bUse;			//使用しているかどうか
}Rope;
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇

void InitRope(void);
void UninitRope(void);
void UpdateRope(void);
void DrawRope(void);
Rope *GetRope(void);

//****************************************
// マクロ定義
//****************************************
#define SIZE_ROPE_WIDTH  (5.0f)
#define SIZE_ROPE_HEIGHT (5.0f)

#endif
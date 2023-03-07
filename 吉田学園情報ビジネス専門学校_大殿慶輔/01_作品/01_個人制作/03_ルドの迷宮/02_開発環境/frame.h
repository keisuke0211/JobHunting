//◇====================◇
//｜　　　　　　　　　　｜
//｜　frame.h           ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _FRAME_H_  //このマクロ定義がされてなかったら
#define _FRAME_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

//*****************************************************************************
// 枠　一覧
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置

	float fWidth;		//横幅
	float fHight;		//高さ

	bool bUse;
}Frame;
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitFrame(void);
void UninitFrame(void);
void UpdateFrame(void);
void DrawaFrame(void);

#endif
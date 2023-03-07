//◇====================◇
//｜　　　　　　　　　　｜
//｜　text.h            ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _TEXT_H_  //このマクロ定義がされてなかったら
#define _TEXT_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

//*****************************************************************************
// テキスト一覧
//*****************************************************************************
typedef enum
{
	TEXT_SCORE = 0,				// SCORE
	TEXT_TIME,					// TIME
	TEXT_DAMAGE,				// Damage
} TEXT;

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitText(void);
void UninitText(void);
void UpdateText(void);
void DrawaText(void);


#endif
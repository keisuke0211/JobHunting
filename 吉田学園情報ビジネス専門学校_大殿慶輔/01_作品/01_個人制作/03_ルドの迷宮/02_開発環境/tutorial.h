//◇====================◇
//｜　　　　　　　　　　｜
//｜　tutorial.h        ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include"main.h"
#include"input.h"
//*****************************************************************************
// ブロック一覧
//*****************************************************************************
typedef enum
{
	TUTORIAL_LABEL_ROCKS02 = 0,		// 基本操作
	TUTORIAL_LABEL_DIR,				// フック操作
	TUTORIAL_LABEL_MAX
}TUTORIAL_LABEL;
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial1(void);

#endif
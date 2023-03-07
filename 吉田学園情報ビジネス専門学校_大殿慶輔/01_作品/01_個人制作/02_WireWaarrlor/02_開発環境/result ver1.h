//◇====================◇
//｜　　　　　　　　　　｜
//｜　resultver1.h      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _RESULT_VER1_H_
#define _RESULt_VER1_H_

#include"main.h"
#include"input.h"
typedef struct
{
	bool bUse;			//使用しているかどうか

}Result1;
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitResult1(void);
void UninitResult1(void);
void UpdateResult1(void);
void DrawResult1(void);

#endif
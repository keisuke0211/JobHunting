//◇====================◇
//｜　　　　　　　　　　｜
//｜　taime.h 　　　　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _TIME_H_  //このマクロ定義がされてなかったら
#define _TIME_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);

//スコアの設定と加算
void SetTime(int nScore);
void AddTime(int nValue);

int GetTime(void);
#endif
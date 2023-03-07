//◇====================◇
//｜　　　　　　　　　　｜
//｜　topscore.h 　 　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _TOPSCORE_H_  //このマクロ定義がされてなかったら
#define _TOPSCORE_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

void InitTopScore(void);
void UninitTopScore(void);
void UpdateTopScore(void);
void DrawTopScore(void);

//スコアの設定と加算
void SetTopScore(int nScore);
//void AddTopScore(int nValue);
int GetTopScore(void);

#endif

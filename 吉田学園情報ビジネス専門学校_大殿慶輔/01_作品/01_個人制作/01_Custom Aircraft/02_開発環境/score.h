//◇====================◇
//｜　　　　　　　　　　｜
//｜　score.h 　　　　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _SCORE_H_  //このマクロ定義がされてなかったら
#define _SCORE_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);

//スコアの設定と加算
void SetScore(int nScore);
void AddScore(int nValue);

int GetScore(void);
#endif

//◇====================◇
//｜　　　　　　　　　　｜
//｜　ranking.h 　　　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _RESULT_SCORE_H_  //このマクロ定義がされてなかったら
#define _RESULT_SCORE_H_  //２重インクルード防止のマクロを定義する

#include"main.h"
#include<stdio.h>

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nScore;
	int nType;
	bool bUse;
}Ranking;

//プロトタイプ宣言
void InitResulyScore(void);
void UninitResulyScore(void);
void UpdateResulyScore(void);
void DrawResulyScore(void);

void SetResult(int nRanking);

#endif

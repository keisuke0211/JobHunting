//◇====================◇
//｜　　　　　　　　　　｜
//｜　ranking.h 　　　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _RESULT_SCORE1_H_  //このマクロ定義がされてなかったら
#define _RESULT_SCORE1_H_  //２重インクルード防止のマクロを定義する

#include"main.h"
#include<stdio.h>

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nScore;
	int nType;
	bool bUse;
}Ranking1;

//プロトタイプ宣言
void InitResulyScore1(void);
void UninitResulyScore1(void);
void UpdateResulyScore1(void);
void DrawResulyScore1(void);

void SetResult1(int nRanking);

#endif

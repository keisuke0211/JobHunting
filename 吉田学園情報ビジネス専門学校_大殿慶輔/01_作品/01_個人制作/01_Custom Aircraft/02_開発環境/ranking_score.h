//◇====================◇
//｜　　　　　　　　　　｜
//｜　ranking.h 　　　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _RANKING_SCORE_H_  //このマクロ定義がされてなかったら
#define _RANKING_SCORE_H_  //２重インクルード防止のマクロを定義する

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
void InitRankingScore(void);
void UninitRankingScore(void);
void UpdateRankingScore(void);
void DrawRankingScore(void);

void LoadRanking(void);
void SaveRanking(void);
void SetRanking(D3DXVECTOR3 pos,int nRanking);

#endif
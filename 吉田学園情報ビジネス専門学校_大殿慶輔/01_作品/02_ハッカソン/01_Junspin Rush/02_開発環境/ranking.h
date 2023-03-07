//==========================================
//
//ランキング表示プログラムのヘッダ[ranking.h]
//Author:石原颯馬
//
//==========================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//マクロ
#define MAX_RANK	(5)		//ランキング表示数

//ランキング構造体定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nScore;			//スコア
	bool bDisp;			//表示・非表示
} Ranking;

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(int nScore);

#endif // !_RANKING_H_

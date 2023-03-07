//==========================================
//
//スコア表示プログラムのヘッダ[score.h]
//Author:石原颯馬
//
//==========================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "main.h"

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore2(void);

#endif // !_SCORE_H_

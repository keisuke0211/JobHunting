//========================================
// 
// 処理のまとまりのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chunk.h ***
//========================================
#ifndef _CHUNK_H_	// このマクロ定義がされていなかった時
#define _CHUNK_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include <stdio.h>

//****************************************
// 列挙型の定義
//****************************************
// カウントの種類
typedef enum
{
	COUNT_TYPE_NORMAL,	// 通常
	COUNT_TYPE_TURN,	// 折り返し
	COUNT_TYPE_MAX,
}COUNT_TYPE;

//****************************************
// プロトタイプ宣言
//****************************************
// 拡大倍率情報の読み込み処理
void LoadScale(FILE *pFile, Scale *pScale);

// カウント処理
int Count(int *pData, int nMin, int nMax, int *pCounter, int nCountTime, COUNT_TYPE countType);

// int型の消費処理
bool IntConsumption(int *pData, int nSubtract);

// float型の値の制御処理
// float型の値のループ制御処理
// int型の値の制御処理
// int型の値のループ制御処理
// int型の値の制御処理(結果を返す)
// int型の値のループ制御処理(結果を返す)
void FloatControl(float *pData, float fMax, float fMin);
void FloatLoopControl(float *pData, float fMax, float fMin);
void IntControl(int *pData, int nMax, int nMin);
void IntLoopControl(int *pData, int nMax, int nMin);
int IntControlReturn(int nData, int nMax, int nMin);
int IntLoopControlReturn(int nData, int nMax, int nMin);

// 抽選処理
bool Lottery(float fProb);

// 小数の乱数を返す
// 乱数を返す
float fRand(float fMax);
int Rand(int nMin, int nMax);

// 与えられた値に応じて1か-1を返す
// 与えられた値が0でない時は0を、0の時は1を返す
// 与えられた値が0以下の時は0を、0以上の時は値をそのまま返す
int PrusMinus(float fData);
int ExistsZero(float fData);
float Prus(float fData);

// 空白埋め
void strinit(char *pData, int nNum);

#endif
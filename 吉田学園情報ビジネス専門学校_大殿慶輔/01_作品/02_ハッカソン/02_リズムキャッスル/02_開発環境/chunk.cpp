//========================================
// 
// 処理のまとまり
// Author:RIKU NISHIMURA
// 
//========================================
// *** chunk.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include <stdio.h>

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadScale関数 - 拡大倍率情報の読み込み処理 -
//========================================
void LoadScale(FILE *pFile, Scale *pScale) 
{
	fscanf(pFile, "%f", &pScale->fWidth);
	fscanf(pFile, "%f", &pScale->fHeight);
	fscanf(pFile, "%f", &pScale->fDepth);
}

//========================================
// Count関数 - カウント処理 -
// Author:RIKU NISHIMURA
//========================================
int Count(int *pData, int nMin, int nMax, int *pCounter, int nCountTime, COUNT_TYPE countType)
{
	if (nCountTime != -1)
	{// カウントにかかる時間が-1でない時、
		if (pCounter == NULL) 
		{// カウンターのポインタがNULLでない時、
			*pData += 1;	// 値を加算
		}
		else
		{// カウンターのポインタがNULLの時、
			*pCounter += 1;	// カウンターを加算

			if (*pCounter >= nCountTime)
			{// カウンターがカウントにかかる時間に達した時、
				*pCounter = 0;	// カウンターを初期化する
				*pData += 1;	// 値を加算
			}
		}
	}
	else
	{// カウントにかかる時間が-1の時、
		// 値をそのまま返す
		return *pData;
	}

	// カウントの種類に応じた値を返す
	switch (countType)
	{
		//========== *** 通常 ***
	case COUNT_TYPE_NORMAL:

		// 値を上限で剰余算した結果を返す
		return nMin + (*pData %= (nMax - nMin + 1));

		//========== *** 折り返す ***
	case COUNT_TYPE_TURN:

		if ((*pData / (nMax - nMin)) % 2 == 0)
		{// 値を上限で除算した結果が偶数だった時、
			// 値を上限で剰余算した結果を返す
			return nMin + (*pData %= (nMax - nMin));
		}
		else
		{//パターンを上限で除算した結果が奇数だった時、
			//上限からパターンを上限で剰余算した結果を減算した結果を返す
			return nMax - (*pData % (nMax - nMin));
		}
	}
}

//========================================
// IntConsumption関数 - int型の値の消費処理 -
// Author:RIKU NISHIMURA
//========================================
bool IntConsumption(int *pData, int nSubtract) 
{
	if ((*pData -= nSubtract) >= 0)
	{// 値から消費量を減算した結果0以上の時、
		return true;	// 真を返す
	}
	else 
	{// 値から消費量を減算した結果0を下回っている時、
		*pData += nSubtract;	// 減算した分加算
		return false;			// 偽を返す
	}
}

//========================================
// FloatControl関数 - float型の値の制御処理 -
// Author:RIKU NISHIMURA
//========================================
void FloatControl(float *pData, float fMax, float fMin)
{
	if (*pData > fMax)
	{// 最大値を上回っていた時、
		// 最大値を代入
		*pData = fMax;
	}
	else if (*pData < fMin)
	{// 最小値を下回っていた時、
		// 最小値を代入
		*pData = fMin;
	}
}

//========================================
// FloatLoopControl関数 - float型の値のループ制御処理 -
// Author:RIKU NISHIMURA
//========================================
void FloatLoopControl(float *pData, float fMax, float fMin)
{
	if (*pData > fMax)
	{// 最大値を上回っていた時、
		// 最大値分減算
		*pData -= (fMax - fMin);
	}
	else if (*pData < fMin)
	{// 最小値を下回っていた時、
		// 最大値分加算
		*pData += (fMax - fMin);
	}
}

//========================================
// IntControl関数 - int型の値の制御処理 -
// Author:RIKU NISHIMURA
//========================================
void IntControl(int *pData, int nMax, int nMin)
{
	if (*pData > nMax)
	{// 最大値を上回っていた時、
		// 最大値を代入
		*pData = nMax;
	}
	else if (*pData < nMin)
	{// 最小値を下回っていた時、
		// 最小値を代入
		*pData = nMin;
	}
}

//========================================
// IntLoopControl関数 - int型の値のループ制御処理 -
// Author:RIKU NISHIMURA
//========================================
void IntLoopControl(int *pData, int nMax, int nMin)
{
	if (*pData >= nMax)
	{// 最大値以上の時、
		// 最小値を代入
		*pData = nMin;
	}
	else if (*pData < nMin)
	{// 最小値を下回っていた時、
		// 最大値を代入
		*pData = nMax - 1;
	}
}


//========================================
// IntControlReturn関数 - int型の値の制御処理(結果を返す) -
// Author:RIKU NISHIMURA
//========================================
int IntControlReturn(int nData, int nMax, int nMin)
{
	if (nData > nMax)
	{// 最大値を上回っていた時、
		// 最大値を代入
		nData = nMax;
	}
	else if (nData < nMin)
	{// 最小値を下回っていた時、
		// 最小値を代入
		nData = nMin;
	}

	return nData;
}

//========================================
// IntLoopControlReturn関数 - int型の値のループ制御処理(結果を返す) -
// Author:RIKU NISHIMURA
//========================================
int IntLoopControlReturn(int nData, int nMax, int nMin)
{
	if (nData >= nMax)
	{// 最大値以上の時、
		// 最小値を代入
		nData = nMin;
	}
	else if (nData < nMin)
	{// 最小値を下回っていた時、
		// 最大値を代入
		nData = nMax - 1;
	}

	return nData;
}

//========================================
// Lottery関数 - 抽選処理 -
// Author:RIKU NISHIMURA
//========================================
bool Lottery(float fProb)
{
	if (fProb > (0.1f * (rand() % 1000)))
	{// 当選した時、
		// 真を返す
		return true;
	}
	else
	{// 当選しなかった時、
		// 偽を返す
		return false;
	}
}

//========================================
// fRand関数 - 小数の乱数を返す -
// Author:RIKU NISHIMURA
//========================================
float fRand(float fMax)
{
	if (fMax == 0) 
	{// 上限が0の時、
		return 0;	// 処理を終了する
	}

	float fNum;	// 小数の値

	fMax *= 100;

	fNum = rand() % (int)fMax;

	return fNum * 0.01f;
}

//========================================
// Rand関数 - 乱数を返す -
// Author:RIKU NISHIMURA
//========================================
int Rand(int nMin, int nMax) 
{
	return nMin + (rand() % (nMax - nMin));
}

//========================================
// PrusMinus関数 - 与えられた値に応じて1を符号付きで返す -
// Author:RIKU NISHIMURA
//========================================
int PrusMinus(float fData)
{
	if (fData == 0)
	{// 引数が0だった時、
		// 0を返す
		return 0;
	}

	// 1を符号付きで返す
	return (fabsf(fData) / fData);
}

//========================================
// ExistsZero関数 - 与えられた値が0でない時は0を、0の時は1を返す -
// Author:RIKU NISHIMURA
//========================================
int ExistsZero(float fData) 
{
	if (fData != 0) 
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}

//========================================
// Prus関数 - 与えられた値が0以下の時は0を、0以上の時は値をそのまま返す -
// Author:RIKU NISHIMURA
//========================================
float Prus(float fData) 
{
	if (fData <= 0) 
	{
		return 0;
	}
	else 
	{
		return fData;
	}
}

//========================================
// strinit関数 - 空白埋め -
// Author:RIKU NISHIMURA
//========================================
void strinit(char *pData, int nNum)
{
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		pData[nCnt] = '\0';
	}
}
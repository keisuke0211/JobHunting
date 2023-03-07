//========================================
//  *** chunk.h ***
//========================================
#ifndef _CHUNK_H_	// このマクロ定義がされていなかった時
#define _CHUNK_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************

// float型の値の制御処理
void FloatControl(float *fData, float fMax, float fMin);

// float型の値のループ制御処理
void FloatLoopControl(float *fData, float fMax, float fMin);

// int型の値の制御処理
void IntControl(int *fData, int fMax, int fMin);

// int型の値のループ制御処理
void IntLoopControl(int *fData, int fMax, int fMin);

#endif
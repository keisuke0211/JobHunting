//========================================
// 
// BG:リザルト[01] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_result_01.h ***
//========================================
#ifndef _BG_RESULT_01_H_	// このマクロ定義がされていなかった時
#define _BG_RESULT_01_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// BG:リザルト[01] の読み込み処理
void LoadBg_result_01(void);

// BG:リザルト[01] の初期化処理
// BG:リザルト[01] の終了処理
// BG:リザルト[01] の更新処理
// BG:リザルト[01] の描画処理
void InitBg_result_01(void);
void UninitBg_result_01(void);
void UpdateBg_result_01(void);
void DrawBg_result_01(void);

#endif
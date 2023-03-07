//========================================
// 
// BG:リザルト[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_result_00.h ***
//========================================
#ifndef _BG_RESULT_00_H_	// このマクロ定義がされていなかった時
#define _BG_RESULT_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// BG:リザルト[00] の読み込み処理
void LoadBg_result_00(void);

// BG:リザルト[00] の初期化処理
// BG:リザルト[00] の終了処理
// BG:リザルト[00] の更新処理
// BG:リザルト[00] の描画処理
void InitBg_result_00(void);
void UninitBg_result_00(void);
void UpdateBg_result_00(void);
void DrawBg_result_00(void);

#endif
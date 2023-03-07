//========================================
// 
// BG:チュートリアル[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_tutorial_00.h ***
//========================================
#ifndef _BG_TUTORIAL_00_H_	// このマクロ定義がされていなかった時
#define _BG_TUTORIAL_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// BG:チュートリアル[00] の読み込み処理
void LoadBg_tutorial_00(void);

// BG:チュートリアル[00] の初期化処理
// BG:チュートリアル[00] の終了処理
// BG:チュートリアル[00] の更新処理
// BG:チュートリアル[00] の描画処理
void InitBg_tutorial_00(void);
void UninitBg_tutorial_00(void);
void UpdateBg_tutorial_00(void);
void DrawBg_tutorial_00(void);

#endif
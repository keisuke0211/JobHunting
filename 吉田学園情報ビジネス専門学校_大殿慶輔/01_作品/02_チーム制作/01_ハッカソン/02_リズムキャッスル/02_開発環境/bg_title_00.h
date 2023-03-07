//========================================
// 
// BG:タイトル[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** bg_title_00.h ***
//========================================
#ifndef _BG_TITLE_00_H_	// このマクロ定義がされていなかった時
#define _BG_TITLE_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// BG:タイトル[00] の読み込み処理
void LoadBg_title_00(void);

// BG:タイトル[00] の初期化処理
// BG:タイトル[00] の終了処理
// BG:タイトル[00] の更新処理
// BG:タイトル[00] の描画処理
void InitBg_title_00(void);
void UninitBg_title_00(void);
void UpdateBg_title_00(void);
void DrawBg_title_00(void);

#endif
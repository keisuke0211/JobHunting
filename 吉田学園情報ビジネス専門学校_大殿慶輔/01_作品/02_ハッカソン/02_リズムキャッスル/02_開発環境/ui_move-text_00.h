//========================================
// 
// UI:移動テキスト[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_move-text_00.h ***
//========================================
#ifndef _UI_MOVETEXT_00_H_	// このマクロ定義がされていなかった時
#define _UI_MOVETEXT_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// プロトタイプ宣言
//****************************************
// UI:移動テキスト[00] の初期化処理
// UI:移動テキスト[00] の終了処理
// UI:移動テキスト[00] の更新処理
void InitUi_moveText_00(void);
void UninitUi_moveText_00(void);
void UpdateUi_moveText_00(void);

// UI:移動テキスト[00] の設定処理
void SetUi_moveText_00(char *pString, int nFont, D3DXVECTOR3 pos, D3DXVECTOR3 move, Color col, int nLife);

#endif
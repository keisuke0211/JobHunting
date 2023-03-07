//========================================
// *** fade.h ***
//========================================
#ifndef _FADE_H_	// このマクロ定義がされていなかった時
#define _FADE_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// 列挙型の定義
//****************************************

// フェードの状態
typedef enum 
{
	FADE_NONE = 0,	// 何もしていない状態
	FADE_IN,		// フェードイン状態
	FADE_OUT,		// フェードアウト状態
	FADE_MAX,
}FADE;

//****************************************
// プロトタイプ宣言
//****************************************

// フェードの設定処理
void SetFade(MODE modeNext);

// フェードの切り替え取得
bool GetFadeSwap(void);

// フェードの初期化処理
// フェードの終了処理
// フェードの更新処理
// フェードの描画処理
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

// フェードの状態を取得
FADE GetFade(void);

#endif
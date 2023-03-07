//========================================
// *** fade.h ***
//========================================
#ifndef _FADE_H_	// このマクロ定義がされていなかった時
#define _FADE_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//========== 列挙型の定義		==========// 

// フェードの状態
typedef enum 
{
	FADE_NONE = 0,	// 何もしていない状態
	FADE_IN,		// フェードイン状態
	FADE_OUT,		// フェードアウト状態
	FADE_MAX,
}FADE;

//========== プロトタイプ宣言	==========//
void InitFade(MODE modeNext);	// フェードの初期化処理
void UninitFade(void);			// フェードの終了処理
void UpdateFade(void);			// フェードの更新処理
void DrawFade(void);			// フェードの描画処理
void SetFade(MODE modeNext);	// フェードの設定処理
FADE GetFade(void);				// フェードの状態を取得

#endif
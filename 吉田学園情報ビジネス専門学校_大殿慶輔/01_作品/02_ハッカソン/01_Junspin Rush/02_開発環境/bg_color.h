//========================================
// *** bg_color.h ***
//========================================
#ifndef _BG_COLOR_H_	// このマクロ定義がされていなかった時
#define _BG_COLOR_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************

// 背景色の初期値
#define BG_START_COLOR	{77, 207, 217, 255}

//****************************************
// 構造体の定義
//****************************************

// 背景色の情報構造体
typedef struct 
{
	Color	col;					// 色
	Color	oldColor;				// 古い色
	Color	newColor;				// 新しい色
	bool	bChangeColor;			// 変色フラグ
	int		nCounterChangeColor;	// 変色カウンター
	int		nTimeChangeColor;		// 変色にかかる時間
}Bg_color;

//****************************************
// プロトタイプ宣言
//****************************************

// 背景色の情報を取得
Bg_color *GetBg_color(void);

// 背景色の初期化処理
void InitBg_color(void);

// 背景色の終了処理
void UninitBg_color(void);

// 背景色の更新処理
void UpdateBg_color(void);

// 背景色の設定処理
void SetBg_color(Color newColor, int nTime);

#endif
//========================================
// *** ui_frame_02.h ***
//========================================
#ifndef _UI_FRAME_02_H_	// このマクロ定義がされていなかった時
#define _UI_FRAME_02_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// フレーム[02] の状態
typedef enum
{
	UI_FRAME_02_STATE_POP = 0,	// 出現
	UI_FRAME_02_STATE_IN_POP,	// 出現中
	UI_FRAME_02_STATE_CLEAR,	// 消えている
	UI_FRAME_02_STATE_IN_CLEAR,	// 消え中
	UI_FRAME_02_STATE_MAX,
}UI_FRAME_02_STATE;

//****************************************
// 構造体の定義
//****************************************

// フレーム[02] の情報構造体
typedef struct
{
	UI_FRAME_02_STATE	state;			// 状態
	int					nCounterState;	// 状態カウンター
	float				fAlpha;			// 全体の透明度
	D3DXVECTOR3			pos;			// 位置
}Ui_frame_02;

//****************************************
// プロトタイプ宣言
//****************************************

// フレーム[02] の情報を取得
Ui_frame_02 *GetUi_frame_02(void);

// フレーム[02] の初期化処理
void InitUi_frame_02(void);

// フレーム[02] の終了処理
void UninitUi_frame_02(void);

// フレーム[02] の更新処理
void UpdateUi_frame_02(void);

// フレーム[02] の描画処理
void DrawUi_frame_02(void);

// フレーム[02] の状態設定処理
void SetStateUi_frame_02(UI_FRAME_02_STATE state);

// フレーム[02] の設定処理
void SetUi_frame_02(D3DXVECTOR3 pos);

#endif
//========================================
// 
// MD:リザルト画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_result_00.h ***
//========================================
#ifndef _MD_RESULT_00_H_	// このマクロ定義がされていなかった時
#define _MD_RESULT_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// 列挙型の定義
//****************************************
// MD:リザルト画面[00] の状態
typedef enum 
{
	MD_RESULT_00_STATE_NONE,		// 無し
	MD_RESULT_00_STATE_NORMAL,		// 通常
	MD_RESULT_00_STATE_RANKINGWAIT,	// ランキング待ち
	MD_RESULT_00_STATE_MAX,
}MD_RESULT_00_STATE;

//****************************************
// 構造体の定義
//****************************************
// MD:リザルト画面[00] の情報構造体
typedef struct 
{
	// 読み込み情報
	int			nPromptTextFont;	// 入力催促テキストのフォント
	D3DXVECTOR3 promptTextPos;		// 入力催促テキストの位置
	Color		promptTextCol;		// 入力催促テキストの色
	int			nWaitBlinkTime;		// 待機時の点滅にかかる時間
	int			nDecisionBlinkTime;	// 決定時の点滅にかかる時間
	int			nBGM;				// BGM
	int			nRankingSetSE;		// ランキングセットSE
	int			nDecisionSE;		// 決定SE

	// 状態関連
	MD_RESULT_00_STATE	state;				// MD:リザルト画面[00] の状態
	int					nCounterState;		// 状態のカウンター
	int					nCounterPromptText;	// 入力催促テキストの汎用カウンター
	int					nUpdateRank;		// 更新順位
	int					nCounterBlink;		// 点滅カウンター
}Md_result_00;

//****************************************
// プロトタイプ宣言
//****************************************
// MD:リザルト画面[00] の情報を取得
Md_result_00 *GetMd_result_00(void);

// MD:リザルト画面[00] の読み込み処理
void LoadMd_result_00(void);

// MD:リザルト画面[00] の初期化処理
// MD:リザルト画面[00] の終了処理
// MD:リザルト画面[00] の更新処理
// MD:リザルト画面[00] の描画処理
void InitMd_result_00(void);
void UninitMd_result_00(void);
void UpdateMd_result_00(void);
void DrawMd_result_00(void);

// MD:リザルト画面[00] の状態を設定
void SetMd_result_00State(MD_RESULT_00_STATE state);

// MD:リザルト画面[00] の背景を設定
void SetBgMd_result_00(int nBg);

#endif
//========================================
// *** md_game_00.h ***
//========================================
#ifndef _MD_GAME_00_H_	// このマクロ定義がされていなかった時
#define _MD_GAME_00_H_	// 二重インクルード防止のマクロを定義する

#include "text.h"

//****************************************
// 列挙型の定義
//****************************************

// ゲーム画面[00] の状態
typedef enum 
{
	MD_GAME_00_STATE_N1 = 0,			// 何もしていない
	MD_GAME_00_STATE_PAUSE,				// ポーズ
	MD_GAME_00_STATE_START_WAIT,		// スタート待ち
	MD_GAME_00_STATE_CLEAR_WAIT,		// クリア待ち
	MD_GAME_00_STATE_NORMAL,			// 通常
	MD_GAME_00_STATE_GAME_CLEAR,		// ゲームクリア
	MD_GAME_00_STATE_GAME_OVER,			// ゲームオーバー
	MD_GAME_00_STATE_DIED_AFTERGLOW,	// 死亡後の余韻
	MD_GAME_00_STATE_END,				// 終了
	MD_GAME_00_STATE_MAX
}MD_GAME_00_STATE;

// ゲーム画面[00] のテキスト番号
typedef enum
{
	MD_GAME_00_TEXT_SIGNAL,			// 合図
	MD_GAME_00_TEXT_TIME,			// 時間
	MD_GAME_00_TEXT_SCORE,			// スコア
	MD_GAME_00_TEXT_GAME_CLEAR,		// ゲームクリア
	MD_GAME_00_TEXT_GAME_OVER,		// ゲームオーバー
	MD_GAME_00_TEXT_MAX,
}MD_GAME_00_TEXT;

//****************************************
// 構造体の定義
//****************************************

//敵構造体の定義
typedef struct
{
	int nType;			//敵・種類
	int in_time;		//出現時間
	float nAngle;		//向き
	D3DXVECTOR3 pos;	//位置
}EnemySet;

//****************************************
// プロトタイプ宣言
//****************************************

// スコアを取得
int *GetScore(void);

// ゲーム画面[00]の初期化処理
void InitMd_game_00(void);

// ゲーム画面[00]の終了処理
void UninitMd_game_00(void);

// ゲーム画面[00]の更新処理
void UpdateMd_game_00(void);

// ゲーム画面[00]の描画処理
void DrawMd_game_00(void);

// ゲーム画面[00]のテキストの情報を取得
Text GetMd_game_00Text(MD_GAME_00_TEXT text);

// ゲームの状態の設定
void SetMd_game_00State(MD_GAME_00_STATE state);

// ゲームの状態の取得
MD_GAME_00_STATE GetGameState(void);

// 敵の配置情報の読み込み
void LoadEnemy(void);

#endif
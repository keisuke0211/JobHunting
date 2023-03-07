//========================================
// 
// 入力関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** input.h ***
//========================================
#ifndef _INPUT_H_	// このマクロ定義がされていなかった時
#define _INPUT_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// マクロ定義
//****************************************

// プレイヤーの左の移動のキー
// プレイヤーの右の移動のキー
// プレイヤーのジャンプのキー
// プレイヤーのしゃがみのキー
// プレイヤーの門のキー
// プレイヤーのレバーのキー
#define PLAYER_LEFT_MOVE_KEY		(DIK_A)
#define PLAYER_RIGHT_MOVE_KEY		(DIK_D)
#define PLAYER_JUMP_KEY				(DIK_SPACE)
#define PLAYER_SQUAT_KEY			(DIK_S)
#define PLAYER_GATE_KEY				(DIK_W)
#define PLAYER_LEVER_KEY			(DIK_W)

//****************************************
// 列挙型の定義
//****************************************

// 動的な入力の種類
typedef enum 
{
	ACTIVE_INPUT_TYPE_KEYBOARD,
	ACTIVE_INPUT_TYPE_CONTROLLER,
	ACTIVE_INPUT_TYPE_MAX,
}ACTIVE_INPUT_TYPE;

// マウス番号
typedef enum
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_MAX,
}MOUSE;

// ボタン番号
typedef enum 
{
	BUTTON_UP = 0,			// 上キー
	BUTTON_DOWN,			// 下キー
	BUTTON_LEFT,			// 左キー
	BUTTON_RIGHT,			// 右キー
	BUTTON_START,			// スタートキー
	BUTTON_BACK,			// バックキー
	BUTTON_LEFT_THUMB,		// 左スティック
	BUTTON_RIGHT_THUMB,		// 右スティック
	BUTTON_LEFT_SHOULDER,	// 左トリガー
	BUTTON_RIGHT_SHOULDER,	// 右トリガー
	BUTTON_A,				// Aボタン
	BUTTON_B,				// Bボタン
	BUTTON_X,				// Xボタン
	BUTTON_Y,				// Yボタン
	BUTTON_LEFT_TRIGGER,	// 左トリガー
	BUTTON_RIGHT_TRIGGER,	// 右トリガー
	BUTTON_MAX,
}BUTTON;

// スティック番号
typedef enum
{
	STICK_TYPE_LEFT = 0,
	STICK_TYPE_RIGHT,
	STICK_TYPE_MAX
}STICK_TYPE;

// スティックの方向
typedef enum 
{
	STICK_ANGLE_UP = 0,
	STICK_ANGLE_DOWN,
	STICK_ANGLE_LEFT,
	STICK_ANGLE_RIGHT,
	STICK_ANGLE_MAX,
}STICK_ANGLE;

//****************************************
// 構造体の定義
//****************************************

// スティックの入力情報構造体
typedef struct 
{
	float	aTplDiameter	// スティックの倒し具合
			[STICK_TYPE_MAX];
	float	aAngle			// スティックの角度
			[STICK_TYPE_MAX];
	bool	aAnglePress		// スティックの方向プレス情報
			[STICK_TYPE_MAX]
			[STICK_ANGLE_MAX];
	bool	aAngleTrigger	// スティックの方向トリガー情報
			[STICK_TYPE_MAX]
			[STICK_ANGLE_MAX];
	bool	aAngleRepeat	// スティックの方向リピート情報
			[STICK_TYPE_MAX]
			[STICK_ANGLE_MAX];
	bool	aAngleRelease	// スティックの方向リリース情報
			[STICK_TYPE_MAX]
			[STICK_ANGLE_MAX];
}STICK_INPUT;

//****************************************
// プロトタイプ宣言
//****************************************

// 画面上のカーソルの位置を取得
// カーソルの移動量を取得
// XInputの状態を取得
// スティックの入力情報を取得
// 動的な入力の種類を取得
D3DXVECTOR3 GetCursorPosOnScreen(void);
D3DXVECTOR3 GetCursorMove(void);
XINPUT_STATE *GetXInputState(void);
STICK_INPUT GetStick(void);
ACTIVE_INPUT_TYPE GetActiveInputType();

// 入力関連の初期化処理
// 入力関連の終了処理
// 入力関連の更新処理
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(bool bPause);

// キーボードのプレス情報を取得
// キーボードのトリガー情報を取得
// キーボードのリピート情報を取得
// キーボードのリリース情報を取得
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

// マウスのプレス情報を取得
// マウスのトリガー情報を取得
// マウスのリピート情報を取得
// マウスのリリース情報を取得
bool GetMousePress(int nMouse);
bool GetMouseTrigger(int nMouse);
bool GetMouseRepeat(int nMouse);
bool GetMouseRelease(int nMouse);

// ボタンのプレス情報を取得
// ボタンのトリガー情報を取得
// ボタンのリピート情報を取得
// ボタンのリリース情報を取得
bool GetButtonPress(int nButton);
bool GetButtonTrigger(int nButton);
bool GetButtonRepeat(int nButton);
bool GetButtonRelease(int nButton);

// コントローラーの振動設定処理
void SetVibration(float fVibration, int nTime);

#endif
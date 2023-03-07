//========================================
//  *** input.h ***
//========================================
#ifndef _INPUT_H_	// このマクロ定義がされていなかった時
#define _INPUT_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

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
	float	aTplDiameter						// スティックの倒し具合
			[STICK_TYPE_MAX];					// 
	float	aAngle								// スティックの角度
			[STICK_TYPE_MAX];					// 
	bool	aAnglePress							// スティックの方向プレス情報
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];	// 
	bool	aAngleTrigger						// スティックの方向トリガー情報
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];	// 
	bool	aAngleRepeat						// スティックの方向リピート情報
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];	// 
	bool	aAngleRelease						// スティックの方向リリース情報
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];	// 
}STICK_INPUT;

//****************************************
// プロトタイプ宣言
//****************************************

// 画面上のカーソルの位置を取得
D3DXVECTOR3 GetCursorPosOnScreen(void);

// XInputの状態を取得
XINPUT_STATE *GetXInputState(void);

// スティックの入力情報を取得
STICK_INPUT GetStick(void);

// 動的な入力の種類を取得
ACTIVE_INPUT_TYPE GetActiveInputType();

// 入力関連の初期化処理
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);

// 入力関連の終了処理
void UninitInput(void);

// キーボードの更新処理
void UpdateKeyboard(void);

// キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey);

// キーボードのトリガー情報を取得
bool GetKeyboardTrigger(int nKey);

// キーボードのリピート情報を取得
bool GetKeyboardRepeat(int nKey);

// キーボードのリリース情報を取得
bool GetKeyboardRelease(int nKey);

// マウスの更新処理
void UpdateMouse(void);

// マウスのプレス情報を取得
bool GetMousePress(int nMouse);

// マウスのトリガー情報を取得
bool GetMouseTrigger(int nMouse);

// マウスのリピート情報を取得
bool GetMouseRepeat(int nMouse);

// マウスのリリース情報を取得
bool GetMouseRelease(int nMouse);

// カーソルの更新処理
void UpdateCursor(void);

// ボタンの更新処理
void UpdateButton(void);

// ボタンのプレス情報を取得
bool GetButtonPress(int nButton);

// ボタンのトリガー情報を取得
bool GetButtonTrigger(int nButton);

// ボタンのリピート情報を取得
bool GetButtonRepeat(int nButton);

// ボタンのリリース情報を取得
bool GetButtonRelease(int nButton);

// スティックの更新処理
void UpdateStick(void);

// コントローラーの振動の更新処理
void UpdateVibration(void);

// コントローラーの振動設定処理
void SetVibration(float fVibration, int nTime);

#endif
//****************************************
//	input.h
//	Author:大殿慶輔
//****************************************
#ifndef _INPUT_H_
#define _INPUT_H_

#include"main.h"

//****************************************
// スティック入力の情報構造体
//****************************************

// スティックの入力情報構造体
typedef struct
{
	float fTplDiameter;	// スティックの倒し具合
	float fAngle;		// スティックの角度
}STICK_INPUT;

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

// ステック番号
typedef enum
{
	STICK_TYPE_LEFT = 0,
	STICK_TYPE_RIGHT,
	STICK_TYPE_MAX
}STICK_TYPE;

//****************************************
// プロトタイプ宣言
//****************************************

HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);	// 入力関連の初期化処理
void UninitInput(void);								// 入力関連の終了処理
void UpdateInput(void);								// 入力関連の更新処理

void UpdateKeyboard(void);							// キーボードの更新処理
void UpdateMouse(void);								// マウスの更新処理
void UpdateCursor(void);							// カーソルの更新処理

/* キーボード */
bool GetKeyboardPress(int nKey);					// キーボードのプレス情報を取得
bool GetKeyboardTrigger(int nKey);					// キーボードのトリガー情報を取得
bool GetKeyboardRepeat(int nKey);					// キーボードのリピート情報を取得
bool GetKeyboardRelease(int nKey);					// キーボードのリリース情報を取得

/* カーソル */
D3DXVECTOR3 GetCursorPosOnScreen(void);				// 画面上のカーソルの位置を取得
D3DXVECTOR3 GetCursorMove(void);					// カーソルの移動量を取得

/* マウス */
bool GetMousePress(int nMouse);						// マウスのプレス情報を取得
bool GetMouseTrigger(int nMouse);					// マウスのトリガー情報を取得
bool GetMouseRepeat(int nMouse);					// マウスのリピート情報を取得
bool GetMouseRelease(int nMouse);					// マウスのリリース情報を取得

XINPUT_STATE *GetXInputState(void);
STICK_INPUT GetStick(STICK_TYPE type);

#endif
//========================================
// 
// 入力関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** input.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// キーの最大数
// リピートの間隔
#define NUM_KEY_MAX			(256)
#define REPEATE_INTERVAL	(160)

// 振動の減算量
#define VIBRATION_SUBTRACT	(0.02f)

//****************************************
// プロトタイプ宣言
//****************************************
// キーボードの更新処理
void UpdateKeyboard(void);

// マウスの更新処理
void UpdateMouse(void);

// カーソルの更新処理
void UpdateCursor(void);

// ボタンの更新処理
void UpdateButton(void);

// スティックの更新処理
void UpdateStick(void);

// コントローラーの振動の更新処理
void UpdateVibration(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECTINPUT8			g_pInput				// DirectInputオブジェクトへのポインタ
						= NULL;
LPDIRECTINPUTDEVICE8	g_pDevKeyboard			// 入力デバイス(キーボード)へのポインタ
						= NULL;
BYTE					g_aKeyStatePress		// キーボードのプレス情報
						[NUM_KEY_MAX];
BYTE					g_aKeyStateTrigger		// キーボードのトリガー情報
						[NUM_KEY_MAX];
BYTE					g_aKeyStateRelease		// キーボードのリリース情報
						[NUM_KEY_MAX];
BYTE					g_aKeyStateRepeat		// キーボードのリピート情報
						[NUM_KEY_MAX];
DWORD					g_aKeyCurrentTime		// キーボードの現在の時間
						[NUM_KEY_MAX];
DWORD					g_aKeyExecLastTime		// キーボードの最後に真を返した時間
						[NUM_KEY_MAX];
BYTE					g_aMouseState			// マウスのプレス情報
						[MOUSE_MAX];
BYTE					g_aMouseStateTrigger	// マウスのトリガー情報
						[MOUSE_MAX];
BYTE					g_aMouseStateRelease	// マウスのリリース情報
						[MOUSE_MAX];
BYTE					g_aMouseStateRepeat		// マウスのリピート情報
						[MOUSE_MAX];
DWORD					g_aMouseCurrentTime		// マウスの現在の時間
						[MOUSE_MAX];
DWORD					g_aMouseExecLastTime	// マウスの最後に真を返した時間
						[MOUSE_MAX];
D3DXVECTOR3				g_cursorPosTemp;		// カーソルの位置を保存
D3DXVECTOR3				g_cursorMove;			// カーソルの移動量
bool					g_aButtonPress			// ボタンのプレス情報
						[BUTTON_MAX];
bool					g_aButtonTrigger		// ボタンのトリガー情報
						[BUTTON_MAX];
bool					g_aButtonRelease		// ボタンのリリース情報
						[BUTTON_MAX];
bool					g_aButtonRepeat			// ボタンのリピート情報
						[BUTTON_MAX];
DWORD					g_aButtonCurrentTime	// ボタンの現在の時間
						[BUTTON_MAX];
DWORD					g_aButtonExecLastTime	// ボタンの最後に真を返した時間
						[BUTTON_MAX];
STICK_INPUT				g_stick;				// スティックの入力情報
DWORD					g_aStickCurrentTime		// スティックの現在の時間
						[STICK_TYPE_MAX][STICK_ANGLE_MAX];
DWORD					g_aStickExecLastTime	// スティックの最後に真を返した時間
						[STICK_TYPE_MAX][STICK_ANGLE_MAX];
int						g_nCounterVibration;	// コントローラーの振動カウンター
int						g_nCounterVibrationMax;	// コントローラーの振動カウンターの最大値を保存
float					g_fVibration;			// 振動倍率
XINPUT_STATE			g_xInputState;			// XInputの状態
ACTIVE_INPUT_TYPE		g_activeInputType;		// 動的な入力の種類

// マウスのマスク
const int g_aMouseMask[MOUSE_MAX] =
{
	VK_LBUTTON,
	VK_RBUTTON,
};

// ボタンのマスク
const int g_aButtonMask[BUTTON_MAX] =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
	-1,
	-1,
};

//========== *** 画面上のカーソルの位置を取得 ***
D3DXVECTOR3 GetCursorPosOnScreen(void)
{
	POINT cursorPos;	// カーソルの位置

	// カーソルの現在位置を取得
	GetCursorPos(&cursorPos);

	RECT rc;	// クライアント領域のサイズ

	// クライアント領域のサイズを取得
	GetClientRect(*GetWindowHandle(), &rc);

	return D3DXVECTOR3(
		(cursorPos.x - GetWindowPos().x) * (SCREEN_WIDTH / (float)rc.right),
		(cursorPos.y - GetWindowPos().y) * (SCREEN_HEIGHT / (float)rc.bottom),
		0.0f);
}

//========== *** カーソルの移動量を取得 ***
D3DXVECTOR3 GetCursorMove(void) 
{
	return g_cursorMove;
}

//========== *** XInputの状態を取得 ***
XINPUT_STATE *GetXInputState(void) 
{
	return &g_xInputState;
}

//========== *** スティックの入力情報を取得 ***
STICK_INPUT GetStick(void)
{
	return g_stick;
}

//========== *** 動的な入力の種類を取得 ***
ACTIVE_INPUT_TYPE GetActiveInputType() 
{
	return g_activeInputType;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateKeyboard関数 - キーボードの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateKeyboard(void)
{
	BYTE	aKeyState	// キーボードの入力情報
			[NUM_KEY_MAX];

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// キーボードのトリガー情報を保存
			g_aKeyStateTrigger[nCntKey] = (g_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];

			// キーボードのリリース情報を保存
			g_aKeyStateRelease[nCntKey] = (g_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey])&~aKeyState[nCntKey];

			// 現在の時間を取得
			g_aKeyCurrentTime[nCntKey] = timeGetTime();

			if (aKeyState[nCntKey] && ((g_aKeyCurrentTime[nCntKey] - g_aKeyExecLastTime[nCntKey]) > REPEATE_INTERVAL))
			{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
				// 最後に真を返した時間を保存
				g_aKeyExecLastTime[nCntKey] = g_aKeyCurrentTime[nCntKey];

				// キーボードのリピート情報を保存
				g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}
			else
			{
				// キーボードのリピート情報を保存
				g_aKeyStateRepeat[nCntKey] = 0;
			}

			// キーボードのプレス情報を保存
			g_aKeyStatePress[nCntKey] = aKeyState[nCntKey];

			if (aKeyState[nCntKey])
			{// 入力が行われた時、
				// 動的な入力の種類をキーボードにする
				g_activeInputType = ACTIVE_INPUT_TYPE_KEYBOARD;
			}
		}
	}
	else
	{// 入力デバイスからデータを取得できなかった時、
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

//========================================
// UpdateMouse関数 - マウスの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMouse(void)
{
	for (int nCntMouse = 0; nCntMouse < MOUSE_MAX; nCntMouse++)
	{
		BYTE	mouseState	// マウスの入力情報
				= GetKeyState(g_aMouseMask[nCntMouse]) & (0x80) ? true : false;;

		// マウスのトリガー情報を保存
		g_aMouseStateTrigger[nCntMouse] = (g_aMouseState[nCntMouse] ^ mouseState)&mouseState;

		// マウスのリリース情報を保存
		g_aMouseStateRelease[nCntMouse] = (g_aMouseState[nCntMouse] ^ mouseState)&~mouseState;

		// 現在の時間を取得
		g_aMouseCurrentTime[nCntMouse] = timeGetTime();

		if (mouseState && ((g_aMouseCurrentTime[nCntMouse] - g_aMouseExecLastTime[nCntMouse]) > REPEATE_INTERVAL))
		{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
			// 最後に真を返した時間を保存
			g_aMouseExecLastTime[nCntMouse] = g_aMouseCurrentTime[nCntMouse];

			// マウスのリピート情報を保存
			g_aMouseStateRepeat[nCntMouse] = mouseState;
		}
		else
		{// キーが入力されていない、もしくは現在の時間と最後に真を返した時間の差がリピートの間隔を越えていない時、
			// マウスのリピート情報を保存
			g_aMouseStateRepeat[nCntMouse] = 0;
		}

		// マウスのプレス情報を保存
		g_aMouseState[nCntMouse] = mouseState;

		if (mouseState)
		{// 入力が行われた時、
			// 動的な入力の種類をキーボードにする
			g_activeInputType = ACTIVE_INPUT_TYPE_KEYBOARD;
		}
	}
}

//========================================
// UpdateCursor関数 - カーソルの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateCursor(void)
{
	if (g_cursorPosTemp != GetCursorPosOnScreen()) 
	{// カーソルの現在位置が保存位置を異なる時、
		// 動的な入力の種類をキーボードにする
		g_activeInputType = ACTIVE_INPUT_TYPE_KEYBOARD;
	}

	// カーソルの移動量を設定
	g_cursorMove = GetCursorPosOnScreen() - g_cursorPosTemp;

	// カーソル位置を保存
	g_cursorPosTemp = GetCursorPosOnScreen();
}

//========================================
// UpdateButton関数 - ボタンの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateButton(void)
{
	bool	aButton	// ボタンの入力情報
			[BUTTON_MAX] 
			= { false };

	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		if (nCntButton == BUTTON_LEFT_TRIGGER)
		{// カウントが左トリガーの時、
			if (GetXInputState()->Gamepad.bLeftTrigger > 0)
			{// 押し込まれている
				aButton[nCntButton] = true;
			}
		}
		else if (nCntButton == BUTTON_RIGHT_TRIGGER)
		{// カウントが右トリガーの時、
			if (GetXInputState()->Gamepad.bRightTrigger > 0)
			{// 押し込まれている
				aButton[nCntButton] = true;
			}
		}
		else if (GetXInputState()->Gamepad.wButtons & g_aButtonMask[nCntButton])
		{// 入力情報を更新
			aButton[nCntButton] = true;
		}

		// ボタンのトリガー情報を保存
		g_aButtonTrigger[nCntButton] = (g_aButtonPress[nCntButton] ^ aButton[nCntButton])&aButton[nCntButton];

		// ボタンのリリース情報を保存
		g_aButtonRelease[nCntButton] = (g_aButtonPress[nCntButton] ^ aButton[nCntButton])&~aButton[nCntButton];

		// 現在の時間を取得
		g_aButtonCurrentTime[nCntButton] = timeGetTime();

		if (aButton[nCntButton] && ((g_aButtonCurrentTime[nCntButton] - g_aButtonExecLastTime[nCntButton]) > REPEATE_INTERVAL))
		{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
			// 最後に真を返した時間を保存
			g_aButtonExecLastTime[nCntButton] = g_aButtonCurrentTime[nCntButton];

			// ボタンのリピート情報を保存
			g_aButtonRepeat[nCntButton] = aButton[nCntButton];
		}
		else
		{
			// ボタンのリピート情報を保存
			g_aButtonRepeat[nCntButton] = 0;
		}

		// ボタンのプレス情報を保存
		g_aButtonPress[nCntButton] = aButton[nCntButton];

		if (aButton[nCntButton])
		{// 入力が行われた時、
			// 動的な入力の種類をコントローラーにする
			g_activeInputType = ACTIVE_INPUT_TYPE_CONTROLLER;
		}
	}
}

//========================================
// UpdateStick関数 - スティックの更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateStick(void)
{
	bool	bAngle	// スティックの入力情報
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];

	// XInputの状態を取得
	g_xInputState.dwPacketNumber = XInputGetState(g_xInputState.dwPacketNumber, &g_xInputState);

	for (int nCntStick = 0; nCntStick < STICK_TYPE_MAX; nCntStick++)
	{
		float X;	// スティックのX軸
		float Y;	// スティックのY軸

		// 種類に応じたスティックの軸の値を取得
		switch (nCntStick)
		{
			//========== *** 左スティック ***
		case STICK_TYPE_LEFT:
			X = GetXInputState()->Gamepad.sThumbLX;
			Y = GetXInputState()->Gamepad.sThumbLY;
			break;
			//========== *** 右スティック ***
		case STICK_TYPE_RIGHT:
			X = GetXInputState()->Gamepad.sThumbRX;
			Y = GetXInputState()->Gamepad.sThumbRY;
			break;
		}

		// 角度を取得
		g_stick.aAngle[nCntStick] = FindAngle(D3DXVECTOR3(X, Y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * -1;

		// スティックの倒し具合を取得
		g_stick.aTplDiameter[nCntStick] = fabsf(X);
		if (g_stick.aTplDiameter[nCntStick] < fabsf(Y)) {
			g_stick.aTplDiameter[nCntStick] = fabsf(Y);
		}
		g_stick.aTplDiameter[nCntStick] /= 32768.0f;

		// 方向入力フラグを初期化
		for (int nCntAngle = 0; nCntAngle < STICK_ANGLE_MAX; nCntAngle++)
		{
			bAngle[nCntStick][nCntAngle] = false;
		}

		if (g_stick.aTplDiameter[nCntStick] > 0)
		{// スティックが倒されている時、
			// 動的な入力の種類をコントローラーにする
			g_activeInputType = ACTIVE_INPUT_TYPE_CONTROLLER;

			if ((g_stick.aAngle[nCntStick] < D3DX_PI * -0.75)
				|| (g_stick.aAngle[nCntStick] > D3DX_PI * 0.75))
			{// 角度が四分割で上に位置する時、上フラグを真にする
				bAngle[nCntStick][STICK_ANGLE_UP] = true;
			}
			else if ((g_stick.aAngle[nCntStick] > D3DX_PI * -0.25)
				&& (g_stick.aAngle[nCntStick] < D3DX_PI * 0.25))
			{// 角度が四分割で下に位置する時、下フラグを真にする
				bAngle[nCntStick][STICK_ANGLE_DOWN] = true;
			}
			else if ((g_stick.aAngle[nCntStick] > D3DX_PI * -0.75)
				&& (g_stick.aAngle[nCntStick] < D3DX_PI * -0.25))
			{// 角度が四分割で左に位置する時、左フラグを真にする
				bAngle[nCntStick][STICK_ANGLE_LEFT] = true;
			}
			else if ((g_stick.aAngle[nCntStick] > D3DX_PI * 0.25)
				&& (g_stick.aAngle[nCntStick] < D3DX_PI * 0.75))
			{// 角度が四分割で右に位置する時、右フラグを真にする
				bAngle[nCntStick][STICK_ANGLE_RIGHT] = true;
			}
		}

		for (int nCntAngle = 0; nCntAngle < STICK_ANGLE_MAX; nCntAngle++)
		{
			// スティックのトリガー情報を保存
			g_stick.aAngleTrigger[nCntStick][nCntAngle] = (g_stick.aAnglePress[nCntStick][nCntAngle] ^ bAngle[nCntStick][nCntAngle])&bAngle[nCntStick][nCntAngle];

			// スティックのリリース情報を保存
			g_stick.aAngleRelease[nCntStick][nCntAngle] = (g_stick.aAnglePress[nCntStick][nCntAngle] ^ bAngle[nCntStick][nCntAngle])&~bAngle[nCntStick][nCntAngle];

			// 現在の時間を取得
			g_aStickCurrentTime[nCntStick][nCntAngle] = timeGetTime();

			if (bAngle[nCntStick][nCntAngle] && ((g_aStickCurrentTime[nCntStick][nCntAngle] - g_aStickExecLastTime[nCntStick][nCntAngle]) > REPEATE_INTERVAL))
			{// キーが入力されていて、かつ現在の時間と最後に真を返した時間の差がリピートの間隔を越えていた時、
				// 最後に真を返した時間を保存
				g_aStickExecLastTime[nCntStick][nCntAngle] = g_aStickCurrentTime[nCntStick][nCntAngle];

				// スティックのリピート情報を保存
				g_stick.aAngleRepeat[nCntStick][nCntAngle] = bAngle[nCntStick][nCntAngle];
			}
			else
			{
				// スティックのリピート情報を保存
				g_stick.aAngleRepeat[nCntStick][nCntAngle] = 0;
			}

			// スティックのプレス情報を保存
			g_stick.aAnglePress[nCntStick][nCntAngle] = bAngle[nCntStick][nCntAngle];

			if (bAngle[nCntStick][nCntAngle])
			{// 入力が行われた時、
				// 動的な入力の種類をコントローラーにする
				g_activeInputType = ACTIVE_INPUT_TYPE_CONTROLLER;
			}
		}
	}
}

//========================================
// UpdateVibration関数 - コントローラーの振動更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateVibration(void) 
{
	if (g_fVibration > 0)
	{// 振動倍率が0を上回っている時、
		// 振動倍率を減算
		g_fVibration -= VIBRATION_SUBTRACT;

		// 振動倍率を制御
		FloatControl(&g_fVibration, 1.0f, 0);

		{// 振動フラグが真の時、
			XINPUT_STATE state;			//XINPUT_STATE型 stateを作成
			XInputGetState(0, &state);	//stateに習得したコントローラの状態を格納
			XINPUT_VIBRATION vibration;	//XINPUT_VIBRATION vibrationを作成

			//vibrationメンバーwLeftMotorSpeedに値を代入 モータースピード100%
			vibration.wLeftMotorSpeed = 65535 * g_fVibration;

			//vibrationメンバーwRightMotorSpeedに値を代入 モータースピード0%
			vibration.wRightMotorSpeed = 65535 * g_fVibration;

			//ジョイスティックにバイブレーターデータを送信
			XInputSetState(0, &vibration);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitInput関数 - 入力関連の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// マウスカーソルを表示
	ShowCursor(true);

	// コントローラーの振動カウンターを初期化
	g_nCounterVibration = 0;

	// 振動倍率を初期化
	g_fVibration = 0;

	// カーソル位置を保存
	g_cursorPosTemp = GetCursorPosOnScreen();

	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) 
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調フォーマット
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//========================================
// UninitInput関数 - 入力関連の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitInput(void)
{
	// XInputを閉じる
	XInputEnable(false);

	// 入力デバイス(キーボード)の破棄
	if (g_pDevKeyboard != NULL) 
	{
		g_pDevKeyboard->Unacquire();	// キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInputオブジェクトの破棄
	if (g_pInput != NULL) 
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//========================================
// UpdateInput関数 - 入力関連の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateInput(bool bPause)
{
	UpdateKeyboard();		// キーボード
	UpdateButton();			// ボタン

	if (!bPause)
	{// ポーズフラグが偽の時、
		// 更新処理
		UpdateCursor();		// カーソル
		UpdateMouse();		// マウス
		UpdateStick();		// スティック
		UpdateVibration();	// コントローラーの振動
	}
}

//========================================
// GetKeyboardPress関数 - キーボードのプレス情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//========================================
// GetKeyboardTrigger関数 - キーボードのトリガー情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================
// GetKeyboardRepeat関数 - キーボードのリピート情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//========================================
// GetKeyboardRelease関数 - キーボードのリリース情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================
// GetMouse関数 - マウスのプレス情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetMousePress(int nMouse)
{
	return g_aMouseState[nMouse];
}

//========================================
// GetMouseTrigger関数 - マウスのトリガー情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseTrigger(int nMouse)
{
	return g_aMouseStateTrigger[nMouse];
}

//========================================
// GetMouseRepeat関数 - マウスのリピート情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseRepeat(int nMouse)
{
	return g_aMouseStateRepeat[nMouse];
}

//========================================
// GetMouseRelease関数 - マウスのリリース情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseRelease(int nMouse)
{
	return g_aMouseStateRelease[nMouse];
}

//========================================
// GetButtonPress関数 - ボタンのプレス情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonPress(int nButton)
{
	return g_aButtonPress[nButton];
}

//========================================
// GetButtonTrigger関数 - ボタンのトリガー情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonTrigger(int nButton)
{
	return g_aButtonTrigger[nButton];
}

//========================================
// GetButtonRepeat関数 - ボタンのリピート情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonRepeat(int nButton)
{
	return g_aButtonRepeat[nButton];
}

//========================================
// GetButtonRelease関数 - ボタンのリリース情報を取得 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonRelease(int nButton)
{
	return g_aButtonRelease[nButton];
}

//========================================
// SetVibration関数 - コントローラーの振動設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetVibration(float fVibration, int nTime)
{
	if (g_activeInputType != ACTIVE_INPUT_TYPE_CONTROLLER) 
	{// 動的な入力の種類がコントローラーで無い時、
		// 処理を終了
		return;
	}

	if (fVibration > g_fVibration)
	{// 振動倍率が元の振動倍率を上回っている時、
		// 振動倍率を代入
		g_fVibration = fVibration;
	}

	// 振動倍率を制御
	FloatControl(&g_fVibration, 1.0f, 0);
}
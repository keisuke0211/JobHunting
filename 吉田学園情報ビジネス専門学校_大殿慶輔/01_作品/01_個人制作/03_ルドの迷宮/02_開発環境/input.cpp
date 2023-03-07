//◇====================◇
//｜　　　　　　　　　　｜
//｜　キーボードの処理  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"input.h"

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_KEY_MAX (256)		//キーの最大数
#define NUM_BUTTON_MAX (14)		//ボタンの最大数
#define REPEATE_INTERVAL	(160)

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECTINPUT8 g_pInput = NULL;				//DirectInoutオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;	//入力デバイス(キーボード)へのポインタ

/* キーボード */
BYTE  g_aKeyStatePress[NUM_KEY_MAX];		// キーボードのプレス情報
BYTE  g_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報
BYTE  g_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート情報
BYTE  g_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報
DWORD g_aKeyCurrentTime[NUM_KEY_MAX];		// キーボードの現在の時間
DWORD g_aKeyExecLastTime[NUM_KEY_MAX];		// キーボードの最後に真を返した時間

/* カーソル */
D3DXVECTOR3	g_cursorPosTemp;				// カーソルの位置を保存
D3DXVECTOR3	g_cursorMove;					// カーソルの移動量

/* マウス */
BYTE	g_aMouseState[MOUSE_MAX];			// マウスのプレス情報
BYTE	g_aMouseStateTrigger[MOUSE_MAX];	// マウスのトリガー情報
BYTE	g_aMouseStateRelease[MOUSE_MAX];	// マウスのリリース情報
BYTE	g_aMouseStateRepeat[MOUSE_MAX];		// マウスのリピート情報
DWORD	g_aMouseCurrentTime[MOUSE_MAX];		// マウスの現在の時間
DWORD	g_aMouseExecLastTime[MOUSE_MAX];	// マウスの最後に真を返した時間

// マウスのマスク
const int g_aMouseMask[MOUSE_MAX] =
{
	VK_LBUTTON,
	VK_RBUTTON,
};


XINPUT_STATE xInput;
ACTIVE_INPUT_TYPE	g_activeInputType;		// 動的な入力の種類

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

//========================================
// 入力関連の初期化処理
//========================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// マウスカーソルを表示
	ShowCursor(false);

	// カーソル位置を保存
	/*g_cursorPosTemp = GetCursorPosOnScreen();*/

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
// 入力関連の終了処理
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
// 入力関連の更新処理
//========================================
void UpdateInput(void)
{
	UpdateKeyboard();	// キーボード

	 // 更新処理
	UpdateCursor();		// カーソル
	UpdateMouse();		// マウス
}

//========================================
// キーボードの更新処理 
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
// カーソルの更新処理 
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
// マウスの更新処理 
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
// GetStick関数 - スティックの入力情報を取得 -
//========================================
XINPUT_STATE *GetXInputState(void)
{
	return &xInput;
}
//========================================
// キーボードのプレス情報を取得
//========================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//========================================
// キーボードのトリガー情報を取得
//========================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================
// キーボードのリピート情報を取得 -
//========================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//========================================
// キーボードのリリース情報を取得
//========================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================
// マウスのプレス情報を取得 -
//========================================
bool GetMousePress(int nMouse)
{
	return g_aMouseState[nMouse];
}

//========================================
// マウスのトリガー情報を取得
//========================================
bool GetMouseTrigger(int nMouse)
{
	return g_aMouseStateTrigger[nMouse];
}

//========================================
// マウスのリピート情報を取得
//========================================
bool GetMouseRepeat(int nMouse)
{
	return g_aMouseStateRepeat[nMouse];
}

//========================================
// マウスのリリース情報を取得
//========================================
bool GetMouseRelease(int nMouse)
{
	return g_aMouseStateRelease[nMouse];
}
//========================================
// GetStick関数 - スティックの入力情報を取得 -
//========================================
STICK_INPUT GetStick(STICK_TYPE type)
{
	STICK_INPUT stick;	// スティックの入力情報
	float X = 0;			// スティックのX軸
	float Y = 0;			// スティックのY軸
						// 種類に応じたスティックの軸の値を取得
	switch (type)
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

	stick.fAngle = atan2f(-Y, -X);
	stick.fAngle -= (D3DX_PI * 0.5f);

	// スティックの倒し具合を取得
	stick.fTplDiameter = fabsf(X);
	if (stick.fTplDiameter < fabsf(Y)) {
		stick.fTplDiameter = fabsf(Y);
	}
	stick.fTplDiameter /= 32768.0f;
	return stick;
}
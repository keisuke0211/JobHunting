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

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECTINPUT8 g_pInput = NULL;//DirectInoutオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;//入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX]; //キーボードのプレス情報
BYTE g_aKeyStateTrigger[NUM_KEY_MAX]; //キーボードのトリガー情報

XINPUT_STATE xInput;
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【キーボード・処理】｜―――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇====================================◇
//｜　　　 キーボードの初期化処理　　 　｜
//◇====================================◇
HRESULT lnitKeyboard(HINSTANCE hlnstance, HWND hWnd)
{

	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hlnstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pInput, NULL)))

	{
		return E_FAIL;
	}
	//入力デバイスの生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))

	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//◇==================================◇
//｜　　　 キーボードの終了処理　　 　｜
//◇==================================◇
void UninitKeyboard(void)
{
	//入力デバイス（キーボード）の破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire(); //キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}
//◇==================================◇
//｜　　　 キーボードの更新処理　　 　｜
//◇==================================◇
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];//キーボードの入力情報
	int nCntKey;

	xInput.dwPacketNumber = XInputGetState(xInput.dwPacketNumber, &xInput);

	//入力デバイスからデータを所得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState),&aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];//キーボードのトリガー情報を保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];//キーボードのプレス情報を保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire(); //キーボードのへのアクセス権を獲得7
	}

}
//キーボードのトリガー情報を所得
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0X80) ? true : false;
}
//キーボードのブレス情報
bool GetKeyboardPress(int nKey)
{
	//        ↓３項演算子　　↓条件式? A:B; 条件式が真ならAを返す　偽ならBを返す
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}
XINPUT_STATE *GetXInputState(void)
{
	return &xInput;
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
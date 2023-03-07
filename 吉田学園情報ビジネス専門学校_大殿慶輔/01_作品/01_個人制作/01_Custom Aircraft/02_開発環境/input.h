//◇====================◇
//｜　　　　　　　　　　｜
//｜　input.h           ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
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
// スティック番号の定義
//****************************************

typedef enum
{
	STICK_TYPE_LEFT = 0,
	STICK_TYPE_RIGHT,
	STICK_TYPE_MAX
}STICK_TYPE;

//****************************************
// プロトタイプ宣言
//****************************************
HRESULT lnitKeyboard(HINSTANCE hlnstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
void UpdateButton(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetButtonPress(int nButton);
bool GetButtonTrigger(int nButton);
XINPUT_STATE *GetXInputState(void);
STICK_INPUT GetStick(STICK_TYPE type);

#endif
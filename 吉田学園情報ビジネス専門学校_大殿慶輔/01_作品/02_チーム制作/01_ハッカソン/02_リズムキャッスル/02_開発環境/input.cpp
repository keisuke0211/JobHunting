//========================================
// 
// ���͊֘A�̏���
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
// �}�N����`
//****************************************
// �L�[�̍ő吔
// ���s�[�g�̊Ԋu
#define NUM_KEY_MAX			(256)
#define REPEATE_INTERVAL	(160)

// �U���̌��Z��
#define VIBRATION_SUBTRACT	(0.02f)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �L�[�{�[�h�̍X�V����
void UpdateKeyboard(void);

// �}�E�X�̍X�V����
void UpdateMouse(void);

// �J�[�\���̍X�V����
void UpdateCursor(void);

// �{�^���̍X�V����
void UpdateButton(void);

// �X�e�B�b�N�̍X�V����
void UpdateStick(void);

// �R���g���[���[�̐U���̍X�V����
void UpdateVibration(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECTINPUT8			g_pInput				// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
						= NULL;
LPDIRECTINPUTDEVICE8	g_pDevKeyboard			// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
						= NULL;
BYTE					g_aKeyStatePress		// �L�[�{�[�h�̃v���X���
						[NUM_KEY_MAX];
BYTE					g_aKeyStateTrigger		// �L�[�{�[�h�̃g���K�[���
						[NUM_KEY_MAX];
BYTE					g_aKeyStateRelease		// �L�[�{�[�h�̃����[�X���
						[NUM_KEY_MAX];
BYTE					g_aKeyStateRepeat		// �L�[�{�[�h�̃��s�[�g���
						[NUM_KEY_MAX];
DWORD					g_aKeyCurrentTime		// �L�[�{�[�h�̌��݂̎���
						[NUM_KEY_MAX];
DWORD					g_aKeyExecLastTime		// �L�[�{�[�h�̍Ō�ɐ^��Ԃ�������
						[NUM_KEY_MAX];
BYTE					g_aMouseState			// �}�E�X�̃v���X���
						[MOUSE_MAX];
BYTE					g_aMouseStateTrigger	// �}�E�X�̃g���K�[���
						[MOUSE_MAX];
BYTE					g_aMouseStateRelease	// �}�E�X�̃����[�X���
						[MOUSE_MAX];
BYTE					g_aMouseStateRepeat		// �}�E�X�̃��s�[�g���
						[MOUSE_MAX];
DWORD					g_aMouseCurrentTime		// �}�E�X�̌��݂̎���
						[MOUSE_MAX];
DWORD					g_aMouseExecLastTime	// �}�E�X�̍Ō�ɐ^��Ԃ�������
						[MOUSE_MAX];
D3DXVECTOR3				g_cursorPosTemp;		// �J�[�\���̈ʒu��ۑ�
D3DXVECTOR3				g_cursorMove;			// �J�[�\���̈ړ���
bool					g_aButtonPress			// �{�^���̃v���X���
						[BUTTON_MAX];
bool					g_aButtonTrigger		// �{�^���̃g���K�[���
						[BUTTON_MAX];
bool					g_aButtonRelease		// �{�^���̃����[�X���
						[BUTTON_MAX];
bool					g_aButtonRepeat			// �{�^���̃��s�[�g���
						[BUTTON_MAX];
DWORD					g_aButtonCurrentTime	// �{�^���̌��݂̎���
						[BUTTON_MAX];
DWORD					g_aButtonExecLastTime	// �{�^���̍Ō�ɐ^��Ԃ�������
						[BUTTON_MAX];
STICK_INPUT				g_stick;				// �X�e�B�b�N�̓��͏��
DWORD					g_aStickCurrentTime		// �X�e�B�b�N�̌��݂̎���
						[STICK_TYPE_MAX][STICK_ANGLE_MAX];
DWORD					g_aStickExecLastTime	// �X�e�B�b�N�̍Ō�ɐ^��Ԃ�������
						[STICK_TYPE_MAX][STICK_ANGLE_MAX];
int						g_nCounterVibration;	// �R���g���[���[�̐U���J�E���^�[
int						g_nCounterVibrationMax;	// �R���g���[���[�̐U���J�E���^�[�̍ő�l��ۑ�
float					g_fVibration;			// �U���{��
XINPUT_STATE			g_xInputState;			// XInput�̏��
ACTIVE_INPUT_TYPE		g_activeInputType;		// ���I�ȓ��͂̎��

// �}�E�X�̃}�X�N
const int g_aMouseMask[MOUSE_MAX] =
{
	VK_LBUTTON,
	VK_RBUTTON,
};

// �{�^���̃}�X�N
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

//========== *** ��ʏ�̃J�[�\���̈ʒu���擾 ***
D3DXVECTOR3 GetCursorPosOnScreen(void)
{
	POINT cursorPos;	// �J�[�\���̈ʒu

	// �J�[�\���̌��݈ʒu���擾
	GetCursorPos(&cursorPos);

	RECT rc;	// �N���C�A���g�̈�̃T�C�Y

	// �N���C�A���g�̈�̃T�C�Y���擾
	GetClientRect(*GetWindowHandle(), &rc);

	return D3DXVECTOR3(
		(cursorPos.x - GetWindowPos().x) * (SCREEN_WIDTH / (float)rc.right),
		(cursorPos.y - GetWindowPos().y) * (SCREEN_HEIGHT / (float)rc.bottom),
		0.0f);
}

//========== *** �J�[�\���̈ړ��ʂ��擾 ***
D3DXVECTOR3 GetCursorMove(void) 
{
	return g_cursorMove;
}

//========== *** XInput�̏�Ԃ��擾 ***
XINPUT_STATE *GetXInputState(void) 
{
	return &g_xInputState;
}

//========== *** �X�e�B�b�N�̓��͏����擾 ***
STICK_INPUT GetStick(void)
{
	return g_stick;
}

//========== *** ���I�ȓ��͂̎�ނ��擾 ***
ACTIVE_INPUT_TYPE GetActiveInputType() 
{
	return g_activeInputType;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateKeyboard�֐� - �L�[�{�[�h�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateKeyboard(void)
{
	BYTE	aKeyState	// �L�[�{�[�h�̓��͏��
			[NUM_KEY_MAX];

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			// �L�[�{�[�h�̃g���K�[����ۑ�
			g_aKeyStateTrigger[nCntKey] = (g_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];

			// �L�[�{�[�h�̃����[�X����ۑ�
			g_aKeyStateRelease[nCntKey] = (g_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey])&~aKeyState[nCntKey];

			// ���݂̎��Ԃ��擾
			g_aKeyCurrentTime[nCntKey] = timeGetTime();

			if (aKeyState[nCntKey] && ((g_aKeyCurrentTime[nCntKey] - g_aKeyExecLastTime[nCntKey]) > REPEATE_INTERVAL))
			{// �L�[�����͂���Ă��āA�����݂̎��ԂƍŌ�ɐ^��Ԃ������Ԃ̍������s�[�g�̊Ԋu���z���Ă������A
				// �Ō�ɐ^��Ԃ������Ԃ�ۑ�
				g_aKeyExecLastTime[nCntKey] = g_aKeyCurrentTime[nCntKey];

				// �L�[�{�[�h�̃��s�[�g����ۑ�
				g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}
			else
			{
				// �L�[�{�[�h�̃��s�[�g����ۑ�
				g_aKeyStateRepeat[nCntKey] = 0;
			}

			// �L�[�{�[�h�̃v���X����ۑ�
			g_aKeyStatePress[nCntKey] = aKeyState[nCntKey];

			if (aKeyState[nCntKey])
			{// ���͂��s��ꂽ���A
				// ���I�ȓ��͂̎�ނ��L�[�{�[�h�ɂ���
				g_activeInputType = ACTIVE_INPUT_TYPE_KEYBOARD;
			}
		}
	}
	else
	{// ���̓f�o�C�X����f�[�^���擾�ł��Ȃ��������A
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}
}

//========================================
// UpdateMouse�֐� - �}�E�X�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMouse(void)
{
	for (int nCntMouse = 0; nCntMouse < MOUSE_MAX; nCntMouse++)
	{
		BYTE	mouseState	// �}�E�X�̓��͏��
				= GetKeyState(g_aMouseMask[nCntMouse]) & (0x80) ? true : false;;

		// �}�E�X�̃g���K�[����ۑ�
		g_aMouseStateTrigger[nCntMouse] = (g_aMouseState[nCntMouse] ^ mouseState)&mouseState;

		// �}�E�X�̃����[�X����ۑ�
		g_aMouseStateRelease[nCntMouse] = (g_aMouseState[nCntMouse] ^ mouseState)&~mouseState;

		// ���݂̎��Ԃ��擾
		g_aMouseCurrentTime[nCntMouse] = timeGetTime();

		if (mouseState && ((g_aMouseCurrentTime[nCntMouse] - g_aMouseExecLastTime[nCntMouse]) > REPEATE_INTERVAL))
		{// �L�[�����͂���Ă��āA�����݂̎��ԂƍŌ�ɐ^��Ԃ������Ԃ̍������s�[�g�̊Ԋu���z���Ă������A
			// �Ō�ɐ^��Ԃ������Ԃ�ۑ�
			g_aMouseExecLastTime[nCntMouse] = g_aMouseCurrentTime[nCntMouse];

			// �}�E�X�̃��s�[�g����ۑ�
			g_aMouseStateRepeat[nCntMouse] = mouseState;
		}
		else
		{// �L�[�����͂���Ă��Ȃ��A�������͌��݂̎��ԂƍŌ�ɐ^��Ԃ������Ԃ̍������s�[�g�̊Ԋu���z���Ă��Ȃ����A
			// �}�E�X�̃��s�[�g����ۑ�
			g_aMouseStateRepeat[nCntMouse] = 0;
		}

		// �}�E�X�̃v���X����ۑ�
		g_aMouseState[nCntMouse] = mouseState;

		if (mouseState)
		{// ���͂��s��ꂽ���A
			// ���I�ȓ��͂̎�ނ��L�[�{�[�h�ɂ���
			g_activeInputType = ACTIVE_INPUT_TYPE_KEYBOARD;
		}
	}
}

//========================================
// UpdateCursor�֐� - �J�[�\���̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateCursor(void)
{
	if (g_cursorPosTemp != GetCursorPosOnScreen()) 
	{// �J�[�\���̌��݈ʒu���ۑ��ʒu���قȂ鎞�A
		// ���I�ȓ��͂̎�ނ��L�[�{�[�h�ɂ���
		g_activeInputType = ACTIVE_INPUT_TYPE_KEYBOARD;
	}

	// �J�[�\���̈ړ��ʂ�ݒ�
	g_cursorMove = GetCursorPosOnScreen() - g_cursorPosTemp;

	// �J�[�\���ʒu��ۑ�
	g_cursorPosTemp = GetCursorPosOnScreen();
}

//========================================
// UpdateButton�֐� - �{�^���̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateButton(void)
{
	bool	aButton	// �{�^���̓��͏��
			[BUTTON_MAX] 
			= { false };

	for (int nCntButton = 0; nCntButton < BUTTON_MAX; nCntButton++)
	{
		if (nCntButton == BUTTON_LEFT_TRIGGER)
		{// �J�E���g�����g���K�[�̎��A
			if (GetXInputState()->Gamepad.bLeftTrigger > 0)
			{// �������܂�Ă���
				aButton[nCntButton] = true;
			}
		}
		else if (nCntButton == BUTTON_RIGHT_TRIGGER)
		{// �J�E���g���E�g���K�[�̎��A
			if (GetXInputState()->Gamepad.bRightTrigger > 0)
			{// �������܂�Ă���
				aButton[nCntButton] = true;
			}
		}
		else if (GetXInputState()->Gamepad.wButtons & g_aButtonMask[nCntButton])
		{// ���͏����X�V
			aButton[nCntButton] = true;
		}

		// �{�^���̃g���K�[����ۑ�
		g_aButtonTrigger[nCntButton] = (g_aButtonPress[nCntButton] ^ aButton[nCntButton])&aButton[nCntButton];

		// �{�^���̃����[�X����ۑ�
		g_aButtonRelease[nCntButton] = (g_aButtonPress[nCntButton] ^ aButton[nCntButton])&~aButton[nCntButton];

		// ���݂̎��Ԃ��擾
		g_aButtonCurrentTime[nCntButton] = timeGetTime();

		if (aButton[nCntButton] && ((g_aButtonCurrentTime[nCntButton] - g_aButtonExecLastTime[nCntButton]) > REPEATE_INTERVAL))
		{// �L�[�����͂���Ă��āA�����݂̎��ԂƍŌ�ɐ^��Ԃ������Ԃ̍������s�[�g�̊Ԋu���z���Ă������A
			// �Ō�ɐ^��Ԃ������Ԃ�ۑ�
			g_aButtonExecLastTime[nCntButton] = g_aButtonCurrentTime[nCntButton];

			// �{�^���̃��s�[�g����ۑ�
			g_aButtonRepeat[nCntButton] = aButton[nCntButton];
		}
		else
		{
			// �{�^���̃��s�[�g����ۑ�
			g_aButtonRepeat[nCntButton] = 0;
		}

		// �{�^���̃v���X����ۑ�
		g_aButtonPress[nCntButton] = aButton[nCntButton];

		if (aButton[nCntButton])
		{// ���͂��s��ꂽ���A
			// ���I�ȓ��͂̎�ނ��R���g���[���[�ɂ���
			g_activeInputType = ACTIVE_INPUT_TYPE_CONTROLLER;
		}
	}
}

//========================================
// UpdateStick�֐� - �X�e�B�b�N�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateStick(void)
{
	bool	bAngle	// �X�e�B�b�N�̓��͏��
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];

	// XInput�̏�Ԃ��擾
	g_xInputState.dwPacketNumber = XInputGetState(g_xInputState.dwPacketNumber, &g_xInputState);

	for (int nCntStick = 0; nCntStick < STICK_TYPE_MAX; nCntStick++)
	{
		float X;	// �X�e�B�b�N��X��
		float Y;	// �X�e�B�b�N��Y��

		// ��ނɉ������X�e�B�b�N�̎��̒l���擾
		switch (nCntStick)
		{
			//========== *** ���X�e�B�b�N ***
		case STICK_TYPE_LEFT:
			X = GetXInputState()->Gamepad.sThumbLX;
			Y = GetXInputState()->Gamepad.sThumbLY;
			break;
			//========== *** �E�X�e�B�b�N ***
		case STICK_TYPE_RIGHT:
			X = GetXInputState()->Gamepad.sThumbRX;
			Y = GetXInputState()->Gamepad.sThumbRY;
			break;
		}

		// �p�x���擾
		g_stick.aAngle[nCntStick] = FindAngle(D3DXVECTOR3(X, Y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f)) * -1;

		// �X�e�B�b�N�̓|������擾
		g_stick.aTplDiameter[nCntStick] = fabsf(X);
		if (g_stick.aTplDiameter[nCntStick] < fabsf(Y)) {
			g_stick.aTplDiameter[nCntStick] = fabsf(Y);
		}
		g_stick.aTplDiameter[nCntStick] /= 32768.0f;

		// �������̓t���O��������
		for (int nCntAngle = 0; nCntAngle < STICK_ANGLE_MAX; nCntAngle++)
		{
			bAngle[nCntStick][nCntAngle] = false;
		}

		if (g_stick.aTplDiameter[nCntStick] > 0)
		{// �X�e�B�b�N���|����Ă��鎞�A
			// ���I�ȓ��͂̎�ނ��R���g���[���[�ɂ���
			g_activeInputType = ACTIVE_INPUT_TYPE_CONTROLLER;

			if ((g_stick.aAngle[nCntStick] < D3DX_PI * -0.75)
				|| (g_stick.aAngle[nCntStick] > D3DX_PI * 0.75))
			{// �p�x���l�����ŏ�Ɉʒu���鎞�A��t���O��^�ɂ���
				bAngle[nCntStick][STICK_ANGLE_UP] = true;
			}
			else if ((g_stick.aAngle[nCntStick] > D3DX_PI * -0.25)
				&& (g_stick.aAngle[nCntStick] < D3DX_PI * 0.25))
			{// �p�x���l�����ŉ��Ɉʒu���鎞�A���t���O��^�ɂ���
				bAngle[nCntStick][STICK_ANGLE_DOWN] = true;
			}
			else if ((g_stick.aAngle[nCntStick] > D3DX_PI * -0.75)
				&& (g_stick.aAngle[nCntStick] < D3DX_PI * -0.25))
			{// �p�x���l�����ō��Ɉʒu���鎞�A���t���O��^�ɂ���
				bAngle[nCntStick][STICK_ANGLE_LEFT] = true;
			}
			else if ((g_stick.aAngle[nCntStick] > D3DX_PI * 0.25)
				&& (g_stick.aAngle[nCntStick] < D3DX_PI * 0.75))
			{// �p�x���l�����ŉE�Ɉʒu���鎞�A�E�t���O��^�ɂ���
				bAngle[nCntStick][STICK_ANGLE_RIGHT] = true;
			}
		}

		for (int nCntAngle = 0; nCntAngle < STICK_ANGLE_MAX; nCntAngle++)
		{
			// �X�e�B�b�N�̃g���K�[����ۑ�
			g_stick.aAngleTrigger[nCntStick][nCntAngle] = (g_stick.aAnglePress[nCntStick][nCntAngle] ^ bAngle[nCntStick][nCntAngle])&bAngle[nCntStick][nCntAngle];

			// �X�e�B�b�N�̃����[�X����ۑ�
			g_stick.aAngleRelease[nCntStick][nCntAngle] = (g_stick.aAnglePress[nCntStick][nCntAngle] ^ bAngle[nCntStick][nCntAngle])&~bAngle[nCntStick][nCntAngle];

			// ���݂̎��Ԃ��擾
			g_aStickCurrentTime[nCntStick][nCntAngle] = timeGetTime();

			if (bAngle[nCntStick][nCntAngle] && ((g_aStickCurrentTime[nCntStick][nCntAngle] - g_aStickExecLastTime[nCntStick][nCntAngle]) > REPEATE_INTERVAL))
			{// �L�[�����͂���Ă��āA�����݂̎��ԂƍŌ�ɐ^��Ԃ������Ԃ̍������s�[�g�̊Ԋu���z���Ă������A
				// �Ō�ɐ^��Ԃ������Ԃ�ۑ�
				g_aStickExecLastTime[nCntStick][nCntAngle] = g_aStickCurrentTime[nCntStick][nCntAngle];

				// �X�e�B�b�N�̃��s�[�g����ۑ�
				g_stick.aAngleRepeat[nCntStick][nCntAngle] = bAngle[nCntStick][nCntAngle];
			}
			else
			{
				// �X�e�B�b�N�̃��s�[�g����ۑ�
				g_stick.aAngleRepeat[nCntStick][nCntAngle] = 0;
			}

			// �X�e�B�b�N�̃v���X����ۑ�
			g_stick.aAnglePress[nCntStick][nCntAngle] = bAngle[nCntStick][nCntAngle];

			if (bAngle[nCntStick][nCntAngle])
			{// ���͂��s��ꂽ���A
				// ���I�ȓ��͂̎�ނ��R���g���[���[�ɂ���
				g_activeInputType = ACTIVE_INPUT_TYPE_CONTROLLER;
			}
		}
	}
}

//========================================
// UpdateVibration�֐� - �R���g���[���[�̐U���X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateVibration(void) 
{
	if (g_fVibration > 0)
	{// �U���{����0�������Ă��鎞�A
		// �U���{�������Z
		g_fVibration -= VIBRATION_SUBTRACT;

		// �U���{���𐧌�
		FloatControl(&g_fVibration, 1.0f, 0);

		{// �U���t���O���^�̎��A
			XINPUT_STATE state;			//XINPUT_STATE�^ state���쐬
			XInputGetState(0, &state);	//state�ɏK�������R���g���[���̏�Ԃ��i�[
			XINPUT_VIBRATION vibration;	//XINPUT_VIBRATION vibration���쐬

			//vibration�����o�[wLeftMotorSpeed�ɒl���� ���[�^�[�X�s�[�h100%
			vibration.wLeftMotorSpeed = 65535 * g_fVibration;

			//vibration�����o�[wRightMotorSpeed�ɒl���� ���[�^�[�X�s�[�h0%
			vibration.wRightMotorSpeed = 65535 * g_fVibration;

			//�W���C�X�e�B�b�N�Ƀo�C�u���[�^�[�f�[�^�𑗐M
			XInputSetState(0, &vibration);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitInput�֐� - ���͊֘A�̏��������� -
// Author:RIKU NISHIMURA
//========================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// �}�E�X�J�[�\����\��
	ShowCursor(true);

	// �R���g���[���[�̐U���J�E���^�[��������
	g_nCounterVibration = 0;

	// �U���{����������
	g_fVibration = 0;

	// �J�[�\���ʒu��ۑ�
	g_cursorPosTemp = GetCursorPosOnScreen();

	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) 
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �����t�H�[�}�b�g
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//========================================
// UninitInput�֐� - ���͊֘A�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitInput(void)
{
	// XInput�����
	XInputEnable(false);

	// ���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL) 
	{
		g_pDevKeyboard->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�������
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL) 
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//========================================
// UpdateInput�֐� - ���͊֘A�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateInput(bool bPause)
{
	UpdateKeyboard();		// �L�[�{�[�h
	UpdateButton();			// �{�^��

	if (!bPause)
	{// �|�[�Y�t���O���U�̎��A
		// �X�V����
		UpdateCursor();		// �J�[�\��
		UpdateMouse();		// �}�E�X
		UpdateStick();		// �X�e�B�b�N
		UpdateVibration();	// �R���g���[���[�̐U��
	}
}

//========================================
// GetKeyboardPress�֐� - �L�[�{�[�h�̃v���X�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//========================================
// GetKeyboardTrigger�֐� - �L�[�{�[�h�̃g���K�[�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//========================================
// GetKeyboardRepeat�֐� - �L�[�{�[�h�̃��s�[�g�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//========================================
// GetKeyboardRelease�֐� - �L�[�{�[�h�̃����[�X�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//========================================
// GetMouse�֐� - �}�E�X�̃v���X�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetMousePress(int nMouse)
{
	return g_aMouseState[nMouse];
}

//========================================
// GetMouseTrigger�֐� - �}�E�X�̃g���K�[�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseTrigger(int nMouse)
{
	return g_aMouseStateTrigger[nMouse];
}

//========================================
// GetMouseRepeat�֐� - �}�E�X�̃��s�[�g�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseRepeat(int nMouse)
{
	return g_aMouseStateRepeat[nMouse];
}

//========================================
// GetMouseRelease�֐� - �}�E�X�̃����[�X�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetMouseRelease(int nMouse)
{
	return g_aMouseStateRelease[nMouse];
}

//========================================
// GetButtonPress�֐� - �{�^���̃v���X�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonPress(int nButton)
{
	return g_aButtonPress[nButton];
}

//========================================
// GetButtonTrigger�֐� - �{�^���̃g���K�[�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonTrigger(int nButton)
{
	return g_aButtonTrigger[nButton];
}

//========================================
// GetButtonRepeat�֐� - �{�^���̃��s�[�g�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonRepeat(int nButton)
{
	return g_aButtonRepeat[nButton];
}

//========================================
// GetButtonRelease�֐� - �{�^���̃����[�X�����擾 -
// Author:RIKU NISHIMURA
//========================================
bool GetButtonRelease(int nButton)
{
	return g_aButtonRelease[nButton];
}

//========================================
// SetVibration�֐� - �R���g���[���[�̐U���ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetVibration(float fVibration, int nTime)
{
	if (g_activeInputType != ACTIVE_INPUT_TYPE_CONTROLLER) 
	{// ���I�ȓ��͂̎�ނ��R���g���[���[�Ŗ������A
		// �������I��
		return;
	}

	if (fVibration > g_fVibration)
	{// �U���{�������̐U���{���������Ă��鎞�A
		// �U���{������
		g_fVibration = fVibration;
	}

	// �U���{���𐧌�
	FloatControl(&g_fVibration, 1.0f, 0);
}
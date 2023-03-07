//========================================
//  *** input.h ***
//========================================
#ifndef _INPUT_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************

// ���I�ȓ��͂̎��
typedef enum 
{
	ACTIVE_INPUT_TYPE_KEYBOARD,
	ACTIVE_INPUT_TYPE_CONTROLLER,
	ACTIVE_INPUT_TYPE_MAX,
}ACTIVE_INPUT_TYPE;

// �}�E�X�ԍ�
typedef enum
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_MAX,
}MOUSE;

// �{�^���ԍ�
typedef enum 
{
	BUTTON_UP = 0,			// ��L�[
	BUTTON_DOWN,			// ���L�[
	BUTTON_LEFT,			// ���L�[
	BUTTON_RIGHT,			// �E�L�[
	BUTTON_START,			// �X�^�[�g�L�[
	BUTTON_BACK,			// �o�b�N�L�[
	BUTTON_LEFT_THUMB,		// ���X�e�B�b�N
	BUTTON_RIGHT_THUMB,		// �E�X�e�B�b�N
	BUTTON_LEFT_SHOULDER,	// ���g���K�[
	BUTTON_RIGHT_SHOULDER,	// �E�g���K�[
	BUTTON_A,				// A�{�^��
	BUTTON_B,				// B�{�^��
	BUTTON_X,				// X�{�^��
	BUTTON_Y,				// Y�{�^��
	BUTTON_LEFT_TRIGGER,	// ���g���K�[
	BUTTON_RIGHT_TRIGGER,	// �E�g���K�[
	BUTTON_MAX,
}BUTTON;

// �X�e�B�b�N�ԍ�
typedef enum
{
	STICK_TYPE_LEFT = 0,
	STICK_TYPE_RIGHT,
	STICK_TYPE_MAX
}STICK_TYPE;

// �X�e�B�b�N�̕���
typedef enum 
{
	STICK_ANGLE_UP = 0,
	STICK_ANGLE_DOWN,
	STICK_ANGLE_LEFT,
	STICK_ANGLE_RIGHT,
	STICK_ANGLE_MAX,
}STICK_ANGLE;

//****************************************
// �\���̂̒�`
//****************************************

// �X�e�B�b�N�̓��͏��\����
typedef struct 
{
	float	aTplDiameter						// �X�e�B�b�N�̓|���
			[STICK_TYPE_MAX];					// 
	float	aAngle								// �X�e�B�b�N�̊p�x
			[STICK_TYPE_MAX];					// 
	bool	aAnglePress							// �X�e�B�b�N�̕����v���X���
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];	// 
	bool	aAngleTrigger						// �X�e�B�b�N�̕����g���K�[���
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];	// 
	bool	aAngleRepeat						// �X�e�B�b�N�̕������s�[�g���
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];	// 
	bool	aAngleRelease						// �X�e�B�b�N�̕��������[�X���
			[STICK_TYPE_MAX][STICK_ANGLE_MAX];	// 
}STICK_INPUT;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ��ʏ�̃J�[�\���̈ʒu���擾
D3DXVECTOR3 GetCursorPosOnScreen(void);

// XInput�̏�Ԃ��擾
XINPUT_STATE *GetXInputState(void);

// �X�e�B�b�N�̓��͏����擾
STICK_INPUT GetStick(void);

// ���I�ȓ��͂̎�ނ��擾
ACTIVE_INPUT_TYPE GetActiveInputType();

// ���͊֘A�̏���������
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);

// ���͊֘A�̏I������
void UninitInput(void);

// �L�[�{�[�h�̍X�V����
void UpdateKeyboard(void);

// �L�[�{�[�h�̃v���X�����擾
bool GetKeyboardPress(int nKey);

// �L�[�{�[�h�̃g���K�[�����擾
bool GetKeyboardTrigger(int nKey);

// �L�[�{�[�h�̃��s�[�g�����擾
bool GetKeyboardRepeat(int nKey);

// �L�[�{�[�h�̃����[�X�����擾
bool GetKeyboardRelease(int nKey);

// �}�E�X�̍X�V����
void UpdateMouse(void);

// �}�E�X�̃v���X�����擾
bool GetMousePress(int nMouse);

// �}�E�X�̃g���K�[�����擾
bool GetMouseTrigger(int nMouse);

// �}�E�X�̃��s�[�g�����擾
bool GetMouseRepeat(int nMouse);

// �}�E�X�̃����[�X�����擾
bool GetMouseRelease(int nMouse);

// �J�[�\���̍X�V����
void UpdateCursor(void);

// �{�^���̍X�V����
void UpdateButton(void);

// �{�^���̃v���X�����擾
bool GetButtonPress(int nButton);

// �{�^���̃g���K�[�����擾
bool GetButtonTrigger(int nButton);

// �{�^���̃��s�[�g�����擾
bool GetButtonRepeat(int nButton);

// �{�^���̃����[�X�����擾
bool GetButtonRelease(int nButton);

// �X�e�B�b�N�̍X�V����
void UpdateStick(void);

// �R���g���[���[�̐U���̍X�V����
void UpdateVibration(void);

// �R���g���[���[�̐U���ݒ菈��
void SetVibration(float fVibration, int nTime);

#endif
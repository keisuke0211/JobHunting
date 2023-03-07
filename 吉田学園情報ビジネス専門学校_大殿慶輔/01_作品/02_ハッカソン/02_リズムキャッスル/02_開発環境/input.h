//========================================
// 
// ���͊֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** input.h ***
//========================================
#ifndef _INPUT_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _INPUT_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �}�N����`
//****************************************

// �v���C���[�̍��̈ړ��̃L�[
// �v���C���[�̉E�̈ړ��̃L�[
// �v���C���[�̃W�����v�̃L�[
// �v���C���[�̂��Ⴊ�݂̃L�[
// �v���C���[�̖�̃L�[
// �v���C���[�̃��o�[�̃L�[
#define PLAYER_LEFT_MOVE_KEY		(DIK_A)
#define PLAYER_RIGHT_MOVE_KEY		(DIK_D)
#define PLAYER_JUMP_KEY				(DIK_SPACE)
#define PLAYER_SQUAT_KEY			(DIK_S)
#define PLAYER_GATE_KEY				(DIK_W)
#define PLAYER_LEVER_KEY			(DIK_W)

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
	float	aTplDiameter	// �X�e�B�b�N�̓|���
			[STICK_TYPE_MAX];
	float	aAngle			// �X�e�B�b�N�̊p�x
			[STICK_TYPE_MAX];
	bool	aAnglePress		// �X�e�B�b�N�̕����v���X���
			[STICK_TYPE_MAX]
			[STICK_ANGLE_MAX];
	bool	aAngleTrigger	// �X�e�B�b�N�̕����g���K�[���
			[STICK_TYPE_MAX]
			[STICK_ANGLE_MAX];
	bool	aAngleRepeat	// �X�e�B�b�N�̕������s�[�g���
			[STICK_TYPE_MAX]
			[STICK_ANGLE_MAX];
	bool	aAngleRelease	// �X�e�B�b�N�̕��������[�X���
			[STICK_TYPE_MAX]
			[STICK_ANGLE_MAX];
}STICK_INPUT;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// ��ʏ�̃J�[�\���̈ʒu���擾
// �J�[�\���̈ړ��ʂ��擾
// XInput�̏�Ԃ��擾
// �X�e�B�b�N�̓��͏����擾
// ���I�ȓ��͂̎�ނ��擾
D3DXVECTOR3 GetCursorPosOnScreen(void);
D3DXVECTOR3 GetCursorMove(void);
XINPUT_STATE *GetXInputState(void);
STICK_INPUT GetStick(void);
ACTIVE_INPUT_TYPE GetActiveInputType();

// ���͊֘A�̏���������
// ���͊֘A�̏I������
// ���͊֘A�̍X�V����
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(bool bPause);

// �L�[�{�[�h�̃v���X�����擾
// �L�[�{�[�h�̃g���K�[�����擾
// �L�[�{�[�h�̃��s�[�g�����擾
// �L�[�{�[�h�̃����[�X�����擾
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

// �}�E�X�̃v���X�����擾
// �}�E�X�̃g���K�[�����擾
// �}�E�X�̃��s�[�g�����擾
// �}�E�X�̃����[�X�����擾
bool GetMousePress(int nMouse);
bool GetMouseTrigger(int nMouse);
bool GetMouseRepeat(int nMouse);
bool GetMouseRelease(int nMouse);

// �{�^���̃v���X�����擾
// �{�^���̃g���K�[�����擾
// �{�^���̃��s�[�g�����擾
// �{�^���̃����[�X�����擾
bool GetButtonPress(int nButton);
bool GetButtonTrigger(int nButton);
bool GetButtonRepeat(int nButton);
bool GetButtonRelease(int nButton);

// �R���g���[���[�̐U���ݒ菈��
void SetVibration(float fVibration, int nTime);

#endif
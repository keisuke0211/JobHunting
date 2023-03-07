//========================================
// 
// MD:�`���[�g���A�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_tutorial_00.h ***
//========================================
#ifndef _MD_TUTORIAL_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_TUTORIAL_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// MD:�`���[�g���A�����[00] �̏��
typedef enum 
{
	MD_TUTORIAL_00_STATE_NONE,		// ����
	MD_TUTORIAL_00_STATE_NORMAL,	// �ʏ�
	MD_TUTORIAL_00_STATE_MAX,
}MD_TUTORIAL_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// MD:�`���[�g���A�����[00] �̏��\����
typedef struct 
{
	// �ǂݍ��ݏ��
	int			nPromptTextFont;	// ���͍Ñ��e�L�X�g�̃t�H���g
	D3DXVECTOR3 promptTextPos;		// ���͍Ñ��e�L�X�g�̈ʒu
	Color		promptTextCol;		// ���͍Ñ��e�L�X�g�̐F
	int			nWaitBlinkTime;		// �ҋ@���̓_�łɂ����鎞��
	int			nDecisionBlinkTime;	// ���莞�̓_�łɂ����鎞��
	int			nBGM;				// BGM
	int			nDecisionSE;		// ����SE

	// ��Ԋ֘A
	MD_TUTORIAL_00_STATE	state;				// MD:�`���[�g���A�����[00] �̏��
	int						nCounterState;		// ��Ԃ̃J�E���^�[
	int						nCounterPromptText;	// ���͍Ñ��e�L�X�g�̔ėp�J�E���^�[
}Md_tutorial_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�`���[�g���A�����[00] �̏����擾
Md_tutorial_00 *GetMd_tutorial_00(void);

// MD:�`���[�g���A�����[00] �̓ǂݍ��ݏ���
void LoadMd_tutorial_00(void);

// MD:�`���[�g���A�����[00] �̏���������
// MD:�`���[�g���A�����[00] �̏I������
// MD:�`���[�g���A�����[00] �̍X�V����
// MD:�`���[�g���A�����[00] �̕`�揈��
void InitMd_tutorial_00(void);
void UninitMd_tutorial_00(void);
void UpdateMd_tutorial_00(void);
void DrawMd_tutorial_00(void);

// MD:�`���[�g���A�����[00] �̏�Ԃ�ݒ�
void SetMd_tutorial_00State(MD_TUTORIAL_00_STATE state);

#endif
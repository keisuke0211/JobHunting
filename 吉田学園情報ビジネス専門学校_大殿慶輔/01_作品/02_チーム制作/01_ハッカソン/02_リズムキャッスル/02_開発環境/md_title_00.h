//========================================
// 
// MD:�^�C�g�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_title_00.h ***
//========================================
#ifndef _MD_TITLE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_TITLE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// MD:�^�C�g�����[00] �̏��
typedef enum 
{
	MD_TITLE_00_STATE_NONE,		// ����
	MD_TITLE_00_STATE_NORMAL,	// �ʏ�
	MD_TITLE_00_STATE_LOGOWAIT,	// ���S�҂�
	MD_TITLE_00_STATE_MAX,
}MD_TITLE_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// MD:�^�C�g�����[00] �̏��\����
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
	MD_TITLE_00_STATE	state;				// MD:�^�C�g�����[00] �̏��
	int					nCounterState;		// ��Ԃ̃J�E���^�[
	int					nCounterPromptText;	// ���͍Ñ��e�L�X�g�̔ėp�J�E���^�[
}Md_title_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�^�C�g�����[00] �̏����擾
Md_title_00 *GetMd_title_00(void);

// MD:�^�C�g�����[00] �̓ǂݍ��ݏ���
void LoadMd_title_00(void);

// MD:�^�C�g�����[00] �̏���������
// MD:�^�C�g�����[00] �̏I������
// MD:�^�C�g�����[00] �̍X�V����
// MD:�^�C�g�����[00] �̕`�揈��
void InitMd_title_00(void);
void UninitMd_title_00(void);
void UpdateMd_title_00(void);
void DrawMd_title_00(void);

// MD:�^�C�g�����[00] �̏�Ԃ�ݒ�
void SetMd_title_00State(MD_TITLE_00_STATE state);

#endif
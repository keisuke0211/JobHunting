//========================================
// 
// MD:���U���g���[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_result_00.h ***
//========================================
#ifndef _MD_RESULT_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_RESULT_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// MD:���U���g���[00] �̏��
typedef enum 
{
	MD_RESULT_00_STATE_NONE,		// ����
	MD_RESULT_00_STATE_NORMAL,		// �ʏ�
	MD_RESULT_00_STATE_RANKINGWAIT,	// �����L���O�҂�
	MD_RESULT_00_STATE_MAX,
}MD_RESULT_00_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// MD:���U���g���[00] �̏��\����
typedef struct 
{
	// �ǂݍ��ݏ��
	int			nPromptTextFont;	// ���͍Ñ��e�L�X�g�̃t�H���g
	D3DXVECTOR3 promptTextPos;		// ���͍Ñ��e�L�X�g�̈ʒu
	Color		promptTextCol;		// ���͍Ñ��e�L�X�g�̐F
	int			nWaitBlinkTime;		// �ҋ@���̓_�łɂ����鎞��
	int			nDecisionBlinkTime;	// ���莞�̓_�łɂ����鎞��
	int			nBGM;				// BGM
	int			nRankingSetSE;		// �����L���O�Z�b�gSE
	int			nDecisionSE;		// ����SE

	// ��Ԋ֘A
	MD_RESULT_00_STATE	state;				// MD:���U���g���[00] �̏��
	int					nCounterState;		// ��Ԃ̃J�E���^�[
	int					nCounterPromptText;	// ���͍Ñ��e�L�X�g�̔ėp�J�E���^�[
	int					nUpdateRank;		// �X�V����
	int					nCounterBlink;		// �_�ŃJ�E���^�[
}Md_result_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:���U���g���[00] �̏����擾
Md_result_00 *GetMd_result_00(void);

// MD:���U���g���[00] �̓ǂݍ��ݏ���
void LoadMd_result_00(void);

// MD:���U���g���[00] �̏���������
// MD:���U���g���[00] �̏I������
// MD:���U���g���[00] �̍X�V����
// MD:���U���g���[00] �̕`�揈��
void InitMd_result_00(void);
void UninitMd_result_00(void);
void UpdateMd_result_00(void);
void DrawMd_result_00(void);

// MD:���U���g���[00] �̏�Ԃ�ݒ�
void SetMd_result_00State(MD_RESULT_00_STATE state);

// MD:���U���g���[00] �̔w�i��ݒ�
void SetBgMd_result_00(int nBg);

#endif
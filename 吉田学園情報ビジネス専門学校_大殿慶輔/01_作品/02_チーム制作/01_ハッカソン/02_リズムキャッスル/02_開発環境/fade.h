//========================================
// *** fade.h ***
//========================================
#ifndef _FADE_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _FADE_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************

// �t�F�[�h�̏��
typedef enum 
{
	FADE_NONE = 0,	// �������Ă��Ȃ����
	FADE_IN,		// �t�F�[�h�C�����
	FADE_OUT,		// �t�F�[�h�A�E�g���
	FADE_MAX,
}FADE;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �t�F�[�h�̐ݒ菈��
void SetFade(MODE modeNext);

// �t�F�[�h�̐؂�ւ��擾
bool GetFadeSwap(void);

// �t�F�[�h�̏���������
// �t�F�[�h�̏I������
// �t�F�[�h�̍X�V����
// �t�F�[�h�̕`�揈��
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

// �t�F�[�h�̏�Ԃ��擾
FADE GetFade(void);

#endif
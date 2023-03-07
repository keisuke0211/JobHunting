//========================================
//  *** setting.h ***
//========================================
#ifndef _SETTING_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _SETTING_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************

// ���ʂ̒i�K
#define VOLUME_STAGE	(20)

//****************************************
// �\���̂̒�`
//****************************************

// �ݒ�̏��\����
typedef struct
{
	int nBGMVolume;		// BGM����
	int nSEVolume;		// SE����
	bool bVibration;	// �U���t���O
}Setting;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �ݒ�̏����擾
Setting *GetSetting(void);

// �ݒ�̏���������
// �ݒ�̏I������
// �ݒ�̍X�V����
void InitSetting(void);
void UninitSetting(void);
void UpdateSetting(void);

// �ݒ�̓ǂݍ��ݏ���
// �ݒ�̏������ݏ���
void LoadSetting(void);
void SaveSetting(void);

#endif
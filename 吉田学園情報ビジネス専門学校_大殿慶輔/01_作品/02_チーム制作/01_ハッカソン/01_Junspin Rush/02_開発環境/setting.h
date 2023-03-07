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
	bool bFullScreen;	// �S��ʃt���O
	bool bVibration;	// �U���t���O
	bool bLaser;		// �����t���O
}Setting;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �ݒ�̏����擾
Setting *GetSetting(void);

// �ݒ�̏���������
void InitSetting(void);

// �ݒ�̏I������
void UninitSetting(void);

// �ݒ�̍X�V����
void UpdateSetting(void);

// �ݒ胁�j���[�̍X�V����
void UpdateSettingMenu(void);

// �ݒ胁�j���[�̐ݒ菈��
void SetSettingMenu(MODE mode);

// �ݒ�̓ǂݍ��ݏ���
void LoadSetting(void);

// �ݒ�̏������ݏ���
void SaveSetting(void);

#endif
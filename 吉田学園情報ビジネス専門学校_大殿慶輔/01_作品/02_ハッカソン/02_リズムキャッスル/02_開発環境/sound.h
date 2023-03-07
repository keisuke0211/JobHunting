//========================================
// 
// �T�E���h�̃w�b�_�t�@�C��
// Author : AKIRA TANAKA
// |
// Arrange : RIKU NISHIMURA
//			 �Ό��D�n
// 
//========================================
// *** sys_stage_00.h ***
//========================================
#ifndef _SOUND_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _SOUND_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// �T�E���h�̎��
typedef enum 
{
	SOUND_TYPE_BGM,	// ���y
	SOUND_TYPE_SE,	// ���ʉ�
	SOUND_TYPE_MAX,
}SOUND_TYPE;

//****************************************
// �\���̂̒�`
//****************************************
// �T�E���h���̍\���̒�`
typedef struct
{
	char		aFileName	// �t�@�C����
				[TXT_MAX];
	int			nCntLoop;	// ���[�v�J�E���g
	float		fVolume;	// ����
	SOUND_TYPE	type;		// �T�E���h�̎��
} SOUNDINFO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �T�E���h�̏����擾
SOUNDINFO *GetSoundInfo(void);

// �T�E���h�̓ǂݍ��ݏ���
void LoadSound(void);

// �T�E���h�̏���������
// �T�E���h�̏I������
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

// �Z�O�����g�Đ�(�Đ����Ȃ��~)
// �Z�O�����g��~(���x���w��)
// �Z�O�����g��~(�S��)
HRESULT PlaySound(int nSoundIdx);
void StopSound(int nSoundIdx);
void StopSound(void);

// �ݒ艹�ʂ�ݒ�
void SetSettingVolume(SOUND_TYPE type);

// ��ޖ��ɉ��ʕύX
// ��ޖ��ɍĐ���~
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);
void StopSoundType(SOUND_TYPE type);

//��ޖ����Ɉꎞ��~�E�ĊJ
void PauseSound(int nSoundIdx);
void RestartSound(int nSoundIdx);

#endif

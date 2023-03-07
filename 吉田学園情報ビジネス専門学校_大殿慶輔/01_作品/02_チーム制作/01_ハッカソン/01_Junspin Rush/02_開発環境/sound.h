//========================================
// 
// �T�E���h�����̃w�b�_�t�@�C��
// Author : AKIRA TANAKA
// |
// Arrange : ���� ����
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
	SOUND_TYPE_BGM = 0,	// ���y
	SOUND_TYPE_SE,		// ���ʉ�
	SOUND_TYPE_MAX,
}SOUND_TYPE;

// �T�E���h�ꗗ
typedef enum
{
	SOUND_LABEL_BGM_TITLE_000 = 0,		// �^�C�g��BGM			[000]
	SOUND_LABEL_BGM_STAGE_000,			// �X�e�[�WBGM			[000]
	SOUND_LABEL_BGM_RESULT_000,			// ���U���gBGM			[000]
	SOUND_LABEL_SE_GAME_CLEAR_000,		// �Q�[���N���ASE		[000]
	SOUND_LABEL_SE_GAME_OVER_000,		// �Q�[���I�[�o�[SE		[000]
	SOUND_LABEL_SE_JUMP_000,			// �W�����vSE			[000]
	SOUND_LABEL_SE_LANDING_000,			// ���nSE				[000]
	SOUND_LABEL_SE_DAMAGE_000,			// �_���[�WSE			[000]
	SOUND_LABEL_SE_SLASH_000,			// �X���b�V��SE			[000]
	SOUND_LABEL_SE_DETERMINATION_000,	// ����SE				[000]
	SOUND_LABEL_SE_METAL_000,			// ����SE				[000]
	SOUND_LABEL_MAX
}SOUND_LABEL;

//****************************************
// �\���̂̒�`
//****************************************

// �T�E���h���̍\���̒�`
typedef struct
{
	char		*pFilename;	// �t�@�C����
	char		*pLoadName;	// �ǂݍ��ݖ�
	int			nCntLoop;	// ���[�v�J�E���g
	float		fVolume;	// ����
	SOUND_TYPE	type;		// �T�E���h�̎��
} SOUNDINFO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

// �T�E���h�̏����擾
SOUNDINFO *GetSoundInfo(void);

// �T�E���h�̎�ނ𕶎��񂩂�ǂݍ���
void StringLoadSound(char aString[TXT_MAX], SOUND_LABEL *pLabel);

// �T�E���h�̏���������
HRESULT InitSound(HWND hWnd);

// �T�E���h�̏I������
void UninitSound(void);

// �Z�O�����g�Đ�(�Đ����Ȃ��~)
HRESULT PlaySound(SOUND_LABEL label);

// �Z�O�����g��~(���x���w��)
void StopSound(SOUND_LABEL label);

// �Z�O�����g��~(�S��)
void StopSound(void);

// ��ޖ��ɉ��ʕύX
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);

// �ݒ艹�ʂ�ݒ�
void SetSettingVolume(SOUND_TYPE type);

// ��ޖ��ɍĐ���~
void StopSoundType(SOUND_TYPE type);

#endif

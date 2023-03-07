//========================================
// 
// �ݒ�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** setting.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "setting.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// �ݒ�̏��̃f�[�^�t�@�C���̑��΃p�X
#define SETTING_DATA_FILE_PATH	"data\\SAVEDATA\\setting.bin"

// ���ʂ̕\���{��
#define VOLUME_DISPLAY_DIAMETER	(5)

// ��ʃ��[�h�̐؂�ւ��ɂ����鎞��
#define WINDOW_MODE_CHANGE_TIME	(20)

// �ݒ胁�j���[�̈ʒu
#define SETTING_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 64, 0.0f))

//****************************************
// �񋓌^�̒�`
//****************************************

// �ݒ胁�j���[
typedef enum
{
	SETTING_MENU_BGM_VOLUME = 0,	// BGM����
	SETTING_MENU_SE_VOLUME,			// SE����
	SETTING_MENU_SCREEN_MODE,		// ��ʃ��[�h
	SETTING_MENU_VIBRATION,			// �U��
	SETTING_MENU_LASER,				// ����
	SETTING_MENU_OK,				// ����
	SETTING_MENU_MAX,
}SETTING_MENU;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �ݒ胁�j���[�̃e�L�X�g��ݒ�
void SetSettingMenuText(void);

//****************************************
// �O���[�o���錾
//****************************************
MODE	g_sourceMode;	// ���̃��[�h

// �ݒ�̏��
Setting g_setting =
{// �����ݒ�
	VOLUME_STAGE / 2,
	VOLUME_STAGE / 2,
	true,
};

// �U���̕\���`��
char g_aVibrationText[2][TXT_MAX] =
{
	"VIB :OFF ",
	"VIB :ON  ",
};

//========== *** �ݒ�̏����擾 ***
Setting *GetSetting(void) 
{
	return &g_setting;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitSetting�֐� - �ݒ�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitSetting(void)
{

}

//========================================
// UninitSetting�֐� - �ݒ�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitSetting(void)
{

}

//========================================
// UpdateSetting�֐� - �ݒ�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateSetting(void)
{
	
}

//========================================
// LoadSetting�֐� - �ݒ�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadSetting(void)
{
	FILE *pFile;	//�t�@�C���̃|�C���^

	//�t�@�C�����J��
	pFile = fopen(SETTING_DATA_FILE_PATH, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J�������A
		//�t�@�C������ݒ��ǂݍ���
		fread(&g_setting, sizeof(Setting), 1, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}

//========================================
// SaveSetting�֐� - �ݒ�̏������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void SaveSetting(void)
{
	FILE *pFile;	//�t�@�C���̃|�C���^

	//�t�@�C�����J��
	pFile = fopen(SETTING_DATA_FILE_PATH, "wb");

	if (pFile != NULL)
	{//�t�@�C�����J�������A
		//�t�@�C���ɐݒ�������o��
		fwrite(&g_setting, sizeof(Setting), 1, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}
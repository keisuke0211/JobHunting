//========================================
// 
// �ݒ�̏���
// Author:���� ����
// 
//========================================
// *** setting.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "setting.h"
#include "md_game_00.h"
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
// �O���[�o���錾
//****************************************
MODE	g_sourceMode;						// ���̃��[�h
int		g_nCounterScreenModeChange = -1;	// ��ʃ��[�h�؂�ւ��J�E���^�[

// �ݒ�̏��
Setting g_setting =
{// �����ݒ�
	VOLUME_STAGE / 2,
	VOLUME_STAGE / 2,
	false,
	true,
	true,
};

// ��ʃ��[�h�̕\���`��
char g_aScreenModeText[2][TXT_MAX] =
{
	"FULL:OFF ",
	"FULL:ON  ",
};

// �U���̕\���`��
char g_aVibrationText[2][TXT_MAX] =
{
	"VIB :OFF ",
	"VIB :ON  ",
};

// �����̕\���`��
char g_aLaserText[2][TXT_MAX] =
{
	"BEAM:OFF ",
	"BEAM:ON  ",
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
//========================================
void InitSetting(void)
{

}

//========================================
// UninitSetting�֐� - �ݒ�̏I������ -
//========================================
void UninitSetting(void)
{

}

//========================================
// UpdateSetting�֐� - �ݒ�̍X�V���� -
//========================================
void UpdateSetting(void)
{
	if (g_nCounterScreenModeChange >= 0) 
	{// ��ʃ��[�h�؂�ւ��J�E���^�[��0�ȏ�̎��A
		// ��ʃ��[�h�؂�ւ��J�E���^�[�����Z
		g_nCounterScreenModeChange--;

		if (g_nCounterScreenModeChange == 0) 
		{// ��ʃ��[�h�؂�ւ��J�E���^�[��0�̎��A
			// �E�C���h�E�̃��[�h��؂�ւ���
			WindowModeChange(g_setting.bFullScreen);
		}
	}

	if (GetKeyboardTrigger(DIK_F11))
	{// F11�L�[�����͂��ꂽ���A
		// �S��ʃt���O��؂�ւ���
		g_setting.bFullScreen ^= 1;

		// ��ʃ��[�h�؂�ւ��J�E���^�[��ݒ�
		g_nCounterScreenModeChange = WINDOW_MODE_CHANGE_TIME;
	}
}

//========================================
// LoadSetting�֐� - �ݒ�̓ǂݍ��ݏ��� -
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

#if _DEBUG
	// �f�o�b�O���A�S��ʃt���O���U�ɂ���B(�t���[�Y�΍�)
	g_setting.bFullScreen = false;
#endif 

}

//========================================
// SaveSetting�֐� - �ݒ�̏������ݏ��� -
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
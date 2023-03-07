//========================================
// 
// MD:�`���[�g���A�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_tutorial_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "color.h"
#include "fade.h"
#include "input.h"
#include "physics.h"
#include "sound.h"
#include "text.h"
#include "bg_tutorial_00.h"		// BG :�`���[�g���A��		[00]
#include "md_tutorial_00.h"		// MD :�`���[�g���A�����	[00]
#include <stdio.h>	

//****************************************
// �}�N����`
//****************************************
// MD:�`���[�g���A�����[00] �̃f�[�^�t�@�C���̑��΃p�X
#define BG_TUTORIAL_00_DATA_FILE_PATH	"data\\GAMEDATA\\MODE\\MD_TUTORIAL_00_DATA.txt"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�`���[�g���A�����[00] �̃p�����[�^�[�̏���������
// MD:�`���[�g���A�����[00] �̏�Ԃɉ������X�V����
// MD:�`���[�g���A�����[00] �̏�Ԃɉ������J�n����
// MD:�`���[�g���A�����[00] �̏�Ԃɉ������I������
void InitParameterMd_tutorial_00(void);
void UpdateMd_tutorial_00State(void);
void StartMd_tutorial_00State(void);
void EndMd_tutorial_00State(void);

//****************************************
// �O���[�o���錾
//****************************************
Md_tutorial_00	g_md_tutorial_00;	// MD:�`���[�g���A�����[00] �̏��

//========== *** MD:�`���[�g���A�����[00] �̏����擾 ***
Md_tutorial_00 *GetMd_tutorial_00(void) 
{
	return &g_md_tutorial_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMd_tutorial_00�֐� - MD:�`���[�g���A�����[00] �̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadMd_tutorial_00(void) 
{
	FILE			*pFile;		// �t�@�C���|�C���^
	char			aDataSearch	// �f�[�^�����p
					[TXT_MAX];
	Md_tutorial_00	*pMd		// MD:�`���[�g���A�����[00] �̏��̃|�C���^
					= &g_md_tutorial_00;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(BG_TUTORIAL_00_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if		(!strcmp(aDataSearch, "END"))					{ fclose(pFile); break; }							// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "PROMPT_TEXT_FONT:"))		{ fscanf(pFile, "%d", &pMd->nPromptTextFont); }		// ���͍Ñ��e�L�X�g�̃t�H���g
		else if (!strcmp(aDataSearch, "PROMPT_TEXT_COLOR:"))	{ LoadColor(pFile, &pMd->promptTextCol); }			// ���͍Ñ��e�L�X�g�̃t�H���g
		else if (!strcmp(aDataSearch, "PROMPT_TEXT_POS:"))		{ LoadVector(pFile, &pMd->promptTextPos); }			// ���͍Ñ��e�L�X�g�̈ʒu
		else if (!strcmp(aDataSearch, "WAIT_BLINK_TIME:"))		{ fscanf(pFile, "%d", &pMd->nWaitBlinkTime); }		// �ҋ@���̓_�łɂ����鎞��
		else if (!strcmp(aDataSearch, "DECISION_BLINK_TIME:"))	{ fscanf(pFile, "%d", &pMd->nDecisionBlinkTime); }	// ���莞�̓_�łɂ����鎞��
		else if (!strcmp(aDataSearch, "BGM:"))					{ fscanf(pFile, "%d", &pMd->nBGM); }				// BGM
		else if (!strcmp(aDataSearch, "DECITION_SE:"))			{ fscanf(pFile, "%d", &pMd->nDecisionSE); }			// ����SE
	}
}

//========================================
// InitParameterMd_tutorial_00�֐� - MD:�`���[�g���A�����[00] �̃p�����[�^�[�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMd_tutorial_00(void)
{
	Md_tutorial_00	*pMd	// MD:�`���[�g���A�����[00] �̏��
				= &g_md_tutorial_00;

	pMd->state				= MD_TUTORIAL_00_STATE_NONE;	// ���
	pMd->nCounterState		= 0;						// ��ԃJ�E���^�[
	pMd->nCounterPromptText = 0;						// ���͍Ñ��e�L�X�g�̔ėp�J�E���^�[
}

//========================================
// StartMd_tutorial_00State�֐� - MD:�`���[�g���A�����[00] �̏�Ԃɉ������J�n���� -
// Author:RIKU NISHIMURA
//========================================
void StartMd_tutorial_00State(void)
{
	Md_tutorial_00	*pMd	// MD:�`���[�g���A�����[00] �̏��
				= &g_md_tutorial_00;

	// ��ԃJ�E���^�[��������
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TUTORIAL_00_STATE_NORMAL: {

		break;
	}
	}
}

//========================================
// EndMd_tutorial_00State�֐� - MD:�`���[�g���A�����[00] �̏�Ԃɉ������I������ -
// Author:RIKU NISHIMURA
//========================================
void EndMd_tutorial_00State(void) 
{
	Md_tutorial_00	*pMd	// MD:�`���[�g���A�����[00] �̏��
				= &g_md_tutorial_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TUTORIAL_00_STATE_NORMAL: {

		break;
	}
	}
}

//========================================
// UpdateMd_tutorial_00State�֐� - MD:�`���[�g���A�����[00] �̏�Ԃɉ������X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_tutorial_00State(void)
{
	Md_tutorial_00	*pMd	// MD:�`���[�g���A�����[00] �̏��
				= &g_md_tutorial_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TUTORIAL_00_STATE_NORMAL: {
		if (((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A))) && (GetFade() == FADE_NONE))
		{// ENTER�L�[orA�{�^���������ꂽ & �t�F�[�h�������̎��A
			PlaySound(pMd->nDecisionSE);	// ����SE���Đ�
			SetFade(MODE_GAME_00);			// ���[�h��MD:�Q�[�����[00] �Ɉڍs
		}
		
		if (GetFade() == FADE_OUT) 
		{// �t�F�[�h�A�E�g��Ԃ̎��A���͍Ñ��e�L�X�g�̔ėp�J�E���^�[�����Z&����
			++pMd->nCounterPromptText %= (pMd->nDecisionBlinkTime * 2);
		}

		if ((pMd->nCounterPromptText / pMd->nDecisionBlinkTime) % 2 == 0)
		{// ���͍Ñ��e�L�X�g�̔ėp�J�E���^�[�̐i�s�x�ɉ����ăe�L�X�g�̐ݒ�
			SetText2D(
				"PRESS A BUTTON",
				0,
				DISPLAY_CENTER,
				pMd->promptTextPos,
				pMd->promptTextCol,
				1.0f, 1.0f);
		}
		break;
	}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitMd_tutorial_00�֐� - MD:�`���[�g���A�����[00]�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitMd_tutorial_00(void)
{
	Md_tutorial_00	*pMd	// MD:�`���[�g���A�����[00] �̏��
				= &g_md_tutorial_00;

	// �p�����[�^�[�̏���������
	InitParameterMd_tutorial_00();

	// ����������
	InitParameterCamera3D();	// �J����(3D)�̃p�����[�^�[
	InitBg_tutorial_00();		// BG :�`���[�g���A��	[00]
	
	// ��Ԃ����S�҂��ɐݒ�
	SetMd_tutorial_00State(MD_TUTORIAL_00_STATE_NORMAL);

	// BGM���Đ�
	PlaySound(7);
}

//========================================
// UninitMd_tutorial_00�֐� - MD:�`���[�g���A�����[00]�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_tutorial_00(void)
{
	// �I������
	UninitBg_tutorial_00();	// BG :�`���[�g���A��	[00]
}

//========================================
// UpdateMd_tutorial_00�֐� - MD:�`���[�g���A�����[00]�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_tutorial_00(void)
{
	// MD:�`���[�g���A�����[00] �̏�Ԃɉ������X�V����
	UpdateMd_tutorial_00State();

	// �X�V����
	UpdateBg_tutorial_00();	// BG :�`���[�g���A��	[00]
}

//========================================
// DrawMd_tutorial_00�֐� - MD:�`���[�g���A�����[00]�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_tutorial_00(void)
{
	// �`�揈��
	DrawBg_tutorial_00();	// BG :�`���[�g���A��	[00]
	DrawText_();			// �e�L�X�g
}

//========================================
// SetMd_tutorial_00State�֐� - MD:�`���[�g���A�����[00] �̏�Ԃ�ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetMd_tutorial_00State(MD_TUTORIAL_00_STATE state)
{
	// MD:�`���[�g���A�����[00] �̏�Ԃɉ������I������
	EndMd_tutorial_00State();

	// MD:�`���[�g���A�����[00] �̏�Ԃ���
	g_md_tutorial_00.state = state;

	// MD:�`���[�g���A�����[00] �̏�Ԃɉ������J�n����
	StartMd_tutorial_00State();
}
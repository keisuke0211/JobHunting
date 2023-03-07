//========================================
// 
// MD:�^�C�g�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_title_00.cpp ***
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
#include "bg_title_00.h"		// BG :�^�C�g��		[00]
#include "md_title_00.h"		// MD :�^�C�g�����	[00]
#include "ui_title-logo_00.h"	// UI :�^�C�g�����S	[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// MD:�^�C�g�����[00] �̃f�[�^�t�@�C���̑��΃p�X
#define BG_TITLE_00_DATA_FILE_PATH	"data\\GAMEDATA\\MODE\\MD_TITLE_00_DATA.txt"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�^�C�g�����[00] �̃p�����[�^�[�̏���������
// MD:�^�C�g�����[00] �̏�Ԃɉ������X�V����
// MD:�^�C�g�����[00] �̏�Ԃɉ������J�n����
// MD:�^�C�g�����[00] �̏�Ԃɉ������I������
void InitParameterMd_title_00(void);
void UpdateMd_title_00State(void);
void StartMd_title_00State(void);
void EndMd_title_00State(void);

//****************************************
// �O���[�o���錾
//****************************************
Md_title_00	g_md_title_00;	// MD:�^�C�g�����[00] �̏��

//========== *** MD:�^�C�g�����[00] �̏����擾 ***
Md_title_00 *GetMd_title_00(void) 
{
	return &g_md_title_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMd_title_00�֐� - MD:�^�C�g�����[00] �̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadMd_title_00(void) 
{
	FILE		*pFile;		// �t�@�C���|�C���^
	char		aDataSearch	// �f�[�^�����p
				[TXT_MAX];
	Md_title_00	*pMd		// MD:�^�C�g�����[00] �̏��̃|�C���^
				= &g_md_title_00;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(BG_TITLE_00_DATA_FILE_PATH, "r");

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
// InitParameterMd_title_00�֐� - MD:�^�C�g�����[00] �̃p�����[�^�[�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMd_title_00(void)
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	pMd->state				= MD_TITLE_00_STATE_NONE;	// ���
	pMd->nCounterState		= 0;						// ��ԃJ�E���^�[
	pMd->nCounterPromptText = 0;						// ���͍Ñ��e�L�X�g�̔ėp�J�E���^�[
}

//========================================
// StartMd_title_00State�֐� - MD:�^�C�g�����[00] �̏�Ԃɉ������J�n���� -
// Author:RIKU NISHIMURA
//========================================
void StartMd_title_00State(void)
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	// ��ԃJ�E���^�[��������
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TITLE_00_STATE_NORMAL: {

		break;
	}
	case /*/ ���S�҂� /*/MD_TITLE_00_STATE_LOGOWAIT: {

		break;
	}
	}
}

//========================================
// EndMd_title_00State�֐� - MD:�^�C�g�����[00] �̏�Ԃɉ������I������ -
// Author:RIKU NISHIMURA
//========================================
void EndMd_title_00State(void) 
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TITLE_00_STATE_NORMAL: {

		break;
	}
	case /*/ ���S�҂� /*/MD_TITLE_00_STATE_LOGOWAIT: {

		break;
	}
	}
}

//========================================
// UpdateMd_title_00State�֐� - MD:�^�C�g�����[00] �̏�Ԃɉ������X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_title_00State(void)
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_TITLE_00_STATE_NORMAL: {
		if (((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A))) && (GetFade()==FADE_NONE)) 
		{// ENTER�L�[orA�{�^���������ꂽ & �t�F�[�h�������̎��A
			PlaySound(pMd->nDecisionSE);	// ����SE���Đ�
			SetFade(MODE_TUTORIAL_00);		// ���[�h��MD:�`���[�g���A�����[00] �Ɉڍs
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
	case /*/ ���S�҂� /*/MD_TITLE_00_STATE_LOGOWAIT: {
		if (GetUi_titleLogo_00()->nCounterArrival >= GetUi_titleLogo_00()->nArrivalTime) 
		{// UI:�^�C�g�����S[00] �̓����J�E���^�[�������J�E���^�[�ɒB���Ă��鎞�A
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);	// ��Ԃ�ʏ�ɐݒ�
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
// InitMd_title_00�֐� - MD:�^�C�g�����[00]�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitMd_title_00(void)
{
	Md_title_00	*pMd	// MD:�^�C�g�����[00] �̏��
				= &g_md_title_00;

	// �p�����[�^�[�̏���������
	InitParameterMd_title_00();

	// ����������
	InitParameterCamera3D();	// �J����(3D)�̃p�����[�^�[
	InitBg_title_00();			// BG :�^�C�g��		[00]
	InitUi_titleLogo_00();		// UI :�^�C�g�����S	[00]

	// ��Ԃ����S�҂��ɐݒ�
	SetMd_title_00State(MD_TITLE_00_STATE_LOGOWAIT);

	// BGM���Đ�
	PlaySound(pMd->nBGM);
}

//========================================
// UninitMd_title_00�֐� - MD:�^�C�g�����[00]�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_title_00(void)
{
	// �I������
	UninitBg_title_00();		// BG :�^�C�g��		[00]
	UninitUi_titleLogo_00();	// UI :�^�C�g�����S	[00]
}

//========================================
// UpdateMd_title_00�֐� - MD:�^�C�g�����[00]�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_title_00(void)
{
	// MD:�^�C�g�����[00] �̏�Ԃɉ������X�V����
	UpdateMd_title_00State();

	// �X�V����
	UpdateBg_title_00();		// BG :�^�C�g��		[00]
	UpdateUi_titleLogo_00();	// UI :�^�C�g�����S	[00]
}

//========================================
// DrawMd_title_00�֐� - MD:�^�C�g�����[00]�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_title_00(void)
{
	// �`�揈��
	DrawBg_title_00();		// BG :�^�C�g��		[00]
	DrawUi_titleLogo_00();	// UI :�^�C�g�����S	[00]
	DrawText_();			// �e�L�X�g
}

//========================================
// SetMd_title_00State�֐� - MD:�^�C�g�����[00] �̏�Ԃ�ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetMd_title_00State(MD_TITLE_00_STATE state)
{
	// MD:�^�C�g�����[00] �̏�Ԃɉ������I������
	EndMd_title_00State();

	// MD:�^�C�g�����[00] �̏�Ԃ���
	g_md_title_00.state = state;

	// MD:�^�C�g�����[00] �̏�Ԃɉ������J�n����
	StartMd_title_00State();
}
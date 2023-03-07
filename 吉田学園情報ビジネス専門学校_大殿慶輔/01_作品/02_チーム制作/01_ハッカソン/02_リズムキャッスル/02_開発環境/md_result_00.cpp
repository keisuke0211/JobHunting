//========================================
// 
// MD:���U���g���[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_result_00.cpp ***
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
#include "player.h"
#include "bg_result_00.h"	// BG :���U���g		[00]
#include "bg_result_01.h"	// BG :���U���g		[01]
#include "md_result_00.h"	// MD :���U���g���	[00]
#include "sys_ranking_00.h"	// SYS:�����L���O	[00]
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// MD:���U���g���[00] �̃f�[�^�t�@�C���̑��΃p�X
#define BG_RESULT_00_DATA_FILE_PATH	"data\\GAMEDATA\\MODE\\MD_RESULT_00_DATA.txt"

// �����L���O�e�L�X�g�̈ʒu
// �����L���O�e�L�X�g�̊J�n�ʒuX
// �����L���O�e�L�X�g�̏I���ʒuX
// �����L���O�e�L�X�g�̈ʒuY
// �����L���O�e�L�X�g�̊ԊuY
// �����L���O�e�L�X�g�Z�b�g�ɂ����鎞��
#define RANKING_TEXT_POS			D3DXVECTOR3(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f,0.0f)
#define RANKING_TEXT_START_POS_X	(PIXEL*-40)
#define RANKING_TEXT_END_POS_X		(PIXEL*0)
#define RANKING_TEXT_POS_Y			(PIXEL*-64)
#define RANKING_TEXT_SPACE_Y		(PIXEL*16)
#define RANKING_TEXT_SET_TIME		(90)

// �����L���O�e�L�X�g�̓_�łɂ����鎞��
// �����L���O�e�L�X�g�̓_�ŐF
#define RANKING_TEXT_BLINK_TIME		(4)
#define RANKING_TEXT_BLINK_COLOR	Color{243,189,63,255}

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:���U���g���[00] �̃p�����[�^�[�̏���������
// MD:���U���g���[00] �̏�Ԃɉ������X�V����
// MD:���U���g���[00] �̏�Ԃɉ������J�n����
// MD:���U���g���[00] �̏�Ԃɉ������I������
void InitParameterMd_result_00(void);
void UpdateMd_result_00State(void);
void StartMd_result_00State(void);
void EndMd_result_00State(void);

//****************************************
// �O���[�o���錾
//****************************************
Md_result_00	g_md_result_00;	// MD:���U���g���[00] �̏��
int				g_nResultBg;	// MD:���U���g���[00] �̔w�i�ԍ�

//========== *** MD:���U���g���[00] �̏����擾 ***
Md_result_00 *GetMd_result_00(void) 
{
	return &g_md_result_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMd_result_00�֐� - MD:���U���g���[00] �̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadMd_result_00(void) 
{
	FILE			*pFile;		// �t�@�C���|�C���^
	char			aDataSearch	// �f�[�^�����p
					[TXT_MAX];
	Md_result_00	*pMd		// MD:���U���g���[00] �̏��̃|�C���^
					= &g_md_result_00;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(BG_RESULT_00_DATA_FILE_PATH, "r");

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
		else if (!strcmp(aDataSearch, "RANKINGSET_SE:"))		{ fscanf(pFile, "%d", &pMd->nRankingSetSE); }		// �����L���O�Z�b�gSE
		else if (!strcmp(aDataSearch, "DECITION_SE:"))			{ fscanf(pFile, "%d", &pMd->nDecisionSE); }			// ����SE
	}
}

//========================================
// InitParameterMd_result_00�֐� - MD:���U���g���[00] �̃p�����[�^�[�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMd_result_00(void)
{
	Md_result_00	*pMd	// MD:���U���g���[00] �̏��
					= &g_md_result_00;

	pMd->state				= MD_RESULT_00_STATE_NONE;	// ���
	pMd->nCounterState		= 0;						// ��ԃJ�E���^�[
	pMd->nCounterPromptText = 0;						// ���͍Ñ��e�L�X�g�̔ėp�J�E���^�[
	pMd->nUpdateRank		= -1;						// �X�V����
}

//========================================
// StartMd_result_00State�֐� - MD:���U���g���[00] �̏�Ԃɉ������J�n���� -
// Author:RIKU NISHIMURA
//========================================
void StartMd_result_00State(void)
{
	Md_result_00	*pMd	// MD:���U���g���[00] �̏��
					= &g_md_result_00;

	// ��ԃJ�E���^�[��������
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_RESULT_00_STATE_NORMAL: {

		break;
	}
	case /*/ �����L���O�҂� /*/MD_RESULT_00_STATE_RANKINGWAIT: {

		break;
	}
	}
}

//========================================
// EndMd_result_00State�֐� - MD:���U���g���[00] �̏�Ԃɉ������I������ -
// Author:RIKU NISHIMURA
//========================================
void EndMd_result_00State(void) 
{
	Md_result_00	*pMd	// MD:���U���g���[00] �̏��
					= &g_md_result_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_RESULT_00_STATE_NORMAL: {

		break;
	}
	case /*/ �����L���O�҂� /*/MD_RESULT_00_STATE_RANKINGWAIT: {

		break;
	}
	}
}

//========================================
// UpdateMd_result_00State�֐� - MD:���U���g���[00] �̏�Ԃɉ������X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_result_00State(void)
{
	Md_result_00	*pMd	// MD:���U���g���[00] �̏��
					= &g_md_result_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_RESULT_00_STATE_NORMAL: {
		if (((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A))) && (GetFade() == FADE_NONE))
		{// ENTER�L�[orA�{�^���������ꂽ & �t�F�[�h�������̎��A
			PlaySound(pMd->nDecisionSE);	// ����SE���Đ�
			SetFade(MODE_TITLE_00);			// ���[�h��MD:�^�C�g�����[00] �Ɉڍs
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

		Sys_ranking_00	*pSys	// �����L���O[00] �̏��̃|�C���^
						= GetSys_ranking_00();

		++pMd->nCounterBlink %= (RANKING_TEXT_BLINK_TIME * 2);	// �_�ŃJ�E���^�[�����Z&����

		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++, pSys++)
		{
			char aString[TXT_MAX];
			sprintf(aString, "%s %8d", GetRankText(nCntRanking), pSys->nScore);
			Color setColor =
				pMd->nUpdateRank == nCntRanking ?
				!(pMd->nCounterBlink / RANKING_TEXT_BLINK_TIME) ?
				INITCOLOR : RANKING_TEXT_BLINK_COLOR :
				INITCOLOR;
			SetText2D(
				aString,
				0,
				DISPLAY_CENTER,
				RANKING_TEXT_POS +
				D3DXVECTOR3(
					RANKING_TEXT_END_POS_X,
					RANKING_TEXT_POS_Y + (RANKING_TEXT_SPACE_Y * nCntRanking),
					0.0f),
				setColor,
				1.0f,
				1.0f);
		}

		break;
	}
	case /*/ �����L���O�҂� /*/MD_RESULT_00_STATE_RANKINGWAIT: {
		// ��ԃJ�E���^�[�����Z
		pMd->nCounterState++;

		// ��ԃJ�E���^�[�𐧌�
		IntControl(&pMd->nCounterState, RANKING_TEXT_SET_TIME, 0);

		Sys_ranking_00	*pSys	// �����L���O[00] �̏��̃|�C���^
						= GetSys_ranking_00();

		const int	nTime	// 1�̃����L���O������ɂ����鎞��
					= RANKING_TEXT_SET_TIME / RANKING_NUM;

		if ((pMd->nCounterState - 1) % nTime == 0)
		{// ��ԃJ�E���^�[��1�̃����L���O������ɂ����鎞�ԂŊ���؂�鎞�A
			PlaySound(pMd->nRankingSetSE);	// �����L���O�Z�b�gSE���Đ�
		}

		for (int nCntRanking = 0; nCntRanking < IntControlReturn((pMd->nCounterState / nTime) + 1, RANKING_NUM, 0); nCntRanking++, pSys++)
		{
			char aString[TXT_MAX];
			sprintf(aString, "%s %8d", GetRankText(nCntRanking), pSys->nScore);
			float fRate =	// �i�s����
				(float)(IntControlReturn(pMd->nCounterState, nTime * (nCntRanking + 1), 0) -
				(nTime * nCntRanking)) / (float)nTime;
			SetText2D(
				aString,
				0,
				DISPLAY_CENTER,
				RANKING_TEXT_POS +
				D3DXVECTOR3(
				(RANKING_TEXT_START_POS_X * (1.0f - fRate)) + (RANKING_TEXT_END_POS_X * fRate),
					RANKING_TEXT_POS_Y + (RANKING_TEXT_SPACE_Y * nCntRanking),
					0.0f),
					{ 255,255,255,(int)(255 * fRate) },
				1.0f,
				1.0f);
		}

		if (pMd->nCounterState >= RANKING_TEXT_SET_TIME)
		{// ��ԃJ�E���^�[���e�L�X�g�Z�b�g�ɂ����鎞�ԂɒB�������A
			SetMd_result_00State(MD_RESULT_00_STATE_NORMAL);	// ��Ԃ�ʏ�ɂ���
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
// InitMd_result_00�֐� - MD:���U���g���[00]�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitMd_result_00(void)
{
	Md_result_00	*pMd	// MD:���U���g���[00] �̏��
					= &g_md_result_00;

	// �p�����[�^�[�̏���������
	InitParameterMd_result_00();

	// ����������
	InitParameterCamera3D();	// �J����(3D)�̃p�����[�^�[
	if (g_nResultBg == 0) { InitBg_result_00(); }	// BG :���U���g		[00]
	if (g_nResultBg == 1) { InitBg_result_01(); }	// BG :���U���g		[01]

	// ��Ԃ������L���O�҂��ɐݒ�
	SetMd_result_00State(MD_RESULT_00_STATE_RANKINGWAIT);

	// BGM���Đ�
	PlaySound(8);

	// �X�R�A�ݒ�
	pMd->nUpdateRank = SetScore(GetPlayer()->nScore);
}

//========================================
// UninitMd_result_00�֐� - MD:���U���g���[00]�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_result_00(void)
{
	// �I������
	if (g_nResultBg == 0) { UninitBg_result_00(); }	// BG :���U���g		[00]
	if (g_nResultBg == 1) { UninitBg_result_01(); }	// BG :���U���g		[01]
}

//========================================
// UpdateMd_result_00�֐� - MD:���U���g���[00]�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_result_00(void)
{
	// MD:���U���g���[00] �̏�Ԃɉ������X�V����
	UpdateMd_result_00State();

	// �X�V����
	if (g_nResultBg == 0) { UpdateBg_result_00(); }	// BG :���U���g		[00]
	if (g_nResultBg == 1) { UpdateBg_result_01(); }	// BG :���U���g		[01]
}

//========================================
// DrawMd_result_00�֐� - MD:���U���g���[00]�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_result_00(void)
{
	// �`�揈��
	if (g_nResultBg == 0) { DrawBg_result_00(); }	// BG :���U���g		[00]
	if (g_nResultBg == 1) { DrawBg_result_01(); }	// BG :���U���g		[01]
	DrawText_();		// �e�L�X�g
}

//========================================
// SetMd_result_00State�֐� - MD:���U���g���[00] �̏�Ԃ�ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetMd_result_00State(MD_RESULT_00_STATE state)
{
	// MD:���U���g���[00] �̏�Ԃɉ������I������
	EndMd_result_00State();

	// MD:���U���g���[00] �̏�Ԃ���
	g_md_result_00.state = state;

	// MD:���U���g���[00] �̏�Ԃɉ������J�n����
	StartMd_result_00State();
}

//========================================
// SetBgMd_result_00�֐� - MD:���U���g���[00] �̔w�i��ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetBgMd_result_00(int nBg) 
{
	g_nResultBg = nBg;
}
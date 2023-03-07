//========================================
// 
// MD:�Q�[�����[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_game_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "text.h"
#include "bg_game_00.h"			// BG :�Q�[��		[00]
#include "chr_enemy_00.h"		// CHR:�G			[00]
#include "md_game_00.h"			// MD :�Q�[�����	[00]
#include "md_result_00.h"		// MD :���U���g���	[00]
#include "obj_castle_00.h"		// OBJ:��			[00]
#include "obj_castle_01.h"		// OBJ:��			[01]
#include "ui_move-text_00.h"	// UI :�ړ��e�L�X�g	[00]
#include "ui_life_000.h"		// UI :���C�t000
#include "eff_light_00.h"		// EFF : ���C�g00
#include "eff_particle_00.h"	// EFF : �p�[�e�B�N��00
#include "player.h"				// �v���C���[
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
#define MD_GAME_00_LIMIT_TIME	(60*120)	// ��������

//****************************************
// �v���g�^�C�v�錾
//****************************************
// MD:�Q�[�����[00] �̃p�����[�^�[�̏���������
// MD:�Q�[�����[00] �̏�Ԃɉ������X�V����
// MD:�Q�[�����[00] �̏�Ԃɉ������J�n����
// MD:�Q�[�����[00] �̏�Ԃɉ������I������
void InitParameterMd_game_00(void);
void UpdateMd_game_00State(void);
void StartMd_game_00State(void);
void EndMd_game_00State(void);

//****************************************
// �O���[�o���錾
//****************************************
Md_game_00	g_md_game_00;	// MD:�Q�[�����[00] �̏��
bool		g_bPause		// �|�[�Y�t���O
			= false;
int			g_nCounterFrame;		// �t���[���J�E���^�[
int			g_nCounterLimitTime;	// �������ԃJ�E���^�[
bool		g_bGameEnd;				// �Q�[���I���t���O
int			g_nTemp;				// �L���p

//========== *** MD:�Q�[�����[00] �̏����擾 ***
Md_game_00 *GetMd_game_00(void) 
{
	return &g_md_game_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterMd_game_00�֐� - MD:�Q�[�����[00] �̃p�����[�^�[�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMd_game_00(void)
{
	Md_game_00	*pMd =	// MD:�Q�[�����[00] �̏��
				&g_md_game_00;

	pMd->state	= MD_GAME_00_STATE_NONE;	// ���
}

//========================================
// StartMd_game_00State�֐� - MD:�Q�[�����[00] �̏�Ԃɉ������J�n���� -
// Author:RIKU NISHIMURA
//========================================
void StartMd_game_00State(void)
{
	Md_game_00	*pMd	// MD:�Q�[�����[00] �̏��
				= &g_md_game_00;

	// ��ԃJ�E���^�[��������
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	}
}

//========================================
// EndMd_game_00State�֐� - MD:�Q�[�����[00] �̏�Ԃɉ������I������ -
// Author:RIKU NISHIMURA
//========================================
void EndMd_game_00State(void) 
{
	Md_game_00	*pMd	// MD:�Q�[�����[00] �̏��
				= &g_md_game_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	}
}

//========================================
// UpdateMd_game_00State�֐� - MD:�Q�[�����[00] �̏�Ԃɉ������X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_game_00State(void)
{
	Md_game_00	*pMd	// MD:�Q�[�����[00] �̏��
				= &g_md_game_00;

	switch (pMd->state)
	{
	case /*/ �ʏ� /*/MD_GAME_00_STATE_NORMAL: {

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
// InitMd_game_00�֐� - MD:�Q�[�����[00]�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitMd_game_00(void)
{
	// �p�����[�^�[�̏���������
	InitParameterMd_game_00();

	// ����������
	InitParameterCamera3D();	// �J����(3D)�̃p�����[�^�[
	InitBg_game_00();			// BG :�Q�[��[00]
	InitChr_enemy_00();			// CHR:�G[00]
	InitObj_castle_00();		// OBJ:��[00]
	InitObj_castle_01();		// OBJ:��[01]
	InitUi_moveText_00();		// UI :�ړ��e�L�X�g[00]
	InitUi_life_000();			// UI :���C�t[00]
	InitEff_light_00();			// EFF : ���C�g00
	InitEff_particle_00();		// EFF : �p�[�e�B�N��00
	InitPlayer();				// �v���C���[

	// ��Ԃ�ʏ�ɐݒ�
	SetMd_game_00State(MD_GAME_00_STATE_NORMAL);

	PlaySound(2);

	// �t���[���J�E���^�[��������
	g_nCounterFrame = 0;

	// �������ԃJ�E���^�[��������
	g_nCounterLimitTime = MD_GAME_00_LIMIT_TIME;

	// �Q�[���I���t���O���U�ɂ���
	g_bGameEnd = false;
}

//========================================
// UninitMd_game_00�֐� - MD:�Q�[�����[00]�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_game_00(void)
{
	UninitPlayer();			//�v���C���[
	UninitEff_light_00();	// EFF : ���C�g00
	UninitBg_game_00();		// BG :�Q�[��[00]
	UninitChr_enemy_00();	// CHR:�G[00]
	UninitObj_castle_00();	// OBJ:��[00]
	UninitObj_castle_01();	// OBJ:��[01]
	UninitUi_moveText_00();	// UI :�ړ��e�L�X�g[00]
	UninitUi_life_000();	// UI :���C�t[00]
}

//========================================
// UpdateMd_game_00�֐� - MD:�Q�[�����[00]�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_game_00(void)
{
	// MD:�Q�[�����[00] �̏�Ԃɉ������X�V����
	UpdateMd_game_00State();

#if _DEBUG
	{
		g_nCounterFrame += g_bPause ^ 1;
		char aString[TXT_MAX];
		sprintf(aString, "FRAME:%d", g_nCounterFrame);
		SetText2D(
			aString,
			2,
			DISPLAY_LEFT,
			D3DXVECTOR3(16.0f, 16.0f, 0.0f), { 255,255,255,255 }, 1.0f, 1.0f);
		if (GetKeyboardTrigger(DIK_P)) 
		{
			g_bPause ^= 1;

			if (g_bPause) 
			{
				PauseSound(2);
			}
			else 
			{
				RestartSound(2);
			}
		}

		if (GetKeyboardTrigger(DIK_T)) 
		{
			g_nTemp = g_nCounterFrame;
		}

		sprintf(aString, "TEMP:%d", g_nTemp);
		SetText2D(
			aString,
			2,
			DISPLAY_LEFT,
			D3DXVECTOR3(16.0f, 40.0f, 0.0f), { 255,255,255,255 }, 1.0f, 1.0f);
	}
#endif

	if (!g_bPause)
	{
		bool bLimitEnd = g_nCounterLimitTime <= 0;	// �������Ԃ��s�����t���O
		bool bLifeDown = GetPlayer()->nLife <= 0;	// ���C�t���s�����t���O

		if (((bLimitEnd) || (bLifeDown)) && (!g_bGameEnd))
		{// �������ԃJ�E���^�[������0�ȉ� or �v���C���[�̗̑͂�0�ȉ��̎��A
			g_bGameEnd = true;				// �Q�[���I���t���O��^�ɂ���
			SetFade(MODE_RESULT_00);		// ���[�h��MD:���U���g���[00] �ɐݒ�
			if (bLimitEnd) 
			{// �������Ԃ��s����
				SetBgMd_result_00(0);
			}
			else 
			{
				SetBgMd_result_00(1);
			}
		}
		else
		{
			g_nCounterLimitTime--;
		}
	}

	{
		char aString[TXT_MAX];
		sprintf(aString, "TIME:%d", g_nCounterLimitTime / 60);
		SetText2D(
			aString,
			0,
			DISPLAY_CENTER,
			D3DXVECTOR3(SCREEN_WIDTH*0.5f, 40.0f, 0.0f), { 255,255,255,255 }, 1.0f, 1.0f);
	}

	if (!g_bPause)
	{
		if (!g_bGameEnd)
		{
			UpdateBg_game_00();		// BG :�Q�[��[00]
			UpdateChr_enemy_00();	// CHR:�G[00]
		}
		UpdateEff_particle_00();
		UpdateEff_light_00();
		UpdateObj_castle_00();	// OBJ:��[00]
		UpdateObj_castle_01();	// OBJ:��[01]
		UpdatePlayer();			// �v���C���[
		UpdateUi_moveText_00();	// UI :�ړ��e�L�X�g[00]
		UpdateUi_life_000();	// UI :���C�t[00]
	}
}

//========================================
// DrawMd_game_00�֐� - MD:�Q�[�����[00]�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_game_00(void)
{
	// �`�揈��
	DrawBg_game_00();		// BG :�Q�[��[00]
	DrawObj_castle_00();	// OBJ:��[00]
	DrawObj_castle_01();	// OBJ:��[01]
	DrawPlayer();			// �v���C���[
	DrawChr_enemy_00();		// CHR:�G[00]
	DrawEff_light_00();		
	DrawUi_life_000();		// UI :���C�t[00]
	DrawText_();			// �e�L�X�g
}

//========================================
// SetMd_game_00State�֐� - MD:�Q�[�����[00] �̏�Ԃ�ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetMd_game_00State(MD_GAME_00_STATE state)
{
	// MD:�Q�[�����[00] �̏�Ԃɉ������I������
	EndMd_game_00State();

	// MD:�Q�[�����[00] �̏�Ԃ���
	g_md_game_00.state = state;

	// MD:�Q�[�����[00] �̏�Ԃɉ������J�n����
	StartMd_game_00State();
}
//========================================
// 
// UI:�ړ��e�L�X�g[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_move-text_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"
#include "text.h"
#include "ui_move-text_00.h"	// UI:�ړ��e�L�X�g	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�ړ��e�L�X�g[00] �̍ő吔
#define UI_MOVETEXT_00_MAX	(64)

//****************************************
// �\���̂̒�`
//****************************************
typedef struct 
{
	bool bUse;
	char aString[TXT_MAX];
	int nFont;
	Color col;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	int nLife;
	int nLifeMax;
}Ui_moveText_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�ړ��e�L�X�g[00] �̃p�����[�^�[����������
void InitParameterUi_moveText_00(Ui_moveText_00 *pUi);

//****************************************
// �O���[�o���錾
//****************************************
Ui_moveText_00	g_aUi_moveText_00
				[UI_MOVETEXT_00_MAX];	// UI:�ړ��e�L�X�g[00] �̕��i���̏��

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_moveText_00�֐� - UI:�ړ��e�L�X�g[00] �̃p�����[�^�[���������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterUi_moveText_00(Ui_moveText_00 *pUi)
{
	pUi->bUse = false;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitUi_moveText_00�֐� - UI:�ړ��e�L�X�g[00] �̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitUi_moveText_00(void)
{
	Ui_moveText_00		*pUi		// UI:�ړ��e�L�X�g[00] �̕��i���̏��̃|�C���^
						= g_aUi_moveText_00;

	for (int nCntUi = 0; nCntUi < UI_MOVETEXT_00_MAX; nCntUi++, pUi++)
	{
		// �p�����[�^�[�̏���������
		InitParameterUi_moveText_00(pUi);
	}
}

//========================================
// UninitUi_moveText_00�֐� - UI:�ړ��e�L�X�g[00] �̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitUi_moveText_00(void)
{
	
}

//========================================
// UpdateUi_moveText_00�֐� - UI:�ړ��e�L�X�g[00] �̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateUi_moveText_00(void)
{
	Ui_moveText_00	*pUi	// UI:�ړ��e�L�X�g[00] �̕��i���̏��̃|�C���^
					= g_aUi_moveText_00;

	for (int nCntUi = 0; nCntUi < UI_MOVETEXT_00_MAX; nCntUi++, pUi++)
	{
		if (!pUi->bUse) 
		{
			continue;
		}
		else if (--pUi->nLife <= 0) 
		{
			pUi->bUse = false;
			continue;
		}

		pUi->pos += pUi->move*((float)pUi->nLife / (float)pUi->nLifeMax);

		Color setCol = pUi->col;
		setCol.a *= ((float)pUi->nLife / (float)pUi->nLifeMax);

		// �e�L�X�g(2D)�̐ݒ菈��
		SetText2D(
			pUi->aString,
			pUi->nFont,
			DISPLAY_CENTER,
			pUi->pos,
			setCol,
			1.0f, 1.0f);
	}
}

//========================================
// SetUi_moveText_00�֐� - UI:�ړ��e�L�X�g[00] �̐ݒ菈�� -
//========================================
void SetUi_moveText_00(char *pString, int nFont, D3DXVECTOR3 pos, D3DXVECTOR3 move, Color col, int nLife)
{
	Ui_moveText_00	*pUi	// UI:�ړ��e�L�X�g[00] �̕��i���̏��̃|�C���^
		= g_aUi_moveText_00;

	for (int nCntUi = 0; nCntUi < UI_MOVETEXT_00_MAX; nCntUi++, pUi++)
	{
		if (pUi->bUse) 
		{
			continue;
		}

		sprintf(pUi->aString, pString);
		pUi->bUse = true;
		pUi->col = col;
		pUi->pos = pos;
		pUi->move = move;
		pUi->nFont = nFont;
		pUi->nLife =
		pUi->nLifeMax = nLife;

		break;
	}
}
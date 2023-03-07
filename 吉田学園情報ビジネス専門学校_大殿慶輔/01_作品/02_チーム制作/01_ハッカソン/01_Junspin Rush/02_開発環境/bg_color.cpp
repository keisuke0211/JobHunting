//========================================
// 
// �w�i�F�̏���
// Author:���� ����
// 
//========================================
//  *** bg_color.cpp ***
//========================================
#include "main.h"
#include "bg_color.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterBg_color(void);

//****************************************
// �O���[�o���錾
//****************************************
Bg_color g_bg_color;	// �w�i�F�̏��

//========== *** �w�i�F�̏����擾 ***
Bg_color *GetBg_color(void) 
{
	return &g_bg_color;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_color - �p�����[�^�[�̏��������� -
//========================================
void InitParameterBg_color(void) 
{
	g_bg_color.col					= BG_START_COLOR;	// �F
	g_bg_color.oldColor				= { 0, 0, 0, 0 };	// �Â��F
	g_bg_color.newColor				= { 0, 0, 0, 0 };	// �V�����F
	g_bg_color.bChangeColor			= false;			// �ϐF�t���O
	g_bg_color.nCounterChangeColor	= 0;				// �ϐF�J�E���^�[
	g_bg_color.nTimeChangeColor		= 0;				// �ϐF�ɂ����鎞��
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitBg_color�֐� - �w�i�F�̏��������� -
//========================================
void InitBg_color(void)
{
	// �p�����[�^�[�̏���������
	InitParameterBg_color();
}

//========================================
// UninitBg_color�֐� - �w�i�F�̏I������ -
//========================================
void UninitBg_color(void)
{
	
}

//========================================
// UpdateBg_color�֐� - �w�i�F�̍X�V���� -
//========================================
void UpdateBg_color(void)
{
	Bg_color	*pBg			// 
				= &g_bg_color;	// �w�i�F�̏��

	if (pBg->bChangeColor) 
	{// �ϐF�t���O���^�̎��A
		// �ϐF�J�E���^�[�����Z
		pBg->nCounterChangeColor++;

		// �Â��F�̐�����ϐF�J�E���^�[�ɉ������������ݒ�
		pBg->col.nR = pBg->oldColor.nR * (1.0f - ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor));
		pBg->col.nG = pBg->oldColor.nG * (1.0f - ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor));
		pBg->col.nB = pBg->oldColor.nB * (1.0f - ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor));

		// �V�����F�̐�����ϐF�J�E���^�[�ɉ��������������Z
		pBg->col.nR += pBg->newColor.nR * ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor);
		pBg->col.nG += pBg->newColor.nG * ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor);
		pBg->col.nB += pBg->newColor.nB * ((float)pBg->nCounterChangeColor / (float)pBg->nTimeChangeColor);

		if (pBg->nCounterChangeColor >= pBg->nTimeChangeColor) 
		{// �ϐF�J�E���^�[���ϐF�ɂ����鎞�ԂɒB�������A
			// �ϐF�t���O���U�ɂ���
			pBg->bChangeColor = false;

			// �F��V�����F�ɕς���
			pBg->col = pBg->newColor;
		}
	}
}

//========================================
// SetBg_color�֐� - �w�i�F�̐ݒ菈�� -
//========================================
void SetBg_color(Color newColor, int nTime)
{
	Bg_color	*pBg			// 
				= &g_bg_color;	// �w�i�F�̏��

	if (nTime > 0) 
	{// �ϐF�ɂ����鎞�Ԃ�0�������Ă������A
		// �Â��F�ƐV�����F��ݒ�
		pBg->oldColor = pBg->col;
		pBg->newColor = newColor;

		// �ϐF�J�E���^�[��������
		pBg->nCounterChangeColor = 0;

		// �ϐF�ɂ����鎞�Ԃ���
		pBg->nTimeChangeColor = nTime;

		// �ϐF�t���O��^�ɂ���
		pBg->bChangeColor = true;
	}
	else 
	{// �ϐF�ɂ����鎞�Ԃ�0�ȉ��ł��������A
		// �F��V�����F�ɕς���
		pBg->col = newColor;
	}
}
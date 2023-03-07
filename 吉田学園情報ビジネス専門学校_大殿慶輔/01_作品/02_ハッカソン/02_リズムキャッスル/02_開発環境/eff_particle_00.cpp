//========================================
// 
// �p�[�e�B�N��[00] �̏���
// Author:���� ����
// 
//========================================
// *** eff_particle_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "eff_particle_00.h"
#include "eff_light_00.h"
#include "md_game_00.h"
#include "chunk.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
#define EFF_PARTICLE_00_MAX						(256)	// �p�[�e�B�N��[00] �̍ő吔
#define EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MIN	(0.2f)	// �p�[�e�B�N��[00] �̈ړ��͂ɂ����闐���{���̉���
#define EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MAX	(2.0f)	// �p�[�e�B�N��[00] �̈ړ��͂ɂ����闐���{���̏��
#define EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MIN	(0.5f)	// �p�[�e�B�N��[00] �̑傫���ɂ����闐���{���̉���
#define EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MAX	(2.0f)	// �p�[�e�B�N��[00] �̑傫���ɂ����闐���{���̏��

// �p�[�e�B�N��[00] �̎�ޖ��̏��
typedef struct
{
	float fWidth;	// ��
	float fHeight;	// ����
	float fMove;	// �ړ���
	int nLife;		// ����
	int nLightLife;	// ���C�g�̎���
	Color col;		// �F
	int nNum;		// ��
}Eff_particle_00Type;

//****************************************
// �v���g�^�C�v�錾 
//****************************************
void SetParameterEff_particle_00	(Eff_particle_00 *pEff);	// �p�[�e�B�N��[00] �̃p�����[�^�[����ނɉ����Đݒ�

//****************************************
// �O���[�o���錾
//****************************************
Eff_particle_00 g_aEff_particle_00[EFF_PARTICLE_00_MAX];	// �p�[�e�B�N��[00] �̏��

// ����[00] �̎�ނ̏��
Eff_particle_00Type g_aEff_particle_00Type[EFF_PARTICLE_00_TYPE_MAX] =
{//   ��        , ����      , �ړ��� , ���� , ���C�g�̎��� , �F
	{ PIXEL * 10 , PIXEL * 10 , 10.0f  , 35   , 5           , { 63, 191, 255 , 255 } , 2},
	{ PIXEL * 10 , PIXEL * 10 , 10.0f  , 35   , 5           , { 243, 191, 63 , 255 } , 2},
	{ PIXEL * 10 , PIXEL * 10 , 10.0f  , 35   , 5           , { 131, 211, 19 , 255 } , 2},
	{ PIXEL * 10 , PIXEL * 10 , 10.0f  , 35   , 5           , { 218, 39, 0 , 255 }	 , 2},
};

//========== *** �p�[�e�B�N��[00] �̏����擾 ***
Eff_particle_00 *GetEff_particle_00(void)
{
	return g_aEff_particle_00;
}

//========================================
// InitEff_particle_00�֐� - �p�[�e�B�N��[00] �̏��������� -
//========================================
void InitEff_particle_00(void)
{
	Eff_particle_00 *pEff = g_aEff_particle_00;	// �p�[�e�B�N��[00] �̏��̃|�C���^

	for (int nCntEff_particle_00 = 0; nCntEff_particle_00 < EFF_PARTICLE_00_MAX; nCntEff_particle_00++, pEff++)
	{
		pEff->bUse = false;						// �g�p����Ă��邩�t���O������������
		pEff->type = (EFF_PARTICLE_00_TYPE)0;	// ��ނ�ݒ肷��

		// �p�����[�^�[����ނɉ����Đݒ�
		SetParameterEff_particle_00(pEff);
	}
}

//========================================
// UpdateEff_particle_00�֐� - �p�[�e�B�N��[00] �̍X�V���� -
//========================================
void UpdateEff_particle_00(void)
{
	Eff_particle_00 *pEff = g_aEff_particle_00;	// �p�[�e�B�N��[00] �̏��̃|�C���^

	for (int nCntEff_particle_00 = 0; nCntEff_particle_00 < EFF_PARTICLE_00_MAX; nCntEff_particle_00++, pEff++)
	{
		if (!g_aEff_particle_00[nCntEff_particle_00].bUse)
		{// �p�[�e�B�N��[00] ���g�p����Ă��Ȃ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// �ʒu���X�V
		pEff->pos.x += pEff->move.x;
		pEff->pos.y += pEff->move.y;

		// �ړ��ʂ�����
		pEff->move.x += (0.0f - pEff->move.x) * 0.1f;
		pEff->move.y += (0.0f - pEff->move.y) * 0.1f;

		// �����̃J�E���g �����Z
		pEff->nLife--;

		if (pEff->nLife <= 0)
		{// �������s�������A
			// �g�p���Ă��Ȃ���Ԃɂ���
			pEff->bUse = false;
		}

		// ���C�g[00] ��ݒ�
		SetEff_light_00(pEff->pos, pEff->rot, 0.0f,
			g_aEff_particle_00Type[pEff->type].col,
			g_aEff_particle_00Type[pEff->type].fWidth
			* ((float)pEff->nLife / (float)g_aEff_particle_00Type[pEff->type].nLife),
			g_aEff_particle_00Type[pEff->type].fHeight
			* ((float)pEff->nLife / (float)g_aEff_particle_00Type[pEff->type].nLife),
			g_aEff_particle_00Type[pEff->type].nLightLife
			* ((float)pEff->nLife / (float)g_aEff_particle_00Type[pEff->type].nLife),
			EFF_LIGHT_00_TYPE_00);
	}
}

//========================================
// SetEff_particle_00�֐� - �p�[�e�B�N��[00]�̐ݒ菈�� -
//========================================
void SetEff_particle_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, EFF_PARTICLE_00_TYPE type)
{
	if (type == EFF_PARTICLE_00_TYPE_NONE) 
	{// ��ނ������̎��A
		// �������I������
		return;
	}

	Eff_particle_00 *pEff = g_aEff_particle_00;	// �p�[�e�B�N��[00] �̏��̃|�C���^
	int nSetNum = 0;

	for (int nCntEff_particle_00 = 0; nCntEff_particle_00 < EFF_PARTICLE_00_MAX; nCntEff_particle_00++, pEff++)
	{
		if (g_aEff_particle_00[nCntEff_particle_00].bUse)
		{// �p�[�e�B�N��[00]���g�p����Ă��鎞�A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// ��ނ���
		pEff->type = type;

		// �p�����[�^�[����ނɉ����Đݒ�
		SetParameterEff_particle_00(&g_aEff_particle_00[nCntEff_particle_00]);

		// �ʒu/�p�x ����
		pEff->pos = pos;
		pEff->rot = rot;

		// �����Ă�������Ɉړ��ʂ����Z
		pEff->move.x = sinf(-fRand(D3DX_PI) + fRand(D3DX_PI * 2)) * pEff->fMove;
		pEff->move.y = cosf(-fRand(D3DX_PI) + fRand(D3DX_PI * 2)) * pEff->fMove;

		// �g�p���Ă����Ԃɂ���
		pEff->bUse = true;

		nSetNum++;

		if (nSetNum >= 6)
		{
			break;
		}
	}
}

//========================================
// SetState�֐� - �p�[�e�B�N��[00] �̃p�����[�^�[����ނɉ����Đݒ� -
//========================================
void SetParameterEff_particle_00(Eff_particle_00 *pEff) 
{
	pEff->fWidth	= g_aEff_particle_00Type[pEff->type].fWidth;	// ��
	pEff->fHeight	= g_aEff_particle_00Type[pEff->type].fHeight;	// ����
	pEff->fMove		= g_aEff_particle_00Type[pEff->type].fMove;		// �ړ���
	pEff->nLife		= g_aEff_particle_00Type[pEff->type].nLife;		// ����

	// �ړ��͂ɗ����𔭐�������
	pEff->fMove *=
		EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MIN + fRand(EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MAX - EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MIN);

	// �傫���ɗ����𔭐�������
	{
		float fScaleDiameter =	// �傫���̔{��
			EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MIN + fRand(EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MAX - EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MIN);

		// ���ƍ����ɔ{����K�p������
		pEff->fWidth *= fScaleDiameter;
		pEff->fHeight *= fScaleDiameter;
	}
}
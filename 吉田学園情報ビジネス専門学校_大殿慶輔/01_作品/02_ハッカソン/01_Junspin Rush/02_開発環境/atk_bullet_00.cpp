//========================================
// 
// �e[00]�̏���
// Author:���� ����
// 
//========================================
// *** atk_bullet_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "physics.h"
#include "polygon.h"
#include "sound.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "attack.h"
#include "chr_enemy_00.h"
#include "eff_light_00.h"
#include "eff_particle_00.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
#define ATK_BULLET_00_ROTATION_TiME			(20)										// �e[00] �̉�]�ɂ����鎞��
#define ATK_BULLET_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ATK_BULLET_00_DATA.txt"	// �G[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X

//****************************************
// �v���g�^�C�v�錾 
//****************************************

// �p�����[�^�[�̏���������
void InitParameter(Atk_bullet_00 *pAtk);

// �p�����[�^�[����ނɉ����Đݒ�
void SetParameter(Atk_bullet_00 *pAtk);

// �����蔻�菈��
void Collision(Atk_bullet_00 *pAtk);

// �@��[00] �Ƃ̓����蔻�菈��
void CollisionChr_fighter_00(Atk_bullet_00 *pAtk);

// �G[00] �Ƃ̓����蔻�菈��
void CollisionChr_enemy_00(Atk_bullet_00 *pAtk);

// �p�[�e�B�N���̐ݒ菈��
void SetParticle(Atk_bullet_00 *pAtk);

// ���C�g�̐ݒ菈��
void SetLight(Atk_bullet_00 *pAtk);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureAtk_bullet_00							// �e�N�X�`���ւ̃|�C���^
						[ATK_BULLET_00_TYPE_MAX][ATK_PARENT_TYPE_MAX]	//
						= {};											//
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffAtk_bullet_00							// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;											//
Atk_bullet_00Type		g_aAtk_bullet_00Type							// �e[00] �̎�ޖ��̏��
						[ATK_BULLET_00_TYPE_MAX];						//
Atk_bullet_00			g_aAtk_bullet_00								// �e[00] �̏��
						[ATK_BULLET_00_MAX];							//

//========== *** �e[00] �̏����擾 ***
Atk_bullet_00 *GetAtk_bullet_00(void)
{
	return g_aAtk_bullet_00;
}

//========== *** �e[00] �̎�ޖ��̏����擾 ***
Atk_bullet_00Type *GetAtk_bullet_00Type(void)
{
	return g_aAtk_bullet_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameter�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameter(Atk_bullet_00 *pAtk)
{
	pAtk->pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pAtk->move				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	pAtk->fMove				= 0.0f;								// �ړ���
	pAtk->rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pAtk->drawRot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// +�`�����
	pAtk->fWidth			= 0.0f;								// ��
	pAtk->fHeight			= 0.0f;								// ����
	pAtk->fHitTestWidth		= 0.0f;								// �����蔻��̕�
	pAtk->fHitTestHeight	= 0.0f;								// �����蔻��̍���
	pAtk->nType				= 0;								// ���
	pAtk->parentType		= (ATK_PARENT_TYPE)0;				// �e�̎��
	pAtk->bUse				= false;							// �g�p����Ă��邩�t���O
	pAtk->nLife				= 0;								// ����
	pAtk->nDamage			= 0;								// �_���[�W
	pAtk->nPenetrationNum	= 0;								// �c��̊ђʉ�
	pAtk->nPattern			= 0;								// �p�^�[��No.
	pAtk->nCounterAnim		= 0;								// �A�j���[�V�����̃J�E���^�[
	pAtk->nCounterRotation	= 0;								// ��]�J�E���^�[
}

//========================================
// SetState�֐� - �p�����[�^�[����ނɉ����Đݒ� -
//========================================
void SetParameter(Atk_bullet_00 *pAtk)
{
	pAtk->fWidth			= g_aAtk_bullet_00Type[pAtk->nType].fWidth;			// ��
	pAtk->fHeight			= g_aAtk_bullet_00Type[pAtk->nType].fHeight;		// ����
	pAtk->fHitTestWidth		= g_aAtk_bullet_00Type[pAtk->nType].fHitTestWidth;	// �����蔻��̕�
	pAtk->fHitTestHeight	= g_aAtk_bullet_00Type[pAtk->nType].fHitTestHeight;	// �����蔻��̍���
	pAtk->fMove				= g_aAtk_bullet_00Type[pAtk->nType].fMove;			// �ړ���
	pAtk->nDamage			= g_aAtk_bullet_00Type[pAtk->nType].nDamage;		// �_���[�W
	pAtk->nPenetrationNum	= g_aAtk_bullet_00Type[pAtk->nType].nPenetration;	// �c��̊ђʉ�
	pAtk->nLife				= g_aAtk_bullet_00Type[pAtk->nType].nLife;			// ������ݒ�
}

//========================================
// Collision�֐� - �����蔻�菈�� -
//========================================
void Collision(Atk_bullet_00 *pAtk) 
{
	// �e�̎�ނɉ����������蔻�菈��
	switch (pAtk->parentType)
	{
		//========== *** �v���C���[ ***
	case ATK_PARENT_TYPE_PLAYER:

		// �G[00]�Ƃ̓����蔻�菈��
		CollisionChr_enemy_00(pAtk);

		if (g_aAtk_bullet_00Type[pAtk->nType].bHitAlly)
		{// �����ɂ������鎞�A
			// �@��[00]�Ƃ̓����蔻�菈��
			CollisionChr_fighter_00(pAtk);
		}

		break;
		//========== *** �G ***
	case ATK_PARENT_TYPE_ENEMY:

		// �@��[00]�Ƃ̓����蔻�菈��
		CollisionChr_fighter_00(pAtk);

		if (g_aAtk_bullet_00Type[pAtk->nType].bHitAlly) 
		{// �����ɂ������鎞�A
			// �G[00]�Ƃ̓����蔻�菈��
			CollisionChr_enemy_00(pAtk);
		}

		break;
	}
}

//========================================
// CollisionChr_fighter_00�֐� - �@��[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionChr_fighter_00(Atk_bullet_00 *pAtk)
{
	//if ((!GetChr_fighter_00()->bUse)
	//	|| (GetChr_fighter_00()->aState[CHR_FIGHTER_00_STATE_INVINCIBLE]))
	//{// �@��[00] ���g�p����Ă��Ȃ���Ԃ����G��Ԃ̎��A
	//	// �������I������
	//	return;
	//}

	//Chr_fighter_00	*pChr	// �@��[00] �̏��̃|�C���^
	//				= GetChr_fighter_00();

	//if (CheckHit(
	//	pAtk->pos,
	//	pAtk->rot,
	//	pAtk->fHitTestWidth,
	//	pAtk->fHeight,
	//	g_aAtk_bullet_00Type[pAtk->nType].hitTestType,
	//	pChr->pos,
	//	pChr->rot,
	//	pChr->fHitTestWidth,
	//	pChr->fHitTestHeight,
	//	GetChr_fighter_00Type()[pChr->nType].hitTestType))
	//{// �e[00] ���@��[00] �̏Փ˂������A
	//	// �@��[00] �̃q�b�g����
	//	HitChr_fighter_00(pAtk->nDamage);

	//	// �e[00] �̔j�󏈗�
	//	DestructionAtk_bullet_00(pAtk);

	//	// �p�[�e�B�N���̐ݒ菈��
	//	SetParticle(pAtk);
	//}
}

//========================================
// CollisionChr_enemy_00�֐� - �G[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionChr_enemy_00(Atk_bullet_00 *pAtk)
{
	Chr_enemy_00	*pChr	// �G[00] �̏��̃|�C���^
					= GetChr_enemy_00();

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if ((!pChr->bUse)
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_POP])
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
			|| (pChr->aState[CHR_ENEMY_00_STATE_SWELL])
			|| (pChr->aAct[CHR_ENEMY_00_ACT_GHOST])
			|| (pChr->aAct[CHR_ENEMY_00_ACT_BULLET_GHOST]))
		{// �g�p����Ă��Ȃ���ԁA�������͏o���������������c��݂����蔲���̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (CheckHit(
			pAtk->pos,
			pAtk->rot,
			pAtk->fHitTestWidth,
			pAtk->fHeight,
			HIT_TEST_TYPE_CIRCLE,
			pChr->pos,
			pChr->rot,
			pChr->fHitTestWidth,
			pChr->fHitTestHeight,
			HIT_TEST_TYPE_CIRCLE))
		{// �e[00] ���G[00] �̓����蔻����ɓ��������A
			// �G[00] �̃q�b�g����
			HitChr_enemy_00(pChr, pAtk->nDamage);

			// �e[00] �̔j�󏈗�
			DestructionAtk_bullet_00(pAtk);

			// �p�[�e�B�N���̐ݒ菈��
			SetParticle(pAtk);

			// �g�p���Ă��Ȃ���Ԃɂ���
			pAtk->bUse = false;

			break;
		}
	}
}

//========================================
// SetParticle�֐� - �p�[�e�B�N���̐ݒ菈�� -
//========================================
void SetParticle(Atk_bullet_00 *pAtk)
{
	// �p�[�e�B�N���̐ݒ菈��
	for (int nCntPtcl = 0; nCntPtcl < g_aAtk_bullet_00Type[pAtk->nType].nPtclNum; nCntPtcl++)
	{
		D3DXVECTOR3 setRot;	// �ݒ肷�����

		// �ݒ肷�������ݒ�
		setRot.z = pAtk->rot.z - (D3DX_PI * 0.25f) + fRand(D3DX_PI * 0.5f) + D3DX_PI;

		// �p�[�e�B�N��[00] ��ݒ�
		SetEff_particle_00(pAtk->pos, setRot, g_aAtk_bullet_00Type[pAtk->nType].aPtclType[pAtk->parentType]);
	}
}

//========================================
// SetLight�֐� - ���C�g�̐ݒ菈�� -
//========================================
void SetLight(Atk_bullet_00 *pAtk)
{
	// ���C�g[00] ��ݒ�
	SetEff_light_00(
		pAtk->pos, 
		pAtk->rot + pAtk->drawRot,
		g_aAtk_bullet_00Type[pAtk->nType].fLightMove,
		g_aAtk_bullet_00Type[pAtk->nType].aLightColor[pAtk->parentType],
		g_aAtk_bullet_00Type[pAtk->nType].fLightWidth,
		g_aAtk_bullet_00Type[pAtk->nType].fLightHeight,
		g_aAtk_bullet_00Type[pAtk->nType].nLightLife,
		EFF_LIGHT_00_TYPE_00);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadAtk_bullet_00�֐� - �e[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadAtk_bullet_00(void)
{
	FILE				*pFile;					// �t�@�C���|�C���^
	char				aDataSearch				//
						[TXT_MAX];				// �f�[�^�����p
	Atk_bullet_00Type	*pAtkType				//
						= g_aAtk_bullet_00Type;	// �e[00] �̎�ޖ��̏��

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(ATK_BULLET_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// �������ʎq��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if (strcmp(aDataSearch, "END") == 0)
		{// �t�@�C�������
			fclose(pFile);
			break;
		}

		if (aDataSearch[0] == '#')
		{// �R�����g��ƈ�v�������A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (strcmp(aDataSearch, "ATK_BULLET_00_TYPE") == 0)
		{// �e[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (strcmp(aDataSearch, "ATK_BULLET_00_TYPE_END") == 0)
				{// �e[00] �̎�ޖ��̏��̓ǂݍ��݂��I��
					pAtkType++;
					break;
				}//====================//
				else if (strcmp(aDataSearch, "�ǂݍ��ݖ�:") == 0)
				{// �ǂݍ��ݖ���ǂݍ���
					fscanf(pFile, "%s", pAtkType->aLoadName);
				}//====================//
				else if (strcmp(aDataSearch, "�e�N�X�`���̑��΃p�X(�v���C���[):") == 0)
				{// �e�N�X�`���̑��΃p�X(�v���C���[)��ǂݍ���
					fscanf(pFile, "%s", pAtkType->aTexturPath[ATK_PARENT_TYPE_PLAYER]);
				}//====================//
				else if (strcmp(aDataSearch, "�e�N�X�`���̑��΃p�X(�G):") == 0)
				{// �e�N�X�`���̑��΃p�X(�G)��ǂݍ���
					fscanf(pFile, "%s", pAtkType->aTexturPath[ATK_PARENT_TYPE_ENEMY]);
				}//====================//
				else if (strcmp(aDataSearch, "�p�^�[���̏��X:") == 0)
				{// �p�^�[�����X��ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nPtnMaxX);
				}//====================//
				else if (strcmp(aDataSearch, "�p�^�[���̏��Y:") == 0)
				{// �p�^�[�����Y��ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nPtnMaxY);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���p�^�[���̉���:") == 0)
				{// �A�j���p�^�[���̉�����ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nAnimPtnMin);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���p�^�[���̏��:") == 0)
				{// �A�j���p�^�[���̏����ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nAnimPtnMax);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���[�V�����̎���:") == 0)
				{// �A�j���[�V�����ɂ����鎞�Ԃ�ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nAnimTime);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���[�V�����̎��:") == 0)
				{// �A�j���[�V�����̎�ނ�ǂݍ���
					fscanf(pFile, "%s", aDataSearch); // ����

					// �A�j���[�V�����̎�ނ𕶎��񂩂�ǂݍ���
					StringLoadAnimType(aDataSearch, &pAtkType->animType);
				}//====================//
				else if (strcmp(aDataSearch, "��:") == 0)
				{// ����ǂݍ���
					fscanf(pFile, "%f", &pAtkType->fWidth);

					// �����s�N�Z���P�ʂŏC��
					pAtkType->fWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "����:") == 0)
				{// ������ǂݍ���
					fscanf(pFile, "%f", &pAtkType->fHeight);

					// �������s�N�Z���P�ʂŏC��
					pAtkType->fHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "�����̎��:") == 0)
				{// �����̎�ނ�ǂݍ���
					fscanf(pFile, "%d", &pAtkType->angleType);
				}//====================//
				else if (strcmp(aDataSearch, "�����蔻��̕�:") == 0)
				{// �����蔻��̕���ǂݍ���
					fscanf(pFile, "%f", &pAtkType->fHitTestWidth);

					// �����蔻����s�N�Z���P�ʂŏC��
					pAtkType->fHitTestWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "�����蔻��̍���:") == 0)
				{// �����蔻��̕���ǂݍ���
					fscanf(pFile, "%f", &pAtkType->fHitTestHeight);

					// �����蔻����s�N�Z���P�ʂŏC��
					pAtkType->fHitTestHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "�����ɂ�������:") == 0)
				{// �����ɂ������邩�t���O��ǂݍ���
					int nBTemp;	// �ꎞ�ۑ��p

					fscanf(pFile, "%d", &nBTemp);

					pAtkType->bHitAlly = nBTemp;
				}//====================//
				else if (strcmp(aDataSearch, "�ړ���:") == 0)
				{// �ړ��͂�ǂݍ���
					fscanf(pFile, "%f", &pAtkType->fMove);
				}//====================//
				else if (strcmp(aDataSearch, "��]��:") == 0)
				{// ��]�͂�ǂݍ���
					fscanf(pFile, "%f", &pAtkType->fRotation);
				}//====================//
				else if (strcmp(aDataSearch, "����:") == 0)
				{// ������ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nLife);
				}//====================//
				else if (strcmp(aDataSearch, "�ђʉ�:") == 0)
				{// �ђʉ񐔂�ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nPenetration);
				}//====================//
				else if (strcmp(aDataSearch, "�_���[�W:") == 0)
				{// �_���[�W��ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nDamage);
				}//====================//
				else if (strcmp(aDataSearch, "�ݒ莞�̃T�E���h:") == 0)
				{// �ݒ莞�̃T�E���h�̓ǂݍ��݂��J�n
					fscanf(pFile, "%s", aDataSearch); // ����

					// �T�E���h���x���𕶎��񂩂�ǂݍ���
					StringLoadSound(aDataSearch, &pAtkType->soundSet);
				}//====================//
				else if (strcmp(aDataSearch, "�����̎��:") == 0)
				{// �����̎�ނ�ǂݍ���
					fscanf(pFile, "%d", &pAtkType->exType);
				}//====================//
				else if (strcmp(aDataSearch, "���C�g�̐F(�v���C���[):") == 0)
				{// ���C�g�̐F(�v���C���[)��ǂݍ���
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_PLAYER].nR);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_PLAYER].nG);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_PLAYER].nB);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_PLAYER].nA);
				}//====================//
				else if (strcmp(aDataSearch, "���C�g�̐F(�G):") == 0)
				{// ���C�g�̐F(�G)��ǂݍ���
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_ENEMY].nR);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_ENEMY].nG);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_ENEMY].nB);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_ENEMY].nA);
				}//====================//
				else if (strcmp(aDataSearch, "���C�g�̕�:") == 0)
				{// ���C�g�̕���ǂݍ���
					fscanf(pFile, "%f", &pAtkType->fLightWidth);

					// ���C�g�̕����s�N�Z���P�ʂŏC��
					pAtkType->fLightWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "���C�g�̍���:") == 0)
				{// ���C�g�̍�����ǂݍ���
					fscanf(pFile, "%f", &pAtkType->fLightHeight);

					// ���C�g�̍������s�N�Z���P�ʂŏC��
					pAtkType->fLightHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "���C�g�̎���:") == 0)
				{// ���C�g�̎�����ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nLightLife);
				}//====================//
				else if (strcmp(aDataSearch, "���C�g�̈ړ���:") == 0)
				{// ���C�g�̈ړ��ʂ�ǂݍ���
					fscanf(pFile, "%d", &pAtkType->fLightMove);
				}//====================//
				else if (strcmp(aDataSearch, "�p�[�e�B�N���̎��(�v���C���[):") == 0)
				{// �p�[�e�B�N���̎��(�v���C���[)��ǂݍ���
					fscanf(pFile, "%d", &pAtkType->aPtclType[ATK_PARENT_TYPE_PLAYER]);
				}//====================//
				else if (strcmp(aDataSearch, "�p�[�e�B�N���̎��(�G):") == 0)
				{// �p�[�e�B�N���̎��(�G)��ǂݍ���
					fscanf(pFile, "%d", &pAtkType->aPtclType[ATK_PARENT_TYPE_ENEMY]);
				}//====================//
				else if (strcmp(aDataSearch, "�p�[�e�B�N���̔�����:") == 0)
				{// �p�[�e�B�N���̔�������ǂݍ���
					fscanf(pFile, "%d", &pAtkType->nPtclNum);
				}//====================//
			}
		}
	}
}

//========================================
// StringLoadAtk_bullet_00�֐� - �e[00] �̎�ނ𕶎��񂩂�ǂݍ��� -
//========================================
void StringLoadAtk_bullet_00(char aString[TXT_MAX], int *pType)
{
	Atk_bullet_00Type	*pAtkType	// �e[00] �̎�ޖ��̏��
						= GetAtk_bullet_00Type();

	for (int nCntAtkType = 0; nCntAtkType < ATK_BULLET_00_TYPE_MAX; nCntAtkType++, pAtkType++)
	{
		if (strcmp(aString, pAtkType->aLoadName) == 0)
		{// �ǂݍ��ݖ�����v�������A
			// �e�̎�ނ���
			*pType = nCntAtkType;
			break;
		}
	}
}

//========================================
// InitAtk_bullet_00�֐� - �e[00] �̏��������� -
//========================================
void InitAtk_bullet_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�ւ̃|�C���^
						= GetDevice();
	VERTEX_2D			*pVtx;	// ���_���ւ̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	for (int nCntAtk_bullet_00_type = 0; nCntAtk_bullet_00_type < ATK_BULLET_00_TYPE_MAX; nCntAtk_bullet_00_type++)
	{
		for (int nCntLookType = 0; nCntLookType < ATK_PARENT_TYPE_MAX; nCntLookType++)
		{
			D3DXCreateTextureFromFile(pDevice, g_aAtk_bullet_00Type[nCntAtk_bullet_00_type].aTexturPath[nCntLookType], &g_aTextureAtk_bullet_00[nCntAtk_bullet_00_type][nCntLookType]);
		}
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ATK_BULLET_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAtk_bullet_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// �e[00] �̏��̃|�C���^
					= g_aAtk_bullet_00;

	for (int nCntAtk_bullet_00 = 0; nCntAtk_bullet_00 < ATK_BULLET_00_MAX; nCntAtk_bullet_00++, pVtx += 4, pAtk++)
	{
		// �p�����[�^�[�����������A��ނɉ����Đݒ�
		InitParameter(pAtk);
		SetParameter(pAtk);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pAtk->pos,
			pAtk->rot,
			true,
			pAtk->fWidth,
			pAtk->fHeight,
			g_aAtk_bullet_00Type[pAtk->nType].angleType);

		// rhw/���_�J���[/�e�N�X�`�����W�̐ݒ�
		SetRHW(pVtx);
		SetVertexColor(pVtx, { 255,255,255,255 });
		SetTexturePosition(pVtx,
			pAtk->nPattern,
			g_aAtk_bullet_00Type[pAtk->nType].nPtnMaxX,
			g_aAtk_bullet_00Type[pAtk->nType].nPtnMaxY,
			NULL);
	}
	
	// ���_���W ���A�����b�N����
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// UninitAtk_bullet_00�֐� - �e[00] �̏I������ -
//========================================
void UninitAtk_bullet_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_TYPE_MAX; nCntAtk++)
	{
		for (int nCntParentType = 0; nCntParentType < ATK_PARENT_TYPE_MAX; nCntParentType++)
		{
			if (g_aTextureAtk_bullet_00[nCntAtk][nCntParentType] != NULL)
			{
				g_aTextureAtk_bullet_00[nCntAtk][nCntParentType]->Release();
				g_aTextureAtk_bullet_00[nCntAtk][nCntParentType] = NULL;
			}
		}
	}
	
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAtk_bullet_00 != NULL) 
	{
		g_pVtxBuffAtk_bullet_00->Release();
		g_pVtxBuffAtk_bullet_00 = NULL;
	}
}

//========================================
// UpdateAtk_bullet_00�֐� - �e[00] �̍X�V���� -
//========================================
void UpdateAtk_bullet_00(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// �e[00] �̏��̃|�C���^
					= g_aAtk_bullet_00;

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pVtx += 4, pAtk++)
	{
		if (!pAtk->bUse)
		{// �e[00] ���g�p����Ă��Ȃ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// �����̃J�E���g�����Z
		pAtk->nLife--;

		if (pAtk->nLife <= 0)
		{// �������s�������A
			// �e[00] �̔j�󏈗�
			DestructionAtk_bullet_00(pAtk);

			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// ���i����
		Driving(&pAtk->pos, pAtk->rot, &pAtk->move, pAtk->fMove);

		// �����蔻�菈��
		Collision(pAtk);

		// ��������i
		pAtk->drawRot.z += g_aAtk_bullet_00Type[pAtk->nType].fRotation;
		ControlAngle(&pAtk->drawRot.z);

		// ���C�g�̐ݒ菈��
		SetLight(pAtk);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pAtk->pos,
			pAtk->rot + pAtk->drawRot,
			true,
			pAtk->fWidth,
			pAtk->fHeight,
			g_aAtk_bullet_00Type[pAtk->nType].angleType);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&pAtk->nPattern,
				g_aAtk_bullet_00Type[pAtk->nType].nAnimPtnMin,
				g_aAtk_bullet_00Type[pAtk->nType].nAnimPtnMax,
				&pAtk->nCounterAnim,
				g_aAtk_bullet_00Type[pAtk->nType].nAnimTime,
				g_aAtk_bullet_00Type[pAtk->nType].animType),
			g_aAtk_bullet_00Type[pAtk->nType].nPtnMaxX,
			g_aAtk_bullet_00Type[pAtk->nType].nPtnMaxY,
			NULL);
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// DrawAtk_bullet_00�֐� - �e[00]�̕`�揈�� -
//========================================
void DrawAtk_bullet_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@ ���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAtk_bullet_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Atk_bullet_00	*pAtk	// �e[00] �̏��̃|�C���^
					= g_aAtk_bullet_00;

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++)
	{
		if (pAtk->bUse)
		{// �e[00] ���g�p����Ă��鎞�A
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureAtk_bullet_00[pAtk->nType][pAtk->parentType]);

			// �e[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAtk * 4, 2);
		}
	}
}

//========================================
// SetAtk_bullet_00�֐� - �e[00]�̐ݒ菈�� -
//========================================
void SetAtk_bullet_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_PARENT_TYPE parentType)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@ �����b�N���A���_���ւ̃|�C���^ ���擾
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// �e[00] �̏��̃|�C���^
		= g_aAtk_bullet_00;

	for (int nCntAtk_bullet_00 = 0; nCntAtk_bullet_00 < ATK_BULLET_00_MAX; nCntAtk_bullet_00++, pAtk++)
	{
		if (pAtk->bUse)
		{// �e[00]���g�p����Ă��鎞�A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (parentType == ATK_PARENT_TYPE_PLAYER)
		{// �e�̎�ނ��v���C���[�̎��A
			// �V���b�gSE���Đ�
			PlaySound(g_aAtk_bullet_00Type[nType].soundSet);
		}

		// �p�����[�^�[��������
		InitParameter(pAtk);

		pAtk->pos			= pos;			// �ʒu����
		pAtk->rot			= rot;			// ��������
		pAtk->nType			= nType;		// ��ނ���
		pAtk->parentType	= parentType;	// �e�̎�ނ���

		// �p�����[�^�[����ނɉ����Đݒ�
		SetParameter(&g_aAtk_bullet_00[nCntAtk_bullet_00]);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pAtk->pos,
			pAtk->rot,
			pAtk->fWidth,
			pAtk->fHeight,
			true,
			g_aAtk_bullet_00Type[pAtk->nType].angleType);

		// �g�p���Ă����Ԃɂ���
		pAtk->bUse = true;

		break;
	}

	// ���_���W ���A�����b�N����
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// DestructionAtk_bullet_00�֐� - �e[00] �̔j�󏈗� -
//========================================
void DestructionAtk_bullet_00(Atk_bullet_00 *pAtk)
{
	if (g_aAtk_bullet_00Type[pAtk->nType].exType != ATK_EXPLOSION_00_TYPE_NOT)
	{// �����̎�ނ������łȂ����A
		// �����̐ݒ菈��
		SetAtk_explosion_00(pAtk->pos, pAtk->rot, g_aAtk_bullet_00Type[pAtk->nType].exType, 0);
	}

	// �g�p���Ă��Ȃ���Ԃɂ���
	pAtk->bUse = false;
}

//========================================
// AllDestructionAtk_bullet_00�֐� - �e[00] �̑S�j�󏈗� -
//========================================
void AllDestructionAtk_bullet_00(ATK_PARENT_TYPE parentType) 
{
	Atk_bullet_00	*pAtk	// �e[00] �̏��̃|�C���^
					= g_aAtk_bullet_00;

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++)
	{
		if ((pAtk->bUse)
			&& (pAtk->parentType == parentType))
		{// �e[00] ���g�p����Ă��āA���e�̎�ނ���v���Ă��鎞�A
			// �j�󏈗�
			DestructionAtk_bullet_00(pAtk);
		}
	}
}
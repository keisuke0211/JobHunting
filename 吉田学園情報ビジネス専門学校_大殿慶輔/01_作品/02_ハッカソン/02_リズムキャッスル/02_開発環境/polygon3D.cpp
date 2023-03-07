//========================================
// 
// �|���S��(3D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon3D.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "light.h"
#include "polygon3D.h"
#include "sound.h"
#include <stdio.h>

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ���[�V����(3D)�̃A�j���[�V�����ǂݍ��ݏ���
// ���[�V����(3D)�̃A�j���[�V��������
void LoadMotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);
void MotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMotionAnim3D�֐� - ���[�V����(3D)�̃A�j���[�V�����ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadMotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
{
	// �T�E���h�̃A�j���[�V�����̓ǂݍ���
	for (int nCntSoundAnim = 0; nCntSoundAnim < motionSet.aMotion[nMotionNum].nSoundAnimNum; nCntSoundAnim++)
	{
		if (pPartsInfo->nCounterMotion == motionSet.aMotion[nMotionNum].aSoundAnim[nCntSoundAnim].nTime)
		{// ���[�V�����J�E���^�[���؂�ւ����ԂɒB�������A
			// �T�E���h���Đ�
			PlaySound(motionSet.aMotion[nMotionNum].aSoundAnim[nCntSoundAnim].nSound);
		}
	}

	// ���i���̃A�j���[�V�����̓ǂݍ���
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		PartsMotion3D	*pPartsMotion	// ���i���̃��[�V�������̃|�C���^
						= &motionSet.aMotion[nMotionNum].aPartsMotion[nCntParts];
		Parts3D			*pParts			// ���i�̏��̃|�C���^
						= &pPartsInfo->aParts[nCntParts];

		// �ʒu�̃A�j���[�V�����̓ǂݍ���
		for (int nCntPosAnim = 0; nCntPosAnim < pPartsMotion->nPosAnimNum; nCntPosAnim++)
		{
			PositionAnim3D	*pPosAnim	// �ʒu�A�j���[�V�����̏��̃|�C���^
							= &pPartsMotion->aPosAnim[nCntPosAnim];

			if (pPartsInfo->nCounterMotion != pPosAnim->nTime)
			{// ���[�V�����J�E���^�[���؂�ւ����ԂɒB���Ă��Ȃ����A
				continue;	// �J��Ԃ�������܂�Ԃ�
			}

			if ((pPosAnim->nTime == -1)
				||
				(pPosAnim->nMoveTime == 0))
			{// �؂�ւ����Ԃ�-1 or �ʒu�A�j���[�V�����ɂ����鎞�Ԃ�0�̎��A
				pParts->pos			=					// �ʒu��
				pParts->targetPos	= pPosAnim->pos;	// �ڕW�ʒu�ɃA�j���[�V�����ʒu����
			}
			else
			{// �؂�ւ����Ԃ�-1�łȂ����A
				pParts->posOld			= pParts->pos;			// ���̈ʒu��ݒ�
				pParts->targetPos		= pPosAnim->pos;		// �ڕW�ʒu����
				pParts->nPosAnimTime	= pPosAnim->nMoveTime;	// �ʒu�A�j���[�V�����ɂ����鎞�Ԃ���
				pParts->nCounterPosAnim = 0;					// �ʒu�A�j���[�V�����J�E���^�[��������
				pParts->bPosAnim		= true;					// �ʒu�A�j���[�V�����t���O��^�ɂ���
			}
		}

		// �����̃A�j���[�V�����̓ǂݍ���
		for (int nCntRotAnim = 0; nCntRotAnim < pPartsMotion->nRotAnimNum; nCntRotAnim++)
		{
			RotationAnim3D	*pRotAnim	// �����A�j���[�V�����̏��̃|�C���^
							= &pPartsMotion->aRotAnim[nCntRotAnim];

			if (pPartsInfo->nCounterMotion != pRotAnim->nTime)
			{// ���[�V�����J�E���^�[���؂�ւ����ԂɒB���Ă��Ȃ����A
				continue;	// �J��Ԃ�������܂�Ԃ�
			}

			if ((pRotAnim->nTime == -1)
				||
				(pRotAnim->nMoveTime == 0))
			{// �؂�ւ����Ԃ�-1 or �����A�j���[�V�����ɂ����鎞�Ԃ�0�̎��A
				pParts->rot			=					// ������
				pParts->targetRot	= pRotAnim->rot;	// �ڕW�����ɃA�j���[�V������������
			}
			else
			{// �؂�ւ����Ԃ�-1�łȂ����A
				pParts->rotOld			= pParts->rot;			// ���̌�����ݒ�
				pParts->targetRot		= pRotAnim->rot;		// �ڕW��������
				pParts->nRotAnimTime	= pRotAnim->nMoveTime;	// �����A�j���[�V�����ɂ����鎞�Ԃ���
				pParts->nCounterRotAnim = 0;					// �����A�j���[�V�����J�E���^�[��������
				pParts->bRotAnim		= true;					// �����A�j���[�V�����t���O��^�ɂ���
			}
		}
	}
}

//========================================
// MotionAnim3D�֐� - ���[�V����(3D)�̃A�j���[�V�������� -
// Author:RIKU NISHIMURA
//========================================
void MotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
{
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		Parts3D	*pParts	// ���i�̏��̃|�C���^
				= &pPartsInfo->aParts[nCntParts];

		if (pParts->bPosAnim)
		{// �ʒu�A�j���[�V�����t���O���^�̎��A
			if (++pParts->nCounterPosAnim >= pParts->nPosAnimTime)
			{// �ʒu�A�j���[�V�����J�E���^�[�����Z�������ʈʒu�A�j���[�V�����ɂ����鎞�Ԃ����������A
				pParts->bPosAnim = false;			// �ʒu�A�j���[�V�����t���O���U�ɂ���
				pParts->pos = pParts->targetPos;	// �ʒu�ɖڕW�ʒu����
			}
		}

		if (pParts->bRotAnim)
		{// �p�x�A�j���[�V�����t���O���^�̎��A
			if (++pParts->nCounterRotAnim >= pParts->nRotAnimTime)
			{// �����A�j���[�V�����J�E���^�[�����Z�������ʌ����A�j���[�V�����ɂ����鎞�Ԃ����������A
				pParts->bRotAnim = false;			// �����A�j���[�V�����t���O���U�ɂ���
				pParts->rot = pParts->targetRot;	// �����ɖڕW��������
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadParts3DSet�֐� - ���i�ݒ���(3D)�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];
	float	fAllScale;	// �S�̂̊g��{��

	// ���i����������
	pPartsSet->nPartsNum = 0;

	// ���i�ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "PARTSSET_END"))	{ break; }								// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "ALL_SCALE:"))	{ fscanf(pFile, "%f", &fAllScale); }	// �S�̂̊g��{��
		else if (!strcmp(aDataSearch, "PARTS"))			{
			Parts3DType	*pPartsType // ���i�̎�ޖ��̏��̃|�C���^
						= &pPartsSet->aPartsType[pPartsSet->nPartsNum];

			// ���i���̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "PARTS_END"))		{ pPartsSet->nPartsNum++; break; }					// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pPartsType->aModelPath); }	// ���f���̑��΃p�X
				else if (!strcmp(aDataSearch, "PARENT:"))		{ fscanf(pFile, "%d", &pPartsType->nParent); }		// �e�p�[�c
				else if (!strcmp(aDataSearch, "RELATIVE_POS:")) {			// �Œ�̑��Έʒu
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.x);	// X
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.y);	// Y
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "RELATIVE_ROT:")) {			// �Œ�̑��Ίp�x
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.x);	// X
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.y);	// Y
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.z);	// Z
					pPartsType->fixedRelativeRot.x *= D3DX_PI;				// �~�����␳
					pPartsType->fixedRelativeRot.y *= D3DX_PI;				// �~�����␳
					pPartsType->fixedRelativeRot.z *= D3DX_PI;				// �~�����␳
				}
				else if (!strcmp(aDataSearch, "ROT_RELATIVE_POS:")) {	// ��]���̑��Έʒu
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.x);	// X
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.y);	// Y
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.z);	// Z
				}
			}
		}
	}
}

//========================================
// InitParts3DInfo�֐� - ���i(3D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitParts3DInfo(Parts3DInfo *pPartsInfo)
{
	pPartsInfo->nCounterMotion	= 0;	// ���[�V�����J�E���^�[
	pPartsInfo->nMotion			= 0;	// ���[�V�����ԍ�

	for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++) 
	{
		pPartsInfo->aParts[nCntParts].posResult			= INITD3DXVECTOR3;	// �ʒu(����)
		pPartsInfo->aParts[nCntParts].pos				= INITD3DXVECTOR3;	// �ʒu
		pPartsInfo->aParts[nCntParts].rotResult			= INITD3DXVECTOR3;	// ����(����)
		pPartsInfo->aParts[nCntParts].rot				= INITD3DXVECTOR3;	// ����
		pPartsInfo->aParts[nCntParts].bPosAnim			= false;			// �ʒu�A�j���[�V�����t���O
		pPartsInfo->aParts[nCntParts].nCounterPosAnim	= 0;				// �ʒu�A�j���[�V�����J�E���^�[
		pPartsInfo->aParts[nCntParts].nPosAnimTime		= 0;				// �ʒu�A�j���[�V�����ɂ����鎞��
		pPartsInfo->aParts[nCntParts].posOld			= INITD3DXVECTOR3;	// ���̈ʒu
		pPartsInfo->aParts[nCntParts].targetPos			= INITD3DXVECTOR3;	// �ڕW�ʒu
		pPartsInfo->aParts[nCntParts].bRotAnim			= false;			// �p�x�A�j���[�V�����t���O
		pPartsInfo->aParts[nCntParts].nCounterRotAnim	= 0;				// �p�x�A�j���[�V�����J�E���^�[
		pPartsInfo->aParts[nCntParts].nRotAnimTime		= 0;				// �p�x�A�j���[�V�����ɂ����鎞��
		pPartsInfo->aParts[nCntParts].rotOld			= INITD3DXVECTOR3;	// ���̊p�x
		pPartsInfo->aParts[nCntParts].targetRot			= INITD3DXVECTOR3;	// �ڕW�p�x
	}
}

//========================================
// InitParts3DTransform�֐� - ���i(3D)�̃g�����X�t�H�[���̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet)
{
	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		= &pPartsInfo->aParts[nCntParts];		// ���i�̏��̃|�C���^
		Parts3DType *pPartsType = &pPartsSet->aPartsType[nCntParts];	// ���i�̎�ޖ��̏��̃|�C���^

		// �ʒu���X�V
		pParts->posResult = pPartsType->fixedRelativePos;
		pParts->rotResult = pPartsType->fixedRelativeRot;
	}
}

//========================================
// GetParts3DTransform�֐� - ���i(3D)�̃g�����X�t�H�[���̎擾���� -
// Author:RIKU NISHIMURA
//========================================
void GetParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet)
{
	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		= &pPartsInfo->aParts[nCntParts];		// ���i�̏��̃|�C���^
		Parts3DType *pPartsType = &pPartsSet->aPartsType[nCntParts];	// ���i�̎�ޖ��̏��̃|�C���^

		if (pParts->bPosAnim)
		{// �ʒu�A�j���[�V�����t���O���^�̎��A
			float	fRate	// �J�E���^�[�̐i�s��
					= (float)pParts->nCounterPosAnim / (float)pParts->nPosAnimTime;

			// �ʒu���X�V
			pParts->pos = (pParts->posOld * (1.0f - fRate)) + (pParts->targetPos * fRate);
		}

		// �ʒu(����)���X�V
		pParts->posResult = pParts->pos + pPartsType->fixedRelativePos;

		if (pParts->bRotAnim)
		{// �����A�j���[�V�����t���O���^�̎��A
			float	fRate	// �J�E���^�[�̐i�s��
					= (float)pParts->nCounterRotAnim / (float)pParts->nRotAnimTime;

			// �������X�V
			pParts->rot = (pParts->rotOld * (1.0f - fRate)) + (pParts->targetRot * fRate);
		}

		// ����(����)���X�V
		pParts->rotResult = pParts->rot + pPartsType->fixedRelativeRot;
	}
}

//========================================
// LoadMotion3DSet�֐� - ���[�V����(3D)�ݒ���̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadMotion3DSet(FILE *pFile, Motion3DSet *pMotionSet)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];

	// ���[�V��������������
	pMotionSet->nMotionNum = 0;

	// ���[�V�����ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "MOTIONSET_END")) { break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "MOTION"))		{
			Motion3D	*pMotion	// ���[�V�����̏��̃|�C���^
						= &pMotionSet->aMotion[pMotionSet->nMotionNum];
			int			nCntParts	// ���i�̃J�E���g
						= 0;

			// �T�E���h�̃A�j���[�V��������������
			pMotion->nSoundAnimNum = 0;

			// ���[�V�������̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "MOTION_END"))	{ pMotionSet->nMotionNum++;	break; }				// ���[�V�����������Z���A�ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "LOOP_TIME:"))	{ fscanf(pFile, "%d", &pMotion->nLoopTime); }		// ���[�v����
				else if (!strcmp(aDataSearch, "LOOP:"))			{ fscanf(pFile, "%d", &pMotion->bLoop); }			// ���[�v���邩�t���O
				else if (!strcmp(aDataSearch, "PARTS"))			{
					PartsMotion3D	*pPartsMotion	// ���i���̃��[�V�����̏��̃|�C���^
									= &pMotion->aPartsMotion[nCntParts];

					// ���i���̃��[�V�����̏��̓ǂݍ��݂��J�n
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if		(!strcmp(aDataSearch, "PARTS_END"))		{ nCntParts++;	break; }	// ���i�̃J�E���g�����Z���A�ǂݍ��݂��I��
						else if (!strcmp(aDataSearch, "POSITION_ANIM")) {
							// �ʒu�̃A�j���[�V�����̏��̓ǂݍ��݂��J�n
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // ����

								if (!strcmp(aDataSearch, "POSITION_ANIM_END")) { break; }								// �ǂݍ��݂��I��
								else if (!strcmp(aDataSearch, "TIME")) {												// �ʒu�̃A�j���[�V�������
									fscanf(pFile, "%d", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].nTime);		// �؂�ւ�����
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.z);		// Z
									fscanf(pFile, "%d", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].nMoveTime);	// ���B�ɂ����鎞��
									pPartsMotion->nPosAnimNum++;														// �ʒu�̃A�j���[�V�����������Z
								}
							}
						}
						else if (!strcmp(aDataSearch, "ROTATION_ANIM")) {
							// �p�x�̃A�j���[�V�����̏��̓ǂݍ��݂��J�n
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // ����

								if		(!strcmp(aDataSearch, "ROTATION_ANIM_END")) { break; }							// �ǂݍ��݂��I��
								else if (!strcmp(aDataSearch, "TIME"))				{									// �p�x�̃A�j���[�V�������
									fscanf(pFile, "%d", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].nTime);		// �؂�ւ�����
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z);		// Z
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x *= D3DX_PI;					// �~�����␳
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y *= D3DX_PI;					// �~�����␳
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z *= D3DX_PI;					// �~�����␳
									fscanf(pFile, "%d", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].nMoveTime);	// ���B�ɂ����鎞��
									pPartsMotion->nRotAnimNum++;														// �p�x�̃A�j���[�V�����������Z
								}
							}
						}
					}
				}
				else if (!strcmp(aDataSearch, "SOUND_ANIM")) {
					// �T�E���h�̃A�j���[�V�����̏��̓ǂݍ��݂��J�n
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if		(!strcmp(aDataSearch, "SOUND_ANIM_END"))	{ break; }					// �ǂݍ��݂��I��
						else if (!strcmp(aDataSearch, "TIME"))				{							// �T�E���h�̃A�j���[�V�������
							fscanf(pFile, "%d", &pMotion->aSoundAnim[pMotion->nSoundAnimNum].nTime);	// �؂�ւ�����
							fscanf(pFile, "%d", &pMotion->aSoundAnim[pMotion->nSoundAnimNum].nSound);	// �T�E���h�ԍ�
							pMotion->nSoundAnimNum++;													// �T�E���h�̃A�j���[�V�����������Z
						}
					}
				}
			}
		}
	}
}

//========================================
// UpdateMotion3D�֐� - ���[�V����(3D)�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
bool UpdateMotion3D(int nMotion, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
{
	if (motionSet.aMotion[nMotion].nLoopTime == -1)
	{// ���[�V�����̃��[�v���Ԃ�-1�̎��A
		// �������I������
		return false;
	}

	// ���[�V�����̃A�j���[�V����(3D)�ǂݍ��ݏ���
	LoadMotionAnim3D(nMotion, motionSet, pPartsInfo, partsSet);

	// ���[�V�����̃A�j���[�V��������
	MotionAnim3D(nMotion, motionSet, pPartsInfo, partsSet);

	bool	bMotionLoop	// ���[�V���������[�v�������t���O
			= false;

	if (++pPartsInfo->nCounterMotion > motionSet.aMotion[nMotion].nLoopTime)
	{// ���[�V�����J�E���^�[�����Z�������ʃ��[�v���ԂɒB�������A
		bMotionLoop = true;	// ���[�V���������[�v�������t���O��^�ɂ���

		if (motionSet.aMotion[nMotion].bLoop)
		{// ���[�v���邩�t���O���^�̎��A
			// ���[�V�����J�E���^�[�����[�v����
			IntLoopControl(&pPartsInfo->nCounterMotion, motionSet.aMotion[nMotion].nLoopTime, 0);
		}
		else
		{// ���[�v���邩�t���O���U�̎��A
			// ���[�V�����J�E���^�[�𐧌�
			IntControl(&pPartsInfo->nCounterMotion, motionSet.aMotion[nMotion].nLoopTime, 0);
		}
	}

	// ���[�V���������[�v�������t���O��Ԃ�
	return bMotionLoop;
}

//========================================
// SetMotion3D�֐� - ���[�V����(3D)�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetMotion3D(Parts3DInfo *pPartsInfo, int nMotion) 
{
	if (pPartsInfo->nMotion == nMotion) 
	{// ���ɂ��̃��[�V�����ԍ��ɐݒ肳��Ă��鎞�A
		return;	// �������I������
	}

	pPartsInfo->nMotion = nMotion;	// ���[�V�����ԍ�����
	pPartsInfo->nCounterMotion = 0;	// ���[�V�����J�E���^�[��������
}

//========================================
// SetVertexPos3D�֐� - ���_���W�̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight)
{
	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
	pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
}

//========================================
// SetNormalLine3D�֐� - �@���x�N�g���̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor) 
{
	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
}

//========================================
// SetVertexColor3D�֐� - ���_�F�̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor3D(VERTEX_3D *pVtx, Color col)
{
	// ���_�F�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// SetTexturePos3D�֐� - �e�N�X�`�����W�̐ݒ菈��(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
{
	if (nPtnMaxX * nPtnMaxY == 0)
	{// �p�^�[���̏����0�̎��A
		// �������I������
		return;
	}

	D3DXVECTOR3 pos;	// �ʒu

	if (pPos == NULL)
	{// �ʒu�̃|�C���^��NULL�̎��A
		// �ʒu��������
		pos = D3DXVECTOR3(0, 0, 0);
	}
	else
	{// �ʒu�̃|�C���^��NULL�Ŗ������A
		// �ʒu��-�␳���đ��
		pos = -*pPos;
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
}

//========================================
// SetMaterial�֐� - �}�e���A���̐ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col)
{
	D3DXCOLOR	mtlColTemp	// �}�e���A���̐F��ۑ�
				= pMat->Diffuse;

	// �}�e���A���̐F��ݒ�
	pMat->Diffuse.r = mtlColTemp.r * ((float)col.r / (float)255);
	pMat->Diffuse.g = mtlColTemp.g * ((float)col.g / (float)255);
	pMat->Diffuse.b = mtlColTemp.b * ((float)col.b / (float)255);
	pMat->Diffuse.a = mtlColTemp.a * ((float)col.a / (float)255);

	// �}�e���A���̐ݒ�
	pDevice->SetMaterial(pMat);

	// �}�e���A���̐F�����ɖ߂�
	pMat->Diffuse = mtlColTemp;
}
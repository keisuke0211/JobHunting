//========================================
// 
// �|���S��(2D)�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon2D.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "light.h"
#include "polygon2D.h"
#include "sound.h"
#include <stdio.h>

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ���[�V�����̃A�j���[�V�����ǂݍ��ݏ���
// ���[�V�����̃A�j���[�V��������
void LoadMotionAnim(int nMotionNum, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet);
void MotionAnim(int nMotionNum, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMotionAnim�֐� - ���[�V�����̃A�j���[�V�����ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadMotionAnim(int nMotionNum, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
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
		PartsMotion *pPartsMotion	// ���i���̃��[�V�������̃|�C���^
					= &motionSet.aMotion[nMotionNum].aPartsMotion[nCntParts];
		Parts2D		*pParts			// ���i�̏��̃|�C���^
					= &pPartsInfo->aParts[nCntParts];

		// �p�^�[��No.�̃A�j���[�V�����̓ǂݍ���
		for (int nCntPtnAnim = 0; nCntPtnAnim < pPartsMotion->nPtnAnimNum; nCntPtnAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aPtnAnim[nCntPtnAnim].nTime)
			{// ���[�V�����J�E���^�[���؂�ւ����ԂɒB�������A
				pParts->nPattern = pPartsMotion->aPtnAnim[nCntPtnAnim].nPattern;	// �p�^�[��No.����
			}
		}

		// �ʒu�̃A�j���[�V�����̓ǂݍ���
		for (int nCntPosAnim = 0; nCntPosAnim < pPartsMotion->nPosAnimNum; nCntPosAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aPosAnim[nCntPosAnim].nTime)
			{// ���[�V�����J�E���^�[���؂�ւ����ԂɒB�������A
				if (pPartsMotion->aPosAnim[nCntPosAnim].nTime == -1)
				{// �؂�ւ����Ԃ�-1�̎��A
					// ���Έʒu��ڕW�ʒu�ɐݒ�
					pParts->relativePos = pPartsMotion->aPosAnim[nCntPosAnim].pos;
				}
				else
				{// �؂�ւ����Ԃ�-1�łȂ����A
					if (pPartsMotion->aPosAnim[nCntPosAnim].nMoveTime == 0) 
					{// �ʒu�A�j���[�V�����ɂ����鎞�Ԃ�0�̎��A
						pParts->relativePos = pPartsMotion->aPosAnim[nCntPosAnim].pos;	// �ʒu��ڕW�ʒu�ɐݒ�
					}
					else
					{// �ʒu�A�j���[�V�����ɂ����鎞�Ԃ�0�łȂ����A
						pParts->posOld			= pParts->relativePos;								// ���̈ʒu��ݒ�
						pParts->targetPos		= pPartsMotion->aPosAnim[nCntPosAnim].pos;			// �ڕW�ʒu����
						pParts->nPosAnimTime	= pPartsMotion->aPosAnim[nCntPosAnim].nMoveTime;	// �ʒu�A�j���[�V�����ɂ����鎞�Ԃ���
						pParts->nCounterPosAnim = 0;												// �ʒu�A�j���[�V�����J�E���^�[��������
						pParts->bPosAnim		= true;												// �ʒu�A�j���[�V�����t���O��^�ɂ���
					}
				}
			}
		}

		// �p�x�̃A�j���[�V�����̓ǂݍ���
		for (int nCntRotAnim = 0; nCntRotAnim < pPartsMotion->nRotAnimNum; nCntRotAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aRotAnim[nCntRotAnim].nTime)
			{// ���[�V�����J�E���^�[���؂�ւ����ԂɒB�������A
				if (pPartsMotion->aPosAnim[nCntRotAnim].nTime == -1)
				{// �؂�ւ����Ԃ�-1�̎��A
					// ���Ίp�x��ڕW�p�x�ɐݒ�
					pParts->relativeRot = pPartsMotion->aRotAnim[nCntRotAnim].rot;
				}
				else
				{// �؂�ւ����Ԃ�-1�łȂ����A
					pParts->rotOld			= pParts->relativeRot;								// ���̊p�x��ݒ�
					pParts->targetRot		= pPartsMotion->aRotAnim[nCntRotAnim].rot;			// �ڕW�p�x����
					pParts->nRotAnimTime	= pPartsMotion->aRotAnim[nCntRotAnim].nMoveTime;	// �p�x�A�j���[�V�����ɂ����鎞�Ԃ���
					pParts->nCounterRotAnim = 0;												// �p�x�A�j���[�V�����J�E���^�[��������
					pParts->bRotAnim		= true;												// �p�x�A�j���[�V�����t���O��^�ɂ���
				}
				
			}
		}

		// �F�̃A�j���[�V�����̓ǂݍ���
		for (int nCntColorAnim = 0; nCntColorAnim < pPartsMotion->nColorAnimNum; nCntColorAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aColorAnim[nCntColorAnim].nTime)
			{// ���[�V�����J�E���^�[���؂�ւ����ԂɒB�������A
				if (pPartsMotion->aPosAnim[nCntColorAnim].nTime == -1)
				{// �؂�ւ����Ԃ�-1�̎��A
					// �F��ڕW�̐F�ɐݒ�
					pParts->col = pPartsMotion->aColorAnim[nCntColorAnim].col;
				}
				else
				{// �؂�ւ����Ԃ�-1�łȂ����A
					pParts->colOld				= pParts->col;											// ���̐F��ݒ�
					pParts->targetCol			= pPartsMotion->aColorAnim[nCntColorAnim].col;			// �ڕW�̐F����
					pParts->nColorAnimTime		= pPartsMotion->aColorAnim[nCntColorAnim].nMoveTime;	// �F�A�j���[�V�����ɂ����鎞�Ԃ���
					pParts->nCounterColorAnim	= 0;													// �F�A�j���[�V�����J�E���^�[��������
					pParts->bColorAnim			= true;													// �F�A�j���[�V�����t���O��^�ɂ���
				}
			}
		}

		// ���̃A�j���[�V�����̓ǂݍ���
		for (int nCntLightAnim = 0; nCntLightAnim < pPartsMotion->nLightAnimNum; nCntLightAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aLightAnim[nCntLightAnim].nTime)
			{// ���[�V�����J�E���^�[���؂�ւ����ԂɒB�������A
				if (pPartsMotion->aPosAnim[nCntLightAnim].nTime == -1)
				{// �؂�ւ����Ԃ�-1�̎��A
					pParts->lightColor		= pPartsMotion->aLightAnim[nCntLightAnim].col;		// ���̐F��ڕW�̌��̐F�ɐݒ�
					pParts->fLightLength	= pPartsMotion->aLightAnim[nCntLightAnim].fLength;	// ���̒�����ڕW�̌��̒����ɐݒ�
				}
				else
				{// �؂�ւ����Ԃ�-1�łȂ����A
					pParts->lightColorOld		= pParts->lightColor;									// ���̌��̐F��ݒ�
					pParts->targetLightColor	= pPartsMotion->aLightAnim[nCntLightAnim].col;			// �ڕW�̌��̐F����
					pParts->fLightLengthOld		= pParts->fLightLength;									// ���̌��̒�����ݒ�
					pParts->fTargetLightLength	= pPartsMotion->aLightAnim[nCntLightAnim].fLength;		// �ڕW�̌��̒�������
					pParts->nLightAnimTime		= pPartsMotion->aLightAnim[nCntLightAnim].nMoveTime;	// ���A�j���[�V�����ɂ����鎞�Ԃ���
					pParts->nCounterLightAnim	= 0;													// ���A�j���[�V�����J�E���^�[��������
					pParts->bLightAnim			= true;													// ���A�j���[�V�����t���O��^�ɂ���
				}
			}
		}
	}
}

//========================================
// MotionAnim�֐� - ���[�V�����̃A�j���[�V�������� -
// Author:RIKU NISHIMURA
//========================================
void MotionAnim(int nMotionNum, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		Parts2D	*pParts	// ���i�̏��̃|�C���^
				= &pPartsInfo->aParts[nCntParts];

		if (pParts->bPosAnim)
		{// �ʒu�A�j���[�V�����t���O���^�̎��A
			// �ʒu���X�V
			pParts->relativePos =
				(pParts->posOld * (1.0f - ((float)pParts->nCounterPosAnim / (float)pParts->nPosAnimTime)))
				+ (pParts->targetPos * ((float)pParts->nCounterPosAnim / (float)pParts->nPosAnimTime));

			if (++pParts->nCounterPosAnim > pParts->nPosAnimTime)
			{// �ʒu�A�j���[�V�����J�E���^�[�����Z�������ʈʒu�A�j���[�V�����ɂ����鎞�ԂɒB�������A
				// �ʒu�A�j���[�V�����t���O���U�ɂ���
				pParts->bPosAnim = false;

				// ���Έʒu��ڕW�ʒu�ɐݒ�
				pParts->relativePos = pParts->targetPos;
			}
		}

		if (pParts->bRotAnim)
		{// �p�x�A�j���[�V�����t���O���^�̎��A
			// �p�x���X�V
			pParts->relativeRot =
				(pParts->rotOld * (1.0f - ((float)pParts->nCounterRotAnim / (float)pParts->nRotAnimTime)))
				+ (pParts->targetRot * ((float)pParts->nCounterRotAnim / (float)pParts->nRotAnimTime));

			if (++pParts->nCounterRotAnim > pParts->nRotAnimTime)
			{// �p�x�A�j���[�V�����J�E���^�[�����Z�������ʊp�x�A�j���[�V�����ɂ����鎞�ԂɒB�������A
				// �p�x�A�j���[�V�����t���O���U�ɂ���
				pParts->bRotAnim = false;

				// ���Ίp�x��ڕW�p�x�ɐݒ�
				pParts->relativeRot = pParts->targetRot;
			}
		}

		if (pParts->bColorAnim)
		{// �F�A�j���[�V�����t���O���^�̎��A
			float fDiameter = (float)pParts->nCounterColorAnim / (float)pParts->nColorAnimTime;	// �J�E���^�[�̐i�s��

			// �F���X�V
			pParts->col.r = (pParts->colOld.r * (1.0f - fDiameter)) + (pParts->targetCol.r * fDiameter);
			pParts->col.g = (pParts->colOld.g * (1.0f - fDiameter)) + (pParts->targetCol.g * fDiameter);
			pParts->col.b = (pParts->colOld.b * (1.0f - fDiameter)) + (pParts->targetCol.b * fDiameter);
			pParts->col.a = (pParts->colOld.a * (1.0f - fDiameter)) + (pParts->targetCol.a * fDiameter);

			if (++pParts->nCounterColorAnim > pParts->nColorAnimTime)
			{// �F�A�j���[�V�����J�E���^�[�����Z�������ʐF�A�j���[�V�����ɂ����鎞�ԂɒB�������A
				pParts->bColorAnim	= false;				// �F�A�j���[�V�����t���O���U�ɂ���
				pParts->col			= pParts->targetCol;	// �F��ڕW�F�ɐݒ�
			}
		}

		if (pParts->bLightAnim)
		{// ���A�j���[�V�����t���O���^�̎��A
			float fDiameter = (float)pParts->nCounterLightAnim / (float)pParts->nLightAnimTime;	// �J�E���^�[�̐i�s��

			// ���̐F���X�V
			pParts->lightColor.r = (pParts->lightColorOld.r * (1.0f - fDiameter)) + (pParts->targetLightColor.r * fDiameter);
			pParts->lightColor.g = (pParts->lightColorOld.g * (1.0f - fDiameter)) + (pParts->targetLightColor.g * fDiameter);
			pParts->lightColor.b = (pParts->lightColorOld.b * (1.0f - fDiameter)) + (pParts->targetLightColor.b * fDiameter);
			pParts->lightColor.a = (pParts->lightColorOld.a * (1.0f - fDiameter)) + (pParts->targetLightColor.a * fDiameter);

			// ���̒������X�V
			pParts->fLightLength = (pParts->fLightLengthOld * (1.0f - fDiameter)) + (pParts->fTargetLightLength * fDiameter);

			if (++pParts->nCounterLightAnim > pParts->nLightAnimTime)
			{// ���A�j���[�V�����J�E���^�[�����Z�������ʌ��A�j���[�V�����ɂ����鎞�ԂɒB�������A
				pParts->bLightAnim		= false;						// ���A�j���[�V�����t���O���U�ɂ���
				pParts->lightColor		= pParts->targetLightColor;		// ���̐F��ڕW�̌��̐F�ɐݒ�
				pParts->fLightLength	= pParts->fTargetLightLength;	// ���̒�����ڕW�̌��̒����ɐݒ�
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
// GetPolygon2DNum�֐� - �|���S��(2D)�̕K�v����Ԃ� -
// Author:RIKU NISHIMURA
//========================================
int GetPolygon2DNum(const Polygon2D *pPoly2D, int nPolyMax)
{
	int nNum = 0;	// �|���S����

	// �|���S�����̍ő吔�����Z
	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++)
	{
		nNum += pPoly2D->nMax;
	}

	return nNum;
}

//========================================
// GetPolygon2DIndex�֐� - ���݂̃J�E���g�̃|���S��(2D)�̔ԍ���Ԃ� -
// Author:RIKU NISHIMURA
//========================================
int GetPolygon2DIndex(const Polygon2D *pPoly2D, int nPolyMax, int nCount)
{
	int nLine = 0;	// ���C��

	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++)
	{
		nLine += pPoly2D->nMax;	// ���C���ɍő吔�����Z

		if (nCount < nLine) 
		{// �J�E���g�����C�������̎��A�J�E���g�̒l��Ԃ�
			return nCntPoly;
		}
	}

	return 0;
}

//========================================
// LoadPolygon2DTexture�֐� - �|���S��(2D)�̃e�N�X�`���̓ǂݍ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadPolygon2DTexture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 *pTextureUi_map_00, const Polygon2D *pPoly2D, int nPolyMax) 
{
	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++, pTextureUi_map_00++)
	{
		D3DXCreateTextureFromFile(pDevice, pPoly2D->aTexturePath, pTextureUi_map_00);
	}
}

//========================================
// LoadParts2DSet�֐� - ���i�ݒ���(2D)�̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadParts2DSet(FILE *pFile, Parts2DSet *pPartsSet)
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

		if		(!strcmp(aDataSearch, "PARTSSET_END"))		{ break; }								// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "ALL_SCALE:"))		{ fscanf(pFile, "%f", &fAllScale); }	// �S�̂̊g��{��
		else if (!strcmp(aDataSearch, "PARTS"))				{
			float		fScale;		// �g��{��
			Parts2DType	*pPartsType // ���i�̎�ޖ��̏��̃|�C���^
						= &pPartsSet->aPartsType[pPartsSet->nPartsNum];
			
			pPartsType->initColor = { 255,255,255,255 };	// �����J���[��������
			pPartsType->nInitPtn = 0;						// �����p�^�[��No.��������

			// ���i���̓ǂݍ��݂��J�n
			while (1) 
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (!strcmp(aDataSearch, "PARTS_END")) {
					pPartsSet->nPartsNum++;									// ���i�������Z
					pPartsType->fWidth				*= fScale * fAllScale;	// ��				�Ɋg��{����K�p
					pPartsType->fHeight				*= fScale * fAllScale;	// ����				�Ɋg��{����K�p
					pPartsType->fixedRelativePos	*= fScale * fAllScale;	// �Œ�̑��Έʒu	�Ɋg��{����K�p
					pPartsType->rotRelativePos		*= fScale * fAllScale;	// ��]���̑��Έʒu	�Ɋg��{����K�p
					break;													// �ǂݍ��݂��I��
				}
				else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))	{ fscanf(pFile, "%s", &pPartsType->aTexturePath); }	// �e�N�X�`���̑��΃p�X
				else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pPartsType->fWidth); }		// ��
				else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pPartsType->fHeight); }		// ����
				else if (!strcmp(aDataSearch, "SCALE:"))		{ fscanf(pFile, "%f", &fScale); }					// �傫���ɂ�����{��
				else if (!strcmp(aDataSearch, "PTN_MAX_X:"))	{ fscanf(pFile, "%d", &pPartsType->nPtnMaxX); }		// �p�^�[�����X
				else if (!strcmp(aDataSearch, "PTN_MAX_Y:"))	{ fscanf(pFile, "%d", &pPartsType->nPtnMaxY); }		// �p�^�[�����Y
				else if (!strcmp(aDataSearch, "PARENT:"))		{ fscanf(pFile, "%d", &pPartsType->nParent); }		// �e�p�[�c
				else if (!strcmp(aDataSearch, "RELATIVE_POS:"))	{			// �Œ�̑��Έʒu
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
				else if (!strcmp(aDataSearch, "ANGLE_TYPE:")) { fscanf(pFile, "%d", &pPartsType->angleType); }		// �����̎��
				else if (!strcmp(aDataSearch, "ILLUMINATE:")) { fscanf(pFile, "%d", &pPartsType->bIlluminate); }	// ���̉e�����󂯂�t���O
				else if (!strcmp(aDataSearch, "INIT_COLOR:")) {												// �����J���[
					fscanf(pFile, "%d", &pPartsType->initColor.r);											// �ԓx
					fscanf(pFile, "%d", &pPartsType->initColor.g);											// �Γx
					fscanf(pFile, "%d", &pPartsType->initColor.b);											// �x
					fscanf(pFile, "%d", &pPartsType->initColor.a);											// �s�����x
				}
				else if (!strcmp(aDataSearch, "INIT_PATTERN:")) { fscanf(pFile, "%d", &pPartsType->nInitPtn); }	// �����̃p�^�[��No.
			}
		}
	}
}

//========================================
// InitParts2D�֐� - ���i�Ǘ�(2D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitParts2DInfo(Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		pPartsInfo->aParts[nCntParts].relativePos	= D3DXVECTOR3(0, 0, 0);
		pPartsInfo->aParts[nCntParts].relativeRot	= D3DXVECTOR3(0, 0, 0);
		pPartsInfo->aParts[nCntParts].bPosAnim		= false;
		pPartsInfo->aParts[nCntParts].bRotAnim		= false;
		pPartsInfo->aParts[nCntParts].bColorAnim	= false;
		pPartsInfo->aParts[nCntParts].bLightAnim	= false;
		pPartsInfo->aParts[nCntParts].col			= partsSet.aPartsType[nCntParts].initColor;
		pPartsInfo->aParts[nCntParts].fLightLength	= 0.0f;
		pPartsInfo->aParts[nCntParts].nPattern		= partsSet.aPartsType[nCntParts].nInitPtn;
	}
}

//========================================
// SetParts2DVertexPos�֐� - ���i(2D)�̒��_���W�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetParts2DVertexPos(VERTEX_2D *pVtxTemp, VERTEX_2D *pVtx, int nPartsNum, Parts2D *pParts, Parts2DInfo *pPartsInfo, Parts2DType *pPartsType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, bool bFlipHorizontal)
{
	D3DXVECTOR3 posTemp	// �ʒu��ۑ� 
				= pParts->pos;
	D3DXVECTOR3 rotTemp	// �p�x��ۑ� 
				= pParts->rot;

	if (pPartsType[nPartsNum].nParent != -1)
	{// �e�ԍ���-1�łȂ����A
		// �ʒu�Ɗp�x��e�̕��i�̂��̂ɐݒ�
		pParts->pos = pPartsInfo->aParts[pPartsType[nPartsNum].nParent].pos;
		pParts->rot = pPartsInfo->aParts[pPartsType[nPartsNum].nParent].rot;
	}
	else
	{// �e�ԍ���-1�̎��A
		// �ʒu�Ɗp�x���Ăяo�����̂��̂ɐݒ�
		pParts->pos = pos;
		pParts->rot = rot;
	}

	{
		Parts2DType	*pThisType			// ���݂̎�ޏ��
					= &pPartsType[nPartsNum];
		D3DXVECTOR3	fixedRelativePos	// �Œ�̑��Έʒu
					= pThisType->fixedRelativePos;
		D3DXVECTOR3	relativePos			// ���Έʒu
					= pParts->relativePos;
		D3DXVECTOR3	rotRelativePos		// ��]���̑��Έʒu
					= pThisType->rotRelativePos;
		D3DXVECTOR3 fixedRelativeRot	// �Œ�̑��Ίp�x
					= pThisType->fixedRelativeRot;
		D3DXVECTOR3 relativeRot			// ���Ίp�x
					= pParts->relativeRot;
		D3DXVECTOR3 rot					// �p�x
					= pParts->rot;

		if (bFlipHorizontal) 
		{// ���E���]�t���O���^�̎��A
			// ���E���]
			fixedRelativePos.x	*= -1;
			relativePos.x		*= -1;
			rotRelativePos.x	*= -1;
			fixedRelativeRot.z	*= -1;
			relativeRot.z		*= -1;
		}

		rot += fixedRelativeRot;																					// �p�x�ɕ��i�̌Œ�̑��Ίp�x�����Z
		pParts->pos.x += sinf(rot.z) * -(fixedRelativePos.y + relativePos.y + rotRelativePos.y);					// �ʒuX�ɕ��i�̑��Έʒu�ƈʒuY�Ɖ�]���̑��ΈʒuX��␳
		pParts->pos.y += cosf(rot.z) * -(fixedRelativePos.y + relativePos.y + rotRelativePos.y);					// �ʒuY�ɕ��i�̑��Έʒu�ƈʒuY�Ɖ�]���̑��ΈʒuY��␳
		pParts->pos.x += sinf(rot.z - (D3DX_PI * 0.5f)) * (fixedRelativePos.x + relativePos.x + rotRelativePos.x);	// �ʒuX�ɕ��i�̑��Έʒu�ƈʒuX�Ɖ�]���̑��ΈʒuX��␳
		pParts->pos.y += cosf(rot.z - (D3DX_PI * 0.5f)) * (fixedRelativePos.x + relativePos.x + rotRelativePos.x);	// �ʒuY�ɕ��i�̑��Έʒu�ƈʒuX�Ɖ�]���̑��ΈʒuY��␳
		rot += relativeRot;																							// �p�x�ɕ��i�̑��Ίp�x�����Z
		pParts->pos.x += sinf(rot.z) * (rotRelativePos.y);															// �ʒuX�ɉ�]���̑��ΈʒuY��␳
		pParts->pos.y += cosf(rot.z) * (rotRelativePos.y);															// �ʒuY�ɉ�]���̑��ΈʒuY��␳
		pParts->pos.x += sinf(rot.z - (D3DX_PI * 0.5f)) * -(rotRelativePos.x);										// �ʒuX�ɉ�]���̑��ΈʒuX��␳
		pParts->pos.y += cosf(rot.z - (D3DX_PI * 0.5f)) * -(rotRelativePos.x);										// �ʒuY�ɉ�]���̑��ΈʒuX��␳

		// �p�x����
		pParts->rot = rot;
	}

	// ���_���W�̐ݒ�
	SetVertexPos2D(pVtx,
		pParts->pos,
		pParts->rot,
		bMatchCamera,
		pPartsType[nPartsNum].fWidth * pParts->scale.x,
		pPartsType[nPartsNum].fHeight * pParts->scale.y,
		pPartsType[nPartsNum].angleType);

	if ((pParts->pos != posTemp)
		||
		(pParts->rot != rotTemp))
	{// �ʒu�A�������͊p�x���ϓ����Ă��鎞�A
		VERTEX_2D	*pVtxChild	// �q�̒��_���̃|�C���^�[
					= pVtxTemp;

		// �q���i�̒��_���W��ݒ肵�Ȃ���
		for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++, pVtxChild += 4)
		{
			if (pPartsType[nCntParts].nParent == nPartsNum)
			{// �J�E���g���ꂽ���i�̐e�ԍ��ƌ��݂̕��i�̔ԍ�����v���Ă��鎞�A
				// �q���i�̒��_���W�̐ݒ�
				SetParts2DVertexPos(pVtxTemp, pVtxChild,
					nCntParts,
					&pPartsInfo->aParts[nCntParts],
					pPartsInfo,
					pPartsType,
					pos,
					rot,
					true,
					bFlipHorizontal);
			}
		}
	}
}

//========================================
// SetParts2DVertexColor�֐� - ���i(2D)�̒��_�F�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetParts2DVertexColor(VERTEX_2D *pVtx, Parts2D parts,bool bIlluminate)
{
	if (bIlluminate)
	{// ���̉e�����󂯂�t���O���^�̎��A
		// ���_�J���[��SYS:��[00] �Ƃ̋����ɉ����Đݒ�
		SetVertexColor2D(pVtx, GetColorForLight2D(parts.pos, parts.col));
	}
	else
	{// ���̉e�����󂯂�t���O���U�̎��A
		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, parts.col);
	}
}

//========================================
// LoadMotionSet�֐� - ���[�V�����ݒ���̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadMotionSet(FILE *pFile, MotionSet *pMotionSet)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];
	float	fAllScale;	// �S�̂̊g��{��

	// ���[�V��������������
	pMotionSet->nMotionNum = 0;

	// ���[�V�����ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "MOTIONSET_END"))	{ break; }								// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "ALL_SCALE:"))	{ fscanf(pFile, "%f", &fAllScale); }	// �S�̂̊g��{��
		else if (!strcmp(aDataSearch, "MOTION"))		{
			Motion	*pMotion	// ���[�V�����̏��̃|�C���^
					= &pMotionSet->aMotion[pMotionSet->nMotionNum];
			int		nCntParts	// ���i�̃J�E���g
					= 0;

			// �T�E���h�̃A�j���[�V��������������
			pMotion->nSoundAnimNum = 0;

			// ���[�V�������̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "MOTION_END"))		{ pMotionSet->nMotionNum++;	break; }				// ���[�V�����������Z���A�ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "LOOP_TIME:"))		{ fscanf(pFile, "%d", &pMotion->nLoopTime); }		// ���[�v����
				else if (!strcmp(aDataSearch, "LOOP:"))				{ fscanf(pFile, "%d", &pMotion->bLoop); }			// ���[�v���邩�t���O
				else if (!strcmp(aDataSearch, "FLIP_HORIZONTAL:"))	{ fscanf(pFile, "%d", &pMotion->bFlipHorizontal); }	// ���E���]�t���O
				else if (!strcmp(aDataSearch, "PARTS"))				{
					PartsMotion *pPartsMotion	// ���i���̃��[�V�����̏��̃|�C���^
								= &pMotion->aPartsMotion[nCntParts];

					// ���i���̃��[�V�����̏��̓ǂݍ��݂��J�n
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if		(!strcmp(aDataSearch, "PARTS_END"))		{ nCntParts++;	break; }	// ���i�̃J�E���g�����Z���A�ǂݍ��݂��I��
						else if (!strcmp(aDataSearch, "PATTERN_ANIM"))	{
							// �p�^�[��No.�̃A�j���[�V�����̏��̓ǂݍ��݂��J�n
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // ����

								if		(!strcmp(aDataSearch, "PATTERN_ANIM_END"))	{ break; }							// �ǂݍ��݂��I��
								else if (!strcmp(aDataSearch, "TIME"))				{									// �p�^�[��No.�̃A�j���[�V�������
									fscanf(pFile, "%d", &pPartsMotion->aPtnAnim[pPartsMotion->nPtnAnimNum].nTime);		// �؂�ւ�����
									fscanf(pFile, "%d", &pPartsMotion->aPtnAnim[pPartsMotion->nPtnAnimNum].nPattern);	// �p�^�[��No.
									pPartsMotion->nPtnAnimNum++;														// �p�^�[��No.�̃A�j���[�V�����������Z
								}
							}
						}
						else if (!strcmp(aDataSearch, "POSITION_ANIM")) {
							// �ʒu�̃A�j���[�V�����̏��̓ǂݍ��݂��J�n
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // ����

								if (!strcmp(aDataSearch, "POSITION_ANIM_END"))	{ break; }								// �ǂݍ��݂��I��
								else if (!strcmp(aDataSearch, "TIME"))			{										// �ʒu�̃A�j���[�V�������
									fscanf(pFile, "%d", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].nTime);		// �؂�ւ�����
									fscanf(pFile, "%s", aDataSearch);													// [
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.z);		// Z
									pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos *= fAllScale;					// �ʒu�Ɋg��{����K�p
									fscanf(pFile, "%s", aDataSearch);													// ]
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
									fscanf(pFile, "%s", aDataSearch);													// [
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z);		// Z
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x *= D3DX_PI;					// �~�����␳
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y *= D3DX_PI;					// �~�����␳
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z *= D3DX_PI;					// �~�����␳
									fscanf(pFile, "%s", aDataSearch);													// ]
									fscanf(pFile, "%d", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].nMoveTime);	// ���B�ɂ����鎞��
									pPartsMotion->nRotAnimNum++;														// �p�x�̃A�j���[�V�����������Z
								}
							}
						}
						else if (!strcmp(aDataSearch, "COLOR_ANIM")) {
							// �F�̃A�j���[�V�����̏��̓ǂݍ��݂��J�n
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // ����

								if (!strcmp(aDataSearch, "COLOR_ANIM_END")) { break; }										// �ǂݍ��݂��I��
								else if (!strcmp(aDataSearch, "TIME"))		{												// �F�̃A�j���[�V�������
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].nTime);		// �؂�ւ�����
									fscanf(pFile, "%s", aDataSearch);														// [
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].col.r);		// �ԓx
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].col.g);		// �Γx
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].col.b);		// �x
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].col.a);		// �s�����x
									fscanf(pFile, "%s", aDataSearch);														// ]
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].nMoveTime);	// ���B�ɂ����鎞��
									pPartsMotion->nColorAnimNum++;															// �F�̃A�j���[�V�����������Z
								}
							}
						}
						else if (!strcmp(aDataSearch, "LIGHT_ANIM")) {
							// ���̃A�j���[�V�����̏��̓ǂݍ��݂��J�n
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // ����

								if (!strcmp(aDataSearch, "LIGHT_ANIM_END")) { break; }										// �ǂݍ��݂��I��
								else if (!strcmp(aDataSearch, "TIME")) {													// ���̃A�j���[�V�������
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].nTime);		// �؂�ւ�����
									fscanf(pFile, "%s", aDataSearch);														// [
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].col.r);		// �ԓx
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].col.g);		// �Γx
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].col.b);		// �x
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].col.a);		// �s�����x
									fscanf(pFile, "%s", aDataSearch);														// ]
									fscanf(pFile, "%f", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].fLength);	// ����
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].nMoveTime);	// ���B�ɂ����鎞��
									pPartsMotion->nLightAnimNum++;															// ���̃A�j���[�V�����������Z
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

						if		(!strcmp(aDataSearch, "SOUND_ANIM_END")) { break; }						// �ǂݍ��݂��I��
						else if (!strcmp(aDataSearch, "TIME")) {										// �T�E���h�̃A�j���[�V�������
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
// UpdateMotion�֐� - ���[�V�����̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
bool UpdateMotion(int nMotion, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	if (motionSet.aMotion[nMotion].nLoopTime == -1) 
	{// ���[�V�����̃��[�v���Ԃ�-1�̎��A
		// �������I������
		return false;
	}

	// ���[�V�����̃A�j���[�V�����ǂݍ��ݏ���
	LoadMotionAnim(nMotion, motionSet, pPartsInfo, partsSet);

	// ���[�V�����̃A�j���[�V��������
	MotionAnim(nMotion, motionSet, pPartsInfo, partsSet);

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
// SetMotion�֐� - ���[�V�����̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetMotion(int nMotion, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	if (nMotion == pPartsInfo->nMotion)
	{// �ݒ肷�郂�[�V�����ԍ������݂̃��[�V�����ԍ��ƈ�v���Ă��鎞�A
		// �����𔲂���
		return;
	}

	pPartsInfo->nMotion			= nMotion;	// ���[�V�����ԍ�����
	pPartsInfo->nCounterMotion	= -1;		// ���[�V�����J�E���^�[��-1�ɂ���
}

//========================================
// SetVertexPos2D�֐� - ���_���W�̐ݒ菈��(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType)
{
	if (bMatchCamera)
	{// �ʒu���J�����ɍ��킹��t���O���^�̎��A
		// �ʒu���J�����ɍ��킹��
		MatchCamera2DPosition(&pos);
	}

	if ((angleType == ANGLE_TYPE_FIXED)
		||
		(rot.z == D3DX_PI))
	{// �p�x�̎�ނ��Œ�A�������͊p�x���^��̎��A
		// ���_���W��ݒ�
		pVtx[0].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[0].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[1].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[1].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[2].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[2].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[3].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[3].pos.y = pos.y + (fHeight * 0.5f);
	}
	else if (rot.z == 0.0f)
	{// �p�x���^���̎��A
		// ���_���W��ݒ�
		pVtx[0].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[0].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[1].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[1].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[2].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[2].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[3].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[3].pos.y = pos.y - (fHeight * 0.5f);
	}
	else 
	{
		float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
		float fAngle = atan2f(fWidth, fHeight);									// �Ίp���̊p�x

		// ���_���W��ݒ�
		pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
		pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
	}
}

//========================================
// SetRHW2D�֐� - rhw�̐ݒ菈��(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetRHW2D(VERTEX_2D *pVtx) 
{
	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//========================================
// SetVertexColor2D�֐� - ���_�F�̐ݒ菈��(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor2D(VERTEX_2D *pVtx, Color col)
{
	// ���_�F�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// SetTexturePos2D�֐� - �e�N�X�`�����W�̐ݒ菈��(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
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
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
}

//========================================
// SetTexturePos2DCut�֐� - �e�N�X�`�����W�̐ݒ菈��(2D)(�؂���ver) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos)
{
	pos.x += 0.5f;
	pos.y += 0.5f;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(pos.x - (cutScale.fWidth * 0.5f), pos.y - (cutScale.fHeight * 0.5f));
	pVtx[1].tex = D3DXVECTOR2(pos.x + (cutScale.fWidth * 0.5f), pos.y - (cutScale.fHeight * 0.5f));
	pVtx[2].tex = D3DXVECTOR2(pos.x - (cutScale.fWidth * 0.5f), pos.y + (cutScale.fHeight * 0.5f));
	pVtx[3].tex = D3DXVECTOR2(pos.x + (cutScale.fWidth * 0.5f), pos.y + (cutScale.fHeight * 0.5f));
}

//========================================
// MatchPosition�֐� - �`��ʒu����ɍ��킹�� -
// Author:RIKU NISHIMURA
//========================================
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos)
{
	// �ʒu����̈ʒu�ɍ��킹��
	pDrawPos->x = pDrawPos->x - basePos.x + (SCREEN_WIDTH * 0.5f);
	pDrawPos->y = pDrawPos->y - basePos.y + (SCREEN_HEIGHT * 0.5f);
}
//========================================
// 
// �|���S���֘A�̏���
// Author:���� ����
// 
//========================================
// *** polygon.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "polygon.h"

//****************************************
// �O���[�o���錾
//****************************************

// �����̎�ޖ��̏��
AngleType g_aAngleType[ANGLE_TYPE_MAX] =
{// �ǂݍ��ݖ�
	"���R",
	"�Œ�",
};

// �A�j���[�V�����̎�ޖ��̏��
AnimType g_aAnimType[ANIM_TYPE_MAX] =
{// �ǂݍ��ݖ�
	"�ʏ�",
	"�܂�Ԃ�",
};

//========== *** �����̎�ޖ��̏����擾 ***
AngleType *GetAngleType(void) 
{
	return g_aAngleType;
}

//========== *** �A�j���[�V�����̎�ޖ��̏����擾 ***
AnimType *GetAnimType(void) 
{
	return g_aAnimType;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StringLoadAngleType�֐� - �����̎�ނ𕶎��񂩂�ǂݍ��� -
//========================================
void StringLoadAngleType(char aString[TXT_MAX], ANGLE_TYPE *pType)
{
	AngleType	*pAngleType	// �����̎�ޖ��̏��̃|�C���^
				= g_aAngleType;

	for (int nCntAngleType = 0; nCntAngleType < ANGLE_TYPE_MAX; nCntAngleType++, pAngleType++)
	{
		if (strcmp(aString, pAngleType->aLoadName) == 0)
		{// �ǂݍ��ݖ�����v�������A
			// �����̎�ނ���
			*pType = (ANGLE_TYPE)nCntAngleType;
			break;
		}
	}
}

//========================================
// StringLoadAnimType�֐� - �A�j���[�V�����̎�ނ𕶎��񂩂�ǂݍ��� -
//========================================
void StringLoadAnimType(char aString[TXT_MAX], ANIM_TYPE *pType)
{
	AnimType	*pAnimType	// �A�j���[�V�����̎�ޖ��̏��̃|�C���^
				= g_aAnimType;

	for (int nCntAnimType = 0; nCntAnimType < ANIM_TYPE_MAX; nCntAnimType++, pAnimType++)
	{
		if (strcmp(aString, pAnimType->aLoadName) == 0)
		{// �ǂݍ��ݖ�����v�������A
			// �A�j���[�V�����̎�ނ���
			*pType = (ANIM_TYPE)nCntAnimType;
			break;
		}
	}
}

//========================================
// SetVertexPosition�֐� - ���_���W�̐ݒ菈�� -
//========================================
void SetVertexPosition(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType)
{
	if (bMatchCamera)
	{// �ʒu���J�����ɍ��킹��t���O���^�̎��A
		// �ʒu���J�����ɍ��킹��
		MatchCameraPosition(&pos);
	}

	if ((angleType == ANGLE_TYPE_FIXED)
		||
		(rot.z == D3DX_PI))
	{// �p�x�̎�ނ��Œ�A�������͌������^��̎��A
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
	{// �������^���̎��A
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
// SetRHW�֐� - rhw�̐ݒ菈�� -
//========================================
void SetRHW(VERTEX_2D *pVtx) 
{
	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//========================================
// SetColor�֐� - ���_�F�̐ݒ菈�� -
//========================================
void SetVertexColor(VERTEX_2D *pVtx, Color col)
{
	// ���_�F�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
	pVtx[1].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
	pVtx[2].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
	pVtx[3].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
}

//========================================
// SetTexture�֐� - �e�N�X�`�����W�̐ݒ菈�� -
//========================================
void SetTexturePosition(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 pos;	// �ʒu

	if (pPos == NULL) 
	{
		pos = D3DXVECTOR3(0, 0, 0);
	}
	else 
	{
		pos = -*pPos;
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
}

//========================================
// AnimationCount�֐� - �A�j���[�V�����̃J�E���g���� -
//========================================
int AnimationCount(int *pPtn, int nPtnMin, int nPtnMax, int *pCounterAnim, int nAnimTime, ANIM_TYPE animType) 
{
	if (nAnimTime != -1) 
	{// �A�j���[�V�����ɂ����鎞�Ԃ�-1�łȂ����A
		// �A�j���[�V�����̃J�E���^�[�����Z
		*pCounterAnim += 1;

		if (*pCounterAnim >= nAnimTime)
		{// �A�j���[�V�����̃J�E���^�[���A�j���[�V�����ɂ����鎞�ԂɒB�������A
			// �A�j���[�V�����̃J�E���^�[������������
			*pCounterAnim = 0;

			// �p�^�[��No.�����Z
			*pPtn += 1;
		}
	}
	else 
	{// �A�j���[�V�����ɂ����鎞�Ԃ�-1�̎��A
		// �p�^�[��No.�����̂܂ܕԂ�
		return *pPtn;
	}

	// �A�j���[�V�����̎�ނɉ������p�^�[��No.��Ԃ�
	switch (animType)
	{
		//========== *** �ʏ� ***
	case ANIM_TYPE_NORMAL:

		// �p�^�[��No.������ŏ�]�Z�������ʂ�Ԃ�
		return nPtnMin + (*pPtn %= (nPtnMax - nPtnMin + 1));

		//========== *** �܂�Ԃ� ***
	case ANIM_TYPE_TURN:
		
		if ((*pPtn / (nPtnMax - nPtnMin)) % 2 == 0)
		{// �p�^�[��No.������ŏ��Z�������ʂ��������������A
			// �p�^�[��No.������ŏ�]�Z�������ʂ�Ԃ�
			return nPtnMin + (*pPtn %= (nPtnMax - nPtnMin));
		}
		else 
		{//�p�^�[��������ŏ��Z�������ʂ�����������A
			//�������p�^�[��������ŏ�]�Z�������ʂ����Z�������ʂ�Ԃ�
			return nPtnMax - (*pPtn % (nPtnMax - nPtnMin));
		}
	}
}
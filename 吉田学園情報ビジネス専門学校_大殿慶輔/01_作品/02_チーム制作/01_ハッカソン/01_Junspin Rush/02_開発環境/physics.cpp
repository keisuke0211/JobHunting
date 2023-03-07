//========================================
// 
// �����֘A�̏���
// Author:���� ����
// 
//========================================
// *** physics.cpp ***
//========================================
#include "main.h"
#include "physics.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �~���m�̏Փ˃`�F�b�N
bool CheckHitCircleAndCircle(
	D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth1, float fHeight1,
	D3DXVECTOR3 pos2, D3DXVECTOR3 rot2, float fWidth2, float fHeight2);

//****************************************
// �O���[�o���錾
//****************************************

// �����蔻��̎�ޖ��̏��
HitTestType g_aHitTestType[HIT_TEST_TYPE_MAX] =
{// �ǂݍ��ݖ�
	"�~�`",
	"�l�p�`"
};

//========== *** �����蔻��̎�ޖ��̏����擾 ***
HitTestType *GetHitTestType(void) 
{
	return g_aHitTestType;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// CheckHitCircleAndCircle�֐� - �~���m�̏Փ˃`�F�b�N -
//========================================
bool CheckHitCircleAndCircle(
	D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth1, float fHeight1, 
	D3DXVECTOR3 pos2, D3DXVECTOR3 rot2, float fWidth2, float fHeight2)
{
	if (FindDistance(pos1, pos2) < fWidth1 + fWidth2) 
	{// 2�_�̋����������蔻��̕��̍��v��������Ă��鎞�A
		// �^��Ԃ�
		return true;
	}
	else 
	{// 2�_�̋����������蔻��̕��̍��v��������Ă��Ȃ����A
		// �U��Ԃ�
		return false;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StringLoadHitTestType�֐� - �����蔻��̎�ނ𕶎��񂩂�ǂݍ��� -
//========================================
void StringLoadHitTestType(char aString[TXT_MAX], HIT_TEST_TYPE *pType)
{
	HitTestType	*pHitTestType	// �����蔻��̎�ޖ��̏��̃|�C���^
				= g_aHitTestType;

	for (int nCntHitTestType = 0; nCntHitTestType < HIT_TEST_TYPE_MAX; nCntHitTestType++, pHitTestType++)
	{
		if (strcmp(aString, pHitTestType->aLoadName) == 0)
		{// �ǂݍ��ݖ�����v�������A
			// �����蔻��̎�ނ���
			*pType = (HIT_TEST_TYPE)nCntHitTestType;
			break;
		}
	}
}

//========================================
// CheckHit�֐� - �Փ˃`�F�b�N -
//========================================
bool CheckHit(
	D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth1, float fHeight1, HIT_TEST_TYPE hitTestType1, 
	D3DXVECTOR3 pos2, D3DXVECTOR3 rot2, float fWidth2, float fHeight2, HIT_TEST_TYPE hitTestType2)
{
	if ((hitTestType1 == HIT_TEST_TYPE_CIRCLE)
		&& (hitTestType2 == HIT_TEST_TYPE_CIRCLE))
	{// �����蔻��̎�ނ��ǂ�����~���������A
		// �~���m�̏Փ˃`�F�b�N
		return CheckHitCircleAndCircle(
			pos1, rot1, fWidth1, fHeight1,
			pos2, rot2, fWidth2, fHeight2);
	}
}

//========================================
// PositionUpdate�֐� - �ʒu�̍X�V���� -
//========================================
void PositionUpdate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove) 
{
	// �ʒu���X�V
	pPos->x += pMove->x;
	pPos->y += pMove->y;

	// �ړ��ʂ��X�V(����)
	pMove->x += (0.0f - pMove->x) * 0.1f;
	pMove->y += (0.0f - pMove->y) * 0.1f;
}

//========================================
// Driving�֐� - ���i���� -
//========================================
void Driving(D3DXVECTOR3 *pPos, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fMove)
{
	// �����Ă�������Ɉړ��ʂ����Z
	pMove->x = sinf(rot.z) * fMove;
	pMove->y = cosf(rot.z) * fMove;

	// �ʒu���X�V
	PositionUpdate(pPos, pMove);
}

//========================================
// TurnAround�֐� - �����]������ -
//========================================
void TurnAround(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation)
{
	// �p�x�𐧌䂷��
	ControlAngle(&pRot->z);

	float fAngleDiff	// 2�_�̊p�x�̍�
		= AngleDifference(
			pRot->z,
			FindAngle(pos, targetPos));

	if (fAngleDiff < 0.0f) 
	{// �p�x�̍����}�C�i�X�������������A
		// �}�C�i�X�����ɉ�]
		pRot->z -= fRotation;
	}
	if (fAngleDiff > 0.0f)
	{// �p�x�̍����v���X�������������A
		// �v���X�����ɉ�]
		pRot->z += fRotation;
	}
}

//========================================
// FindDistance�֐� - 2�_�̋��������߂� -
//========================================
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float x = pos1.x - pos2.x;	// ��������X��
	float y = pos1.y - pos2.y;	// ��������Y��

	// 2�_�̋�����Ԃ�
	return sqrtf((x * x) + (y * y));
}

//========================================
// FindAngle�֐� - 2�_�̊p�x�����߂� -
//========================================
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos)
{
	float fAngle;	// �p�x

	fAngle = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1;

	return fAngle;
}

//========================================
// AngleDifference�֐� - �p�x�̍������߂� -
//========================================
float AngleDifference(float fAngle, float fTargetAngle)
{
	float fAngleDifference = 0.0f;	// �p�x�̍�

	// �p�x�𐧌䂷��
	ControlAngle(&fAngle);

	if ((fAngle >= 0.0f) && (fTargetAngle >= 0.0f))
	{// �ǂ���̊p�x���v���X�̎��A
		// �p�x�̍���ݒ�
		fAngleDifference = fTargetAngle - fAngle;

		return fAngleDifference;
	}
	else if ((fAngle <= 0.0f) && (fTargetAngle <= 0.0f))
	{// �ǂ���̊p�x���}�C�i�X�̎��A
		// �p�x�̍���ݒ�
		fAngleDifference = fTargetAngle - fAngle;

		return fAngleDifference;
	}

	if ((fAngle > 0.0f) && (fTargetAngle < 0.0f))
	{// �p�x���v���X�ŁA�ڕW�p�x���}�C�i�X�̎��A
		if (fAngle - D3DX_PI < fTargetAngle)
		{// �ڕW�p�x�������ɋ߂����A
			// �p�x�̍���ݒ�
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle - D3DX_PI > fTargetAngle)
		{// �ڕW�p�x���㑤�ɋ߂����A
			// �p�x�̍���ݒ�
			fAngleDifference = (D3DX_PI - fAngle) + (D3DX_PI + fTargetAngle);
		}
	}

	if ((fAngle < 0.0f) && (fTargetAngle > 0.0f))
	{// �p�x���}�C�i�X�ŁA�ڕW�p�x���v���X�̎��A
		if (fAngle + D3DX_PI > fTargetAngle)
		{// �ڕW�p�x�������ɋ߂����A
			// �p�x�̍���ݒ�
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle + D3DX_PI < fTargetAngle)
		{// �ڕW�p�x���㑤�ɋ߂����A
			// �p�x�̍���ݒ�
			fAngleDifference = -(D3DX_PI + fAngle) - (D3DX_PI - fTargetAngle);
		}
	}

	return fAngleDifference;
}

//========================================
// ControlAngle�֐� - �p�x�𐧌䂷�� -
//========================================
void ControlAngle(float *pAngle)
{
	if (*pAngle > D3DX_PI)
	{// �v���X�̉~�����������Ă��鎞�A
		*pAngle = (D3DX_PI - (*pAngle - D3DX_PI)) * -1;
	}
	else if (*pAngle < -D3DX_PI)
	{// �}�C�i�X�̉~������������Ă��鎞�A
		*pAngle = (D3DX_PI + (*pAngle + D3DX_PI));
	}
}
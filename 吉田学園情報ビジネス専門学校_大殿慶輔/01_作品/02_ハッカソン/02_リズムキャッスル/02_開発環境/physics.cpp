//========================================
// 
// �����֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** physics.cpp ***
//========================================
#include "chunk.h"
#include "main.h"
#include "physics.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// �����蔻�莞�̏����̈�
#define HIT_TEST_DECIMAL_PLACE	(20)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �l�p����l�p�̏Փ˃`�F�b�N
void SquareSquareCollision(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo	, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo	, HitTestParts pairHitTestParts);

// �l�p����l�p�̏Փ˃`�F�b�N(�����낵���[�h)
void SquareSquareCollisionLookDown(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo, HitTestParts pairHitTestParts);

// �_����l�p�̏Փ˃`�F�b�N(�����낵���[�h)
void PointSquareCollisionLookDown(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo, HitTestParts pairHitTestParts,
	bool bRflc, bool bSlip);

// ���ˊp�����߂�(�����낵)
float FindReflectionAngleLookDown(D3DXVECTOR3 point, D3DXVECTOR3 posOld, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadVector�֐� - �x�N�g���̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadVector(FILE *pFile, D3DXVECTOR3 *pVector)
{
	fscanf(pFile, "%f", &pVector->x);
	fscanf(pFile, "%f", &pVector->y);
	fscanf(pFile, "%f", &pVector->z);
}

//========================================
// SquareSquareCollision�֐� - �l�p����l�p�̏Փ˃`�F�b�N -
// Author:RIKU NISHIMURA
//========================================
void SquareSquareCollision(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo	, HitTestParts myHitTestParts, 
	CollisionInfo pairCollInfo	, HitTestParts pairHitTestParts)
{
	HitTest myHitTest	// �����̓����蔻����
			= myCollInfo.hitTest;
	HitTest pairHitTest	// ����̓����蔻����
			= pairCollInfo.hitTest;

	// �����蔻��̌v�Z�ɕK�v�ȃp�����[�^�[�𐮐��^�ɕϊ�(���Z�덷�΍�)
	int nMyX		= myCollInfo.pPos->x					* HIT_TEST_DECIMAL_PLACE;
	int nMyY		= myCollInfo.pPos->y					* HIT_TEST_DECIMAL_PLACE;
	int nMyPartsX	= myHitTestParts.fixedRelativePos.x		* HIT_TEST_DECIMAL_PLACE;
	int nMyPartsY	= myHitTestParts.fixedRelativePos.y		* HIT_TEST_DECIMAL_PLACE;
	int nMyOldX		= myCollInfo.posOld.x					* HIT_TEST_DECIMAL_PLACE;
	int nMyOldY		= myCollInfo.posOld.y					* HIT_TEST_DECIMAL_PLACE;
	int nMyWidth	= myHitTestParts.fWidth					* HIT_TEST_DECIMAL_PLACE;
	int nMyHeight	= myHitTestParts.fHeight				* HIT_TEST_DECIMAL_PLACE;
	int nPairX		= pairCollInfo.pPos->x					* HIT_TEST_DECIMAL_PLACE;
	int nPairY		= pairCollInfo.pPos->y					* HIT_TEST_DECIMAL_PLACE;
	int nPairPartsX = pairHitTestParts.fixedRelativePos.x	* HIT_TEST_DECIMAL_PLACE;
	int nPairPartsY = pairHitTestParts.fixedRelativePos.y	* HIT_TEST_DECIMAL_PLACE;
	int nPairOldX	= pairCollInfo.posOld.x					* HIT_TEST_DECIMAL_PLACE;
	int nPairOldY	= pairCollInfo.posOld.y					* HIT_TEST_DECIMAL_PLACE;
	int nPairWidth	= pairHitTestParts.fWidth				* HIT_TEST_DECIMAL_PLACE;
	int nPairHeight = pairHitTestParts.fHeight				* HIT_TEST_DECIMAL_PLACE;

	if (((nMyY + nMyPartsY) + (nMyHeight * 0.5f) > (nPairY + nPairPartsY) - (nPairHeight * 0.5f))
		&&
		((nMyY + nMyPartsY) - (nMyHeight * 0.5f) < (nPairY + nPairPartsY) + (nPairHeight * 0.5f)))
	{// ��ӂƉ��ӂ�����̕��̓����̎��A
		if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))		// ���݂̉E�ӂ�����̍��ӂ����E�Ɉʒu���Ă���
			&&																								// &
			((nMyOldX + nMyPartsX) + (nMyWidth * 0.5f) <= (nPairOldX + nPairPartsX) - (nPairWidth * 0.5f)))	// �ߋ��̉E�ӂ�����̍��ӂ������Ɉʒu���Ă���
		{// ������߂荞��ł��鎞�A

			if (!pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
			{// ������̂��蔲�����U�̎��A
				if (pCollision != NULL)
				{// �Փˏ��NULL�łȂ����A
					pCollision->aHitDirection[HIT_DIRECTION_LEFT] = true;	// ����������̏Փ˃t���O��^�ɂ���
					pCollision->bHitDirection = true;						// �Փ˃t���O��^�ɂ���
				}
				myCollInfo.pPos->x =									// �ʒu�𑊎�̓����蔻��̍��ɂ���
					(nPairX + nPairPartsX - (nPairWidth * 0.5f) - nMyPartsX - (nMyWidth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
			}
		}
		else if (((nMyX + nMyPartsX) - (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f))	// ���ӂ�����̉E�ӂ������Ɉʒu���Ă���
			&&																								// &
			((nMyOldX + nMyPartsX) - (nMyWidth * 0.5f) >= (nPairOldX + nPairPartsX) + (nPairWidth * 0.5f)))	// �ߋ��̍��ӂ�����̉E�ӂ����E�Ɉʒu���Ă���
		{// �E����߂荞��ł��鎞�A

			if (!pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
			{// �E����̂��蔲�����U�̎��A
				if (pCollision != NULL)
				{// �Փˏ��NULL�łȂ����A
					pCollision->aHitDirection[HIT_DIRECTION_RIGHT] = true;	// �E��������̏Փ˃t���O��^�ɂ���
					pCollision->bHitDirection = true;						// �Փ˃t���O��^�ɂ���
				}
				myCollInfo.pPos->x =									// �ʒu�𑊎�̓����蔻��̉E�ɂ���
					(nPairX + nPairPartsX + (nPairWidth * 0.5f) - nMyPartsX + (nMyWidth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
			}
		}

		if (pCollision != NULL)
		{// �Փˏ��NULL�łȂ����A
			if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
				&&
				((nMyX + nMyPartsX) + (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
			{// ���݂̉E�ӂ�����̓����蔻��̓����̎��A
				pCollision->aInside[HIT_DIRECTION_LEFT] = true;	// ����������̓����t���O��^�ɂ���
				pCollision->bHit = true;						// ������t���O��^�ɂ���
				pCollision->aHit[HIT_DIRECTION_LEFT] = true;	// ����������̓�����t���O��^�ɂ���

				if (pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
				{// ������̂��蔲�����^�̎��A
					pCollision->bGhost = true;						// ���蔲���t���O��^�ɂ���
					pCollision->aGhost[HIT_DIRECTION_LEFT] = true;	// ����������̂��蔲���t���O��^�ɂ���
				}
			}

			if (((nMyX + nMyPartsX) - (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
				&&
				((nMyX + nMyPartsX) - (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
			{// ���݂̍��ӂ�����̓����蔻��̓����̎��A
				pCollision->aInside[HIT_DIRECTION_RIGHT] = true;	// �E��������̓����t���O��^�ɂ���
				pCollision->bHit = true;							// ������t���O��^�ɂ���
				pCollision->aHit[HIT_DIRECTION_RIGHT] = true;		// �E��������̓�����t���O��^�ɂ���

				if (pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
				{// �E����̂��蔲�����^�̎��A
					pCollision->bGhost = true;						// ���蔲���t���O��^�ɂ���
					pCollision->aGhost[HIT_DIRECTION_RIGHT] = true;	// �E��������̂��蔲���t���O��^�ɂ���
				}
			}
		}
	}

	if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) > (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
		&&
		((nMyX + nMyPartsX) - (nMyWidth * 0.5f) < (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
	{// ���ӂƉE�ӂ�����̕��̓����̎��A
		if (((nMyY + nMyPartsY) + (nMyHeight * 0.5f) > (nPairY + nPairPartsY) - (nPairHeight * 0.5f))			// ����������̏�ӂ������Ɉʒu���Ă���
			&&																									// &
			((nMyOldY + nMyPartsY) + (nMyHeight * 0.5f) <= (nPairOldY + nPairPartsY) - (nPairHeight * 0.5f)))	// �ߋ��̑���������̏�ӂ�����Ɉʒu���Ă���
		{// �ォ��߂荞��ł��鎞�A

			if (!pairHitTestParts.aGhost[HIT_DIRECTION_TOP])
			{// �ォ��̂��蔲�����U�̎��A
				if (pCollision != NULL)
				{// �Փˏ��NULL�łȂ����A
					pCollision->aHitDirection[HIT_DIRECTION_TOP] = true;	// ���������̏Փ˃t���O��^�ɂ���
					pCollision->bHitDirection = true;						// �Փ˃t���O��^�ɂ���
				}
				myCollInfo.pPos->y =	// �ʒu�𑊎�̓����蔻��̏�ɂ���
					(nPairY + nPairPartsY - (nPairHeight * 0.5f) - nMyPartsY - (nMyHeight * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
			}
		}
		else if (((nMyY + nMyPartsY) - (nMyHeight * 0.5f) < (nPairY + nPairPartsY) + (nPairHeight * 0.5f))		// ��������̉��ӂ�����Ɉʒu���Ă���
			&&																									// &
			((nMyOldY + nMyPartsY) - (nMyHeight * 0.5f) >= (nPairOldY + nPairPartsY) + (nPairHeight * 0.5f)))	// �ߋ��̓�������̉��ӂ������Ɉʒu���Ă���
		{// ������߂荞��ł��鎞�A

			if (!pairHitTestParts.aGhost[HIT_DIRECTION_BOTTOM])
			{// ������̂��蔲�����U�̎��A
				if (pCollision != NULL)
				{// �Փˏ��NULL�łȂ����A
					pCollision->aHitDirection[HIT_DIRECTION_BOTTOM] = true;	// ����������̏Փ˃t���O��^�ɂ���
					pCollision->bHitDirection = true;						// �Փ˃t���O��^�ɂ���
				}
				myCollInfo.pPos->y = 									// �ʒu�𑊎�̓����蔻��̉��ɂ���
					(nPairY + nPairPartsY + (nPairHeight * 0.5f) - nMyPartsY + (nMyHeight * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
			}
		}

		if (pCollision != NULL)
		{// �Փˏ��NULL�łȂ����A
			if (((nMyY + nMyPartsY) + (nMyHeight * 0.5f) >= (nPairY + nPairPartsY) - (nPairHeight * 0.5f))
				&&
				((nMyY + nMyPartsY) + (nMyHeight * 0.5f) <= (nPairY + nPairPartsY) + (nPairHeight * 0.5f)))
			{// ���݂̉��ӂ�����̓����蔻��̓����̎��A
				pCollision->aInside[HIT_DIRECTION_TOP] = true;	// ���������̓����t���O��^�ɂ���

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_TOP])
				{// �ォ��̂��蔲�����U�̎��A
					pCollision->bHit = true;					// ������t���O��^�ɂ���
					pCollision->aHit[HIT_DIRECTION_TOP] = true;	// ���������̓�����t���O��^�ɂ���
				}
				else
				{// �ォ��̂��蔲�����^�̎��A
					pCollision->bGhost = true;						// ���蔲���t���O��^�ɂ���
					pCollision->aGhost[HIT_DIRECTION_TOP] = true;	// ���������̂��蔲���t���O��^�ɂ���
				}
			}

			if (((nMyY + nMyPartsY) - (nMyHeight * 0.5f) >= (nPairY + nPairPartsY) - (nPairHeight * 0.5f))
				&&
				((nMyY + nMyPartsY) - (nMyHeight * 0.5f) <= (nPairY + nPairPartsY) + (nPairHeight * 0.5f)))
			{// ���݂̏�ӂ�����̓����蔻��̓����̎��A
				pCollision->aInside[HIT_DIRECTION_BOTTOM] = true;	// ����������̓����t���O��^�ɂ���

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_BOTTOM])
				{// ������̂��蔲�����U�̎��A
					pCollision->bHit = true;						// ������t���O��^�ɂ���
					pCollision->aHit[HIT_DIRECTION_BOTTOM] = true;	// ����������̓�����t���O��^�ɂ���
				}
				else
				{// ������̂��蔲�����^�̎��A
					pCollision->bGhost = true;							// ���蔲���t���O��^�ɂ���
					pCollision->aGhost[HIT_DIRECTION_BOTTOM] = true;	// ����������̂��蔲���t���O��^�ɂ���
				}
			}
		}
	}
}

//========================================
// SquareSquareCollisionLookDown�֐� - �l�p����l�p�̏Փ˃`�F�b�N(�����낵���[�h) -
// Author:RIKU NISHIMURA
//========================================
void SquareSquareCollisionLookDown(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo, HitTestParts pairHitTestParts)
{
	HitTest myHitTest	// �����̓����蔻����
			= myCollInfo.hitTest;
	HitTest pairHitTest	// ����̓����蔻����
			= pairCollInfo.hitTest;

	// �����蔻��̌v�Z�ɕK�v�ȃp�����[�^�[�𐮐��^�ɕϊ�(���Z�덷�΍�)
	int nMyX		= myCollInfo.pPos->x					* HIT_TEST_DECIMAL_PLACE;
	int nMyZ		= myCollInfo.pPos->z					* HIT_TEST_DECIMAL_PLACE;
	int nMyPartsX	= myHitTestParts.fixedRelativePos.x		* HIT_TEST_DECIMAL_PLACE;
	int nMyPartsZ	= myHitTestParts.fixedRelativePos.z		* HIT_TEST_DECIMAL_PLACE;
	int nMyOldX		= myCollInfo.posOld.x					* HIT_TEST_DECIMAL_PLACE;
	int nMyOldZ		= myCollInfo.posOld.z					* HIT_TEST_DECIMAL_PLACE;
	int nMyWidth	= myHitTestParts.fWidth					* HIT_TEST_DECIMAL_PLACE;
	int nMyDepth	= myHitTestParts.fDepth					* HIT_TEST_DECIMAL_PLACE;
	int nPairX		= pairCollInfo.pPos->x					* HIT_TEST_DECIMAL_PLACE;
	int nPairZ		= pairCollInfo.pPos->z					* HIT_TEST_DECIMAL_PLACE;
	int nPairPartsX = pairHitTestParts.fixedRelativePos.x	* HIT_TEST_DECIMAL_PLACE;
	int nPairPartsZ = pairHitTestParts.fixedRelativePos.z	* HIT_TEST_DECIMAL_PLACE;
	int nPairOldX	= pairCollInfo.posOld.x					* HIT_TEST_DECIMAL_PLACE;
	int nPairOldZ	= pairCollInfo.posOld.z					* HIT_TEST_DECIMAL_PLACE;
	int nPairWidth	= pairHitTestParts.fWidth				* HIT_TEST_DECIMAL_PLACE;
	int nPairDepth	= pairHitTestParts.fDepth				* HIT_TEST_DECIMAL_PLACE;

	switch (vector)
	{
		//========== *** X���� ***
	case VECTOR_X:
	{
		if (((nMyZ + nMyPartsZ) + (nMyDepth * 0.5f) > (nPairZ + nPairPartsZ) - (nPairDepth * 0.5f))
			&&
			((nMyZ + nMyPartsZ) - (nMyDepth * 0.5f) < (nPairZ + nPairPartsZ) + (nPairDepth * 0.5f)))
		{// ���ӂƎ�O�ӂ�����̕��̓����̎��A
			if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))		// ���݂̉E�ӂ�����̍��ӂ����E�Ɉʒu���Ă���
				&&																								// &
				((nMyOldX + nMyPartsX) + (nMyWidth * 0.5f) <= (nPairOldX + nPairPartsX) - (nPairWidth * 0.5f)))	// �ߋ��̉E�ӂ�����̍��ӂ������Ɉʒu���Ă���
			{// ������߂荞��ł��鎞�A

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
				{// ������̂��蔲�����U�̎��A
					if (pCollision != NULL)
					{// �Փˏ��NULL�łȂ����A
						pCollision->bHitDirection =	// �Փ˃t���O��^�ɂ���
						pCollision->aHitDirection[HIT_DIRECTION_LEFT] = true;
					}
					myCollInfo.pPos->x =	// �ʒu�𑊎�̓����蔻��̍��ɂ���
						(nPairX + nPairPartsX - (nPairWidth * 0.5f) - nMyPartsX - (nMyWidth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
				}
			}
			else if (((nMyX + nMyPartsX) - (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f))	// ���ӂ�����̉E�ӂ������Ɉʒu���Ă���
				&&																								// &
				((nMyOldX + nMyPartsX) - (nMyWidth * 0.5f) >= (nPairOldX + nPairPartsX) + (nPairWidth * 0.5f)))	// �ߋ��̍��ӂ�����̉E�ӂ����E�Ɉʒu���Ă���
			{// �E����߂荞��ł��鎞�A

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
				{// ������̂��蔲�����U�̎��A
					if (pCollision != NULL)
					{// �Փˏ��NULL�łȂ����A
						pCollision->bHitDirection =	// �Փ˃t���O��^�ɂ���
						pCollision->aHitDirection[HIT_DIRECTION_RIGHT] = true;
					}
					myCollInfo.pPos->x =	// �ʒu�𑊎�̓����蔻��̉E�ɂ���
						(nPairX + nPairPartsX + (nPairWidth * 0.5f) - nMyPartsX + (nMyWidth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
				}
			}

			if (pCollision != NULL)
			{// �Փˏ��NULL�łȂ����A
				if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
					&&
					((nMyX + nMyPartsX) + (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
				{// ���݂̉E�ӂ�����̓����蔻��̓����̎��A
					pCollision->aInside[HIT_DIRECTION_LEFT] = true;	// ����������̓����t���O��^�ɂ���
					
					if (pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
					{// ������̂��蔲�����^�̎��A
						pCollision->bGhost = true;						// ���蔲���t���O��^�ɂ���
						pCollision->aGhost[HIT_DIRECTION_LEFT] = true;	// ����������̂��蔲���t���O��^�ɂ���
					}
					else
					{// �E����̂��蔲�����U�̎��A
						pCollision->bHit = true;						// ������t���O��^�ɂ���
						pCollision->aHit[HIT_DIRECTION_LEFT] = true;	// ����������̓�����t���O��^�ɂ���
					}
				}

				if (((nMyX + nMyPartsX) - (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
					&&
					((nMyX + nMyPartsX) - (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
				{// ���݂̍��ӂ�����̓����蔻��̓����̎��A
					pCollision->aInside[HIT_DIRECTION_RIGHT] = true;	// �E��������̓����t���O��^�ɂ���
					
					if (pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
					{// �E����̂��蔲�����^�̎��A
						pCollision->bGhost = true;						// ���蔲���t���O��^�ɂ���
						pCollision->aGhost[HIT_DIRECTION_RIGHT] = true;	// �E��������̂��蔲���t���O��^�ɂ���
					}
					else
					{// �E����̂��蔲�����U�̎��A
						pCollision->bHit = true;							// ������t���O��^�ɂ���
						pCollision->aHit[HIT_DIRECTION_RIGHT] = true;		// �E��������̓�����t���O��^�ɂ���
					}
				}
			}
		}
	}
		break;
		//========== *** Z���� ***
	case VECTOR_Z:
	{
		if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) > (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
			&&
			((nMyX + nMyPartsX) - (nMyWidth * 0.5f) < (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
		{// ���ӂƉE�ӂ�����̕��̓����̎��A
			if (((nMyZ + nMyPartsZ) + (nMyDepth * 0.5f) > (nPairZ + nPairPartsZ) - (nPairDepth * 0.5f))			// ����������̉��ӂ�����O�Ɉʒu���Ă���
				&&																								// &
				((nMyOldZ + nMyPartsZ) + (nMyDepth * 0.5f) <= (nPairOldZ + nPairPartsZ) - (nPairDepth * 0.5f)))	// �ߋ��̑���������̉��ӂ������Ɉʒu���Ă���
			{// ������߂荞��ł��鎞�A

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_BACK])
				{// ������̂��蔲�����U�̎��A
					if (pCollision != NULL)
					{// �Փˏ��NULL�łȂ����A
						pCollision->bHitDirection =	// �Փ˃t���O��^�ɂ���
						pCollision->aHitDirection[HIT_DIRECTION_BACK] = true;
					}
					myCollInfo.pPos->z = 	// �ʒu�𑊎�̓����蔻��̉��ɂ���
						(nPairZ + nPairPartsZ - (nPairDepth * 0.5f) - nMyPartsZ - (nMyDepth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
				}
			}
			else if (((nMyZ + nMyPartsZ) - (nMyDepth * 0.5f) < (nPairZ + nPairPartsZ) + (nPairDepth * 0.5f))	// ��������̎�O�ӂ������Ɉʒu���Ă���
				&&																								// &
				((nMyOldZ + nMyPartsZ) - (nMyDepth * 0.5f) >= (nPairOldZ + nPairPartsZ) + (nPairDepth * 0.5f)))	// �ߋ��̓�������̎�O�ӂ�����O�Ɉʒu���Ă���
			{// ��O����߂荞��ł��鎞�A

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_FRONT])
				{// ��O����̂��蔲�����U�̎��A
					if (pCollision != NULL)
					{// �Փˏ��NULL�łȂ����A
						pCollision->bHitDirection =	// �Փ˃t���O��^�ɂ���
							pCollision->aHitDirection[HIT_DIRECTION_FRONT] = true;
					}
					myCollInfo.pPos->z = // �ʒu�𑊎�̓����蔻��̎�O�ɂ���
						(nPairZ + nPairPartsZ + (nPairDepth * 0.5f) - nMyPartsZ + (nMyDepth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
				}
			}

			if (pCollision != NULL)
			{// �Փˏ��NULL�łȂ����A
				if (((nMyZ + nMyPartsZ) + (nMyDepth * 0.5f) >= (nPairZ + nPairPartsZ) - (nPairDepth * 0.5f))
					&&
					((nMyZ + nMyPartsZ) + (nMyDepth * 0.5f) <= (nPairZ + nPairPartsZ) + (nPairDepth * 0.5f)))
				{// ���݂̎�O�ӂ�����̓����蔻��̓����̎��A
					pCollision->aInside[HIT_DIRECTION_BACK] = true;	// ����������̓����t���O��^�ɂ���
					
					if (pairHitTestParts.aGhost[HIT_DIRECTION_BACK])
					{// ������̂��蔲�����^�̎��A
						pCollision->bGhost = true;						// ���蔲���t���O��^�ɂ���
						pCollision->aGhost[HIT_DIRECTION_BACK] = true;	// ����������̂��蔲���t���O��^�ɂ���
					}
					else 
					{// ������̂��蔲�����U�̎��A
						pCollision->bHit = true;						// ������t���O��^�ɂ���
						pCollision->aHit[HIT_DIRECTION_BACK] = true;	// ����������̓�����t���O��^�ɂ���
					}
				}

				if (((nMyZ + nMyPartsZ) - (nMyDepth * 0.5f) >= (nPairZ + nPairPartsZ) - (nPairDepth * 0.5f))
					&&
					((nMyZ + nMyPartsZ) - (nMyDepth * 0.5f) <= (nPairZ + nPairPartsZ) + (nPairDepth * 0.5f)))
				{// ���݂̉��ӂ�����̓����蔻��̓����̎��A
					pCollision->aInside[HIT_DIRECTION_FRONT] = true;	// ��O��������̓����t���O��^�ɂ���
					
					if (pairHitTestParts.aGhost[HIT_DIRECTION_FRONT])
					{// ��O����̂��蔲�����^�̎��A
						pCollision->bGhost = true;						// ���蔲���t���O��^�ɂ���
						pCollision->aGhost[HIT_DIRECTION_FRONT] = true;	// ��O��������̂��蔲���t���O��^�ɂ���
					}
					else
					{// ��O����̂��蔲�����U�̎��A
						pCollision->bHit = true;						// ������t���O��^�ɂ���
						pCollision->aHit[HIT_DIRECTION_FRONT] = true;	// ��O��������̓�����t���O��^�ɂ���
					}
				}
			}
		}
	}
		break;
	}
}

//========================================
// PointSquareCollisionLookDown�֐� - �_����l�p�̏Փ˃`�F�b�N(�����낵���[�h) -
// Author:RIKU NISHIMURA
//========================================
void PointSquareCollisionLookDown(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo, HitTestParts pairHitTestParts,
	bool bRflc, bool bSlip)
{
	float fMyScale	= (myHitTestParts.fWidth * myHitTestParts.fDepth) * 0.5f;	// ���g�̑傫��
	float fWidth	= pairHitTestParts.fWidth + (fMyScale * 0.5f);				// ��
	float fDepth	= pairHitTestParts.fDepth + (fMyScale * 0.5f);				// ���s��

	D3DXVECTOR3 pos0, pos1, pos2, pos3;	// ���v���Ɍ��ĉE�������4���_�̈ʒu(����)
	pos0.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (sinf(pairCollInfo.pRot->y) * (fDepth * -0.5f));
	pos0.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (cosf(pairCollInfo.pRot->y) * (fDepth * -0.5f));
	pos1.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (sinf(pairCollInfo.pRot->y) * (fDepth * -0.5f));
	pos1.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (cosf(pairCollInfo.pRot->y) * (fDepth * -0.5f));
	pos2.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (sinf(pairCollInfo.pRot->y) * (fDepth * 0.5f));
	pos2.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (cosf(pairCollInfo.pRot->y) * (fDepth * 0.5f));
	pos3.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (sinf(pairCollInfo.pRot->y) * (fDepth * 0.5f));
	pos3.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (cosf(pairCollInfo.pRot->y) * (fDepth * 0.5f));
	D3DXVECTOR3 posOld0, posOld1, posOld2, posOld3;	// ���v���Ɍ��ĉE�������4���_�̉ߋ��̈ʒu(����)
	posOld0.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (sinf(pairCollInfo.rotOld.y) * (fDepth * -0.5f));
	posOld0.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (cosf(pairCollInfo.rotOld.y) * (fDepth * -0.5f));
	posOld1.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (sinf(pairCollInfo.rotOld.y) * (fDepth * -0.5f));
	posOld1.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (cosf(pairCollInfo.rotOld.y) * (fDepth * -0.5f));
	posOld2.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (sinf(pairCollInfo.rotOld.y) * (fDepth * 0.5f));
	posOld2.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (cosf(pairCollInfo.rotOld.y) * (fDepth * 0.5f));
	posOld3.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (sinf(pairCollInfo.rotOld.y) * (fDepth * 0.5f));
	posOld3.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (cosf(pairCollInfo.rotOld.y) * (fDepth * 0.5f));

	bool aInside[HIT_DIRECTION_MAX];	// �����t���O

	// ����
	{
		D3DXVECTOR3 vecToPos	= *myCollInfo.pPos - pos2;
		D3DXVECTOR3 vecToPosOld = myCollInfo.posOld - posOld2;
		D3DXVECTOR3 point =	// ��_�̍��W
			FindIntersectionPointLookDown(myCollInfo.posOld, *myCollInfo.pPos, pos2, pos3);
		float	fDistance3toP = FindDistanceLookDown(pos3, point);	// ���_3�����_�܂ł̋���
		float	fDistance2toP = FindDistanceLookDown(pos2, point);	// ���_2�����_�܂ł̋���
		float	fDistance3to2 = FindDistanceLookDown(pos3, pos2);	// ���_3���璸�_2�܂ł̋���

		// ���ӂ���̓����t���O���擾
		aInside[HIT_DIRECTION_BACK] = (((pos3 - pos2).z * vecToPos.x) - ((pos3 - pos2).x * vecToPos.z) >= 0);

		if (((aInside[HIT_DIRECTION_BACK]) && (((posOld3 - posOld2).z * vecToPosOld.x) - ((posOld3 - posOld2).x * vecToPosOld.z) <= 0))
			&&
			(fDistance3toP <= fDistance3to2)
			&&
			(fDistance2toP <= fDistance3to2))
		{// ���ӂ��瓖�����Ă��鎞�A
			if (pCollision != NULL) {
				pCollision->bHitDirection =	// �Փ˃t���O��^�ɂ���
				pCollision->aHitDirection[HIT_DIRECTION_BACK] = true;
				pCollision->fLength	// �߂荞�݋�����ݒ�
					= FindDistanceLookDown(point, *myCollInfo.pPos);

				if (pairHitTestParts.aGhost[HIT_DIRECTION_BACK])
				{// ����̂��蔲���t���O���^�̎��A
					pCollision->bGhost =	// ���蔲���t���O��^�ɂ���
					pCollision->aGhost[HIT_DIRECTION_BACK] = true;
					return;
				}
				else
				{// ����̂��蔲���t���O���U�̎��A
					pCollision->bHit =	// ������t���O��^�ɂ���
					pCollision->aHit[HIT_DIRECTION_BACK] = true;
				}
			}

			if (bSlip)
			{// ����t���O���^�̎��A
				float fAngle = FindAngleLookDown(pos2, pos3) - (D3DX_PI * 0.5f);	// ����

				// �ʒu���X�V
				D3DXVECTOR3 pos = FindIntersectionPointLookDown(
					*myCollInfo.pPos,
					*myCollInfo.pPos + D3DXVECTOR3(sinf(fAngle),0.0f,cosf(fAngle)),
					pos3,
					pos2);
				myCollInfo.pPos->x = pos.x + sinf(fAngle) * 0.01f;
				myCollInfo.pPos->z = pos.z + cosf(fAngle) * 0.01f;
			}
			else
			{// ����t���O���U�̎��A
				// �ʒu���X�V
				myCollInfo.pPos->x = point.x;
				myCollInfo.pPos->z = point.z;
			}

			if ((bRflc) && (myCollInfo.pRot != NULL))
			{// ���˃t���O���^ & �����̃|�C���^��NULL�łȂ����A
				// �����𔽎˂�����
				myCollInfo.pRot->y = FindReflectionAngleLookDown(point, myCollInfo.posOld, pos3, pos2);
				
				// ���˕����ɔ��ʉ��Z(���܂荞�ݖh�~)
				myCollInfo.pPos->x += sinf(myCollInfo.pRot->y);
				myCollInfo.pPos->z += cosf(myCollInfo.pRot->y);
			}

			return;
		}
	}

	// ��O��
	{
		D3DXVECTOR3 vecToPos	= *myCollInfo.pPos - pos0;
		D3DXVECTOR3 vecToPosOld = myCollInfo.posOld - posOld0;
		D3DXVECTOR3 point =	// ��_�̍��W
			FindIntersectionPointLookDown(myCollInfo.posOld, *myCollInfo.pPos, pos0, pos1);
		float	fDistance1toP = FindDistanceLookDown(pos1, point);	// ���_1�����_�܂ł̋���
		float	fDistance0toP = FindDistanceLookDown(pos0, point);	// ���_0�����_�܂ł̋���
		float	fDistance1to0 = FindDistanceLookDown(pos1, pos0);	// ���_1���璸�_0�܂ł̋���

		// ��O�ӂ���̓����t���O���擾
		aInside[HIT_DIRECTION_FRONT] = (((pos1 - pos0).z * vecToPos.x) - ((pos1 - pos0).x * vecToPos.z) >= 0);

		if (((aInside[HIT_DIRECTION_FRONT]) && (((posOld1 - posOld0).z * vecToPosOld.x) - ((posOld1 - posOld0).x * vecToPosOld.z) <= 0))
			&&
			(fDistance1toP <= fDistance1to0)
			&&
			(fDistance0toP <= fDistance1to0))
		{// ��O�ӂ��瓖�����Ă��鎞�A
			if (pCollision != NULL) {
				pCollision->bHitDirection =	// �Փ˃t���O��^�ɂ���
				pCollision->aHitDirection[HIT_DIRECTION_FRONT] = true;
				pCollision->fLength	// �߂荞�݋�����ݒ�
					= FindDistanceLookDown(point, *myCollInfo.pPos);

				if (pairHitTestParts.aGhost[HIT_DIRECTION_FRONT])
				{// ����̂��蔲���t���O���^�̎��A
					pCollision->bGhost =	// ���蔲���t���O��^�ɂ���
					pCollision->aGhost[HIT_DIRECTION_FRONT] = true;
					return;
				}
				else
				{// ����̂��蔲���t���O���U�̎��A
					pCollision->bHit =	// ������t���O��^�ɂ���
					pCollision->aHit[HIT_DIRECTION_FRONT] = true;
				}
			}

			if(bSlip)
			{// ����t���O���^�̎��A
				float fAngle = FindAngleLookDown(pos0, pos1) - (D3DX_PI * 0.5f);	// ����

				// �ʒu���X�V
				D3DXVECTOR3 pos = FindIntersectionPointLookDown(
					*myCollInfo.pPos,
					*myCollInfo.pPos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)),
					pos1,
					pos0);
				myCollInfo.pPos->x = pos.x + sinf(fAngle) * 0.01f;
				myCollInfo.pPos->z = pos.z + cosf(fAngle) * 0.01f;
			}
			else
			{// ����t���O���U�̎��A
				// �ʒu���X�V
				myCollInfo.pPos->x = point.x;
				myCollInfo.pPos->z = point.z;
			}

			if ((bRflc) && (myCollInfo.pRot != NULL))
			{// ���˃t���O���^ & �����̃|�C���^��NULL�łȂ����A
				// �����𔽎˂�����
				myCollInfo.pRot->y = FindReflectionAngleLookDown(point, myCollInfo.posOld, pos1, pos0);

				// ���˕����ɔ��ʉ��Z(���܂荞�ݖh�~)
				myCollInfo.pPos->x += sinf(myCollInfo.pRot->y);
				myCollInfo.pPos->z += cosf(myCollInfo.pRot->y);
			}

			return;
		}
	}

	// ����
	{
		D3DXVECTOR3 vecToPos	= *myCollInfo.pPos - pos1;
		D3DXVECTOR3 vecToPosOld = myCollInfo.posOld - posOld1;
		D3DXVECTOR3 point =	// ��_�̍��W
					FindIntersectionPointLookDown(myCollInfo.posOld, *myCollInfo.pPos, pos1, pos2);
		float	fDistance2toP = FindDistanceLookDown(pos2, point);	// ���_2�����_�܂ł̋���
		float	fDistance1toP = FindDistanceLookDown(pos1, point);	// ���_1�����_�܂ł̋���
		float	fDistance2to1 = FindDistanceLookDown(pos2, pos1);	// ���_2���璸�_1�܂ł̋���

		// ���ӂ���̓����t���O���擾
		aInside[HIT_DIRECTION_LEFT] = (((pos2 - pos1).z * vecToPos.x) - ((pos2 - pos1).x * vecToPos.z) >= 0);

		if (((aInside[HIT_DIRECTION_LEFT]) && (((posOld2 - posOld1).z * vecToPosOld.x) - ((posOld2 - posOld1).x * vecToPosOld.z) <= 0))
			&&
			(FindDistanceLookDown(pos2, point) <= FindDistanceLookDown(pos2, pos1))
			&&
			(FindDistanceLookDown(pos1, point) <= FindDistanceLookDown(pos2, pos1)))
		{// ���ӂ��瓖�����Ă��鎞�A
			if (pCollision != NULL) {
				pCollision->bHitDirection =	// �Փ˃t���O��^�ɂ���
				pCollision->aHitDirection[HIT_DIRECTION_LEFT] = true;
				pCollision->fLength	// �߂荞�݋�����ݒ�
					= FindDistanceLookDown(point, *myCollInfo.pPos);

				if (pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
				{// ����̂��蔲���t���O���^�̎��A
					pCollision->bGhost =	// ���蔲���t���O��^�ɂ���
					pCollision->aGhost[HIT_DIRECTION_LEFT] = true;
					return;
				}
				else
				{// ����̂��蔲���t���O���U�̎��A
					pCollision->bHit =	// ������t���O��^�ɂ���
					pCollision->aHit[HIT_DIRECTION_LEFT] = true;
				}
			}

			if (bSlip)
			{// ����t���O���^�̎��A
				float fAngle = FindAngleLookDown(pos1, pos2) - (D3DX_PI * 0.5f);	// ����

				// �ʒu���X�V
				D3DXVECTOR3 pos = FindIntersectionPointLookDown(
					*myCollInfo.pPos,
					*myCollInfo.pPos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)),
					pos2,
					pos1);
				myCollInfo.pPos->x = pos.x + sinf(fAngle) * 0.01f;
				myCollInfo.pPos->z = pos.z + cosf(fAngle) * 0.01f;
			}
			else
			{// ����t���O���U�̎��A
				// �ʒu���X�V
				myCollInfo.pPos->x = point.x;
				myCollInfo.pPos->z = point.z;
			}

			if ((bRflc) && (myCollInfo.pRot != NULL))
			{// ���˃t���O���^ & �����̃|�C���^��NULL�łȂ����A
				// �����𔽎˂�����
				myCollInfo.pRot->y = FindReflectionAngleLookDown(point, myCollInfo.posOld, pos2, pos1);

				// ���˕����ɔ��ʉ��Z(���܂荞�ݖh�~)
				myCollInfo.pPos->x += sinf(myCollInfo.pRot->y);
				myCollInfo.pPos->z += cosf(myCollInfo.pRot->y);
			}

			return;
		}
	}
	
	// �E��
	{
		D3DXVECTOR3 vecToPos	= *myCollInfo.pPos - pos3;
		D3DXVECTOR3 vecToPosOld = myCollInfo.posOld - posOld3;
		D3DXVECTOR3 point =	// ��_�̍��W
			FindIntersectionPointLookDown(myCollInfo.posOld, *myCollInfo.pPos, pos3, pos0);
		float	fDistance0toP = FindDistanceLookDown(pos0, point);	// ���_0�����_�܂ł̋���
		float	fDistance3toP = FindDistanceLookDown(pos3, point);	// ���_3�����_�܂ł̋���
		float	fDistance0to3 = FindDistanceLookDown(pos0, pos3);	// ���_0���璸�_3�܂ł̋���

		// �E�ӂ���̓����t���O���擾
		aInside[HIT_DIRECTION_RIGHT] = (((pos0 - pos3).z * vecToPos.x) - ((pos0 - pos3).x * vecToPos.z) >= 0);

		if (((aInside[HIT_DIRECTION_RIGHT]) && (((posOld0 - posOld3).z * vecToPosOld.x) - ((posOld0 - posOld3).x * vecToPosOld.z) <= 0))
			&&
			(fDistance0toP <= fDistance0to3)
			&&
			(fDistance3toP <= fDistance0to3))
		{// �E�ӂ��瓖�����Ă��鎞�A
			if (pCollision != NULL) {
				pCollision->bHitDirection =	// �Փ˃t���O��^�ɂ���
				pCollision->aHitDirection[HIT_DIRECTION_RIGHT] = true;
				pCollision->fLength	// �߂荞�݋�����ݒ�
					= FindDistanceLookDown(point, *myCollInfo.pPos);
				
				if (pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
				{// ����̂��蔲���t���O���^�̎��A
					pCollision->bGhost =	// ���蔲���t���O��^�ɂ���
					pCollision->aGhost[HIT_DIRECTION_RIGHT] = true;
					return;
				}
				else
				{// ����̂��蔲���t���O���U�̎��A
					pCollision->bHit =	// ������t���O��^�ɂ���
					pCollision->aHit[HIT_DIRECTION_RIGHT] = true;
				}
			}

			if (bSlip)
			{// ����t���O���^�̎��A
				float fAngle = FindAngleLookDown(pos3, pos0) - (D3DX_PI * 0.5f);	// ����

				// �ʒu���X�V
				D3DXVECTOR3 pos = FindIntersectionPointLookDown(
					*myCollInfo.pPos,
					*myCollInfo.pPos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)),
					pos0,
					pos3);
				myCollInfo.pPos->x = pos.x + sinf(fAngle) * 0.01f;
				myCollInfo.pPos->z = pos.z + cosf(fAngle) * 0.01f;
			}
			else
			{// ����t���O���U�̎��A
				// �ʒu���X�V
				myCollInfo.pPos->x = point.x;
				myCollInfo.pPos->z = point.z;
			}

			if ((bRflc) && (myCollInfo.pRot != NULL))
			{// ���˃t���O���^ & �����̃|�C���^��NULL�łȂ����A
				// �����𔽎˂�����
				myCollInfo.pRot->y = FindReflectionAngleLookDown(point, myCollInfo.posOld, pos0, pos3);

				// ���˕����ɔ��ʉ��Z(���܂荞�ݖh�~)
				myCollInfo.pPos->x += sinf(myCollInfo.pRot->y);
				myCollInfo.pPos->z += cosf(myCollInfo.pRot->y);
			}

			return;
		}
	}

	if (pCollision != NULL) {
		if (aInside[HIT_DIRECTION_BACK] &&
			aInside[HIT_DIRECTION_FRONT] &&
			aInside[HIT_DIRECTION_LEFT] &&
			aInside[HIT_DIRECTION_RIGHT])
		{// �S�Ă̕�����������̎��A
			pCollision->bInside = true;	// ������t���O��^�ɂ���
		}
	}
}

//========================================
// FindReflectionAngleLookDown�֐� - ���ˊp�����߂�(�����낵) -
// Author:RIKU NISHIMURA
//========================================
float FindReflectionAngleLookDown(D3DXVECTOR3 point,D3DXVECTOR3 posOld,D3DXVECTOR3 pos0,D3DXVECTOR3 pos1) 
{
	float fAngle = FindAngleLookDown(pos0, pos1) + (D3DX_PI * 0.5f);	// �ǂ̍��[����E�[�܂ł̊p�x

	// posOld����ǂɑ΂��Đ����Ȍ�_�����_�܂ł̋���
	float fDistance = FindDistanceLookDown(
		FindIntersectionPointLookDown(	// posOld����ǂɑ΂��Đ����Ȍ�_
			posOld,
			posOld + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)),
			pos0,
			pos1),
		point	// ��_
	);

	// �ڕW�ʒu(��_����̊p�x�����߂��)
	D3DXVECTOR3 targetPos = posOld;

	if (AngleDifference(FindAngleLookDown(pos0, pos1), FindAngleLookDown(point, posOld)) <= D3DX_PI * 0.5f)
	{// �ǂɐ����̐��ɑ΂��č�������Փ˂������A
		targetPos.x += sinf(FindAngleLookDown(pos1, pos0)) * (fDistance * 2.0f);
		targetPos.z += cosf(FindAngleLookDown(pos1, pos0)) * (fDistance * 2.0f);
	}
	else
	{// �ǂɐ����̐��ɑ΂��ĉE������Փ˂������A
		targetPos.x += sinf(FindAngleLookDown(pos0, pos1)) * (fDistance * 2.0f);
		targetPos.z += cosf(FindAngleLookDown(pos0, pos1)) * (fDistance * 2.0f);
	}

	// ���ˊp��Ԃ�
	return FindAngleLookDown(point, targetPos);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadHitTestSet�֐� - �����蔻��ݒ���̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadHitTestSet(FILE *pFile, HitTestSet *pHitTestSet)
{
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];

	// �����蔻��̐���������
	pHitTestSet->nHitTestNum = 0;
	
	// �����蔻��ݒ���̓ǂݍ��݂��J�n
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // ����

		if		(!strcmp(aDataSearch, "HITTESTSET_END"))	{ break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "HITTEST"))			{
			pHitTestSet->aHitTest[pHitTestSet->nHitTestNum].nHitTestPartsNum = 0;	// �����蔻��̕��i����������

			// �����蔻����̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if		(!strcmp(aDataSearch, "HITTEST_END"))	{
					pHitTestSet->nHitTestNum++;	// �����蔻�萔�����Z
					break;						// �ǂݍ��݂��I��
				}
				else if (!strcmp(aDataSearch, "HITTEST_PARTS")) {
					HitTestParts	*pHitTest	// ���i�̎�ޖ��̏��̃|�C���^
									= &pHitTestSet->aHitTest[pHitTestSet->nHitTestNum].aHitTestParts[pHitTestSet->aHitTest[pHitTestSet->nHitTestNum].nHitTestPartsNum];

					// �����蔻����̓ǂݍ��݂��J�n
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (!strcmp(aDataSearch, "HITTEST_PARTS_END")) {
							pHitTestSet->aHitTest[pHitTestSet->nHitTestNum].nHitTestPartsNum++;	// �����蔻��̕��i�������Z
							break;																// �ǂݍ��݂��I��
						}
						else if (!strcmp(aDataSearch, "TYPE:"))			{ fscanf(pFile, "%d", &pHitTest->hitTestForm); }	// �����蔻��̎��
						else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pHitTest->fWidth); }			// ��
						else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pHitTest->fHeight); }		// ����
						else if (!strcmp(aDataSearch, "DEPTH:"))		{ fscanf(pFile, "%f", &pHitTest->fDepth); }			// ���s��
						else if (!strcmp(aDataSearch, "RELATIVE_POS:")) {		// ���Έʒu
							fscanf(pFile, "%f", &pHitTest->fixedRelativePos.x);	// X
							fscanf(pFile, "%f", &pHitTest->fixedRelativePos.y);	// Y
							fscanf(pFile, "%f", &pHitTest->fixedRelativePos.z);	// Z
						}
						else if (!strcmp(aDataSearch, "GHOST:"))	{						// ���蔲���t���O
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_TOP]);		// ���
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_BOTTOM]);	// ����
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_LEFT]);		// ����
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_RIGHT]);	// �E��
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_BACK]);		// ��
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_FRONT]);	// ��O
						}
					}
				}
			}
		}
	}
}

//========================================
// CollisionCheck�֐� - �Փ˃`�F�b�N -
// Author:RIKU NISHIMURA
//========================================
void CollisionCheck(VECTOR vector, Collision *pCollision, CollisionInfo myCollInfo, CollisionInfo pairCollInfo, COLLCHK_MODE mode)
{
	for (int nCntMyParts = 0; nCntMyParts < myCollInfo.hitTest.nHitTestPartsNum; nCntMyParts++)
	{
		for (int nCntPairParts = 0; nCntPairParts < pairCollInfo.hitTest.nHitTestPartsNum; nCntPairParts++)
		{
			HIT_TEST_FORM	my					= myCollInfo.hitTest.aHitTestParts[nCntMyParts].hitTestForm;		// �����̓����蔻��̎��
			HIT_TEST_FORM	pair				= pairCollInfo.hitTest.aHitTestParts[nCntPairParts].hitTestForm;	// ����̓����蔻��̎��
			HitTestParts	myHitTestParts		= myCollInfo.hitTest.aHitTestParts[nCntMyParts];					// �����̓����蔻��̕��i
			HitTestParts	pairHitTestParts	= pairCollInfo.hitTest.aHitTestParts[nCntPairParts];				// ����̓����蔻��̕��i

			// ���[�h�ɉ���������
			switch (mode)
			{
				//========== *** �ʏ� ***
			case COLLCHK_MODE_NORMAL:
				/*/ �l�p����l�p /*/if (my == HIT_TEST_FORM_SQUARE && pair == HIT_TEST_FORM_SQUARE) { SquareSquareCollision(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts); }
				break;
				//========== *** �����낵 ***
			case COLLCHK_MODE_LOOKDOWN:
				/*/ �l�p			����l�p /*/if (my == HIT_TEST_FORM_SQUARE			&& pair == HIT_TEST_FORM_SQUARE) { SquareSquareCollisionLookDown(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts); }
				/*/ �_				����l�p /*/if (my == HIT_TEST_FORM_POINT			&& pair == HIT_TEST_FORM_SQUARE) { PointSquareCollisionLookDown(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts, false,true); }
				/*/ �_(����)		����l�p /*/if (my == HIT_TEST_FORM_POINT_RFLC		&& pair == HIT_TEST_FORM_SQUARE) { PointSquareCollisionLookDown(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts, true,false); }
				/*/ �_(���薳��)	����l�p /*/if (my == HIT_TEST_FORM_POINT_NOSLIP	&& pair == HIT_TEST_FORM_SQUARE) { PointSquareCollisionLookDown(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts, false,false); }
				break;
			}
		}
	}
}

//========================================
// InsideCheck�֐� - �����`�F�b�N -
// Author:RIKU NISHIMURA
//========================================
void InsideCheck(Collision *pCollision) 
{
	if (pCollision == NULL) 
	{// �Փˏ��NULL�̎��A
		// �������I��
		return;
	}

	pCollision->bInside = true;	// �����t���O��^�ɂ���

	for (int nCntHitDirection = 0; nCntHitDirection < HIT_DIRECTION_MAX; nCntHitDirection++)
	{
		if (!pCollision->aInside[nCntHitDirection])
		{// �J�E���g���������t���O���U�̎��A
			pCollision->bInside = false;	// �����t���O(����)���U�ɂ���
			break;							// �J��Ԃ������𔲂���
		}
	}
}

//========================================
// CollisionOverwrite�֐� - �Փˏ��̏㏑�� -
// Author:RIKU NISHIMURA
//========================================
void CollisionOverwrite(Collision *pOverwriteCollision, Collision collision) 
{
	pOverwriteCollision->bHit			|= collision.bHit;			// ������t���O
	pOverwriteCollision->bHitDirection	|= collision.bHitDirection;	// �Փ˃t���O
	pOverwriteCollision->bInside		|= collision.bInside;		// �����t���O
	pOverwriteCollision->bGhost			|= collision.bGhost;		// ���蔲���t���O

	// �������̓�����t���O/�Փ˃t���O/�����t���O/���蔲���t���O���㏑��
	for (int nCntDirection = 0; nCntDirection < HIT_DIRECTION_MAX; nCntDirection++)
	{
		pOverwriteCollision->aHit[nCntDirection]			|= collision.aHit[nCntDirection];
		pOverwriteCollision->aHitDirection[nCntDirection]	|= collision.aHitDirection[nCntDirection];
		pOverwriteCollision->aInside[nCntDirection]			|= collision.aInside[nCntDirection];
		pOverwriteCollision->aGhost[nCntDirection]			|= collision.aGhost[nCntDirection];
	}
}

//========================================
// PositionUpdate�֐� - �ʒu�̍X�V���� -
// Author:RIKU NISHIMURA
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
// Author:RIKU NISHIMURA
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
// Author:RIKU NISHIMURA
//========================================
void TurnAround(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation)
{
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
// TurnAroundLookDown�֐� - �����]������(�����낵) -
// Author:RIKU NISHIMURA
//========================================
void TurnAroundLookDown(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation)
{
	float fAngleDiff	// 2�_�̊p�x�̍�
		= AngleDifference(
			pRot->y,
			FindAngleLookDown(pos, targetPos));

	if (fAngleDiff < 0.0f)
	{// �p�x�̍����}�C�i�X�������������A
		// �}�C�i�X�����ɉ�]
		pRot->y -= fRotation;
	}
	if (fAngleDiff > 0.0f)
	{// �p�x�̍����v���X�������������A
		// �v���X�����ɉ�]
		pRot->y += fRotation;
	}
}

//========================================
// FindDistance�֐� - 2�_�̋��������߂� -
// Author:RIKU NISHIMURA
//========================================
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float x = pos1.x - pos2.x;	// ��������X��
	float y = pos1.y - pos2.y;	// ��������Y��

	// 2�_�̋�����Ԃ�
	return sqrtf((x * x) + (y * y));
}

//========================================
// FindDistanceLookDown�֐� - 2�_�̋��������߂�(�����낵) -
// Author:RIKU NISHIMURA
//========================================
float FindDistanceLookDown(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float x = pos1.x - pos2.x;	// ��������X��
	float z = pos1.z - pos2.z;	// ��������Z��

	// 2�_�̋�����Ԃ�
	return sqrtf((x * x) + (z * z));
}

//========================================
// FindAngle�֐� - 2�_�̊p�x�����߂� -
// Author:RIKU NISHIMURA
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
// FindAngleLookDown�֐� - 2�_�̊p�x�����߂�(�����낵) -
// Author:RIKU NISHIMURA
//========================================
float FindAngleLookDown(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos)
{
	float fAngle;	// �p�x

	fAngle = atan2f(targetPos.z - pos.z, targetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1;

	return fAngle;
}


//========================================
// FindIntersectionPointLookDown�֐� - ��_�����߂�(�����낵) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 FindIntersectionPointLookDown(D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2)
{
	float fVecA1 = FindDistanceLookDown(posA2, posA1);	// �x�N�g��a1
	float fVecA2 = FindDistanceLookDown(posB1, posB2);	// �x�N�g��a2
	float fVecB1 = FindDistanceLookDown(posA1, posB1);	// �x�N�g��b1
	float fVecB2 = FindDistanceLookDown(posB2, posA1);	// �x�N�g��b2

	float fAreaS1 = (	// �ʐ�s1
		((posA2.x - posA1.x) * (posB2.z - posA1.z)) -
		((posA2.z - posA1.z) * (posB2.x - posA1.x))) * 0.5f;
	float fAreaS2 = (	// �ʐ�s2
		((posA2.x - posA1.x) * (posA1.z - posB1.z)) -
		((posA2.z - posA1.z) * (posA1.x - posB1.x))) * 0.5f;

	// ��_�̍��W��Ԃ�
	return	D3DXVECTOR3(
			posB2.x + (((posB1.x - posB2.x) * fAreaS1) / (fAreaS1 + fAreaS2)),
			0.0f,
			posB2.z + (((posB1.z - posB2.z) * fAreaS1) / (fAreaS1 + fAreaS2)));
}

//========================================
// AngleDifference�֐� - �p�x�̍������߂� -
// Author:RIKU NISHIMURA
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

	if ((fAngle >= 0.0f) && (fTargetAngle <= 0.0f))
	{// �p�x���v���X�ŁA�ڕW�p�x���}�C�i�X�̎��A
		if (fAngle - D3DX_PI <= fTargetAngle)
		{// �ڕW�p�x�������ɋ߂����A
			// �p�x�̍���ݒ�
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle - D3DX_PI >= fTargetAngle)
		{// �ڕW�p�x���㑤�ɋ߂����A
			// �p�x�̍���ݒ�
			fAngleDifference = (D3DX_PI - fAngle) + (D3DX_PI + fTargetAngle);
		}
	}

	if ((fAngle <= 0.0f) && (fTargetAngle >= 0.0f))
	{// �p�x���}�C�i�X�ŁA�ڕW�p�x���v���X�̎��A
		if (fAngle + D3DX_PI >= fTargetAngle)
		{// �ڕW�p�x�������ɋ߂����A
			// �p�x�̍���ݒ�
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle + D3DX_PI <= fTargetAngle)
		{// �ڕW�p�x���㑤�ɋ߂����A
			// �p�x�̍���ݒ�
			fAngleDifference = -(D3DX_PI + fAngle) - (D3DX_PI - fTargetAngle);
		}
	}

	return fAngleDifference;
}

//========================================
// ControlAngle�֐� - �p�x�𐧌䂷�� -
// Author:RIKU NISHIMURA
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

//========================================
// ConvertAngleToDirection�֐� - �p�x���w�肳�ꂽ���̕����ɕϊ����� -
// Author:RIKU NISHIMURA
//========================================
float ConvertAngleToDirection(float fAngle, int nDirection)
{
	float fRange = ((D3DX_PI * 2) / nDirection);	// �������̊p�x��

	for (int nCntDirection = 0; nCntDirection < nDirection + 1; nCntDirection++)
	{
		if ((fAngle >= -D3DX_PI - (fRange * 0.5f) + (fRange * nCntDirection))
			&&
			(fAngle <= -D3DX_PI + (fRange * 0.5f) + (fRange * nCntDirection)))
		{// �J�E���g�̕����̊p�x���̓����̎��A
			fAngle = -D3DX_PI + (fRange * nCntDirection);	// �p�x��ݒ�
			break;	// �J��Ԃ������𔲂���
		}
	}

	return fAngle;
}

//========================================
// ConvToBirdsEyeView�֐� - �ʒu���Ր}�ɕϊ�����(Z��Y) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvPosToBirdsEyeView(D3DXVECTOR3 pos)
{
	float fYTemp = pos.y;
	pos.y = -pos.z;
	pos.z = fYTemp;

	return pos;
}

//========================================
// ConvToBirdsEyeView�֐� - �������Ր}�ɕϊ�����(Z��Y) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvRotToBirdsEyeView(D3DXVECTOR3 rot)
{
	float fYTemp = rot.y;
	rot.y = -rot.z;
	rot.y *= -1;
	rot.y -= D3DX_PI;
	rot.z = fYTemp;
	rot.z += D3DX_PI;
	rot.z *= -1;

	return rot;
}

//========================================
// ControlRot�֐� - �����𐧌䂷�� -
// Author:RIKU NISHIMURA
//========================================
void RotControl(D3DXVECTOR3 *pRot)
{
	FloatLoopControl(&pRot->x, D3DX_PI, -D3DX_PI);
	FloatLoopControl(&pRot->y, D3DX_PI, -D3DX_PI);
	FloatLoopControl(&pRot->z, D3DX_PI, -D3DX_PI);
}

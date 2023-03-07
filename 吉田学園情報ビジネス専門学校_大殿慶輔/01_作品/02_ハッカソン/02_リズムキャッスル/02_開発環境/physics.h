//========================================
// 
// �����֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** physics.h ***
//========================================
#ifndef _PHYSICS_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _PHYSICS_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include <stdio.h>
#include "main.h"

//****************************************
// �}�N����`
//****************************************
// �����蔻��̍ő吔
// �����蔻��̕��i�̍ő吔
#define HIT_TEST_MAX		(16)
#define HIT_TEST_PARTS_MAX	(32)

//****************************************
// �񋓌^�̒�`
//****************************************
// �Փ˃`�F�b�N�̃��[�h
typedef enum 
{
	COLLCHK_MODE_NORMAL,	// �ʏ�
	COLLCHK_MODE_LOOKDOWN,	// �����낵
}COLLCHK_MODE;

// �����蔻��̌`
typedef enum
{
	HIT_TEST_FORM_CIRCLE,		// �~�`
	HIT_TEST_FORM_SQUARE,		// �l�p�`
	HIT_TEST_FORM_POINT,		// �_
	HIT_TEST_FORM_POINT_RFLC,	// �_(����)
	HIT_TEST_FORM_POINT_NOSLIP,	// �_(���薳��)
	HIT_TEST_FORM_MAX,
}HIT_TEST_FORM;

// �Փ˕���
typedef enum 
{
	HIT_DIRECTION_TOP,		// ��
	HIT_DIRECTION_BOTTOM,	// ��
	HIT_DIRECTION_LEFT,		// ��
	HIT_DIRECTION_RIGHT,	// �E
	HIT_DIRECTION_BACK,		// ��
	HIT_DIRECTION_FRONT,	// ��O
	HIT_DIRECTION_MAX,
}HIT_DIRECTION;

// �x�N�g���̎��
typedef enum 
{
	VECTOR_X,		// X����
	VECTOR_Y,		// Y����
	VECTOR_Z,		// Z����
	VECTOR_NONE,	// ����
	VECTOR_MAX
}VECTOR;

// ���_
typedef enum 
{
	VERTEX_X0Y0,	// ����
	VERTEX_X1Y0,	// �E��
	VERTEX_X0Y1,	// ����
	VERTEX_X1Y1,	// �E��
	VERTEX_MAX,
}VERTEX;

// 4����
typedef enum 
{
	DIRECTION_UP,			// ��
	DIRECTION_DOWN,			// ��
	DIRECTION_LEFT,			// ��
	DIRECTION_RIGHT,		// �E
	DIRECTION_BACK,			// ��
	DIRECTION_FRONT,		// ��O
	
	DIRECTION_LEFT_BACK,	// ����
	DIRECTION_LEFT_FRONT,	// ����O
	DIRECTION_RIGHT_BACK,	// �E��
	DIRECTION_RIGHT_FRONT,	// �E��O
	DIRECTION_MAX,
}DIRECTION;

// �������
typedef enum 
{
	SWING_DIRECTION_LEFT,	// ��
	SWING_DIRECTION_RIGHT,	// �E
}SWING_DIRECTION;

//****************************************
// �\���̂̒�`
//****************************************
// �Փˏ��\����
typedef struct
{
	bool	bHitDirection;	// �Փ˃t���O
	bool	aHitDirection	// �������̏Փ˃t���O
			[HIT_DIRECTION_MAX];
	bool	bInside;		// �͈͓��t���O
	bool	aInside			// �������͈͓̔��t���O
			[HIT_DIRECTION_MAX];
	bool	bHit;			// ������t���O�������I�ɐڐG���Ă���
	bool	aHit			// �������̓�����t���O
			[HIT_DIRECTION_MAX];
	bool	bGhost;			// ���蔲���t���O���񕨗��I�ɐڐG���Ă���
	bool	aGhost			// �������̂��蔲���t���O
			[HIT_DIRECTION_MAX];
	float	fLength;		// �߂荞�݋���
}Collision;

// �����蔻��̕��i���\����
typedef struct
{
	HIT_TEST_FORM	hitTestForm;		// �����蔻��̌`
	float			fWidth;				// ��
	float			fHeight;			// ����
	float			fDepth;				// ���s��
	D3DXVECTOR3		fixedRelativePos;	// ���Έʒu
	bool			aGhost				// ���蔲�����
					[HIT_DIRECTION_MAX];
}HitTestParts;

// �����蔻��̏��\����
typedef struct 
{
	int				nHitTestPartsNum;	// �����蔻��̕��i�̐�
	HitTestParts	aHitTestParts		// �����蔻��̕��i�̏��
					[HIT_TEST_PARTS_MAX];
}HitTest;

// �����蔻��̐ݒ���\����
typedef struct
{
	int		nHitTestNum;	// �����蔻��̐�
	HitTest aHitTest		// �����蔻��̏��
			[HIT_TEST_MAX];
}HitTestSet;

// �����蔻��Ǘ��\����
typedef struct
{
	int	nHitTest;	// �����蔻��ԍ�
}HitTestInfo;

// �Փ˔���ɕK�v�ȏ��\����
typedef struct
{
	D3DXVECTOR3 *pPos;		// ���݂̈ʒu�̃|�C���^
	D3DXVECTOR3 posOld;		// �ߋ��̈ʒu�̃|�C���^
	D3DXVECTOR3 *pMove;		// �ړ��ʂ̃|�C���^
	D3DXVECTOR3 *pRot;		// ���݂̌����̃|�C���^
	D3DXVECTOR3 rotOld;		// �ߋ��̌����̃|�C���^
	HitTest		hitTest;	// �����蔻��̏��
}CollisionInfo;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �x�N�g�����̓ǂݍ��ݏ���
// �����蔻��ݒ���̓ǂݍ��ݏ���
void LoadVector(FILE *pFile, D3DXVECTOR3 *pVector);
void LoadHitTestSet(FILE *pFile, HitTestSet *pHitTestSet);

// �Փ˃`�F�b�N
// �����`�F�b�N
// �Փˏ��̏㏑��
void CollisionCheck(VECTOR vector, Collision *pCollision, CollisionInfo myCollInfo, CollisionInfo pairCollInfo, COLLCHK_MODE mode);
void InsideCheck(Collision *pCollision);
void CollisionOverwrite(Collision *pOverwriteCollision, Collision collision);

// �ʒu�̍X�V����
void PositionUpdate(D3DXVECTOR3 *pos, D3DXVECTOR3 *move);

// ���i����
void Driving(D3DXVECTOR3 *pos, D3DXVECTOR3 rot, D3DXVECTOR3 *move, float fMove);

// �����]������
// �����]������(�����낵)
void TurnAround(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation);
void TurnAroundLookDown(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation);

// 2�_�̋��������߂�
// 2�_�̋��������߂�(�����낵)
// 2�_�̊p�x�����߂�
// 2�_�̊p�x�����߂�(�����낵)
// ��_�̍��W�����߂�(�����낵)
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
float FindDistanceLookDown(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
float FindAngleLookDown(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
D3DXVECTOR3 FindIntersectionPointLookDown(D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2);

// �p�x�̍������߂�
// �p�x�𐧌䂷��
// �p�x���w�肳�ꂽ���̕����ɕϊ�����
float AngleDifference(float fAngle, float fTargetAngle);
void ControlAngle(float *pAngle);
float ConvertAngleToDirection(float fAngle, int nDirection);

// �ʒu���Ր}�ɕϊ�����(Z��Y)
// �������Ր}�ɕϊ�����(Z��Y)
D3DXVECTOR3 ConvPosToBirdsEyeView(D3DXVECTOR3 pos);
D3DXVECTOR3 ConvRotToBirdsEyeView(D3DXVECTOR3 rot);

// �����𐧌�
void RotControl(D3DXVECTOR3 *pRot);

#endif
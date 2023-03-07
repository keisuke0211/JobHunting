//========================================
//  *** physics.h ***
//========================================
#ifndef _PHYSICS_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _PHYSICS_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �����蔻��̎��
typedef enum
{
	HIT_TEST_TYPE_CIRCLE = 0,	// �~�`
	HIT_TEST_TYPE_SQUARE,		// �l�p�`
	HIT_TEST_TYPE_MAX,
}HIT_TEST_TYPE;

//****************************************
// �\���̂̒�`
//****************************************

// �����蔻��̎�ޖ��̏��
typedef struct 
{
	// ���ފ֘A
	char aLoadName[TXT_MAX];	// �ǂݍ��ݖ�
}HitTestType;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �����蔻��̎�ޖ��̏����擾
HitTestType *GetHitTestType(void);

// �����蔻��̎�ނ𕶎��񂩂�ǂݍ���
void StringLoadHitTestType(char aString[TXT_MAX], HIT_TEST_TYPE *pType);

// �Փ˃`�F�b�N
bool CheckHit(
	D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth1, float fHeight1, HIT_TEST_TYPE hitTestType1,
	D3DXVECTOR3 pos2, D3DXVECTOR3 rot2, float fWidth2, float fHeight2, HIT_TEST_TYPE hitTestType2);

// �ʒu�̍X�V����
void PositionUpdate(D3DXVECTOR3 *pos, D3DXVECTOR3 *move);

// ���i����
void Driving(D3DXVECTOR3 *pos, D3DXVECTOR3 rot, D3DXVECTOR3 *move, float fMove);

// �����]������
void TurnAround(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation);

// 2�_�̋��������߂�
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);

// 2�_�̊p�x�����߂�
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);

// �p�x�̍������߂�
float AngleDifference(float fAngle, float fTargetAngle);

// �p�x�𐧌䂷��
void ControlAngle(float *pAngle);

#endif
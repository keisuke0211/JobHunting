//========================================
//  *** polygon.h ***
//========================================
#ifndef _POLYGON_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _POLYGON_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �����̎��
typedef enum
{
	ANGLE_TYPE_FREE = 0,	// ���R
	ANGLE_TYPE_FIXED,		// �Œ�
	ANGLE_TYPE_MAX,
}ANGLE_TYPE;

// �A�j���[�V�����̎��
typedef enum
{
	ANIM_TYPE_NORMAL = 0,	// �ʏ�
	ANIM_TYPE_TURN,			// �܂�Ԃ�
	ANIM_TYPE_MAX,
}ANIM_TYPE;

//****************************************
// �\���̂̒�`
//****************************************

// �����̎�ޖ��̏��
typedef struct 
{
	// ���ފ֘A
	char aLoadName[TXT_MAX];	// �ǂݍ��ݖ�
}AngleType;

// �A�j���[�V�����̎�ޖ��̏��
typedef struct
{
	// ���ފ֘A
	char aLoadName[TXT_MAX];	// �ǂݍ��ݖ�
}AnimType;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �����̎�ޖ��̏����擾
AngleType *GetAngleType(void);

// �A�j���[�V�����̎�ޖ��̏����擾
AnimType *GetAnimType(void);

// �����̎�ނ𕶎��񂩂�ǂݍ���
void StringLoadAngleType(char aString[TXT_MAX], ANGLE_TYPE *pType);

// �A�j���[�V�����̎�ނ𕶎��񂩂�ǂݍ���
void StringLoadAnimType(char aString[TXT_MAX], ANIM_TYPE *pType);

// ���_���W�̐ݒ菈��
void SetVertexPosition(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType);

// rhw�̐ݒ菈��
void SetRHW(VERTEX_2D *pVtx);

// ���_�F�̐ݒ菈��
void SetVertexColor(VERTEX_2D *pVtx, Color col);

// �e�N�X�`�����W�̐ݒ菈��
void SetTexturePosition(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);

// �A�j���[�V�����̃J�E���g����
int AnimationCount(int *pPtn, int nPtnMin, int nPtnMax, int *pCounterAnim, int nAnimTime, ANIM_TYPE animType);

#endif
//========================================
//  *** eff_particle_00.h ***
//========================================
#ifndef _EFF_PARTICLE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_PARTICLE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �e�̎��
typedef enum 
{
	EFF_PARTICLE_00_TYPE_SPARK_00 = 0,	// �Ή�[00]
	EFF_PARTICLE_00_TYPE_SPARK_01,		// �Ή�[01]
	EFF_PARTICLE_00_TYPE_01,			// �e�̔j��(�v���C���[)
	EFF_PARTICLE_00_TYPE_02,			// �e�̔j��(�G)
	EFF_PARTICLE_00_TYPE_MAX,
	EFF_PARTICLE_00_TYPE_N1
}EFF_PARTICLE_00_TYPE;

//****************************************
// �\���̂̒�`
//****************************************

// �e[00]�\���̂̒�`
typedef struct
{
								//========== *** �ʒu�֘A ***
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	float fMove;				// �ړ���
	float fRotation;			// ��]��
	D3DXVECTOR3 rot;			// ����
	float fWidth;				// ��
	float fHeight;				// ����
	float fHitTestRadius;		// �����蔻��̔��a
								//========== *** ���ޏ�� ***
	EFF_PARTICLE_00_TYPE type;	// ���
	bool bUse;					// �g�p����Ă��邩�t���O
								//========== *** ��Ԓl ***
	int nLife;					// ����
}Eff_particle_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitEff_particle_00	(void);															// �e[00] �̏���������
void UpdateEff_particle_00	(void);															// �e[00] �̍X�V����
void SetEff_particle_00		(D3DXVECTOR3 pos, D3DXVECTOR3 rot, EFF_PARTICLE_00_TYPE type);	// �e[00] �̐ݒ菈��

#endif
//========================================
//  *** atk_explosion_00.h ***
//========================================
#ifndef _ATK_EXPLOSION_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ATK_EXPLOSION_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// ����[00] �̎��
typedef enum
{
	ATK_EXPLOSION_00_TYPE_00 = 0,
	ATK_EXPLOSION_00_TYPE_01,
	ATK_EXPLOSION_00_TYPE_02,
	ATK_EXPLOSION_00_TYPE_03,
	ATK_EXPLOSION_00_TYPE_04,
	ATK_EXPLOSION_00_TYPE_05,
	ATK_EXPLOSION_00_TYPE_06,
	ATK_EXPLOSION_00_TYPE_07,
	ATK_EXPLOSION_00_TYPE_08,
	ATK_EXPLOSION_00_TYPE_MAX,
	ATK_EXPLOSION_00_TYPE_NOT
}ATK_EXPLOSION_00_TYPE;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void InitAtk_explosion_00	(void);																			// ����[00] �̏���������
void UninitAtk_explosion_00	(void);																			// ����[00] �̏I������
void UpdateAtk_explosion_00	(void);																			// ����[00] �̍X�V����
void DrawAtk_explosion_00	(void);																			// ����[00] �̕`�揈��
void SetAtk_explosion_00	(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ATK_EXPLOSION_00_TYPE type, int nCntChain);	// ����[00] �̐ݒ菈��

#endif
//========================================
//  *** eff_light_00.h ***
//========================================
#ifndef _EFF_LIGHT_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_LIGHT_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//========== �񋓌^�̒�`		==========// 

// ���C�g[00]�̏��
typedef enum
{
	EFF_LIGHT_00_STATE_NORMAL = 0,	// �ʏ�
	EFF_LIGHT_00_STATE_MAX
}EFF_LIGHT_00_STATE;

// ���C�g�̎��
typedef enum 
{
	EFF_LIGHT_00_TYPE_00 = 0,
	EFF_LIGHT_00_TYPE_01,
	EFF_LIGHT_00_TYPE_MAX
}EFF_LIGHT_00_TYPE;

//========== �v���g�^�C�v�錾	==========// 
void InitEff_light_00(void);																													// ���C�g[00] �̏���������
void UninitEff_light_00(void);																													// ���C�g[00] �̏I������
void UpdateEff_light_00(void);																													// ���C�g[00] �̍X�V����
void DrawEff_light_00(void);																													// ���C�g[00] �̕`�揈��
void SetEff_light_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fMove, Color col, float fWidth, float fHeight, int nLife, EFF_LIGHT_00_TYPE type);	// ���C�g[00] �̐ݒ菈��

#endif
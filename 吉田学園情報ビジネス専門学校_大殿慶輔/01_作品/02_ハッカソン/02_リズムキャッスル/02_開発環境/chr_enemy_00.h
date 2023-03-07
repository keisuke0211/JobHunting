//========================================
// 
// CHR:�G[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_enemy_00.h ***
//========================================
#ifndef _CHR_ENEMY_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHR_ENEMY_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// CHR:�G[00] �̑������
typedef enum 
{
	CHR_ENEMY_00_ELEM_A,
	CHR_ENEMY_00_ELEM_B,
	CHR_ENEMY_00_ELEM_X,
	CHR_ENEMY_00_ELEM_Y,
}CHR_ENEMY_00_ELEM;

//****************************************
// �\���̒�`
//****************************************
// CHR:�G[00] �̏��
typedef struct
{
	int		nType;			// ���

	D3DXVECTOR3 pos;		// �ʒu
	bool	bUse;			// �g�p����Ă��邩�t���O
	int		nCounterMove;	// �ړ��J�E���^�[
	int		nPtn;			// �p�^�[��No.
	int		nCounterAnim;	// �A�j���[�V�����J�E���^�[
}Chr_enemy_00;

// CHR:�G[00] �̎�ޖ��̏��
typedef struct
{
	char				aTexturePath	// �e�N�X�`���̑��΃p�X
						[TXT_MAX];
	float				fWidth;			// ��
	float				fHeight;		// ����
	int					nPtnWidth;		// �p�^�[����
	int					nPtnHeight;		// �p�^�[������
	int					nMoveTime;		// �ړ��ɂ����鎞��
	CHR_ENEMY_00_ELEM	elem;			// ����
}Chr_enemy_00Type;

//****************************************
// �}�N����`
//****************************************
//�G�̍ő吔
#define CHR_ENEMY_00_MAX	(128)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// CHR:�G[00] �̏��̎擾
// CHR:�G[00] �̎�ޖ��̏��̎擾
Chr_enemy_00 *GetChr_enemy_00(void);
Chr_enemy_00Type *GetChr_enemy_00Type(void);

void LoadChr_enemy_00(void);

// CHR:�G[00] �̏���������
// CHR:�G[00] �̏I������
// CHR:�G[00] �̍X�V����
// CHR:�G[00] �̕`�揈��
void InitChr_enemy_00(void);
void UninitChr_enemy_00(void);
void UpdateChr_enemy_00(void);
void DrawChr_enemy_00(void);

// CHR:�G[00] �̐ݒ菈��
void SetChr_enemy_00(int nType);

#endif
//========================================
//  *** atk_bullet_00.h ***
//========================================
#ifndef _ATK_BULLET_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _ATK_BULLET_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "attack.h"
#include "polygon.h"
#include "atk_explosion_00.h"
#include "eff_particle_00.h"

//****************************************
// �}�N����`
//****************************************
#define ATK_BULLET_00_MAX		(256)	// �e[00] �̍ő吔
#define ATK_BULLET_00_TYPE_MAX	(80)	// �e[00] �̎�ނ̍ő�

//****************************************
// �\���̂̒�`
//****************************************

// �e[00]�\���̂̒�`
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	float		fMove;			// �ړ���
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 drawRot;		// +�`�����
	float		fWidth;			// ��
	float		fHeight;		// ����
	float		fHitTestWidth;	// �����蔻��̕�
	float		fHitTestHeight;	// �����蔻��̍���

	// ���ޏ��
	int				nType;		// ���
	ATK_PARENT_TYPE	parentType;	// �e�̎��

	// ��Ԓl
	bool	bUse;				// �g�p����Ă��邩�t���O
	int		nLife;				// ����
	int		nDamage;			// �_���[�W
	int		nPenetrationNum;	// �c��̊ђʉ�
	int		nPattern;			// �p�^�[��No.
	int		nCounterAnim;		// �A�j���[�V�����̃J�E���^�[
	int		nCounterRotation;	// ��]�J�E���^�[
}Atk_bullet_00;

// �e[00] �̎�ޖ��̏��
typedef struct
{
	// ���ޏ��
	char	aLoadName	// �ǂݍ��ݖ�
			[TXT_MAX];

	// �����ڊ֘A
	char		aTexturPath						// �e�N�X�`���̑��΃p�X
				[ATK_PARENT_TYPE_MAX][TXT_MAX];	//
	int			nPtnMaxX;						// �p�^�[���̍ő吔(��)
	int			nPtnMaxY;						// �p�^�[���̍ő吔(�c)
	int			nAnimPtnMin;					// �A�j���p�^�[��No.�̉���
	int			nAnimPtnMax;					// �A�j���p�^�[��No.�̏��
	int			nAnimTime;						// �A�j���[�V�����̐؂�ւ�����
	ANIM_TYPE	animType;						// �A�j���[�V�����̎��
	float		fWidth;							// ��
	float		fHeight;						// ����
	ANGLE_TYPE	angleType;						// �����̎��
	float		fRotation;						// ��]��

	// ���\�֘A5
	float			fHitTestWidth;	// �����蔻��̕�
	float			fHitTestHeight;	// �����蔻��̍���
	bool			bHitAlly;		// �����ɂ������邩�t���O
	float			fMove;			// �ړ���
	int				nLife;			// ����
	int				nPenetration;	// �ђʉ�
	int				nDamage;		// �_���[�W

	// ���o�֘A
	SOUND_LABEL				soundSet;				// �ݒ莞�̃T�E���h
	ATK_EXPLOSION_00_TYPE	exType;					// �����̎��
	bool					bRotation;				// ��]�t���O
	Color					aLightColor				// ���C�g�̐F
							[ATK_PARENT_TYPE_MAX];	// 
	float					fLightWidth;			// ���C�g�̕�
	float					fLightHeight;			// ���C�g�̍���
	int						nLightLife;				// ���C�g�̎���
	float					fLightMove;				// ���C�g�̈ړ���
	EFF_PARTICLE_00_TYPE	aPtclType				// �p�[�e�B�N���̎��
							[ATK_PARENT_TYPE_MAX];	//
	int						nPtclNum;				// �p�[�e�B�N���̔�����
}Atk_bullet_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �e[00] �̏����擾
Atk_bullet_00 *GetAtk_bullet_00(void);

// �e[00] �̎�ޖ��̏����擾
Atk_bullet_00Type *GetAtk_bullet_00Type(void);

// �e[00] �̓ǂݍ��ݏ���
void LoadAtk_bullet_00(void);

// �e[00] �̎�ނ𕶎��񂩂�ǂݍ���
void StringLoadAtk_bullet_00(char aString[TXT_MAX], int *pType);

// �e[00] �̏���������
void InitAtk_bullet_00(void);

// �e[00] �̏I������
void UninitAtk_bullet_00(void);

// �e[00] �̍X�V����
void UpdateAtk_bullet_00(void);

// �e[00] �̕`�揈��
void DrawAtk_bullet_00(void);

// �e[00] �̐ݒ菈��
void SetAtk_bullet_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_PARENT_TYPE lookType);

// �e[00] �̔j�󏈗�
void DestructionAtk_bullet_00(Atk_bullet_00 *pAtk);

// �e[00] �̑S�j�󏈗�
void AllDestructionAtk_bullet_00(ATK_PARENT_TYPE parentType);

#endif
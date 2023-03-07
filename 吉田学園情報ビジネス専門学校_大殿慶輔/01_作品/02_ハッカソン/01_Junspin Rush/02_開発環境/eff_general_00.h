//========================================
//  *** eff_general_00.h ***
//========================================
#ifndef _EFF_GENERAL_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _EFF_GENERAL_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "polygon.h"
#include "sound.h"

//****************************************
// �}�N����`
//****************************************
#define EFF_GENERAL_00_TYPE_MAX				(80)	// �G�t�F�N�g�S��[00] �̎�ނ̍ő吔
#define EFF_GENERAL_00_MAX					(256)	// �G�t�F�N�g�S��[00] �̍ő吔
#define EFF_GENERAL_00_SOUND_MEMORY_MAX		(128)	// �G�t�F�N�g�S��[00] �̃T�E���h���̍ő�L����
#define EFF_GENERAL_00_SOUND_PATTERN_MAX	(128)	// �G�t�F�N�g�S��[00] �̃p�^�[��No.���̍ő�L����
#define EFF_GENERAL_00_SUMMON_MEMORY_MAX	(128)	// �G�t�F�N�g�S��[00] �̏������̍ő�L����

//****************************************
// �\���̂̒�`
//****************************************

// �G�t�F�N�g�S��[00] �̃T�E���h���\����
typedef struct
{
	int			nTime;		// �T�E���h��炷����
	SOUND_LABEL	soundLabel;	// �炷�T�E���h�̎��
}Eff_general_00Sound;

// �G�t�F�N�g�S��[00] �̃p�^�[��No.���\����
typedef struct
{
	int	nTime;		// �p�^�[��No.��ς��鎞��
	int	nPattern;	// �p�^�[��No.
}Eff_general_00Pattern;

// �G�t�F�N�g�S��[00] �̏������\����
typedef struct
{
	D3DXVECTOR3 pos;		// ��������ʒu
	float		fAngle;		// ��������p�x
	int			nTime;		// �������鎞��
	int			nEnemyType;	// ��������G�t�F�N�g�S�ʂ̎��
	int			nParentNum;	// ��������G�t�F�N�g�S�ʂ̐e�ԍ�
	int			nChildNum;	// ��������G�t�F�N�g�S�ʂ̎q���ԍ�
}Eff_general_00Summon;

// �G�t�F�N�g�S��[00] �\���̂̒�`
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3	pos;			// �ʒu
	D3DXVECTOR3	rot;			// ����
	D3DXVECTOR3	targetRot;		// �ڕW����
	float		fWidth;			// ��
	float		fHeight;		// ����
	D3DXVECTOR3	*pParentPos;	// �e�̈ʒu�̃|�C���^
	D3DXVECTOR3	parentPosTemp;	// �e�̈ʒu��ۑ�
	D3DXVECTOR3	*pParentRot;	// �e�̌����̃|�C���^
	D3DXVECTOR3	parentRotTemp;	// �e�̌�����ۑ�
	
	// ���ޏ��
	int	nType;	// ���
	
	// ��Ԓl
	int		nLife;				// ����
	bool	bUse;				// �g�p����Ă��邩�t���O
	bool	bChild;				// �q�t���O
	int		nPattern;			// �e�N�X�`���̃p�^�[��No.
	int		nCounterAnim;		// �A�j���[�V�����̃J�E���^�[
	int		nCounterSound;		// �T�E���h�J�E���^�[
	int		nCounterPattern;	// �p�^�[��No.�J�E���^�[
	int		nCounterSummon;		// �����J�E���^�[
	bool	bDraw;				// �`��t���O
}Eff_general_00;

// �G�t�F�N�g�S��[00] �̎�ޖ��̏��
typedef struct
{
	// ���ޏ��
	char	aLoadName	// �ǂݍ��ݖ�
			[TXT_MAX];	//

	// �����ڊ֘A
	char		aTexturPath		// �e�N�X�`���̑��΃p�X
				[TXT_MAX];		//
	int			nPtnMaxX;		// �p�^�[���̍ő吔(��)
	int			nPtnMaxY;		// �p�^�[���̍ő吔(�c)
	int			nAnimPtnMin;	// �A�j���p�^�[��No.�̉���
	int			nAnimPtnMax;	// �A�j���p�^�[��No.�̏��
	int			nAnimTime;		// �A�j���[�V�����ɂ����鎞��
	ANIM_TYPE	animType;		// �A�j���[�V�����̎��
	float		fWidth;			// ��
	float		fHeight;		// ����
	ANGLE_TYPE	angleType;		// �����̎��

	// ���\�֘A
	int						nLife;								// ����
	int						nSoundNum;							// �T�E���h�̐�
	Eff_general_00Sound		aSound								//
							[EFF_GENERAL_00_SOUND_MEMORY_MAX];	// �T�E���h�̏��
	int						nSoundLoopTime;						// �T�E���h�̃��[�v����
	int						nPatternNum;						// �p�^�[��No.�̐�
	Eff_general_00Pattern	aPattern							//
							[EFF_GENERAL_00_SOUND_PATTERN_MAX];	// �p�^�[��No.�̏��
	int						nPatternLoopTime;					// �p�^�[��No.�̃��[�v����
	int						nSummonNum;							// �����̐�
	Eff_general_00Summon	aSummon								//
							[EFF_GENERAL_00_SUMMON_MEMORY_MAX];	// �����̏��
	int						nSummonLoopTime;					// �����̃��[�v����
}Eff_general_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �G�t�F�N�g�S��[00] �̓ǂݍ��ݏ���
void LoadEff_general_00(void);

// �G�t�F�N�g�S��[00] �̎�ނ𕶎��񂩂�ǂݍ���
void StringLoadEff_general_00Type(char aString[TXT_MAX], int *pType);

// �G�t�F�N�g�S��[00] �̏���������
void InitEff_general_00(void);

// �G�t�F�N�g�S��[00] �̏I������
void UninitEff_general_00(void);

// �G�t�F�N�g�S��[00] �̍X�V����
void UpdateEff_general_00(void);

// �G�t�F�N�g�S��[00] �̕`�揈��
void DrawEff_general_00(void);

// �G�t�F�N�g�S��[00] �̐ݒ菈��
void SetEff_general_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bParent, D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pParentRot);

#endif
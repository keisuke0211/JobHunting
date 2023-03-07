//========================================
//  *** chr_enemy_00.h ***
//========================================
#ifndef _CHR_ENEMY_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHR_ENEMY_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "polygon.h"
#include "sound.h"
#include "atk_explosion_00.h"

//****************************************
// �}�N����`
//****************************************
// �G[00] �̎�ނ̍ő吔
// �G[00] �̍ő吔
// �G[00] �̍s�����̍ő�L����
// �G[00] �̃T�E���h���̍ő�L����
// �G[00] �̃p�^�[��No.���̍ő�L����
// �G[00] �̃G�t�F�N�g���̍ő�L����
// �G[00] �̔��ˏ��̍ő�L����
// �G[00] �̏������̍ő�L����
#define CHR_ENEMY_00_TYPE_MAX			(80)
#define CHR_ENEMY_00_MAX				(256)
#define CHR_ENEMY_00_ACT_MEMORY_MAX		(128)
#define CHR_ENEMY_00_SOUND_MEMORY_MAX	(128)
#define CHR_ENEMY_00_PATTERN_MEMORY_MAX	(128)
#define CHR_ENEMY_00_EFFECT_MEMORY_MAX	(128)
#define CHR_ENEMY_00_SHOT_MEMORY_MAX	(128)
#define CHR_ENEMY_00_SUMMON_MEMORY_MAX	(128)

// �G[00] �̖c��ݑ҂��ɂ����鎞�Ԃ�(���S�n����̋�����)�ϊ��{��
// �G[00] �̖c��ݑ҂��ɂ����鎞�Ԃ�(�e����̋�����)�ϊ��{��
#define CHR_ENEMY_00_SWELL_WAIT_TIME_DIAMETER_EXPLOSION	(0.1f)
#define CHR_ENEMY_00_SWELL_WAIT_TIME_DIAMETER_PARENT	(0.2f)

//****************************************
// �񋓌^�̒�`
//****************************************

// �G[00] �̏��
typedef enum
{
	CHR_ENEMY_00_STATE_NORMAL = 0,				// �ʏ�
	CHR_ENEMY_00_STATE_IN_POP,					// �o����
	CHR_ENEMY_00_STATE_IN_CLEAR,				// ������
	CHR_ENEMY_00_STATE_DAMAGE,					// �_���[�W
	CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE,	// �����ϐ�
	CHR_ENEMY_00_STATE_SWELL,					// �c���
	CHR_ENEMY_00_STATE_SWELL_WAIT,				// �c��ݑ҂�
	CHR_ENEMY_00_STATE_MAX
}CHR_ENEMY_00_STATE;

// �G[00] �̍s���̓ǂݍ��݂̎��
typedef enum
{
	CHR_ENEMY_00_ACT_LOAD_PROGRESS_START = 0 ,			// �O�i�J�n
	CHR_ENEMY_00_ACT_LOAD_PROGRESS_END,					// �O�i�I��
	CHR_ENEMY_00_ACT_LOAD_STRONG_PROGRESS_START,		// ���O�i�J�n
	CHR_ENEMY_00_ACT_LOAD_STRONG_PROGRESS_END,			// ���O�i�I��
	CHR_ENEMY_00_ACT_LOAD_LIGHT_PROGRESS_START,			// ��O�i�J�n
	CHR_ENEMY_00_ACT_LOAD_LIGHT_PROGRESS_END,			// ��O�i�I��
	CHR_ENEMY_00_ACT_LOAD_BACKWARD_START,				// ��i�J�n
	CHR_ENEMY_00_ACT_LOAD_BACKWARD_END,					// ��i�I��
	CHR_ENEMY_00_ACT_LOAD_STRONG_BACKWARD_START,		// ����i�J�n
	CHR_ENEMY_00_ACT_LOAD_STRONG_BACKWARD_END,			// ����i�I��
	CHR_ENEMY_00_ACT_LOAD_LIGHT_BACKWARD_START,			// ���i�J�n
	CHR_ENEMY_00_ACT_LOAD_LIGHT_BACKWARD_END,			// ���i�I��
	CHR_ENEMY_00_ACT_LOAD_DRIVING,						// ���i
	CHR_ENEMY_00_ACT_LOAD_SLIDE_LEFT_START,				// ���X���C�h�J�n
	CHR_ENEMY_00_ACT_LOAD_SLIDE_LEFT_END,				// ���X���C�h�I��
	CHR_ENEMY_00_ACT_LOAD_SLIDE_RIGHT_START,			// �E�X���C�h�J�n
	CHR_ENEMY_00_ACT_LOAD_SLIDE_RIGHT_END,				// �E�X���C�h�I��
	CHR_ENEMY_00_ACT_LOAD_TURN_AROUND_TO_PLAYER_START,	// �v���C���[�����]���J�n
	CHR_ENEMY_00_ACT_LOAD_TURN_AROUND_TO_PLAYER_END,	// �v���C���[�����]���I��
	CHR_ENEMY_00_ACT_LOAD_TRACKING_PLAYER_START,		// �v���C���[�ǔ��J�n
	CHR_ENEMY_00_ACT_LOAD_TRACKING_PLAYER_END,			// �v���C���[�ǔ��I��
	CHR_ENEMY_00_ACT_LOAD_STARE_PLAYER_START,			// �v���C���[�Î��J�n
	CHR_ENEMY_00_ACT_LOAD_STARE_PLAYER_END,				// �v���C���[�Î��I��
	CHR_ENEMY_00_ACT_LOAD_STARE_CENTER_START,			// ���S�Î��J�n
	CHR_ENEMY_00_ACT_LOAD_STARE_CENTER_END,				// ���S�Î��I��
	CHR_ENEMY_00_ACT_LOAD_GHOST_START,					// ���蔲���J�n
	CHR_ENEMY_00_ACT_LOAD_GHOST_END,					// ���蔲���I��
	CHR_ENEMY_00_ACT_LOAD_BULLET_GHOST_START,			// �e���蔲���J�n
	CHR_ENEMY_00_ACT_LOAD_BULLET_GHOST_END,				// �e���蔲���I��
	CHR_ENEMY_00_ACT_LOAD_INVINCIBLE_START,				// ���G�J�n
	CHR_ENEMY_00_ACT_LOAD_INVINCIBLE_END,				// ���G�I��
	CHR_ENEMY_00_ACT_LOAD_KNOCK_BACK_START,				// �m�b�N�o�b�N�J�n
	CHR_ENEMY_00_ACT_LOAD_KNOCK_BACK_END,				// �m�b�N�o�b�N�I��
	CHR_ENEMY_00_ACT_LOAD_SCALE_DOWN_START,				// �k���J�n
	CHR_ENEMY_00_ACT_LOAD_SCALE_DOWN_END,				// �k���I��
	CHR_ENEMY_00_ACT_LOAD_SUMMONING_ON_DIED,			// ���S������
	CHR_ENEMY_00_ACT_LOAD_CHILD_RELEASE,				// �e����
	CHR_ENEMY_00_ACT_LOAD_IMMEDIATE_POP,				// ���o��
	CHR_ENEMY_00_ACT_LOAD_IMMEDIATE_CLEAR,				// ��������
	CHR_ENEMY_00_ACT_LOAD_CLEAR,						// ������
	CHR_ENEMY_00_ACT_LOAD_LOOP,							// ���[�v
	CHR_ENEMY_00_ACT_LOAD_MAX
}CHR_ENEMY_00_ACT_LOAD;

// �G[00] �̍s���̎��
typedef enum
{
	CHR_ENEMY_00_ACT_PROGRESS = 0,		// �O�i
	CHR_ENEMY_00_ACT_STRONG_PROGRESS,	// ���O�i
	CHR_ENEMY_00_ACT_LIGHT_PROGRESS,	// ��O�i
	CHR_ENEMY_00_ACT_BACKWARD,			// ��i
	CHR_ENEMY_00_ACT_STRONG_BACKWARD,	// ����i
	CHR_ENEMY_00_ACT_LIGHT_BACKWARD,	// ���i
	CHR_ENEMY_00_ACT_SLIDE_LEFT,		// ���X���C�h
	CHR_ENEMY_00_ACT_SLIDE_RIGHT,		// �E�X���C�h
	CHR_ENEMY_00_ACT_TURN_AROUND,		// �����]��
	CHR_ENEMY_00_ACT_TRACKING,			// �ǔ�
	CHR_ENEMY_00_ACT_STARE,				// �Î�
	CHR_ENEMY_00_ACT_GHOST,				// ���蔲��
	CHR_ENEMY_00_ACT_BULLET_GHOST,		// �e���蔲��
	CHR_ENEMY_00_ACT_INVINCIBLE,		// ���G
	CHR_ENEMY_00_ACT_KNOCK_BACK,		// �m�b�N�o�b�N
	CHR_ENEMY_00_ACT_SCALE_DOWN,		// �k��
	CHR_ENEMY_00_ACT_SUMMONING_ON_DIED,	// ���S������
	CHR_ENEMY_00_ACT_MAX
}CHR_ENEMY_00_ACT;

//****************************************
// �\���̂̒�`
//****************************************

// �G[00] �̍s�����\����
typedef struct
{
	int						nTime;	// �s�����鎞��
	CHR_ENEMY_00_ACT_LOAD	type;	// �s���̎��
}Chr_enemy_00Act;

// �G[00] �̃T�E���h���\����
typedef struct
{
	int			nTime;		// �T�E���h��炷����
	SOUND_LABEL	soundLabel;	// �炷�T�E���h�̎��
}Chr_enemy_00Sound;

// �G[00] �̃p�^�[��No.���\����
typedef struct
{
	int	nTime;		// �p�^�[��No.��ς��鎞��
	int	nPattern;	// �p�^�[��No.
}Chr_enemy_00Pattern;

typedef struct 
{
	D3DXVECTOR3 pos;			// �z�u����ʒu
	float		fAngle;			// �z�u����p�x
	int			nTime;			// �z�u���鎞��
	int			nEffectType;	// �z�u����G�t�F�N�g�̎��
	bool		bChild;			// �q�t���O
}Chr_enemy_00Effect;

// �G[00] �̔��ˏ��\����
typedef struct
{
	D3DXVECTOR3 pos;			// ���˂���ʒu
	float		fAngle;			// ���˂���p�x
	int			nTime;			// ���˂��鎞��
	int			nBulletType;	// ���˂���e�̎��
}Chr_enemy_00Shot;

// �G[00] �̏������\����
typedef struct
{
	D3DXVECTOR3 pos;		// ��������ʒu
	float		fAngle;		// ��������p�x
	int			nTime;		// �������鎞��
	int			nEnemyType;	// ��������G�̎��
	int			nParentNum;	// ��������G�̐e�ԍ�
	int			nChildNum;	// ��������G�̎q���ԍ�
}Chr_enemy_00Summon;

// �G[00] �\���̂̒�`
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3		pos;				// �ʒu
	D3DXVECTOR3		move;				// �ړ���
	float			fMove;				// �ړ���
	float			fRotation;			// ��]��
	D3DXVECTOR3		rot;				// ����
	D3DXVECTOR3		targetRot;			// �ڕW����
	float			fWidth;				// ��
	float			fHeight;			// ����
	float			fHitTestWidth;		// �����蔻��̕�
	float			fHitTestHeight;		// �����蔻��̍���
	ScaleDiameter	scale;				// �傫���̔{��
	D3DXVECTOR3		*pTurnAroundPos;	// �����]���ʒu
	D3DXVECTOR3		*pTrackingPos;		// �ǔ��ʒu
	D3DXVECTOR3		*pStarePos;			// �Î��ʒu
	
	// ���ޏ��
	int		nType;		// ���
	int		nParentNum;	// �e�ԍ�
	int		nChildNum;	// �q���ԍ�
	
	// ��Ԓl
	bool	bUse;						// �g�p����Ă��邩�t���O
	bool	bBomb;						// ���e�����t���O
	bool	bReceivedExplosion;			// �������󂯂��t���O
	int		nSwellWaitTime;				// �c��ݑ҂��ɂ����鎞��
	int		nLife;						// �̗�
	bool	aState						// ��ԃt���O
			[CHR_ENEMY_00_STATE_MAX];	//
	int		aCounterState				// ��ԊǗ��J�E���^�[
			[CHR_ENEMY_00_STATE_MAX];	//
	int		nPattern;					// �e�N�X�`���̃p�^�[��No.
	int		nCounterAnim;				// �A�j���[�V�����̃J�E���^�[
	int		nCounterAct;				// �s���J�E���^�[
	bool	aAct						// �s���t���O
			[CHR_ENEMY_00_ACT_MAX];		//
	int		nCounterSound;				// �T�E���h�J�E���^�[
	int		nCounterPattern;			// �p�^�[��No.�J�E���^�[
	int		nCounterEffect;				// �G�t�F�N�g�J�E���^�[
	int		nCounterShot;				// ���˃J�E���^�[
	int		nCounterSummon;				// �����J�E���^�[
	int		nCntChain;					// �A���̃J�E���g
}Chr_enemy_00;

// �G[00] �̎�ޖ��̏��
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
	float					fHitTestWidth;						// �����蔻��̕�
	float					fHitTestHeight;						// �����蔻��̍���
	int						nLife;								// �̗�
	float					fMove;								// �ړ���
	float					fRotation;							// ��]��
	int						nHitDamage;							// �Փ˃_���[�W
	bool					bHitBreak;							// �Փˎ��ɔj�󂷂邩
	ATK_EXPLOSION_00_TYPE	explosionType;						// �����^�C�v
	int						nScore;								// �X�R�A
	int						nActNum;							// �s���̐�
	Chr_enemy_00Act			aAct								// 
							[CHR_ENEMY_00_ACT_MEMORY_MAX];		// �s���̏��
	int						nSoundNum;							// �T�E���h�̐�
	Chr_enemy_00Sound		aSound								//
							[CHR_ENEMY_00_SOUND_MEMORY_MAX];	// �T�E���h�̏��
	int						nSoundLoopTime;						// �T�E���h�̃��[�v����
	int						nPatternNum;						// �p�^�[��No.�̐�
	Chr_enemy_00Pattern		aPattern							//
							[CHR_ENEMY_00_PATTERN_MEMORY_MAX];	// �p�^�[��No.�̏��
	int						nPatternLoopTime;					// �p�^�[��No.�̃��[�v����
	int						nEffectNum;							// �G�t�F�N�g�̐�
	Chr_enemy_00Effect		aEffect								//
							[CHR_ENEMY_00_EFFECT_MEMORY_MAX];	// �G�t�F�N�g�̏��
	int						nEffectLoopTime;					// �G�t�F�N�g�̃��[�v����
	int						nShotNum;							// ���˂̐�
	Chr_enemy_00Shot		aShot								//
							[CHR_ENEMY_00_SHOT_MEMORY_MAX];		// ���˂̏��
	int						nShotLoopTime;						// ���˂̃��[�v����
	int						nSummonNum;							// �����̐�
	Chr_enemy_00Summon		aSummon								//
							[CHR_ENEMY_00_SUMMON_MEMORY_MAX];	// �����̏��
	int						nSummonLoopTime;					// �����̃��[�v����

	// ���o�֘A
	bool		bBonusSound;	// �{�[�i�X�T�E���h�t���O
	SOUND_LABEL	damageSound;	// �_���[�W���̃T�E���h�̎��
}Chr_enemy_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �G[00] �̏����擾
Chr_enemy_00 *GetChr_enemy_00(void);

// �G[00] �̎�ޖ��̏����擾
Chr_enemy_00Type *GetChr_enemy_00Type(void);

// �G[00] �̓ǂݍ��ݏ���
void LoadChr_enemy_00(void);

// �G[00] �̎�ނ𕶎��񂩂�ǂݍ���
void StringLoadChr_enemy_00Type(char aString[TXT_MAX], int *pType);

// �G[00] �̏���������
void InitChr_enemy_00(void);

// �G[00] �̏I������
void UninitChr_enemy_00(void);

// �G[00] �̍X�V����
void UpdateChr_enemy_00(void);

// �G[00] �̕`�揈��
void DrawChr_enemy_00(void);

// �G[00] �̐ݒ菈��
void SetChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bAreaBoss, bool bBomb, int nParentNum, int nChildNum);

// �G[00] �̃q�b�g����
void HitChr_enemy_00(Chr_enemy_00 *pChr, int nDamage);

// ����[00] �p�̓G[00] �̃q�b�g����
bool HitChr_enemy_00ForAtk_explosion_00(Chr_enemy_00 *pChr, int nDamage, int nCntChain);

// �G[00] �̔j�󏈗�
void DestructionChr_enemy_00(Chr_enemy_00 *pChr);

// �G[00] ��S�ď�������
void RemoveAllChr_enemy_00(void);

#endif
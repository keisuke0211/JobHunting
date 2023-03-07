//========================================
// 
// �|���S��(2D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon2D.h ***
//========================================
#ifndef _POLYGON2D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _POLYGON2D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// ���i(2D)�̍ő吔
// ���[�V����(2D)�̍ő吔
// ���[�V����(2D)�̃p�^�[��No.	�̃A�j���[�V�������̍ő吔
// ���[�V����(2D)�̈ʒu			�̃A�j���[�V�������̍ő吔
// ���[�V����(2D)�̊p�x			�̃A�j���[�V�������̍ő吔
// ���[�V����(2D)�̐F			�̃A�j���[�V�������̍ő吔
// ���[�V����(2D)�̌�			�̃A�j���[�V�������̍ő吔
// ���[�V����(2D)�̃T�E���h		�̃A�j���[�V�������̍ő吔
#define PARTS_2D_MAX				(24)
#define MOTION_2D_MAX				(16)
#define MOTION_2D_PATTERN_ANIM_MAX	(8)
#define MOTION_2D_POSITION_ANIM_MAX	(8)
#define MOTION_2D_ROTATION_ANIM_MAX	(8)
#define MOTION_2D_COLOR_ANIM_MAX	(8)
#define MOTION_2D_LIGHT_ANIM_MAX	(8)
#define MOTION_2D_SOUND_ANIM_MAX	(8)
//****************************************
// �񋓌^�̒�`
//****************************************
// �p�x�̎��
typedef enum
{
	ANGLE_TYPE_FREE,	// ���R
	ANGLE_TYPE_FIXED,	// �Œ�
	ANGLE_TYPE_MAX,
}ANGLE_TYPE;
//****************************************
// �\���̂̒�`
//****************************************
// �|���S�����̏��(2D)
typedef struct 
{
	char	aTexturePath	// �e�N�X�`���̑��΃p�X
			[TXT_MAX];
	float	fWidth;			// ��
	float	fHeight;		// ����
	int		nMax;			// �ő吔
}Polygon2D;
//====================
// *** ���i�֘A(2D)
//====================
// ���i(2D)�\����
typedef struct 
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 relativePos;	// ���Έʒu
	D3DXVECTOR3 rot;			// �p�x
	D3DXVECTOR3 relativeRot;	// ���Ίp�x
	D3DXVECTOR3 scale			// �g��{��
				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���֘A
	int		nPattern;	// �p�^�[��No.

	// ��Ԋ֘A
	Color	col;			// �F
	Color	lightColor;		// ���̐F
	float	fLightLength;	// ���̒���

	// ���[�V�����֘A
	bool		bPosAnim;			// �ʒu�A�j���[�V�����t���O
	int			nCounterPosAnim;	// �ʒu�A�j���[�V�����J�E���^�[
	int			nPosAnimTime;		// �ʒu�A�j���[�V�����ɂ����鎞��
	D3DXVECTOR3 posOld;				// ���̈ʒu
	D3DXVECTOR3 targetPos;			// �ڕW�ʒu

	bool		bRotAnim;			// �p�x�A�j���[�V�����t���O
	int			nCounterRotAnim;	// �p�x�A�j���[�V�����J�E���^�[
	int			nRotAnimTime;		// �p�x�A�j���[�V�����ɂ����鎞��
	D3DXVECTOR3 rotOld;				// ���̊p�x
	D3DXVECTOR3 targetRot;			// �ڕW�p�x
	
	bool		bColorAnim;			// �F�A�j���[�V�����t���O
	int			nCounterColorAnim;	// �F�A�j���[�V�����J�E���^�[
	int			nColorAnimTime;		// �F�A�j���[�V�����ɂ����鎞��
	Color		colOld;				// ���̐F
	Color		targetCol;			// �ڕW�F
	
	bool		bLightAnim;			// ���A�j���[�V�����t���O
	int			nCounterLightAnim;	// ���A�j���[�V�����J�E���^�[
	int			nLightAnimTime;		// ���A�j���[�V�����ɂ����鎞��
	Color		lightColorOld;		// ���̌��̐F
	Color		targetLightColor;	// �ڕW���̐F
	float		fLightLengthOld;	// ���̌��̒���
	float		fTargetLightLength;	// �ڕW�̌��̒���
}Parts2D;
// ���i(2D)�Ǘ��\����
typedef struct 
{
	int		nMotion;		// ���[�V�����ԍ�
	int		nCounterMotion;	// ���[�V�����J�E���^�[
	Parts2D	aParts			// ���i���
			[PARTS_2D_MAX];
}Parts2DInfo;
// ���i(2D)�̎�ޖ��̏��\����
typedef struct 
{
	// �ǂݍ��ݏ��
	char		aTexturePath		// �e�N�X�`���̑��΃p�X
				[TXT_MAX];
	float		fWidth;				// ��
	float		fHeight;			// ����
	int			nPtnMaxX;			// �p�^�[�����X
	int			nPtnMaxY;			// �p�^�[�����Y
	int			nParent;			// �e�ԍ�
	D3DXVECTOR3	fixedRelativePos;	// �Œ�̑��Έʒu
	D3DXVECTOR3 fixedRelativeRot;	// �Œ�̑��Ίp�x
	D3DXVECTOR3	rotRelativePos;		// ��]���̑��Έʒu
	ANGLE_TYPE	angleType;			// �p�x�̎��
	bool		bIlluminate;		// ���̉e�����󂯂�t���O
	Color		initColor;			// �����J���[
	int			nInitPtn;			// �����p�^�[��No.
}Parts2DType;

// ���i�ݒ���\����
typedef struct 
{
	int			nPartsNum;	// ���i��
	Parts2DType	aPartsType	// ���i�̎�ޖ��̏��
				[PARTS_2D_MAX];
}Parts2DSet;
//====================
// *** ���[�V�����֘A(3D)
//====================
// �p�^�[��No.�̃A�j���[�V�������\����
typedef struct 
{
	int	nPattern;	// �p�^�[��No.
	int	nTime;		// �؂�ւ�����
}PatternAnim;
// �ʒu�̃A�j���[�V�������\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	int			nMoveTime;	// ���B�ɂ����鎞��
	int			nTime;		// �؂�ւ�����
}PositionAnim;
// �p�x�̃A�j���[�V�������\����
typedef struct
{
	D3DXVECTOR3 rot;		// �p�x
	int			nMoveTime;	// ���B�ɂ����鎞��
	int			nTime;		// �؂�ւ�����
}RotationAnim;
// �F�̃A�j���[�V�������\����
typedef struct
{
	Color	col;		// �F
	int		nMoveTime;	// ���B�ɂ����鎞��
	int		nTime;		// �؂�ւ�����
}ColorAnim;
// ���̃A�j���[�V�������\����
typedef struct
{
	Color	col;		// �F
	float	fLength;	// ����
	int		nMoveTime;	// ���B�ɂ����鎞��
	int		nTime;		// �؂�ւ�����
}LightAnim;
// �T�E���h�̃A�j���[�V�������\����
typedef struct
{
	int	nSound;	// �T�E���h�ԍ�
	int	nTime;	// �؂�ւ�����
}SoundAnim;
// ���i���̃��[�V�������\����
typedef struct 
{
	int				nPtnAnimNum;	// �p�^�[��No.�̃A�j���[�V������
	PatternAnim		aPtnAnim		// �p�^�[��No.�̃A�j���[�V�������
					[MOTION_2D_PATTERN_ANIM_MAX];
	int				nPosAnimNum;	// �ʒu(����)�̃A�j���[�V������
	PositionAnim	aPosAnim		// �ʒu(����)�̃A�j���[�V�������
					[MOTION_2D_POSITION_ANIM_MAX];
	int				nRotAnimNum;	// �p�x(����)�̃A�j���[�V������
	RotationAnim	aRotAnim		// �p�x(����)�̃A�j���[�V�������
					[MOTION_2D_ROTATION_ANIM_MAX];
	int				nColorAnimNum;	// �F�̃A�j���[�V������
	ColorAnim		aColorAnim		// �F�̃A�j���[�V�������
					[MOTION_2D_COLOR_ANIM_MAX];
	int				nLightAnimNum;	// ���̃A�j���[�V������
	LightAnim		aLightAnim		// ���̃A�j���[�V�������
					[MOTION_2D_LIGHT_ANIM_MAX];
}PartsMotion;
// ���[�V�������\����
typedef struct
{
	int			nLoopTime;			// ���[�v����
	bool		bLoop;				// ���[�v���邩�t���O
	PartsMotion	aPartsMotion		// ���i���̃��[�V�������
				[PARTS_2D_MAX];		
	bool		bFlipHorizontal;	// ���E���]�t���O
	int			nSoundAnimNum;		// �T�E���h�̃A�j���[�V������
	SoundAnim	aSoundAnim			// �T�E���h�̃A�j���[�V�������
				[MOTION_2D_SOUND_ANIM_MAX];
}Motion;
// ���[�V�����ݒ���\����
typedef struct
{
	int		nMotionNum;	// ���[�V������
	Motion	aMotion		// ���[�V�����̏��
			[MOTION_2D_MAX];
}MotionSet;
//****************************************
// �v���g�^�C�v�錾
//****************************************
// �|���S��(2D)�̕K�v����Ԃ�
// ���݂̃J�E���g�̃|���S��(2D)�̔ԍ���Ԃ�
// �|���S��(2D)�̃e�N�X�`���̓ǂݍ���
int GetPolygon2DNum(const Polygon2D *pPoly2D, int nPolyMax);
int GetPolygon2DIndex(const Polygon2D *pPoly2D, int nPolyMax, int nCount);
void LoadPolygon2DTexture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 *pTextureUi_map_00, const Polygon2D *pPoly2D, int nPolyMax);

// ���i�ݒ���̓ǂݍ��ݏ���
// ���i�Ǘ��̏���������
// ���i�̒��_���W�̐ݒ菈��
// ���i�̒��_�J���[�̐ݒ菈��
void LoadParts2DSet(FILE *pFile, Parts2DSet *pPartsSet);
void InitParts2DInfo(Parts2DInfo *pPartsInfo, Parts2DSet partsSet);
void SetParts2DVertexPos(VERTEX_2D *pVtxTemp, VERTEX_2D *pVtx, int nPartsNum, Parts2D *pParts, Parts2DInfo *pPartsInfo, Parts2DType *pPartsType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, bool bFlipHorizontal);
void SetParts2DVertexColor(VERTEX_2D *pVtx, Parts2D parts, bool bIlluminate);

// ���[�V�����ݒ���̓ǂݍ��ݏ���
// ���[�V�����̍X�V����
// ���[�V�����̐ݒ菈��
void LoadMotionSet(FILE *pFile, MotionSet *pMotionSet);
bool UpdateMotion(int nMotion, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet);
void SetMotion(int nMotion, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet);

// ���_���W�̐ݒ菈��(2D)
// rhw�̐ݒ菈��(2D)
// ���_�F�̐ݒ菈��(2D)
// �e�N�X�`�����W�̐ݒ菈��(2D)
// �e�N�X�`�����W�̐ݒ菈��(2D)(�؂���ver)
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType);
void SetRHW2D(VERTEX_2D *pVtx);
void SetVertexColor2D(VERTEX_2D *pVtx, Color col);
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos);

// �`��ʒu����ɍ��킹��
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos);

#endif
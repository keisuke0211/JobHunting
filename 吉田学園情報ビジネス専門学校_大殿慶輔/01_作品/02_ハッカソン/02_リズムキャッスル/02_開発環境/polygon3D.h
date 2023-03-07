//========================================
// 
// �|���S��(3D)�֘A�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon3D.h ***
//========================================
#ifndef _POLYGON3D_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _POLYGON3D_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
#define MATERIAL_3D_MAX				(10)	// �}�e���A��(3D)�̏��
#define PARTS_3D_MAX				(8)		// ���i(3D)�̍ő吔
#define MOTION_3D_MAX				(16)	// ���[�V����(3D)�̍ő吔
#define MOTION_3D_POSITION_ANIM_MAX	(8)		// ���[�V����(3D)�̈ʒu			�̃A�j���[�V�������̍ő吔
#define MOTION_3D_ROTATION_ANIM_MAX	(8)		// ���[�V����(3D)�̊p�x			�̃A�j���[�V�������̍ő吔
#define MOTION_3D_SOUND_ANIM_MAX	(8)		// ���[�V����(3D)�̃T�E���h		�̃A�j���[�V�������̍ő吔

//****************************************
// �\���̂̒�`
//****************************************
//====================
// *** ���i�֘A(3D)
//====================
// ���i(3D)�\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 posResult;	// �ʒu(����)
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rotResult;	// ����(����)
	D3DXVECTOR3 rot;		// ����

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
}Parts3D;

// ���i(3D)�Ǘ��\����
typedef struct
{
	int		nMotion;		// ���[�V�����ԍ�
	int		nCounterMotion;	// ���[�V�����J�E���^�[
	Parts3D	aParts			// ���i���
			[PARTS_3D_MAX];
}Parts3DInfo;

// ���i(3D)�̎�ޖ��̏��\����
typedef struct
{
	// �ǂݍ��ݏ��
	char		aModelPath			// ���f���̑��΃p�X
				[TXT_MAX];
	int			nParent;			// �e�ԍ�
	D3DXVECTOR3	fixedRelativePos;	// �Œ�̑��Έʒu
	D3DXVECTOR3 fixedRelativeRot;	// �Œ�̑��Ίp�x
	D3DXVECTOR3	rotRelativePos;		// ��]���̑��Έʒu
}Parts3DType;

// ���i�ݒ���\����
typedef struct
{
	int			nPartsNum;	// ���i��
	Parts3DType	aPartsType	// ���i�̎�ޖ��̏��
				[PARTS_3D_MAX];
}Parts3DSet;

//====================
// *** ���[�V�����֘A(3D)
//====================
// �ʒu�̃A�j���[�V����(3D)���\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	int			nMoveTime;	// ���B�ɂ����鎞��
	int			nTime;		// �؂�ւ�����
}PositionAnim3D;

// �p�x�̃A�j���[�V����(3D)���\����
typedef struct
{
	D3DXVECTOR3 rot;		// �p�x
	int			nMoveTime;	// ���B�ɂ����鎞��
	int			nTime;		// �؂�ւ�����
}RotationAnim3D;

// �T�E���h�̃A�j���[�V����(3D)���\����
typedef struct
{
	int	nSound;	// �T�E���h�ԍ�
	int	nTime;	// �؂�ւ�����
}SoundAnim3D;

// ���i���̃��[�V����(3D)���\����
typedef struct
{
	int				nPosAnimNum;	// �ʒu(����)�̃A�j���[�V������
	PositionAnim3D	aPosAnim		// �ʒu(����)�̃A�j���[�V�������
		[MOTION_3D_POSITION_ANIM_MAX];
	int				nRotAnimNum;	// �p�x(����)�̃A�j���[�V������
	RotationAnim3D	aRotAnim		// �p�x(����)�̃A�j���[�V�������
		[MOTION_3D_ROTATION_ANIM_MAX];
}PartsMotion3D;

// ���[�V����(3D)���\����
typedef struct
{
	int				nLoopTime;		// ���[�v����
	bool			bLoop;			// ���[�v���邩�t���O
	PartsMotion3D	aPartsMotion	// ���i���̃��[�V�������
					[PARTS_3D_MAX];
	int				nSoundAnimNum;	// �T�E���h�̃A�j���[�V������
	SoundAnim3D		aSoundAnim		// �T�E���h�̃A�j���[�V�������
					[MOTION_3D_SOUND_ANIM_MAX];
}Motion3D;

// ���[�V����(3D)�ݒ���\����
typedef struct
{
	int			nMotionNum;	// ���[�V������
	Motion3D	aMotion		// ���[�V�����̏��
				[MOTION_3D_MAX];
}Motion3DSet;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ���i�ݒ���(3D)�̓ǂݍ��ݏ���
// ���i(3D)�̏���������
// ���i(3D)�̃g�����X�t�H�[���̏���������
// ���i(3D)�̃g�����X�t�H�[���̎擾����
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet);
void InitParts3DInfo(Parts3DInfo *pPartsInfo);
void InitParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);
void GetParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);

// ���[�V����(3D)�ݒ���̓ǂݍ��ݏ���
// ���[�V����(3D)�̍X�V����
// ���[�V����(3D)�̐ݒ菈��
void LoadMotion3DSet(FILE *pFile, Motion3DSet *pMotionSet);
bool UpdateMotion3D(int nMotion, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);
void SetMotion3D(Parts3DInfo *pPartsInfo, int nMotion);

// ���_���W�̐ݒ菈��(3D)
// �@���x�N�g���̐ݒ菈��(3D)
// ���_�F�̐ݒ菈��(3D)
// �e�N�X�`�����W�̐ݒ菈��(3D)
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight);
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
void SetVertexColor3D(VERTEX_3D *pVtx, Color col);
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);

// �}�e���A���̐ݒ�
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col);

#endif
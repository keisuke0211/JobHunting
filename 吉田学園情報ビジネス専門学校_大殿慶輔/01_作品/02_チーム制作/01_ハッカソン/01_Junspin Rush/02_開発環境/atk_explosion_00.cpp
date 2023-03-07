//========================================
// 
// ����[00] �̏���
// Author:���� ����
// 
//========================================
// *** atk_explosion_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "sound.h"
#include "chr_enemy_00.h"
#include "eff_particle_00.h"
#include "atk_explosion_00.h"
#include "input.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
#define ATK_EXPLOSION_00_MAX	(256)	// ����[00] �̍ő吔

//****************************************
// �񋓌^�̒�`
//****************************************

// �W�I�^�C�v
typedef enum
{
	TARGET_TYPE_ENEMY_ONRY
}TARGET_TYPE;

//****************************************
// �\���̂̒�`
//****************************************

// ����[00] �\���̂̒�`
typedef struct 
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 rot;			// ����
	float fWidth;				// ��
	float fHeight;				// ����
	float fHitTestRadius;		// �����蔻��̔��a
	
	// ���ޏ��
	ATK_EXPLOSION_00_TYPE type;	// ���
	bool bUse;					// �g�p����Ă��邩�t���O

	// ��Ԓl
	int nDamage;				// �_���[�W
	int nPattern;				// �p�^�[��No.
	int nCounterAnim;			// �A�j���[�V�����̃J�E���^�[
	int nCntChain;				// �A���̃J�E���g
}Atk_explosion_00;

// ����[00] �̎�ޖ��̏��
typedef struct
{
	char aTexturPath[TXT_MAX];		// �e�N�X�`���̑��΃p�X
	int nPtnMaxX;					// �p�^�[���̍ő吔(��)
	int nPtnMaxY;					// �p�^�[���̍ő吔(�c)
	int nAnimChangeTime;			// �A�j���[�V�����̐؂�ւ�����
	float fWidth;					// ��
	float fHeight;					// ����
	float fHitRadius;				// �����蔻��̔��a
	int nDamage;					// �_���[�W
	SOUND_LABEL soundSet;			// �ݒ莞�̃T�E���h
	EFF_PARTICLE_00_TYPE ptclType;	// �p�[�e�B�N��[00] �̎��
	int nPtclNum;					// �p�[�e�B�N���̔�����
	float fVibration;				// �U���̋���
	int nStopTime;					// �q�b�g�X�g�b�v�̎���
	TARGET_TYPE targetType;			// �ڕW�^�C�v
	bool bChain;					// �A���t���O
}Atk_explosion_00Type;

//****************************************
// �v���g�^�C�v�錾
//****************************************
void CollisionChr_fighter_00	(Atk_explosion_00 *pAtk);	// �@��[00] �Ƃ̓����蔻�菈��
void CollisionChr_enemy_00		(Atk_explosion_00 *pAtk);	// �G[00] �Ƃ̓����蔻��
void SetState					(Atk_explosion_00 *pAtk);	// ����[00] �̃X�e�[�^�X����ނɉ����Đݒ�

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9 g_aTextureAtk_explosion_00[ATK_EXPLOSION_00_TYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAtk_explosion_00 = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
Atk_explosion_00 g_aAtk_explosion_00[ATK_EXPLOSION_00_MAX];						// ����[00] �̏��

// ����[00] �̎�ނ̏��
Atk_explosion_00Type g_aAtk_explosion_00Type[ATK_EXPLOSION_00_TYPE_MAX] =
{//   �e�N�X�`���̑��΃p�X                          , �p�^�[���̍ő�X, Y, �A�j���[�V�����ɂ����鎞��, ��         , ����       , �����蔻��̔��a, �_���[�W, SE                           , �p�[�e�B�N���̎��           , �p�[�e�B�N���̔����� , �U���̋���, �X�g�b�v����, �W�I�^�C�v
	{ "data\\TEXTURE\\eff_explosion_002.png"		, 4              , 1, 4                         , PIXEL * 16 , PIXEL * 16 , PIXEL * 8       , 1       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 10                   , 4.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_001.png"		, 4              , 2, 4                         , PIXEL * 32 , PIXEL * 32 , PIXEL * 16      , 2       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 30                   , 4.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_000_medium.png" , 4              , 2, 4                         , PIXEL * 32 , PIXEL * 32 , PIXEL * 16      , 2       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_01, 40                   , 16.0f     , 40           , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_000_lange.png"	, 4              , 2, 4                         , PIXEL * 64 , PIXEL * 64 , PIXEL * 32      , 3       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_01, 50                   , 64.0f     , 80           , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_shot_break_001.png"	    , 4              , 1, 4                         , PIXEL * 16 , PIXEL * 16 , PIXEL * 0       , 0       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_N1    , 0                    , 0.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , false},
	{ "data\\TEXTURE\\eff_explosion_002.png"	    , 4              , 1, 4                         , PIXEL * 16 , PIXEL * 16 , PIXEL * 0       , 100     , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_N1    , 0                    , 0.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_001.png"		, 4              , 2, 4                         , PIXEL * 32 , PIXEL * 32 , PIXEL * 0       , 0       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 30                   , 0.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_002.png"	    , 4              , 1, 4                         , PIXEL * 16 , PIXEL * 16 , PIXEL * 8       , 100     , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 10                   , 2.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , false},
	{ "data\\TEXTURE\\eff_explosion_001.png"		, 4              , 2, 4                         , PIXEL * 32 , PIXEL * 32 , PIXEL * 16      , 2       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 30                   , 4.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , false},
};

//========================================
// InitAtk_explosion_00�֐� - ����[00] �̏��������� -
//========================================
void InitAtk_explosion_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntAtk_explosion_00_type = 0; nCntAtk_explosion_00_type < ATK_EXPLOSION_00_TYPE_MAX; nCntAtk_explosion_00_type++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aAtk_explosion_00Type[nCntAtk_explosion_00_type].aTexturPath, &g_aTextureAtk_explosion_00[nCntAtk_explosion_00_type]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ATK_EXPLOSION_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAtk_explosion_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_explosion_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_MAX; nCntAtk_explosion_00++, pVtx += 4)
	{
		Atk_explosion_00 *pAtk = &g_aAtk_explosion_00[nCntAtk_explosion_00];	// ����[00] �̏��̃|�C���^

		pAtk->nCounterAnim	= 0;								// �J�E���^�[X������������
		pAtk->nPattern		= 0;								// �p�^�[��No.������������
		pAtk->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu������������
		pAtk->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����������������
		pAtk->bUse			= false;							// �g�p����Ă��邩�t���O������������
		pAtk->type			= ATK_EXPLOSION_00_TYPE_00;			// ��ނ�����������			

		// �X�e�[�^�X����ނɉ����Đݒ�
		SetState(pAtk);

		{
			D3DXVECTOR3 pos = pAtk->pos;												// �ʒu
			D3DXVECTOR3 rot = pAtk->rot;												// ����
			float fWidth	= pAtk->fWidth;												// ��
			float fHeight	= pAtk->fHeight;											// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		{
			int nPtn		= pAtk->nPattern;								// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX	= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxX;	// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY	= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxY;	// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffAtk_explosion_00->Unlock();
}

//========================================
// UninitAtk_explosion_00�֐� - ����[00] �̏I������ -
//========================================
void UninitAtk_explosion_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_TYPE_MAX; nCntAtk_explosion_00++)
	{
		if (g_aTextureAtk_explosion_00[nCntAtk_explosion_00] != NULL)
		{
			g_aTextureAtk_explosion_00[nCntAtk_explosion_00]->Release();
			g_aTextureAtk_explosion_00[nCntAtk_explosion_00] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffAtk_explosion_00 != NULL) 
	{
		g_pVtxBuffAtk_explosion_00->Release();
		g_pVtxBuffAtk_explosion_00 = NULL;
	}
}

//========================================
// UpdateAtk_explosion_00�֐� - ����[00] �̍X�V���� -
//========================================
void UpdateAtk_explosion_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_explosion_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_MAX; nCntAtk_explosion_00++, pVtx += 4)
	{
		if (!g_aAtk_explosion_00[nCntAtk_explosion_00].bUse)
		{// ����[00] ���g�p����Ă����ԂłȂ����A
			// �J��Ԃ����X�L�b�v
			continue;
		}

		Atk_explosion_00 *pAtk = &g_aAtk_explosion_00[nCntAtk_explosion_00];	// ����[00] �̏��̃|�C���^

		D3DXVECTOR3 pos		= pAtk->pos;											// �ʒu
		D3DXVECTOR3 rot		= pAtk->rot;											// ����
		int nPtn			= pAtk->nPattern;										// �e�N�X�`���̃p�^�[��No.
		float fWidth		= pAtk->fWidth;											// ��
		float fHeight		= pAtk->fHeight;										// ����
		int nAnimChangeTime = g_aAtk_explosion_00Type[pAtk->type].nAnimChangeTime;	// �A�j���[�V�����̐؂�ւ�����
		int nPtnMaxX		= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxX;			// �e�N�X�`���̃p�^�[����(��)
		int nPtnMaxY		= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxY;			// �e�N�X�`���̃p�^�[����(�c)

		// �A�j���[�V�����̃J�E���^�[�����Z
		pAtk->nCounterAnim++;

		if (pAtk->nCounterAnim == nAnimChangeTime)
		{// �A�j���[�V�����̃J�E���^�[���؂�ւ����ԂɒB�������A
			// �A�j���[�V�����̃J�E���^�[��������
			pAtk->nCounterAnim = 0;

			// �p�^�[�����X�V
			pAtk->nPattern++;
		}

		if (pAtk->nPattern == nPtnMaxX * nPtnMaxY)
		{// ����[00] �̃p�^�[��No.���p�^�[���̏�������������A
			pAtk->bUse = false; // �g�p���Ă��Ȃ���Ԃɂ���
		}

		{
			D3DXVECTOR3 drawPos = pos;													// �`��ʒu
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// �`��ʒu��ݒ�
			MatchCameraPosition(&drawPos);

			// ���_���W��ݒ�
			pVtx[0].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		{
			float fAlpha = 1.0f - ((float)((pAtk->nPattern * nAnimChangeTime) + pAtk->nCounterAnim) 
				/ (float)(nPtnMaxX * nPtnMaxY * nAnimChangeTime));	// �����x

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * fAlpha));
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * fAlpha));
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * fAlpha));
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * fAlpha));
		}

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffAtk_explosion_00->Unlock();
}

//========================================
// DrawAtk_explosion_00�֐� - ����[00] �̕`�揈�� -
//========================================
void DrawAtk_explosion_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffAtk_explosion_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_MAX; nCntAtk_explosion_00++)
	{
		if (g_aAtk_explosion_00[nCntAtk_explosion_00].bUse)
		{// �g�p����Ă��鎞�A
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureAtk_explosion_00[g_aAtk_explosion_00[nCntAtk_explosion_00].type]);

			// �@��[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAtk_explosion_00 * 4, 2);
		}
	}
}

//========================================
// Atk_explosion_00KeyInput�֐� - ����[00] �̐ݒ菈�� -
//========================================
void SetAtk_explosion_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ATK_EXPLOSION_00_TYPE type, int nCntChain)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffAtk_explosion_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_MAX; nCntAtk_explosion_00++, pVtx += 4)
	{
		if (g_aAtk_explosion_00[nCntAtk_explosion_00].bUse)
		{// ����[00] ���g�p����Ă��鎞�A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// ���ԑ҂�
		Sleep(g_aAtk_explosion_00Type[type].nStopTime);

		Atk_explosion_00 *pAtk = &g_aAtk_explosion_00[nCntAtk_explosion_00];	// ����[00] �̏��̃|�C���^

		// �U����ݒ�
		SetCameraVibration(g_aAtk_explosion_00Type[type].fVibration);

		// ����[00] �̎�ނ�ݒ肷��
		pAtk->type = type;

		// �A���̃J�E���g����
		pAtk->nCntChain = nCntChain - (g_aAtk_explosion_00Type[type].bChain ^ 1);

		// �X�e�[�^�X����ނɉ����Đݒ�
		SetState(pAtk);

		// ���ʉ����Đ�
		PlaySound(g_aAtk_explosion_00Type[type].soundSet);

		// �ʒu/�p�x����
		pAtk->pos = pos;
		pAtk->rot = rot;

		{
			D3DXVECTOR3 drawPos = pAtk->pos;												// �`��ʒu
			D3DXVECTOR3 rot		= pAtk->rot;												// ����
			float fWidth		= pAtk->fWidth;												// ��
			float fHeight		= pAtk->fHeight;											// ����
			float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle		= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// �`��ʒu��ݒ�
			MatchCameraPosition(&drawPos);

			// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		pAtk->nPattern		= 0;	// �p�^�[��No.������������
		pAtk->nCounterAnim	= 0;	// �A�j���[�V�����̃J�E���^�[������������

		{
			int nPtn			= pAtk->nPattern;								// �e�N�X�`���̃p�^�[��No.
			int nPtnMaxX		= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxX;	// �e�N�X�`���̃p�^�[����(��)
			int nPtnMaxY		= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxY;	// �e�N�X�`���̃p�^�[����(�c)

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}

		// �ڕW�^�C�v�ɉ����������蔻�菈��
		switch (g_aAtk_explosion_00Type[pAtk->type].targetType)
		{
			//========== *** �G�̂� ***
		case TARGET_TYPE_ENEMY_ONRY:

			// �G[00]�Ƃ̓����蔻�菈��
			CollisionChr_enemy_00(pAtk);

			break;
		}

		// �g�p���Ă����Ԃɂ���
		pAtk->bUse = true;

		// �p�[�e�B�N���̐ݒ菈��
		for (int nCntPtcl = 0; nCntPtcl < g_aAtk_explosion_00Type[pAtk->type].nPtclNum; nCntPtcl++) 
		{
			D3DXVECTOR3 setRot;	// �ݒ肷�������ݒ�
			setRot.z = -D3DX_PI + fRand(D3DX_PI * 2);

			// �p�[�e�B�N��[00] ��ݒ�
			SetEff_particle_00(pos, setRot, g_aAtk_explosion_00Type[pAtk->type].ptclType);
		}

		break;
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffAtk_explosion_00->Unlock();
}

//========================================
// SetState�֐� - ����[00] �̃X�e�[�^�X����ނɉ����Đݒ� -
//========================================
void SetState(Atk_explosion_00 *pAtk_explosion_00)
{
	pAtk_explosion_00->fWidth				= g_aAtk_explosion_00Type[pAtk_explosion_00->type].fWidth;				// ��
	pAtk_explosion_00->fHeight				= g_aAtk_explosion_00Type[pAtk_explosion_00->type].fHeight;				// ����
	pAtk_explosion_00->fHitTestRadius		= g_aAtk_explosion_00Type[pAtk_explosion_00->type].fHitRadius;			// �����蔻��̔��a
	pAtk_explosion_00->nDamage				= g_aAtk_explosion_00Type[pAtk_explosion_00->type].nDamage;				// �_���[�W
}

//========================================
// CollisionChr_fighter_00�֐� - �@��[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionChr_fighter_00(Atk_explosion_00 *pAtk)
{
	
}

//========================================
// CollisionChr_enemy_00�֐� - �G[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionChr_enemy_00(Atk_explosion_00 *pAtk)
{
	Chr_enemy_00 *pChr = GetChr_enemy_00();	// �G[00] �̏��̃|�C���^

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if ((!pChr->bUse)
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_POP])
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
			|| (pChr->aState[CHR_ENEMY_00_STATE_SWELL])
			|| (pChr->aAct[CHR_ENEMY_00_ACT_GHOST]))
		{// �g�p����Ă��Ȃ���ԁA�������͏o���������������c��݂����蔲���̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		
	}
}
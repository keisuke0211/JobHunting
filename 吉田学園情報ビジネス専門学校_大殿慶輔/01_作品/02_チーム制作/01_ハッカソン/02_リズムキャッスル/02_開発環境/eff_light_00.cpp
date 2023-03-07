//========================================
// 
// ���C�g[00]�̏���
// Author:���� ����
// 
//========================================
// *** eff_light_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "eff_light_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// �}�N����`
//****************************************
#define EFF_LIGHT_00_MAX	(1024)	// ���C�g[00] �̍ő吔

//****************************************
// �\���̂̒�`
//****************************************

// ���C�g[00]�\���̂̒�`
typedef struct
{
												//========== *** �ʒu�֘A ***
	D3DXVECTOR3 pos;							// �ʒu
	D3DXVECTOR3 move;							// �ړ���
	float fMove;								// �ړ���
	D3DXVECTOR3 rot;							// ����
	float fWidth;								// ��
	float fHeight;								// ����
												//========== *** ���ޏ�� ***
	int nLife;									// ����
	int nLifeMax;								// �����̍ő�l
	EFF_LIGHT_00_TYPE type;						// ���
	bool bUse;									// �g�p����Ă��邩�t���O
												//========== *** ��Ԓl ***
	Color col;									// �F
	bool aState[EFF_LIGHT_00_STATE_MAX];		// ��ԃt���O
	int aCounterState[EFF_LIGHT_00_STATE_MAX];	// ��ԊǗ��J�E���^�[
}Eff_light_00;

// ���C�g[00] �̎�ޖ��̏��
typedef struct
{
	char aTexturPath[TXT_MAX];	// �e�N�X�`���̑��΃p�X
}Eff_light_00Type;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9 g_aTextureEff_light_00[EFF_LIGHT_00_TYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_light_00 = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Eff_light_00 g_aEff_light_00[EFF_LIGHT_00_MAX];							// ���C�g[00] �̏��

// ���C�g[00] �̎�ނ̏��
Eff_light_00Type g_aEff_light_00Type[EFF_LIGHT_00_TYPE_MAX] =
{//   �e�N�X�`���̑��΃p�X
	{ "data\\TEXTURE\\Effect\\effect000.jpg"},
};

//========================================
// InitEff_light_00�֐� - ���C�g[00]�̏��������� -
//========================================
void InitEff_light_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntEff_light_00_type = 0; nCntEff_light_00_type < EFF_LIGHT_00_TYPE_MAX; nCntEff_light_00_type++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aEff_light_00Type[nCntEff_light_00_type].aTexturPath, &g_aTextureEff_light_00[nCntEff_light_00_type]);
	}

	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++) 
	{
		g_aEff_light_00[nCntEff_light_00].bUse	= false;				// �g�p����Ă��邩�t���O������������
		g_aEff_light_00[nCntEff_light_00].type	= EFF_LIGHT_00_TYPE_00;	// ��ނ�������
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * EFF_LIGHT_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_light_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++, pVtx += 4)
	{
		{
			D3DXVECTOR3 pos = g_aEff_light_00[nCntEff_light_00].pos;					// �ʒu
			D3DXVECTOR3 rot = g_aEff_light_00[nCntEff_light_00].rot;					// ����
			float fWidth	= g_aEff_light_00[nCntEff_light_00].fWidth;					// ��
			float fHeight	= g_aEff_light_00[nCntEff_light_00].fHeight;				// ����
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
			float fAngle	= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

			// ���_���W��ݒ�
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
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

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	
	// ���_���W���A�����b�N����
	g_pVtxBuffEff_light_00->Unlock();
}

//========================================
// UninitEff_light_00�֐� - ���C�g[00]�̏I������ -
//========================================
void UninitEff_light_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_TYPE_MAX; nCntEff_light_00++)
	{
		if (g_aTextureEff_light_00[nCntEff_light_00] != NULL)
		{
			g_aTextureEff_light_00[nCntEff_light_00]->Release();
			g_aTextureEff_light_00[nCntEff_light_00] = NULL;
		}
	}
	
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEff_light_00 != NULL) 
	{
		g_pVtxBuffEff_light_00->Release();
		g_pVtxBuffEff_light_00 = NULL;
	}
}

//========================================
// UpdateEff_light_00�֐� - ���C�g[00]�̍X�V���� -
//========================================
void UpdateEff_light_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	POINT vertexPos[4];			// �l�p�`�̒��_�̍��W
	
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++, pVtx += 4)
	{
		if (g_aEff_light_00[nCntEff_light_00].bUse)
		{// ���C�g[00]���g�p����Ă��鎞

			// ���������Z����
			g_aEff_light_00[nCntEff_light_00].nLife--;

			if (g_aEff_light_00[nCntEff_light_00].nLife <= 0) 
			{// �������s�������A
				// �g�p���ĂȂ���Ԃɂ���
				g_aEff_light_00[nCntEff_light_00].bUse = false;
			}

			D3DXVECTOR3 *move	= &g_aEff_light_00[nCntEff_light_00].move;		// �ړ���
			D3DXVECTOR3 rot		= g_aEff_light_00[nCntEff_light_00].rot;		// ����
			float fMove			= g_aEff_light_00[nCntEff_light_00].fMove;		// �ړ���
			float fWidth		= g_aEff_light_00[nCntEff_light_00].fWidth;		// ��
			float fHeight		= g_aEff_light_00[nCntEff_light_00].fHeight;	// ����

			// ��ԏ���
			for (int nCntState = 0; nCntState < EFF_LIGHT_00_STATE_MAX; nCntState++)
			{
				if (g_aEff_light_00[nCntEff_light_00].aState[nCntState])
				{// ���̏�Ԃ��^�̎�

					switch (nCntState)
					{
					
					}
				}
			}

			// �ʒu/�傫�����X�V
			g_aEff_light_00[nCntEff_light_00].pos.x	+= move->x;
			g_aEff_light_00[nCntEff_light_00].pos.y	+= move->y;

			// �ړ��ʂ��X�V(����)
			move->x += (0.0f - move->x) * 0.1;
			move->y += (0.0f - move->y) * 0.1;

			{
				D3DXVECTOR3 drawPos = g_aEff_light_00[nCntEff_light_00].pos;					// �`��ʒu
				float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
				float fAngle		= atan2f(fWidth, fHeight);									// �Ίp���̊p�x

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
				int nR = g_aEff_light_00[nCntEff_light_00].col.r;	// �Ԃ̋���
				int nG = g_aEff_light_00[nCntEff_light_00].col.g;	// �΂̋���
				int nB = g_aEff_light_00[nCntEff_light_00].col.b;	// �̋���
				int nA = g_aEff_light_00[nCntEff_light_00].col.a;	// �����x

				// �����x�������ɉ����Đݒ�
				nA *= ((float)g_aEff_light_00[nCntEff_light_00].nLife / (float)g_aEff_light_00[nCntEff_light_00].nLifeMax);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(nR, nG, nB, nA);
				pVtx[1].col = D3DCOLOR_RGBA(nR, nG, nB, nA);
				pVtx[2].col = D3DCOLOR_RGBA(nR, nG, nB, nA);
				pVtx[3].col = D3DCOLOR_RGBA(nR, nG, nB, nA);
			}
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffEff_light_00->Unlock();
}

//========================================
// DrawEff_light_00�֐� - ���C�g[00]�̕`�揈�� -
//========================================
void DrawEff_light_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEff_light_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++)
	{
		if (g_aEff_light_00[nCntEff_light_00].bUse)
		{// ���C�g[00]���g�p����Ă��鎞

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureEff_light_00[g_aEff_light_00[nCntEff_light_00].type]);

			// ���C�g[00]�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEff_light_00 * 4, 2);
		}
	}

	// �A���t�@�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
// SetEff_light_00�֐� - ���C�g[00]�̐ݒ菈�� -
//========================================
void SetEff_light_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fMove, Color col, float fWidth, float fHeight, int nLife, EFF_LIGHT_00_TYPE type)
{
	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^
	int nCntEff_light_00;	// ���C�g[00] �̃J�E���g

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++)
	{
		if (g_aEff_light_00[nCntEff_light_00].bUse == false)
		{// ���C�g[00]���g�p����Ă��Ȃ���

			// ������ݒ�
			g_aEff_light_00[nCntEff_light_00].nLife = nLife;
			g_aEff_light_00[nCntEff_light_00].nLifeMax = nLife;

			// �ʒu/�p�x����
			g_aEff_light_00[nCntEff_light_00].pos = pos;
			g_aEff_light_00[nCntEff_light_00].rot = rot;

			// �ړ��ʂ�ݒ�
			g_aEff_light_00[nCntEff_light_00].move.x = sinf(rot.z) * fMove;
			g_aEff_light_00[nCntEff_light_00].move.y = cosf(rot.z) * fMove;

			// ��ނ���
			g_aEff_light_00[nCntEff_light_00].type = type;

			// �F����
			g_aEff_light_00[nCntEff_light_00].col = col;

			// ���ƍ�����ݒ�
			g_aEff_light_00[nCntEff_light_00].fWidth = fWidth;
			g_aEff_light_00[nCntEff_light_00].fHeight = fHeight;

			for (int nCntState = 0; nCntState < EFF_LIGHT_00_STATE_MAX; nCntState++)
			{
				// ��ԂƏ�ԃJ�E���^�[��������
				g_aEff_light_00[nCntEff_light_00].aState[nCntState] = false;
				g_aEff_light_00[nCntEff_light_00].aCounterState[nCntState] = 0;
			}

			{
				D3DXVECTOR3 pos = g_aEff_light_00[nCntEff_light_00].pos;					// �ʒu
				D3DXVECTOR3 rot = g_aEff_light_00[nCntEff_light_00].rot;					// ����
				float fWidth = g_aEff_light_00[nCntEff_light_00].fWidth;					// ��
				float fHeight = g_aEff_light_00[nCntEff_light_00].fHeight;				// ����
				float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// �Ίp���̒���
				float fAngle = atan2f(fWidth, fHeight);									// �Ίp���̊p�x

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

			// �g�p���Ă����Ԃɂ���
			g_aEff_light_00[nCntEff_light_00].bUse = true;

			break;
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffEff_light_00->Unlock();
}
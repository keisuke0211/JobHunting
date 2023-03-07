/*-----------------------------------------------------------------------------------------------------

���C�t����[life.cpp]
Author:���V����

-------------------------------------------------------------------------------------------------------*/
#include "life.h"
#include "input.h"
#include "player.h"

#define MAX_HEART			(3)					//���C�t�̕\�����鐔�@= �v���C���[�̍ő僉�C�t
#define TEX_LIFE			"data/TEXTURE/ui_life_000.PNG"	//���C�t�̉摜��
#define TEX_LIFE_WIDTH		(0.25f)				//�e�N�X�`�����W�̕�
#define HEART_WIDTH			(64.0f)				//�n�[�g�̃T�C�Y��A�n�[�g���m�̊Ԋu

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureLife = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffLife = NULL;	//���_�o�b�t�@�ւ̃|�C���^
int g_nLife = MAX_HEART;			//���C�t

//���C�t�̏���������
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_LIFE, &g_pTextureLife);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_HEART, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLife, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_HEART; nCntLife++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		{
			pVtx[0].pos = D3DXVECTOR3(20.0f + nCntLife * HEART_WIDTH, 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(84.0f + nCntLife * HEART_WIDTH, 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(20.0f + nCntLife * HEART_WIDTH, 94.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(84.0f + nCntLife * HEART_WIDTH, 94.0f, 0.0f);
		}

		//rhw�̐ݒ�
		{
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffLife->Unlock();
}

//���C�t�̏I������
void UninitLife(void)
{
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//���C�t�̍X�V����
void UpdateLife(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void* *)&pVtx, 0);

	//�̗͎擾
	g_nLife = GetPlayer()->nLife;

	for (int nCntLife = 0; nCntLife < MAX_HEART; nCntLife++, pVtx += 4)
	{
		//�J�E���^�[���̗͂�����
		if (nCntLife < g_nLife)
		{
			//�V�N�ȃn�[�g��\��
			{
				pVtx[0].tex.x = 0.0f;
				pVtx[1].tex.x = 0.5f;
				pVtx[2].tex.x = 0.0f;
				pVtx[3].tex.x = 0.5f;
			}
		}

		//�͊����Ă���n�[�g��\��
		else
		{
			//���_���W�̐ݒ�
			{
				pVtx[0].tex.x = 0.5f;
				pVtx[1].tex.x = 1.0f;
				pVtx[2].tex.x = 0.5f;
				pVtx[3].tex.x = 1.0f;
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffLife->Unlock();
}

//���C�t�̕`�揈��
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntGround = 0; nCntGround < MAX_HEART; nCntGround++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGround * 4, 2);
	}
}
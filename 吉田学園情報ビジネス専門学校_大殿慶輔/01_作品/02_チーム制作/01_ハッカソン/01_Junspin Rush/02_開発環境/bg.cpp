/*==========================================================================================================================

�Q�[���w�i�̏���[bg.cpp]
Author:���V����

==========================================================================================================================*/
#include "main.h"
#include "bg.h"
#include "input.h"

//�}�N����`
#define BG_HEIGHT			(930.0f)		//�w�i�\���̍���
#define BG_WIDTH			(2048.0f)		//�w�i�\���̕�
#define BG_TEX_MOUNTAIN		"data/TEXTURE/bg_mountain_000.PNG"	

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureBg = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBg = NULL;	//���_�o�b�t�@�ւ̃|�C���^
float g_BgTexV;										//�w�i�X�N���[���p�ϐ�

//�w�i�̏���������
void InitBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_TEX_MOUNTAIN, &g_pTextureBg);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL);

	float g_aTexV = 0.0f;		//�w�i�X�N���[���p�ϐ�

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void* *)&pVtx, 0);
	
	//�w�i(�R)�̒��_���̐ݒ�
	{
		//���_���W�̐ݒ�
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, BG_HEIGHT - 1024.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1024.0f * 5, BG_HEIGHT - 1024.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, BG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1024.0f * 5, BG_HEIGHT, 0.0f);
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
			pVtx[1].tex = D3DXVECTOR2(5.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(5.0f, 1.0f);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBg->Unlock();
}

//�w�i�̏I������
void UninitBg(void)
{
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//�w�i�̍X�V����
void UpdateBg(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void* *)&pVtx, 0);

	if (GetMode() == MODE_GAME_00)
	{// �Q�[���̎��A
		//�e�N�X�`�����W�̊J�n�ʒu�̍X�V
		g_BgTexV += 0.0002f;
	}

	//�e�N�X�`���̍��W
	pVtx[0].tex.x = g_BgTexV;
	pVtx[1].tex.x = g_BgTexV + 5.0f;
	pVtx[2].tex.x = g_BgTexV;
	pVtx[3].tex.x = g_BgTexV + 5.0f;

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffBg->Unlock();
}

//�w�i�̕`�揈��
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg);
	
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		
}
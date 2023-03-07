/*==========================================================================================================================

�n�ʏ���[ground.cpp]
Author:���V����

==========================================================================================================================*/
#include "ground.h"

//�}�N����`
#define BG_TEX_GROUND		"data/TEXTURE/obj_soil_000.PNG"		//�n�ʂ̃e�N�X�`��
#define MAX_GROUND			(21)								//�n�ʂ̐�
#define GROUND_POS_Y		(696.0f)							//�n�ʂ�z�u���鍂��
#define GROUND_LENGTH		(64.0f)								//�n�ʂ̒���
#define GROUND_WIDTH		(64.0f)								//�ד��m�̒n�ʂ̒��S�̕�

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffGround = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9			g_pTextureGround = NULL;	//�e�N�X�`���ւ̃|�C���^
float g_GroundMoveX;									//�n�ʂ����ɓ�����
D3DXVECTOR3 g_Groundpos;

//�n�ʂ̏���������
void InitGround(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_TEX_GROUND, &g_pTextureGround);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GROUND, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffGround, NULL);

	//���ɓ������ϐ���������
	g_GroundMoveX = 1.0f;

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGround->Lock(0, 0, (void* *)&pVtx, 0);

	//���_���W�̐ݒ�
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f,		SCREEN_HEIGHT - GROUND_LENGTH, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f,	SCREEN_HEIGHT - GROUND_LENGTH, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,		SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f,	SCREEN_HEIGHT, 0.0f);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f , 0.0f);
		pVtx[1].tex = D3DXVECTOR2(10.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f , 1.0f);
		pVtx[3].tex = D3DXVECTOR2(10.0f, 1.0f);
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffGround->Unlock();
}

//�n�ʂ̏I������
void UninitGround(void)
{
	if (g_pTextureGround != NULL)
	{
		g_pTextureGround->Release();
		g_pTextureGround = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGround != NULL)
	{
		g_pVtxBuffGround->Release();
		g_pVtxBuffGround = NULL;
	}
}

//�n�ʂ̍X�V����
void UpdateGround(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGround->Lock(0, 0, (void* *)&pVtx, 0);

	g_GroundMoveX += 0.02f;

	//���_���W�̐ݒ�
	if (GetMode() == MODE_GAME_00)
	{// �Q�[���̎��A
		pVtx[0].tex.x = g_GroundMoveX;
		pVtx[1].tex.x = g_GroundMoveX + 10.0f;
		pVtx[2].tex.x = g_GroundMoveX;
		pVtx[3].tex.x = g_GroundMoveX + 10.0f;
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffGround->Unlock();
}

//�n�ʂ̕`�揈��
void DrawGround(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGround, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGround);

	for (int nCntGround = 0; nCntGround < MAX_GROUND; nCntGround++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGround * 4, 2);
	}
}
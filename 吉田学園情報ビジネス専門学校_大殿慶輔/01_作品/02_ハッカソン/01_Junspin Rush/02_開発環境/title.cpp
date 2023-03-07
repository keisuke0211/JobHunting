//��======================��
//�b�@�@  �@�@�@�@�@�@�@�@�b
//�b�@�^�C�g����ʂ̏����@�b
//�b�@Author:��a�c��@   �b
//�b                      �b
//��======================��
#include"title.h"
#include"sound.h"
#include "fade.h"
#include "bg.h"
#include "ground.h"
#include "cloud.h"
#include "ui_frame_02.h"
//��==============��
//�b�@�}�N����`�@�b
//��==============��
#define NUM_BG (2)      //�w�i�̐�
//��==================��
//�b�@�O���[�o���ϐ��@�b
//��==================��
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTitle = NULL;		//���_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_BG] = {};    //�e�N�X�`��(4����)�ւ̃|�C���^
D3DXCOLOR g_colorTitle;								//�|���S��(�t�F�[�h)�̐F

int g_nCnTFlash;									 //�_�Ŏ���
bool bFlash;										 //�_�Ő؂�ւ�
TITLE g_Title[NUM_BG];								 //�G�̏��

//==============================================================================================================================================
//�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�b�y�^�C�g����ʁE�����z�b�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\
//==============================================================================================================================================

//��======================================��
//�b�@�@�@ �^�C�g����ʂ̏����������@�@ �@�b
//��======================================��
void InitTitle(void)
{
	PlaySound(SOUND_LABEL_BGM_TITLE_000);

	int nCntTitle;
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̏���
	pDevice = GetDevice();

	//���\�\�\�\�\�\�\�\�\�\�\�\��
	//�b�@�e�N�X�`���̓ǂݍ��݁@�b
	//���\�\�\�\�\�\�\�\�\�\�\�\��
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\bg000.jpg",
		&g_pTextureTitle[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\-PRESS ENTER-.png",
		&g_pTextureTitle[1]);

	//���\�\�\�\�\�\�\�\�\�\�\�\��
	//�b�@�@ �A�j���[�V���� �@�@�b
	//���\�\�\�\�\�\�\�\�\�\�\�\��

	g_nCnTFlash = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTitle,
		NULL);

	//�ݒ�p�|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffTitle->Lock(0, 0, (void **)&pVtx, 0);

	bFlash = false;
	for (nCntTitle = 0; nCntTitle < NUM_BG; nCntTitle++, pVtx += 4)
	{
		g_colorTitle = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�����|���S��(�s����)�ɂ��Ă���
		g_Title[nCntTitle].bUse = true;
		//���\�\�\�\�\�\�\�\�\�\�\�\��
		//�b �@�@���_���W��ݒ�@�@ �b
		//���\�\�\�\�\�\�\�\�\�\�\�\��

		//�@�@�@�@�@�@�@�@�@�@�@�@ x       y     z
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f); //�E���Őݒ肷��
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); //�����Őݒ肷��

	    //���\�\�\�\�\�\�\�\�\�\�\�\��
	    //�b�@�@�@�@rhw�̐ݒ�@�@�@ �b
	    //���\�\�\�\�\�\�\�\�\�\�\�\��
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���\�\�\�\�\�\�\�\�\�\�\�\��
		//�b�@�@�@ ���_�J���[ �@�@�@�b
		//���\�\�\�\�\�\�\�\�\�\�\�\��

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//���\�\�\�\�\�\�\�\�\�\�\�\��
		//�b�@�e�N�X�`�����W�̐ݒ�@�b
		//���\�\�\�\�\�\�\�\�\�\�\�\��

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	g_Title[4].state = TITLESTATE_NORMAL;
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxbuffTitle->Unlock();

	InitBg();		// �w�i
	InitGround();	// �n��
	InitCloud();	// �_
	SetCloud2();		// �_�ݒ�
	InitUi_frame_02();

	SetStateUi_frame_02(UI_FRAME_02_STATE_IN_POP);
}
//��====================================��
//�b�@�@�@ �^�C�g����ʂ̏I�������@�@ �@�b
//��====================================��
void UninitTitle(void)
{
	int nCntTitle;

	//�T�E���h�̒�~
	StopSound();

	//���\�\�\�\�\�\�\�\�\�\�\�\��
	//�b�@�@�e�N�X�`���̔j���@�@�b
	//���\�\�\�\�\�\�\�\�\�\�\�\��
	for (nCntTitle = 0; nCntTitle < NUM_BG; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}
	//���\�\�\�\�\�\�\�\�\�\�\�\�\��
	//�b�@�@���_�o�b�t�@�̔j���@�@�b
	//���\�\�\�\�\�\�\�\�\�\�\�\�\��
	if (g_pVtxbuffTitle != NULL)
	{
		g_pVtxbuffTitle->Release();
		g_pVtxbuffTitle = NULL;
	}

	UninitBg();		// �w�i
	UninitGround();	// �n��
	UninitCloud();	// �_
	UninitUi_frame_02();
}
//��====================================��
//�b�@�@�@ �^�C�g����ʂ̍X�V�����@�@ �@�b
//��====================================��
void UpdateTitle(void)
{	
	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{// ����L�[ [ ENTER�L�[ ] �������ꂽ
		PlaySound(SOUND_LABEL_SE_DETERMINATION_000);
		//���[�h�̐ݒ�
		SetFade(MODE_GAME_00);
	}
	//�_��
	g_nCnTFlash++;

	//�ݒ�p�|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffTitle->Lock(0, 0, (void **)&pVtx, 0);

	g_pVtxbuffTitle->Unlock();


	if (g_nCnTFlash >= 26 && bFlash == false)
	{//�_�Ő؂�ւ����ԂɂȂ���
		g_Title[1].bUse = g_Title[1].bUse ? false : true;
		g_nCnTFlash = 0;
	}
	else if (g_nCnTFlash >= 6 && bFlash == true)
	{
		g_Title[1].bUse = g_Title[1].bUse ? false : true;
		g_nCnTFlash = 0;
	}

	UpdateBg();		// �w�i
	UpdateGround();	// �n��
	UpdateCloud();	// �_
	UpdateUi_frame_02();
}
//��====================================��
//�b�@�@�@ �^�C�g����ʂ̕`�揈���@�@ �@�b
//��====================================��
void DrawTitle(void)
{
	DrawCloud();	// �_
	DrawBg();		// �w�i
	DrawGround();	// �n��
	DrawUi_frame_02();

	int nCntTitle;
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̏���
	pDevice = GetDevice();

	//���\�\�\�\�\�\�\�\�\�\�\�\��
	//�b�f�[�^�X�g���[���̐ݒ�@�b
	//���\�\�\�\�\�\�\�\�\�\�\�\��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffTitle, 0, sizeof(VERTEX_2D));

	//���\�\�\�\�\�\�\�\�\�\�\�\��
	//�b���_�t�H�[�}�b�g�̐ݒ�@�b
	//���\�\�\�\�\�\�\�\�\�\�\�\��
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���\�\�\�\�\�\�\�\�\�\�\�\��
	//�b�@�@�e�N�X�`���̐ݒ�@�@�b
	//���\�\�\�\�\�\�\�\�\�\�\�\��
	
	for (nCntTitle = 1; nCntTitle < NUM_BG; nCntTitle++)
	{
		if (g_Title[nCntTitle].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);
			//���\�\�\�\�\�\�\�\�\�\�\�\��
			//�b�@�@�|���S���̕`��@�@�@�b
			//���\�\�\�\�\�\�\�\�\�\�\�\��

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 4);
		}
	}
}
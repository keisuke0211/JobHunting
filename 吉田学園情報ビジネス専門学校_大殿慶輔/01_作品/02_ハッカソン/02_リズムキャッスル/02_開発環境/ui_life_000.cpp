//========================================
//
//���C�t000�̏���
//Author:���� ����
//
//========================================
// *** ui_life_000.cpp ***
//========================================
#include "main.h"
#include "ui_life_000.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//========== �}�N����`			==========//
#define UI_LIFE_000_TEXTURE_PATH					"data\\TEXTURE\\UserInterface\\ui_life_000.png"		//���C�t000�̕\���`��
#define UI_LIFE_000_TEXTURE_X_PATTERN_MAX			(4)										//���C�t000�̃e�N�X�`���̃p�^�[����(��)
#define UI_LIFE_000_TEXTURE_Y_PATTERN_MAX			(1)										//���C�t000�̃e�N�X�`���̃p�^�[����(�c)
#define UI_LIFE_000_WIDTH							(PIXEL * 16)							//���C�t000�̕�
#define UI_LIFE_000_HEIGHT							(PIXEL * 16)							//���C�t000�̍���
#define UI_LIFE_000_X								(PIXEL * 16)							//���C�t000�̈ʒuX
#define UI_LIFE_000_Y								(PIXEL * 16)							//���C�t000�̈ʒuY
#define UI_LIFE_000_SPACE_X							(PIXEL * 16)							//���C�t000�̔z�u�ԊuX
#define UI_LIFE_000_HEARTBEAT_ANIM_START_PATTERN	(0)										//���C�t000�̌ۓ��̃A�j���[�V�����̊J�n�p�^�[��No.
#define UI_LIFE_000_HEARTBEAT_ANIM_END_PATTERN		(3)										//���C�t000�̌ۓ��̃A�j���[�V�����̏I���p�^�[��No.
#define UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_0	(70)									//���C�t000�̌ۓ��̃A�j���[�V�����̐؂�ւ�����[0]
#define UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_1	(3)										//���C�t000�̌ۓ��̃A�j���[�V�����̐؂�ւ�����[1]
#define UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_2	(2)										//���C�t000�̌ۓ��̃A�j���[�V�����̐؂�ւ�����[2]
#define UI_LIFE_000_STOP_PATTERN					(3)										//���C�t000�̒�~�̃p�^�[��No.

//========== �O���[�o���錾		==========//
LPDIRECT3DTEXTURE9 g_pTextureUi_life_000 = NULL;		//���C�t000�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_life_000 = NULL;	//���C�t000�̒��_�o�b�t�@�ւ̃|�C���^
Ui_life_000 g_aUi_life_000[5];							//���C�t000�̏��

//========================================
//InitUi_life_000�֐� - ���C�t000�̏��������� -
//========================================
void InitUi_life_000(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int nCntUi_life_000;		//�N����000�̃J�E���g

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, UI_LIFE_000_TEXTURE_PATH, &g_pTextureUi_life_000);

	for (nCntUi_life_000 = 0; nCntUi_life_000 < 5; nCntUi_life_000++)
	{
		g_aUi_life_000[nCntUi_life_000].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu������������
		g_aUi_life_000[nCntUi_life_000].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����������������
		g_aUi_life_000[nCntUi_life_000].nPattern = 0;							//�p�^�[��No.������������
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 5,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_life_000,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_life_000->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi_life_000 = 0; nCntUi_life_000 < 5; nCntUi_life_000++, pVtx += 4)
	{
		//���_���W��ݒ�
		pVtx[0].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x - (UI_LIFE_000_WIDTH * 0.5f) + (nCntUi_life_000 * UI_LIFE_000_SPACE_X);
		pVtx[0].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y - UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[1].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x + (UI_LIFE_000_WIDTH * 0.5f) + (nCntUi_life_000 * UI_LIFE_000_SPACE_X);
		pVtx[1].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y - UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[2].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x - (UI_LIFE_000_WIDTH * 0.5f) + (nCntUi_life_000 * UI_LIFE_000_SPACE_X);
		pVtx[2].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y + UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[3].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x + (UI_LIFE_000_WIDTH * 0.5f) + (nCntUi_life_000 * UI_LIFE_000_SPACE_X);
		pVtx[3].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y + UI_LIFE_000_HEIGHT * 0.5f;
		
		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), ((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[1].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) + (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), ((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[2].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), (((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX)) + (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[3].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) + (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), (((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX)) + (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
	}

	//���_���W���A�����b�N����
	g_pVtxBuffUi_life_000->Unlock();
}

//========================================
//UninitUi_life_000�֐� - ���C�t000�̏I������ -
//========================================
void UninitUi_life_000(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureUi_life_000 != NULL)
	{
		g_pTextureUi_life_000->Release();
		g_pTextureUi_life_000 = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_life_000 != NULL)
	{
		g_pVtxBuffUi_life_000->Release();
		g_pVtxBuffUi_life_000 = NULL;
	}
}

//========================================
//UpdateUi_life_000�֐� - ���C�t000�̍X�V���� -
//========================================
void UpdateUi_life_000(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int nCntUi_life_000;		//���C�t000�̃J�E���g

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_life_000->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi_life_000 = 0; nCntUi_life_000 < 5; nCntUi_life_000++, pVtx += 4)
	{
		//�A�j���[�V�����̃J�E���^�[�����Z
		g_aUi_life_000[nCntUi_life_000].nCounterAnim++;

		if (((g_aUi_life_000[nCntUi_life_000].nCounterAnim >= UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_0) && g_aUi_life_000[nCntUi_life_000].nPattern == UI_LIFE_000_HEARTBEAT_ANIM_START_PATTERN)
			|| ((g_aUi_life_000[nCntUi_life_000].nCounterAnim >= UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_1) && g_aUi_life_000[nCntUi_life_000].nPattern == UI_LIFE_000_HEARTBEAT_ANIM_START_PATTERN + 1)
			|| ((g_aUi_life_000[nCntUi_life_000].nCounterAnim >= UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_2) && g_aUi_life_000[nCntUi_life_000].nPattern == UI_LIFE_000_HEARTBEAT_ANIM_START_PATTERN + 2))
		{//�J�E���^�[���A�j���[�V�����̐؂�ւ����ԂɒB������

			//�J�E���^�[��������
			g_aUi_life_000[nCntUi_life_000].nCounterAnim = 0;

			//�p�^�[�����X�V
			g_aUi_life_000[nCntUi_life_000].nPattern = ++g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_HEARTBEAT_ANIM_END_PATTERN;
		}

		if (nCntUi_life_000 >= GetPlayer()->nLife)
		{//���C�t000�̃J�E���g���v���C���[�̗͈̑ȏゾ������

			//�p�^�[��No.���~�Őݒ�
			g_aUi_life_000[nCntUi_life_000].nPattern = UI_LIFE_000_STOP_PATTERN;
		}
		else if (g_aUi_life_000[nCntUi_life_000].nPattern == UI_LIFE_000_STOP_PATTERN)
		{//�p�^�[��No.����~��������

			//�p�^�[��No.������������
			g_aUi_life_000[nCntUi_life_000].nPattern = 0;
		}

		//�ʒu��ݒ肷��
		g_aUi_life_000[nCntUi_life_000].pos = D3DXVECTOR3(UI_LIFE_000_X + (nCntUi_life_000 * UI_LIFE_000_SPACE_X), UI_LIFE_000_Y, 0.0f);

		//���_���W��ݒ�
		pVtx[0].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x - UI_LIFE_000_WIDTH * 0.5f;
		pVtx[0].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y - UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[1].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x + UI_LIFE_000_WIDTH * 0.5f;
		pVtx[1].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y - UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[2].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x - UI_LIFE_000_WIDTH * 0.5f;
		pVtx[2].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y + UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[3].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x + UI_LIFE_000_WIDTH * 0.5f;
		pVtx[3].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y + UI_LIFE_000_HEIGHT * 0.5f;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), ((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[1].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) + (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), ((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[2].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), (((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX)) + (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[3].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) + (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), (((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX)) + (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
	}

	//���_���W���A�����b�N����
	g_pVtxBuffUi_life_000->Unlock();
}

//========================================
//DrawUi_life_000�֐� - ���C�t000�̕`�揈�� -
//========================================
void DrawUi_life_000(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int nCntUi_life_000;		//���C�t000�̃J�E���g

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_life_000, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureUi_life_000);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_life_000->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi_life_000 = 0; nCntUi_life_000 < 5; nCntUi_life_000++) 
	{
		//�N����000�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi_life_000 * 4, 2);
	}

	//���_���W���A�����b�N����
	g_pVtxBuffUi_life_000->Unlock();
}
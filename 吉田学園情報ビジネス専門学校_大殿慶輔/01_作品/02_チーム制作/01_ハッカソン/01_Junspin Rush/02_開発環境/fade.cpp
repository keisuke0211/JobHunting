//========================================
// 
// �t�F�[�h�̏���
// Author:���� ����
// 
//========================================
//  *** fade.cpp ***
//========================================
#include "main.h"
#include "fade.h"
#include "sound.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//========== �}�N����`			==========//
#define FADE_ALPHA_TRANSITIVE_VALUE	(0.025f)	// �t�F�[�h�̓����x�̐��ڒl

//========== �O���[�o���錾		==========//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
FADE g_fade;									// �t�F�[�h�̏��
MODE g_modeNext;								// ���̉��(���[�h)
D3DXCOLOR g_colorFade;							// �|���S��(�t�F�[�h)�̐F

//========== �t�F�[�h�̏�Ԃ��擾	======//
FADE GetFade(void) 
{
	return g_fade;
}

//========================================
// InitFade�֐� - �t�F�[�h�̏��������� -
//========================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	POINT vertexPos[4];			// �l�p�`�̒��_�̍��W

	g_fade		= FADE_IN;								// �t�F�[�h�C����Ԃ�
	g_modeNext	= modeNext;								// ���̉��(���[�h)��ݒ�
	g_colorFade	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// �����|���S��(�s����)�ɂ���
	
	// BGM�̉��ʂ�ύX����
	SoundTypeVolumeChange(SOUND_TYPE_BGM, 0.0f);

	// ���[�h�̐ݒ�
	SetMode(g_modeNext);

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W��ݒ�
	pVtx[0].pos.x = 0.0f;
	pVtx[0].pos.y = 0.0f;
	pVtx[1].pos.x = SCREEN_WIDTH;
	pVtx[1].pos.y = 0.0f;
	pVtx[2].pos.x = 0.0f;
	pVtx[2].pos.y = SCREEN_HEIGHT;
	pVtx[3].pos.x = SCREEN_WIDTH;
	pVtx[3].pos.y = SCREEN_HEIGHT;

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

	// ���_���W���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//========================================
// UninitFade�֐� - �t�F�[�h�̏I������ -
//========================================
void UninitFade(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL) 
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//========================================
// UpdateFade�֐� - �t�F�[�h�̍X�V���� -
//========================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE) 
	{
		if (g_fade == FADE_IN)
		{// �t�F�[�h�C�����
			// �|���S���𓧖��ɂ��Ă���
			g_colorFade.a -= FADE_ALPHA_TRANSITIVE_VALUE;

			// BGM�̉��ʂ�ύX����
			SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - g_colorFade.a);

			if (g_colorFade.a <= 0.0f) 
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade == FADE_OUT) 
		{// �t�F�[�h�A�E�g���
			// �|���S����s�����ɂ��Ă���
			g_colorFade.a += FADE_ALPHA_TRANSITIVE_VALUE;

			// BGM�̉��ʂ�ύX����
			SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - g_colorFade.a);

			if (g_colorFade.a >= 1.0f) 
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;

				// BGM���ꊇ��~����
				StopSoundType(SOUND_TYPE_BGM);

				// ���[�h��ݒ肷��
				SetMode(g_modeNext);
			}
		}

		VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�																						   
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		// ���_���W���A�����b�N����
		g_pVtxBuffFade->Unlock();
	}
}

//========================================
// DrawFade�֐� - �t�F�[�h�̕`�揈�� -
//========================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �t�F�[�h�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetFade�֐� - �t�F�[�h�̐ݒ菈�� -
//========================================
void SetFade(MODE modeNext)
{
	g_fade		= FADE_OUT;								// �t�F�[�h�A�E�g��Ԃ�
	g_modeNext	= modeNext;								// ���̉��(���[�h)��ݒ�
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �����|���S��(����)�ɂ���
}
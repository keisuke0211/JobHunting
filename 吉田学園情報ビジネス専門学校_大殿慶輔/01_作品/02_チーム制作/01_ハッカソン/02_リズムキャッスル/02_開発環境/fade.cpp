//========================================
// 
// �t�F�[�h�̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** fade.cpp ***
//========================================
#include "main.h"
#include "fade.h"
#include "polygon2D.h"
#include "sound.h"

//****************************************
// �}�N����`
//****************************************

// �t�F�[�h�̓����x�̐��ڒl
#define FADE_ALPHA_TRANSITIVE_VALUE	(5)

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffFade	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
FADE					g_fade;			// �t�F�[�h�̏��
FADE					g_fadeTemp;		// �t�F�[�h�̏�Ԃ�ۑ�
MODE					g_modeNext;		// ���̉��(���[�h)
Color					g_colorFade;	// �|���S��(�t�F�[�h)�̐F

//========== *** �t�F�[�h�̏�Ԃ��擾 ***
FADE GetFade(void) 
{
	return g_fade;
}

//========== *** �t�F�[�h�̐؂�ւ��擾 ***
bool GetFadeSwap(void) 
{
	return (g_fadeTemp == FADE_OUT) && (g_fade == FADE_IN);
}

//========================================
// InitFade�֐� - �t�F�[�h�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	g_fade		= FADE_IN;			// �t�F�[�h�C����Ԃ�
	g_modeNext	= modeNext;			// ���̉��(���[�h)��ݒ�
	g_colorFade = { 0,0,0,255 };	// �����|���S��(�s����)�ɂ���
	
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
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(0, 0, 0),
		false,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, g_colorFade);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//========================================
// UninitFade�֐� - �t�F�[�h�̏I������ -
// Author:RIKU NISHIMURA
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
// Author:RIKU NISHIMURA
//========================================
void UpdateFade(void)
{
	// �t�F�[�h�̏�Ԃ�ۑ�
	g_fadeTemp = g_fade;

	if (g_fade != FADE_NONE) 
	{
		if (g_fade == FADE_IN)
		{// �t�F�[�h�C�����
			// �|���S���𓧖��ɂ��Ă���
			g_colorFade.a -= FADE_ALPHA_TRANSITIVE_VALUE;

			if (g_modeNext != MODE_NONE)
			{// ���̃��[�h�������łȂ����A
				// BGM��SE�̉��ʂ�ύX����
				SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - ((float)g_colorFade.a / (float)255));
				SoundTypeVolumeChange(SOUND_TYPE_SE, 1.0f - ((float)g_colorFade.a / (float)255));
			}

			if (g_colorFade.a <= 0) 
			{// �����x��0�ȉ��̎��A
				g_colorFade.a = 0;	// �����x��0�ɐݒ�
				g_fade = FADE_NONE;	// �t�F�[�h�A�E�g��Ԃɂ���
			}
		}
		else if (g_fade == FADE_OUT) 
		{// �t�F�[�h�A�E�g���
			// �|���S����s�����ɂ��Ă���
			g_colorFade.a += FADE_ALPHA_TRANSITIVE_VALUE;

			if (g_modeNext != MODE_NONE)
			{// ���̃��[�h�������łȂ����A
				// BGM��SE�̉��ʂ�ύX����
				SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - ((float)g_colorFade.a / (float)255));
				SoundTypeVolumeChange(SOUND_TYPE_SE, 1.0f - ((float)g_colorFade.a / (float)255));
			}

			if (g_colorFade.a > 255) 
			{// �����x���ő�l�����������A
				g_colorFade.a = 255;	// �����x���ő�l�ɐݒ�
				g_fade = FADE_IN;		// �t�F�[�h�C����Ԃɂ���

				if (g_modeNext != MODE_NONE)
				{// ���̃��[�h�������łȂ����A
					// BGM��SE���ꊇ�Œ�~
					StopSoundType(SOUND_TYPE_BGM);
					StopSoundType(SOUND_TYPE_SE);

					// ���[�h��ݒ肷��
					SetMode(g_modeNext);
				}
			}
		}

		VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
																					   
		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, g_colorFade);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffFade->Unlock();
	}
}

//========================================
// DrawFade�֐� - �t�F�[�h�̕`�揈�� -
// Author:RIKU NISHIMURA
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
// Author:RIKU NISHIMURA
//========================================
void SetFade(MODE modeNext)
{
	g_fade		= FADE_OUT;			// �t�F�[�h�A�E�g��Ԃ�
	g_modeNext	= modeNext;			// ���̉��(���[�h)��ݒ�
	g_colorFade = { 0, 0, 0, 0 };	// �����|���S��(����)�ɂ���
}
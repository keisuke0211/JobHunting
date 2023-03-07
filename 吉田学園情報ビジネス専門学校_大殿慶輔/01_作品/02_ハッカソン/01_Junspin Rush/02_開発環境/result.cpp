//==========================================
//
//���U���g�֌W�v���O����[result.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "ranking.h"
#include "md_game_00.h"
#include "sound.h"

//�}�N��
#define RETURN_TIME			(5000)	//�^�C�g����ʂɖ߂�܂ł̎���
#define RESULT_USEPOLY		(1)		//�|���S����
#define RESULT_TEXTURE_NUM	(1)		//�e�N�X�`����

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_TEXTURE_NUM];	//�e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffResult = NULL;			//���_�o�b�t�@�|�C���^
DWORD g_dwCurrentTime, g_dwStartTime;						//���U���g��ʊJ�n����̎��Ԃƌ��݂̎���

//========================
//����������
//========================
void InitResult(void)
{
	PlaySound(SOUND_LABEL_BGM_RESULT_000);

	int nCntResultPolyData;		//�J�E���^
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\-PRESS ENTER-.png",
		&g_pTextureResult[0]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RESULT_USEPOLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffResult,
		NULL);

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffResult->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntResultPolyData = 0; nCntResultPolyData < RESULT_USEPOLY; nCntResultPolyData++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = 0.0f;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = SCREEN_WIDTH;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = 0.0f;
		pVtx[2].pos.y = SCREEN_HEIGHT;
		pVtx[3].pos.x = SCREEN_WIDTH;
		pVtx[3].pos.y = SCREEN_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//���W�ϊ��p�W���ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffResult->Unlock();

	//���Ԑݒ�
	g_dwCurrentTime = 0;
	g_dwStartTime = timeGetTime();

	SetRanking(*GetScore());

	InitRanking();
}

//========================
//�I������
//========================
void UninitResult(void)
{
	int nCntUninitResult;	//�e�N�X�`���j���J�E���^

	//�����L���O�I������
	UninitRanking();

	for (nCntUninitResult = 0; nCntUninitResult < RESULT_TEXTURE_NUM; nCntUninitResult++)
	{
		if (g_pTextureResult[nCntUninitResult] != NULL)
		{
			g_pTextureResult[nCntUninitResult]->Release();
			g_pTextureResult[nCntUninitResult] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffResult != NULL)
	{
		g_pVtxbuffResult->Release();
		g_pVtxbuffResult = NULL;
	}
}

//========================
//�X�V����
//========================
void UpdateResult(void)
{
	//�����L���O�X�V
	UpdateRanking();

	g_dwCurrentTime = timeGetTime();

	if (GetKeyboardTrigger(DIK_RETURN) == true 
		&& GetFade() == FADE_NONE)
	{
		//�^�C�g���ɖ߂�
		SetFade(MODE_TITLE_00);
	}
}

//========================
//�`�揈��
//========================
void DrawResult(void)
{
	int nCntTexPattern;		//�J�E���^

	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffResult, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	for (nCntTexPattern = 0; nCntTexPattern < RESULT_TEXTURE_NUM; nCntTexPattern ++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexPattern]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexPattern * 4, 2);
	}
	//�����L���O�`��
	DrawRanking();
}
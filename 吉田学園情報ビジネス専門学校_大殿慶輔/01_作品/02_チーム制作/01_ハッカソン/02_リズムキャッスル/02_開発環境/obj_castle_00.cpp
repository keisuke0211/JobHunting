//========================================
// 
// OBJ:��[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_castle_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"
#include "sound.h"
#include "obj_castle_00.h"	// OBJ:��	[00]

//****************************************
// �}�N����`
//****************************************
// OBJ:��[00] �̃e�N�X�`���̑��΃p�X
#define OBJ_CASTLE_00_TEXTURE_PATH	"data\\TEXTURE\\BackGround\\castle.png"
#define OBJ_CASTLE_00_WIDTH			(240)
#define OBJ_CASTLE_00_HEIGHT		(240)
#define OBJ_CASTLE_00_POS			D3DXVECTOR3(160.0f,520.0f,0.0f)
#define OBJ_CASTLE_00_DAMAGE_TIME	(40)
#define OBJ_CASTLE_00_VIB_MAX		(20)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:��[00] �̃p�����[�^�[����������
void InitParameterObj_castle_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_castle_00	// �e�N�X�`���ւ̃|�C���^
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObj_castle_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Obj_castle_00		g_obj_castle_00;			// OBJ:��[00] �̕��i���̏��

//========== *** OBJ:��[00] �̏����擾 ***
Obj_castle_00 *GetObj_castle_00(void) 
{
	return &g_obj_castle_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_castle_00�֐� - OBJ:��[00] �̃p�����[�^�[���������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterObj_castle_00(void)
{
	Obj_castle_00	*pObj	// OBJ:��[00] �̏��̃|�C���^
					= &g_obj_castle_00;

	pObj->nCounterDamage = 0;	// �����J�E���^�[
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitObj_castle_00�֐� - OBJ:��[00] �̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitObj_castle_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^
	Obj_castle_00	*pObj		// OBJ:��[00] �̕��i���̏��̃|�C���^
						= &g_obj_castle_00;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, OBJ_CASTLE_00_TEXTURE_PATH, &g_aTextureObj_castle_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffObj_castle_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffObj_castle_00->Lock(0, 0, (void**)&pVtx, 0);

	// �p�����[�^�[�̏���������
	InitParameterObj_castle_00();

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		OBJ_CASTLE_00_POS,
		INITD3DXVECTOR3,
		false,
		OBJ_CASTLE_00_WIDTH,
		OBJ_CASTLE_00_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, INITCOLOR);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffObj_castle_00->Unlock();
}

//========================================
// UninitObj_castle_00�֐� - OBJ:��[00] �̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitObj_castle_00(void)
{
	// �e�N�X�`���̔j��
	if (g_aTextureObj_castle_00 != NULL)
	{
		g_aTextureObj_castle_00->Release();
		g_aTextureObj_castle_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffObj_castle_00 != NULL) 
	{
		g_pVtxBuffObj_castle_00->Release();
		g_pVtxBuffObj_castle_00 = NULL;
	}
}

//========================================
// UpdateObj_castle_00�֐� - OBJ:��[00] �̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateObj_castle_00(void)
{
	VERTEX_2D				*pVtx;	// ���_���ւ̃|�C���^
	Obj_castle_00		*pObj	// OBJ:��[00] �̕��i���̏��̃|�C���^
							= &g_obj_castle_00;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffObj_castle_00->Lock(0, 0, (void**)&pVtx, 0);

	if (pObj->nCounterDamage > 0) 
	{
		pObj->nCounterDamage--;
	}

	float fDamageRate = (float)pObj->nCounterDamage / (float)OBJ_CASTLE_00_DAMAGE_TIME;

	// ���_���W��ݒ�
	{
		int nVib = (OBJ_CASTLE_00_VIB_MAX * fDamageRate) + 1;	// �U�ꕝ

		SetVertexPos2D(pVtx,
			OBJ_CASTLE_00_POS + D3DXVECTOR3((-rand() % nVib) + (rand() % nVib),0.0f,0.0f),
			INITD3DXVECTOR3,
			false,
			OBJ_CASTLE_00_WIDTH,
			OBJ_CASTLE_00_HEIGHT,
			ANGLE_TYPE_FIXED);
	}
	// ���_�J���[�̐ݒ�
	{
		SetVertexColor2D(pVtx,
		{
			255,
			(int)(255 * (1.0f - fDamageRate)),
			(int)(255 * (1.0f - fDamageRate)),
			255,
		});
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffObj_castle_00->Unlock();
}

//========================================
// DrawObj_castle_00�֐� - OBJ:��[00] �̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawObj_castle_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffObj_castle_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureObj_castle_00);

	// OBJ:��[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetDamageObj_castle_00�֐� - OBJ:��[00] �̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetDamageObj_castle_00(void) 
{
	Obj_castle_00	*pObj	// OBJ:��[00] �̕��i���̏��̃|�C���^
					= &g_obj_castle_00;

	pObj->nCounterDamage = OBJ_CASTLE_00_DAMAGE_TIME;
	PlaySound(6);
}
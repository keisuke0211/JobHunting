//========================================
// 
// OBJ:��[01] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** obj_castle_01.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"
#include "obj_castle_01.h"	// OBJ:��	[01]

//****************************************
// �}�N����`
//****************************************
// OBJ:��[01] �̃e�N�X�`���̑��΃p�X
#define OBJ_CASTLE_01_TEXTURE_PATH	"data\\TEXTURE\\BackGround\\enemycastle.png"
#define OBJ_CASTLE_01_WIDTH			(240)
#define OBJ_CASTLE_01_HEIGHT		(240)
#define OBJ_CASTLE_01_POS			D3DXVECTOR3(SCREEN_WIDTH-160.0f,520.0f,0.0f)
#define OBJ_CASTLE_01_DAMAGE_TIME	(40)
#define OBJ_CASTLE_01_VIB_MAX		(20)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:��[01] �̃p�����[�^�[����������
void InitParameterObj_castle_01(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureObj_castle_01	// �e�N�X�`���ւ̃|�C���^
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffObj_castle_01	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Obj_castle_01			g_obj_castle_01;		// OBJ:��[01] �̕��i���̏��

//========== *** OBJ:��[01] �̏����擾 ***
Obj_castle_01 *GetObj_castle_01(void) 
{
	return &g_obj_castle_01;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterObj_castle_01�֐� - OBJ:��[01] �̃p�����[�^�[���������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterObj_castle_01(void)
{
	Obj_castle_01	*pObj	// OBJ:��[01] �̏��̃|�C���^
					= &g_obj_castle_01;

	pObj->nCounterDamage = 0;	// �����J�E���^�[
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitObj_castle_01�֐� - OBJ:��[01] �̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitObj_castle_01(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^
	Obj_castle_01	*pObj		// OBJ:��[01] �̕��i���̏��̃|�C���^
						= &g_obj_castle_01;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, OBJ_CASTLE_01_TEXTURE_PATH, &g_aTextureObj_castle_01);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffObj_castle_01,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffObj_castle_01->Lock(0, 0, (void**)&pVtx, 0);

	// �p�����[�^�[�̏���������
	InitParameterObj_castle_01();

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		OBJ_CASTLE_01_POS,
		INITD3DXVECTOR3,
		false,
		OBJ_CASTLE_01_WIDTH,
		OBJ_CASTLE_01_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, INITCOLOR);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffObj_castle_01->Unlock();
}

//========================================
// UninitObj_castle_01�֐� - OBJ:��[01] �̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitObj_castle_01(void)
{
	// �e�N�X�`���̔j��
	if (g_aTextureObj_castle_01 != NULL)
	{
		g_aTextureObj_castle_01->Release();
		g_aTextureObj_castle_01 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffObj_castle_01 != NULL) 
	{
		g_pVtxBuffObj_castle_01->Release();
		g_pVtxBuffObj_castle_01 = NULL;
	}
}

//========================================
// UpdateObj_castle_01�֐� - OBJ:��[01] �̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateObj_castle_01(void)
{
	VERTEX_2D		*pVtx;	// ���_���ւ̃|�C���^
	Obj_castle_01	*pObj	// OBJ:��[01] �̕��i���̏��̃|�C���^
					= &g_obj_castle_01;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffObj_castle_01->Lock(0, 0, (void**)&pVtx, 0);

	if (pObj->nCounterDamage > 0) 
	{
		pObj->nCounterDamage--;
	}

	float fDamageRate = (float)pObj->nCounterDamage / (float)OBJ_CASTLE_01_DAMAGE_TIME;

	// ���_���W��ݒ�
	{
		int nVib = (OBJ_CASTLE_01_VIB_MAX * fDamageRate) + 1;	// �U�ꕝ

		SetVertexPos2D(pVtx,
			OBJ_CASTLE_01_POS + D3DXVECTOR3((-rand() % nVib) + (rand() % nVib),0.0f,0.0f),
			INITD3DXVECTOR3,
			false,
			OBJ_CASTLE_01_WIDTH,
			OBJ_CASTLE_01_HEIGHT,
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
	g_pVtxBuffObj_castle_01->Unlock();
}

//========================================
// DrawObj_castle_01�֐� - OBJ:��[01] �̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawObj_castle_01(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffObj_castle_01, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureObj_castle_01);

	// OBJ:��[01] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetDamageObj_castle_01�֐� - OBJ:��[01] �̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetDamageObj_castle_01(void) 
{
	Obj_castle_01	*pObj	// OBJ:��[01] �̕��i���̏��̃|�C���^
					= &g_obj_castle_01;

	pObj->nCounterDamage = OBJ_CASTLE_01_DAMAGE_TIME;
}
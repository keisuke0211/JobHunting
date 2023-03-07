//========================================
// 
// BG:���U���g[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** bg_result_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "physics.h"
#include "polygon2D.h"
#include "bg_result_00.h"	// BG:���U���g	[00]

//****************************************
// �}�N����`
//****************************************
// BG:���U���g[00] �̃f�[�^�t�@�C���̑��΃p�X
#define BG_RESULT_00_DATA_FILE_PATH	"data\\GAMEDATA\\BACKGROUND\\BG_RESULT_00_DATA.txt"

// BG:���U���g[00] �̕��i�̍ő吔
#define BG_RESULT_00_PARTS_MAX	(4)

//****************************************
// �\���̂̒�`
//****************************************
// BG:���U���g[00] �̊Ǘ����
typedef struct 
{
	int			nPartNum;	// ���i��
}Bg_result_00Control;

// BG:���U���g[00] �̕��i���̏��
typedef struct
{
	char		aTexturePath	// �e�N�X�`���̑��΃p�X
				[TXT_MAX];
	D3DXVECTOR3	move;			// �ړ���
	Scale		cutScale;		// �؂���{��
	D3DXVECTOR3	pos;			// �ʒu
}Bg_result_00Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// BG:���U���g[00] �̃p�����[�^�[����������
void InitParameterBg_result_00(Bg_result_00Parts *pParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureBg_result_00	// �e�N�X�`���ւ̃|�C���^
						[BG_RESULT_00_PARTS_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg_result_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Bg_result_00Control		g_bg_result_00Control;	// BG:���U���g[00] �̊Ǘ����
Bg_result_00Parts		g_aBg_result_00Parts		// BG:���U���g[00] �̕��i���̏��
						[BG_RESULT_00_PARTS_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterBg_result_00�֐� - BG:���U���g[00] �̃p�����[�^�[���������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterBg_result_00(Bg_result_00Parts *pParts)
{
	pParts->pos = INITD3DXVECTOR3;	// �ʒu
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadBg_result_00�֐� - BG:���U���g[00] �̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadBg_result_00(void) 
{
	FILE				*pFile;		// �t�@�C���|�C���^
	char				aDataSearch	// �f�[�^�����p
						[TXT_MAX];
	Bg_result_00Control	*pCtl		// BG:���U���g[00] �̊Ǘ����̃|�C���^
						= &g_bg_result_00Control;
	Bg_result_00Parts	*pParts		// BG:���U���g[00] �̕��i���̏��̃|�C���^
						= g_aBg_result_00Parts;

	// ���i����������
	pCtl->nPartNum = 0;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(BG_RESULT_00_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if		(!strcmp(aDataSearch, "END"))		{ fclose(pFile); break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "PARTSSET"))
		{// ���i�̐ݒ���̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// ����

				if		(!strcmp(aDataSearch, "PARTSSET_END"))	{ break; }	// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "PARTS"))
				{// ���i���̏��̓ǂݍ��݂��J�n
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	// ����

						if (!strcmp(aDataSearch, "PARTS_END")) {
							pParts++;			// �|�C���^��i�߂�
							pCtl->nPartNum++;	// ���i�������Z
							break;
						}
						else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))	{ fscanf(pFile, "%s", &pParts->aTexturePath); }	// �t�@�C����
						else if (!strcmp(aDataSearch, "CUT_SCALE:"))	{ LoadScale(pFile, &pParts->cutScale); }		// �؂���{��
						else if (!strcmp(aDataSearch, "MOVE:"))			{ LoadVector(pFile, &pParts->move); }			// �ړ���
					}
				}
			}
		}
	}
}

//========================================
// InitBg_result_00�֐� - BG:���U���g[00] �̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitBg_result_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^
	Bg_result_00Control	*pCtl		// BG:���U���g[00] �̊Ǘ����̃|�C���^
						= &g_bg_result_00Control;
	Bg_result_00Parts	*pParts		// BG:���U���g[00] �̕��i���̏��̃|�C���^
						= g_aBg_result_00Parts;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntParts = 0; nCntParts < pCtl->nPartNum; nCntParts++)
	{
		D3DXCreateTextureFromFile(pDevice, pParts[nCntParts].aTexturePath, &g_aTextureBg_result_00[nCntParts]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * pCtl->nPartNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg_result_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_result_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < pCtl->nPartNum; nCntParts++, pVtx += 4)
	{
		// �p�����[�^�[�̏���������
		InitParameterBg_result_00(pParts);

		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			ANGLE_TYPE_FIXED);

		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, INITCOLOR);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx, 0, 1, 1, NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg_result_00->Unlock();
}

//========================================
// UninitBg_result_00�֐� - BG:���U���g[00] �̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitBg_result_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < BG_RESULT_00_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureBg_result_00[nCntParts] != NULL)
		{
			g_aTextureBg_result_00[nCntParts]->Release();
			g_aTextureBg_result_00[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg_result_00 != NULL) 
	{
		g_pVtxBuffBg_result_00->Release();
		g_pVtxBuffBg_result_00 = NULL;
	}
}

//========================================
// UpdateBg_result_00�֐� - BG:���U���g[00] �̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateBg_result_00(void)
{
	VERTEX_2D			*pVtx;	// ���_���ւ̃|�C���^
	Bg_result_00Control	*pCtl	// BG:���U���g[00] �̊Ǘ����̃|�C���^
						= &g_bg_result_00Control;
	Bg_result_00Parts	*pParts	// BG:���U���g[00] �̕��i���̏��̃|�C���^
						= g_aBg_result_00Parts;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg_result_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < pCtl->nPartNum; nCntParts++, pVtx += 4, pParts++)
	{
		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2DCut(pVtx, pParts->cutScale, pParts->pos);

		// �ʒu���X�V
		pParts->pos += pParts->move;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg_result_00->Unlock();
}

//========================================
// DrawBg_result_00�֐� - BG:���U���g[00] �̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawBg_result_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	Bg_result_00Control	*pCtl		// BG:���U���g[00] �̊Ǘ����̃|�C���^
						= &g_bg_result_00Control;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg_result_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntParts = 0; nCntParts < pCtl->nPartNum; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureBg_result_00[nCntParts]);

		// BG:���U���g[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntParts, 2);
	}
}
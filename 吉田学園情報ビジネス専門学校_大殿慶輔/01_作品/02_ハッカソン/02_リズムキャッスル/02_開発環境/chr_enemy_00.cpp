//========================================
// 
// CHR:�G[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chr_enemy_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "text.h"
#include "player.h"
#include "chr_enemy_00.h"	// CHR:�G	[00]
#include "obj_castle_00.h"	
#include "ui_move-text_00.h"

//****************************************
// �}�N����`
//****************************************
// CHR:�G[00] �̐����f�[�^�t�@�C���̑��΃p�X
#define CHR_ENEMY_00_DATA_FILE_PATH	"data\\GAMEDATA\\CHARACTER\\CHR_ENEMY_00_GENERATOR.txt"

// CHR:�G[00] �̍ő吔
// CHR:�G[00] �̎�ނ̍ő吔
#define CHR_ENEMY_00_TYPE_MAX	(4)
#define CHR_ENEMY_00_SET_MAX	(256)

// CHR:�G[00] �̃A�j���[�V�����ɂ����鎞��
#define CHR_ENEMY_00_ANIM_TIME	(4)

// CHR:�G[00] �̊J�n�ʒu/�I���ʒu
#define CHR_ENEMY_00_START_POS	D3DXVECTOR3((SCREEN_WIDTH*0.5f)+300.0f,SCREEN_HEIGHT-130.0f,0.0f)
#define CHR_ENEMY_00_END_POS	D3DXVECTOR3((SCREEN_WIDTH*0.5f)-280.0f,SCREEN_HEIGHT-130.0f,0.0f)

//****************************************
// �\���̂̒�`
//****************************************
// �ݒ���
typedef struct 
{
	int nTime;	// ����
	int nType;	// ���
}Chr_enemy_00Set;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// CHR:�G[00] �̃p�����[�^�[����������
void InitParameterChr_enemy_00(Chr_enemy_00 *pChr);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureChr_enemy_00	// �e�N�X�`���ւ̃|�C���^
						[CHR_ENEMY_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChr_enemy_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Chr_enemy_00			g_aChr_enemy_00			// CHR:�G[00] �̏��
						[CHR_ENEMY_00_MAX];
Chr_enemy_00Set			g_aChr_enemy_00Set		// �ݒ���
						[CHR_ENEMY_00_SET_MAX];
int g_nSetCounter;
int g_nSetNum = 0;

// CHR:�G[00] �̎�ޖ��̏��
Chr_enemy_00Type g_aChr_enemy_00Type[CHR_ENEMY_00_TYPE_MAX] =
{
	{ "data\\TEXTURE\\Enemy\\Slime-Biue.png"	,120.0f,60.0f,11,1,300,CHR_ENEMY_00_ELEM_X },
	{ "data\\TEXTURE\\Enemy\\Slime-Yellow.png"	,120.0f,60.0f,11,1,300,CHR_ENEMY_00_ELEM_Y },
	{ "data\\TEXTURE\\Enemy\\Slime-Green.png"	,120.0f,60.0f,11,1,300,CHR_ENEMY_00_ELEM_A },
	{ "data\\TEXTURE\\Enemy\\Slime-Red.png"		,120.0f,60.0f,11,1,300,CHR_ENEMY_00_ELEM_B },
};

//========== *** CHR:�G[00] �̏����擾 ***
Chr_enemy_00 *GetChr_enemy_00(void)
{
	return g_aChr_enemy_00;
}

//========== *** CHR:�G[00] �̎�ޖ��̏����擾 ***
Chr_enemy_00Type *GetChr_enemy_00Type(void)
{
	return g_aChr_enemy_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterChr_enemy_00�֐� - CHR:�G[00] �̃p�����[�^�[���������� -
//========================================
void InitParameterChr_enemy_00(Chr_enemy_00 *pChr)
{
	pChr->nType = 0;
	pChr->bUse = false;
	pChr->nCounterMove = 0;	// �ړ��J�E���^�[
	pChr->nPtn = 0;			// �p�^�[��No.
	pChr->nCounterAnim = 0;	// �A�j���[�V�����J�E���^�[
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadChr_enemy_00�֐� - CHR:�G[00] �̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadChr_enemy_00(void)
{
	FILE			*pFile;		// �t�@�C���|�C���^
	char			aDataSearch	// �f�[�^�����p
					[TXT_MAX];
	Chr_enemy_00Set	*pSet		// MD:���U���g���[00] �̏��̃|�C���^
					= g_aChr_enemy_00Set;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(CHR_ENEMY_00_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if		(!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }							// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "TIME:")) { 
			fscanf(pFile, "%d", &pSet->nTime);
			fscanf(pFile, "%d", &pSet->nType);
			pSet++;
			g_nSetNum++;
		}
	}
}

//========================================
// InitChr_enemy_00�֐� - CHR:�G[00] �̏��������� -
//========================================
void InitChr_enemy_00(void)
{
	g_nSetCounter = 0;

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntType = 0; nCntType < CHR_ENEMY_00_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aChr_enemy_00Type[nCntType].aTexturePath, &g_aTextureChr_enemy_00[nCntType]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CHR_ENEMY_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChr_enemy_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++,pVtx+=4)
	{
		Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��
							= &g_aChr_enemy_00Type[pChr->nType];

		// �p�����[�^�[�̏���������
		InitParameterChr_enemy_00(pChr);

		// rhw�̐ݒ�
		SetRHW2D(pVtx);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx, 0, pType->nPtnWidth, pType->nPtnHeight, NULL);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, INITCOLOR);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// UninitChr_enemy_00�֐� - CHR:�G[00] �̏I������ -
//========================================
void UninitChr_enemy_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntType = 0; nCntType < CHR_ENEMY_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureChr_enemy_00[nCntType] != NULL)
		{
			g_aTextureChr_enemy_00[nCntType]->Release();
			g_aTextureChr_enemy_00[nCntType] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffChr_enemy_00 != NULL) 
	{
		g_pVtxBuffChr_enemy_00->Release();
		g_pVtxBuffChr_enemy_00 = NULL;
	}
}

//========================================
// UpdateChr_enemy_00�֐� - CHR:�G[00] �̍X�V���� -
//========================================
void UpdateChr_enemy_00(void)
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00Set	*pSet	// MD:���U���g���[00] �̏��̃|�C���^
					= g_aChr_enemy_00Set;

	for (int nCntSet = 0; nCntSet < g_nSetNum; nCntSet++,pSet++)
	{
		//�t���[���P��
		if (g_nSetCounter == pSet->nTime - g_aChr_enemy_00Type[pSet->nType].nMoveTime)
		{// �ݒ莞�ԂɒB����
			SetChr_enemy_00(pSet->nType);
		}
	}

	g_nSetCounter++;

	Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++,pVtx+=4)
	{
		if (!pChr->bUse) 
		{
			continue;
		}

		Chr_enemy_00Type	*pType	// CHR:�G[00] �̎�ޖ��̏��
							= &g_aChr_enemy_00Type[pChr->nType];

		if (++pChr->nCounterMove >= pType->nMoveTime + 20)
		{//�G���ʂ�߂���
			Player *pPlayer = GetPlayer();
			pPlayer->nLife--;		//�̗͌��炷
			pPlayer->nCombo = 0;	//�R���{���Z�b�g
			pPlayer->nCounterComboMoveTime = 0;
			SetUi_moveText_00("MISS...", 1, pPlayer->pos, D3DXVECTOR3(0.0f, -3.0f, 0.0f), { 0,0,0,255 }, 60);	//���������ݒ�
			pChr->bUse = false;
			SetDamageObj_castle_00();	// ��̃_���[�W�ݒ�
		}
		
		float fRate = (float)pChr->nCounterMove / (float)pType->nMoveTime;
		D3DXVECTOR3 setPos = 
			(CHR_ENEMY_00_START_POS * (1.0f - fRate)) +
			(CHR_ENEMY_00_END_POS * fRate);

		pChr->pos = setPos;

		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			setPos,
			INITD3DXVECTOR3,
			false,
			pType->fWidth,
			pType->fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ菈��
		SetTexturePos2D(
			pVtx, 
			Count(
				&pChr->nPtn, 
				0, 
				(pType->nPtnWidth*pType->nPtnHeight)-1,
				&pChr->nCounterAnim,
				CHR_ENEMY_00_ANIM_TIME,
				COUNT_TYPE_TURN),
			pType->nPtnWidth, pType->nPtnHeight, NULL);

		{
			char aString[TXT_MAX];
			Color col;

			switch (pType->elem)
			{
			case CHR_ENEMY_00_ELEM_A:
				sprintf(aString, "A");
				col = { 131,211,19,255 };
				break;
			case CHR_ENEMY_00_ELEM_B:
				sprintf(aString, "B");
				col = { 219,43,0,255 };
				break;
			case CHR_ENEMY_00_ELEM_X:
				sprintf(aString, "X");
				col = { 0,115,239,255 };
				break;
			case CHR_ENEMY_00_ELEM_Y:
				sprintf(aString, "Y");
				col = { 243,191,63,255 };
				break;
			}

			SetText2D(aString, 2, DISPLAY_CENTER, setPos + D3DXVECTOR3(0.0,-8.0f,0.0f), col, 1.0f, 1.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// DrawChr_enemy_00�֐� - CHR:�G[00] �̕`�揈�� -
//========================================
void DrawChr_enemy_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffChr_enemy_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse)
		{
			continue;
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureChr_enemy_00[pChr->nType]);

		// CHR:�G[00] �̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntChr, 2);
	}
}

//========================================
// SetChr_enemy_00�֐� - CHR:�G[00] �̐ݒ菈�� -
//========================================
void SetChr_enemy_00(int nType) 
{
	Chr_enemy_00	*pChr	// CHR:�G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{
			continue;
		}

		pChr->bUse = true;
		pChr->nCounterMove = 0;
		pChr->nType = nType;

		break;
	}
}
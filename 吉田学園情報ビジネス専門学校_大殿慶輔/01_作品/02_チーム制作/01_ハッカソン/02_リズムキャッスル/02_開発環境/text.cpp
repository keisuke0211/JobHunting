//========================================
// 
// �e�L�X�g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** text.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "physics.h"
#include "polygon2D.h"
#include "polygon3D.h"
#include "text.h"

//****************************************
// �}�N����`
//****************************************
// �e�L�X�g�̕������̍ő吔
#define TEXT_DIGIT_MAX	(TXT_MAX)

// �t�H���g�̃f�[�^�t�@�C���̑��΃p�X
// �t�H���g�̍ő吔
#define FONT_DATA_FILE	"data\\GAMEDATA\\TEXT\\FONT_DATA.txt"
#define FONT_MAX		(8)

//****************************************
// �\���̂̒�`
//****************************************
// �t�H���g�̎�ޖ��̏��\����
typedef struct
{
	char	aTexturePath	// �e�N�X�`���t�@�C���̑��΃p�X
			[TXT_MAX];
	float	fWidth;			// ��
	float	fHeight;		// ����
	float	fSpaceX;		// �z�u�ԊuX
	float	fSpaceY;		// �z�u�ԊuY
	char	cStartChar;		// �J�n����
	int		nPtnWidth;		// �p�^�[����
	int		nPtnHeight;		// �p�^�[������
}Font;

// �e�L�X�g(2D)���\���̂��`
typedef struct
{
	char	aString	// ������
			[TXT_MAX];
	int		nFont;	// �t�H���g
}Text2D;

// �e�L�X�g(2D)�̊Ǘ����\���̂��`
typedef struct 
{
	int nUseNum;	// �g�p����Ă��鐔
}Text2DControl;

// �e�L�X�g(3D)���\���̂��`
typedef struct
{
	char		aString		// ������
				[TXT_MAX];
	int			nFont;		// �t�H���g
	D3DXVECTOR3 pos;		// �ʒu
	bool		bZtest;		// Z�e�X�g�t���O
	bool		bLighting;	// ���C�e�B���O�t���O
	bool		bBillboard;	// �r���{�[�h�t���O
}Text3D;

// �e�L�X�g(3D)�̊Ǘ����\���̂��`
typedef struct
{
	int nUseNum;	// �g�p����Ă��鐔
}Text3DControl;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �e�L�X�g(2D)�̃p�����[�^�[�̏���������
// �e�L�X�g(2D)�̊Ǘ����̃p�����[�^�[�̏���������
void InitParameterText2D(Text2D *pText);
void InitParameterText2DControl(void);

// �e�L�X�g(2D)�̏���������
// �e�L�X�g(2D)�̏I������
// �e�L�X�g(2D)�̕`�揈��
void InitText2D(void);
void UninitText2D(void);
void DrawText2D(void);

// �e�L�X�g(3D)�̃p�����[�^�[�̏���������
// �e�L�X�g(3D)�̊Ǘ����̃p�����[�^�[�̏���������
void InitParameterText3D(Text3D *pText);
void InitParameterText3DControl(void);

// �e�L�X�g(3D)�̏���������
// �e�L�X�g(3D)�̏I������
// �e�L�X�g(3D)�̕`�揈��
void InitText3D(void);
void UninitText3D(void);
void DrawText3D(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureText		// �e�N�X�`���ւ̃|�C���^
						[FONT_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffText2D 	// �e�L�X�g(2D)�̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
Text2D					g_text2D			// �e�L�X�g(2D)�̏��
						[TEXT2D_MAX];
Text2DControl			g_text2DControl;	// �e�L�X�g(2D)�̊Ǘ����
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffText3D 	// �e�L�X�g(3D)�̒��_�o�b�t�@�ւ̃|�C���^
						= NULL;
Text3D					g_text3D			// �e�L�X�g(3D)�̏��
						[TEXT2D_MAX];
Text3DControl			g_text3DControl;	// �e�L�X�g(3D)�̊Ǘ����
D3DXMATRIX				g_mtxWorldText3D;	// UI:�A�C�e���A�C�R��[00] �̃��[���h�}�g���b�N�X
Font					g_aFont				// �t�H���g���̏��
						[FONT_MAX];

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadFont�֐� - �t�H���g�̓ǂݍ��ݏ��� -
//========================================
void LoadFont(void) 
{
	FILE	*pFile;		// �t�@�C���|�C���^
	char	aDataSearch	// �f�[�^�����p
			[TXT_MAX];
	Font	*pFont		// �t�H���g���̏��̃|�C���^
			= g_aFont;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(FONT_DATA_FILE, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (!strcmp(aDataSearch, "END")) { fclose(pFile); break; }	// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "FONTSET"))
		{// ���i�̐ݒ���̓ǂݍ��݂��J�n
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// ����

				if		(!strcmp(aDataSearch, "FONTSET_END")) { break; }	// �ǂݍ��݂��I��
				else if (!strcmp(aDataSearch, "FONT"))
				{// ���i���̏��̓ǂݍ��݂��J�n
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	// ����

						if (!strcmp(aDataSearch, "FONT_END")) {
							pFont++;	// �|�C���^��i�߂�
							break;
						}
						else if (!strcmp(aDataSearch, "TEXTURE_PATH:")) { fscanf(pFile, "%s", &pFont->aTexturePath); }	// �t�@�C����
						else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pFont->fWidth); }		// ��
						else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pFont->fHeight); }		// ����
						else if (!strcmp(aDataSearch, "SPACE_X:"))		{ fscanf(pFile, "%f", &pFont->fSpaceX); }		// �z�u�ԊuX
						else if (!strcmp(aDataSearch, "SPACE_Y:"))		{ fscanf(pFile, "%f", &pFont->fSpaceY); }		// �z�u�ԊuY
						else if (!strcmp(aDataSearch, "START_CHAR:"))	{ fscanf(pFile, "%s", &pFont->cStartChar); }	// �J�n����
						else if (!strcmp(aDataSearch, "PTN_WIDTH:"))	{ fscanf(pFile, "%d", &pFont->nPtnWidth); }		// �p�^�[����
						else if (!strcmp(aDataSearch, "PTN_HEIGHT:"))	{ fscanf(pFile, "%d", &pFont->nPtnHeight); }	// �p�^�[������
					}
				}
			}
		}
	}
}

//============================================================
//-----|------------------------------------------------------
//=====| �e�L�X�g(2D)
//-----|------------------------------------------------------
//============================================================

//========================================
// InitParameterText2D�֐� - �e�L�X�g(2D)�̃p�����[�^�[�̏��������� -
//========================================
void InitParameterText2D(Text2D *pText) 
{
	pText->nFont = (int)0;	// �t�H���g
}

//========================================
// InitParameterText2DControl�֐� - �e�L�X�g(2D)�̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterText2DControl(void)
{
	Text2DControl	*pTxtCtl	// �e�L�X�g(2D)�̊Ǘ����̃|�C���^
					= &g_text2DControl;

	pTxtCtl->nUseNum = 0;	// �g�p����Ă��鐔
}

//========================================
// InitText2D�֐� - �e�L�X�g(2D)�̏��������� -
//========================================
void InitText2D(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TEXT2D_MAX * TEXT_DIGIT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffText2D,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffText2D->Lock(0, 0, (void**)&pVtx, 0);

	// �Ǘ����̃p�����[�^�[�̏���������
	InitParameterText2DControl();

	Text2D	*pTxt	// �e�L�X�g(2D)�̏��̃|�C���^
			= g_text2D;

	for (int nCntText = 0; nCntText < TEXT2D_MAX; nCntText++, pTxt++)
	{
		// �p�����[�^�[�̏���������
		InitParameterText2D(pTxt);

		for (int nCntDigit = 0; nCntDigit < TEXT_DIGIT_MAX; nCntDigit++, pVtx += 4)
		{
			// rhw�̐ݒ�
			SetRHW2D(pVtx);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffText2D->Unlock();
}

//========================================
// UninitText2D�֐� - �e�L�X�g(2D)�̏I������ -
//========================================
void UninitText2D(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffText2D != NULL)
	{
		g_pVtxBuffText2D->Release();
		g_pVtxBuffText2D = NULL;
	}
}

//========================================
// DrawText2D�֐� - �e�L�X�g2D�̕`�揈�� -
//========================================
void DrawText2D(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffText2D, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Text2D			*pTxt		// �e�L�X�g�̏��̃|�C���^
					= g_text2D;
	Text2DControl	*pTxtCtl	// �e�L�X�g(2D)�̊Ǘ����̃|�C���^
					= &g_text2DControl;

	for (int nCntTxt = 0; nCntTxt < pTxtCtl->nUseNum; nCntTxt++, pTxt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureText[pTxt->nFont]);

		for (int nCntDigit = 0; nCntDigit < strlen(pTxt->aString); nCntDigit++)
		{
			if ((nCntDigit < strlen(pTxt->aString)) && pTxt->aString[nCntDigit] != ' ')
			{// ���̃J�E���g��������̌�����������Ă��� & ���̕������󔒂Ŗ������A
				// �e�L�X�g�̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntTxt * TEXT_DIGIT_MAX) + nCntDigit) * 4, 2);
			}
		}
	}

	pTxtCtl->nUseNum = 0;	// �g�p���Ă��鐔������������
}

//============================================================
//-----|------------------------------------------------------
//=====| �e�L�X�g(3D)
//-----|------------------------------------------------------
//============================================================

//========================================
// InitParameterText3D�֐� - �e�L�X�g(3D)�̃p�����[�^�[�̏��������� -
//========================================
void InitParameterText3D(Text3D *pText)
{
	pText->nFont	= (int)0;							// �t�H���g
	pText->pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pText->bZtest	= false;							// Z�e�X�g�t���O
}

//========================================
// InitParameterText3DControl�֐� - �e�L�X�g(3D)�̊Ǘ����̃p�����[�^�[�̏��������� -
//========================================
void InitParameterText3DControl(void)
{
	Text3DControl	*pTxtCtl	// �e�L�X�g(3D)�̊Ǘ����̃|�C���^
					= &g_text3DControl;

	pTxtCtl->nUseNum = 0;	// �g�p����Ă��鐔
}

//========================================
// InitText3D�֐� - �e�L�X�g(3D)�̏��������� -
//========================================
void InitText3D(void)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * TEXT3D_MAX * TEXT_DIGIT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffText3D,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	// �Ǘ����̃p�����[�^�[�̏���������
	InitParameterText3DControl();

	Text3D	*pTxt	// �e�L�X�g(3D)�̏��̃|�C���^
		= g_text3D;

	for (int nCntText = 0; nCntText < TEXT3D_MAX; nCntText++, pTxt++)
	{
		// �p�����[�^�[�̏���������
		InitParameterText3D(pTxt);
	}
}

//========================================
// UninitText3D�֐� - �e�L�X�g(3D)�̏I������ -
//========================================
void UninitText3D(void)
{
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffText3D != NULL)
	{
		g_pVtxBuffText3D->Release();
		g_pVtxBuffText3D = NULL;
	}
}

//========================================
// DrawText3D�֐� - �e�L�X�g3D�̕`�揈�� -
//========================================
void DrawText3D(void)
{
	LPDIRECT3DDEVICE9	pDevice		// �f�o�C�X�擾
						= GetDevice();
	D3DXMATRIX			mtxRot,		// �v�Z�p�}�g���b�N�X
						mtxTrans;
	D3DXMATRIX			mtxView;	// �r���[�}�g���b�N�X�擾�p

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffText3D, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	Text3D			*pTxt		// �e�L�X�g�̏��̃|�C���^
					= g_text3D;
	Text3DControl	*pTxtCtl	// �e�L�X�g(3D)�̊Ǘ����̃|�C���^
					= &g_text3DControl;

	for (int nCntTxt = 0; nCntTxt < pTxtCtl->nUseNum; nCntTxt++, pTxt++)
	{
		if (!pTxt->bZtest) 
		{// Z�e�X�g�t���O���U�̎��A
			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureText[pTxt->nFont]);

		for (int nCntDigit = 0; nCntDigit < strlen(pTxt->aString); nCntDigit++)
		{
			if ((nCntDigit < strlen(pTxt->aString)) && pTxt->aString[nCntDigit] != ' ')
			{// ���̃J�E���g��������̌�����������Ă��� & ���̕������󔒂Ŗ������A
				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_mtxWorldText3D);

				// �r���[�}�g���b�N�X���擾
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				if (pTxt->bBillboard) 
				{// �r���{�[�h�t���O���^�̎��A
					// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
					D3DXMatrixInverse(&g_mtxWorldText3D, NULL, &mtxView);
					g_mtxWorldText3D._41 = 0.0f;
					g_mtxWorldText3D._42 = 0.0f;
					g_mtxWorldText3D._43 = 0.0f;
				}
				else
				{// �r���{�[�h�t���O���U�̎��A
					// �����𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxRot, FindAngleLookDown(GetCamera3D()->posV, GetCamera3D()->posR), 0.0f, 0.0f);
					D3DXMatrixMultiply(&g_mtxWorldText3D, &g_mtxWorldText3D, &mtxRot);
				}

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, pTxt->pos.x, pTxt->pos.y, pTxt->pos.z);
				D3DXMatrixMultiply(&g_mtxWorldText3D, &g_mtxWorldText3D, &mtxTrans);

				// ���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldText3D);

				// �e�L�X�g�̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntTxt * TEXT_DIGIT_MAX) + nCntDigit) * 4, 2);
			}
		}

		if (!pTxt->bZtest)
		{// Z�e�X�g�t���O���U�̎��A
			// Z�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}

	pTxtCtl->nUseNum = 0;	// �g�p���Ă��鐔������������

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitText�֐� - �e�L�X�g�̏��������� -
//========================================
void InitText(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aFont[nCntFont].aTexturePath, &g_pTextureText[nCntFont]);
	}

	// ����������
	InitText3D();	// �e�L�X�g(3D)
	InitText2D();	// �e�L�X�g(2D)
}

//========================================
// UninitText�֐� - �e�L�X�g�̏I������ -
//========================================
void UninitText(void)
{
	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureText[nCntFont] != NULL)
		{
			g_pTextureText[nCntFont]->Release();
			g_pTextureText[nCntFont] = NULL;
		}
	}

	// �I������
	UninitText3D();	// �e�L�X�g(3D)
	UninitText2D();	// �e�L�X�g(2D)
}

//========================================
// DrawText_�֐� - �e�L�X�g�̕`�揈�� -
//========================================
void DrawText_(void)
{
	// �`�揈��
	DrawText3D();	// �e�L�X�g(3D)
	DrawText2D();	// �e�L�X�g(2D)
}

//============================================================
//-----|------------------------------------------------------
//=====| �e�L�X�g(2D)
//-----|------------------------------------------------------
//============================================================

//========================================
// SetText2D�֐� - �e�L�X�g(2D)�̐ݒ菈�� -
//========================================
void SetText2D(char *pString, int nFont, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight)
{
	VERTEX_2D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffText2D->Lock(0, 0, (void**)&pVtx, 0);

	Text2DControl	*pTxtCtl	// �e�L�X�g(2D)�̊Ǘ����̃|�C���^
					= &g_text2DControl;
	Text2D			*pTxt		// �e�L�X�g(2D)�̏��̃|�C���^
					= &g_text2D[pTxtCtl->nUseNum];

	strcpy(pTxt->aString, pString);	// ���������
	pTxt->nFont = nFont;			// �t�H���g����

	pVtx += 4 * pTxtCtl->nUseNum * TEXT_DIGIT_MAX;
	for (int nCntDigit = 0; nCntDigit < strlen(pString); nCntDigit++, pVtx += 4)
	{
		D3DXVECTOR3 setPos = pos;	// �ݒ�ʒu

		// �\���`���ɉ������ݒ�ʒu�̐ݒ�
		switch (disp)
		{
		case /*/ �������� /*/DISPLAY_CENTER:
			setPos.x -= (((g_aFont[nFont].fSpaceX * PIXEL) * fWidth) * strlen(pString)) * 0.5f;		// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ����̔��������Z
			setPos.x += ((float)nCntDigit + 0.5f) * ((g_aFont[nFont].fSpaceX * PIXEL) * fWidth);	// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		case /*/ ���l�� /*/DISPLAY_LEFT:
			setPos.x += nCntDigit * ((g_aFont[nFont].fSpaceX * PIXEL) * fWidth);	// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		case /*/ �E�l�� /*/DISPLAY_RIGHT:
			setPos.x -= ((g_aFont[nFont].fSpaceX * PIXEL) * fWidth) * (strlen(pString) - 1);	// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ��������Z
			setPos.x += nCntDigit * ((g_aFont[nFont].fSpaceX * PIXEL) * fWidth);				// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			break;
		}

		// ���_���W��ݒ�
		SetVertexPos2D(pVtx,
			setPos,
			INITD3DXVECTOR3,
			false,
			(g_aFont[nFont].fWidth * PIXEL) * fWidth,
			(g_aFont[nFont].fHeight * PIXEL) * fHeight,
			ANGLE_TYPE_FIXED);

		// ���_�J���[�̐ݒ�
		SetVertexColor2D(pVtx, col);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos2D(pVtx,
			pString[nCntDigit] - g_aFont[nFont].cStartChar,
			g_aFont[nFont].nPtnWidth,
			g_aFont[nFont].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffText2D->Unlock();

	pTxtCtl->nUseNum++;	// �g�p����Ă��鐔�����Z
}

//============================================================
//-----|------------------------------------------------------
//=====| �e�L�X�g(3D)
//-----|------------------------------------------------------
//============================================================

//========================================
// SetText3D�֐� - �e�L�X�g(3D)�̐ݒ菈�� -
//========================================
void SetText3D(char *pString, int nFont, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard)
{
	VERTEX_3D	*pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffText3D->Lock(0, 0, (void**)&pVtx, 0);

	Text3DControl	*pTxtCtl	// �e�L�X�g(3D)�̊Ǘ����̃|�C���^
					= &g_text3DControl;
	Text3D			*pTxt		// �e�L�X�g(3D)�̏��̃|�C���^
					= &g_text3D[pTxtCtl->nUseNum];

	strcpy(pTxt->aString, pString);	// ���������
	pTxt->nFont = nFont;			// �t�H���g����
	pTxt->pos = pos;				// �ʒu����
	pTxt->bZtest = bZtest;			// Z�e�X�g�t���O����
	pTxt->bLighting = bLighting;	// ���C�e�B���O�t���O����
	pTxt->bBillboard = bBillboard;	// �r���{�[�h�t���O����

	pVtx += 4 * pTxtCtl->nUseNum * TEXT_DIGIT_MAX;
	for (int nCntDigit = 0; nCntDigit < strlen(pString); nCntDigit++, pVtx += 4)
	{
		D3DXVECTOR3 setPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ݒ�ʒu

		// �\���`���ɉ������ݒ�ʒu�̐ݒ�
		switch (disp)
		{
		case /*/ �������� /*/DISPLAY_CENTER:
			// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ����̔��������Z
			setPos.x -= (((g_aFont[nFont].fSpaceX * PIXEL3D) * fWidth) * strlen(pString)) * 0.5f;

			// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			setPos.x += ((float)nCntDigit + 0.5f) * ((g_aFont[nFont].fSpaceX * PIXEL3D) * fWidth);
			break;
		case /*/ ���l�� /*/DISPLAY_LEFT:
			// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			setPos.x += nCntDigit * ((g_aFont[nFont].fSpaceX * PIXEL3D) * fWidth);
			break;
		case /*/ �E�l�� /*/DISPLAY_RIGHT:
			// �ݒ�ʒu�ɕ����̍ŏI�I�Ȓ��������Z
			setPos.x -= ((g_aFont[nFont].fSpaceX * PIXEL3D) * fWidth) * (strlen(pString) - 1);

			// �ݒ�ʒu�ɔz�u�ԊuX��K�p(���Z)
			setPos.x += nCntDigit * ((g_aFont[nFont].fSpaceX * PIXEL3D) * fWidth);
			break;
		}

		float fCWidth	= (g_aFont[nFont].fWidth * PIXEL3D) * fWidth;
		float fCHeight	= (g_aFont[nFont].fHeight * PIXEL3D) * fHeight;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(setPos.x - (fCWidth * 0.5f), fCHeight + (fCHeight * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(setPos.x + (fCWidth * 0.5f), fCHeight + (fCHeight * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(setPos.x - (fCWidth * 0.5f), fCHeight - (fCHeight * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(setPos.x + (fCWidth * 0.5f), fCHeight - (fCHeight * 0.5f), 0.0f);

		// ���_�J���[�̐ݒ�
		SetVertexColor3D(pVtx, col);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePos3D(pVtx,
			pString[nCntDigit] - g_aFont[nFont].cStartChar,
			g_aFont[nFont].nPtnWidth,
			g_aFont[nFont].nPtnHeight,
			NULL);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffText3D->Unlock();

	pTxtCtl->nUseNum++;	// �g�p����Ă��鐔�����Z
}
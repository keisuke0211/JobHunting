//========================================
// 
// �e�L�X�g�̏���
// Author:���� ����
// 
//========================================
//  *** text.cpp ***
//========================================
#include "main.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// �}�N����`
//****************************************
#define TEXT_DIGIT_MAX	(128)	// �e�L�X�g�̍ő啶����

//****************************************
// �\���̂̒�`
//****************************************

// �e�L�X�g���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	bool bUse;				// �g���Ă��邩�t���O
	char aString[TXT_MAX];	// �\�����镶����
	TEXT_MODE mode;			// ���[�h
	FONT font;				// �t�H���g
}Txt_00;

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9 g_pTextureTxt_00[FONT_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 pVtxBuffTxt_00 = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Txt_00 g_text[TEXT_MAX];							// �e�L�X�g�̏��

// �t�H���g�̏��
Font g_aFont[FONT_MAX] =
{
	{ "data\\Texture\\txt_font_000.png"		,PIXEL * 8	,PIXEL * 8	,PIXEL * 8	,PIXEL * 8	, '!' , 8 , 8 },	// �t�H���g[00]
	{ "data\\Texture\\txt_font_001.png"		,PIXEL * 16	,PIXEL * 16 ,PIXEL * 16 ,PIXEL * 16	, '!' , 8 , 8 },	// �t�H���g[01]
	{ "data\\Texture\\txt_font_002.png"		,PIXEL * 4	,PIXEL * 8  ,PIXEL * 4  ,PIXEL * 8	, '!' , 8 , 8 },	// �t�H���g[02]
	{ "data\\Texture\\txt_font_003.png"		,PIXEL * 16	,PIXEL * 16 ,PIXEL * 16 ,PIXEL * 16 , '!' , 8 , 8 },	// �t�H���g[03]
	{ "data\\Texture\\txt_number_000.png"	,PIXEL * 64	,PIXEL * 64 ,PIXEL * 64 ,PIXEL * 64 , '0' , 8 , 2 },	// �t�H���g[04]
};

// �e�L�X�g�̎g�p�̈�̒l
int g_aTextField[TEXT_FIELD_MAX] = { 20,88,20 };

//========== *** �t�H���g�̏����擾 ***
Font *GetFont(void) 
{
	return g_aFont;
}

//========================================
// InitTxt_00�֐� - �e�L�X�g�̏��������� -
//========================================
void InitTxt_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	POINT vertexPos[4];			// �l�p�`�̒��_�̍��W
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++) 
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aFont[nCntFont].aPath, &g_pTextureTxt_00[nCntFont]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TEXT_MAX * TEXT_DIGIT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuffTxt_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuffTxt_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt_00 = 0; nCntTxt_00 < TEXT_MAX; nCntTxt_00++) 
	{
		g_text[nCntTxt_00].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu������������
		g_text[nCntTxt_00].bUse = false;						// �g�p����Ă��Ȃ���Ԃɂ���
		sprintf(g_text[nCntTxt_00].aString, "0");				// �\�����镶�����������

		for (int nCntDigit = 0; nCntDigit < TEXT_DIGIT_MAX; nCntDigit++, pVtx += 4)
		{
			float fWidth	= g_aFont[0].fWidth;	// ��
			float fHeight	= g_aFont[0].fHeight;	// ����
			float fSpaceX	= g_aFont[0].fSpaceX;	// �z�u�ԊuX
			float fSpaceY	= g_aFont[0].fSpaceY;	// �z�u�ԊuY

			// ���_�̍��W����
			vertexPos[0].x = g_text[nCntTxt_00].pos.x - (fWidth * 0.5f) + (nCntDigit * fSpaceX);
			vertexPos[0].y = g_text[nCntTxt_00].pos.y - fHeight * 0.5f;
			vertexPos[1].x = g_text[nCntTxt_00].pos.x + (fWidth * 0.5f) + (nCntDigit * fSpaceX);
			vertexPos[1].y = g_text[nCntTxt_00].pos.y - fHeight * 0.5f;
			vertexPos[2].x = g_text[nCntTxt_00].pos.x - (fWidth * 0.5f) + (nCntDigit * fSpaceX);
			vertexPos[2].y = g_text[nCntTxt_00].pos.y + fHeight * 0.5f;
			vertexPos[3].x = g_text[nCntTxt_00].pos.x + (fWidth * 0.5f) + (nCntDigit * fSpaceX);
			vertexPos[3].y = g_text[nCntTxt_00].pos.y + fHeight * 0.5f;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(vertexPos[0].x, vertexPos[0].y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(vertexPos[1].x, vertexPos[1].y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(vertexPos[2].x, vertexPos[2].y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(vertexPos[3].x, vertexPos[3].y, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2((g_text[nCntTxt_00].aString[0] % g_aFont[0].nPtnMaxX) * (1.0f / g_aFont[0].nPtnMaxX), ((g_text[nCntTxt_00].aString[0] / g_aFont[0].nPtnMaxX)) * (1.0f / g_aFont[0].nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((g_text[nCntTxt_00].aString[0] % g_aFont[0].nPtnMaxX) * (1.0f / g_aFont[0].nPtnMaxX) + (1.0f / g_aFont[0].nPtnMaxX), ((g_text[nCntTxt_00].aString[0] / g_aFont[0].nPtnMaxX)) * (1.0f / g_aFont[0].nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((g_text[nCntTxt_00].aString[0] % g_aFont[0].nPtnMaxX) * (1.0f / g_aFont[0].nPtnMaxX), (((g_text[nCntTxt_00].aString[0] / g_aFont[0].nPtnMaxX)) * (1.0f / g_aFont[0].nPtnMaxY)) + (1.0f / g_aFont[0].nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((g_text[nCntTxt_00].aString[0] % g_aFont[0].nPtnMaxX) * (1.0f / g_aFont[0].nPtnMaxX) + (1.0f / g_aFont[0].nPtnMaxX), (((g_text[nCntTxt_00].aString[0] / g_aFont[0].nPtnMaxX)) * (1.0f / g_aFont[0].nPtnMaxY)) + (1.0f / g_aFont[0].nPtnMaxY));
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuffTxt_00->Unlock();
}

//========================================
// UninitTxt_00�֐� - �e�L�X�g�̏I������ -
//========================================
void UninitTxt_00(void)
{
	int nCntTxt_00;	// �e�L�X�g�̃J�E���g
	int nCntDigit;	// ���̃J�E���g

	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureTxt_00[nCntFont] != NULL)
		{
			g_pTextureTxt_00[nCntFont]->Release();
			g_pTextureTxt_00[nCntFont] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (pVtxBuffTxt_00 != NULL)
	{
		pVtxBuffTxt_00->Release();
		pVtxBuffTxt_00 = NULL;
	}
}

//========================================
// UpdateTxt_00�֐� - �e�L�X�g�̍X�V���� -
//========================================
void UpdateTxt_00(TEXT_FIELD textField, int nNumber, char aString[TXT_MAX], D3DXVECTOR3 pos, Color col, TEXT_MODE mode, FONT font, float fTextWidth, float fTextHeight)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^
	int nPattern;				// �p�^�[��No.
	int nStartNumber = 0;		// �J�n�ԍ�

	for (int nCntField = 0; nCntField < textField; nCntField++)
	{
		// �J�n�ԍ��ɗ̈�̒l�����Z
		nStartNumber += g_aTextField[nCntField];
	}

	// �e�L�X�g�ԍ������킹��
	nNumber += nStartNumber;

	// �t�H���g����
	g_text[nNumber].font = font;

	// �\�����镶�������
	sprintf(g_text[nNumber].aString, "%s", aString);

	// �ʒu����
	g_text[nNumber].pos = pos;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuffTxt_00->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̃|�C���^���Y���̈ʒu�܂Ői�߂�
	pVtx += nNumber * TEXT_DIGIT_MAX * 4;

	for (int nCntDigit = 0; nCntDigit < strlen(aString); nCntDigit++, pVtx += 4)
	{
		D3DXVECTOR3 pos = g_text[nNumber].pos;					// �ʒu
		float fWidth	= g_aFont[font].fWidth * fTextWidth;	// ��
		float fHeight	= g_aFont[font].fHeight * fTextHeight;	// ����
		float fSpaceX	= g_aFont[font].fSpaceX * fTextWidth;	// �z�u�ԊuX
		float fSpaceY	= g_aFont[font].fSpaceY * fTextHeight;	// �z�u�ԊuY

		switch (mode)
		{
		case TEXT_MODE_LEFT: // ���l��

			// ���_�̍��W����
			pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth, pos.y - fHeight * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX), pos.y + fHeight * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth, pos.y + fHeight * 0.5f, 0.0f);

			break;
		case TEXT_MODE_RIGHT: // �E�l��

			// ���_�̍��W����
			pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) - (strlen(aString) * fSpaceX), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth - (strlen(aString) * fSpaceX), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) - (strlen(aString) * fSpaceX), pos.y + fHeight * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth - (strlen(aString) * fSpaceX), pos.y + fHeight * 0.5f, 0.0f);

			break;
		case TEXT_MODE_CENTER: // ���S

			// ���_�̍��W����
			pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) - (strlen(aString) * fSpaceX * 0.5f), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth - (strlen(aString) * fSpaceX * 0.5f), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) - (strlen(aString) * fSpaceX * 0.5f), pos.y + fHeight * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth - (strlen(aString) * fSpaceX * 0.5f), pos.y + fHeight * 0.5f, 0.0f);

			break;
		}

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
		pVtx[1].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
		pVtx[2].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
		pVtx[3].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(((aString[nCntDigit] - g_aFont[font].cStartChar) % g_aFont[font].nPtnMaxX) * (1.0f / g_aFont[font].nPtnMaxX), 
			(((aString[nCntDigit] - g_aFont[font].cStartChar) / g_aFont[font].nPtnMaxX)) * (1.0f / g_aFont[font].nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2(((aString[nCntDigit] - g_aFont[font].cStartChar) % g_aFont[font].nPtnMaxX) * (1.0f / g_aFont[font].nPtnMaxX) + (1.0f / g_aFont[font].nPtnMaxX), 
			(((aString[nCntDigit] - g_aFont[font].cStartChar) / g_aFont[font].nPtnMaxX)) * (1.0f / g_aFont[font].nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2(((aString[nCntDigit] - g_aFont[font].cStartChar) % g_aFont[font].nPtnMaxX) * (1.0f / g_aFont[font].nPtnMaxX), 
			((((aString[nCntDigit] - g_aFont[font].cStartChar) / g_aFont[font].nPtnMaxX)) * (1.0f / g_aFont[font].nPtnMaxY)) + (1.0f / g_aFont[font].nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2(((aString[nCntDigit] - g_aFont[font].cStartChar) % g_aFont[font].nPtnMaxX) * (1.0f / g_aFont[font].nPtnMaxX) + (1.0f / g_aFont[font].nPtnMaxX), 
			((((aString[nCntDigit] - g_aFont[font].cStartChar) / g_aFont[font].nPtnMaxX)) * (1.0f / g_aFont[font].nPtnMaxY)) + (1.0f / g_aFont[font].nPtnMaxY));
	}

	// ���_���W���A�����b�N����
	pVtxBuffTxt_00->Unlock();
}

//========================================
// DrawTxt_00�֐� - �e�L�X�g�̕`�揈�� -
//========================================
void DrawTxt_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	int nCntTxt_00;				// �e�L�X�g�̃J�E���g
	int nCntDigit;				// ���̃J�E���g
	char aString[TXT_MAX];		// �e�L�X�g�̕�������i�[

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, pVtxBuffTxt_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTxt_00 = 0; nCntTxt_00 < TEXT_MAX; nCntTxt_00++) 
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTxt_00[g_text[nCntTxt_00].font]);

		for (nCntDigit = 0; nCntDigit < TEXT_DIGIT_MAX; nCntDigit++) 
		{
			if ((g_text[nCntTxt_00].bUse && nCntDigit < strlen(g_text[nCntTxt_00].aString)) && g_text[nCntTxt_00].aString[nCntDigit] != ' ')
			{// �g�p����Ă����ԂŁA�����̃J�E���g���\������l�̌�����������Ă��鎞

				// �e�L�X�g�̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntTxt_00 * TEXT_DIGIT_MAX) + nCntDigit) * 4, 2);
			}
		}
	}
}

//========================================
// ShowTxt_00�֐� - �e�L�X�g�̕\��/��\����؂�ւ��� -
//========================================
void ShowTxt_00(TEXT_FIELD textField, int nNumber, bool bShow)
{
	int nStartNumber = 0;	// �J�n�ԍ�

	for (int nCntField = 0; nCntField < textField; nCntField++) 
	{
		// �J�n�ԍ��ɗ̈�̒l�����Z
		nStartNumber += g_aTextField[nCntField];
	}

	// �e�L�X�g�ԍ������킹��
	nNumber += nStartNumber;

	if (g_text[nNumber].bUse != bShow)
	{
		g_text[nNumber].bUse = bShow;
	}
}
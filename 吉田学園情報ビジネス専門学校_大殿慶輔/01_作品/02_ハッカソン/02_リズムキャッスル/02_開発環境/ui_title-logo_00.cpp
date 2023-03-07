//========================================
// 
// UI:�^�C�g�����S[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_title-logo_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"
#include "ui_title-logo_00.h"	// UI:�^�C�g�����S	[00]

//****************************************
// �}�N����`
//****************************************
// UI:�^�C�g�����S[00] �̃f�[�^�t�@�C���̑��΃p�X
#define UI_TITLELOGO_00_DATA_FILE_PATH	"data\\GAMEDATA\\USERINTERFACE\\UI_TITLELOGO_00_DATA.txt"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�^�C�g�����S[00] �̃p�����[�^�[����������
void InitParameterUi_titleLogo_00(void);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_titleLogo_00	// �e�N�X�`���ւ̃|�C���^
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_titleLogo_00	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;
Ui_titleLogo_00			g_ui_titleLogo_00;			// UI:�^�C�g�����S[00] �̕��i���̏��

//========== *** UI:�^�C�g�����S[00] �̏����擾 ***
Ui_titleLogo_00 *GetUi_titleLogo_00(void) 
{
	return &g_ui_titleLogo_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̃p�����[�^�[���������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterUi_titleLogo_00(void)
{
	Ui_titleLogo_00 *pUi	// UI:�^�C�g�����S[00] �̏��̃|�C���^
					= &g_ui_titleLogo_00;

	pUi->nCounterArrival = 0;	// �����J�E���^�[
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̓ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void LoadUi_titleLogo_00(void) 
{
	FILE			*pFile;		// �t�@�C���|�C���^
	char			aDataSearch	// �f�[�^�����p
					[TXT_MAX];
	Ui_titleLogo_00	*pUi		// UI:�^�C�g�����S[00] �̏��̃|�C���^
					= &g_ui_titleLogo_00;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(UI_TITLELOGO_00_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// END��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if		(!strcmp(aDataSearch, "END"))			{ fclose(pFile); break; }						// �ǂݍ��݂��I��
		else if (!strcmp(aDataSearch, "TEXTURE_PATH:")) { fscanf(pFile, "%s", &pUi->aTexturePath); }	// �t�@�C����
		else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pUi->fWidth); }			// ��
		else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pUi->fHeight); }			// ����
		else if (!strcmp(aDataSearch, "START_POS:"))	{ LoadVector(pFile, &pUi->startPos); }			// �J�n�ʒu
		else if (!strcmp(aDataSearch, "END_POS:"))		{ LoadVector(pFile, &pUi->endPos); }			// �I���ʒu
		else if (!strcmp(aDataSearch, "ARRIVAL_TIME:")) { fscanf(pFile, "%d", &pUi->nArrivalTime); }	// �����ɂ����鎞��
	}
}

//========================================
// InitUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitUi_titleLogo_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^
	Ui_titleLogo_00		*pUi		// UI:�^�C�g�����S[00] �̕��i���̏��̃|�C���^
						= &g_ui_titleLogo_00;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, pUi->aTexturePath, &g_aTextureUi_titleLogo_00);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_titleLogo_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_titleLogo_00->Lock(0, 0, (void**)&pVtx, 0);

	// �p�����[�^�[�̏���������
	InitParameterUi_titleLogo_00();

	// ���_���W��ݒ�
	SetVertexPos2D(pVtx,
		pUi->startPos,
		INITD3DXVECTOR3,
		false,
		pUi->fWidth,
		pUi->fHeight,
		ANGLE_TYPE_FIXED);

	// rhw�̐ݒ�
	SetRHW2D(pVtx);

	// ���_�J���[�̐ݒ�
	SetVertexColor2D(pVtx, INITCOLOR);

	// �e�N�X�`�����W�̐ݒ�
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_titleLogo_00->Unlock();
}

//========================================
// UninitUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitUi_titleLogo_00(void)
{
	// �e�N�X�`���̔j��
	if (g_aTextureUi_titleLogo_00 != NULL)
	{
		g_aTextureUi_titleLogo_00->Release();
		g_aTextureUi_titleLogo_00 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_titleLogo_00 != NULL) 
	{
		g_pVtxBuffUi_titleLogo_00->Release();
		g_pVtxBuffUi_titleLogo_00 = NULL;
	}
}

//========================================
// UpdateUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateUi_titleLogo_00(void)
{
	VERTEX_2D				*pVtx;	// ���_���ւ̃|�C���^
	Ui_titleLogo_00			*pUi	// UI:�^�C�g�����S[00] �̕��i���̏��̃|�C���^
							= &g_ui_titleLogo_00;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_titleLogo_00->Lock(0, 0, (void**)&pVtx, 0);

	if (pUi->nCounterArrival < pUi->nArrivalTime)
	{// �����J�E���^�[�������ɂ����鎞�ԂɒB���Ă��Ȃ����A
		if (GetKeyboardTrigger(DIK_RETURN)||GetButtonTrigger(BUTTON_A))
		{// ENTER�L�[�������ꂽ�u�ԁA�����J�E���^�[�ɏ������
			pUi->nCounterArrival = pUi->nArrivalTime;
		}
		else
		{// ENTER�L�[��������Ă��Ȃ����A�����J�E���^�[�����Z
			pUi->nCounterArrival++;
		}
	}

	// ���_���W��ݒ�
	{
		float	fRate	// �J�E���^�[�̐i�s��
				= (float)pUi->nCounterArrival / (float)pUi->nArrivalTime;

		SetVertexPos2D(pVtx,
			(pUi->startPos * (1.0f - fRate)) +
			(pUi->endPos * fRate),
			INITD3DXVECTOR3,
			false,
			pUi->fWidth,
			pUi->fHeight,
			ANGLE_TYPE_FIXED);
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUi_titleLogo_00->Unlock();
}

//========================================
// DrawUi_titleLogo_00�֐� - UI:�^�C�g�����S[00] �̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawUi_titleLogo_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_titleLogo_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aTextureUi_titleLogo_00);

	// UI:�^�C�g�����S[00] �̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
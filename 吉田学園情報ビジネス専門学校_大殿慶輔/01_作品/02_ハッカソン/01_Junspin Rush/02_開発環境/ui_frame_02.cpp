//========================================
// 
// �t���[��[02] �̏���
// Author:���� ����
// 
//========================================
//  *** ui_frame_02.cpp ***
//========================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "polygon.h"
#include "md_game_00.h"
#include "ui_frame_02.h"

//****************************************
// �}�N����`
//****************************************

// �t���[��[02] �̏o���ɂ����鎞��
// �t���[��[02] �̏�����̂ɂ����鎞��
// �t���[��[02] �̓����x�̍ő�l
#define UI_FRAME_02_IN_POP_TIME		(10)
#define UI_FRAME_02_IN_CLEAR_TIME	(10)
#define UI_FRAME_02_ALPHA_MAX		(1.0f)

//****************************************
// �񋓌^�̒�`
//****************************************

// �t���[��[02] �̕��i�ԍ�
typedef enum
{
	UI_FRAME_02_PARTS_FRAME = 0,	// �t���[��
	UI_FRAME_02_PARTS_MAX,
}UI_FRAME_02_PARTS;

//****************************************
// �\���̂̒�`
//****************************************

// �t���[��[02] �̃|���S�����̏��\����
typedef struct
{
	char			aTexturePath	// �e�N�X�`���t�@�C���̑��΃p�X
					[TXT_MAX];		// 
	int				nPtnMaxX;		// �p�^�[���̏��(��)
	int				nPtnMaxY;		// �p�^�[���̏��(�c)
	int				nPtnMin;		// �p�^�[��No.�̉���
	int				nPtnMax;		// �p�^�[��No.�̏��
	int				nAnimTime;		// �A�j���[�V�����ɂ����鎞��
	float			fWidth;			// ��
	float			fHeight;		// ����
	int				nPolygonNum;	// �K�v�|���S����
	D3DXVECTOR3		relativePos;	// ���Έʒu

	ScaleDiameter	scale;			// �傫���ɂ�����{��
	int				nPtn;			// �p�^�[��No.
	int				nCounterAnim;	// �A�j���[�V�����̃J�E���^�[
}Ui_frame_02Parts;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �p�����[�^�[�̏���������
void InitParameterUi_frame_02(void);

// ���i(�t���[��)�̍X�V����
void UpdateUi_frame_02Parts_keyboard(void);

// ��ԏ���
void StateProcessUi_frame_02(void);

// �K�v�|���S�����𒲂ׂ�
int CheckPolygonNumUi_frame_02(int nParts);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_frame_02	// �e�N�X�`���ւ̃|�C���^
						[UI_FRAME_02_PARTS_MAX]	//
						= {};					//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_frame_02	// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;					//
Ui_frame_02				g_ui_frame_02;			// �t���[��[02] �̏��

// �t���[��[02] �̕��i���̏��
Ui_frame_02Parts g_aUi_frame_02Parts[UI_FRAME_02_PARTS_MAX] =
{
	{ "data\\TEXTURE\\Jumspin Rush �^�C�g�����.png"   , 1 , 1 , 0 , 0 , -1 , PIXEL * 1555  * 0.2f, PIXEL * 1066 * 0.2f, 1 , D3DXVECTOR3(SCREEN_WIDTH * 0.5f, PIXEL * 60, 0.0f) },
};

//========== *** �t���[��[02] �����擾 ***
Ui_frame_02 *GetUi_frame_02(void) 
{
	return &g_ui_frame_02;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateUi_frame_02Parts_keyboard�֐� - ���i(�t���[��)�̍X�V���� -
//========================================
void UpdateUi_frame_02Parts_keyboard(void)
{
	Ui_frame_02Parts	*pParts						// 
						= &g_aUi_frame_02Parts		//
						[UI_FRAME_02_PARTS_FRAME];	// ���i�̏��̃|�C���^
	VERTEX_2D			*pVtx;						// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_frame_02->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���ւ̃|�C���^�����̕��i�̊J�n�_�܂ł��炷
	pVtx += (4 * CheckPolygonNumUi_frame_02(UI_FRAME_02_PARTS_FRAME));

	for (int nCntPolygon = 0; nCntPolygon < pParts->nPolygonNum; nCntPolygon++, pVtx += 4)
	{
		// ���_���W��ݒ�
		SetVertexPosition(pVtx,
			g_ui_frame_02.pos + g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].relativePos,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			false,
			g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].fWidth
			* g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale.fWidth,
			g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].fHeight
			* g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale.fHeight,
			ANGLE_TYPE_FIXED);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtn,
				g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtnMin,
				g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtnMax,
				&g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nCounterAnim,
				g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nAnimTime,
				ANIM_TYPE_NORMAL),
			g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtnMaxX,
			g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].nPtnMaxY,
			NULL);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,(int)(255 * g_ui_frame_02.fAlpha) });
	}
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcessUi_frame_02(void)
{
	Ui_frame_02	*pUi				// 
				= &g_ui_frame_02;	// �t���[��[02] �̏��̃|�C���^ 

	switch (pUi->state)
	{
		//========== *** �o�� ***
	case UI_FRAME_02_STATE_POP:
	{

	}
		break;
		//========== *** �o���� ***
	case UI_FRAME_02_STATE_IN_POP:
	{
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// �����x�ƍ����̔{�����X�V
		pUi->fAlpha =
		g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale.fHeight =
			UI_FRAME_02_ALPHA_MAX * ((float)pUi->nCounterState / (float)UI_FRAME_02_IN_POP_TIME);

		if (pUi->nCounterState >= UI_FRAME_02_IN_POP_TIME)
		{// ��ԃJ�E���^�[���o���ɂ����鎞�ԂɒB�������A
			// �o����Ԃɂ���
			SetStateUi_frame_02(UI_FRAME_02_STATE_POP);

			// �����x���X�V
			pUi->fAlpha = UI_FRAME_02_ALPHA_MAX;
		}
	}
		break;
		//========== *** �����Ă��� ***
	case UI_FRAME_02_STATE_CLEAR:
	{

	}
		break;
		//========== *** ������ ***
	case UI_FRAME_02_STATE_IN_CLEAR:
	{
		// ��ԃJ�E���^�[�����Z
		pUi->nCounterState++;

		// �����x���X�V
		pUi->fAlpha = 
		g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale.fHeight = 
			UI_FRAME_02_ALPHA_MAX * (1.0f - ((float)pUi->nCounterState / (float)UI_FRAME_02_IN_CLEAR_TIME));

		if (pUi->nCounterState >= UI_FRAME_02_IN_CLEAR_TIME)
		{// ��ԃJ�E���^�[��������̂ɂ����鎞�ԂɒB�������A
			// �����Ă����Ԃɂ���
			SetStateUi_frame_02(UI_FRAME_02_STATE_CLEAR);

			// �Q�[���̏�Ԃ�ʏ�ɐݒ�
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}
	}
		break;
	}
}

//========================================
// CheckPolygonNum�֐� - �K�v�|���S�����𒲂ׂ� -
//========================================
int CheckPolygonNumUi_frame_02(int nParts) 
{
	int nPolygonNum	// 
		= 0;		// �K�v�|���S����

	// �|���S�����̕K�v�|���S���������Z���Ă���
	for (int nCntPolygon = 0; nCntPolygon < nParts; nCntPolygon++)
	{
		nPolygonNum += g_aUi_frame_02Parts[nCntPolygon].nPolygonNum;
	}

	// �K�v�|���S������Ԃ�
	return nPolygonNum;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_frame_02�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameterUi_frame_02(void)
{
	g_ui_frame_02.state			= UI_FRAME_02_STATE_CLEAR;	// ���
	g_ui_frame_02.nCounterState	= 0;						// ��ԃJ�E���^�[
	g_ui_frame_02.fAlpha		= 0.0f;						// �S�̂̓����x

	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++) 
	{
		g_aUi_frame_02Parts[UI_FRAME_02_PARTS_FRAME].scale = { 1.0f,1.0f };	// �傫���̔{��
	}
}

//========================================
// InitUi_frame_02�֐� - �t���[��[02] �̏��������� -
//========================================
void InitUi_frame_02(void)
{
	// �p�����[�^�[�̏���������
	InitParameterUi_frame_02();

	LPDIRECT3DDEVICE9	pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D			*pVtx;		// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CheckPolygonNumUi_frame_02(UI_FRAME_02_PARTS_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_frame_02,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUi_frame_02->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice, g_aUi_frame_02Parts[nCntParts].aTexturePath, &g_aTextureUi_frame_02[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_frame_02Parts[nCntParts].nPolygonNum; nCntPolygon++, pVtx += 4)
		{
			// ���_���W��ݒ�
			SetVertexPosition(pVtx,
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				false,
				g_aUi_frame_02Parts[nCntParts].fWidth,
				g_aUi_frame_02Parts[nCntParts].fHeight,
				ANGLE_TYPE_FREE);

			// rhw�̐ݒ�
			SetRHW(pVtx);

			// ���_�J���[�̐ݒ�
			SetVertexColor(pVtx, { 255,255,255,255 });

			// �e�N�X�`�����W�̐ݒ�
			SetTexturePosition(pVtx, 
				0, 
				g_aUi_frame_02Parts[nCntParts].nPtnMaxX, 
				g_aUi_frame_02Parts[nCntParts].nPtnMaxY,
				NULL);
		}
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffUi_frame_02->Unlock();
}

//========================================
// UninitUi_frame_02�֐� - �t���[��[02] �̏I������ -
//========================================
void UninitUi_frame_02(void)
{
	// �e�N�X�`���̔j��
	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++)
	{
		if (g_aTextureUi_frame_02[nCntParts] != NULL)
		{
			g_aTextureUi_frame_02[nCntParts]->Release();
			g_aTextureUi_frame_02[nCntParts] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffUi_frame_02 != NULL) 
	{
		g_pVtxBuffUi_frame_02->Release();
		g_pVtxBuffUi_frame_02 = NULL;
	}
}

//========================================
// UpdateUi_frame_02�֐� - �t���[��[02] �̍X�V���� -
//========================================
void UpdateUi_frame_02(void) 
{
	Ui_frame_02	*pUi				//
				= &g_ui_frame_02;	// �t���[��[02] �̏��̃|�C���^

	// ��ԏ���
	StateProcessUi_frame_02();

	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++) 
	{
		// ���i�ԍ��ɉ������X�V����
		switch (nCntParts)
		{
			//========== *** �t���[���̐��� ***
		case UI_FRAME_02_PARTS_FRAME:
		{
			// ���i(�t���[���̐���)�̍X�V����
			UpdateUi_frame_02Parts_keyboard();
		}
			break;
		}
	}
}

//========================================
// DrawUi_frame_02�֐� - �t���[��[02] �̕`�揈�� -
//========================================
void DrawUi_frame_02(void)
{
	Ui_frame_02	*pUi				//
				= &g_ui_frame_02;	// �t���[��[02] �̏��̃|�C���^

	if (pUi->state == UI_FRAME_02_STATE_CLEAR)
	{// �����Ă����Ԃ̎��A
		// �������I������
		return;
	}

	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUi_frame_02, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nPolygonNum	//
		= 0;		// �|���S���ԍ�

	for (int nCntParts = 0; nCntParts < UI_FRAME_02_PARTS_MAX; nCntParts++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_aTextureUi_frame_02[nCntParts]);

		for (int nCntPolygon = 0; nCntPolygon < g_aUi_frame_02Parts[nCntParts].nPolygonNum; nCntPolygon++, nPolygonNum++)
		{
			// �t���[��[02] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPolygonNum * 4, 2);
		}
	}
}

//========================================
// SetState�֐� - �t���[��[02] �̏�Ԑݒ菈�� -
//========================================
void SetStateUi_frame_02(UI_FRAME_02_STATE state) 
{
	Ui_frame_02	*pUi				//
				= &g_ui_frame_02;	// �t���[��[02] �̏��̃|�C���^ 

	switch (state)
	{
		//========== *** �o�� ***
	case UI_FRAME_02_STATE_POP:
	{

	}
		break;
		//========== *** �o���� ***
	case UI_FRAME_02_STATE_IN_POP:
	{

	}
		break;
		//========== *** �����Ă��� ***
	case UI_FRAME_02_STATE_CLEAR:
	{

	}
		break;
		//========== *** ������ ***
	case UI_FRAME_02_STATE_IN_CLEAR:
	{

	}
		break;
	}

	// ��Ԃ���
	pUi->state = state;

	// ��ԃJ�E���^�[��������
	pUi->nCounterState = 0;
}

//========================================
// SetUi_frame_02�֐� - �t���[��[02] �̐ݒ菈�� -
//========================================
void SetUi_frame_02(D3DXVECTOR3 pos)
{
	Ui_frame_02	*pUi				//
				= &g_ui_frame_02;	// �t���[��[02] �̏��̃|�C���^ 

	if (pUi->state != UI_FRAME_02_STATE_CLEAR) 
	{// �����Ă����ԂłȂ����A
		// �������I������
		return;
	}

	// �ʒu����
	pUi->pos = pos;

	// �o�����̏�Ԃɂ���
	SetStateUi_frame_02(UI_FRAME_02_STATE_IN_POP);
}
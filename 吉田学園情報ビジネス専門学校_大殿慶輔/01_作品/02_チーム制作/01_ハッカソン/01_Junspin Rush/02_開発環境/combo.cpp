//==========================================
//
//�R���{�\���v���O����[combo.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "combo.h"

//�}�N��
#define MAX_COMBOMASS				(64)	//�R���{��̍ő吔
#define COMBO_1COLUMN_USEPOLY		(8)		//�R���{��Ŏg���|���S�����i6�`8�j
#define COMBOPOLY_SIZE_WIDTH		(128)	//�e�N�X�`���T�C�Y���i���ۂ�4�{�j
#define COMBOPOLY_SIZE_HEIGHT		(128)	//�e�N�X�`���T�C�Y�����i���ۂ�4�{�j
#define COMBOPOLY_PATT_WIDTH		(8)		//�e�N�X�`���p�^�[����
#define COMBOPOLY_PATT_HEIGHT		(8)		//�e�N�X�`���p�^�[������
#define MAX_COMBO					(999)	//�ő�R���{��
#define COMBO_STR_NUM				(5)		//�����̌�ɕ\������COMBO�̕�����

//�R���{�|���S���\���̂̒�`
typedef struct
{
	D3DXCOLOR col;		//�F
	bool bUse;			//�g�p���Ă��邩
} ComboPoly;

//�R���{��\����
typedef struct
{
	ComboPoly cpData[COMBO_1COLUMN_USEPOLY];	//�R���{�|���S��
	D3DXVECTOR3 pos;							//�ʒu
	D3DXVECTOR3 move;							//�ړ���
	int nLife;									//����
	bool bUse;									//�g�p���Ă��邩
} ComboMass;

LPDIRECT3DTEXTURE9 g_pTextureCombo;			//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffCombo;	//���_�o�b�t�@�|�C���^
ComboMass g_aComboMass[MAX_COMBOMASS];			//�R���{��\����
int g_nComboNum;							//�R���{��

//========================
//�R���{����������
//========================
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//�f�o�C�X�̃|�C���^
	int nCntComboMass,nCntComboPoly;				//�J�E���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\txt_font_000.PNG",
		&g_pTextureCombo);

	//�R���{�\���̂̒��g������
	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		g_aComboMass[nCntComboMass].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ʒu
		g_aComboMass[nCntComboMass].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ړ���
		g_aComboMass[nCntComboMass].nLife = 0;													//����
		for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++)			//�R���{�|���S��
		{
			g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
			g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse = false;								//�g�p���Ă��邩
		}
		g_aComboMass[nCntComboMass].bUse = false;														//�g�p���Ă��邩
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COMBOMASS * COMBO_1COLUMN_USEPOLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffCombo,
		NULL);

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffCombo->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++, pVtx += 4)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffCombo->Unlock();
}

//========================
//�R���{�I������
//========================
void UninitCombo(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureCombo != NULL)
	{
		g_pTextureCombo->Release();
		g_pTextureCombo = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffCombo != NULL)
	{
		g_pVtxbuffCombo->Release();
		g_pVtxbuffCombo = NULL;
	}
}

//========================
//�R���{�X�V����
//========================
void UpdateCombo(void)
{
	int nCntComboMass, nCntComboPoly;		//�J�E���^
	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffCombo->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		if (g_aComboMass[nCntComboMass].bUse)
		{//�R���{���g�p����Ă���
			//�ʒu�ړ�
			g_aComboMass[nCntComboMass].pos -= g_aComboMass[nCntComboMass].move;

			for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++, pVtx += 4)
			{
				if (g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse == true)
				{
					//�s�����x�X�V
					g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col.a -= 1.0f / (float)g_aComboMass[nCntComboMass].nLife;

					//���W�X�V
					pVtx[0].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * nCntComboPoly;
					pVtx[0].pos.y = g_aComboMass[nCntComboMass].pos.y;
					pVtx[1].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * (nCntComboPoly + 1);
					pVtx[1].pos.y = g_aComboMass[nCntComboMass].pos.y;
					pVtx[2].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * nCntComboPoly;
					pVtx[2].pos.y = g_aComboMass[nCntComboMass].pos.y + COMBOPOLY_SIZE_HEIGHT / COMBOPOLY_PATT_HEIGHT;
					pVtx[3].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * (nCntComboPoly + 1);
					pVtx[3].pos.y = g_aComboMass[nCntComboMass].pos.y + COMBOPOLY_SIZE_HEIGHT / COMBOPOLY_PATT_HEIGHT;

					//�F�i�s�����x�j�X�V
					pVtx[0].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
					pVtx[1].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
					pVtx[2].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
					pVtx[3].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
				}
			}
			
			//�������s���������
			g_aComboMass[nCntComboMass].nLife--;
			if (g_aComboMass[nCntComboMass].nLife <= 0)
			{
				g_aComboMass[nCntComboMass].bUse = false;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffCombo->Unlock();
}

//========================
//�R���{�`�揈��
//========================
void DrawCombo(void)
{
	int nCntComboMass, nCntComboPoly;				//�J�E���^
	LPDIRECT3DDEVICE9 pDevice;						//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffCombo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureCombo);

	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		if (g_aComboMass[nCntComboMass].bUse == true)
		{
			for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++)
			{
				if (g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse == true)
				{
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntComboMass * COMBO_1COLUMN_USEPOLY + nCntComboPoly) * 4, 2);
				}
			}
		}
	}
}

//========================
//�R���{�ݒ菈��
//========================
void SetCombo(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife)
{
	return;

	const int nStrNumCombo[COMBO_STR_NUM] = { PATT_NUM_C ,PATT_NUM_O ,PATT_NUM_M ,PATT_NUM_B ,PATT_NUM_O };
	int aTexUV[COMBO_1COLUMN_USEPOLY];					//�����i�[
	int nCntComboMass, nCntComboPoly ,nCntComboStr;		//�J�E���^

	//�R���{���𑝂₷
	g_nComboNum++;
	if (g_nComboNum > MAX_COMBO)
	{
		g_nComboNum = MAX_COMBO;
	}

	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		if (!g_aComboMass[nCntComboMass].bUse)
		{
			//�ݒ�
			g_aComboMass[nCntComboMass].pos = pos;
			g_aComboMass[nCntComboMass].move = move;
			g_aComboMass[nCntComboMass].nLife = nLife;

			//���_���W�ݒ�
			VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

			g_pVtxbuffCombo->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += 4 * nCntComboMass * COMBO_1COLUMN_USEPOLY;

			nCntComboPoly = 0;	//�J�E���^������

			do 
			{
				//��������
				aTexUV[nCntComboMass] = g_nComboNum % (int)pow(10, (nCntComboPoly + 1)) / (int)pow(10, nCntComboPoly) + SCORE_PATT_OFFSET;
				g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse = true;
				nCntComboPoly++;
			} while (g_nComboNum / (int)pow(10, nCntComboPoly) != 0);

			//COMBO�ݒ�
			for (nCntComboStr = 0; nCntComboStr < COMBO_STR_NUM; nCntComboStr++, nCntComboPoly++)
			{
				aTexUV[nCntComboPoly] = nStrNumCombo[nCntComboStr];
				g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse = true;
			}

			for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++, pVtx += 4)
			{
				g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col = col;		//�F�ύX
				//g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse = true;		//�g�p���Ă����Ԃɂ���

																					//���_���W
				pVtx[0].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * nCntComboPoly;
				pVtx[0].pos.y = g_aComboMass[nCntComboMass].pos.y;
				pVtx[1].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * (nCntComboPoly + 1);
				pVtx[1].pos.y = g_aComboMass[nCntComboMass].pos.y;
				pVtx[2].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * nCntComboPoly;
				pVtx[2].pos.y = g_aComboMass[nCntComboMass].pos.y + COMBOPOLY_SIZE_HEIGHT / COMBOPOLY_PATT_HEIGHT;
				pVtx[3].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * (nCntComboPoly + 1);
				pVtx[3].pos.y = g_aComboMass[nCntComboMass].pos.y + COMBOPOLY_SIZE_HEIGHT / COMBOPOLY_PATT_HEIGHT;

				//���_�J���[
				pVtx[0].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
				pVtx[1].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
				pVtx[2].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
				pVtx[3].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((float)(aTexUV[nCntComboPoly] % COMBOPOLY_PATT_WIDTH) / COMBOPOLY_PATT_WIDTH,
					(float)(aTexUV[nCntComboPoly] / COMBOPOLY_PATT_WIDTH) / COMBOPOLY_PATT_HEIGHT);
				pVtx[1].tex = D3DXVECTOR2((float)(aTexUV[nCntComboPoly] % COMBOPOLY_PATT_WIDTH + 1) / COMBOPOLY_PATT_WIDTH,
					(float)(aTexUV[nCntComboPoly] / COMBOPOLY_PATT_WIDTH) / COMBOPOLY_PATT_HEIGHT);
				pVtx[2].tex = D3DXVECTOR2((float)(aTexUV[nCntComboPoly] % COMBOPOLY_PATT_WIDTH) / COMBOPOLY_PATT_WIDTH,
					(float)(aTexUV[nCntComboPoly] / COMBOPOLY_PATT_WIDTH + 1) / COMBOPOLY_PATT_HEIGHT);
				pVtx[3].tex = D3DXVECTOR2((float)(aTexUV[nCntComboPoly] % COMBOPOLY_PATT_WIDTH + 1) / COMBOPOLY_PATT_WIDTH,
					(float)(aTexUV[nCntComboPoly] / COMBOPOLY_PATT_WIDTH + 1) / COMBOPOLY_PATT_HEIGHT);
			}

			g_pVtxbuffCombo->Unlock();
		}
		//�g�p���Ă��邱�Ƃɂ���
		g_aComboMass[nCntComboMass].bUse = true;

		//������
		break;
	}
}

//========================
//�R���{���Z�b�g
//========================
void ResetCombo(void)
{
	g_nComboNum = 0;
}
/*-----------------------------------------------------------------------------------------------------

�_����[cloud.cpp]
Author:���V����

-------------------------------------------------------------------------------------------------------*/
#include "cloud.h"
#include "input.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

//�_�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	int nType;				//���
	bool bUse;				//�g���Ă��邩�ǂ���
}CLOUD;

//�_�̎�ލ\����
typedef enum
{
	CLOUD_TYPE_SMALLEST = 0,		//��ԏ������_
	CLOUD_TYPE_SMALL,				//�����߂ȉ_
	CLOUD_TYPE_MEDIUM,				//�����炢�T�C�Y�̉_
	CLOUD_TYPE_BIG,					//�����_(��ԑ傫��)
	CLOUD_TYPE_MAX,
}CLOUD_TYPE;

#define MAX_CLOUD_TYPE		(4)								//�_�̎��
#define MAX_CLOUD			(128)							//���C�t�̕\�����鐔�@= �v���C���[�̍ő僉�C�t
#define CLOUD_000			"data/TEXTURE/cloud_000.PNG"	//�_(��)�̉摜��
#define CLOUD_001			"data/TEXTURE/cloud_001.PNG"	//�_(�� Part2)�̉摜��
#define CLOUD_002			"data/TEXTURE/cloud_002.PNG"	//�_(��)�̉摜��
#define CLOUD_003			"data/TEXTURE/cloud_003.PNG"	//�_(��)�̉摜��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9			g_pTextureCloud[MAX_CLOUD_TYPE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffCloud = NULL;					//���_�o�b�t�@�ւ̃|�C���^
CLOUD g_Cloud[MAX_CLOUD];											//�N���E�h�̏��
int g_nCounterAppearCloud;											//�_�̔����^�C�~���O

//�_�̏���������
void InitCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, CLOUD_000, &g_pTextureCloud[CLOUD_TYPE_SMALLEST]);
	D3DXCreateTextureFromFile(pDevice, CLOUD_001, &g_pTextureCloud[CLOUD_TYPE_SMALL]);
	D3DXCreateTextureFromFile(pDevice, CLOUD_002, &g_pTextureCloud[CLOUD_TYPE_MEDIUM]);
	D3DXCreateTextureFromFile(pDevice, CLOUD_003, &g_pTextureCloud[CLOUD_TYPE_BIG]);

	//�_�̏�񏉊���
	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++)
	{
		g_Cloud[nCntCloud].pos  = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
		g_Cloud[nCntCloud].move = D3DXVECTOR3(0.3f, 0.0f, 0.0f);
		g_Cloud[nCntCloud].bUse = false;
	}
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CLOUD, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCloud, NULL);

	//�|�C���^��ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCloud->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		//rhw�̐ݒ�
		{
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
		}

		//���_�J���[�̐ݒ�
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//�e�N�X�`���̍��W
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.00001f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.00001f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffCloud->Unlock();

	//�����_���֐��̃V�[�h���������ݒ�
	srand((unsigned int)time(NULL));

	//�_���o��^�C�~���O�����߂�
	g_nCounterAppearCloud = rand() % 600;

	//�ŏ��ɉ_�𕡐���o��������
	SetCloud();
	SetCloud();
	SetCloud();
}

//�_�̏I������
void UninitCloud(void)
{
	//�e�N�X�`��(�S��)�̔j��
	for (int nCntCloud = 0; nCntCloud < CLOUD_TYPE_MAX; nCntCloud++)
	{
		if (g_pTextureCloud[nCntCloud] != NULL)
		{
			g_pTextureCloud[nCntCloud]->Release();
			g_pTextureCloud[nCntCloud] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCloud != NULL)
	{
		g_pVtxBuffCloud->Release();
		g_pVtxBuffCloud = NULL;
	}
}

//�_�̍X�V����
void UpdateCloud(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCloud->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++, pVtx += 4)
	{
		//�_���g�p����Ă���
		if (g_Cloud[nCntCloud].bUse == true)
		{
			g_Cloud[nCntCloud].pos.x -= g_Cloud[nCntCloud].move.x;

			//�_�̃T�C�Y���Ƃɒ��_���W��傫������
			switch (g_Cloud[nCntCloud].nType)
			{
			case CLOUD_TYPE_SMALLEST:		//��ԏ������T�C�Y�ƂQ�Ԗڂɏ������T�C�Y
			case CLOUD_TYPE_SMALL:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				break;

			case CLOUD_TYPE_MEDIUM:			//�����炢�T�C�Y�ƍő�T�C�Y
			case CLOUD_TYPE_BIG:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				break;
			}

			//�_����ʊO�ɏo�Ă��܂���
			if (g_Cloud[nCntCloud].pos.x < -200.0f)
			{
				g_Cloud[nCntCloud].bUse = false;
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffCloud->Unlock();

	if (g_nCounterAppearCloud > 0)
	{
		g_nCounterAppearCloud--;

		if (g_nCounterAppearCloud == 0)
		{
			//�_����������
			SetCloud();

			//�_���o��^�C�~���O�����߂�
			g_nCounterAppearCloud = rand() % 600;
		}
	}
}

//�_�̕`�揈��
void DrawCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCloud, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++)
	{
		if (g_Cloud[nCntCloud].bUse == true)
		{
			int nType = g_Cloud[nCntCloud].nType;

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCloud[nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCloud * 4, 2);
		}
	}
}

//�_�̐ݒ菈��
void SetCloud(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCloud->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++, pVtx += 4)
	{
		//�_���g���Ă��Ȃ�
		if (g_Cloud[nCntCloud].bUse == false)
		{
			//�_�̏o��X���W���E��ʒ[��肳��ɉ��ɐݒ�
			g_Cloud[nCntCloud].pos.x = (float)(rand() % 200) + 1380.0f;

			//�_��Y���W�������_���Őݒ�
			g_Cloud[nCntCloud].pos.y = (float)(rand() % (int)(SCREEN_HEIGHT * 0.5f)) + 50.0f;

			//�_�̎�ނ����߂�
			g_Cloud[nCntCloud].nType = rand() % CLOUD_TYPE_MAX;

			//�_�̃T�C�Y���Ƃɒ��_���W��傫������
			switch (g_Cloud[nCntCloud].nType)
			{
			case CLOUD_TYPE_SMALLEST:		//��ԏ������T�C�Y�ƂQ�Ԗڂɏ������T�C�Y
			case CLOUD_TYPE_SMALL:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				break;

			case CLOUD_TYPE_MEDIUM:			//�����炢�T�C�Y�ƍő�T�C�Y
			case CLOUD_TYPE_BIG:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				break;
			}
	
			//�g�p��Ԃɂ���
			g_Cloud[nCntCloud].bUse = true;			
			
			break;
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffCloud->Unlock();
}

//�_�̐ݒ菈��
void SetCloud2(void)
{
	VERTEX_2D *pVtx;

	int n = 5;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCloud->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++, pVtx += 4)
	{
		//�_���g���Ă��Ȃ�
		if (g_Cloud[nCntCloud].bUse == false)
		{
			//�_�̏o��X���W���E��ʒ[��肳��ɉ��ɐݒ�
			g_Cloud[nCntCloud].pos.x = (float)(rand() % SCREEN_WIDTH);

			//�_��Y���W�������_���Őݒ�
			g_Cloud[nCntCloud].pos.y = (float)(rand() % (int)(SCREEN_HEIGHT * 0.5f)) + 50.0f;

			//�_�̎�ނ����߂�
			g_Cloud[nCntCloud].nType = rand() % CLOUD_TYPE_MAX;

			//�_�̃T�C�Y���Ƃɒ��_���W��傫������
			switch (g_Cloud[nCntCloud].nType)
			{
			case CLOUD_TYPE_SMALLEST:		//��ԏ������T�C�Y�ƂQ�Ԗڂɏ������T�C�Y
			case CLOUD_TYPE_SMALL:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				break;

			case CLOUD_TYPE_MEDIUM:			//�����炢�T�C�Y�ƍő�T�C�Y
			case CLOUD_TYPE_BIG:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				break;
			}

			//�g�p��Ԃɂ���
			g_Cloud[nCntCloud].bUse = true;

			n--;

			if (n == 0) 
			{
				break;
			}
		}
	}

	//���_�o�b�t�@�����b�N����
	g_pVtxBuffCloud->Unlock();
}
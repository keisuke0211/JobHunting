//==========================================
//
//�X�R�A�\���v���O����[Score.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "score.h"
#include "player.h"

//�}�N��
#define SCORE_USEPOLY			(15)	//�|���S����
#define SCORE_SIZE_WIDTH		(256)	//�e�N�X�`���T�C�Y���i���ۂ�4�{�j
#define SCORE_SIZE_HEIGHT		(256)	//�e�N�X�`���T�C�Y�����i���ۂ�4�{�j
#define SCORE_PATT_WIDTH		(8)		//�e�N�X�`���p�^�[����
#define SCORE_PATT_HEIGHT		(8)		//�e�N�X�`���p�^�[������
#define SCORE_CHARPART_NUM		(7)		//���������Ŏg�p����|���S����
#define SCORE_NUMPART_NUM		(8)		//���������Ŏg�p����|���S����

//�O���[�o��
LPDIRECT3DTEXTURE9 g_pTextureScore;							//�e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffScore;					//���_�o�b�t�@�|�C���^
D3DXVECTOR3 g_posScore;										//�X�R�A���W
int g_nScore2;												//�ő�̗͔��f��̃X�R�A�̒l

//========================
//����������
//========================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^
	int nCntScore;				//�J�E���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\txt_font_000.PNG",
		&g_pTextureScore);

	//�X�R�A�ʒu�E�l�E��ޏ�����
	g_posScore = D3DXVECTOR3(784.0f, 16.0f, 0.0f);
	g_nScore2 = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SCORE_USEPOLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffScore,
		NULL);

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < SCORE_USEPOLY; nCntScore++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x + (SCORE_SIZE_WIDTH / SCORE_PATT_WIDTH * nCntScore), 
			g_posScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + (SCORE_SIZE_WIDTH / SCORE_PATT_WIDTH * (nCntScore + 1)),
			g_posScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x + (SCORE_SIZE_WIDTH / SCORE_PATT_WIDTH * nCntScore),
			g_posScore.y + (SCORE_SIZE_HEIGHT / SCORE_PATT_HEIGHT), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + (SCORE_SIZE_WIDTH / SCORE_PATT_WIDTH * (nCntScore + 1)),
			g_posScore.y + (SCORE_SIZE_HEIGHT / SCORE_PATT_HEIGHT), 0.0f);

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

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffScore->Unlock();
}

//========================
//�I������
//========================
void UninitScore(void)
{
	int nCntTexUnini;		//�e�N�X�`���j���J�E���^

	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffScore != NULL)
	{
		g_pVtxbuffScore->Release();
		g_pVtxbuffScore = NULL;
	}
}

//========================
//�X�V����
//========================
void UpdateScore(void)
{
	int aTexU[SCORE_USEPOLY];		//�e���̐����i�[
	int nCntScoreCulc;			//�v�Z�J�E���^

	//���������̃p�^�[���ԍ��ݒ�
	aTexU[0] = PATT_NUM_S;
	aTexU[1] = PATT_NUM_C;
	aTexU[2] = PATT_NUM_O;
	aTexU[3] = PATT_NUM_R;
	aTexU[4] = PATT_NUM_E;
	aTexU[5] = PATT_NUM_SPACE;
	aTexU[6] = PATT_NUM_COLON;

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScoreCulc = 0; nCntScoreCulc < SCORE_NUMPART_NUM; nCntScoreCulc++)
	{
		//��������
		aTexU[nCntScoreCulc + SCORE_CHARPART_NUM] = g_nScore2 % (int)pow(10, (SCORE_NUMPART_NUM - nCntScoreCulc)) / (int)pow(10, SCORE_NUMPART_NUM - 1 - nCntScoreCulc) + SCORE_PATT_OFFSET;
	}

	for (nCntScoreCulc = 0; nCntScoreCulc < SCORE_USEPOLY; nCntScoreCulc++, pVtx += 4)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((float)(aTexU[nCntScoreCulc] % SCORE_PATT_WIDTH) / SCORE_PATT_WIDTH,
			(float)(aTexU[nCntScoreCulc] / SCORE_PATT_WIDTH) / SCORE_PATT_HEIGHT);
		pVtx[1].tex = D3DXVECTOR2((float)(aTexU[nCntScoreCulc] % SCORE_PATT_WIDTH + 1) / SCORE_PATT_WIDTH,
			(float)(aTexU[nCntScoreCulc] / SCORE_PATT_WIDTH) / SCORE_PATT_HEIGHT);
		pVtx[2].tex = D3DXVECTOR2((float)(aTexU[nCntScoreCulc] % SCORE_PATT_WIDTH) / SCORE_PATT_WIDTH,
			(float)(aTexU[nCntScoreCulc] / SCORE_PATT_WIDTH + 1) / SCORE_PATT_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2((float)(aTexU[nCntScoreCulc] % SCORE_PATT_WIDTH + 1) / SCORE_PATT_WIDTH,
			(float)(aTexU[nCntScoreCulc] / SCORE_PATT_WIDTH + 1) / SCORE_PATT_HEIGHT);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffScore->Unlock();
}

//========================
//�`�揈��
//========================
void DrawScore(void)
{
	int nCntScore;				//�J�E���^
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < SCORE_USEPOLY; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//========================
//�ݒ菈��
//========================
void SetScore(int nScore)
{
	g_nScore2 = nScore;
}

//========================
//�X�R�A���Z����
//========================
void AddScore(int nValue)
{
	g_nScore2 += nValue;
}

//========================
//�X�R�A�擾
//========================
int GetScore2(void) 
{
	return g_nScore2;
}
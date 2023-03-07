//==========================================
//
//�����L���O�\���v���O����[ranking.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "ranking.h"
#include "input.h"
#include "fade.h"

//�}�N��
#define RANK_CHARPART_NUM			(4)		//���������Ŏg�p����|���S����
#define RANK_NUMPART_NUM			(8)		//���������Ŏg�p����|���S����
#define RANK_1COLUMN_USEPOLY		(12)	//�X�R�A����
#define RANK_SCORETEX_SIZE_WIDTH	(512)	//�X�R�A�e�N�X�`����
#define RANK_SCORETEX_SIZE_HEIGHT	(512)	//�X�R�A�e�N�X�`������
#define RANK_SCORETEX_NUM_WIDTH		(8)		//�X�R�A�e�N�X�`���p�^�[����
#define RANK_SCORETEX_NUM_HEIGHT	(8)		//�X�R�A�e�N�X�`���p�^�[������
#define RANK_FLASH_TIME				(20)	//�_�Ŏ��ԁi�t���[���j

//�O���[�o��
LPDIRECT3DTEXTURE9 g_pTextureRanking;		//�w�i���e�N�X�`���|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffRanking;	//�����̒��_�o�b�t�@�|�C���^�i���S)
Ranking g_aRanking[MAX_RANK];				//�����L���O�\����
int g_nNowScore;							//�v���C����
int g_nRankUpdate = -1;						//�����L���O�X�V�ӏ�
int g_nTimerRanking;						//��ʑJ�ڃJ�E���^�[
int g_nCounterFlash;						//�_�ŃJ�E���^�[

//========================
//����������
//========================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//�f�o�C�X�̃|�C���^
	int nCntRankingNum, nCntRankingStr;			//�J�E���^
	int aTexUV[RANK_1COLUMN_USEPOLY];			//�����i�[
	const int nStrNumRank[MAX_RANK][RANK_CHARPART_NUM] = {
		{ 1 + SCORE_PATT_OFFSET ,PATT_NUM_S ,PATT_NUM_T ,PATT_NUM_SPACE },
		{ 2 + SCORE_PATT_OFFSET ,PATT_NUM_N ,PATT_NUM_D ,PATT_NUM_SPACE },
		{ 3 + SCORE_PATT_OFFSET ,PATT_NUM_R ,PATT_NUM_D ,PATT_NUM_SPACE },
		{ 4 + SCORE_PATT_OFFSET ,PATT_NUM_T ,PATT_NUM_H ,PATT_NUM_SPACE },
		{ 5 + SCORE_PATT_OFFSET ,PATT_NUM_T ,PATT_NUM_H ,PATT_NUM_SPACE }
	};
	const int nStrNumYou[RANK_CHARPART_NUM] = { PATT_NUM_Y,PATT_NUM_O,PATT_NUM_U,PATT_NUM_SPACE };

	D3DXVECTOR3 posNowScore;

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\txt_font_001.PNG",
		&g_pTextureRanking);

	//���_�o�b�t�@�̐����i�X�R�A�j
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RANK_1COLUMN_USEPOLY * (MAX_RANK + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffRanking,
		NULL);

	//�ʒu�ݒ�
	posNowScore = D3DXVECTOR3(256.0f, 120.0f, 0.0f);
	g_aRanking[0].pos = D3DXVECTOR3(256.0f, 200.0f, 0.0f);
	g_aRanking[1].pos = D3DXVECTOR3(256.0f, 264.0f, 0.0f);
	g_aRanking[2].pos = D3DXVECTOR3(256.0f, 328.0f, 0.0f);
	g_aRanking[3].pos = D3DXVECTOR3(256.0f, 392.0f, 0.0f);
	g_aRanking[4].pos = D3DXVECTOR3(256.0f, 456.0f, 0.0f);

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���ʒ��_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	//�v���C�X�R�A
	//����
	for (nCntRankingStr = 0; nCntRankingStr < RANK_CHARPART_NUM; nCntRankingStr++)
	{
		aTexUV[nCntRankingStr] = nStrNumYou[nCntRankingStr];
	}

	//����
	for (nCntRankingStr = 0; nCntRankingStr < RANK_NUMPART_NUM; nCntRankingStr++)
	{
		aTexUV[nCntRankingStr + RANK_CHARPART_NUM] = g_nNowScore % (int)pow(10, (RANK_NUMPART_NUM - nCntRankingStr)) / (int)pow(10, RANK_NUMPART_NUM - 1 - nCntRankingStr) + SCORE_PATT_OFFSET;
	}

	//���ݒ�
	for (nCntRankingStr = 0; nCntRankingStr < RANK_1COLUMN_USEPOLY; nCntRankingStr++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(posNowScore.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * nCntRankingStr,
			posNowScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(posNowScore.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * (nCntRankingStr + 1),
			posNowScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(posNowScore.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * nCntRankingStr,
			posNowScore.y + RANK_SCORETEX_SIZE_HEIGHT / RANK_SCORETEX_NUM_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(posNowScore.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * (nCntRankingStr + 1),
			posNowScore.y + RANK_SCORETEX_SIZE_HEIGHT / RANK_SCORETEX_NUM_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_WIDTH,
			(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_HEIGHT);
		pVtx[1].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_WIDTH,
			(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_HEIGHT);
		pVtx[2].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_WIDTH,
			(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_WIDTH,
			(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_HEIGHT);
	}

	for (nCntRankingNum = 0; nCntRankingNum < MAX_RANK; nCntRankingNum++)
	{
		//�ʒu�ݒ�
		//����
		for (nCntRankingStr = 0; nCntRankingStr < RANK_CHARPART_NUM; nCntRankingStr++)
		{
			aTexUV[nCntRankingStr] = nStrNumRank[nCntRankingNum][nCntRankingStr];
		}

		//����
		for (nCntRankingStr = 0; nCntRankingStr < RANK_NUMPART_NUM; nCntRankingStr++)
		{
			aTexUV[nCntRankingStr + RANK_CHARPART_NUM] = g_aRanking[nCntRankingNum].nScore % (int)pow(10, (RANK_NUMPART_NUM - nCntRankingStr)) / (int)pow(10, RANK_NUMPART_NUM - 1 - nCntRankingStr) + SCORE_PATT_OFFSET;
		}

		for (nCntRankingStr = 0; nCntRankingStr < RANK_1COLUMN_USEPOLY; nCntRankingStr++, pVtx += 4)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aRanking[nCntRankingNum].pos.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * nCntRankingStr,
				g_aRanking[nCntRankingNum].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRanking[nCntRankingNum].pos.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * (nCntRankingStr + 1),
				g_aRanking[nCntRankingNum].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRanking[nCntRankingNum].pos.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * nCntRankingStr,
				g_aRanking[nCntRankingNum].pos.y + RANK_SCORETEX_SIZE_HEIGHT / RANK_SCORETEX_NUM_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRanking[nCntRankingNum].pos.x + RANK_SCORETEX_SIZE_WIDTH / RANK_SCORETEX_NUM_WIDTH * (nCntRankingStr + 1),
				g_aRanking[nCntRankingNum].pos.y + RANK_SCORETEX_SIZE_HEIGHT / RANK_SCORETEX_NUM_HEIGHT, 0.0f);

			//���W�ϊ��p�W���ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			if (nCntRankingNum == g_nRankUpdate)
			{
				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 0.94f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.94f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.94f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.94f, 0.0f, 1.0f);
			}
			else
			{
				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_WIDTH,
				(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_HEIGHT);
			pVtx[1].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_WIDTH,
				(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_HEIGHT);
			pVtx[2].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH) / RANK_SCORETEX_NUM_WIDTH,
				(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_HEIGHT);
			pVtx[3].tex = D3DXVECTOR2((float)(aTexUV[nCntRankingStr] % RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_WIDTH,
				(float)(aTexUV[nCntRankingStr] / RANK_SCORETEX_NUM_WIDTH + 1) / RANK_SCORETEX_NUM_HEIGHT);
		}
		g_aRanking[nCntRankingNum].bDisp = true;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffRanking->Unlock();

	//�_�Ŏ��ԏ�����
	g_nCounterFlash = 0;
}

//========================
//�I������
//========================
void UninitRanking(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffRanking != NULL)
	{
		g_pVtxbuffRanking->Release();
		g_pVtxbuffRanking = NULL;
	}
}

//========================
//�X�V����
//========================
void UpdateRanking(void)
{
	int nCntRank;		//�J�E���^
	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	g_nCounterFlash++;

	//���ʒ��_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffRanking->Lock(0, 0, (void **)&pVtx, 0);

	if (g_nRankUpdate != -1 && g_nCounterFlash >= RANK_FLASH_TIME)
	{
		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			if (nCntRank == g_nRankUpdate)
			{
				g_aRanking[nCntRank].bDisp = g_aRanking[nCntRank].bDisp ? false : true;
				g_nCounterFlash = 0;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffRanking->Unlock();
}

//========================
//�`�揈��
//========================
void DrawRanking(void)
{
	int nCntRankingNum, nCntRankingScore;			//�J�E���^
	LPDIRECT3DDEVICE9 pDevice;						//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���ʒ��_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffRanking, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�v���C���ʂ̃X�R�A�\��
	for (nCntRankingScore = 0; nCntRankingScore < RANK_1COLUMN_USEPOLY; nCntRankingScore++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureRanking);

		//�`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRankingScore * 4, 2);
	}

	//�����L���O�\��
	for (nCntRankingNum = 0; nCntRankingNum < MAX_RANK; nCntRankingNum++)
	{
		if (g_aRanking[nCntRankingNum].bDisp)
		{
			for (nCntRankingScore = 0; nCntRankingScore < RANK_1COLUMN_USEPOLY; nCntRankingScore++)
			{
				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, g_pTextureRanking);

				//�`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntRankingNum + 1) * RANK_1COLUMN_USEPOLY + nCntRankingScore) * 4, 2);
			}
		}
	}
}

//========================
//���Z�b�g����
//========================
void ResetRanking(void)
{
	g_aRanking[0].nScore = 5000;
	g_aRanking[1].nScore = 3000;
	g_aRanking[2].nScore = 2000;
	g_aRanking[3].nScore = 1000;
	g_aRanking[4].nScore = 600;
}

//========================
//�ݒ菈��
//========================
void SetRanking(int nScore)
{
	int nCntRank;	//�J�E���^
	int nTempScore;	//�X�R�A���u����

	g_nNowScore = nScore;

	if (g_aRanking[MAX_RANK - 1].nScore < nScore)
	{//���Ȃ��Ƃ�5�ʈȏ�ɓ���
		g_aRanking[MAX_RANK - 1].nScore = nScore;
		g_nRankUpdate = MAX_RANK - 1;

		//�\�[�g
		for (nCntRank = MAX_RANK - 1; nCntRank > 0; nCntRank--)
		{
			if (g_aRanking[nCntRank].nScore > g_aRanking[nCntRank - 1].nScore)
			{
				nTempScore = g_aRanking[nCntRank - 1].nScore;
				g_aRanking[nCntRank - 1].nScore = g_aRanking[nCntRank].nScore;
				g_aRanking[nCntRank].nScore = nTempScore;
				g_nRankUpdate--;
			}
			else
			{
				break;
			}
		}
	}
}
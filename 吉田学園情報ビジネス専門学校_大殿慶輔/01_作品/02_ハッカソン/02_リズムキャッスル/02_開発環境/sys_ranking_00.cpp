//========================================
// 
// SYS:�����L���O[00] �̏���
// Author:RIKU NISHIMURA
// 
//========================================
// *** sys_ranking_00.cpp ***
//========================================
#include "main.h"
#include "sys_ranking_00.h"
#include <stdio.h>

//****************************************
// �}�N����`
//****************************************
// �����L���O[00] �̏��̃f�[�^�t�@�C���̑��΃p�X
#define SYS_RANKING_00_RANKING_DATA_FILE_PATH	"data\\SAVEDATA\\ranking.bin"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �X�R�A�����L���O���~���Ƀ\�[�g����
void ScoreSortDesc(int *nScore, int *nUpdateRank);

//****************************************
// �O���[�o���錾
//****************************************
// �����L���O[00] �̏��
Sys_ranking_00	g_sys_ranking_00[RANKING_NUM] = 
{
	{ 5000 },
	{ 2500 },
	{ 500 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
};

// ���ʂ̕\���`��
char g_aRankText[RANKING_NUM][TXT_MAX] =
{
	"1ST",
	"2ND",
	"3RD",
	"4TH",
	"5TH",
	"6TH",
	"7TH",
	"8TH",
};

//========== *** �V�X�e��:�����L���O[00] �̏����擾 ***
Sys_ranking_00 *GetSys_ranking_00(void)
{
	return g_sys_ranking_00;
}

//========== *** ���ʂ̕\���`�����擾 ***
char *GetRankText(int nRank) 
{
	return g_aRankText[nRank];
}

//========================================
// InitSys_ranking_00�֐� - �V�X�e��:�����L���O[00] �̏��������� -
//========================================
void InitSys_ranking_00(void)
{
	
}

//========================================
// UninitSys_ranking_00�֐� - �V�X�e��:�����L���O[00] �̏I������ -
//========================================
void UninitSys_ranking_00(void) 
{
	
}

//========================================
// UpdateSys_ranking_00�֐� - �V�X�e��:�����L���O[00] �̍X�V���� -
//========================================
void UpdateSys_ranking_00(void) 
{
	
}

//========================================
// SetScore�֐� - �����L���O�ɃX�R�A��}������ -
//========================================
int SetScore(int nScore)
{
	int nUpdateRank = -1;	// �X�R�A���X�V��������

	// �X�R�A���~���Ƀ\�[�g����
	ScoreSortDesc(&nScore, &nUpdateRank);

	if (nScore > g_sys_ranking_00[RANKING_NUM - 1].nScore)
	{// �ł��������l���}������l���傫��������
		// �ł��������l�ɑ}������l��������
		g_sys_ranking_00[RANKING_NUM - 1].nScore = nScore;

		// �X�R�A���X�V�������ʂ�������
		nUpdateRank = RANKING_NUM - 1;
	}

	// �X�R�A���~���Ƀ\�[�g����
	ScoreSortDesc(&nScore, &nUpdateRank);

	// �X�R�A���X�V�������ʂ�Ԃ�
	return nUpdateRank;
}

//========================================
// ScoreSortDesc�֐� - �X�R�A�����L���O���~���Ƀ\�[�g���� -
//========================================
void ScoreSortDesc(int *nScore, int *nUpdateRank)
{
	int		nTemp;						// �ꎞ�I�ɒl���i�[
	int		nCntCompare1, nCntCompare2;	// ��r�̃J�E���g
	bool	bUpdateRank					//
			= false;					// ���ʍX�V�t���O

	for (nCntCompare1 = 0; nCntCompare1 < RANKING_NUM - 1; nCntCompare1++)
	{
		for (nCntCompare2 = nCntCompare1 + 1; nCntCompare2 < RANKING_NUM; nCntCompare2++)
		{
			if (g_sys_ranking_00[nCntCompare2].nScore > g_sys_ranking_00[nCntCompare1].nScore)
			{// �v�f2���v�f1�̒l�������Ă������A
				// �l�����ւ���
				nTemp = g_sys_ranking_00[nCntCompare1].nScore;
				g_sys_ranking_00[nCntCompare1].nScore = g_sys_ranking_00[nCntCompare2].nScore;
				g_sys_ranking_00[nCntCompare2].nScore = nTemp;

				if (bUpdateRank == false)
				{// ���ʍX�V�t���O���U�̎��A
					// ���ʍX�V�t���O��^�ɂ���
					bUpdateRank = true;

					// �X�R�A���X�V�������ʂ�������
					*nUpdateRank = nCntCompare1;
				}
			}
		}
	}
}

//========================================
// LoadSys_ranking_00�֐� - �V�X�e��:�����L���O[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadSys_ranking_00(void)
{
	FILE *pFile;	//�t�@�C���̃|�C���^

	//�t�@�C�����J��
	pFile = fopen(SYS_RANKING_00_RANKING_DATA_FILE_PATH, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J�������A
		//�t�@�C�����烉���L���O��ǂݍ���
		fread(g_sys_ranking_00, sizeof(Sys_ranking_00), RANKING_NUM, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}

//========================================
// SaveSys_ranking_00�֐� - �V�X�e��:�����L���O[00] �̏������ݏ��� -
//========================================
void SaveSys_ranking_00(void)
{
	FILE *pFile;	//�t�@�C���̃|�C���^

	//�t�@�C�����J��
	pFile = fopen(SYS_RANKING_00_RANKING_DATA_FILE_PATH, "wb");

	if (pFile != NULL)
	{//�t�@�C�����J�������A
		//�t�@�C���Ƀ����L���O�������o��
		fwrite(g_sys_ranking_00, sizeof(Sys_ranking_00), RANKING_NUM, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}
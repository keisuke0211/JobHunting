//========================================
// 
// �����̂܂Ƃ܂�
// Author:RIKU NISHIMURA
// 
//========================================
// *** chunk.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include <stdio.h>

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadScale�֐� - �g��{�����̓ǂݍ��ݏ��� -
//========================================
void LoadScale(FILE *pFile, Scale *pScale) 
{
	fscanf(pFile, "%f", &pScale->fWidth);
	fscanf(pFile, "%f", &pScale->fHeight);
	fscanf(pFile, "%f", &pScale->fDepth);
}

//========================================
// Count�֐� - �J�E���g���� -
// Author:RIKU NISHIMURA
//========================================
int Count(int *pData, int nMin, int nMax, int *pCounter, int nCountTime, COUNT_TYPE countType)
{
	if (nCountTime != -1)
	{// �J�E���g�ɂ����鎞�Ԃ�-1�łȂ����A
		if (pCounter == NULL) 
		{// �J�E���^�[�̃|�C���^��NULL�łȂ����A
			*pData += 1;	// �l�����Z
		}
		else
		{// �J�E���^�[�̃|�C���^��NULL�̎��A
			*pCounter += 1;	// �J�E���^�[�����Z

			if (*pCounter >= nCountTime)
			{// �J�E���^�[���J�E���g�ɂ����鎞�ԂɒB�������A
				*pCounter = 0;	// �J�E���^�[������������
				*pData += 1;	// �l�����Z
			}
		}
	}
	else
	{// �J�E���g�ɂ����鎞�Ԃ�-1�̎��A
		// �l�����̂܂ܕԂ�
		return *pData;
	}

	// �J�E���g�̎�ނɉ������l��Ԃ�
	switch (countType)
	{
		//========== *** �ʏ� ***
	case COUNT_TYPE_NORMAL:

		// �l������ŏ�]�Z�������ʂ�Ԃ�
		return nMin + (*pData %= (nMax - nMin + 1));

		//========== *** �܂�Ԃ� ***
	case COUNT_TYPE_TURN:

		if ((*pData / (nMax - nMin)) % 2 == 0)
		{// �l������ŏ��Z�������ʂ��������������A
			// �l������ŏ�]�Z�������ʂ�Ԃ�
			return nMin + (*pData %= (nMax - nMin));
		}
		else
		{//�p�^�[��������ŏ��Z�������ʂ�����������A
			//�������p�^�[��������ŏ�]�Z�������ʂ����Z�������ʂ�Ԃ�
			return nMax - (*pData % (nMax - nMin));
		}
	}
}

//========================================
// IntConsumption�֐� - int�^�̒l�̏���� -
// Author:RIKU NISHIMURA
//========================================
bool IntConsumption(int *pData, int nSubtract) 
{
	if ((*pData -= nSubtract) >= 0)
	{// �l�������ʂ����Z��������0�ȏ�̎��A
		return true;	// �^��Ԃ�
	}
	else 
	{// �l�������ʂ����Z��������0��������Ă��鎞�A
		*pData += nSubtract;	// ���Z���������Z
		return false;			// �U��Ԃ�
	}
}

//========================================
// FloatControl�֐� - float�^�̒l�̐��䏈�� -
// Author:RIKU NISHIMURA
//========================================
void FloatControl(float *pData, float fMax, float fMin)
{
	if (*pData > fMax)
	{// �ő�l�������Ă������A
		// �ő�l����
		*pData = fMax;
	}
	else if (*pData < fMin)
	{// �ŏ��l��������Ă������A
		// �ŏ��l����
		*pData = fMin;
	}
}

//========================================
// FloatLoopControl�֐� - float�^�̒l�̃��[�v���䏈�� -
// Author:RIKU NISHIMURA
//========================================
void FloatLoopControl(float *pData, float fMax, float fMin)
{
	if (*pData > fMax)
	{// �ő�l�������Ă������A
		// �ő�l�����Z
		*pData -= (fMax - fMin);
	}
	else if (*pData < fMin)
	{// �ŏ��l��������Ă������A
		// �ő�l�����Z
		*pData += (fMax - fMin);
	}
}

//========================================
// IntControl�֐� - int�^�̒l�̐��䏈�� -
// Author:RIKU NISHIMURA
//========================================
void IntControl(int *pData, int nMax, int nMin)
{
	if (*pData > nMax)
	{// �ő�l�������Ă������A
		// �ő�l����
		*pData = nMax;
	}
	else if (*pData < nMin)
	{// �ŏ��l��������Ă������A
		// �ŏ��l����
		*pData = nMin;
	}
}

//========================================
// IntLoopControl�֐� - int�^�̒l�̃��[�v���䏈�� -
// Author:RIKU NISHIMURA
//========================================
void IntLoopControl(int *pData, int nMax, int nMin)
{
	if (*pData >= nMax)
	{// �ő�l�ȏ�̎��A
		// �ŏ��l����
		*pData = nMin;
	}
	else if (*pData < nMin)
	{// �ŏ��l��������Ă������A
		// �ő�l����
		*pData = nMax - 1;
	}
}


//========================================
// IntControlReturn�֐� - int�^�̒l�̐��䏈��(���ʂ�Ԃ�) -
// Author:RIKU NISHIMURA
//========================================
int IntControlReturn(int nData, int nMax, int nMin)
{
	if (nData > nMax)
	{// �ő�l�������Ă������A
		// �ő�l����
		nData = nMax;
	}
	else if (nData < nMin)
	{// �ŏ��l��������Ă������A
		// �ŏ��l����
		nData = nMin;
	}

	return nData;
}

//========================================
// IntLoopControlReturn�֐� - int�^�̒l�̃��[�v���䏈��(���ʂ�Ԃ�) -
// Author:RIKU NISHIMURA
//========================================
int IntLoopControlReturn(int nData, int nMax, int nMin)
{
	if (nData >= nMax)
	{// �ő�l�ȏ�̎��A
		// �ŏ��l����
		nData = nMin;
	}
	else if (nData < nMin)
	{// �ŏ��l��������Ă������A
		// �ő�l����
		nData = nMax - 1;
	}

	return nData;
}

//========================================
// Lottery�֐� - ���I���� -
// Author:RIKU NISHIMURA
//========================================
bool Lottery(float fProb)
{
	if (fProb > (0.1f * (rand() % 1000)))
	{// ���I�������A
		// �^��Ԃ�
		return true;
	}
	else
	{// ���I���Ȃ��������A
		// �U��Ԃ�
		return false;
	}
}

//========================================
// fRand�֐� - �����̗�����Ԃ� -
// Author:RIKU NISHIMURA
//========================================
float fRand(float fMax)
{
	if (fMax == 0) 
	{// �����0�̎��A
		return 0;	// �������I������
	}

	float fNum;	// �����̒l

	fMax *= 100;

	fNum = rand() % (int)fMax;

	return fNum * 0.01f;
}

//========================================
// Rand�֐� - ������Ԃ� -
// Author:RIKU NISHIMURA
//========================================
int Rand(int nMin, int nMax) 
{
	return nMin + (rand() % (nMax - nMin));
}

//========================================
// PrusMinus�֐� - �^����ꂽ�l�ɉ�����1�𕄍��t���ŕԂ� -
// Author:RIKU NISHIMURA
//========================================
int PrusMinus(float fData)
{
	if (fData == 0)
	{// ������0���������A
		// 0��Ԃ�
		return 0;
	}

	// 1�𕄍��t���ŕԂ�
	return (fabsf(fData) / fData);
}

//========================================
// ExistsZero�֐� - �^����ꂽ�l��0�łȂ�����0���A0�̎���1��Ԃ� -
// Author:RIKU NISHIMURA
//========================================
int ExistsZero(float fData) 
{
	if (fData != 0) 
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}

//========================================
// Prus�֐� - �^����ꂽ�l��0�ȉ��̎���0���A0�ȏ�̎��͒l�����̂܂ܕԂ� -
// Author:RIKU NISHIMURA
//========================================
float Prus(float fData) 
{
	if (fData <= 0) 
	{
		return 0;
	}
	else 
	{
		return fData;
	}
}

//========================================
// strinit�֐� - �󔒖��� -
// Author:RIKU NISHIMURA
//========================================
void strinit(char *pData, int nNum)
{
	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		pData[nCnt] = '\0';
	}
}
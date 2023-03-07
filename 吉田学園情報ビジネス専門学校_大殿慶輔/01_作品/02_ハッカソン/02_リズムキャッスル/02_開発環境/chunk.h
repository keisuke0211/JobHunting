//========================================
// 
// �����̂܂Ƃ܂�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chunk.h ***
//========================================
#ifndef _CHUNK_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHUNK_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include <stdio.h>

//****************************************
// �񋓌^�̒�`
//****************************************
// �J�E���g�̎��
typedef enum
{
	COUNT_TYPE_NORMAL,	// �ʏ�
	COUNT_TYPE_TURN,	// �܂�Ԃ�
	COUNT_TYPE_MAX,
}COUNT_TYPE;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �g��{�����̓ǂݍ��ݏ���
void LoadScale(FILE *pFile, Scale *pScale);

// �J�E���g����
int Count(int *pData, int nMin, int nMax, int *pCounter, int nCountTime, COUNT_TYPE countType);

// int�^�̏����
bool IntConsumption(int *pData, int nSubtract);

// float�^�̒l�̐��䏈��
// float�^�̒l�̃��[�v���䏈��
// int�^�̒l�̐��䏈��
// int�^�̒l�̃��[�v���䏈��
// int�^�̒l�̐��䏈��(���ʂ�Ԃ�)
// int�^�̒l�̃��[�v���䏈��(���ʂ�Ԃ�)
void FloatControl(float *pData, float fMax, float fMin);
void FloatLoopControl(float *pData, float fMax, float fMin);
void IntControl(int *pData, int nMax, int nMin);
void IntLoopControl(int *pData, int nMax, int nMin);
int IntControlReturn(int nData, int nMax, int nMin);
int IntLoopControlReturn(int nData, int nMax, int nMin);

// ���I����
bool Lottery(float fProb);

// �����̗�����Ԃ�
// ������Ԃ�
float fRand(float fMax);
int Rand(int nMin, int nMax);

// �^����ꂽ�l�ɉ�����1��-1��Ԃ�
// �^����ꂽ�l��0�łȂ�����0���A0�̎���1��Ԃ�
// �^����ꂽ�l��0�ȉ��̎���0���A0�ȏ�̎��͒l�����̂܂ܕԂ�
int PrusMinus(float fData);
int ExistsZero(float fData);
float Prus(float fData);

// �󔒖���
void strinit(char *pData, int nNum);

#endif
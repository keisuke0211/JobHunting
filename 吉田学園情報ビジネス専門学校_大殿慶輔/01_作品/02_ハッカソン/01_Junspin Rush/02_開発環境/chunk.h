//========================================
//  *** chunk.h ***
//========================================
#ifndef _CHUNK_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CHUNK_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �v���g�^�C�v�錾
//****************************************

// float�^�̒l�̐��䏈��
void FloatControl(float *fData, float fMax, float fMin);

// float�^�̒l�̃��[�v���䏈��
void FloatLoopControl(float *fData, float fMax, float fMin);

// int�^�̒l�̐��䏈��
void IntControl(int *fData, int fMax, int fMin);

// int�^�̒l�̃��[�v���䏈��
void IntLoopControl(int *fData, int fMax, int fMin);

#endif
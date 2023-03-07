//========================================
// 
// ���̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** light.h ***
//========================================
#ifndef _LIGHT_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _LIGHT_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �\���̂̒�`
//****************************************
// ��(2D)�̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;		// �ʒu
	float		fLength;	// ����

	// ��Ԋ֘A
	bool	bUse;	// �g�p����Ă��邩�t���O
	Color	col;	// �F
}Light2D;

// ��(3D)�̊Ǘ����\����
typedef struct
{
	Color col;			// �F
	float fBrightness;	// ���邳
}Light3DControl;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ���̏���������
// ���̏I������
// ���̍X�V����
void InitLight(void);
void UninitLight(void);
void UpdateLight(void);

// ��(2D)�̗\�񏈗�
// ��(2D)�Ƃ̋�������F���擾
void ReserveLight2D(D3DXVECTOR3 pos, Color col, float fLength);
Color GetColorForLight2D(D3DXVECTOR3 pos, Color myCol);

// ��(3D)�̊Ǘ������擾
Light3DControl *GetLight3DControl(void);

// ��(3D)�̖��邳�ݒ菈��
// ��(3D)�̐F�ݒ菈��
void SetBrightnessLight3D(float fBrightness);
void SetColorLight3D(Color col);

#endif
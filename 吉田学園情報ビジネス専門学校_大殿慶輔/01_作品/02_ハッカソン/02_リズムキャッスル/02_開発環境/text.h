//========================================
// 
// �e�L�X�g�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** text.h ***
//========================================
#ifndef _TEXT_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _TEXT_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �}�N����`
//****************************************
// �e�L�X�g(2D)�̍ő吔
#define TEXT2D_MAX	(128)

// �e�L�X�g(3D)�̍ő吔
#define TEXT3D_MAX	(128)

// �\���`��
typedef enum
{
	DISPLAY_CENTER,	// ��������
	DISPLAY_LEFT,	// ���l��
	DISPLAY_RIGHT,	// �E�l��
}DISPLAY;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �t�H���g�̓ǂݍ��ݏ���
void LoadFont(void);

// �e�L�X�g�̏���������
// �e�L�X�g�̏I������
// �e�L�X�g�̕`�揈��
void InitText(void);
void UninitText(void);
void DrawText_(void);

// �e�L�X�g(2D)�̐ݒ菈��
void SetText2D(char *pString, int nFont, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight);

// �e�L�X�g(3D)�̐ݒ菈��
void SetText3D(char *pString, int nFont, DISPLAY disp, D3DXVECTOR3 pos, Color col, float fWidth, float fHeight, bool bZtest, bool bLighting, bool bBillboard);

#endif
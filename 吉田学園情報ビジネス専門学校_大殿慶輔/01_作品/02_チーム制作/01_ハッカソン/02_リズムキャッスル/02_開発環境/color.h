//========================================
// 
// �F�̏����̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
//  *** color.h ***
//========================================
#ifndef _COLOR_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _COLOR_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include <stdio.h>

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �F�̓ǂݍ��ݏ���
void LoadColor(FILE *pFile, Color *pCol);

// �F�ɖ��邳�𔽉f
Color BrightnessToColor(Color col, float fBrightness);

// Color�`���̐F��D3DXCOLOR�`���ɕϊ�
// Color�`���̐F��D3DCOLOR�`���ɕϊ�
D3DXCOLOR ColorToD3DXCOLOR(Color col);
D3DCOLOR ColorToD3DCOLOR(Color col);

// Color�`���̐F�̒l�������ŕԂ�
// Color�`���̐F�����Z
// Color�`���̐F��������
Color ColorRate(Color col, float fRate);
Color ColorAdd(Color col1, Color col2);
Color ColorMix(Color col1, Color col2);

#endif
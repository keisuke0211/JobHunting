//========================================
// 
// UI:�ړ��e�L�X�g[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_move-text_00.h ***
//========================================
#ifndef _UI_MOVETEXT_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_MOVETEXT_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�ړ��e�L�X�g[00] �̏���������
// UI:�ړ��e�L�X�g[00] �̏I������
// UI:�ړ��e�L�X�g[00] �̍X�V����
void InitUi_moveText_00(void);
void UninitUi_moveText_00(void);
void UpdateUi_moveText_00(void);

// UI:�ړ��e�L�X�g[00] �̐ݒ菈��
void SetUi_moveText_00(char *pString, int nFont, D3DXVECTOR3 pos, D3DXVECTOR3 move, Color col, int nLife);

#endif
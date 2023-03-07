//========================================
// 
// UI:�^�C�g�����S[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_title-logo_00.h ***
//========================================
#ifndef _UI_TITLELOGO_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_TITLELOGO_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �\���̂̒�`
//****************************************
// UI:�^�C�g�����S[00] �̏��
typedef struct
{
	// �ǂݍ��ݏ��
	char		aTexturePath	// �e�N�X�`���̑��΃p�X
				[TXT_MAX];
	float		fWidth;			// ��
	float		fHeight;		// ����
	D3DXVECTOR3	startPos;		// �J�n�ʒu
	D3DXVECTOR3	endPos;			// �I���ʒu
	int			nArrivalTime;	// �����ɂ����鎞��

	// ��Ԋ֘A
	int nCounterArrival;	// �����J�E���^�[
}Ui_titleLogo_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// UI:�^�C�g�����S[00] �̓ǂݍ��ݏ���
void LoadUi_titleLogo_00(void);

// UI:�^�C�g�����S[00] �̏����擾
Ui_titleLogo_00 *GetUi_titleLogo_00(void);

// UI:�^�C�g�����S[00] �̏���������
// UI:�^�C�g�����S[00] �̏I������
// UI:�^�C�g�����S[00] �̍X�V����
// UI:�^�C�g�����S[00] �̕`�揈��
void InitUi_titleLogo_00(void);
void UninitUi_titleLogo_00(void);
void UpdateUi_titleLogo_00(void);
void DrawUi_titleLogo_00(void);

#endif
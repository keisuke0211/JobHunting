//========================================
// 
// OBJ:��[01] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_castle_01.h ***
//========================================
#ifndef _OBJ_CASTLE_01_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_CASTLE_01_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:��[01] �̏��
typedef struct
{
	// ��Ԋ֘A
	int nCounterDamage;	// �_���[�W�J�E���^�[
}Obj_castle_01;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:��[01] �̏����擾
Obj_castle_01 *GetObj_castle_01(void);

// OBJ:��[01] �̏���������
// OBJ:��[01] �̏I������
// OBJ:��[01] �̍X�V����
// OBJ:��[01] �̕`�揈��
void InitObj_castle_01(void);
void UninitObj_castle_01(void);
void UpdateObj_castle_01(void);
void DrawObj_castle_01(void);

void SetDamageObj_castle_01(void);

#endif
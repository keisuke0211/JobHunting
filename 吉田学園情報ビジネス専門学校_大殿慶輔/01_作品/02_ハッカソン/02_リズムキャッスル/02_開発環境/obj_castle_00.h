//========================================
// 
// OBJ:��[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_castle_00.h ***
//========================================
#ifndef _OBJ_CASTLE_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _OBJ_CASTLE_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//****************************************
// �\���̂̒�`
//****************************************
// OBJ:��[00] �̏��
typedef struct
{
	// ��Ԋ֘A
	int nCounterDamage;	// �_���[�W�J�E���^�[
}Obj_castle_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// OBJ:��[00] �̏����擾
Obj_castle_00 *GetObj_castle_00(void);

// OBJ:��[00] �̏���������
// OBJ:��[00] �̏I������
// OBJ:��[00] �̍X�V����
// OBJ:��[00] �̕`�揈��
void InitObj_castle_00(void);
void UninitObj_castle_00(void);
void UpdateObj_castle_00(void);
void DrawObj_castle_00(void);

void SetDamageObj_castle_00(void);

#endif
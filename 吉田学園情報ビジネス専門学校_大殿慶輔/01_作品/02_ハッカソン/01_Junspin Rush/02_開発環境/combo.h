//==========================================
//
//�R���{�\���v���O�����̃w�b�_[combo.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "main.h"

//�v���g�^�C�v�錾
void InitCombo(void);
void UninitCombo(void);
void UpdateCombo(void);
void DrawCombo(void);
void SetCombo(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife);
void ResetCombo(void);

#endif // !_COMBO_H_

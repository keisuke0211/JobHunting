//==========================================
//
//コンボ表示プログラムのヘッダ[combo.h]
//Author:石原颯馬
//
//==========================================
#ifndef _COMBO_H_
#define _COMBO_H_

#include "main.h"

//プロトタイプ宣言
void InitCombo(void);
void UninitCombo(void);
void UpdateCombo(void);
void DrawCombo(void);
void SetCombo(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife);
void ResetCombo(void);

#endif // !_COMBO_H_

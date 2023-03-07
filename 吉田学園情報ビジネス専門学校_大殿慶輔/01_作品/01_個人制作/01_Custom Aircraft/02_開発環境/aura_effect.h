//◇====================◇
//｜　　　　　　　　　　｜
//｜　aura_effect.h     ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _AURAEFFECT_H_
#define _AURAEFFECT_H_

#include"main.h"

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitAuraEffect(void);
void UninitAuraEffect(void);
void UpdateAuraEffect(void);
void DrawAuraEffect(void);
void SetAuraEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nType);

#endif
//◇====================◇
//｜　　　　　　　　　　｜
//｜　effect.h          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _GOAL_EFFECT_H_
#define _GOAL_EFFECT_H_

#include"main.h"

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitGoalEffect(void);
void UninitGoalEffect(void);
void UpdateGoalEffect(void);
void DrawGoalEffect(void);
void SetGoalEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nType);

#endif
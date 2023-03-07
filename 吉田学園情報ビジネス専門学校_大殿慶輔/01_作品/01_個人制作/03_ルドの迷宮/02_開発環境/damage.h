//◇====================◇
//｜　　　　　　　　　　｜
//｜　damage.h 　　 　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _DAMAGE_H_  //このマクロ定義がされてなかったら
#define _DAMAGE_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

void InitDamage(void);
void UninitDamage(void);
void UpdateDamage(void);
void DrawDamage(void);

//スコアの設定と加算
void SetDamage(int nScore);
void AddDamage(int nValue);

int GetDamage(void);
#endif
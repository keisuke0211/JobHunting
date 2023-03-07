//◇====================◇
//｜　　　　　　　　　　｜
//｜　life.h  　　　　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _LIFE_H_  //このマクロ定義がされてなかったら
#define _LIFE_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

void InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);

////ライフの設定と減算
void SetLife(int nLife);
void AddLife(int nValue);
void DeleteLife(int nValus);

int GetLife(void);
#endif
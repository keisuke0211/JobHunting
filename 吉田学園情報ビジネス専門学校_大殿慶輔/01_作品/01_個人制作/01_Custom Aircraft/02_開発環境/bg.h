//◇====================◇
//｜　　　　　　　　　　｜
//｜　bg.h              ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _BG_H_  //このマクロ定義がされてなかったら
#define _BG_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitBg(void);
void UninitBg(void);
void UpdateBg(void);
void DrawBg(void);

#endif
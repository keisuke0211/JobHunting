//◇====================◇
//｜　　　　　　　　　　｜
//｜　frame.h           ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _FRAME_H_  //このマクロ定義がされてなかったら
#define _FRAME_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitFrame(void);
void UninitFrame(void);
void UpdateFrame(void);
void DrawFrame(void);

#endif
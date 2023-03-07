//◇====================◇
//｜　　　　　　　　　　｜
//｜　fade.h		　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

//◇==================◇
//｜　 列挙型構造体 　｜
//◇==================◇

//フェードの状態
typedef enum
{
	FADE_NONE = 0,//何もしていない状態
	FADE_IN,//フェードイン状態
	FADE_OUT,//フェードアウト状態
	FADE_MAX,
}FADE;
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇

//プロトタイプ宣言
void lnitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(MODE modeNext);
FADE GetFade(void);

#endif
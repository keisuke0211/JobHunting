//◇====================◇
//｜　　　　　　　　　　｜
//｜　title.h           ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _TITLE_H_
#define _TITLE_H_

#include"main.h"
#include"input.h"

//◇==================◇
//｜　 列挙型構造体 　｜
//◇==================◇

//文字の状態
typedef enum
{
	TITLESTATE_NORMAL = 0,	//通常状態
	TITLESTATE_FLASHING, 	//点滅状態
	TITLESTATE_MAX
}TITLESTATE;

//◇====================◇
//｜ 　　情報構造体 　　｜
//◇====================◇
typedef struct
{
	
	TITLESTATE state;	//状態
	int nCounterState;	//状態管理カウンター
	bool bUse;			//使用しているかどうか

}TITLE;
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif
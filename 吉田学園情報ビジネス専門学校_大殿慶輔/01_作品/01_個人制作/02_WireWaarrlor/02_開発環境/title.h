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

//*****************************************************************************
// タイトル背景一覧
//*****************************************************************************
typedef enum
{
	TITLE_BG000 = 0,	// 星空
	TITLE_BG001,		// 雲１
	TITLE_BG002,		// 雲２
	TITLE_TEXT00,		// タイトル名
	TITLE_TEXT002,		// press enter
	TITLE_PLAYER,		// Pプレイヤーセット１
	TITLE_MAX,
}TITLE_BG;
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
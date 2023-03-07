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
	TITLE_MAX,
}TITLE_BG;
//◇====================◇
//｜ 　　情報構造体 　　｜
//◇====================◇
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス

	TITLESTATE state;	//状態
	int nCounterState;	//状態管理カウンター

	float fWidth;		//横幅
	float fHight;		//高さ

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
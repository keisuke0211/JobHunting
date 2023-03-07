//◇====================◇
//｜　　　　　　　　　　｜
//｜　game.h            ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _GAME_H_
#define _GAME_H_

#include"main.h"
#include"input.h"

//*****************************************************************************
// ゲームの状態
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,		//何もしていない状態
	GAMESTATE_NORMAL,		//通常状態・ゲーム進行中
	GAMESTATE_END1,			//終了状態・敵全滅
	GAMESTATE_END2,			//プレイヤー滅
	GAMESTATE_MAX
}GAMESTATE;

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state, int nCounter);
void SetEnablePause(bool bPause);
GAMESTATE GetGameState(void);

#endif
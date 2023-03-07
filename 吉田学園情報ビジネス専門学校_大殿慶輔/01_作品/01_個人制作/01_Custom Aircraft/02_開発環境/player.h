//◇====================◇
//｜　　　　　　　　　　｜
//｜　player/h          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _PLAYER_H_  //このマクロ定義がされてなかったら
#define _PLAYER_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

//****************************************
// プレイヤーの状態
//****************************************

typedef enum
{
	PLAYERSTATE_APPEAR = 0,		//出現状態(点滅)
	PLAYERSTATE_NORMAL,			//通常状態
	PLAYERSTATE_DAMAGE,			//ダメージ状態
	PLAYERSTATE_WAIT,			//点滅状態
	PLAYERSTATE_DEATH,			//死亡状態
	PLAYESTATE_MAX
}PLAYERSTATE;

//****************************************
// プレイヤーの構造体の定義
//****************************************

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;    //向き
	D3DXVECTOR3 move;   //移動量
	PLAYERSTATE state;	//状態
	int nCounterState;	//状態管理カウンター
	int nLife;			//体力
	bool bUse;			//使用しているかどうか
	bool bDrw;			//表示　する・しない
	bool bBeing;		//プレイヤーの状態　生存・死亡
	bool bBph;			//コントローラの振動の有無
}Player;
//****************************************
// プロトタイプ宣言
//****************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void); //４つのデータをmain.cppから移動
void HitPlayer(int nCntPlayer, int nDamage);
Player *Getplayer(void);
//****************************************
// マクロ定義
//****************************************
#define SIZE_WIDTH  (32.0f)
#define SIZE_HEIGHT (32.0f)
#define PLAYER_SPEED (2.0f)
#define SPEED       (12.0f)
#define UP          (0.75f)
#define DOWN        (0.25f) 

#endif
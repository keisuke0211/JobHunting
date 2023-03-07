//◇====================◇
//｜　　　　　　　　　　｜
//｜　enemy.h		　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _SUMMON_H_
#define _SUMMON_H_

#include"main.h"
#include<stdio.h>
//****************************************
// マクロ定義
//****************************************
#define MAX_ENEMY (128) //敵の最大数
#define SIZE_ENEMY (48) //敵の大きさ

//****************************************
// 列挙型の定義
//****************************************

//敵の状態
typedef enum
{
	SUMMONSTATE_NORMAL = 0,	//通常状態
	SUMMONSTATE_DAMAGE, 	//ダメージ状態
	SUMMONSTATE_MAX
}SUMMONSTATE;

//****************************************
// 構造体の定義
//****************************************

//敵構造体の定義
typedef struct
{
	int nType;			//敵・種類
	int sType;			//弾・種類
	int m_pattern;		//移動パターン
	int s_pattern;		//発射パターン
	int speed;			//弾スピード
	int nLife;			//体力
	int score;			//スコア

	int in_time;		//出現時間
	int stop_time;		//停止時間
	int shot_time;		//弾発射時間
	int out_time;		//帰還時間
	int nCntMove;		//移動間隔
	int nBullet;		//攻撃間隔
	int time;			//プレイヤーの位置を確認する時間
	int Tracking;		//追尾する時間
	int nCntStop;		//停止時間

	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;    //向き


	SUMMONSTATE state;	//状態
	int nCounterState;	//状態管理カウンター

	bool bUse;			//使用しているかどうか
	bool Reversal;		//移動方向反転
	bool m_switch;		//行動・切り替え
	bool bTime;			//待機のオン・オフ
	bool nStop;			//停止フラグ
	bool bDrw;			//表示　する・しない
	bool bHit;			//当たり判定　オン・オフ
}Summon;

//****************************************
// プロトタイプ宣言
//****************************************
void InitSummon(void);
void UninitSummon(void);
void UpdateSummon(void);
void DrawSummon(void);
void LoadSummon(void);

void HitSummon(int nCntEnemy, int nDamage);
void EnemySummon(int p_pattern);				//場所・種類

Summon *GetSummon(void);
int GetNumSummon(void);
int GetnCharge1(void);
int GetnCharge2(void);

#endif
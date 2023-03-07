//◇====================◇
//｜　　　　　　　　　　｜
//｜　enemy.h		　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include"main.h"
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
	ENEMYSTATE_NORMAL = 0,	//通常状態
	ENEMYSTATE_DAMAGE, 	//ダメージ状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

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
	int stand_time;		//待機時間

	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;    //向き


	ENEMYSTATE state;	//状態
	int nCounterState;	//状態管理カウンター

	bool bUse;			//使用しているかどうか
	bool Reversal;		//移動方向反転
	bool m_switch;		//行動・切り替え
	bool bTime;			//待機のオン・オフ
	bool bDrw;			//表示　する・しない
	bool bHit;			//当たり判定　オン・オフ
}Enemy;

//****************************************
// プロトタイプ宣言
//****************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void LoadEnemy(void);

//void SetEnemy(D3DXVECTOR3 pos, int nType);	//位置と種類
void HitEnemy(int nCntEnemy, int nDamage);

Enemy *GetEnemy(void);
int GetNumEnemy(void);
int GetCunt(void);

#endif
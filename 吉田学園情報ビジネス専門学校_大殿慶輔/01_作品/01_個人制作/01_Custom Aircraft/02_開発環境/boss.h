//◇====================◇
//｜　　　　　　　　　　｜
//｜　boos.h  　　　　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _BOSS_H_  //このマクロ定義がされてなかったら
#define _BOSS_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_BOSS	(4)		//敵の最大数
#define SIZE_BOSS	(80)	//ボスの大きさ
//◇==================◇
//｜　 列挙型構造体 　｜
//◇==================◇

//ボスの状態
typedef enum
{
	BOSSSTATE_NORMAL = 0,	//通常状態
	BOSSSTATE_DAMAGE, 	//ダメージ状態
	BOSSSTATE_MAX
}BOSSSTATE;

//◇====================◇
//｜ 　　情報構造体 　　｜
//◇====================◇
//敵構造体の定義
typedef struct
{
	int nType;				//敵・種類
	int sType;				//弾・種類
	int score;				//スコア
	int m_pattern;			//移動パターン
	int s_pattern;			//発射パターン
	int state_pattern;		//状態推移のパターン
	int in_time;			//出現時間
	int stop_time;			//停止時間
	int shot_time;			//弾発射時間
	int out_time;			//帰還時間
	int stand_time;			//待機時間

	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量

	int speed;				//弾スピード
	int init_nLife;			//初期体力
	int curt_nLife;			//現在の体力

	BOSSSTATE state;		//状態
	int nCounterState;		//状態管理カウンター
	int nBullet;			//攻撃間隔
	int nReset;				//チャージリセット
	
	bool bHit;				//当たり判定ありなし
	bool bTime;				//待機時間のカウント　開始・終了
	bool bUse;				//使用しているかどうか
	bool bDrw;				//表示するかしないか
	bool bSummon;			//召喚したか
}Boss;

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
void SetBoss(D3DXVECTOR3 pos, int nCntBoss);
void HitBoss(int nCntBoss, int nDamage);
void LoadBoss(void);
Boss *GetBoss(void);

int GetNumBoss(void);
int GetnCharge(void);
int GetinCnt(void);

#endif
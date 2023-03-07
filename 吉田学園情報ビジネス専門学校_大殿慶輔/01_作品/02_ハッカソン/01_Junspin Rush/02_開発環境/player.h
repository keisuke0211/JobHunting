//==========================================
//
//プレイヤー関係プログラムのヘッダ[player.h]
//Author:石原颯馬
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//マクロ
#define PLAYER_SIZE_WIDTH		(128)	//プレイヤーのポリゴンサイズ（幅）
#define PLAYER_SIZE_HEIGHT		(64)	//プレイヤーのポリゴンサイズ（高さ)
#define PLAYER_HIT_TEST_RADIUS	(16.0f)	//プレイヤーの当たり判定の半径
#define DEFAULT_LIFE			(3)		//初期体力
#define GROUND_HEIGHT			(64.0f)	//地面の高さ
#define DAMAGE_TIME				(60)	//ダメージ時間

//プレイヤー状態（列挙）
typedef enum
{
	PLAYERSTATE_FREEFALL = 0,	//落下
	PLAYERSTATE_NORMAL,			//通常
	PLAYERSTATE_DAMAGE,			//ダメージ
	PLAYERSTATE_JUMP,			//ジャンプ
	PLAYERSTATE_MAX
} PLAYERSTATE;

//プレイヤー型構造体定義
typedef struct
{
	//位置
	D3DXVECTOR3 pos;					//位置

	//回転関係
	D3DXVECTOR3 rot;					//向き
	float fLength;						//対角線の長さ
	float fAngle;						//対角線の角度
	float fRotSpeed;					//回転速度

	//アニメーション関係
	int nCounterAnim;					//アニメーションカウンタ
	int nDamageCounter;					//ダメージカウンター
	int nPatternAnim;					//アニメーションパターンNo.
	bool bLanding;						//着地フラグ

	//状態
	PLAYERSTATE state;					//状態
	bool bTurn;							//回っているか
	bool bChainJump;					//2度目以降のジャンプ中のフラグ
	int nLife;							//体力

	//運動関係
	float fSpeed;						//速度（u）
	DWORD dwExecSkyLastTime;			//ジャンプ・自由落下を始めた時間
	int nCounterTurn;					//回転時間

	//クールタイム
	int nCoolTime;						//クールタイム

	//表示・非表示
	bool bDisp;							//生きているかどうか
} Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

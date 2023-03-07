//◇====================◇
//｜　　　　　　　　　　｜
//｜　player/h          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _PLAYER_H_  //このマクロ定義がされてなかったら
#define _PLAYER_H_  //２重インクルード防止のマクロを定義する

#include"main.h"
#include "block.h"
//****************************************
// プレイヤーの構造体の定義
//****************************************

typedef struct
{
	D3DXVECTOR3 pos;	//現在の位置
	D3DXVECTOR3 posOld;	//前回の位置
	D3DXVECTOR3 Draepos;//描画位置
	D3DXVECTOR3 rot;    //向き
	D3DXVECTOR3 move;   //移動量

	int nType;			//画像種類
	int nCounterAnim;	//アニメーションカウンター
	int nPatternAnim;	//アニメーションパターンNo.
	int nCntReturn;		//プレイヤーがフックの所に移動するカウント
	int nCntFriction;	//摩擦の減少カウント
	int bCheckpoint;	//中間地点にふれたか

	float fLength;		//プレイヤーの長さ
	float fAngle;		//プレイヤーの角度
	float fTwo_Range;	//プレイヤーとルアーの距離
	float fTwo_Angle;	//プレイヤーとルアーの角度

	//振り子関係
	float fx;			//支点　X座標
	float fy;			//支点　Y座標
	float rot1;			//プレイヤーの角度
	float rot_spd;		//角速度(１フレームで角度が変化する量)
	float length;		//紐の長さ(計測)
	float lengthFixed;	//紐の長さ(固定)
	float g;			//重力加速度
	float rad;			//現在の重りの位置　向き
	float px;			//		〃			X座標
	float py;			//		〃			
	float vx;			//重力移動量を反映したプレイヤーの位置　ｘ座標
	float vy;			//　			〃						Ｙ座標
	float t;			//
	float gx;			//重力移動量を反映したプレイヤーの位置　ｘ座標
	float gy;			//　			〃						Ｙ座標
	float r;			//２つの重りの位置の角度差
	float sub;			//プレイヤーの角度と２つの重りの角度差を引いた
	float friction;		//摩擦量

	//放物線運動
	float px1;			//重りの位置　X座標（分かりやすく同じ変数を使う）
	float py1;			//	  〃	　Y座標
	float dx;			//X方向の速度
	float dy;			//Y方向の速度
	float gx1;			//重力加速度 X座標
	float gy1;			//重力加速度 Y座標

	//bool一覧
	bool bLure;
	bool bDraw;			//表示するか
	bool bIdle;			//待機ON・OFF
	bool bJump;			//ジャンプ中かどうか
	bool bSpace;		//プレイヤーとルアーの間隔を計算したかしてないか
	bool bCamera;		//カメラＯＮ・ＯＦＦ
	bool bMeasure;		//計測したかどうか(振り子)
	bool bMeasure2;		//計測したかどうか(放物線運動)
	bool bPendulum;		//振り子する・しない
	bool bAutoLure;		//最初の自動移動したかどうか
	bool bInertia;		//放物線運動するかどうか
	bool bOperation;	//操作出来るかどうか？
	bool bLanding;		//着地したかどうか;
	bool bOnce;			//１回だけ処理する
	bool bDirection;	//放物線の進む方向

	Block *pBlock;		//対象ブロックへのポインタ
}Player;
//****************************************
// プロトタイプ宣言
//****************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);	//４つのデータをmain.cppから移動
void PlayerAction(void);//プレイヤーの操作、移動処理
void LureAction(void);	//ルアーの操作、移動処理
void Pendulum(void);	//振り子時の処理
void PendulumEnd(void); //振り子終了時の処理
Player *Getplayer(void);
//****************************************
// マクロ定義
//****************************************
#define DEFAULT_INT			(0)						//intの初期値
#define DEFAULT_FLOAT		(0.0f)					//floatの初期値
#define SIZE_WIDTH			(16.0f)					// プレイヤーの幅
#define SIZE_HEIGHT			(26.0f)					// 	  〃      高さ
#define PLAYER_SPEED		(1.5f)					// 	  〃      移動速度
#define PLAYER_JUMP			(18.0f)					// 	  〃      ジャンプ量
#define UP					(0.75f)					// 斜め上の角度
#define DOWN				(0.25f)					// 斜め下の角度
#define MIN_STRING			(10.0f)					// 最小の糸の長さ
#define MAX_STRING			(350.0f)				// 最大の糸の長さ
#define AMOUNT_OF_FRICTION	(0.0005f)				// 摩擦量
#define MIN_FRICTION		(0.0f)					// 摩擦の最小値
#define MAX_FRICTION		(3.2f)					// 摩擦の最大値
#define MIN_GRAVITY			(0.05f)					// 最小の重力
#define MAX_GRAVITY			(1.0f)					// 最高の重力

#endif
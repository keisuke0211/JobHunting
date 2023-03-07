//========================================
// 
// 物理関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** physics.h ***
//========================================
#ifndef _PHYSICS_H_	// このマクロ定義がされていなかった時
#define _PHYSICS_H_	// 二重インクルード防止のマクロを定義する

#include <stdio.h>
#include "main.h"

//****************************************
// マクロ定義
//****************************************
// 当たり判定の最大数
// 当たり判定の部品の最大数
#define HIT_TEST_MAX		(16)
#define HIT_TEST_PARTS_MAX	(32)

//****************************************
// 列挙型の定義
//****************************************
// 衝突チェックのモード
typedef enum 
{
	COLLCHK_MODE_NORMAL,	// 通常
	COLLCHK_MODE_LOOKDOWN,	// 見下ろし
}COLLCHK_MODE;

// 当たり判定の形
typedef enum
{
	HIT_TEST_FORM_CIRCLE,		// 円形
	HIT_TEST_FORM_SQUARE,		// 四角形
	HIT_TEST_FORM_POINT,		// 点
	HIT_TEST_FORM_POINT_RFLC,	// 点(反射)
	HIT_TEST_FORM_POINT_NOSLIP,	// 点(滑り無し)
	HIT_TEST_FORM_MAX,
}HIT_TEST_FORM;

// 衝突方向
typedef enum 
{
	HIT_DIRECTION_TOP,		// 上
	HIT_DIRECTION_BOTTOM,	// 下
	HIT_DIRECTION_LEFT,		// 左
	HIT_DIRECTION_RIGHT,	// 右
	HIT_DIRECTION_BACK,		// 奥
	HIT_DIRECTION_FRONT,	// 手前
	HIT_DIRECTION_MAX,
}HIT_DIRECTION;

// ベクトルの種類
typedef enum 
{
	VECTOR_X,		// X方向
	VECTOR_Y,		// Y方向
	VECTOR_Z,		// Z方向
	VECTOR_NONE,	// 無し
	VECTOR_MAX
}VECTOR;

// 頂点
typedef enum 
{
	VERTEX_X0Y0,	// 左上
	VERTEX_X1Y0,	// 右上
	VERTEX_X0Y1,	// 左下
	VERTEX_X1Y1,	// 右下
	VERTEX_MAX,
}VERTEX;

// 4方向
typedef enum 
{
	DIRECTION_UP,			// 上
	DIRECTION_DOWN,			// 下
	DIRECTION_LEFT,			// 左
	DIRECTION_RIGHT,		// 右
	DIRECTION_BACK,			// 奥
	DIRECTION_FRONT,		// 手前
	
	DIRECTION_LEFT_BACK,	// 左奥
	DIRECTION_LEFT_FRONT,	// 左手前
	DIRECTION_RIGHT_BACK,	// 右奥
	DIRECTION_RIGHT_FRONT,	// 右手前
	DIRECTION_MAX,
}DIRECTION;

// 旋回方向
typedef enum 
{
	SWING_DIRECTION_LEFT,	// 左
	SWING_DIRECTION_RIGHT,	// 右
}SWING_DIRECTION;

//****************************************
// 構造体の定義
//****************************************
// 衝突情報構造体
typedef struct
{
	bool	bHitDirection;	// 衝突フラグ
	bool	aHitDirection	// 方向毎の衝突フラグ
			[HIT_DIRECTION_MAX];
	bool	bInside;		// 範囲内フラグ
	bool	aInside			// 方向毎の範囲内フラグ
			[HIT_DIRECTION_MAX];
	bool	bHit;			// 当たりフラグ※物理的に接触している
	bool	aHit			// 方向毎の当たりフラグ
			[HIT_DIRECTION_MAX];
	bool	bGhost;			// すり抜けフラグ※非物理的に接触している
	bool	aGhost			// 方向毎のすり抜けフラグ
			[HIT_DIRECTION_MAX];
	float	fLength;		// めり込み距離
}Collision;

// 当たり判定の部品情報構造体
typedef struct
{
	HIT_TEST_FORM	hitTestForm;		// 当たり判定の形
	float			fWidth;				// 幅
	float			fHeight;			// 高さ
	float			fDepth;				// 奥行き
	D3DXVECTOR3		fixedRelativePos;	// 相対位置
	bool			aGhost				// すり抜け情報
					[HIT_DIRECTION_MAX];
}HitTestParts;

// 当たり判定の情報構造体
typedef struct 
{
	int				nHitTestPartsNum;	// 当たり判定の部品の数
	HitTestParts	aHitTestParts		// 当たり判定の部品の情報
					[HIT_TEST_PARTS_MAX];
}HitTest;

// 当たり判定の設定情報構造体
typedef struct
{
	int		nHitTestNum;	// 当たり判定の数
	HitTest aHitTest		// 当たり判定の情報
			[HIT_TEST_MAX];
}HitTestSet;

// 当たり判定管理構造体
typedef struct
{
	int	nHitTest;	// 当たり判定番号
}HitTestInfo;

// 衝突判定に必要な情報構造体
typedef struct
{
	D3DXVECTOR3 *pPos;		// 現在の位置のポインタ
	D3DXVECTOR3 posOld;		// 過去の位置のポインタ
	D3DXVECTOR3 *pMove;		// 移動量のポインタ
	D3DXVECTOR3 *pRot;		// 現在の向きのポインタ
	D3DXVECTOR3 rotOld;		// 過去の向きのポインタ
	HitTest		hitTest;	// 当たり判定の情報
}CollisionInfo;

//****************************************
// プロトタイプ宣言
//****************************************
// ベクトル情報の読み込み処理
// 当たり判定設定情報の読み込み処理
void LoadVector(FILE *pFile, D3DXVECTOR3 *pVector);
void LoadHitTestSet(FILE *pFile, HitTestSet *pHitTestSet);

// 衝突チェック
// 内側チェック
// 衝突情報の上書き
void CollisionCheck(VECTOR vector, Collision *pCollision, CollisionInfo myCollInfo, CollisionInfo pairCollInfo, COLLCHK_MODE mode);
void InsideCheck(Collision *pCollision);
void CollisionOverwrite(Collision *pOverwriteCollision, Collision collision);

// 位置の更新処理
void PositionUpdate(D3DXVECTOR3 *pos, D3DXVECTOR3 *move);

// 推進処理
void Driving(D3DXVECTOR3 *pos, D3DXVECTOR3 rot, D3DXVECTOR3 *move, float fMove);

// 方向転換処理
// 方向転換処理(見下ろし)
void TurnAround(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation);
void TurnAroundLookDown(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation);

// 2点の距離を求める
// 2点の距離を求める(見下ろし)
// 2点の角度を求める
// 2点の角度を求める(見下ろし)
// 交点の座標を求める(見下ろし)
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
float FindDistanceLookDown(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
float FindAngleLookDown(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);
D3DXVECTOR3 FindIntersectionPointLookDown(D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2);

// 角度の差を求める
// 角度を制御する
// 角度を指定された数の方向に変換する
float AngleDifference(float fAngle, float fTargetAngle);
void ControlAngle(float *pAngle);
float ConvertAngleToDirection(float fAngle, int nDirection);

// 位置を鳥瞰図に変換する(Z⇔Y)
// 向きを鳥瞰図に変換する(Z⇔Y)
D3DXVECTOR3 ConvPosToBirdsEyeView(D3DXVECTOR3 pos);
D3DXVECTOR3 ConvRotToBirdsEyeView(D3DXVECTOR3 rot);

// 向きを制御
void RotControl(D3DXVECTOR3 *pRot);

#endif
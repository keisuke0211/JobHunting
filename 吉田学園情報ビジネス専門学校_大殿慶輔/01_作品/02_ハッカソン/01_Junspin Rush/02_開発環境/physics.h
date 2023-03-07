//========================================
//  *** physics.h ***
//========================================
#ifndef _PHYSICS_H_	// このマクロ定義がされていなかった時
#define _PHYSICS_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// 当たり判定の種類
typedef enum
{
	HIT_TEST_TYPE_CIRCLE = 0,	// 円形
	HIT_TEST_TYPE_SQUARE,		// 四角形
	HIT_TEST_TYPE_MAX,
}HIT_TEST_TYPE;

//****************************************
// 構造体の定義
//****************************************

// 当たり判定の種類毎の情報
typedef struct 
{
	// 分類関連
	char aLoadName[TXT_MAX];	// 読み込み名
}HitTestType;

//****************************************
// プロトタイプ宣言
//****************************************

// 当たり判定の種類毎の情報を取得
HitTestType *GetHitTestType(void);

// 当たり判定の種類を文字列から読み込む
void StringLoadHitTestType(char aString[TXT_MAX], HIT_TEST_TYPE *pType);

// 衝突チェック
bool CheckHit(
	D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth1, float fHeight1, HIT_TEST_TYPE hitTestType1,
	D3DXVECTOR3 pos2, D3DXVECTOR3 rot2, float fWidth2, float fHeight2, HIT_TEST_TYPE hitTestType2);

// 位置の更新処理
void PositionUpdate(D3DXVECTOR3 *pos, D3DXVECTOR3 *move);

// 推進処理
void Driving(D3DXVECTOR3 *pos, D3DXVECTOR3 rot, D3DXVECTOR3 *move, float fMove);

// 方向転換処理
void TurnAround(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation);

// 2点の距離を求める
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);

// 2点の角度を求める
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos);

// 角度の差を求める
float AngleDifference(float fAngle, float fTargetAngle);

// 角度を制御する
void ControlAngle(float *pAngle);

#endif
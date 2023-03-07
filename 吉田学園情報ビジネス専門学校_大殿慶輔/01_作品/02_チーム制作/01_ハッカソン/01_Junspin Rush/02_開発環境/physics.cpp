//========================================
// 
// 物理関連の処理
// Author:西村 吏功
// 
//========================================
// *** physics.cpp ***
//========================================
#include "main.h"
#include "physics.h"

//****************************************
// プロトタイプ宣言
//****************************************

// 円同士の衝突チェック
bool CheckHitCircleAndCircle(
	D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth1, float fHeight1,
	D3DXVECTOR3 pos2, D3DXVECTOR3 rot2, float fWidth2, float fHeight2);

//****************************************
// グローバル宣言
//****************************************

// 当たり判定の種類毎の情報
HitTestType g_aHitTestType[HIT_TEST_TYPE_MAX] =
{// 読み込み名
	"円形",
	"四角形"
};

//========== *** 当たり判定の種類毎の情報を取得 ***
HitTestType *GetHitTestType(void) 
{
	return g_aHitTestType;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// CheckHitCircleAndCircle関数 - 円同士の衝突チェック -
//========================================
bool CheckHitCircleAndCircle(
	D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth1, float fHeight1, 
	D3DXVECTOR3 pos2, D3DXVECTOR3 rot2, float fWidth2, float fHeight2)
{
	if (FindDistance(pos1, pos2) < fWidth1 + fWidth2) 
	{// 2点の距離が当たり判定の幅の合計を下回っている時、
		// 真を返す
		return true;
	}
	else 
	{// 2点の距離が当たり判定の幅の合計を下回っていない時、
		// 偽を返す
		return false;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StringLoadHitTestType関数 - 当たり判定の種類を文字列から読み込む -
//========================================
void StringLoadHitTestType(char aString[TXT_MAX], HIT_TEST_TYPE *pType)
{
	HitTestType	*pHitTestType	// 当たり判定の種類毎の情報のポインタ
				= g_aHitTestType;

	for (int nCntHitTestType = 0; nCntHitTestType < HIT_TEST_TYPE_MAX; nCntHitTestType++, pHitTestType++)
	{
		if (strcmp(aString, pHitTestType->aLoadName) == 0)
		{// 読み込み名が一致した時、
			// 当たり判定の種類を代入
			*pType = (HIT_TEST_TYPE)nCntHitTestType;
			break;
		}
	}
}

//========================================
// CheckHit関数 - 衝突チェック -
//========================================
bool CheckHit(
	D3DXVECTOR3 pos1, D3DXVECTOR3 rot1, float fWidth1, float fHeight1, HIT_TEST_TYPE hitTestType1, 
	D3DXVECTOR3 pos2, D3DXVECTOR3 rot2, float fWidth2, float fHeight2, HIT_TEST_TYPE hitTestType2)
{
	if ((hitTestType1 == HIT_TEST_TYPE_CIRCLE)
		&& (hitTestType2 == HIT_TEST_TYPE_CIRCLE))
	{// 当たり判定の種類がどちらも円だった時、
		// 円同士の衝突チェック
		return CheckHitCircleAndCircle(
			pos1, rot1, fWidth1, fHeight1,
			pos2, rot2, fWidth2, fHeight2);
	}
}

//========================================
// PositionUpdate関数 - 位置の更新処理 -
//========================================
void PositionUpdate(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove) 
{
	// 位置を更新
	pPos->x += pMove->x;
	pPos->y += pMove->y;

	// 移動量を更新(減衰)
	pMove->x += (0.0f - pMove->x) * 0.1f;
	pMove->y += (0.0f - pMove->y) * 0.1f;
}

//========================================
// Driving関数 - 推進処理 -
//========================================
void Driving(D3DXVECTOR3 *pPos, D3DXVECTOR3 rot, D3DXVECTOR3 *pMove, float fMove)
{
	// 向いている方向に移動量を加算
	pMove->x = sinf(rot.z) * fMove;
	pMove->y = cosf(rot.z) * fMove;

	// 位置を更新
	PositionUpdate(pPos, pMove);
}

//========================================
// TurnAround関数 - 方向転換処理 -
//========================================
void TurnAround(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation)
{
	// 角度を制御する
	ControlAngle(&pRot->z);

	float fAngleDiff	// 2点の角度の差
		= AngleDifference(
			pRot->z,
			FindAngle(pos, targetPos));

	if (fAngleDiff < 0.0f) 
	{// 角度の差がマイナス方向だった時、
		// マイナス方向に回転
		pRot->z -= fRotation;
	}
	if (fAngleDiff > 0.0f)
	{// 角度の差がプラス方向だった時、
		// プラス方向に回転
		pRot->z += fRotation;
	}
}

//========================================
// FindDistance関数 - 2点の距離を求める -
//========================================
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float x = pos1.x - pos2.x;	// 平方根のX辺
	float y = pos1.y - pos2.y;	// 平方根のY辺

	// 2点の距離を返す
	return sqrtf((x * x) + (y * y));
}

//========================================
// FindAngle関数 - 2点の角度を求める -
//========================================
float FindAngle(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos)
{
	float fAngle;	// 角度

	fAngle = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1;

	return fAngle;
}

//========================================
// AngleDifference関数 - 角度の差を求める -
//========================================
float AngleDifference(float fAngle, float fTargetAngle)
{
	float fAngleDifference = 0.0f;	// 角度の差

	// 角度を制御する
	ControlAngle(&fAngle);

	if ((fAngle >= 0.0f) && (fTargetAngle >= 0.0f))
	{// どちらの角度もプラスの時、
		// 角度の差を設定
		fAngleDifference = fTargetAngle - fAngle;

		return fAngleDifference;
	}
	else if ((fAngle <= 0.0f) && (fTargetAngle <= 0.0f))
	{// どちらの角度もマイナスの時、
		// 角度の差を設定
		fAngleDifference = fTargetAngle - fAngle;

		return fAngleDifference;
	}

	if ((fAngle > 0.0f) && (fTargetAngle < 0.0f))
	{// 角度がプラスで、目標角度がマイナスの時、
		if (fAngle - D3DX_PI < fTargetAngle)
		{// 目標角度が下側に近い時、
			// 角度の差を設定
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle - D3DX_PI > fTargetAngle)
		{// 目標角度が上側に近い時、
			// 角度の差を設定
			fAngleDifference = (D3DX_PI - fAngle) + (D3DX_PI + fTargetAngle);
		}
	}

	if ((fAngle < 0.0f) && (fTargetAngle > 0.0f))
	{// 角度がマイナスで、目標角度がプラスの時、
		if (fAngle + D3DX_PI > fTargetAngle)
		{// 目標角度が下側に近い時、
			// 角度の差を設定
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle + D3DX_PI < fTargetAngle)
		{// 目標角度が上側に近い時、
			// 角度の差を設定
			fAngleDifference = -(D3DX_PI + fAngle) - (D3DX_PI - fTargetAngle);
		}
	}

	return fAngleDifference;
}

//========================================
// ControlAngle関数 - 角度を制御する -
//========================================
void ControlAngle(float *pAngle)
{
	if (*pAngle > D3DX_PI)
	{// プラスの円周率を上回っている時、
		*pAngle = (D3DX_PI - (*pAngle - D3DX_PI)) * -1;
	}
	else if (*pAngle < -D3DX_PI)
	{// マイナスの円周率を下回っている時、
		*pAngle = (D3DX_PI + (*pAngle + D3DX_PI));
	}
}
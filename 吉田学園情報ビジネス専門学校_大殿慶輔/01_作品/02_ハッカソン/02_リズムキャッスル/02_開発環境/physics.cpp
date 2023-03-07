//========================================
// 
// 物理関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** physics.cpp ***
//========================================
#include "chunk.h"
#include "main.h"
#include "physics.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// 当たり判定時の小数の位
#define HIT_TEST_DECIMAL_PLACE	(20)

//****************************************
// プロトタイプ宣言
//****************************************
// 四角から四角の衝突チェック
void SquareSquareCollision(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo	, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo	, HitTestParts pairHitTestParts);

// 四角から四角の衝突チェック(見下ろしモード)
void SquareSquareCollisionLookDown(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo, HitTestParts pairHitTestParts);

// 点から四角の衝突チェック(見下ろしモード)
void PointSquareCollisionLookDown(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo, HitTestParts pairHitTestParts,
	bool bRflc, bool bSlip);

// 反射角を求める(見下ろし)
float FindReflectionAngleLookDown(D3DXVECTOR3 point, D3DXVECTOR3 posOld, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadVector関数 - ベクトルの読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadVector(FILE *pFile, D3DXVECTOR3 *pVector)
{
	fscanf(pFile, "%f", &pVector->x);
	fscanf(pFile, "%f", &pVector->y);
	fscanf(pFile, "%f", &pVector->z);
}

//========================================
// SquareSquareCollision関数 - 四角から四角の衝突チェック -
// Author:RIKU NISHIMURA
//========================================
void SquareSquareCollision(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo	, HitTestParts myHitTestParts, 
	CollisionInfo pairCollInfo	, HitTestParts pairHitTestParts)
{
	HitTest myHitTest	// 自分の当たり判定情報
			= myCollInfo.hitTest;
	HitTest pairHitTest	// 相手の当たり判定情報
			= pairCollInfo.hitTest;

	// 当たり判定の計算に必要なパラメーターを整数型に変換(演算誤差対策)
	int nMyX		= myCollInfo.pPos->x					* HIT_TEST_DECIMAL_PLACE;
	int nMyY		= myCollInfo.pPos->y					* HIT_TEST_DECIMAL_PLACE;
	int nMyPartsX	= myHitTestParts.fixedRelativePos.x		* HIT_TEST_DECIMAL_PLACE;
	int nMyPartsY	= myHitTestParts.fixedRelativePos.y		* HIT_TEST_DECIMAL_PLACE;
	int nMyOldX		= myCollInfo.posOld.x					* HIT_TEST_DECIMAL_PLACE;
	int nMyOldY		= myCollInfo.posOld.y					* HIT_TEST_DECIMAL_PLACE;
	int nMyWidth	= myHitTestParts.fWidth					* HIT_TEST_DECIMAL_PLACE;
	int nMyHeight	= myHitTestParts.fHeight				* HIT_TEST_DECIMAL_PLACE;
	int nPairX		= pairCollInfo.pPos->x					* HIT_TEST_DECIMAL_PLACE;
	int nPairY		= pairCollInfo.pPos->y					* HIT_TEST_DECIMAL_PLACE;
	int nPairPartsX = pairHitTestParts.fixedRelativePos.x	* HIT_TEST_DECIMAL_PLACE;
	int nPairPartsY = pairHitTestParts.fixedRelativePos.y	* HIT_TEST_DECIMAL_PLACE;
	int nPairOldX	= pairCollInfo.posOld.x					* HIT_TEST_DECIMAL_PLACE;
	int nPairOldY	= pairCollInfo.posOld.y					* HIT_TEST_DECIMAL_PLACE;
	int nPairWidth	= pairHitTestParts.fWidth				* HIT_TEST_DECIMAL_PLACE;
	int nPairHeight = pairHitTestParts.fHeight				* HIT_TEST_DECIMAL_PLACE;

	if (((nMyY + nMyPartsY) + (nMyHeight * 0.5f) > (nPairY + nPairPartsY) - (nPairHeight * 0.5f))
		&&
		((nMyY + nMyPartsY) - (nMyHeight * 0.5f) < (nPairY + nPairPartsY) + (nPairHeight * 0.5f)))
	{// 上辺と下辺が相手の幅の内側の時、
		if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))		// 現在の右辺が相手の左辺よりも右に位置している
			&&																								// &
			((nMyOldX + nMyPartsX) + (nMyWidth * 0.5f) <= (nPairOldX + nPairPartsX) - (nPairWidth * 0.5f)))	// 過去の右辺が相手の左辺よりも左に位置している
		{// 左からめり込んでいる時、

			if (!pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
			{// 左からのすり抜けが偽の時、
				if (pCollision != NULL)
				{// 衝突情報がNULLでない時、
					pCollision->aHitDirection[HIT_DIRECTION_LEFT] = true;	// 左方向からの衝突フラグを真にする
					pCollision->bHitDirection = true;						// 衝突フラグを真にする
				}
				myCollInfo.pPos->x =									// 位置を相手の当たり判定の左にする
					(nPairX + nPairPartsX - (nPairWidth * 0.5f) - nMyPartsX - (nMyWidth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
			}
		}
		else if (((nMyX + nMyPartsX) - (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f))	// 左辺が相手の右辺よりも左に位置している
			&&																								// &
			((nMyOldX + nMyPartsX) - (nMyWidth * 0.5f) >= (nPairOldX + nPairPartsX) + (nPairWidth * 0.5f)))	// 過去の左辺が相手の右辺よりも右に位置している
		{// 右からめり込んでいる時、

			if (!pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
			{// 右からのすり抜けが偽の時、
				if (pCollision != NULL)
				{// 衝突情報がNULLでない時、
					pCollision->aHitDirection[HIT_DIRECTION_RIGHT] = true;	// 右方向からの衝突フラグを真にする
					pCollision->bHitDirection = true;						// 衝突フラグを真にする
				}
				myCollInfo.pPos->x =									// 位置を相手の当たり判定の右にする
					(nPairX + nPairPartsX + (nPairWidth * 0.5f) - nMyPartsX + (nMyWidth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
			}
		}

		if (pCollision != NULL)
		{// 衝突情報がNULLでない時、
			if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
				&&
				((nMyX + nMyPartsX) + (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
			{// 現在の右辺が相手の当たり判定の内側の時、
				pCollision->aInside[HIT_DIRECTION_LEFT] = true;	// 左方向からの内側フラグを真にする
				pCollision->bHit = true;						// 当たりフラグを真にする
				pCollision->aHit[HIT_DIRECTION_LEFT] = true;	// 左方向からの当たりフラグを真にする

				if (pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
				{// 左からのすり抜けが真の時、
					pCollision->bGhost = true;						// すり抜けフラグを真にする
					pCollision->aGhost[HIT_DIRECTION_LEFT] = true;	// 左方向からのすり抜けフラグを真にする
				}
			}

			if (((nMyX + nMyPartsX) - (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
				&&
				((nMyX + nMyPartsX) - (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
			{// 現在の左辺が相手の当たり判定の内側の時、
				pCollision->aInside[HIT_DIRECTION_RIGHT] = true;	// 右方向からの内側フラグを真にする
				pCollision->bHit = true;							// 当たりフラグを真にする
				pCollision->aHit[HIT_DIRECTION_RIGHT] = true;		// 右方向からの当たりフラグを真にする

				if (pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
				{// 右からのすり抜けが真の時、
					pCollision->bGhost = true;						// すり抜けフラグを真にする
					pCollision->aGhost[HIT_DIRECTION_RIGHT] = true;	// 右方向からのすり抜けフラグを真にする
				}
			}
		}
	}

	if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) > (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
		&&
		((nMyX + nMyPartsX) - (nMyWidth * 0.5f) < (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
	{// 左辺と右辺が相手の幅の内側の時、
		if (((nMyY + nMyPartsY) + (nMyHeight * 0.5f) > (nPairY + nPairPartsY) - (nPairHeight * 0.5f))			// 足元が相手の上辺よりも下に位置している
			&&																									// &
			((nMyOldY + nMyPartsY) + (nMyHeight * 0.5f) <= (nPairOldY + nPairPartsY) - (nPairHeight * 0.5f)))	// 過去の足元が相手の上辺よりも上に位置している
		{// 上からめり込んでいる時、

			if (!pairHitTestParts.aGhost[HIT_DIRECTION_TOP])
			{// 上からのすり抜けが偽の時、
				if (pCollision != NULL)
				{// 衝突情報がNULLでない時、
					pCollision->aHitDirection[HIT_DIRECTION_TOP] = true;	// 上方向からの衝突フラグを真にする
					pCollision->bHitDirection = true;						// 衝突フラグを真にする
				}
				myCollInfo.pPos->y =	// 位置を相手の当たり判定の上にする
					(nPairY + nPairPartsY - (nPairHeight * 0.5f) - nMyPartsY - (nMyHeight * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
			}
		}
		else if (((nMyY + nMyPartsY) - (nMyHeight * 0.5f) < (nPairY + nPairPartsY) + (nPairHeight * 0.5f))		// 頭が相手の下辺よりも上に位置している
			&&																									// &
			((nMyOldY + nMyPartsY) - (nMyHeight * 0.5f) >= (nPairOldY + nPairPartsY) + (nPairHeight * 0.5f)))	// 過去の頭が相手の下辺よりも下に位置している
		{// 下からめり込んでいる時、

			if (!pairHitTestParts.aGhost[HIT_DIRECTION_BOTTOM])
			{// 下からのすり抜けが偽の時、
				if (pCollision != NULL)
				{// 衝突情報がNULLでない時、
					pCollision->aHitDirection[HIT_DIRECTION_BOTTOM] = true;	// 下方向からの衝突フラグを真にする
					pCollision->bHitDirection = true;						// 衝突フラグを真にする
				}
				myCollInfo.pPos->y = 									// 位置を相手の当たり判定の下にする
					(nPairY + nPairPartsY + (nPairHeight * 0.5f) - nMyPartsY + (nMyHeight * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
			}
		}

		if (pCollision != NULL)
		{// 衝突情報がNULLでない時、
			if (((nMyY + nMyPartsY) + (nMyHeight * 0.5f) >= (nPairY + nPairPartsY) - (nPairHeight * 0.5f))
				&&
				((nMyY + nMyPartsY) + (nMyHeight * 0.5f) <= (nPairY + nPairPartsY) + (nPairHeight * 0.5f)))
			{// 現在の下辺が相手の当たり判定の内側の時、
				pCollision->aInside[HIT_DIRECTION_TOP] = true;	// 上方向からの内側フラグを真にする

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_TOP])
				{// 上からのすり抜けが偽の時、
					pCollision->bHit = true;					// 当たりフラグを真にする
					pCollision->aHit[HIT_DIRECTION_TOP] = true;	// 上方向からの当たりフラグを真にする
				}
				else
				{// 上からのすり抜けが真の時、
					pCollision->bGhost = true;						// すり抜けフラグを真にする
					pCollision->aGhost[HIT_DIRECTION_TOP] = true;	// 上方向からのすり抜けフラグを真にする
				}
			}

			if (((nMyY + nMyPartsY) - (nMyHeight * 0.5f) >= (nPairY + nPairPartsY) - (nPairHeight * 0.5f))
				&&
				((nMyY + nMyPartsY) - (nMyHeight * 0.5f) <= (nPairY + nPairPartsY) + (nPairHeight * 0.5f)))
			{// 現在の上辺が相手の当たり判定の内側の時、
				pCollision->aInside[HIT_DIRECTION_BOTTOM] = true;	// 下方向からの内側フラグを真にする

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_BOTTOM])
				{// 下からのすり抜けが偽の時、
					pCollision->bHit = true;						// 当たりフラグを真にする
					pCollision->aHit[HIT_DIRECTION_BOTTOM] = true;	// 下方向からの当たりフラグを真にする
				}
				else
				{// 下からのすり抜けが真の時、
					pCollision->bGhost = true;							// すり抜けフラグを真にする
					pCollision->aGhost[HIT_DIRECTION_BOTTOM] = true;	// 下方向からのすり抜けフラグを真にする
				}
			}
		}
	}
}

//========================================
// SquareSquareCollisionLookDown関数 - 四角から四角の衝突チェック(見下ろしモード) -
// Author:RIKU NISHIMURA
//========================================
void SquareSquareCollisionLookDown(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo, HitTestParts pairHitTestParts)
{
	HitTest myHitTest	// 自分の当たり判定情報
			= myCollInfo.hitTest;
	HitTest pairHitTest	// 相手の当たり判定情報
			= pairCollInfo.hitTest;

	// 当たり判定の計算に必要なパラメーターを整数型に変換(演算誤差対策)
	int nMyX		= myCollInfo.pPos->x					* HIT_TEST_DECIMAL_PLACE;
	int nMyZ		= myCollInfo.pPos->z					* HIT_TEST_DECIMAL_PLACE;
	int nMyPartsX	= myHitTestParts.fixedRelativePos.x		* HIT_TEST_DECIMAL_PLACE;
	int nMyPartsZ	= myHitTestParts.fixedRelativePos.z		* HIT_TEST_DECIMAL_PLACE;
	int nMyOldX		= myCollInfo.posOld.x					* HIT_TEST_DECIMAL_PLACE;
	int nMyOldZ		= myCollInfo.posOld.z					* HIT_TEST_DECIMAL_PLACE;
	int nMyWidth	= myHitTestParts.fWidth					* HIT_TEST_DECIMAL_PLACE;
	int nMyDepth	= myHitTestParts.fDepth					* HIT_TEST_DECIMAL_PLACE;
	int nPairX		= pairCollInfo.pPos->x					* HIT_TEST_DECIMAL_PLACE;
	int nPairZ		= pairCollInfo.pPos->z					* HIT_TEST_DECIMAL_PLACE;
	int nPairPartsX = pairHitTestParts.fixedRelativePos.x	* HIT_TEST_DECIMAL_PLACE;
	int nPairPartsZ = pairHitTestParts.fixedRelativePos.z	* HIT_TEST_DECIMAL_PLACE;
	int nPairOldX	= pairCollInfo.posOld.x					* HIT_TEST_DECIMAL_PLACE;
	int nPairOldZ	= pairCollInfo.posOld.z					* HIT_TEST_DECIMAL_PLACE;
	int nPairWidth	= pairHitTestParts.fWidth				* HIT_TEST_DECIMAL_PLACE;
	int nPairDepth	= pairHitTestParts.fDepth				* HIT_TEST_DECIMAL_PLACE;

	switch (vector)
	{
		//========== *** X方向 ***
	case VECTOR_X:
	{
		if (((nMyZ + nMyPartsZ) + (nMyDepth * 0.5f) > (nPairZ + nPairPartsZ) - (nPairDepth * 0.5f))
			&&
			((nMyZ + nMyPartsZ) - (nMyDepth * 0.5f) < (nPairZ + nPairPartsZ) + (nPairDepth * 0.5f)))
		{// 奥辺と手前辺が相手の幅の内側の時、
			if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))		// 現在の右辺が相手の左辺よりも右に位置している
				&&																								// &
				((nMyOldX + nMyPartsX) + (nMyWidth * 0.5f) <= (nPairOldX + nPairPartsX) - (nPairWidth * 0.5f)))	// 過去の右辺が相手の左辺よりも左に位置している
			{// 左からめり込んでいる時、

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
				{// 左からのすり抜けが偽の時、
					if (pCollision != NULL)
					{// 衝突情報がNULLでない時、
						pCollision->bHitDirection =	// 衝突フラグを真にする
						pCollision->aHitDirection[HIT_DIRECTION_LEFT] = true;
					}
					myCollInfo.pPos->x =	// 位置を相手の当たり判定の左にする
						(nPairX + nPairPartsX - (nPairWidth * 0.5f) - nMyPartsX - (nMyWidth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
				}
			}
			else if (((nMyX + nMyPartsX) - (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f))	// 左辺が相手の右辺よりも左に位置している
				&&																								// &
				((nMyOldX + nMyPartsX) - (nMyWidth * 0.5f) >= (nPairOldX + nPairPartsX) + (nPairWidth * 0.5f)))	// 過去の左辺が相手の右辺よりも右に位置している
			{// 右からめり込んでいる時、

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
				{// 左からのすり抜けが偽の時、
					if (pCollision != NULL)
					{// 衝突情報がNULLでない時、
						pCollision->bHitDirection =	// 衝突フラグを真にする
						pCollision->aHitDirection[HIT_DIRECTION_RIGHT] = true;
					}
					myCollInfo.pPos->x =	// 位置を相手の当たり判定の右にする
						(nPairX + nPairPartsX + (nPairWidth * 0.5f) - nMyPartsX + (nMyWidth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
				}
			}

			if (pCollision != NULL)
			{// 衝突情報がNULLでない時、
				if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
					&&
					((nMyX + nMyPartsX) + (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
				{// 現在の右辺が相手の当たり判定の内側の時、
					pCollision->aInside[HIT_DIRECTION_LEFT] = true;	// 左方向からの内側フラグを真にする
					
					if (pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
					{// 左からのすり抜けが真の時、
						pCollision->bGhost = true;						// すり抜けフラグを真にする
						pCollision->aGhost[HIT_DIRECTION_LEFT] = true;	// 左方向からのすり抜けフラグを真にする
					}
					else
					{// 右からのすり抜けが偽の時、
						pCollision->bHit = true;						// 当たりフラグを真にする
						pCollision->aHit[HIT_DIRECTION_LEFT] = true;	// 左方向からの当たりフラグを真にする
					}
				}

				if (((nMyX + nMyPartsX) - (nMyWidth * 0.5f) >= (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
					&&
					((nMyX + nMyPartsX) - (nMyWidth * 0.5f) <= (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
				{// 現在の左辺が相手の当たり判定の内側の時、
					pCollision->aInside[HIT_DIRECTION_RIGHT] = true;	// 右方向からの内側フラグを真にする
					
					if (pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
					{// 右からのすり抜けが真の時、
						pCollision->bGhost = true;						// すり抜けフラグを真にする
						pCollision->aGhost[HIT_DIRECTION_RIGHT] = true;	// 右方向からのすり抜けフラグを真にする
					}
					else
					{// 右からのすり抜けが偽の時、
						pCollision->bHit = true;							// 当たりフラグを真にする
						pCollision->aHit[HIT_DIRECTION_RIGHT] = true;		// 右方向からの当たりフラグを真にする
					}
				}
			}
		}
	}
		break;
		//========== *** Z方向 ***
	case VECTOR_Z:
	{
		if (((nMyX + nMyPartsX) + (nMyWidth * 0.5f) > (nPairX + nPairPartsX) - (nPairWidth * 0.5f))
			&&
			((nMyX + nMyPartsX) - (nMyWidth * 0.5f) < (nPairX + nPairPartsX) + (nPairWidth * 0.5f)))
		{// 左辺と右辺が相手の幅の内側の時、
			if (((nMyZ + nMyPartsZ) + (nMyDepth * 0.5f) > (nPairZ + nPairPartsZ) - (nPairDepth * 0.5f))			// 足元が相手の奥辺よりも手前に位置している
				&&																								// &
				((nMyOldZ + nMyPartsZ) + (nMyDepth * 0.5f) <= (nPairOldZ + nPairPartsZ) - (nPairDepth * 0.5f)))	// 過去の足元が相手の奥辺よりも奥に位置している
			{// 奥からめり込んでいる時、

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_BACK])
				{// 左からのすり抜けが偽の時、
					if (pCollision != NULL)
					{// 衝突情報がNULLでない時、
						pCollision->bHitDirection =	// 衝突フラグを真にする
						pCollision->aHitDirection[HIT_DIRECTION_BACK] = true;
					}
					myCollInfo.pPos->z = 	// 位置を相手の当たり判定の奥にする
						(nPairZ + nPairPartsZ - (nPairDepth * 0.5f) - nMyPartsZ - (nMyDepth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
				}
			}
			else if (((nMyZ + nMyPartsZ) - (nMyDepth * 0.5f) < (nPairZ + nPairPartsZ) + (nPairDepth * 0.5f))	// 頭が相手の手前辺よりも奥に位置している
				&&																								// &
				((nMyOldZ + nMyPartsZ) - (nMyDepth * 0.5f) >= (nPairOldZ + nPairPartsZ) + (nPairDepth * 0.5f)))	// 過去の頭が相手の手前辺よりも手前に位置している
			{// 手前からめり込んでいる時、

				if (!pairHitTestParts.aGhost[HIT_DIRECTION_FRONT])
				{// 手前からのすり抜けが偽の時、
					if (pCollision != NULL)
					{// 衝突情報がNULLでない時、
						pCollision->bHitDirection =	// 衝突フラグを真にする
							pCollision->aHitDirection[HIT_DIRECTION_FRONT] = true;
					}
					myCollInfo.pPos->z = // 位置を相手の当たり判定の手前にする
						(nPairZ + nPairPartsZ + (nPairDepth * 0.5f) - nMyPartsZ + (nMyDepth * 0.5f)) / HIT_TEST_DECIMAL_PLACE;
				}
			}

			if (pCollision != NULL)
			{// 衝突情報がNULLでない時、
				if (((nMyZ + nMyPartsZ) + (nMyDepth * 0.5f) >= (nPairZ + nPairPartsZ) - (nPairDepth * 0.5f))
					&&
					((nMyZ + nMyPartsZ) + (nMyDepth * 0.5f) <= (nPairZ + nPairPartsZ) + (nPairDepth * 0.5f)))
				{// 現在の手前辺が相手の当たり判定の内側の時、
					pCollision->aInside[HIT_DIRECTION_BACK] = true;	// 奥方向からの内側フラグを真にする
					
					if (pairHitTestParts.aGhost[HIT_DIRECTION_BACK])
					{// 奥からのすり抜けが真の時、
						pCollision->bGhost = true;						// すり抜けフラグを真にする
						pCollision->aGhost[HIT_DIRECTION_BACK] = true;	// 奥方向からのすり抜けフラグを真にする
					}
					else 
					{// 奥からのすり抜けが偽の時、
						pCollision->bHit = true;						// 当たりフラグを真にする
						pCollision->aHit[HIT_DIRECTION_BACK] = true;	// 奥方向からの当たりフラグを真にする
					}
				}

				if (((nMyZ + nMyPartsZ) - (nMyDepth * 0.5f) >= (nPairZ + nPairPartsZ) - (nPairDepth * 0.5f))
					&&
					((nMyZ + nMyPartsZ) - (nMyDepth * 0.5f) <= (nPairZ + nPairPartsZ) + (nPairDepth * 0.5f)))
				{// 現在の奥辺が相手の当たり判定の内側の時、
					pCollision->aInside[HIT_DIRECTION_FRONT] = true;	// 手前方向からの内側フラグを真にする
					
					if (pairHitTestParts.aGhost[HIT_DIRECTION_FRONT])
					{// 手前からのすり抜けが真の時、
						pCollision->bGhost = true;						// すり抜けフラグを真にする
						pCollision->aGhost[HIT_DIRECTION_FRONT] = true;	// 手前方向からのすり抜けフラグを真にする
					}
					else
					{// 手前からのすり抜けが偽の時、
						pCollision->bHit = true;						// 当たりフラグを真にする
						pCollision->aHit[HIT_DIRECTION_FRONT] = true;	// 手前方向からの当たりフラグを真にする
					}
				}
			}
		}
	}
		break;
	}
}

//========================================
// PointSquareCollisionLookDown関数 - 点から四角の衝突チェック(見下ろしモード) -
// Author:RIKU NISHIMURA
//========================================
void PointSquareCollisionLookDown(VECTOR vector, Collision *pCollision,
	CollisionInfo myCollInfo, HitTestParts myHitTestParts,
	CollisionInfo pairCollInfo, HitTestParts pairHitTestParts,
	bool bRflc, bool bSlip)
{
	float fMyScale	= (myHitTestParts.fWidth * myHitTestParts.fDepth) * 0.5f;	// 自身の大きさ
	float fWidth	= pairHitTestParts.fWidth + (fMyScale * 0.5f);				// 幅
	float fDepth	= pairHitTestParts.fDepth + (fMyScale * 0.5f);				// 奥行き

	D3DXVECTOR3 pos0, pos1, pos2, pos3;	// 時計回りに見て右下からの4頂点の位置(相手)
	pos0.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (sinf(pairCollInfo.pRot->y) * (fDepth * -0.5f));
	pos0.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (cosf(pairCollInfo.pRot->y) * (fDepth * -0.5f));
	pos1.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (sinf(pairCollInfo.pRot->y) * (fDepth * -0.5f));
	pos1.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (cosf(pairCollInfo.pRot->y) * (fDepth * -0.5f));
	pos2.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (sinf(pairCollInfo.pRot->y) * (fDepth * 0.5f));
	pos2.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (cosf(pairCollInfo.pRot->y) * (fDepth * 0.5f));
	pos3.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (sinf(pairCollInfo.pRot->y) * (fDepth * 0.5f));
	pos3.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.pRot->y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (cosf(pairCollInfo.pRot->y) * (fDepth * 0.5f));
	D3DXVECTOR3 posOld0, posOld1, posOld2, posOld3;	// 時計回りに見て右下からの4頂点の過去の位置(相手)
	posOld0.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (sinf(pairCollInfo.rotOld.y) * (fDepth * -0.5f));
	posOld0.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (cosf(pairCollInfo.rotOld.y) * (fDepth * -0.5f));
	posOld1.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (sinf(pairCollInfo.rotOld.y) * (fDepth * -0.5f));
	posOld1.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (cosf(pairCollInfo.rotOld.y) * (fDepth * -0.5f));
	posOld2.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (sinf(pairCollInfo.rotOld.y) * (fDepth * 0.5f));
	posOld2.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth *  0.5f)) + (cosf(pairCollInfo.rotOld.y) * (fDepth * 0.5f));
	posOld3.x = (pairHitTestParts.fixedRelativePos.x + pairCollInfo.pPos->x) + (sinf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (sinf(pairCollInfo.rotOld.y) * (fDepth * 0.5f));
	posOld3.z = (pairHitTestParts.fixedRelativePos.z + pairCollInfo.pPos->z) + (cosf(pairCollInfo.rotOld.y - (D3DX_PI * 0.5f)) * (fWidth * -0.5f)) + (cosf(pairCollInfo.rotOld.y) * (fDepth * 0.5f));

	bool aInside[HIT_DIRECTION_MAX];	// 内側フラグ

	// 奥辺
	{
		D3DXVECTOR3 vecToPos	= *myCollInfo.pPos - pos2;
		D3DXVECTOR3 vecToPosOld = myCollInfo.posOld - posOld2;
		D3DXVECTOR3 point =	// 交点の座標
			FindIntersectionPointLookDown(myCollInfo.posOld, *myCollInfo.pPos, pos2, pos3);
		float	fDistance3toP = FindDistanceLookDown(pos3, point);	// 頂点3から交点までの距離
		float	fDistance2toP = FindDistanceLookDown(pos2, point);	// 頂点2から交点までの距離
		float	fDistance3to2 = FindDistanceLookDown(pos3, pos2);	// 頂点3から頂点2までの距離

		// 奥辺からの内側フラグを取得
		aInside[HIT_DIRECTION_BACK] = (((pos3 - pos2).z * vecToPos.x) - ((pos3 - pos2).x * vecToPos.z) >= 0);

		if (((aInside[HIT_DIRECTION_BACK]) && (((posOld3 - posOld2).z * vecToPosOld.x) - ((posOld3 - posOld2).x * vecToPosOld.z) <= 0))
			&&
			(fDistance3toP <= fDistance3to2)
			&&
			(fDistance2toP <= fDistance3to2))
		{// 奥辺から当たっている時、
			if (pCollision != NULL) {
				pCollision->bHitDirection =	// 衝突フラグを真にする
				pCollision->aHitDirection[HIT_DIRECTION_BACK] = true;
				pCollision->fLength	// めり込み距離を設定
					= FindDistanceLookDown(point, *myCollInfo.pPos);

				if (pairHitTestParts.aGhost[HIT_DIRECTION_BACK])
				{// 相手のすり抜けフラグが真の時、
					pCollision->bGhost =	// すり抜けフラグを真にする
					pCollision->aGhost[HIT_DIRECTION_BACK] = true;
					return;
				}
				else
				{// 相手のすり抜けフラグが偽の時、
					pCollision->bHit =	// 当たりフラグを真にする
					pCollision->aHit[HIT_DIRECTION_BACK] = true;
				}
			}

			if (bSlip)
			{// 滑りフラグが真の時、
				float fAngle = FindAngleLookDown(pos2, pos3) - (D3DX_PI * 0.5f);	// 向き

				// 位置を更新
				D3DXVECTOR3 pos = FindIntersectionPointLookDown(
					*myCollInfo.pPos,
					*myCollInfo.pPos + D3DXVECTOR3(sinf(fAngle),0.0f,cosf(fAngle)),
					pos3,
					pos2);
				myCollInfo.pPos->x = pos.x + sinf(fAngle) * 0.01f;
				myCollInfo.pPos->z = pos.z + cosf(fAngle) * 0.01f;
			}
			else
			{// 滑りフラグが偽の時、
				// 位置を更新
				myCollInfo.pPos->x = point.x;
				myCollInfo.pPos->z = point.z;
			}

			if ((bRflc) && (myCollInfo.pRot != NULL))
			{// 反射フラグが真 & 向きのポインタがNULLでない時、
				// 向きを反射させる
				myCollInfo.pRot->y = FindReflectionAngleLookDown(point, myCollInfo.posOld, pos3, pos2);
				
				// 反射方向に微量加算(埋まり込み防止)
				myCollInfo.pPos->x += sinf(myCollInfo.pRot->y);
				myCollInfo.pPos->z += cosf(myCollInfo.pRot->y);
			}

			return;
		}
	}

	// 手前辺
	{
		D3DXVECTOR3 vecToPos	= *myCollInfo.pPos - pos0;
		D3DXVECTOR3 vecToPosOld = myCollInfo.posOld - posOld0;
		D3DXVECTOR3 point =	// 交点の座標
			FindIntersectionPointLookDown(myCollInfo.posOld, *myCollInfo.pPos, pos0, pos1);
		float	fDistance1toP = FindDistanceLookDown(pos1, point);	// 頂点1から交点までの距離
		float	fDistance0toP = FindDistanceLookDown(pos0, point);	// 頂点0から交点までの距離
		float	fDistance1to0 = FindDistanceLookDown(pos1, pos0);	// 頂点1から頂点0までの距離

		// 手前辺からの内側フラグを取得
		aInside[HIT_DIRECTION_FRONT] = (((pos1 - pos0).z * vecToPos.x) - ((pos1 - pos0).x * vecToPos.z) >= 0);

		if (((aInside[HIT_DIRECTION_FRONT]) && (((posOld1 - posOld0).z * vecToPosOld.x) - ((posOld1 - posOld0).x * vecToPosOld.z) <= 0))
			&&
			(fDistance1toP <= fDistance1to0)
			&&
			(fDistance0toP <= fDistance1to0))
		{// 手前辺から当たっている時、
			if (pCollision != NULL) {
				pCollision->bHitDirection =	// 衝突フラグを真にする
				pCollision->aHitDirection[HIT_DIRECTION_FRONT] = true;
				pCollision->fLength	// めり込み距離を設定
					= FindDistanceLookDown(point, *myCollInfo.pPos);

				if (pairHitTestParts.aGhost[HIT_DIRECTION_FRONT])
				{// 相手のすり抜けフラグが真の時、
					pCollision->bGhost =	// すり抜けフラグを真にする
					pCollision->aGhost[HIT_DIRECTION_FRONT] = true;
					return;
				}
				else
				{// 相手のすり抜けフラグが偽の時、
					pCollision->bHit =	// 当たりフラグを真にする
					pCollision->aHit[HIT_DIRECTION_FRONT] = true;
				}
			}

			if(bSlip)
			{// 滑りフラグが真の時、
				float fAngle = FindAngleLookDown(pos0, pos1) - (D3DX_PI * 0.5f);	// 向き

				// 位置を更新
				D3DXVECTOR3 pos = FindIntersectionPointLookDown(
					*myCollInfo.pPos,
					*myCollInfo.pPos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)),
					pos1,
					pos0);
				myCollInfo.pPos->x = pos.x + sinf(fAngle) * 0.01f;
				myCollInfo.pPos->z = pos.z + cosf(fAngle) * 0.01f;
			}
			else
			{// 滑りフラグが偽の時、
				// 位置を更新
				myCollInfo.pPos->x = point.x;
				myCollInfo.pPos->z = point.z;
			}

			if ((bRflc) && (myCollInfo.pRot != NULL))
			{// 反射フラグが真 & 向きのポインタがNULLでない時、
				// 向きを反射させる
				myCollInfo.pRot->y = FindReflectionAngleLookDown(point, myCollInfo.posOld, pos1, pos0);

				// 反射方向に微量加算(埋まり込み防止)
				myCollInfo.pPos->x += sinf(myCollInfo.pRot->y);
				myCollInfo.pPos->z += cosf(myCollInfo.pRot->y);
			}

			return;
		}
	}

	// 左辺
	{
		D3DXVECTOR3 vecToPos	= *myCollInfo.pPos - pos1;
		D3DXVECTOR3 vecToPosOld = myCollInfo.posOld - posOld1;
		D3DXVECTOR3 point =	// 交点の座標
					FindIntersectionPointLookDown(myCollInfo.posOld, *myCollInfo.pPos, pos1, pos2);
		float	fDistance2toP = FindDistanceLookDown(pos2, point);	// 頂点2から交点までの距離
		float	fDistance1toP = FindDistanceLookDown(pos1, point);	// 頂点1から交点までの距離
		float	fDistance2to1 = FindDistanceLookDown(pos2, pos1);	// 頂点2から頂点1までの距離

		// 左辺からの内側フラグを取得
		aInside[HIT_DIRECTION_LEFT] = (((pos2 - pos1).z * vecToPos.x) - ((pos2 - pos1).x * vecToPos.z) >= 0);

		if (((aInside[HIT_DIRECTION_LEFT]) && (((posOld2 - posOld1).z * vecToPosOld.x) - ((posOld2 - posOld1).x * vecToPosOld.z) <= 0))
			&&
			(FindDistanceLookDown(pos2, point) <= FindDistanceLookDown(pos2, pos1))
			&&
			(FindDistanceLookDown(pos1, point) <= FindDistanceLookDown(pos2, pos1)))
		{// 左辺から当たっている時、
			if (pCollision != NULL) {
				pCollision->bHitDirection =	// 衝突フラグを真にする
				pCollision->aHitDirection[HIT_DIRECTION_LEFT] = true;
				pCollision->fLength	// めり込み距離を設定
					= FindDistanceLookDown(point, *myCollInfo.pPos);

				if (pairHitTestParts.aGhost[HIT_DIRECTION_LEFT])
				{// 相手のすり抜けフラグが真の時、
					pCollision->bGhost =	// すり抜けフラグを真にする
					pCollision->aGhost[HIT_DIRECTION_LEFT] = true;
					return;
				}
				else
				{// 相手のすり抜けフラグが偽の時、
					pCollision->bHit =	// 当たりフラグを真にする
					pCollision->aHit[HIT_DIRECTION_LEFT] = true;
				}
			}

			if (bSlip)
			{// 滑りフラグが真の時、
				float fAngle = FindAngleLookDown(pos1, pos2) - (D3DX_PI * 0.5f);	// 向き

				// 位置を更新
				D3DXVECTOR3 pos = FindIntersectionPointLookDown(
					*myCollInfo.pPos,
					*myCollInfo.pPos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)),
					pos2,
					pos1);
				myCollInfo.pPos->x = pos.x + sinf(fAngle) * 0.01f;
				myCollInfo.pPos->z = pos.z + cosf(fAngle) * 0.01f;
			}
			else
			{// 滑りフラグが偽の時、
				// 位置を更新
				myCollInfo.pPos->x = point.x;
				myCollInfo.pPos->z = point.z;
			}

			if ((bRflc) && (myCollInfo.pRot != NULL))
			{// 反射フラグが真 & 向きのポインタがNULLでない時、
				// 向きを反射させる
				myCollInfo.pRot->y = FindReflectionAngleLookDown(point, myCollInfo.posOld, pos2, pos1);

				// 反射方向に微量加算(埋まり込み防止)
				myCollInfo.pPos->x += sinf(myCollInfo.pRot->y);
				myCollInfo.pPos->z += cosf(myCollInfo.pRot->y);
			}

			return;
		}
	}
	
	// 右辺
	{
		D3DXVECTOR3 vecToPos	= *myCollInfo.pPos - pos3;
		D3DXVECTOR3 vecToPosOld = myCollInfo.posOld - posOld3;
		D3DXVECTOR3 point =	// 交点の座標
			FindIntersectionPointLookDown(myCollInfo.posOld, *myCollInfo.pPos, pos3, pos0);
		float	fDistance0toP = FindDistanceLookDown(pos0, point);	// 頂点0から交点までの距離
		float	fDistance3toP = FindDistanceLookDown(pos3, point);	// 頂点3から交点までの距離
		float	fDistance0to3 = FindDistanceLookDown(pos0, pos3);	// 頂点0から頂点3までの距離

		// 右辺からの内側フラグを取得
		aInside[HIT_DIRECTION_RIGHT] = (((pos0 - pos3).z * vecToPos.x) - ((pos0 - pos3).x * vecToPos.z) >= 0);

		if (((aInside[HIT_DIRECTION_RIGHT]) && (((posOld0 - posOld3).z * vecToPosOld.x) - ((posOld0 - posOld3).x * vecToPosOld.z) <= 0))
			&&
			(fDistance0toP <= fDistance0to3)
			&&
			(fDistance3toP <= fDistance0to3))
		{// 右辺から当たっている時、
			if (pCollision != NULL) {
				pCollision->bHitDirection =	// 衝突フラグを真にする
				pCollision->aHitDirection[HIT_DIRECTION_RIGHT] = true;
				pCollision->fLength	// めり込み距離を設定
					= FindDistanceLookDown(point, *myCollInfo.pPos);
				
				if (pairHitTestParts.aGhost[HIT_DIRECTION_RIGHT])
				{// 相手のすり抜けフラグが真の時、
					pCollision->bGhost =	// すり抜けフラグを真にする
					pCollision->aGhost[HIT_DIRECTION_RIGHT] = true;
					return;
				}
				else
				{// 相手のすり抜けフラグが偽の時、
					pCollision->bHit =	// 当たりフラグを真にする
					pCollision->aHit[HIT_DIRECTION_RIGHT] = true;
				}
			}

			if (bSlip)
			{// 滑りフラグが真の時、
				float fAngle = FindAngleLookDown(pos3, pos0) - (D3DX_PI * 0.5f);	// 向き

				// 位置を更新
				D3DXVECTOR3 pos = FindIntersectionPointLookDown(
					*myCollInfo.pPos,
					*myCollInfo.pPos + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)),
					pos0,
					pos3);
				myCollInfo.pPos->x = pos.x + sinf(fAngle) * 0.01f;
				myCollInfo.pPos->z = pos.z + cosf(fAngle) * 0.01f;
			}
			else
			{// 滑りフラグが偽の時、
				// 位置を更新
				myCollInfo.pPos->x = point.x;
				myCollInfo.pPos->z = point.z;
			}

			if ((bRflc) && (myCollInfo.pRot != NULL))
			{// 反射フラグが真 & 向きのポインタがNULLでない時、
				// 向きを反射させる
				myCollInfo.pRot->y = FindReflectionAngleLookDown(point, myCollInfo.posOld, pos0, pos3);

				// 反射方向に微量加算(埋まり込み防止)
				myCollInfo.pPos->x += sinf(myCollInfo.pRot->y);
				myCollInfo.pPos->z += cosf(myCollInfo.pRot->y);
			}

			return;
		}
	}

	if (pCollision != NULL) {
		if (aInside[HIT_DIRECTION_BACK] &&
			aInside[HIT_DIRECTION_FRONT] &&
			aInside[HIT_DIRECTION_LEFT] &&
			aInside[HIT_DIRECTION_RIGHT])
		{// 全ての方向から内側の時、
			pCollision->bInside = true;	// 当たりフラグを真にする
		}
	}
}

//========================================
// FindReflectionAngleLookDown関数 - 反射角を求める(見下ろし) -
// Author:RIKU NISHIMURA
//========================================
float FindReflectionAngleLookDown(D3DXVECTOR3 point,D3DXVECTOR3 posOld,D3DXVECTOR3 pos0,D3DXVECTOR3 pos1) 
{
	float fAngle = FindAngleLookDown(pos0, pos1) + (D3DX_PI * 0.5f);	// 壁の左端から右端までの角度

	// posOldから壁に対して垂直な交点から交点までの距離
	float fDistance = FindDistanceLookDown(
		FindIntersectionPointLookDown(	// posOldから壁に対して垂直な交点
			posOld,
			posOld + D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle)),
			pos0,
			pos1),
		point	// 交点
	);

	// 目標位置(交点からの角度を求める先)
	D3DXVECTOR3 targetPos = posOld;

	if (AngleDifference(FindAngleLookDown(pos0, pos1), FindAngleLookDown(point, posOld)) <= D3DX_PI * 0.5f)
	{// 壁に垂直の線に対して左側から衝突した時、
		targetPos.x += sinf(FindAngleLookDown(pos1, pos0)) * (fDistance * 2.0f);
		targetPos.z += cosf(FindAngleLookDown(pos1, pos0)) * (fDistance * 2.0f);
	}
	else
	{// 壁に垂直の線に対して右側から衝突した時、
		targetPos.x += sinf(FindAngleLookDown(pos0, pos1)) * (fDistance * 2.0f);
		targetPos.z += cosf(FindAngleLookDown(pos0, pos1)) * (fDistance * 2.0f);
	}

	// 反射角を返す
	return FindAngleLookDown(point, targetPos);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadHitTestSet関数 - 当たり判定設定情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadHitTestSet(FILE *pFile, HitTestSet *pHitTestSet)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];

	// 当たり判定の数を初期化
	pHitTestSet->nHitTestNum = 0;
	
	// 当たり判定設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "HITTESTSET_END"))	{ break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "HITTEST"))			{
			pHitTestSet->aHitTest[pHitTestSet->nHitTestNum].nHitTestPartsNum = 0;	// 当たり判定の部品数を初期化

			// 当たり判定情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "HITTEST_END"))	{
					pHitTestSet->nHitTestNum++;	// 当たり判定数を加算
					break;						// 読み込みを終了
				}
				else if (!strcmp(aDataSearch, "HITTEST_PARTS")) {
					HitTestParts	*pHitTest	// 部品の種類毎の情報のポインタ
									= &pHitTestSet->aHitTest[pHitTestSet->nHitTestNum].aHitTestParts[pHitTestSet->aHitTest[pHitTestSet->nHitTestNum].nHitTestPartsNum];

					// 当たり判定情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (!strcmp(aDataSearch, "HITTEST_PARTS_END")) {
							pHitTestSet->aHitTest[pHitTestSet->nHitTestNum].nHitTestPartsNum++;	// 当たり判定の部品数を加算
							break;																// 読み込みを終了
						}
						else if (!strcmp(aDataSearch, "TYPE:"))			{ fscanf(pFile, "%d", &pHitTest->hitTestForm); }	// 当たり判定の種類
						else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pHitTest->fWidth); }			// 幅
						else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pHitTest->fHeight); }		// 高さ
						else if (!strcmp(aDataSearch, "DEPTH:"))		{ fscanf(pFile, "%f", &pHitTest->fDepth); }			// 奥行き
						else if (!strcmp(aDataSearch, "RELATIVE_POS:")) {		// 相対位置
							fscanf(pFile, "%f", &pHitTest->fixedRelativePos.x);	// X
							fscanf(pFile, "%f", &pHitTest->fixedRelativePos.y);	// Y
							fscanf(pFile, "%f", &pHitTest->fixedRelativePos.z);	// Z
						}
						else if (!strcmp(aDataSearch, "GHOST:"))	{						// すり抜けフラグ
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_TOP]);		// 上辺
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_BOTTOM]);	// 下辺
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_LEFT]);		// 左辺
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_RIGHT]);	// 右辺
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_BACK]);		// 奥
							fscanf(pFile, "%d", &pHitTest->aGhost[HIT_DIRECTION_FRONT]);	// 手前
						}
					}
				}
			}
		}
	}
}

//========================================
// CollisionCheck関数 - 衝突チェック -
// Author:RIKU NISHIMURA
//========================================
void CollisionCheck(VECTOR vector, Collision *pCollision, CollisionInfo myCollInfo, CollisionInfo pairCollInfo, COLLCHK_MODE mode)
{
	for (int nCntMyParts = 0; nCntMyParts < myCollInfo.hitTest.nHitTestPartsNum; nCntMyParts++)
	{
		for (int nCntPairParts = 0; nCntPairParts < pairCollInfo.hitTest.nHitTestPartsNum; nCntPairParts++)
		{
			HIT_TEST_FORM	my					= myCollInfo.hitTest.aHitTestParts[nCntMyParts].hitTestForm;		// 自分の当たり判定の種類
			HIT_TEST_FORM	pair				= pairCollInfo.hitTest.aHitTestParts[nCntPairParts].hitTestForm;	// 相手の当たり判定の種類
			HitTestParts	myHitTestParts		= myCollInfo.hitTest.aHitTestParts[nCntMyParts];					// 自分の当たり判定の部品
			HitTestParts	pairHitTestParts	= pairCollInfo.hitTest.aHitTestParts[nCntPairParts];				// 相手の当たり判定の部品

			// モードに応じた処理
			switch (mode)
			{
				//========== *** 通常 ***
			case COLLCHK_MODE_NORMAL:
				/*/ 四角から四角 /*/if (my == HIT_TEST_FORM_SQUARE && pair == HIT_TEST_FORM_SQUARE) { SquareSquareCollision(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts); }
				break;
				//========== *** 見下ろし ***
			case COLLCHK_MODE_LOOKDOWN:
				/*/ 四角			から四角 /*/if (my == HIT_TEST_FORM_SQUARE			&& pair == HIT_TEST_FORM_SQUARE) { SquareSquareCollisionLookDown(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts); }
				/*/ 点				から四角 /*/if (my == HIT_TEST_FORM_POINT			&& pair == HIT_TEST_FORM_SQUARE) { PointSquareCollisionLookDown(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts, false,true); }
				/*/ 点(反射)		から四角 /*/if (my == HIT_TEST_FORM_POINT_RFLC		&& pair == HIT_TEST_FORM_SQUARE) { PointSquareCollisionLookDown(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts, true,false); }
				/*/ 点(滑り無し)	から四角 /*/if (my == HIT_TEST_FORM_POINT_NOSLIP	&& pair == HIT_TEST_FORM_SQUARE) { PointSquareCollisionLookDown(vector, pCollision, myCollInfo, myHitTestParts, pairCollInfo, pairHitTestParts, false,false); }
				break;
			}
		}
	}
}

//========================================
// InsideCheck関数 - 内側チェック -
// Author:RIKU NISHIMURA
//========================================
void InsideCheck(Collision *pCollision) 
{
	if (pCollision == NULL) 
	{// 衝突情報がNULLの時、
		// 処理を終了
		return;
	}

	pCollision->bInside = true;	// 内側フラグを真にする

	for (int nCntHitDirection = 0; nCntHitDirection < HIT_DIRECTION_MAX; nCntHitDirection++)
	{
		if (!pCollision->aInside[nCntHitDirection])
		{// カウントした内側フラグが偽の時、
			pCollision->bInside = false;	// 内側フラグ(統括)を偽にする
			break;							// 繰り返し処理を抜ける
		}
	}
}

//========================================
// CollisionOverwrite関数 - 衝突情報の上書き -
// Author:RIKU NISHIMURA
//========================================
void CollisionOverwrite(Collision *pOverwriteCollision, Collision collision) 
{
	pOverwriteCollision->bHit			|= collision.bHit;			// 当たりフラグ
	pOverwriteCollision->bHitDirection	|= collision.bHitDirection;	// 衝突フラグ
	pOverwriteCollision->bInside		|= collision.bInside;		// 内側フラグ
	pOverwriteCollision->bGhost			|= collision.bGhost;		// すり抜けフラグ

	// 方向毎の当たりフラグ/衝突フラグ/内側フラグ/すり抜けフラグを上書き
	for (int nCntDirection = 0; nCntDirection < HIT_DIRECTION_MAX; nCntDirection++)
	{
		pOverwriteCollision->aHit[nCntDirection]			|= collision.aHit[nCntDirection];
		pOverwriteCollision->aHitDirection[nCntDirection]	|= collision.aHitDirection[nCntDirection];
		pOverwriteCollision->aInside[nCntDirection]			|= collision.aInside[nCntDirection];
		pOverwriteCollision->aGhost[nCntDirection]			|= collision.aGhost[nCntDirection];
	}
}

//========================================
// PositionUpdate関数 - 位置の更新処理 -
// Author:RIKU NISHIMURA
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
// Author:RIKU NISHIMURA
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
// Author:RIKU NISHIMURA
//========================================
void TurnAround(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation)
{
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
// TurnAroundLookDown関数 - 方向転換処理(見下ろし) -
// Author:RIKU NISHIMURA
//========================================
void TurnAroundLookDown(D3DXVECTOR3 *pRot, D3DXVECTOR3 pos, D3DXVECTOR3 targetPos, float fRotation)
{
	float fAngleDiff	// 2点の角度の差
		= AngleDifference(
			pRot->y,
			FindAngleLookDown(pos, targetPos));

	if (fAngleDiff < 0.0f)
	{// 角度の差がマイナス方向だった時、
		// マイナス方向に回転
		pRot->y -= fRotation;
	}
	if (fAngleDiff > 0.0f)
	{// 角度の差がプラス方向だった時、
		// プラス方向に回転
		pRot->y += fRotation;
	}
}

//========================================
// FindDistance関数 - 2点の距離を求める -
// Author:RIKU NISHIMURA
//========================================
float FindDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float x = pos1.x - pos2.x;	// 平方根のX辺
	float y = pos1.y - pos2.y;	// 平方根のY辺

	// 2点の距離を返す
	return sqrtf((x * x) + (y * y));
}

//========================================
// FindDistanceLookDown関数 - 2点の距離を求める(見下ろし) -
// Author:RIKU NISHIMURA
//========================================
float FindDistanceLookDown(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float x = pos1.x - pos2.x;	// 平方根のX辺
	float z = pos1.z - pos2.z;	// 平方根のZ辺

	// 2点の距離を返す
	return sqrtf((x * x) + (z * z));
}

//========================================
// FindAngle関数 - 2点の角度を求める -
// Author:RIKU NISHIMURA
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
// FindAngleLookDown関数 - 2点の角度を求める(見下ろし) -
// Author:RIKU NISHIMURA
//========================================
float FindAngleLookDown(D3DXVECTOR3 pos, D3DXVECTOR3 targetPos)
{
	float fAngle;	// 角度

	fAngle = atan2f(targetPos.z - pos.z, targetPos.x - pos.x);

	fAngle -= (D3DX_PI * 0.5f);
	fAngle *= -1;

	return fAngle;
}


//========================================
// FindIntersectionPointLookDown関数 - 交点を求める(見下ろし) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 FindIntersectionPointLookDown(D3DXVECTOR3 posA1, D3DXVECTOR3 posA2, D3DXVECTOR3 posB1, D3DXVECTOR3 posB2)
{
	float fVecA1 = FindDistanceLookDown(posA2, posA1);	// ベクトルa1
	float fVecA2 = FindDistanceLookDown(posB1, posB2);	// ベクトルa2
	float fVecB1 = FindDistanceLookDown(posA1, posB1);	// ベクトルb1
	float fVecB2 = FindDistanceLookDown(posB2, posA1);	// ベクトルb2

	float fAreaS1 = (	// 面積s1
		((posA2.x - posA1.x) * (posB2.z - posA1.z)) -
		((posA2.z - posA1.z) * (posB2.x - posA1.x))) * 0.5f;
	float fAreaS2 = (	// 面積s2
		((posA2.x - posA1.x) * (posA1.z - posB1.z)) -
		((posA2.z - posA1.z) * (posA1.x - posB1.x))) * 0.5f;

	// 交点の座標を返す
	return	D3DXVECTOR3(
			posB2.x + (((posB1.x - posB2.x) * fAreaS1) / (fAreaS1 + fAreaS2)),
			0.0f,
			posB2.z + (((posB1.z - posB2.z) * fAreaS1) / (fAreaS1 + fAreaS2)));
}

//========================================
// AngleDifference関数 - 角度の差を求める -
// Author:RIKU NISHIMURA
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

	if ((fAngle >= 0.0f) && (fTargetAngle <= 0.0f))
	{// 角度がプラスで、目標角度がマイナスの時、
		if (fAngle - D3DX_PI <= fTargetAngle)
		{// 目標角度が下側に近い時、
			// 角度の差を設定
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle - D3DX_PI >= fTargetAngle)
		{// 目標角度が上側に近い時、
			// 角度の差を設定
			fAngleDifference = (D3DX_PI - fAngle) + (D3DX_PI + fTargetAngle);
		}
	}

	if ((fAngle <= 0.0f) && (fTargetAngle >= 0.0f))
	{// 角度がマイナスで、目標角度がプラスの時、
		if (fAngle + D3DX_PI >= fTargetAngle)
		{// 目標角度が下側に近い時、
			// 角度の差を設定
			fAngleDifference = fTargetAngle - fAngle;
		}
		else if (fAngle + D3DX_PI <= fTargetAngle)
		{// 目標角度が上側に近い時、
			// 角度の差を設定
			fAngleDifference = -(D3DX_PI + fAngle) - (D3DX_PI - fTargetAngle);
		}
	}

	return fAngleDifference;
}

//========================================
// ControlAngle関数 - 角度を制御する -
// Author:RIKU NISHIMURA
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

//========================================
// ConvertAngleToDirection関数 - 角度を指定された数の方向に変換する -
// Author:RIKU NISHIMURA
//========================================
float ConvertAngleToDirection(float fAngle, int nDirection)
{
	float fRange = ((D3DX_PI * 2) / nDirection);	// 方向毎の角度幅

	for (int nCntDirection = 0; nCntDirection < nDirection + 1; nCntDirection++)
	{
		if ((fAngle >= -D3DX_PI - (fRange * 0.5f) + (fRange * nCntDirection))
			&&
			(fAngle <= -D3DX_PI + (fRange * 0.5f) + (fRange * nCntDirection)))
		{// カウントの方向の角度幅の内側の時、
			fAngle = -D3DX_PI + (fRange * nCntDirection);	// 角度を設定
			break;	// 繰り返し処理を抜ける
		}
	}

	return fAngle;
}

//========================================
// ConvToBirdsEyeView関数 - 位置を鳥瞰図に変換する(Z⇔Y) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvPosToBirdsEyeView(D3DXVECTOR3 pos)
{
	float fYTemp = pos.y;
	pos.y = -pos.z;
	pos.z = fYTemp;

	return pos;
}

//========================================
// ConvToBirdsEyeView関数 - 向きを鳥瞰図に変換する(Z⇔Y) -
// Author:RIKU NISHIMURA
//========================================
D3DXVECTOR3 ConvRotToBirdsEyeView(D3DXVECTOR3 rot)
{
	float fYTemp = rot.y;
	rot.y = -rot.z;
	rot.y *= -1;
	rot.y -= D3DX_PI;
	rot.z = fYTemp;
	rot.z += D3DX_PI;
	rot.z *= -1;

	return rot;
}

//========================================
// ControlRot関数 - 向きを制御する -
// Author:RIKU NISHIMURA
//========================================
void RotControl(D3DXVECTOR3 *pRot)
{
	FloatLoopControl(&pRot->x, D3DX_PI, -D3DX_PI);
	FloatLoopControl(&pRot->y, D3DX_PI, -D3DX_PI);
	FloatLoopControl(&pRot->z, D3DX_PI, -D3DX_PI);
}

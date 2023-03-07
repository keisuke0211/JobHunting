//========================================
// 
// ポリゴン関連の処理
// Author:西村 吏功
// 
//========================================
// *** polygon.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "polygon.h"

//****************************************
// グローバル宣言
//****************************************

// 向きの種類毎の情報
AngleType g_aAngleType[ANGLE_TYPE_MAX] =
{// 読み込み名
	"自由",
	"固定",
};

// アニメーションの種類毎の情報
AnimType g_aAnimType[ANIM_TYPE_MAX] =
{// 読み込み名
	"通常",
	"折り返す",
};

//========== *** 向きの種類毎の情報を取得 ***
AngleType *GetAngleType(void) 
{
	return g_aAngleType;
}

//========== *** アニメーションの種類毎の情報を取得 ***
AnimType *GetAnimType(void) 
{
	return g_aAnimType;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StringLoadAngleType関数 - 向きの種類を文字列から読み込む -
//========================================
void StringLoadAngleType(char aString[TXT_MAX], ANGLE_TYPE *pType)
{
	AngleType	*pAngleType	// 向きの種類毎の情報のポインタ
				= g_aAngleType;

	for (int nCntAngleType = 0; nCntAngleType < ANGLE_TYPE_MAX; nCntAngleType++, pAngleType++)
	{
		if (strcmp(aString, pAngleType->aLoadName) == 0)
		{// 読み込み名が一致した時、
			// 向きの種類を代入
			*pType = (ANGLE_TYPE)nCntAngleType;
			break;
		}
	}
}

//========================================
// StringLoadAnimType関数 - アニメーションの種類を文字列から読み込む -
//========================================
void StringLoadAnimType(char aString[TXT_MAX], ANIM_TYPE *pType)
{
	AnimType	*pAnimType	// アニメーションの種類毎の情報のポインタ
				= g_aAnimType;

	for (int nCntAnimType = 0; nCntAnimType < ANIM_TYPE_MAX; nCntAnimType++, pAnimType++)
	{
		if (strcmp(aString, pAnimType->aLoadName) == 0)
		{// 読み込み名が一致した時、
			// アニメーションの種類を代入
			*pType = (ANIM_TYPE)nCntAnimType;
			break;
		}
	}
}

//========================================
// SetVertexPosition関数 - 頂点座標の設定処理 -
//========================================
void SetVertexPosition(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType)
{
	if (bMatchCamera)
	{// 位置をカメラに合わせるフラグが真の時、
		// 位置をカメラに合わせる
		MatchCameraPosition(&pos);
	}

	if ((angleType == ANGLE_TYPE_FIXED)
		||
		(rot.z == D3DX_PI))
	{// 角度の種類が固定、もしくは向きが真上の時、
		// 頂点座標を設定
		pVtx[0].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[0].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[1].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[1].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[2].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[2].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[3].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[3].pos.y = pos.y + (fHeight * 0.5f);
	}
	else if (rot.z == 0.0f)
	{// 向きが真下の時、
		// 頂点座標を設定
		pVtx[0].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[0].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[1].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[1].pos.y = pos.y + (fHeight * 0.5f);
		pVtx[2].pos.x = pos.x + (fWidth * 0.5f);
		pVtx[2].pos.y = pos.y - (fHeight * 0.5f);
		pVtx[3].pos.x = pos.x - (fWidth * 0.5f);
		pVtx[3].pos.y = pos.y - (fHeight * 0.5f);
	}
	else 
	{
		float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
		float fAngle = atan2f(fWidth, fHeight);									// 対角線の角度

		// 頂点座標を設定
		pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
		pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
		pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
		pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
	}
}

//========================================
// SetRHW関数 - rhwの設定処理 -
//========================================
void SetRHW(VERTEX_2D *pVtx) 
{
	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//========================================
// SetColor関数 - 頂点色の設定処理 -
//========================================
void SetVertexColor(VERTEX_2D *pVtx, Color col)
{
	// 頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
	pVtx[1].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
	pVtx[2].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
	pVtx[3].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
}

//========================================
// SetTexture関数 - テクスチャ座標の設定処理 -
//========================================
void SetTexturePosition(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 pos;	// 位置

	if (pPos == NULL) 
	{
		pos = D3DXVECTOR3(0, 0, 0);
	}
	else 
	{
		pos = -*pPos;
	}

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
}

//========================================
// AnimationCount関数 - アニメーションのカウント処理 -
//========================================
int AnimationCount(int *pPtn, int nPtnMin, int nPtnMax, int *pCounterAnim, int nAnimTime, ANIM_TYPE animType) 
{
	if (nAnimTime != -1) 
	{// アニメーションにかかる時間が-1でない時、
		// アニメーションのカウンターを加算
		*pCounterAnim += 1;

		if (*pCounterAnim >= nAnimTime)
		{// アニメーションのカウンターがアニメーションにかかる時間に達した時、
			// アニメーションのカウンターを初期化する
			*pCounterAnim = 0;

			// パターンNo.を加算
			*pPtn += 1;
		}
	}
	else 
	{// アニメーションにかかる時間が-1の時、
		// パターンNo.をそのまま返す
		return *pPtn;
	}

	// アニメーションの種類に応じたパターンNo.を返す
	switch (animType)
	{
		//========== *** 通常 ***
	case ANIM_TYPE_NORMAL:

		// パターンNo.を上限で剰余算した結果を返す
		return nPtnMin + (*pPtn %= (nPtnMax - nPtnMin + 1));

		//========== *** 折り返す ***
	case ANIM_TYPE_TURN:
		
		if ((*pPtn / (nPtnMax - nPtnMin)) % 2 == 0)
		{// パターンNo.を上限で除算した結果が偶数だった時、
			// パターンNo.を上限で剰余算した結果を返す
			return nPtnMin + (*pPtn %= (nPtnMax - nPtnMin));
		}
		else 
		{//パターンを上限で除算した結果が奇数だった時、
			//上限からパターンを上限で剰余算した結果を減算した結果を返す
			return nPtnMax - (*pPtn % (nPtnMax - nPtnMin));
		}
	}
}
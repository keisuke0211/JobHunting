//========================================
// 
// ポリゴン(2D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon2D.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "light.h"
#include "polygon2D.h"
#include "sound.h"
#include <stdio.h>

//****************************************
// プロトタイプ宣言
//****************************************
// モーションのアニメーション読み込み処理
// モーションのアニメーション処理
void LoadMotionAnim(int nMotionNum, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet);
void MotionAnim(int nMotionNum, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMotionAnim関数 - モーションのアニメーション読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMotionAnim(int nMotionNum, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	// サウンドのアニメーションの読み込み
	for (int nCntSoundAnim = 0; nCntSoundAnim < motionSet.aMotion[nMotionNum].nSoundAnimNum; nCntSoundAnim++)
	{
		if (pPartsInfo->nCounterMotion == motionSet.aMotion[nMotionNum].aSoundAnim[nCntSoundAnim].nTime)
		{// モーションカウンターが切り替え時間に達した時、
			// サウンドを再生
			PlaySound(motionSet.aMotion[nMotionNum].aSoundAnim[nCntSoundAnim].nSound);
		}
	}

	// 部品毎のアニメーションの読み込み
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		PartsMotion *pPartsMotion	// 部品毎のモーション情報のポインタ
					= &motionSet.aMotion[nMotionNum].aPartsMotion[nCntParts];
		Parts2D		*pParts			// 部品の情報のポインタ
					= &pPartsInfo->aParts[nCntParts];

		// パターンNo.のアニメーションの読み込み
		for (int nCntPtnAnim = 0; nCntPtnAnim < pPartsMotion->nPtnAnimNum; nCntPtnAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aPtnAnim[nCntPtnAnim].nTime)
			{// モーションカウンターが切り替え時間に達した時、
				pParts->nPattern = pPartsMotion->aPtnAnim[nCntPtnAnim].nPattern;	// パターンNo.を代入
			}
		}

		// 位置のアニメーションの読み込み
		for (int nCntPosAnim = 0; nCntPosAnim < pPartsMotion->nPosAnimNum; nCntPosAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aPosAnim[nCntPosAnim].nTime)
			{// モーションカウンターが切り替え時間に達した時、
				if (pPartsMotion->aPosAnim[nCntPosAnim].nTime == -1)
				{// 切り替え時間が-1の時、
					// 相対位置を目標位置に設定
					pParts->relativePos = pPartsMotion->aPosAnim[nCntPosAnim].pos;
				}
				else
				{// 切り替え時間が-1でない時、
					if (pPartsMotion->aPosAnim[nCntPosAnim].nMoveTime == 0) 
					{// 位置アニメーションにかかる時間が0の時、
						pParts->relativePos = pPartsMotion->aPosAnim[nCntPosAnim].pos;	// 位置を目標位置に設定
					}
					else
					{// 位置アニメーションにかかる時間が0でない時、
						pParts->posOld			= pParts->relativePos;								// 元の位置を設定
						pParts->targetPos		= pPartsMotion->aPosAnim[nCntPosAnim].pos;			// 目標位置を代入
						pParts->nPosAnimTime	= pPartsMotion->aPosAnim[nCntPosAnim].nMoveTime;	// 位置アニメーションにかかる時間を代入
						pParts->nCounterPosAnim = 0;												// 位置アニメーションカウンターを初期化
						pParts->bPosAnim		= true;												// 位置アニメーションフラグを真にする
					}
				}
			}
		}

		// 角度のアニメーションの読み込み
		for (int nCntRotAnim = 0; nCntRotAnim < pPartsMotion->nRotAnimNum; nCntRotAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aRotAnim[nCntRotAnim].nTime)
			{// モーションカウンターが切り替え時間に達した時、
				if (pPartsMotion->aPosAnim[nCntRotAnim].nTime == -1)
				{// 切り替え時間が-1の時、
					// 相対角度を目標角度に設定
					pParts->relativeRot = pPartsMotion->aRotAnim[nCntRotAnim].rot;
				}
				else
				{// 切り替え時間が-1でない時、
					pParts->rotOld			= pParts->relativeRot;								// 元の角度を設定
					pParts->targetRot		= pPartsMotion->aRotAnim[nCntRotAnim].rot;			// 目標角度を代入
					pParts->nRotAnimTime	= pPartsMotion->aRotAnim[nCntRotAnim].nMoveTime;	// 角度アニメーションにかかる時間を代入
					pParts->nCounterRotAnim = 0;												// 角度アニメーションカウンターを初期化
					pParts->bRotAnim		= true;												// 角度アニメーションフラグを真にする
				}
				
			}
		}

		// 色のアニメーションの読み込み
		for (int nCntColorAnim = 0; nCntColorAnim < pPartsMotion->nColorAnimNum; nCntColorAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aColorAnim[nCntColorAnim].nTime)
			{// モーションカウンターが切り替え時間に達した時、
				if (pPartsMotion->aPosAnim[nCntColorAnim].nTime == -1)
				{// 切り替え時間が-1の時、
					// 色を目標の色に設定
					pParts->col = pPartsMotion->aColorAnim[nCntColorAnim].col;
				}
				else
				{// 切り替え時間が-1でない時、
					pParts->colOld				= pParts->col;											// 元の色を設定
					pParts->targetCol			= pPartsMotion->aColorAnim[nCntColorAnim].col;			// 目標の色を代入
					pParts->nColorAnimTime		= pPartsMotion->aColorAnim[nCntColorAnim].nMoveTime;	// 色アニメーションにかかる時間を代入
					pParts->nCounterColorAnim	= 0;													// 色アニメーションカウンターを初期化
					pParts->bColorAnim			= true;													// 色アニメーションフラグを真にする
				}
			}
		}

		// 光のアニメーションの読み込み
		for (int nCntLightAnim = 0; nCntLightAnim < pPartsMotion->nLightAnimNum; nCntLightAnim++)
		{
			if (pPartsInfo->nCounterMotion == pPartsMotion->aLightAnim[nCntLightAnim].nTime)
			{// モーションカウンターが切り替え時間に達した時、
				if (pPartsMotion->aPosAnim[nCntLightAnim].nTime == -1)
				{// 切り替え時間が-1の時、
					pParts->lightColor		= pPartsMotion->aLightAnim[nCntLightAnim].col;		// 光の色を目標の光の色に設定
					pParts->fLightLength	= pPartsMotion->aLightAnim[nCntLightAnim].fLength;	// 光の長さを目標の光の長さに設定
				}
				else
				{// 切り替え時間が-1でない時、
					pParts->lightColorOld		= pParts->lightColor;									// 元の光の色を設定
					pParts->targetLightColor	= pPartsMotion->aLightAnim[nCntLightAnim].col;			// 目標の光の色を代入
					pParts->fLightLengthOld		= pParts->fLightLength;									// 元の光の長さを設定
					pParts->fTargetLightLength	= pPartsMotion->aLightAnim[nCntLightAnim].fLength;		// 目標の光の長さを代入
					pParts->nLightAnimTime		= pPartsMotion->aLightAnim[nCntLightAnim].nMoveTime;	// 光アニメーションにかかる時間を代入
					pParts->nCounterLightAnim	= 0;													// 光アニメーションカウンターを初期化
					pParts->bLightAnim			= true;													// 光アニメーションフラグを真にする
				}
			}
		}
	}
}

//========================================
// MotionAnim関数 - モーションのアニメーション処理 -
// Author:RIKU NISHIMURA
//========================================
void MotionAnim(int nMotionNum, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		Parts2D	*pParts	// 部品の情報のポインタ
				= &pPartsInfo->aParts[nCntParts];

		if (pParts->bPosAnim)
		{// 位置アニメーションフラグが真の時、
			// 位置を更新
			pParts->relativePos =
				(pParts->posOld * (1.0f - ((float)pParts->nCounterPosAnim / (float)pParts->nPosAnimTime)))
				+ (pParts->targetPos * ((float)pParts->nCounterPosAnim / (float)pParts->nPosAnimTime));

			if (++pParts->nCounterPosAnim > pParts->nPosAnimTime)
			{// 位置アニメーションカウンターを加算した結果位置アニメーションにかかる時間に達した時、
				// 位置アニメーションフラグを偽にする
				pParts->bPosAnim = false;

				// 相対位置を目標位置に設定
				pParts->relativePos = pParts->targetPos;
			}
		}

		if (pParts->bRotAnim)
		{// 角度アニメーションフラグが真の時、
			// 角度を更新
			pParts->relativeRot =
				(pParts->rotOld * (1.0f - ((float)pParts->nCounterRotAnim / (float)pParts->nRotAnimTime)))
				+ (pParts->targetRot * ((float)pParts->nCounterRotAnim / (float)pParts->nRotAnimTime));

			if (++pParts->nCounterRotAnim > pParts->nRotAnimTime)
			{// 角度アニメーションカウンターを加算した結果角度アニメーションにかかる時間に達した時、
				// 角度アニメーションフラグを偽にする
				pParts->bRotAnim = false;

				// 相対角度を目標角度に設定
				pParts->relativeRot = pParts->targetRot;
			}
		}

		if (pParts->bColorAnim)
		{// 色アニメーションフラグが真の時、
			float fDiameter = (float)pParts->nCounterColorAnim / (float)pParts->nColorAnimTime;	// カウンターの進行率

			// 色を更新
			pParts->col.r = (pParts->colOld.r * (1.0f - fDiameter)) + (pParts->targetCol.r * fDiameter);
			pParts->col.g = (pParts->colOld.g * (1.0f - fDiameter)) + (pParts->targetCol.g * fDiameter);
			pParts->col.b = (pParts->colOld.b * (1.0f - fDiameter)) + (pParts->targetCol.b * fDiameter);
			pParts->col.a = (pParts->colOld.a * (1.0f - fDiameter)) + (pParts->targetCol.a * fDiameter);

			if (++pParts->nCounterColorAnim > pParts->nColorAnimTime)
			{// 色アニメーションカウンターを加算した結果色アニメーションにかかる時間に達した時、
				pParts->bColorAnim	= false;				// 色アニメーションフラグを偽にする
				pParts->col			= pParts->targetCol;	// 色を目標色に設定
			}
		}

		if (pParts->bLightAnim)
		{// 光アニメーションフラグが真の時、
			float fDiameter = (float)pParts->nCounterLightAnim / (float)pParts->nLightAnimTime;	// カウンターの進行率

			// 光の色を更新
			pParts->lightColor.r = (pParts->lightColorOld.r * (1.0f - fDiameter)) + (pParts->targetLightColor.r * fDiameter);
			pParts->lightColor.g = (pParts->lightColorOld.g * (1.0f - fDiameter)) + (pParts->targetLightColor.g * fDiameter);
			pParts->lightColor.b = (pParts->lightColorOld.b * (1.0f - fDiameter)) + (pParts->targetLightColor.b * fDiameter);
			pParts->lightColor.a = (pParts->lightColorOld.a * (1.0f - fDiameter)) + (pParts->targetLightColor.a * fDiameter);

			// 光の長さを更新
			pParts->fLightLength = (pParts->fLightLengthOld * (1.0f - fDiameter)) + (pParts->fTargetLightLength * fDiameter);

			if (++pParts->nCounterLightAnim > pParts->nLightAnimTime)
			{// 光アニメーションカウンターを加算した結果光アニメーションにかかる時間に達した時、
				pParts->bLightAnim		= false;						// 光アニメーションフラグを偽にする
				pParts->lightColor		= pParts->targetLightColor;		// 光の色を目標の光の色に設定
				pParts->fLightLength	= pParts->fTargetLightLength;	// 光の長さを目標の光の長さに設定
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// GetPolygon2DNum関数 - ポリゴン(2D)の必要数を返す -
// Author:RIKU NISHIMURA
//========================================
int GetPolygon2DNum(const Polygon2D *pPoly2D, int nPolyMax)
{
	int nNum = 0;	// ポリゴン数

	// ポリゴン毎の最大数を加算
	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++)
	{
		nNum += pPoly2D->nMax;
	}

	return nNum;
}

//========================================
// GetPolygon2DIndex関数 - 現在のカウントのポリゴン(2D)の番号を返す -
// Author:RIKU NISHIMURA
//========================================
int GetPolygon2DIndex(const Polygon2D *pPoly2D, int nPolyMax, int nCount)
{
	int nLine = 0;	// ライン

	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++)
	{
		nLine += pPoly2D->nMax;	// ラインに最大数を加算

		if (nCount < nLine) 
		{// カウントがライン未満の時、カウントの値を返す
			return nCntPoly;
		}
	}

	return 0;
}

//========================================
// LoadPolygon2DTexture関数 - ポリゴン(2D)のテクスチャの読み込み -
// Author:RIKU NISHIMURA
//========================================
void LoadPolygon2DTexture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 *pTextureUi_map_00, const Polygon2D *pPoly2D, int nPolyMax) 
{
	for (int nCntPoly = 0; nCntPoly < nPolyMax; nCntPoly++, pPoly2D++, pTextureUi_map_00++)
	{
		D3DXCreateTextureFromFile(pDevice, pPoly2D->aTexturePath, pTextureUi_map_00);
	}
}

//========================================
// LoadParts2DSet関数 - 部品設定情報(2D)の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadParts2DSet(FILE *pFile, Parts2DSet *pPartsSet)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];
	float	fAllScale;	// 全体の拡大倍率

	// 部品数を初期化
	pPartsSet->nPartsNum = 0;

	// 部品設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "PARTSSET_END"))		{ break; }								// 読み込みを終了
		else if (!strcmp(aDataSearch, "ALL_SCALE:"))		{ fscanf(pFile, "%f", &fAllScale); }	// 全体の拡大倍率
		else if (!strcmp(aDataSearch, "PARTS"))				{
			float		fScale;		// 拡大倍率
			Parts2DType	*pPartsType // 部品の種類毎の情報のポインタ
						= &pPartsSet->aPartsType[pPartsSet->nPartsNum];
			
			pPartsType->initColor = { 255,255,255,255 };	// 初期カラーを初期化
			pPartsType->nInitPtn = 0;						// 初期パターンNo.を初期化

			// 部品情報の読み込みを開始
			while (1) 
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (!strcmp(aDataSearch, "PARTS_END")) {
					pPartsSet->nPartsNum++;									// 部品数を加算
					pPartsType->fWidth				*= fScale * fAllScale;	// 幅				に拡大倍率を適用
					pPartsType->fHeight				*= fScale * fAllScale;	// 高さ				に拡大倍率を適用
					pPartsType->fixedRelativePos	*= fScale * fAllScale;	// 固定の相対位置	に拡大倍率を適用
					pPartsType->rotRelativePos		*= fScale * fAllScale;	// 回転軸の相対位置	に拡大倍率を適用
					break;													// 読み込みを終了
				}
				else if (!strcmp(aDataSearch, "TEXTURE_PATH:"))	{ fscanf(pFile, "%s", &pPartsType->aTexturePath); }	// テクスチャの相対パス
				else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pPartsType->fWidth); }		// 幅
				else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pPartsType->fHeight); }		// 高さ
				else if (!strcmp(aDataSearch, "SCALE:"))		{ fscanf(pFile, "%f", &fScale); }					// 大きさにかかる倍率
				else if (!strcmp(aDataSearch, "PTN_MAX_X:"))	{ fscanf(pFile, "%d", &pPartsType->nPtnMaxX); }		// パターン上限X
				else if (!strcmp(aDataSearch, "PTN_MAX_Y:"))	{ fscanf(pFile, "%d", &pPartsType->nPtnMaxY); }		// パターン上限Y
				else if (!strcmp(aDataSearch, "PARENT:"))		{ fscanf(pFile, "%d", &pPartsType->nParent); }		// 親パーツ
				else if (!strcmp(aDataSearch, "RELATIVE_POS:"))	{			// 固定の相対位置
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.x);	// X
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.y);	// Y
					fscanf(pFile, "%f", &pPartsType->fixedRelativePos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "RELATIVE_ROT:")) {			// 固定の相対角度
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.x);	// X
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.y);	// Y
					fscanf(pFile, "%f", &pPartsType->fixedRelativeRot.z);	// Z
					pPartsType->fixedRelativeRot.x *= D3DX_PI;				// 円周率補正
					pPartsType->fixedRelativeRot.y *= D3DX_PI;				// 円周率補正
					pPartsType->fixedRelativeRot.z *= D3DX_PI;				// 円周率補正
				}
				else if (!strcmp(aDataSearch, "ROT_RELATIVE_POS:")) {	// 回転軸の相対位置
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.x);	// X
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.y);	// Y
					fscanf(pFile, "%f", &pPartsType->rotRelativePos.z);	// Z
				}
				else if (!strcmp(aDataSearch, "ANGLE_TYPE:")) { fscanf(pFile, "%d", &pPartsType->angleType); }		// 向きの種類
				else if (!strcmp(aDataSearch, "ILLUMINATE:")) { fscanf(pFile, "%d", &pPartsType->bIlluminate); }	// 光の影響を受けるフラグ
				else if (!strcmp(aDataSearch, "INIT_COLOR:")) {												// 初期カラー
					fscanf(pFile, "%d", &pPartsType->initColor.r);											// 赤度
					fscanf(pFile, "%d", &pPartsType->initColor.g);											// 緑度
					fscanf(pFile, "%d", &pPartsType->initColor.b);											// 青度
					fscanf(pFile, "%d", &pPartsType->initColor.a);											// 不透明度
				}
				else if (!strcmp(aDataSearch, "INIT_PATTERN:")) { fscanf(pFile, "%d", &pPartsType->nInitPtn); }	// 初期のパターンNo.
			}
		}
	}
}

//========================================
// InitParts2D関数 - 部品管理(2D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParts2DInfo(Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		pPartsInfo->aParts[nCntParts].relativePos	= D3DXVECTOR3(0, 0, 0);
		pPartsInfo->aParts[nCntParts].relativeRot	= D3DXVECTOR3(0, 0, 0);
		pPartsInfo->aParts[nCntParts].bPosAnim		= false;
		pPartsInfo->aParts[nCntParts].bRotAnim		= false;
		pPartsInfo->aParts[nCntParts].bColorAnim	= false;
		pPartsInfo->aParts[nCntParts].bLightAnim	= false;
		pPartsInfo->aParts[nCntParts].col			= partsSet.aPartsType[nCntParts].initColor;
		pPartsInfo->aParts[nCntParts].fLightLength	= 0.0f;
		pPartsInfo->aParts[nCntParts].nPattern		= partsSet.aPartsType[nCntParts].nInitPtn;
	}
}

//========================================
// SetParts2DVertexPos関数 - 部品(2D)の頂点座標の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetParts2DVertexPos(VERTEX_2D *pVtxTemp, VERTEX_2D *pVtx, int nPartsNum, Parts2D *pParts, Parts2DInfo *pPartsInfo, Parts2DType *pPartsType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, bool bFlipHorizontal)
{
	D3DXVECTOR3 posTemp	// 位置を保存 
				= pParts->pos;
	D3DXVECTOR3 rotTemp	// 角度を保存 
				= pParts->rot;

	if (pPartsType[nPartsNum].nParent != -1)
	{// 親番号が-1でない時、
		// 位置と角度を親の部品のものに設定
		pParts->pos = pPartsInfo->aParts[pPartsType[nPartsNum].nParent].pos;
		pParts->rot = pPartsInfo->aParts[pPartsType[nPartsNum].nParent].rot;
	}
	else
	{// 親番号が-1の時、
		// 位置と角度を呼び出し元のものに設定
		pParts->pos = pos;
		pParts->rot = rot;
	}

	{
		Parts2DType	*pThisType			// 現在の種類情報
					= &pPartsType[nPartsNum];
		D3DXVECTOR3	fixedRelativePos	// 固定の相対位置
					= pThisType->fixedRelativePos;
		D3DXVECTOR3	relativePos			// 相対位置
					= pParts->relativePos;
		D3DXVECTOR3	rotRelativePos		// 回転軸の相対位置
					= pThisType->rotRelativePos;
		D3DXVECTOR3 fixedRelativeRot	// 固定の相対角度
					= pThisType->fixedRelativeRot;
		D3DXVECTOR3 relativeRot			// 相対角度
					= pParts->relativeRot;
		D3DXVECTOR3 rot					// 角度
					= pParts->rot;

		if (bFlipHorizontal) 
		{// 左右反転フラグが真の時、
			// 左右反転
			fixedRelativePos.x	*= -1;
			relativePos.x		*= -1;
			rotRelativePos.x	*= -1;
			fixedRelativeRot.z	*= -1;
			relativeRot.z		*= -1;
		}

		rot += fixedRelativeRot;																					// 角度に部品の固定の相対角度を加算
		pParts->pos.x += sinf(rot.z) * -(fixedRelativePos.y + relativePos.y + rotRelativePos.y);					// 位置Xに部品の相対位置と位置Yと回転軸の相対位置Xを補正
		pParts->pos.y += cosf(rot.z) * -(fixedRelativePos.y + relativePos.y + rotRelativePos.y);					// 位置Yに部品の相対位置と位置Yと回転軸の相対位置Yを補正
		pParts->pos.x += sinf(rot.z - (D3DX_PI * 0.5f)) * (fixedRelativePos.x + relativePos.x + rotRelativePos.x);	// 位置Xに部品の相対位置と位置Xと回転軸の相対位置Xを補正
		pParts->pos.y += cosf(rot.z - (D3DX_PI * 0.5f)) * (fixedRelativePos.x + relativePos.x + rotRelativePos.x);	// 位置Yに部品の相対位置と位置Xと回転軸の相対位置Yを補正
		rot += relativeRot;																							// 角度に部品の相対角度を加算
		pParts->pos.x += sinf(rot.z) * (rotRelativePos.y);															// 位置Xに回転軸の相対位置Yを補正
		pParts->pos.y += cosf(rot.z) * (rotRelativePos.y);															// 位置Yに回転軸の相対位置Yを補正
		pParts->pos.x += sinf(rot.z - (D3DX_PI * 0.5f)) * -(rotRelativePos.x);										// 位置Xに回転軸の相対位置Xを補正
		pParts->pos.y += cosf(rot.z - (D3DX_PI * 0.5f)) * -(rotRelativePos.x);										// 位置Yに回転軸の相対位置Xを補正

		// 角度を代入
		pParts->rot = rot;
	}

	// 頂点座標の設定
	SetVertexPos2D(pVtx,
		pParts->pos,
		pParts->rot,
		bMatchCamera,
		pPartsType[nPartsNum].fWidth * pParts->scale.x,
		pPartsType[nPartsNum].fHeight * pParts->scale.y,
		pPartsType[nPartsNum].angleType);

	if ((pParts->pos != posTemp)
		||
		(pParts->rot != rotTemp))
	{// 位置、もしくは角度が変動している時、
		VERTEX_2D	*pVtxChild	// 子の頂点情報のポインター
					= pVtxTemp;

		// 子部品の頂点座標を設定しなおす
		for (int nCntParts = 0; nCntParts < nPartsNum; nCntParts++, pVtxChild += 4)
		{
			if (pPartsType[nCntParts].nParent == nPartsNum)
			{// カウントされた部品の親番号と現在の部品の番号が一致している時、
				// 子部品の頂点座標の設定
				SetParts2DVertexPos(pVtxTemp, pVtxChild,
					nCntParts,
					&pPartsInfo->aParts[nCntParts],
					pPartsInfo,
					pPartsType,
					pos,
					rot,
					true,
					bFlipHorizontal);
			}
		}
	}
}

//========================================
// SetParts2DVertexColor関数 - 部品(2D)の頂点色の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetParts2DVertexColor(VERTEX_2D *pVtx, Parts2D parts,bool bIlluminate)
{
	if (bIlluminate)
	{// 光の影響を受けるフラグが真の時、
		// 頂点カラーをSYS:光[00] との距離に応じて設定
		SetVertexColor2D(pVtx, GetColorForLight2D(parts.pos, parts.col));
	}
	else
	{// 光の影響を受けるフラグが偽の時、
		// 頂点カラーの設定
		SetVertexColor2D(pVtx, parts.col);
	}
}

//========================================
// LoadMotionSet関数 - モーション設定情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMotionSet(FILE *pFile, MotionSet *pMotionSet)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];
	float	fAllScale;	// 全体の拡大倍率

	// モーション数を初期化
	pMotionSet->nMotionNum = 0;

	// モーション設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "MOTIONSET_END"))	{ break; }								// 読み込みを終了
		else if (!strcmp(aDataSearch, "ALL_SCALE:"))	{ fscanf(pFile, "%f", &fAllScale); }	// 全体の拡大倍率
		else if (!strcmp(aDataSearch, "MOTION"))		{
			Motion	*pMotion	// モーションの情報のポインタ
					= &pMotionSet->aMotion[pMotionSet->nMotionNum];
			int		nCntParts	// 部品のカウント
					= 0;

			// サウンドのアニメーション数を初期化
			pMotion->nSoundAnimNum = 0;

			// モーション情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "MOTION_END"))		{ pMotionSet->nMotionNum++;	break; }				// モーション数を加算し、読み込みを終了
				else if (!strcmp(aDataSearch, "LOOP_TIME:"))		{ fscanf(pFile, "%d", &pMotion->nLoopTime); }		// ループ時間
				else if (!strcmp(aDataSearch, "LOOP:"))				{ fscanf(pFile, "%d", &pMotion->bLoop); }			// ループするかフラグ
				else if (!strcmp(aDataSearch, "FLIP_HORIZONTAL:"))	{ fscanf(pFile, "%d", &pMotion->bFlipHorizontal); }	// 左右反転フラグ
				else if (!strcmp(aDataSearch, "PARTS"))				{
					PartsMotion *pPartsMotion	// 部品毎のモーションの情報のポインタ
								= &pMotion->aPartsMotion[nCntParts];

					// 部品毎のモーションの情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if		(!strcmp(aDataSearch, "PARTS_END"))		{ nCntParts++;	break; }	// 部品のカウントを加算し、読み込みを終了
						else if (!strcmp(aDataSearch, "PATTERN_ANIM"))	{
							// パターンNo.のアニメーションの情報の読み込みを開始
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索

								if		(!strcmp(aDataSearch, "PATTERN_ANIM_END"))	{ break; }							// 読み込みを終了
								else if (!strcmp(aDataSearch, "TIME"))				{									// パターンNo.のアニメーション情報
									fscanf(pFile, "%d", &pPartsMotion->aPtnAnim[pPartsMotion->nPtnAnimNum].nTime);		// 切り替え時間
									fscanf(pFile, "%d", &pPartsMotion->aPtnAnim[pPartsMotion->nPtnAnimNum].nPattern);	// パターンNo.
									pPartsMotion->nPtnAnimNum++;														// パターンNo.のアニメーション数を加算
								}
							}
						}
						else if (!strcmp(aDataSearch, "POSITION_ANIM")) {
							// 位置のアニメーションの情報の読み込みを開始
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索

								if (!strcmp(aDataSearch, "POSITION_ANIM_END"))	{ break; }								// 読み込みを終了
								else if (!strcmp(aDataSearch, "TIME"))			{										// 位置のアニメーション情報
									fscanf(pFile, "%d", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].nTime);		// 切り替え時間
									fscanf(pFile, "%s", aDataSearch);													// [
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.z);		// Z
									pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos *= fAllScale;					// 位置に拡大倍率を適用
									fscanf(pFile, "%s", aDataSearch);													// ]
									fscanf(pFile, "%d", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].nMoveTime);	// 到達にかかる時間
									pPartsMotion->nPosAnimNum++;														// 位置のアニメーション数を加算
								}
							}
						}
						else if (!strcmp(aDataSearch, "ROTATION_ANIM")) {
							// 角度のアニメーションの情報の読み込みを開始
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索

								if		(!strcmp(aDataSearch, "ROTATION_ANIM_END")) { break; }							// 読み込みを終了
								else if (!strcmp(aDataSearch, "TIME"))				{									// 角度のアニメーション情報
									fscanf(pFile, "%d", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].nTime);		// 切り替え時間
									fscanf(pFile, "%s", aDataSearch);													// [
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z);		// Z
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x *= D3DX_PI;					// 円周率補正
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y *= D3DX_PI;					// 円周率補正
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z *= D3DX_PI;					// 円周率補正
									fscanf(pFile, "%s", aDataSearch);													// ]
									fscanf(pFile, "%d", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].nMoveTime);	// 到達にかかる時間
									pPartsMotion->nRotAnimNum++;														// 角度のアニメーション数を加算
								}
							}
						}
						else if (!strcmp(aDataSearch, "COLOR_ANIM")) {
							// 色のアニメーションの情報の読み込みを開始
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索

								if (!strcmp(aDataSearch, "COLOR_ANIM_END")) { break; }										// 読み込みを終了
								else if (!strcmp(aDataSearch, "TIME"))		{												// 色のアニメーション情報
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].nTime);		// 切り替え時間
									fscanf(pFile, "%s", aDataSearch);														// [
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].col.r);		// 赤度
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].col.g);		// 緑度
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].col.b);		// 青度
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].col.a);		// 不透明度
									fscanf(pFile, "%s", aDataSearch);														// ]
									fscanf(pFile, "%d", &pPartsMotion->aColorAnim[pPartsMotion->nColorAnimNum].nMoveTime);	// 到達にかかる時間
									pPartsMotion->nColorAnimNum++;															// 色のアニメーション数を加算
								}
							}
						}
						else if (!strcmp(aDataSearch, "LIGHT_ANIM")) {
							// 光のアニメーションの情報の読み込みを開始
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索

								if (!strcmp(aDataSearch, "LIGHT_ANIM_END")) { break; }										// 読み込みを終了
								else if (!strcmp(aDataSearch, "TIME")) {													// 光のアニメーション情報
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].nTime);		// 切り替え時間
									fscanf(pFile, "%s", aDataSearch);														// [
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].col.r);		// 赤度
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].col.g);		// 緑度
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].col.b);		// 青度
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].col.a);		// 不透明度
									fscanf(pFile, "%s", aDataSearch);														// ]
									fscanf(pFile, "%f", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].fLength);	// 長さ
									fscanf(pFile, "%d", &pPartsMotion->aLightAnim[pPartsMotion->nLightAnimNum].nMoveTime);	// 到達にかかる時間
									pPartsMotion->nLightAnimNum++;															// 光のアニメーション数を加算
								}
							}
						}
					}
				}
				else if (!strcmp(aDataSearch, "SOUND_ANIM")) {
					// サウンドのアニメーションの情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if		(!strcmp(aDataSearch, "SOUND_ANIM_END")) { break; }						// 読み込みを終了
						else if (!strcmp(aDataSearch, "TIME")) {										// サウンドのアニメーション情報
							fscanf(pFile, "%d", &pMotion->aSoundAnim[pMotion->nSoundAnimNum].nTime);	// 切り替え時間
							fscanf(pFile, "%d", &pMotion->aSoundAnim[pMotion->nSoundAnimNum].nSound);	// サウンド番号
							pMotion->nSoundAnimNum++;													// サウンドのアニメーション数を加算
						}
					}
				}
			}
		}
	}
}

//========================================
// UpdateMotion関数 - モーションの更新処理 -
// Author:RIKU NISHIMURA
//========================================
bool UpdateMotion(int nMotion, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	if (motionSet.aMotion[nMotion].nLoopTime == -1) 
	{// モーションのループ時間が-1の時、
		// 処理を終了する
		return false;
	}

	// モーションのアニメーション読み込み処理
	LoadMotionAnim(nMotion, motionSet, pPartsInfo, partsSet);

	// モーションのアニメーション処理
	MotionAnim(nMotion, motionSet, pPartsInfo, partsSet);

	bool	bMotionLoop	// モーションがループしたかフラグ
			= false;

	if (++pPartsInfo->nCounterMotion > motionSet.aMotion[nMotion].nLoopTime) 
	{// モーションカウンターを加算した結果ループ時間に達した時、
		bMotionLoop = true;	// モーションがループしたかフラグを真にする

		if (motionSet.aMotion[nMotion].bLoop)
		{// ループするかフラグが真の時、
			// モーションカウンターをループ制御
			IntLoopControl(&pPartsInfo->nCounterMotion, motionSet.aMotion[nMotion].nLoopTime, 0);
		}
		else 
		{// ループするかフラグが偽の時、
			// モーションカウンターを制御
			IntControl(&pPartsInfo->nCounterMotion, motionSet.aMotion[nMotion].nLoopTime, 0);
		}
	}

	// モーションがループしたかフラグを返す
	return bMotionLoop;
}

//========================================
// SetMotion関数 - モーションの設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetMotion(int nMotion, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet)
{
	if (nMotion == pPartsInfo->nMotion)
	{// 設定するモーション番号が現在のモーション番号と一致している時、
		// 処理を抜ける
		return;
	}

	pPartsInfo->nMotion			= nMotion;	// モーション番号を代入
	pPartsInfo->nCounterMotion	= -1;		// モーションカウンターを-1にする
}

//========================================
// SetVertexPos2D関数 - 頂点座標の設定処理(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType)
{
	if (bMatchCamera)
	{// 位置をカメラに合わせるフラグが真の時、
		// 位置をカメラに合わせる
		MatchCamera2DPosition(&pos);
	}

	if ((angleType == ANGLE_TYPE_FIXED)
		||
		(rot.z == D3DX_PI))
	{// 角度の種類が固定、もしくは角度が真上の時、
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
	{// 角度が真下の時、
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
// SetRHW2D関数 - rhwの設定処理(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetRHW2D(VERTEX_2D *pVtx) 
{
	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
}

//========================================
// SetVertexColor2D関数 - 頂点色の設定処理(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor2D(VERTEX_2D *pVtx, Color col)
{
	// 頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// SetTexturePos2D関数 - テクスチャ座標の設定処理(2D) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
{
	if (nPtnMaxX * nPtnMaxY == 0) 
	{// パターンの上限が0の時、
		// 処理を終了する
		return;
	}

	D3DXVECTOR3 pos;	// 位置

	if (pPos == NULL) 
	{// 位置のポインタがNULLの時、
		// 位置を初期化
		pos = D3DXVECTOR3(0, 0, 0);
	}
	else 
	{// 位置のポインタがNULLで無い時、
		// 位置を-補正して代入
		pos = -*pPos;
	}

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)							+ pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	+ pos.x, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY)	+ pos.y);
}

//========================================
// SetTexturePos2DCut関数 - テクスチャ座標の設定処理(2D)(切り取りver) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos)
{
	pos.x += 0.5f;
	pos.y += 0.5f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(pos.x - (cutScale.fWidth * 0.5f), pos.y - (cutScale.fHeight * 0.5f));
	pVtx[1].tex = D3DXVECTOR2(pos.x + (cutScale.fWidth * 0.5f), pos.y - (cutScale.fHeight * 0.5f));
	pVtx[2].tex = D3DXVECTOR2(pos.x - (cutScale.fWidth * 0.5f), pos.y + (cutScale.fHeight * 0.5f));
	pVtx[3].tex = D3DXVECTOR2(pos.x + (cutScale.fWidth * 0.5f), pos.y + (cutScale.fHeight * 0.5f));
}

//========================================
// MatchPosition関数 - 描画位置を基準に合わせる -
// Author:RIKU NISHIMURA
//========================================
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos)
{
	// 位置を基準の位置に合わせる
	pDrawPos->x = pDrawPos->x - basePos.x + (SCREEN_WIDTH * 0.5f);
	pDrawPos->y = pDrawPos->y - basePos.y + (SCREEN_HEIGHT * 0.5f);
}
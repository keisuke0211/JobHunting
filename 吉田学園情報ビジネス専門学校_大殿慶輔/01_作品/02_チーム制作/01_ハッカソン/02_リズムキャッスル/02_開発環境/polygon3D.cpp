//========================================
// 
// ポリゴン(3D)関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** polygon3D.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "light.h"
#include "polygon3D.h"
#include "sound.h"
#include <stdio.h>

//****************************************
// プロトタイプ宣言
//****************************************
// モーション(3D)のアニメーション読み込み処理
// モーション(3D)のアニメーション処理
void LoadMotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);
void MotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMotionAnim3D関数 - モーション(3D)のアニメーション読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
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
		PartsMotion3D	*pPartsMotion	// 部品毎のモーション情報のポインタ
						= &motionSet.aMotion[nMotionNum].aPartsMotion[nCntParts];
		Parts3D			*pParts			// 部品の情報のポインタ
						= &pPartsInfo->aParts[nCntParts];

		// 位置のアニメーションの読み込み
		for (int nCntPosAnim = 0; nCntPosAnim < pPartsMotion->nPosAnimNum; nCntPosAnim++)
		{
			PositionAnim3D	*pPosAnim	// 位置アニメーションの情報のポインタ
							= &pPartsMotion->aPosAnim[nCntPosAnim];

			if (pPartsInfo->nCounterMotion != pPosAnim->nTime)
			{// モーションカウンターが切り替え時間に達していない時、
				continue;	// 繰り返し処理を折り返す
			}

			if ((pPosAnim->nTime == -1)
				||
				(pPosAnim->nMoveTime == 0))
			{// 切り替え時間が-1 or 位置アニメーションにかかる時間が0の時、
				pParts->pos			=					// 位置と
				pParts->targetPos	= pPosAnim->pos;	// 目標位置にアニメーション位置を代入
			}
			else
			{// 切り替え時間が-1でない時、
				pParts->posOld			= pParts->pos;			// 元の位置を設定
				pParts->targetPos		= pPosAnim->pos;		// 目標位置を代入
				pParts->nPosAnimTime	= pPosAnim->nMoveTime;	// 位置アニメーションにかかる時間を代入
				pParts->nCounterPosAnim = 0;					// 位置アニメーションカウンターを初期化
				pParts->bPosAnim		= true;					// 位置アニメーションフラグを真にする
			}
		}

		// 向きのアニメーションの読み込み
		for (int nCntRotAnim = 0; nCntRotAnim < pPartsMotion->nRotAnimNum; nCntRotAnim++)
		{
			RotationAnim3D	*pRotAnim	// 向きアニメーションの情報のポインタ
							= &pPartsMotion->aRotAnim[nCntRotAnim];

			if (pPartsInfo->nCounterMotion != pRotAnim->nTime)
			{// モーションカウンターが切り替え時間に達していない時、
				continue;	// 繰り返し処理を折り返す
			}

			if ((pRotAnim->nTime == -1)
				||
				(pRotAnim->nMoveTime == 0))
			{// 切り替え時間が-1 or 向きアニメーションにかかる時間が0の時、
				pParts->rot			=					// 向きと
				pParts->targetRot	= pRotAnim->rot;	// 目標向きにアニメーション向きを代入
			}
			else
			{// 切り替え時間が-1でない時、
				pParts->rotOld			= pParts->rot;			// 元の向きを設定
				pParts->targetRot		= pRotAnim->rot;		// 目標向きを代入
				pParts->nRotAnimTime	= pRotAnim->nMoveTime;	// 向きアニメーションにかかる時間を代入
				pParts->nCounterRotAnim = 0;					// 向きアニメーションカウンターを初期化
				pParts->bRotAnim		= true;					// 向きアニメーションフラグを真にする
			}
		}
	}
}

//========================================
// MotionAnim3D関数 - モーション(3D)のアニメーション処理 -
// Author:RIKU NISHIMURA
//========================================
void MotionAnim3D(int nMotionNum, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
{
	for (int nCntParts = 0; nCntParts < partsSet.nPartsNum; nCntParts++)
	{
		Parts3D	*pParts	// 部品の情報のポインタ
				= &pPartsInfo->aParts[nCntParts];

		if (pParts->bPosAnim)
		{// 位置アニメーションフラグが真の時、
			if (++pParts->nCounterPosAnim >= pParts->nPosAnimTime)
			{// 位置アニメーションカウンターを加算した結果位置アニメーションにかかる時間を上回った時、
				pParts->bPosAnim = false;			// 位置アニメーションフラグを偽にする
				pParts->pos = pParts->targetPos;	// 位置に目標位置を代入
			}
		}

		if (pParts->bRotAnim)
		{// 角度アニメーションフラグが真の時、
			if (++pParts->nCounterRotAnim >= pParts->nRotAnimTime)
			{// 向きアニメーションカウンターを加算した結果向きアニメーションにかかる時間を上回った時、
				pParts->bRotAnim = false;			// 向きアニメーションフラグを偽にする
				pParts->rot = pParts->targetRot;	// 向きに目標向きを代入
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
// LoadParts3DSet関数 - 部品設定情報(3D)の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet)
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

		if		(!strcmp(aDataSearch, "PARTSSET_END"))	{ break; }								// 読み込みを終了
		else if (!strcmp(aDataSearch, "ALL_SCALE:"))	{ fscanf(pFile, "%f", &fAllScale); }	// 全体の拡大倍率
		else if (!strcmp(aDataSearch, "PARTS"))			{
			Parts3DType	*pPartsType // 部品の種類毎の情報のポインタ
						= &pPartsSet->aPartsType[pPartsSet->nPartsNum];

			// 部品情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "PARTS_END"))		{ pPartsSet->nPartsNum++; break; }					// 読み込みを終了
				else if (!strcmp(aDataSearch, "MODEL_PATH:"))	{ fscanf(pFile, "%s", &pPartsType->aModelPath); }	// モデルの相対パス
				else if (!strcmp(aDataSearch, "PARENT:"))		{ fscanf(pFile, "%d", &pPartsType->nParent); }		// 親パーツ
				else if (!strcmp(aDataSearch, "RELATIVE_POS:")) {			// 固定の相対位置
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
			}
		}
	}
}

//========================================
// InitParts3DInfo関数 - 部品(3D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParts3DInfo(Parts3DInfo *pPartsInfo)
{
	pPartsInfo->nCounterMotion	= 0;	// モーションカウンター
	pPartsInfo->nMotion			= 0;	// モーション番号

	for (int nCntParts = 0; nCntParts < PARTS_3D_MAX; nCntParts++) 
	{
		pPartsInfo->aParts[nCntParts].posResult			= INITD3DXVECTOR3;	// 位置(結果)
		pPartsInfo->aParts[nCntParts].pos				= INITD3DXVECTOR3;	// 位置
		pPartsInfo->aParts[nCntParts].rotResult			= INITD3DXVECTOR3;	// 向き(結果)
		pPartsInfo->aParts[nCntParts].rot				= INITD3DXVECTOR3;	// 向き
		pPartsInfo->aParts[nCntParts].bPosAnim			= false;			// 位置アニメーションフラグ
		pPartsInfo->aParts[nCntParts].nCounterPosAnim	= 0;				// 位置アニメーションカウンター
		pPartsInfo->aParts[nCntParts].nPosAnimTime		= 0;				// 位置アニメーションにかかる時間
		pPartsInfo->aParts[nCntParts].posOld			= INITD3DXVECTOR3;	// 元の位置
		pPartsInfo->aParts[nCntParts].targetPos			= INITD3DXVECTOR3;	// 目標位置
		pPartsInfo->aParts[nCntParts].bRotAnim			= false;			// 角度アニメーションフラグ
		pPartsInfo->aParts[nCntParts].nCounterRotAnim	= 0;				// 角度アニメーションカウンター
		pPartsInfo->aParts[nCntParts].nRotAnimTime		= 0;				// 角度アニメーションにかかる時間
		pPartsInfo->aParts[nCntParts].rotOld			= INITD3DXVECTOR3;	// 元の角度
		pPartsInfo->aParts[nCntParts].targetRot			= INITD3DXVECTOR3;	// 目標角度
	}
}

//========================================
// InitParts3DTransform関数 - 部品(3D)のトランスフォームの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet)
{
	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		= &pPartsInfo->aParts[nCntParts];		// 部品の情報のポインタ
		Parts3DType *pPartsType = &pPartsSet->aPartsType[nCntParts];	// 部品の種類毎の情報のポインタ

		// 位置を更新
		pParts->posResult = pPartsType->fixedRelativePos;
		pParts->rotResult = pPartsType->fixedRelativeRot;
	}
}

//========================================
// GetParts3DTransform関数 - 部品(3D)のトランスフォームの取得処理 -
// Author:RIKU NISHIMURA
//========================================
void GetParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet)
{
	for (int nCntParts = 0; nCntParts < pPartsSet->nPartsNum; nCntParts++)
	{
		Parts3D		*pParts		= &pPartsInfo->aParts[nCntParts];		// 部品の情報のポインタ
		Parts3DType *pPartsType = &pPartsSet->aPartsType[nCntParts];	// 部品の種類毎の情報のポインタ

		if (pParts->bPosAnim)
		{// 位置アニメーションフラグが真の時、
			float	fRate	// カウンターの進行率
					= (float)pParts->nCounterPosAnim / (float)pParts->nPosAnimTime;

			// 位置を更新
			pParts->pos = (pParts->posOld * (1.0f - fRate)) + (pParts->targetPos * fRate);
		}

		// 位置(結果)を更新
		pParts->posResult = pParts->pos + pPartsType->fixedRelativePos;

		if (pParts->bRotAnim)
		{// 向きアニメーションフラグが真の時、
			float	fRate	// カウンターの進行率
					= (float)pParts->nCounterRotAnim / (float)pParts->nRotAnimTime;

			// 向きを更新
			pParts->rot = (pParts->rotOld * (1.0f - fRate)) + (pParts->targetRot * fRate);
		}

		// 向き(結果)を更新
		pParts->rotResult = pParts->rot + pPartsType->fixedRelativeRot;
	}
}

//========================================
// LoadMotion3DSet関数 - モーション(3D)設定情報の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMotion3DSet(FILE *pFile, Motion3DSet *pMotionSet)
{
	char	aDataSearch	// データ検索用
			[TXT_MAX];

	// モーション数を初期化
	pMotionSet->nMotionNum = 0;

	// モーション設定情報の読み込みを開始
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if		(!strcmp(aDataSearch, "MOTIONSET_END")) { break; }	// 読み込みを終了
		else if (!strcmp(aDataSearch, "MOTION"))		{
			Motion3D	*pMotion	// モーションの情報のポインタ
						= &pMotionSet->aMotion[pMotionSet->nMotionNum];
			int			nCntParts	// 部品のカウント
						= 0;

			// サウンドのアニメーション数を初期化
			pMotion->nSoundAnimNum = 0;

			// モーション情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if		(!strcmp(aDataSearch, "MOTION_END"))	{ pMotionSet->nMotionNum++;	break; }				// モーション数を加算し、読み込みを終了
				else if (!strcmp(aDataSearch, "LOOP_TIME:"))	{ fscanf(pFile, "%d", &pMotion->nLoopTime); }		// ループ時間
				else if (!strcmp(aDataSearch, "LOOP:"))			{ fscanf(pFile, "%d", &pMotion->bLoop); }			// ループするかフラグ
				else if (!strcmp(aDataSearch, "PARTS"))			{
					PartsMotion3D	*pPartsMotion	// 部品毎のモーションの情報のポインタ
									= &pMotion->aPartsMotion[nCntParts];

					// 部品毎のモーションの情報の読み込みを開始
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if		(!strcmp(aDataSearch, "PARTS_END"))		{ nCntParts++;	break; }	// 部品のカウントを加算し、読み込みを終了
						else if (!strcmp(aDataSearch, "POSITION_ANIM")) {
							// 位置のアニメーションの情報の読み込みを開始
							while (1)
							{
								fscanf(pFile, "%s", aDataSearch); // 検索

								if (!strcmp(aDataSearch, "POSITION_ANIM_END")) { break; }								// 読み込みを終了
								else if (!strcmp(aDataSearch, "TIME")) {												// 位置のアニメーション情報
									fscanf(pFile, "%d", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].nTime);		// 切り替え時間
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aPosAnim[pPartsMotion->nPosAnimNum].pos.z);		// Z
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
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x);		// X
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y);		// Y
									fscanf(pFile, "%f", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z);		// Z
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.x *= D3DX_PI;					// 円周率補正
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.y *= D3DX_PI;					// 円周率補正
									pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].rot.z *= D3DX_PI;					// 円周率補正
									fscanf(pFile, "%d", &pPartsMotion->aRotAnim[pPartsMotion->nRotAnimNum].nMoveTime);	// 到達にかかる時間
									pPartsMotion->nRotAnimNum++;														// 角度のアニメーション数を加算
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

						if		(!strcmp(aDataSearch, "SOUND_ANIM_END"))	{ break; }					// 読み込みを終了
						else if (!strcmp(aDataSearch, "TIME"))				{							// サウンドのアニメーション情報
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
// UpdateMotion3D関数 - モーション(3D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
bool UpdateMotion3D(int nMotion, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet)
{
	if (motionSet.aMotion[nMotion].nLoopTime == -1)
	{// モーションのループ時間が-1の時、
		// 処理を終了する
		return false;
	}

	// モーションのアニメーション(3D)読み込み処理
	LoadMotionAnim3D(nMotion, motionSet, pPartsInfo, partsSet);

	// モーションのアニメーション処理
	MotionAnim3D(nMotion, motionSet, pPartsInfo, partsSet);

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
// SetMotion3D関数 - モーション(3D)の設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetMotion3D(Parts3DInfo *pPartsInfo, int nMotion) 
{
	if (pPartsInfo->nMotion == nMotion) 
	{// 既にそのモーション番号に設定されている時、
		return;	// 処理を終了する
	}

	pPartsInfo->nMotion = nMotion;	// モーション番号を代入
	pPartsInfo->nCounterMotion = 0;	// モーションカウンターを初期化
}

//========================================
// SetVertexPos3D関数 - 頂点座標の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight)
{
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[1].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f,  (fHeight * 0.5f));
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
	pVtx[3].pos = D3DXVECTOR3( (fWidth * 0.5f), 0.0f, -(fHeight * 0.5f));
}

//========================================
// SetNormalLine3D関数 - 法線ベクトルの設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor) 
{
	// 法線ベクトルの設定
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;
}

//========================================
// SetVertexColor3D関数 - 頂点色の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetVertexColor3D(VERTEX_3D *pVtx, Color col)
{
	// 頂点色の設定
	pVtx[0].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DCOLOR_RGBA(col.r, col.g, col.b, col.a);
}

//========================================
// SetTexturePos3D関数 - テクスチャ座標の設定処理(3D) -
// Author:RIKU NISHIMURA
//========================================
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos)
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
	pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY) + pos.y);
	pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + pos.x						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
	pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX) + pos.x	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY) + pos.y);
}

//========================================
// SetMaterial関数 - マテリアルの設定 -
// Author:RIKU NISHIMURA
//========================================
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col)
{
	D3DXCOLOR	mtlColTemp	// マテリアルの色を保存
				= pMat->Diffuse;

	// マテリアルの色を設定
	pMat->Diffuse.r = mtlColTemp.r * ((float)col.r / (float)255);
	pMat->Diffuse.g = mtlColTemp.g * ((float)col.g / (float)255);
	pMat->Diffuse.b = mtlColTemp.b * ((float)col.b / (float)255);
	pMat->Diffuse.a = mtlColTemp.a * ((float)col.a / (float)255);

	// マテリアルの設定
	pDevice->SetMaterial(pMat);

	// マテリアルの色を元に戻す
	pMat->Diffuse = mtlColTemp;
}
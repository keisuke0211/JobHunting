//========================================
// 
// 弾[00]の処理
// Author:西村 吏功
// 
//========================================
// *** atk_bullet_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "physics.h"
#include "polygon.h"
#include "sound.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "attack.h"
#include "chr_enemy_00.h"
#include "eff_light_00.h"
#include "eff_particle_00.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
#define ATK_BULLET_00_ROTATION_TiME			(20)										// 弾[00] の回転にかかる時間
#define ATK_BULLET_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\ATK_BULLET_00_DATA.txt"	// 敵[00] の種類毎の情報のデータファイルの相対パス

//****************************************
// プロトタイプ宣言 
//****************************************

// パラメーターの初期化処理
void InitParameter(Atk_bullet_00 *pAtk);

// パラメーターを種類に応じて設定
void SetParameter(Atk_bullet_00 *pAtk);

// 当たり判定処理
void Collision(Atk_bullet_00 *pAtk);

// 機体[00] との当たり判定処理
void CollisionChr_fighter_00(Atk_bullet_00 *pAtk);

// 敵[00] との当たり判定処理
void CollisionChr_enemy_00(Atk_bullet_00 *pAtk);

// パーティクルの設定処理
void SetParticle(Atk_bullet_00 *pAtk);

// ライトの設定処理
void SetLight(Atk_bullet_00 *pAtk);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureAtk_bullet_00							// テクスチャへのポインタ
						[ATK_BULLET_00_TYPE_MAX][ATK_PARENT_TYPE_MAX]	//
						= {};											//
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffAtk_bullet_00							// 頂点バッファへのポインタ
						= NULL;											//
Atk_bullet_00Type		g_aAtk_bullet_00Type							// 弾[00] の種類毎の情報
						[ATK_BULLET_00_TYPE_MAX];						//
Atk_bullet_00			g_aAtk_bullet_00								// 弾[00] の情報
						[ATK_BULLET_00_MAX];							//

//========== *** 弾[00] の情報を取得 ***
Atk_bullet_00 *GetAtk_bullet_00(void)
{
	return g_aAtk_bullet_00;
}

//========== *** 弾[00] の種類毎の情報を取得 ***
Atk_bullet_00Type *GetAtk_bullet_00Type(void)
{
	return g_aAtk_bullet_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameter関数 - パラメーターの初期化処理 -
//========================================
void InitParameter(Atk_bullet_00 *pAtk)
{
	pAtk->pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pAtk->move				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	pAtk->fMove				= 0.0f;								// 移動力
	pAtk->rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pAtk->drawRot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// +描画向き
	pAtk->fWidth			= 0.0f;								// 幅
	pAtk->fHeight			= 0.0f;								// 高さ
	pAtk->fHitTestWidth		= 0.0f;								// 当たり判定の幅
	pAtk->fHitTestHeight	= 0.0f;								// 当たり判定の高さ
	pAtk->nType				= 0;								// 種類
	pAtk->parentType		= (ATK_PARENT_TYPE)0;				// 親の種類
	pAtk->bUse				= false;							// 使用されているかフラグ
	pAtk->nLife				= 0;								// 寿命
	pAtk->nDamage			= 0;								// ダメージ
	pAtk->nPenetrationNum	= 0;								// 残りの貫通回数
	pAtk->nPattern			= 0;								// パターンNo.
	pAtk->nCounterAnim		= 0;								// アニメーションのカウンター
	pAtk->nCounterRotation	= 0;								// 回転カウンター
}

//========================================
// SetState関数 - パラメーターを種類に応じて設定 -
//========================================
void SetParameter(Atk_bullet_00 *pAtk)
{
	pAtk->fWidth			= g_aAtk_bullet_00Type[pAtk->nType].fWidth;			// 幅
	pAtk->fHeight			= g_aAtk_bullet_00Type[pAtk->nType].fHeight;		// 高さ
	pAtk->fHitTestWidth		= g_aAtk_bullet_00Type[pAtk->nType].fHitTestWidth;	// 当たり判定の幅
	pAtk->fHitTestHeight	= g_aAtk_bullet_00Type[pAtk->nType].fHitTestHeight;	// 当たり判定の高さ
	pAtk->fMove				= g_aAtk_bullet_00Type[pAtk->nType].fMove;			// 移動力
	pAtk->nDamage			= g_aAtk_bullet_00Type[pAtk->nType].nDamage;		// ダメージ
	pAtk->nPenetrationNum	= g_aAtk_bullet_00Type[pAtk->nType].nPenetration;	// 残りの貫通回数
	pAtk->nLife				= g_aAtk_bullet_00Type[pAtk->nType].nLife;			// 寿命を設定
}

//========================================
// Collision関数 - 当たり判定処理 -
//========================================
void Collision(Atk_bullet_00 *pAtk) 
{
	// 親の種類に応じた当たり判定処理
	switch (pAtk->parentType)
	{
		//========== *** プレイヤー ***
	case ATK_PARENT_TYPE_PLAYER:

		// 敵[00]との当たり判定処理
		CollisionChr_enemy_00(pAtk);

		if (g_aAtk_bullet_00Type[pAtk->nType].bHitAlly)
		{// 味方にも当たる時、
			// 機体[00]との当たり判定処理
			CollisionChr_fighter_00(pAtk);
		}

		break;
		//========== *** 敵 ***
	case ATK_PARENT_TYPE_ENEMY:

		// 機体[00]との当たり判定処理
		CollisionChr_fighter_00(pAtk);

		if (g_aAtk_bullet_00Type[pAtk->nType].bHitAlly) 
		{// 味方にも当たる時、
			// 敵[00]との当たり判定処理
			CollisionChr_enemy_00(pAtk);
		}

		break;
	}
}

//========================================
// CollisionChr_fighter_00関数 - 機体[00]との当たり判定処理 -
//========================================
void CollisionChr_fighter_00(Atk_bullet_00 *pAtk)
{
	//if ((!GetChr_fighter_00()->bUse)
	//	|| (GetChr_fighter_00()->aState[CHR_FIGHTER_00_STATE_INVINCIBLE]))
	//{// 機体[00] が使用されていない状態か無敵状態の時、
	//	// 処理を終了する
	//	return;
	//}

	//Chr_fighter_00	*pChr	// 機体[00] の情報のポインタ
	//				= GetChr_fighter_00();

	//if (CheckHit(
	//	pAtk->pos,
	//	pAtk->rot,
	//	pAtk->fHitTestWidth,
	//	pAtk->fHeight,
	//	g_aAtk_bullet_00Type[pAtk->nType].hitTestType,
	//	pChr->pos,
	//	pChr->rot,
	//	pChr->fHitTestWidth,
	//	pChr->fHitTestHeight,
	//	GetChr_fighter_00Type()[pChr->nType].hitTestType))
	//{// 弾[00] が機体[00] の衝突した時、
	//	// 機体[00] のヒット処理
	//	HitChr_fighter_00(pAtk->nDamage);

	//	// 弾[00] の破壊処理
	//	DestructionAtk_bullet_00(pAtk);

	//	// パーティクルの設定処理
	//	SetParticle(pAtk);
	//}
}

//========================================
// CollisionChr_enemy_00関数 - 敵[00]との当たり判定処理 -
//========================================
void CollisionChr_enemy_00(Atk_bullet_00 *pAtk)
{
	Chr_enemy_00	*pChr	// 敵[00] の情報のポインタ
					= GetChr_enemy_00();

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if ((!pChr->bUse)
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_POP])
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
			|| (pChr->aState[CHR_ENEMY_00_STATE_SWELL])
			|| (pChr->aAct[CHR_ENEMY_00_ACT_GHOST])
			|| (pChr->aAct[CHR_ENEMY_00_ACT_BULLET_GHOST]))
		{// 使用されていない状態、もしくは出現中か消え中か膨らみかすり抜けの時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (CheckHit(
			pAtk->pos,
			pAtk->rot,
			pAtk->fHitTestWidth,
			pAtk->fHeight,
			HIT_TEST_TYPE_CIRCLE,
			pChr->pos,
			pChr->rot,
			pChr->fHitTestWidth,
			pChr->fHitTestHeight,
			HIT_TEST_TYPE_CIRCLE))
		{// 弾[00] が敵[00] の当たり判定内に入った時、
			// 敵[00] のヒット処理
			HitChr_enemy_00(pChr, pAtk->nDamage);

			// 弾[00] の破壊処理
			DestructionAtk_bullet_00(pAtk);

			// パーティクルの設定処理
			SetParticle(pAtk);

			// 使用していない状態にする
			pAtk->bUse = false;

			break;
		}
	}
}

//========================================
// SetParticle関数 - パーティクルの設定処理 -
//========================================
void SetParticle(Atk_bullet_00 *pAtk)
{
	// パーティクルの設定処理
	for (int nCntPtcl = 0; nCntPtcl < g_aAtk_bullet_00Type[pAtk->nType].nPtclNum; nCntPtcl++)
	{
		D3DXVECTOR3 setRot;	// 設定する向き

		// 設定する向きを設定
		setRot.z = pAtk->rot.z - (D3DX_PI * 0.25f) + fRand(D3DX_PI * 0.5f) + D3DX_PI;

		// パーティクル[00] を設定
		SetEff_particle_00(pAtk->pos, setRot, g_aAtk_bullet_00Type[pAtk->nType].aPtclType[pAtk->parentType]);
	}
}

//========================================
// SetLight関数 - ライトの設定処理 -
//========================================
void SetLight(Atk_bullet_00 *pAtk)
{
	// ライト[00] を設定
	SetEff_light_00(
		pAtk->pos, 
		pAtk->rot + pAtk->drawRot,
		g_aAtk_bullet_00Type[pAtk->nType].fLightMove,
		g_aAtk_bullet_00Type[pAtk->nType].aLightColor[pAtk->parentType],
		g_aAtk_bullet_00Type[pAtk->nType].fLightWidth,
		g_aAtk_bullet_00Type[pAtk->nType].fLightHeight,
		g_aAtk_bullet_00Type[pAtk->nType].nLightLife,
		EFF_LIGHT_00_TYPE_00);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadAtk_bullet_00関数 - 弾[00] の読み込み処理 -
//========================================
void LoadAtk_bullet_00(void)
{
	FILE				*pFile;					// ファイルポインタ
	char				aDataSearch				//
						[TXT_MAX];				// データ検索用
	Atk_bullet_00Type	*pAtkType				//
						= g_aAtk_bullet_00Type;	// 弾[00] の種類毎の情報

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(ATK_BULLET_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// 文末識別子が見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch); // 検索

		if (strcmp(aDataSearch, "END") == 0)
		{// ファイルを閉じる
			fclose(pFile);
			break;
		}

		if (aDataSearch[0] == '#')
		{// コメント印と一致した時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (strcmp(aDataSearch, "ATK_BULLET_00_TYPE") == 0)
		{// 弾[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (strcmp(aDataSearch, "ATK_BULLET_00_TYPE_END") == 0)
				{// 弾[00] の種類毎の情報の読み込みを終了
					pAtkType++;
					break;
				}//====================//
				else if (strcmp(aDataSearch, "読み込み名:") == 0)
				{// 読み込み名を読み込む
					fscanf(pFile, "%s", pAtkType->aLoadName);
				}//====================//
				else if (strcmp(aDataSearch, "テクスチャの相対パス(プレイヤー):") == 0)
				{// テクスチャの相対パス(プレイヤー)を読み込む
					fscanf(pFile, "%s", pAtkType->aTexturPath[ATK_PARENT_TYPE_PLAYER]);
				}//====================//
				else if (strcmp(aDataSearch, "テクスチャの相対パス(敵):") == 0)
				{// テクスチャの相対パス(敵)を読み込む
					fscanf(pFile, "%s", pAtkType->aTexturPath[ATK_PARENT_TYPE_ENEMY]);
				}//====================//
				else if (strcmp(aDataSearch, "パターンの上限X:") == 0)
				{// パターン上限Xを読み込む
					fscanf(pFile, "%d", &pAtkType->nPtnMaxX);
				}//====================//
				else if (strcmp(aDataSearch, "パターンの上限Y:") == 0)
				{// パターン上限Yを読み込む
					fscanf(pFile, "%d", &pAtkType->nPtnMaxY);
				}//====================//
				else if (strcmp(aDataSearch, "アニメパターンの下限:") == 0)
				{// アニメパターンの下限を読み込む
					fscanf(pFile, "%d", &pAtkType->nAnimPtnMin);
				}//====================//
				else if (strcmp(aDataSearch, "アニメパターンの上限:") == 0)
				{// アニメパターンの上限を読み込む
					fscanf(pFile, "%d", &pAtkType->nAnimPtnMax);
				}//====================//
				else if (strcmp(aDataSearch, "アニメーションの時間:") == 0)
				{// アニメーションにかかる時間を読み込む
					fscanf(pFile, "%d", &pAtkType->nAnimTime);
				}//====================//
				else if (strcmp(aDataSearch, "アニメーションの種類:") == 0)
				{// アニメーションの種類を読み込む
					fscanf(pFile, "%s", aDataSearch); // 検索

					// アニメーションの種類を文字列から読み込む
					StringLoadAnimType(aDataSearch, &pAtkType->animType);
				}//====================//
				else if (strcmp(aDataSearch, "幅:") == 0)
				{// 幅を読み込む
					fscanf(pFile, "%f", &pAtkType->fWidth);

					// 幅をピクセル単位で修正
					pAtkType->fWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "高さ:") == 0)
				{// 高さを読み込む
					fscanf(pFile, "%f", &pAtkType->fHeight);

					// 高さをピクセル単位で修正
					pAtkType->fHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "向きの種類:") == 0)
				{// 向きの種類を読み込む
					fscanf(pFile, "%d", &pAtkType->angleType);
				}//====================//
				else if (strcmp(aDataSearch, "当たり判定の幅:") == 0)
				{// 当たり判定の幅を読み込む
					fscanf(pFile, "%f", &pAtkType->fHitTestWidth);

					// 当たり判定をピクセル単位で修正
					pAtkType->fHitTestWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "当たり判定の高さ:") == 0)
				{// 当たり判定の幅を読み込む
					fscanf(pFile, "%f", &pAtkType->fHitTestHeight);

					// 当たり判定をピクセル単位で修正
					pAtkType->fHitTestHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "味方にも当たる:") == 0)
				{// 味方にも当たるかフラグを読み込む
					int nBTemp;	// 一時保存用

					fscanf(pFile, "%d", &nBTemp);

					pAtkType->bHitAlly = nBTemp;
				}//====================//
				else if (strcmp(aDataSearch, "移動力:") == 0)
				{// 移動力を読み込む
					fscanf(pFile, "%f", &pAtkType->fMove);
				}//====================//
				else if (strcmp(aDataSearch, "回転力:") == 0)
				{// 回転力を読み込む
					fscanf(pFile, "%f", &pAtkType->fRotation);
				}//====================//
				else if (strcmp(aDataSearch, "寿命:") == 0)
				{// 寿命を読み込む
					fscanf(pFile, "%d", &pAtkType->nLife);
				}//====================//
				else if (strcmp(aDataSearch, "貫通回数:") == 0)
				{// 貫通回数を読み込む
					fscanf(pFile, "%d", &pAtkType->nPenetration);
				}//====================//
				else if (strcmp(aDataSearch, "ダメージ:") == 0)
				{// ダメージを読み込む
					fscanf(pFile, "%d", &pAtkType->nDamage);
				}//====================//
				else if (strcmp(aDataSearch, "設定時のサウンド:") == 0)
				{// 設定時のサウンドの読み込みを開始
					fscanf(pFile, "%s", aDataSearch); // 検索

					// サウンドラベルを文字列から読み込む
					StringLoadSound(aDataSearch, &pAtkType->soundSet);
				}//====================//
				else if (strcmp(aDataSearch, "爆発の種類:") == 0)
				{// 爆発の種類を読み込む
					fscanf(pFile, "%d", &pAtkType->exType);
				}//====================//
				else if (strcmp(aDataSearch, "ライトの色(プレイヤー):") == 0)
				{// ライトの色(プレイヤー)を読み込む
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_PLAYER].nR);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_PLAYER].nG);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_PLAYER].nB);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_PLAYER].nA);
				}//====================//
				else if (strcmp(aDataSearch, "ライトの色(敵):") == 0)
				{// ライトの色(敵)を読み込む
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_ENEMY].nR);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_ENEMY].nG);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_ENEMY].nB);
					fscanf(pFile, "%d", &pAtkType->aLightColor[ATK_PARENT_TYPE_ENEMY].nA);
				}//====================//
				else if (strcmp(aDataSearch, "ライトの幅:") == 0)
				{// ライトの幅を読み込む
					fscanf(pFile, "%f", &pAtkType->fLightWidth);

					// ライトの幅をピクセル単位で修正
					pAtkType->fLightWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "ライトの高さ:") == 0)
				{// ライトの高さを読み込む
					fscanf(pFile, "%f", &pAtkType->fLightHeight);

					// ライトの高さをピクセル単位で修正
					pAtkType->fLightHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "ライトの寿命:") == 0)
				{// ライトの寿命を読み込む
					fscanf(pFile, "%d", &pAtkType->nLightLife);
				}//====================//
				else if (strcmp(aDataSearch, "ライトの移動量:") == 0)
				{// ライトの移動量を読み込む
					fscanf(pFile, "%d", &pAtkType->fLightMove);
				}//====================//
				else if (strcmp(aDataSearch, "パーティクルの種類(プレイヤー):") == 0)
				{// パーティクルの種類(プレイヤー)を読み込む
					fscanf(pFile, "%d", &pAtkType->aPtclType[ATK_PARENT_TYPE_PLAYER]);
				}//====================//
				else if (strcmp(aDataSearch, "パーティクルの種類(敵):") == 0)
				{// パーティクルの種類(敵)を読み込む
					fscanf(pFile, "%d", &pAtkType->aPtclType[ATK_PARENT_TYPE_ENEMY]);
				}//====================//
				else if (strcmp(aDataSearch, "パーティクルの発生数:") == 0)
				{// パーティクルの発生数を読み込む
					fscanf(pFile, "%d", &pAtkType->nPtclNum);
				}//====================//
			}
		}
	}
}

//========================================
// StringLoadAtk_bullet_00関数 - 弾[00] の種類を文字列から読み込む -
//========================================
void StringLoadAtk_bullet_00(char aString[TXT_MAX], int *pType)
{
	Atk_bullet_00Type	*pAtkType	// 弾[00] の種類毎の情報
						= GetAtk_bullet_00Type();

	for (int nCntAtkType = 0; nCntAtkType < ATK_BULLET_00_TYPE_MAX; nCntAtkType++, pAtkType++)
	{
		if (strcmp(aString, pAtkType->aLoadName) == 0)
		{// 読み込み名が一致した時、
			// 弾の種類を代入
			*pType = nCntAtkType;
			break;
		}
	}
}

//========================================
// InitAtk_bullet_00関数 - 弾[00] の初期化処理 -
//========================================
void InitAtk_bullet_00(void)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイスへのポインタ
						= GetDevice();
	VERTEX_2D			*pVtx;	// 頂点情報へのポインタ

	// テクスチャの読み込み
	for (int nCntAtk_bullet_00_type = 0; nCntAtk_bullet_00_type < ATK_BULLET_00_TYPE_MAX; nCntAtk_bullet_00_type++)
	{
		for (int nCntLookType = 0; nCntLookType < ATK_PARENT_TYPE_MAX; nCntLookType++)
		{
			D3DXCreateTextureFromFile(pDevice, g_aAtk_bullet_00Type[nCntAtk_bullet_00_type].aTexturPath[nCntLookType], &g_aTextureAtk_bullet_00[nCntAtk_bullet_00_type][nCntLookType]);
		}
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ATK_BULLET_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAtk_bullet_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// 弾[00] の情報のポインタ
					= g_aAtk_bullet_00;

	for (int nCntAtk_bullet_00 = 0; nCntAtk_bullet_00 < ATK_BULLET_00_MAX; nCntAtk_bullet_00++, pVtx += 4, pAtk++)
	{
		// パラメーターを初期化し、種類に応じて設定
		InitParameter(pAtk);
		SetParameter(pAtk);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pAtk->pos,
			pAtk->rot,
			true,
			pAtk->fWidth,
			pAtk->fHeight,
			g_aAtk_bullet_00Type[pAtk->nType].angleType);

		// rhw/頂点カラー/テクスチャ座標の設定
		SetRHW(pVtx);
		SetVertexColor(pVtx, { 255,255,255,255 });
		SetTexturePosition(pVtx,
			pAtk->nPattern,
			g_aAtk_bullet_00Type[pAtk->nType].nPtnMaxX,
			g_aAtk_bullet_00Type[pAtk->nType].nPtnMaxY,
			NULL);
	}
	
	// 頂点座標 をアンロックする
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// UninitAtk_bullet_00関数 - 弾[00] の終了処理 -
//========================================
void UninitAtk_bullet_00(void)
{
	// テクスチャの破棄
	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_TYPE_MAX; nCntAtk++)
	{
		for (int nCntParentType = 0; nCntParentType < ATK_PARENT_TYPE_MAX; nCntParentType++)
		{
			if (g_aTextureAtk_bullet_00[nCntAtk][nCntParentType] != NULL)
			{
				g_aTextureAtk_bullet_00[nCntAtk][nCntParentType]->Release();
				g_aTextureAtk_bullet_00[nCntAtk][nCntParentType] = NULL;
			}
		}
	}
	
	// 頂点バッファの破棄
	if (g_pVtxBuffAtk_bullet_00 != NULL) 
	{
		g_pVtxBuffAtk_bullet_00->Release();
		g_pVtxBuffAtk_bullet_00 = NULL;
	}
}

//========================================
// UpdateAtk_bullet_00関数 - 弾[00] の更新処理 -
//========================================
void UpdateAtk_bullet_00(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// 弾[00] の情報のポインタ
					= g_aAtk_bullet_00;

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pVtx += 4, pAtk++)
	{
		if (!pAtk->bUse)
		{// 弾[00] が使用されていない時、
			// 繰り返し処理を折り返す
			continue;
		}

		// 寿命のカウントを減算
		pAtk->nLife--;

		if (pAtk->nLife <= 0)
		{// 寿命が尽きた時、
			// 弾[00] の破壊処理
			DestructionAtk_bullet_00(pAtk);

			// 繰り返し処理を折り返す
			continue;
		}

		// 推進処理
		Driving(&pAtk->pos, pAtk->rot, &pAtk->move, pAtk->fMove);

		// 当たり判定処理
		Collision(pAtk);

		// 向きを後進
		pAtk->drawRot.z += g_aAtk_bullet_00Type[pAtk->nType].fRotation;
		ControlAngle(&pAtk->drawRot.z);

		// ライトの設定処理
		SetLight(pAtk);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pAtk->pos,
			pAtk->rot + pAtk->drawRot,
			true,
			pAtk->fWidth,
			pAtk->fHeight,
			g_aAtk_bullet_00Type[pAtk->nType].angleType);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&pAtk->nPattern,
				g_aAtk_bullet_00Type[pAtk->nType].nAnimPtnMin,
				g_aAtk_bullet_00Type[pAtk->nType].nAnimPtnMax,
				&pAtk->nCounterAnim,
				g_aAtk_bullet_00Type[pAtk->nType].nAnimTime,
				g_aAtk_bullet_00Type[pAtk->nType].animType),
			g_aAtk_bullet_00Type[pAtk->nType].nPtnMaxX,
			g_aAtk_bullet_00Type[pAtk->nType].nPtnMaxY,
			NULL);
	}

	// 頂点座標 をアンロックする
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// DrawAtk_bullet_00関数 - 弾[00]の描画処理 -
//========================================
void DrawAtk_bullet_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファ をデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAtk_bullet_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Atk_bullet_00	*pAtk	// 弾[00] の情報のポインタ
					= g_aAtk_bullet_00;

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++)
	{
		if (pAtk->bUse)
		{// 弾[00] が使用されている時、
			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureAtk_bullet_00[pAtk->nType][pAtk->parentType]);

			// 弾[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAtk * 4, 2);
		}
	}
}

//========================================
// SetAtk_bullet_00関数 - 弾[00]の設定処理 -
//========================================
void SetAtk_bullet_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_PARENT_TYPE parentType)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファ をロックし、頂点情報へのポインタ を取得
	g_pVtxBuffAtk_bullet_00->Lock(0, 0, (void**)&pVtx, 0);

	Atk_bullet_00	*pAtk	// 弾[00] の情報のポインタ
		= g_aAtk_bullet_00;

	for (int nCntAtk_bullet_00 = 0; nCntAtk_bullet_00 < ATK_BULLET_00_MAX; nCntAtk_bullet_00++, pAtk++)
	{
		if (pAtk->bUse)
		{// 弾[00]が使用されている時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (parentType == ATK_PARENT_TYPE_PLAYER)
		{// 親の種類がプレイヤーの時、
			// ショットSEを再生
			PlaySound(g_aAtk_bullet_00Type[nType].soundSet);
		}

		// パラメーターを初期化
		InitParameter(pAtk);

		pAtk->pos			= pos;			// 位置を代入
		pAtk->rot			= rot;			// 向きを代入
		pAtk->nType			= nType;		// 種類を代入
		pAtk->parentType	= parentType;	// 親の種類を代入

		// パラメーターを種類に応じて設定
		SetParameter(&g_aAtk_bullet_00[nCntAtk_bullet_00]);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pAtk->pos,
			pAtk->rot,
			pAtk->fWidth,
			pAtk->fHeight,
			true,
			g_aAtk_bullet_00Type[pAtk->nType].angleType);

		// 使用している状態にする
		pAtk->bUse = true;

		break;
	}

	// 頂点座標 をアンロックする
	g_pVtxBuffAtk_bullet_00->Unlock();
}

//========================================
// DestructionAtk_bullet_00関数 - 弾[00] の破壊処理 -
//========================================
void DestructionAtk_bullet_00(Atk_bullet_00 *pAtk)
{
	if (g_aAtk_bullet_00Type[pAtk->nType].exType != ATK_EXPLOSION_00_TYPE_NOT)
	{// 爆発の種類が無しでない時、
		// 爆発の設定処理
		SetAtk_explosion_00(pAtk->pos, pAtk->rot, g_aAtk_bullet_00Type[pAtk->nType].exType, 0);
	}

	// 使用していない状態にする
	pAtk->bUse = false;
}

//========================================
// AllDestructionAtk_bullet_00関数 - 弾[00] の全破壊処理 -
//========================================
void AllDestructionAtk_bullet_00(ATK_PARENT_TYPE parentType) 
{
	Atk_bullet_00	*pAtk	// 弾[00] の情報のポインタ
					= g_aAtk_bullet_00;

	for (int nCntAtk = 0; nCntAtk < ATK_BULLET_00_MAX; nCntAtk++, pAtk++)
	{
		if ((pAtk->bUse)
			&& (pAtk->parentType == parentType))
		{// 弾[00] が使用されていて、かつ親の種類が一致している時、
			// 破壊処理
			DestructionAtk_bullet_00(pAtk);
		}
	}
}
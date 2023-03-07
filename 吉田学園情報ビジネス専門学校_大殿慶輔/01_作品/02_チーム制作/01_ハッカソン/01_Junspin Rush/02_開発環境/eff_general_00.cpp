//========================================
// 
// エフェクト全般[00] の処理
// Author:西村 吏功
// 
//========================================
// *** eff_general_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "sound.h"
#include "physics.h"
#include "polygon.h"
#include "eff_general_00.h"
#include <stdio.h>
#include <float.h>

//****************************************
// マクロ定義
//****************************************

// エフェクト全般[00] の種類毎の情報のデータファイルの相対パス
#define EFF_GENERAL_00_TYPE_DATA_FILE_PATH	"data\\GAMEDATA\\EFF_GENERAL_00_TYPE_DATA.txt"

//****************************************
// プロトタイプ宣言
//****************************************

// パラメーターの初期化処理
void InitParameter(Eff_general_00 *pEff);

// パラメーターを種類に応じて設定
void SetParameter(Eff_general_00 *pEff);

// サウンド読み込み処理
void SoundLoading(Eff_general_00 *pEff);

// パターンNo.読み込み処理
void PatternLoading(Eff_general_00 *pEff);

// 召喚読み込み処理
void SummonLoading(Eff_general_00 *pEff);

// 召喚処理
void Summon(Eff_general_00 *pEff, int nCntSummon);

// 親の追跡処理
void ParentTrackingProcess(Eff_general_00 *pEff);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureEff_general_00	// テクスチャへのポインタ
						[EFF_GENERAL_00_TYPE_MAX]	//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_general_00	// 頂点バッファへのポインタ
						= NULL;						//
Eff_general_00			g_aEff_general_00			// エフェクト全般[00] の情報
						[EFF_GENERAL_00_MAX];		//
Eff_general_00Type		g_aEff_general_00Type		// エフェクト全般[00] の種類毎の情報
						[EFF_GENERAL_00_TYPE_MAX];	//

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameter関数 - パラメーターの初期化処理 -
//========================================
void InitParameter(Eff_general_00 *pEff) 
{
	pEff->pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pEff->rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pEff->targetRot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標向き
	pEff->fWidth			= 0.0f;								// 幅
	pEff->fHeight			= 0.0f;								// 高さ
	pEff->nType				= 0;								// 種類
	pEff->bUse				= false;							// 使用されているかフラグ
	pEff->nLife				= 0;								// 寿命
	pEff->nPattern			= 0;								// パターンNo.
	pEff->nCounterAnim		= 0;								// アニメーションのカウンター
	pEff->nCounterSound		= 0;								// サウンドカウンター
	pEff->nCounterPattern	= 0;								// パターンNo.カウンター
	pEff->nCounterSummon	= 0;								// 召喚カウンター
	pEff->bDraw				= true;								// 描画フラグ
}

//========================================
// SetParameter関数 - パラメーターを種類に応じて設定 -
//========================================
void SetParameter(Eff_general_00 *pEff)
{
	pEff->fWidth	= g_aEff_general_00Type[pEff->nType].fWidth;	// 幅
	pEff->fHeight	= g_aEff_general_00Type[pEff->nType].fHeight;	// 高さ
	pEff->nLife		= g_aEff_general_00Type[pEff->nType].nLife;		// 寿命
}

//========================================
// SoundLoading関数 - サウンド読み込み処理 -
//========================================
void SoundLoading(Eff_general_00 *pEff)
{
	for (int nCntSound = 0; nCntSound < g_aEff_general_00Type[pEff->nType].nSoundNum; nCntSound++)
	{
		if (pEff->nCounterSound == g_aEff_general_00Type[pEff->nType].aSound[nCntSound].nTime)
		{// サウンドカウンターがサウンドを鳴らす時間と一致した時、
			// サウンドを再生
			PlaySound(g_aEff_general_00Type[pEff->nType].aSound[nCntSound].soundLabel);
		}
	}

	if ((pEff->nCounterSound >= g_aEff_general_00Type[pEff->nType].nSoundLoopTime)
		&& (g_aEff_general_00Type[pEff->nType].nSoundLoopTime != -1))
	{// サウンドカウンターがサウンドのループ時間に達した時、かつサウンドのループ時間が-1でない時、
		// サウンドカウンターを初期化
		pEff->nCounterSound = -1;
	}

	// サウンドカウンターを加算
	pEff->nCounterSound++;
}

//========================================
// PatternLoading関数 - パターンNo.読み込み処理 -
//========================================
void PatternLoading(Eff_general_00 *pEff)
{
	for (int nCntPattern = 0; nCntPattern < g_aEff_general_00Type[pEff->nType].nPatternNum; nCntPattern++)
	{
		if (pEff->nCounterPattern == g_aEff_general_00Type[pEff->nType].aPattern[nCntPattern].nTime)
		{// パターンNo.カウンターがパターンNo.を変更する時間と一致した時、
			// パターンNo.を更新
			pEff->nPattern = g_aEff_general_00Type[pEff->nType].aPattern[nCntPattern].nPattern;

			if (pEff->nPattern == -1) 
			{// パターンNo.が-1だった時、
				// 描画フラグを偽にする
				pEff->bDraw = false;
			}
			else 
			{// パターンNo.が-1でない時、
				// 描画フラグを真にする
				pEff->bDraw = true;
			}
		}
	}

	if ((pEff->nCounterPattern >= g_aEff_general_00Type[pEff->nType].nPatternLoopTime)
		&& (g_aEff_general_00Type[pEff->nType].nPatternLoopTime != -1))
	{// パターンNo.カウンターがパターンNo.のループ時間に達した時、かつパターンNo.のループ時間が-1でない時、
		// パターンNo.カウンターを初期化
		pEff->nCounterPattern = -1;
	}

	// パターンNo.カウンターを加算
	pEff->nCounterPattern++;
}

//========================================
// SummonLoading関数 - 召喚読み込み処理 -
//========================================
void SummonLoading(Eff_general_00 *pEff)
{
	for (int nCntSummon = 0; nCntSummon < g_aEff_general_00Type[pEff->nType].nSummonNum; nCntSummon++)
	{
		if (pEff->nCounterSummon == g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].nTime)
		{// 召喚カウンターが召喚する時間と一致した時、
			// 召喚処理
			Summon(pEff, nCntSummon);
		}
	}

	if ((pEff->nCounterSummon >= g_aEff_general_00Type[pEff->nType].nSummonLoopTime)
		&& (g_aEff_general_00Type[pEff->nType].nSummonLoopTime != -1))
	{// 召喚カウンターが召喚のループ時間に達した時、かつ召喚のループ時間が-1でない時、
		// 召喚カウンターを初期化
		pEff->nCounterSummon = -1;
	}

	// 召喚カウンターを加算
	pEff->nCounterSummon++;
}

//========================================
// Summon関数 - 召喚処理 -
//========================================
void Summon(Eff_general_00 *pEff, int nCntSummon) 
{
	D3DXVECTOR3 setPos			//
				= pEff->pos;	// 設定位置

	// 設定位置を設定
	setPos.x += sinf(pEff->rot.z - D3DX_PI * 0.5f)
		* g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].pos.x;
	setPos.y += cosf(pEff->rot.z - D3DX_PI * 0.5f)
		* g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].pos.x;
	setPos.x += sinf(pEff->rot.z + D3DX_PI)
		* g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].pos.y;
	setPos.y += cosf(pEff->rot.z + D3DX_PI)
		* g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].pos.y;

	// エフェクト全般[00] の設定処理
	SetEff_general_00(
		setPos,
		D3DXVECTOR3(
			0.0f,
			0.0f,
			pEff->rot.z + g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].fAngle),
		g_aEff_general_00Type[pEff->nType].aSummon[nCntSummon].nEnemyType,
		false, 
		NULL,
		NULL);
}

//========================================
// ParentTrackingProcess関数- 親の追跡処理 -
//========================================
void ParentTrackingProcess(Eff_general_00 *pEff)
{
	if (!pEff->bChild)
	{// 子フラグが偽の時、
		// 処理を終了する
		return;
	}

	// 位置を更新
	pEff->pos += *pEff->pParentPos - pEff->parentPosTemp;

	D3DXVECTOR3 rot;											// 親から子への向き
	float		fDistance										// 
				= FindDistance(*pEff->pParentPos, pEff->pos);	// 親と子の距離

	// 親から子への向きを設定
	rot.z = FindAngle(*pEff->pParentPos, pEff->pos) + (pEff->pParentRot->z - pEff->parentRotTemp.z);

	// 子に向きの差分を適用
	pEff->rot.z += (pEff->pParentRot->z - pEff->parentRotTemp.z);

	// 位置を親の所に戻した後、
	// 正しい角度に距離を加算する
	pEff->pos = *pEff->pParentPos;
	pEff->pos.x += sinf(rot.z) * fDistance;
	pEff->pos.y += cosf(rot.z) * fDistance;

	// 親の位置と角度を保存
	pEff->parentPosTemp = *pEff->pParentPos;
	pEff->parentRotTemp = *pEff->pParentRot;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadEff_general_00関数 - エフェクト全般[00] の読み込み処理 -
//========================================
void LoadEff_general_00(void)
{
	FILE *pFile;				// ファイルポインタ
	char aDataSearch[TXT_MAX];	// データ検索用

	Eff_general_00Type	*pEffType	// エフェクト全般[00] の種類毎の情報
						= g_aEff_general_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(EFF_GENERAL_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// 文末識別子が見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

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

		if (strcmp(aDataSearch, "EFF_GENERAL_00_TYPE") == 0)
		{// エフェクト全般[00] の種類毎の情報の読み込みを開始

			// 召喚数を初期化
			pEffType->nSummonNum = 0;

			// サウンド数を初期化
			pEffType->nSoundNum = 0;

			// パターンNo.数を初期化
			pEffType->nPatternNum = 0;

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (strcmp(aDataSearch, "EFF_GENERAL_00_TYPE_END") == 0)
				{// エフェクト全般[00] の種類毎の情報の読み込みを終了
					pEffType++;
					break;
				}//====================//
				else if (strcmp(aDataSearch, "読み込み名:") == 0)
				{// 読み込み名を読み込む
					fscanf(pFile, "%s", pEffType->aLoadName);
				}//====================//
				else if (strcmp(aDataSearch, "テクスチャの相対パス:") == 0)
				{// テクスチャの相対パスを読み込む
					fscanf(pFile, "%s", pEffType->aTexturPath);
				}//====================//
				else if (strcmp(aDataSearch, "パターンの上限X:") == 0)
				{// パターン上限Xを読み込む
					fscanf(pFile, "%d", &pEffType->nPtnMaxX);
				}//====================//
				else if (strcmp(aDataSearch, "アニメパターンの下限:") == 0)
				{// アニメパターンの下限を読み込む
					fscanf(pFile, "%d", &pEffType->nAnimPtnMin);
				}//====================//
				else if (strcmp(aDataSearch, "アニメパターンの上限:") == 0)
				{// アニメパターンの上限を読み込む
					fscanf(pFile, "%d", &pEffType->nAnimPtnMax);
				}//====================//
				else if (strcmp(aDataSearch, "パターンの上限Y:") == 0)
				{// パターン上限Yを読み込む
					fscanf(pFile, "%d", &pEffType->nPtnMaxY);
				}//====================//
				else if (strcmp(aDataSearch, "アニメーションの時間:") == 0)
				{// アニメーションにかかる時間を読み込む
					fscanf(pFile, "%d", &pEffType->nAnimTime);
				}//====================//
				else if (strcmp(aDataSearch, "アニメーションの種類:") == 0)
				{// アニメーションの種類を読み込む
					fscanf(pFile, "%s", aDataSearch); // 検索

					// アニメーションの種類を文字列から読み込む
					StringLoadAnimType(aDataSearch, &pEffType->animType);
				}//====================//
				else if (strcmp(aDataSearch, "幅:") == 0)
				{// 幅を読み込む
					fscanf(pFile, "%f", &pEffType->fWidth);

					// 幅をピクセル単位で修正
					pEffType->fWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "高さ:") == 0)
				{// 高さを読み込む
					fscanf(pFile, "%f", &pEffType->fHeight);

					// 高さをピクセル単位で修正
					pEffType->fHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "向きの種類:") == 0)
				{// 向きの種類を読み込む
					fscanf(pFile, "%s", aDataSearch); // 検索

					// 向きの種類を文字列から読み込む
					StringLoadAngleType(aDataSearch, &pEffType->angleType);
				}//====================//
				else if (strcmp(aDataSearch, "寿命:") == 0)
				{// 寿命を読み込む
					fscanf(pFile, "%d", &pEffType->nLife);
				}//====================//
				else if (strcmp(aDataSearch, "SOUND") == 0)
				{// サウンドの情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "SOUND_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}//====================//
						else if (strcmp(aDataSearch, "ループ時間:") == 0)
						{// サウンドのループ時間を読み込む
							fscanf(pFile, "%d", &pEffType->nSoundLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "サウンド:") == 0)
						{// サウンドを鳴らす時間を読み込む
							fscanf(pFile, "%d", &pEffType->aSound[pEffType->nSoundNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // 検索

							// サウンドの種類を文字列から読み込む
							StringLoadSound(aDataSearch, &pEffType->aSound[pEffType->nSoundNum].soundLabel);

							// サウンド数を加算
							pEffType->nSoundNum++;
						}//====================//
					}
				}//====================//
				else if (strcmp(aDataSearch, "PATTERN") == 0)
				{// パターンNo.の情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "PATTERN_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}//====================//
						else if (strcmp(aDataSearch, "ループ時間:") == 0)
						{// パターンNo.のループ時間を読み込む
							fscanf(pFile, "%d", &pEffType->nPatternLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "パターン:") == 0)
						{// パターンNo.を変更する時間を読み込む
							fscanf(pFile, "%d", &pEffType->aPattern[pEffType->nPatternNum].nTime);

							// パターンNo.を読み込む
							fscanf(pFile, "%d", &pEffType->aPattern[pEffType->nPatternNum].nPattern);

							// パターンNo.数を加算
							pEffType->nPatternNum++;
						}//====================//
					}
				}//====================//
				else if (strcmp(aDataSearch, "SUMMON") == 0)
				{// 発射の情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "SUMMON_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}//====================//
						else if (strcmp(aDataSearch, "ループ時間:") == 0)
						{// 召喚のループ時間を読み込む
							fscanf(pFile, "%d", &pEffType->nSummonLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "召喚:") == 0)
						{// 召喚する時間を読み込む
							fscanf(pFile, "%d", &pEffType->aSummon[pEffType->nSummonNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // 検索

							if (strcmp(aDataSearch, "[") == 0)
							{
								// 召喚する親番号と子番号を初期化
								pEffType->aSummon[pEffType->nSummonNum].nParentNum	= -1;
								pEffType->aSummon[pEffType->nSummonNum].nChildNum	= -1;

								while (1)
								{
									fscanf(pFile, "%s", aDataSearch); // 検索

									if (strcmp(aDataSearch, "]") == 0)
									{// 繰り返し処理を抜ける
										break;
									}//====================//
									else if (strcmp(aDataSearch, "種類:") == 0)
									{// エフェクト全般の種類を読み込む
										fscanf(pFile, "%s", aDataSearch); // 検索

										// エフェクト全般の種類を文字列から読み込む
										StringLoadEff_general_00Type(aDataSearch, &pEffType->aSummon[pEffType->nSummonNum].nEnemyType);
									}//====================//
									else if (strcmp(aDataSearch, "X:") == 0)
									{// 召喚する位置Xを読み込む
										fscanf(pFile, "%f", &pEffType->aSummon[pEffType->nSummonNum].pos.x);
									}//====================//
									else if (strcmp(aDataSearch, "Y:") == 0)
									{// 召喚する位置Yを読み込む
										fscanf(pFile, "%f", &pEffType->aSummon[pEffType->nSummonNum].pos.y);
									}//====================//
									else if (strcmp(aDataSearch, "角度:") == 0)
									{// 召喚する角度を読み込む
										fscanf(pFile, "%f", &pEffType->aSummon[pEffType->nSummonNum].fAngle);

										// 角度を正しい値に変換
										pEffType->aSummon[pEffType->nSummonNum].fAngle *= D3DX_PI;
									}//====================//
									else if (strcmp(aDataSearch, "親:") == 0)
									{// 親番号を読み込む
										fscanf(pFile, "%d", &pEffType->aSummon[pEffType->nSummonNum].nParentNum);
									}//====================//
									else if (strcmp(aDataSearch, "子:") == 0)
									{// 子供番号を読み込む
										fscanf(pFile, "%d", &pEffType->aSummon[pEffType->nSummonNum].nChildNum);
									}//====================//
								}
							}

							// 召喚数を加算
							pEffType->nSummonNum+=1;
						}
					}
				}//====================//
			}
		}
	}
}

//========================================
// StringLoadEff_general_00Type関数 - エフェクト全般[00] の種類を文字列から読み込む -
//========================================
void StringLoadEff_general_00Type(char aString[TXT_MAX], int *pType)
{
	Eff_general_00Type	*pEffType					//
						= g_aEff_general_00Type;	// エフェクト全般[00] の種類毎の情報

	for (int nCntChrType = 0; nCntChrType < EFF_GENERAL_00_TYPE_MAX; nCntChrType++, pEffType++)
	{
		if (strcmp(aString, pEffType->aLoadName) == 0)
		{// 読み込み名が一致した時、
			// エフェクト全般の種類を代入
			*pType = nCntChrType;
			break;
		}
	}
}

//========================================
// InitEff_general_00関数 - エフェクト全般[00] の初期化処理 -
//========================================
void InitEff_general_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntEff_general_00Type = 0; nCntEff_general_00Type < EFF_GENERAL_00_TYPE_MAX; nCntEff_general_00Type++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aEff_general_00Type[nCntEff_general_00Type].aTexturPath, &g_aTextureEff_general_00[nCntEff_general_00Type]);
	}
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * EFF_GENERAL_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_general_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_general_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_general_00	*pEff	// エフェクト全般[00] の情報のポインタ
					= g_aEff_general_00;

	for (int nCntChr = 0; nCntChr < EFF_GENERAL_00_MAX; nCntChr++, pVtx+=4, pEff++)
	{
		// パラメーターを初期化し、種類に応じて設定
		InitParameter(pEff);
		SetParameter(pEff);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pEff->pos,
			pEff->rot,
			true,
			pEff->fWidth,
			pEff->fHeight,
			g_aEff_general_00Type[pEff->nType].angleType);

		// rhwの設定
		SetRHW(pVtx);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			pEff->nPattern,
			g_aEff_general_00Type[pEff->nType].nPtnMaxX,
			g_aEff_general_00Type[pEff->nType].nPtnMaxY,
			NULL);
	}

	// 頂点座標をアンロックする
	g_pVtxBuffEff_general_00->Unlock();
}

//========================================
// UninitEff_general_00関数 - エフェクト全般[00] の終了処理 -
//========================================
void UninitEff_general_00(void)
{
	// テクスチャの破棄
	for (int nCntEff_general_00 = 0; nCntEff_general_00 < EFF_GENERAL_00_TYPE_MAX; nCntEff_general_00++)
	{
		if (g_aTextureEff_general_00[nCntEff_general_00] != NULL)
		{
			g_aTextureEff_general_00[nCntEff_general_00]->Release();
			g_aTextureEff_general_00[nCntEff_general_00] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEff_general_00 != NULL) 
	{
		g_pVtxBuffEff_general_00->Release();
		g_pVtxBuffEff_general_00 = NULL;
	}
}

//========================================
// UpdateEff_general_00関数 - エフェクト全般[00] の更新処理 -
//========================================
void UpdateEff_general_00(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_general_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_general_00	*pEff	// エフェクト全般[00] の情報のポインタ
					= g_aEff_general_00;

	for (int nCntChr = 0; nCntChr < EFF_GENERAL_00_MAX; nCntChr++, pEff++, pVtx+=4)
	{
		if (!pEff->bUse)
		{// 使用されていない時、
			// 繰り返し処理を折り返す
			continue;
		}
		else if (--pEff->nLife <= 0) 
		{// 寿命を減算した結果尽きた時、
			// 使用していない状態にする
			pEff->bUse = false;

			// 繰り返し処理を折り返す
			continue;
		}

		// 親の追跡処理
		ParentTrackingProcess(pEff);

		// サウンド読み込み処理
		SoundLoading(pEff);

		// パターンNo.読み込み処理
		PatternLoading(pEff);

		// 召喚読み込み処理
		SummonLoading(pEff);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&pEff->nPattern,
				g_aEff_general_00Type[pEff->nType].nAnimPtnMin,
				g_aEff_general_00Type[pEff->nType].nAnimPtnMax,
				&pEff->nCounterAnim,
				g_aEff_general_00Type[pEff->nType].nAnimTime,
				g_aEff_general_00Type[pEff->nType].animType),
			g_aEff_general_00Type[pEff->nType].nPtnMaxX,
			g_aEff_general_00Type[pEff->nType].nPtnMaxY,
			NULL);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pEff->pos,
			pEff->rot,
			true,
			pEff->fWidth,
			pEff->fHeight,
			g_aEff_general_00Type[pEff->nType].angleType);
	}

	// 頂点座標をアンロックする
	g_pVtxBuffEff_general_00->Unlock();
}

//========================================
// DrawEff_general_00関数 - エフェクト全般[00] の描画処理 -
//========================================
void DrawEff_general_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEff_general_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Eff_general_00	*pEff					//
					= g_aEff_general_00;	// エフェクト全般[00] の情報のポインタ

	for (int nCntChr = 0; nCntChr < EFF_GENERAL_00_MAX; nCntChr++, pEff++)
	{
		if ((pEff->bUse)
			&& (pEff->bDraw))
		{// 使用されている状態、かつ描画フラグが真の時、
			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureEff_general_00[pEff->nType]);

			// エフェクト全般[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntChr * 4, 2);
		}
	}
}

//========================================
// SetEff_general_00関数 - エフェクト全般[00] の設定処理 -
//========================================
void SetEff_general_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bParent, D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pParentRot)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_general_00->Lock(0, 0, (void**)&pVtx, 0);

	Eff_general_00	*pEff					//
					= g_aEff_general_00;	// エフェクト全般[00] の情報のポインタ

	for (int nCntChr = 0; nCntChr < EFF_GENERAL_00_MAX; nCntChr++, pVtx += 4, pEff++)
	{
		if (pEff->bUse)
		{// 使用されている状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		// パラメータを初期化する
		InitParameter(pEff);

		pEff->pos		= pos;		// 位置を代入
		pEff->rot		= rot;		// 角度を代入
		pEff->nType		= nType;	// 種類を代入
		pEff->bChild	= bParent;	// 親子フラグを代入

		if (bParent) 
		{// 親子フラグが真の時、
			// 親の位置と向きのポインタを代入
			pEff->pParentPos = pParentPos;
			pEff->pParentRot = pParentRot;

			// 親の位置と向きを保存
			pEff->parentPosTemp = *pParentPos;
			pEff->parentRotTemp = *pParentRot;
		}

		// パラメータを種類に応じて設定
		SetParameter(pEff);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pEff->pos,
			pEff->rot,
			true,
			pEff->fWidth,
			pEff->fHeight,
			g_aEff_general_00Type[pEff->nType].angleType);

		// 使用している状態にする
		pEff->bUse = true;

		break;
	}

	// 頂点座標をアンロックする
	g_pVtxBuffEff_general_00->Unlock();
}
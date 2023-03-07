//========================================
// 
// CHR:敵[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** chr_enemy_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "polygon2D.h"
#include "text.h"
#include "player.h"
#include "chr_enemy_00.h"	// CHR:敵	[00]
#include "obj_castle_00.h"	
#include "ui_move-text_00.h"

//****************************************
// マクロ定義
//****************************************
// CHR:敵[00] の生成データファイルの相対パス
#define CHR_ENEMY_00_DATA_FILE_PATH	"data\\GAMEDATA\\CHARACTER\\CHR_ENEMY_00_GENERATOR.txt"

// CHR:敵[00] の最大数
// CHR:敵[00] の種類の最大数
#define CHR_ENEMY_00_TYPE_MAX	(4)
#define CHR_ENEMY_00_SET_MAX	(256)

// CHR:敵[00] のアニメーションにかかる時間
#define CHR_ENEMY_00_ANIM_TIME	(4)

// CHR:敵[00] の開始位置/終了位置
#define CHR_ENEMY_00_START_POS	D3DXVECTOR3((SCREEN_WIDTH*0.5f)+300.0f,SCREEN_HEIGHT-130.0f,0.0f)
#define CHR_ENEMY_00_END_POS	D3DXVECTOR3((SCREEN_WIDTH*0.5f)-280.0f,SCREEN_HEIGHT-130.0f,0.0f)

//****************************************
// 構造体の定義
//****************************************
// 設定情報
typedef struct 
{
	int nTime;	// 時間
	int nType;	// 種類
}Chr_enemy_00Set;

//****************************************
// プロトタイプ宣言
//****************************************
// CHR:敵[00] のパラメーター初期化処理
void InitParameterChr_enemy_00(Chr_enemy_00 *pChr);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureChr_enemy_00	// テクスチャへのポインタ
						[CHR_ENEMY_00_TYPE_MAX]
						= {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChr_enemy_00	// 頂点バッファへのポインタ
						= NULL;
Chr_enemy_00			g_aChr_enemy_00			// CHR:敵[00] の情報
						[CHR_ENEMY_00_MAX];
Chr_enemy_00Set			g_aChr_enemy_00Set		// 設定情報
						[CHR_ENEMY_00_SET_MAX];
int g_nSetCounter;
int g_nSetNum = 0;

// CHR:敵[00] の種類毎の情報
Chr_enemy_00Type g_aChr_enemy_00Type[CHR_ENEMY_00_TYPE_MAX] =
{
	{ "data\\TEXTURE\\Enemy\\Slime-Biue.png"	,120.0f,60.0f,11,1,300,CHR_ENEMY_00_ELEM_X },
	{ "data\\TEXTURE\\Enemy\\Slime-Yellow.png"	,120.0f,60.0f,11,1,300,CHR_ENEMY_00_ELEM_Y },
	{ "data\\TEXTURE\\Enemy\\Slime-Green.png"	,120.0f,60.0f,11,1,300,CHR_ENEMY_00_ELEM_A },
	{ "data\\TEXTURE\\Enemy\\Slime-Red.png"		,120.0f,60.0f,11,1,300,CHR_ENEMY_00_ELEM_B },
};

//========== *** CHR:敵[00] の情報を取得 ***
Chr_enemy_00 *GetChr_enemy_00(void)
{
	return g_aChr_enemy_00;
}

//========== *** CHR:敵[00] の種類毎の情報を取得 ***
Chr_enemy_00Type *GetChr_enemy_00Type(void)
{
	return g_aChr_enemy_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterChr_enemy_00関数 - CHR:敵[00] のパラメーター初期化処理 -
//========================================
void InitParameterChr_enemy_00(Chr_enemy_00 *pChr)
{
	pChr->nType = 0;
	pChr->bUse = false;
	pChr->nCounterMove = 0;	// 移動カウンター
	pChr->nPtn = 0;			// パターンNo.
	pChr->nCounterAnim = 0;	// アニメーションカウンター
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadChr_enemy_00関数 - CHR:敵[00] の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadChr_enemy_00(void)
{
	FILE			*pFile;		// ファイルポインタ
	char			aDataSearch	// データ検索用
					[TXT_MAX];
	Chr_enemy_00Set	*pSet		// MD:リザルト画面[00] の情報のポインタ
					= g_aChr_enemy_00Set;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(CHR_ENEMY_00_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if		(!strcmp(aDataSearch, "END"))	{ fclose(pFile); break; }							// 読み込みを終了
		else if (!strcmp(aDataSearch, "TIME:")) { 
			fscanf(pFile, "%d", &pSet->nTime);
			fscanf(pFile, "%d", &pSet->nType);
			pSet++;
			g_nSetNum++;
		}
	}
}

//========================================
// InitChr_enemy_00関数 - CHR:敵[00] の初期化処理 -
//========================================
void InitChr_enemy_00(void)
{
	g_nSetCounter = 0;

	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntType = 0; nCntType < CHR_ENEMY_00_TYPE_MAX; nCntType++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aChr_enemy_00Type[nCntType].aTexturePath, &g_aTextureChr_enemy_00[nCntType]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CHR_ENEMY_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChr_enemy_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++,pVtx+=4)
	{
		Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報
							= &g_aChr_enemy_00Type[pChr->nType];

		// パラメーターの初期化処理
		InitParameterChr_enemy_00(pChr);

		// rhwの設定
		SetRHW2D(pVtx);

		// テクスチャ座標の設定
		SetTexturePos2D(pVtx, 0, pType->nPtnWidth, pType->nPtnHeight, NULL);

		// 頂点カラーの設定
		SetVertexColor2D(pVtx, INITCOLOR);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// UninitChr_enemy_00関数 - CHR:敵[00] の終了処理 -
//========================================
void UninitChr_enemy_00(void)
{
	// テクスチャの破棄
	for (int nCntType = 0; nCntType < CHR_ENEMY_00_TYPE_MAX; nCntType++)
	{
		if (g_aTextureChr_enemy_00[nCntType] != NULL)
		{
			g_aTextureChr_enemy_00[nCntType]->Release();
			g_aTextureChr_enemy_00[nCntType] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffChr_enemy_00 != NULL) 
	{
		g_pVtxBuffChr_enemy_00->Release();
		g_pVtxBuffChr_enemy_00 = NULL;
	}
}

//========================================
// UpdateChr_enemy_00関数 - CHR:敵[00] の更新処理 -
//========================================
void UpdateChr_enemy_00(void)
{
	VERTEX_2D	*pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00Set	*pSet	// MD:リザルト画面[00] の情報のポインタ
					= g_aChr_enemy_00Set;

	for (int nCntSet = 0; nCntSet < g_nSetNum; nCntSet++,pSet++)
	{
		//フレーム単位
		if (g_nSetCounter == pSet->nTime - g_aChr_enemy_00Type[pSet->nType].nMoveTime)
		{// 設定時間に達した
			SetChr_enemy_00(pSet->nType);
		}
	}

	g_nSetCounter++;

	Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++,pVtx+=4)
	{
		if (!pChr->bUse) 
		{
			continue;
		}

		Chr_enemy_00Type	*pType	// CHR:敵[00] の種類毎の情報
							= &g_aChr_enemy_00Type[pChr->nType];

		if (++pChr->nCounterMove >= pType->nMoveTime + 20)
		{//敵が通り過ぎた
			Player *pPlayer = GetPlayer();
			pPlayer->nLife--;		//体力減らす
			pPlayer->nCombo = 0;	//コンボリセット
			pPlayer->nCounterComboMoveTime = 0;
			SetUi_moveText_00("MISS...", 1, pPlayer->pos, D3DXVECTOR3(0.0f, -3.0f, 0.0f), { 0,0,0,255 }, 60);	//動く文字設定
			pChr->bUse = false;
			SetDamageObj_castle_00();	// 城のダメージ設定
		}
		
		float fRate = (float)pChr->nCounterMove / (float)pType->nMoveTime;
		D3DXVECTOR3 setPos = 
			(CHR_ENEMY_00_START_POS * (1.0f - fRate)) +
			(CHR_ENEMY_00_END_POS * fRate);

		pChr->pos = setPos;

		// 頂点座標を設定
		SetVertexPos2D(pVtx,
			setPos,
			INITD3DXVECTOR3,
			false,
			pType->fWidth,
			pType->fHeight,
			ANGLE_TYPE_FIXED);

		// テクスチャ座標の設定処理
		SetTexturePos2D(
			pVtx, 
			Count(
				&pChr->nPtn, 
				0, 
				(pType->nPtnWidth*pType->nPtnHeight)-1,
				&pChr->nCounterAnim,
				CHR_ENEMY_00_ANIM_TIME,
				COUNT_TYPE_TURN),
			pType->nPtnWidth, pType->nPtnHeight, NULL);

		{
			char aString[TXT_MAX];
			Color col;

			switch (pType->elem)
			{
			case CHR_ENEMY_00_ELEM_A:
				sprintf(aString, "A");
				col = { 131,211,19,255 };
				break;
			case CHR_ENEMY_00_ELEM_B:
				sprintf(aString, "B");
				col = { 219,43,0,255 };
				break;
			case CHR_ENEMY_00_ELEM_X:
				sprintf(aString, "X");
				col = { 0,115,239,255 };
				break;
			case CHR_ENEMY_00_ELEM_Y:
				sprintf(aString, "Y");
				col = { 243,191,63,255 };
				break;
			}

			SetText2D(aString, 2, DISPLAY_CENTER, setPos + D3DXVECTOR3(0.0,-8.0f,0.0f), col, 1.0f, 1.0f);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// DrawChr_enemy_00関数 - CHR:敵[00] の描画処理 -
//========================================
void DrawChr_enemy_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffChr_enemy_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse)
		{
			continue;
		}

		// テクスチャの設定
		pDevice->SetTexture(0, g_aTextureChr_enemy_00[pChr->nType]);

		// CHR:敵[00] の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntChr, 2);
	}
}

//========================================
// SetChr_enemy_00関数 - CHR:敵[00] の設定処理 -
//========================================
void SetChr_enemy_00(int nType) 
{
	Chr_enemy_00	*pChr	// CHR:敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{
			continue;
		}

		pChr->bUse = true;
		pChr->nCounterMove = 0;
		pChr->nType = nType;

		break;
	}
}
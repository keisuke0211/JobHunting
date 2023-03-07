//========================================
// 
// UI:タイトルロゴ[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** ui_title-logo_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"
#include "ui_title-logo_00.h"	// UI:タイトルロゴ	[00]

//****************************************
// マクロ定義
//****************************************
// UI:タイトルロゴ[00] のデータファイルの相対パス
#define UI_TITLELOGO_00_DATA_FILE_PATH	"data\\GAMEDATA\\USERINTERFACE\\UI_TITLELOGO_00_DATA.txt"

//****************************************
// プロトタイプ宣言
//****************************************
// UI:タイトルロゴ[00] のパラメーター初期化処理
void InitParameterUi_titleLogo_00(void);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureUi_titleLogo_00	// テクスチャへのポインタ
						= NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_titleLogo_00	// 頂点バッファへのポインタ
						= NULL;
Ui_titleLogo_00			g_ui_titleLogo_00;			// UI:タイトルロゴ[00] の部品毎の情報

//========== *** UI:タイトルロゴ[00] の情報を取得 ***
Ui_titleLogo_00 *GetUi_titleLogo_00(void) 
{
	return &g_ui_titleLogo_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterUi_titleLogo_00関数 - UI:タイトルロゴ[00] のパラメーター初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterUi_titleLogo_00(void)
{
	Ui_titleLogo_00 *pUi	// UI:タイトルロゴ[00] の情報のポインタ
					= &g_ui_titleLogo_00;

	pUi->nCounterArrival = 0;	// 到着カウンター
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadUi_titleLogo_00関数 - UI:タイトルロゴ[00] の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadUi_titleLogo_00(void) 
{
	FILE			*pFile;		// ファイルポインタ
	char			aDataSearch	// データ検索用
					[TXT_MAX];
	Ui_titleLogo_00	*pUi		// UI:タイトルロゴ[00] の情報のポインタ
					= &g_ui_titleLogo_00;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(UI_TITLELOGO_00_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if		(!strcmp(aDataSearch, "END"))			{ fclose(pFile); break; }						// 読み込みを終了
		else if (!strcmp(aDataSearch, "TEXTURE_PATH:")) { fscanf(pFile, "%s", &pUi->aTexturePath); }	// ファイル名
		else if (!strcmp(aDataSearch, "WIDTH:"))		{ fscanf(pFile, "%f", &pUi->fWidth); }			// 幅
		else if (!strcmp(aDataSearch, "HEIGHT:"))		{ fscanf(pFile, "%f", &pUi->fHeight); }			// 高さ
		else if (!strcmp(aDataSearch, "START_POS:"))	{ LoadVector(pFile, &pUi->startPos); }			// 開始位置
		else if (!strcmp(aDataSearch, "END_POS:"))		{ LoadVector(pFile, &pUi->endPos); }			// 終了位置
		else if (!strcmp(aDataSearch, "ARRIVAL_TIME:")) { fscanf(pFile, "%d", &pUi->nArrivalTime); }	// 到着にかかる時間
	}
}

//========================================
// InitUi_titleLogo_00関数 - UI:タイトルロゴ[00] の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitUi_titleLogo_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ
	Ui_titleLogo_00		*pUi		// UI:タイトルロゴ[00] の部品毎の情報のポインタ
						= &g_ui_titleLogo_00;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, pUi->aTexturePath, &g_aTextureUi_titleLogo_00);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_titleLogo_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_titleLogo_00->Lock(0, 0, (void**)&pVtx, 0);

	// パラメーターの初期化処理
	InitParameterUi_titleLogo_00();

	// 頂点座標を設定
	SetVertexPos2D(pVtx,
		pUi->startPos,
		INITD3DXVECTOR3,
		false,
		pUi->fWidth,
		pUi->fHeight,
		ANGLE_TYPE_FIXED);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, INITCOLOR);

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_titleLogo_00->Unlock();
}

//========================================
// UninitUi_titleLogo_00関数 - UI:タイトルロゴ[00] の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitUi_titleLogo_00(void)
{
	// テクスチャの破棄
	if (g_aTextureUi_titleLogo_00 != NULL)
	{
		g_aTextureUi_titleLogo_00->Release();
		g_aTextureUi_titleLogo_00 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffUi_titleLogo_00 != NULL) 
	{
		g_pVtxBuffUi_titleLogo_00->Release();
		g_pVtxBuffUi_titleLogo_00 = NULL;
	}
}

//========================================
// UpdateUi_titleLogo_00関数 - UI:タイトルロゴ[00] の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateUi_titleLogo_00(void)
{
	VERTEX_2D				*pVtx;	// 頂点情報へのポインタ
	Ui_titleLogo_00			*pUi	// UI:タイトルロゴ[00] の部品毎の情報のポインタ
							= &g_ui_titleLogo_00;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_titleLogo_00->Lock(0, 0, (void**)&pVtx, 0);

	if (pUi->nCounterArrival < pUi->nArrivalTime)
	{// 到着カウンターが到着にかかる時間に達していない時、
		if (GetKeyboardTrigger(DIK_RETURN)||GetButtonTrigger(BUTTON_A))
		{// ENTERキーが押された瞬間、到着カウンターに上限を代入
			pUi->nCounterArrival = pUi->nArrivalTime;
		}
		else
		{// ENTERキーが押されていない時、到着カウンターを加算
			pUi->nCounterArrival++;
		}
	}

	// 頂点座標を設定
	{
		float	fRate	// カウンターの進行率
				= (float)pUi->nCounterArrival / (float)pUi->nArrivalTime;

		SetVertexPos2D(pVtx,
			(pUi->startPos * (1.0f - fRate)) +
			(pUi->endPos * fRate),
			INITD3DXVECTOR3,
			false,
			pUi->fWidth,
			pUi->fHeight,
			ANGLE_TYPE_FIXED);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUi_titleLogo_00->Unlock();
}

//========================================
// DrawUi_titleLogo_00関数 - UI:タイトルロゴ[00] の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawUi_titleLogo_00(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_titleLogo_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_aTextureUi_titleLogo_00);

	// UI:タイトルロゴ[00] の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
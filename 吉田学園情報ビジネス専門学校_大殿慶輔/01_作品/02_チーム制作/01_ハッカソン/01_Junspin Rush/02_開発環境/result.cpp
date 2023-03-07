//==========================================
//
//リザルト関係プログラム[result.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "ranking.h"
#include "md_game_00.h"
#include "sound.h"

//マクロ
#define RETURN_TIME			(5000)	//タイトル画面に戻るまでの時間
#define RESULT_USEPOLY		(1)		//ポリゴン数
#define RESULT_TEXTURE_NUM	(1)		//テクスチャ数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_TEXTURE_NUM];	//テクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffResult = NULL;			//頂点バッファポインタ
DWORD g_dwCurrentTime, g_dwStartTime;						//リザルト画面開始からの時間と現在の時間

//========================
//初期化処理
//========================
void InitResult(void)
{
	PlaySound(SOUND_LABEL_BGM_RESULT_000);

	int nCntResultPolyData;		//カウンタ
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\-PRESS ENTER-.png",
		&g_pTextureResult[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RESULT_USEPOLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffResult,
		NULL);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffResult->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntResultPolyData = 0; nCntResultPolyData < RESULT_USEPOLY; nCntResultPolyData++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos.x = 0.0f;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = SCREEN_WIDTH;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = 0.0f;
		pVtx[2].pos.y = SCREEN_HEIGHT;
		pVtx[3].pos.x = SCREEN_WIDTH;
		pVtx[3].pos.y = SCREEN_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxbuffResult->Unlock();

	//時間設定
	g_dwCurrentTime = 0;
	g_dwStartTime = timeGetTime();

	SetRanking(*GetScore());

	InitRanking();
}

//========================
//終了処理
//========================
void UninitResult(void)
{
	int nCntUninitResult;	//テクスチャ破棄カウンタ

	//ランキング終了処理
	UninitRanking();

	for (nCntUninitResult = 0; nCntUninitResult < RESULT_TEXTURE_NUM; nCntUninitResult++)
	{
		if (g_pTextureResult[nCntUninitResult] != NULL)
		{
			g_pTextureResult[nCntUninitResult]->Release();
			g_pTextureResult[nCntUninitResult] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxbuffResult != NULL)
	{
		g_pVtxbuffResult->Release();
		g_pVtxbuffResult = NULL;
	}
}

//========================
//更新処理
//========================
void UpdateResult(void)
{
	//ランキング更新
	UpdateRanking();

	g_dwCurrentTime = timeGetTime();

	if (GetKeyboardTrigger(DIK_RETURN) == true 
		&& GetFade() == FADE_NONE)
	{
		//タイトルに戻る
		SetFade(MODE_TITLE_00);
	}
}

//========================
//描画処理
//========================
void DrawResult(void)
{
	int nCntTexPattern;		//カウンタ

	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	for (nCntTexPattern = 0; nCntTexPattern < RESULT_TEXTURE_NUM; nCntTexPattern ++)
	{
		pDevice->SetTexture(0, g_pTextureResult[nCntTexPattern]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexPattern * 4, 2);
	}
	//ランキング描画
	DrawRanking();
}
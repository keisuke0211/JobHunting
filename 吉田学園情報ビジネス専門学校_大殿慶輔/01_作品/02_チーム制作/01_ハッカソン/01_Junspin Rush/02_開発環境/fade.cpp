//========================================
// 
// フェードの処理
// Author:西村 吏功
// 
//========================================
//  *** fade.cpp ***
//========================================
#include "main.h"
#include "fade.h"
#include "sound.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//========== マクロ定義			==========//
#define FADE_ALPHA_TRANSITIVE_VALUE	(0.025f)	// フェードの透明度の推移値

//========== グローバル宣言		==========//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファへのポインタ
FADE g_fade;									// フェードの状態
MODE g_modeNext;								// 次の画面(モード)
D3DXCOLOR g_colorFade;							// ポリゴン(フェード)の色

//========== フェードの状態を取得	======//
FADE GetFade(void) 
{
	return g_fade;
}

//========================================
// InitFade関数 - フェードの初期化処理 -
//========================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	POINT vertexPos[4];			// 四角形の頂点の座標

	g_fade		= FADE_IN;								// フェードイン状態に
	g_modeNext	= modeNext;								// 次の画面(モード)を設定
	g_colorFade	= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 黒いポリゴン(不透明)にする
	
	// BGMの音量を変更する
	SoundTypeVolumeChange(SOUND_TYPE_BGM, 0.0f);

	// モードの設定
	SetMode(g_modeNext);

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標を設定
	pVtx[0].pos.x = 0.0f;
	pVtx[0].pos.y = 0.0f;
	pVtx[1].pos.x = SCREEN_WIDTH;
	pVtx[1].pos.y = 0.0f;
	pVtx[2].pos.x = 0.0f;
	pVtx[2].pos.y = SCREEN_HEIGHT;
	pVtx[3].pos.x = SCREEN_WIDTH;
	pVtx[3].pos.y = SCREEN_HEIGHT;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点座標をアンロックする
	g_pVtxBuffFade->Unlock();
}

//========================================
// UninitFade関数 - フェードの終了処理 -
//========================================
void UninitFade(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffFade != NULL) 
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//========================================
// UpdateFade関数 - フェードの更新処理 -
//========================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE) 
	{
		if (g_fade == FADE_IN)
		{// フェードイン状態
			// ポリゴンを透明にしていく
			g_colorFade.a -= FADE_ALPHA_TRANSITIVE_VALUE;

			// BGMの音量を変更する
			SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - g_colorFade.a);

			if (g_colorFade.a <= 0.0f) 
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
		}
		else if (g_fade == FADE_OUT) 
		{// フェードアウト状態
			// ポリゴンを不透明にしていく
			g_colorFade.a += FADE_ALPHA_TRANSITIVE_VALUE;

			// BGMの音量を変更する
			SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - g_colorFade.a);

			if (g_colorFade.a >= 1.0f) 
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;

				// BGMを一括停止する
				StopSoundType(SOUND_TYPE_BGM);

				// モードを設定する
				SetMode(g_modeNext);
			}
		}

		VERTEX_2D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラーの設定																						   
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		// 頂点座標をアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//========================================
// DrawFade関数 - フェードの描画処理 -
//========================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// フェードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================================
// SetFade関数 - フェードの設定処理 -
//========================================
void SetFade(MODE modeNext)
{
	g_fade		= FADE_OUT;								// フェードアウト状態に
	g_modeNext	= modeNext;								// 次の画面(モード)を設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒いポリゴン(透明)にする
}
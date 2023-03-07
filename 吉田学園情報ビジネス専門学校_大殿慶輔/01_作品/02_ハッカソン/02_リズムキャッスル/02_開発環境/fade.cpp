//========================================
// 
// フェードの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** fade.cpp ***
//========================================
#include "main.h"
#include "fade.h"
#include "polygon2D.h"
#include "sound.h"

//****************************************
// マクロ定義
//****************************************

// フェードの透明度の推移値
#define FADE_ALPHA_TRANSITIVE_VALUE	(5)

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffFade	// 頂点バッファへのポインタ
						= NULL;
FADE					g_fade;			// フェードの状態
FADE					g_fadeTemp;		// フェードの状態を保存
MODE					g_modeNext;		// 次の画面(モード)
Color					g_colorFade;	// ポリゴン(フェード)の色

//========== *** フェードの状態を取得 ***
FADE GetFade(void) 
{
	return g_fade;
}

//========== *** フェードの切り替え取得 ***
bool GetFadeSwap(void) 
{
	return (g_fadeTemp == FADE_OUT) && (g_fade == FADE_IN);
}

//========================================
// InitFade関数 - フェードの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ
	VERTEX_2D			*pVtx;		// 頂点情報へのポインタ

	g_fade		= FADE_IN;			// フェードイン状態に
	g_modeNext	= modeNext;			// 次の画面(モード)を設定
	g_colorFade = { 0,0,0,255 };	// 黒いポリゴン(不透明)にする
	
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
	SetVertexPos2D(pVtx,
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		D3DXVECTOR3(0, 0, 0),
		false,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		ANGLE_TYPE_FIXED);

	// rhwの設定
	SetRHW2D(pVtx);

	// 頂点カラーの設定
	SetVertexColor2D(pVtx, g_colorFade);

	// テクスチャ座標の設定
	SetTexturePos2D(pVtx, 0, 1, 1, NULL);

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//========================================
// UninitFade関数 - フェードの終了処理 -
// Author:RIKU NISHIMURA
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
// Author:RIKU NISHIMURA
//========================================
void UpdateFade(void)
{
	// フェードの状態を保存
	g_fadeTemp = g_fade;

	if (g_fade != FADE_NONE) 
	{
		if (g_fade == FADE_IN)
		{// フェードイン状態
			// ポリゴンを透明にしていく
			g_colorFade.a -= FADE_ALPHA_TRANSITIVE_VALUE;

			if (g_modeNext != MODE_NONE)
			{// 次のモードが無しでない時、
				// BGMとSEの音量を変更する
				SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - ((float)g_colorFade.a / (float)255));
				SoundTypeVolumeChange(SOUND_TYPE_SE, 1.0f - ((float)g_colorFade.a / (float)255));
			}

			if (g_colorFade.a <= 0) 
			{// 透明度が0以下の時、
				g_colorFade.a = 0;	// 透明度を0に設定
				g_fade = FADE_NONE;	// フェードアウト状態にする
			}
		}
		else if (g_fade == FADE_OUT) 
		{// フェードアウト状態
			// ポリゴンを不透明にしていく
			g_colorFade.a += FADE_ALPHA_TRANSITIVE_VALUE;

			if (g_modeNext != MODE_NONE)
			{// 次のモードが無しでない時、
				// BGMとSEの音量を変更する
				SoundTypeVolumeChange(SOUND_TYPE_BGM, 1.0f - ((float)g_colorFade.a / (float)255));
				SoundTypeVolumeChange(SOUND_TYPE_SE, 1.0f - ((float)g_colorFade.a / (float)255));
			}

			if (g_colorFade.a > 255) 
			{// 透明度が最大値を上回った時、
				g_colorFade.a = 255;	// 透明度を最大値に設定
				g_fade = FADE_IN;		// フェードイン状態にする

				if (g_modeNext != MODE_NONE)
				{// 次のモードが無しでない時、
					// BGMとSEを一括で停止
					StopSoundType(SOUND_TYPE_BGM);
					StopSoundType(SOUND_TYPE_SE);

					// モードを設定する
					SetMode(g_modeNext);
				}
			}
		}

		VERTEX_2D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
																					   
		// 頂点カラーの設定
		SetVertexColor2D(pVtx, g_colorFade);

		// 頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//========================================
// DrawFade関数 - フェードの描画処理 -
// Author:RIKU NISHIMURA
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
// Author:RIKU NISHIMURA
//========================================
void SetFade(MODE modeNext)
{
	g_fade		= FADE_OUT;			// フェードアウト状態に
	g_modeNext	= modeNext;			// 次の画面(モード)を設定
	g_colorFade = { 0, 0, 0, 0 };	// 黒いポリゴン(透明)にする
}
//========================================
//
//ライフ000の処理
//Author:西村 吏功
//
//========================================
// *** ui_life_000.cpp ***
//========================================
#include "main.h"
#include "ui_life_000.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//========== マクロ定義			==========//
#define UI_LIFE_000_TEXTURE_PATH					"data\\TEXTURE\\UserInterface\\ui_life_000.png"		//ライフ000の表示形式
#define UI_LIFE_000_TEXTURE_X_PATTERN_MAX			(4)										//ライフ000のテクスチャのパターン数(横)
#define UI_LIFE_000_TEXTURE_Y_PATTERN_MAX			(1)										//ライフ000のテクスチャのパターン数(縦)
#define UI_LIFE_000_WIDTH							(PIXEL * 16)							//ライフ000の幅
#define UI_LIFE_000_HEIGHT							(PIXEL * 16)							//ライフ000の高さ
#define UI_LIFE_000_X								(PIXEL * 16)							//ライフ000の位置X
#define UI_LIFE_000_Y								(PIXEL * 16)							//ライフ000の位置Y
#define UI_LIFE_000_SPACE_X							(PIXEL * 16)							//ライフ000の配置間隔X
#define UI_LIFE_000_HEARTBEAT_ANIM_START_PATTERN	(0)										//ライフ000の鼓動のアニメーションの開始パターンNo.
#define UI_LIFE_000_HEARTBEAT_ANIM_END_PATTERN		(3)										//ライフ000の鼓動のアニメーションの終了パターンNo.
#define UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_0	(70)									//ライフ000の鼓動のアニメーションの切り替え時間[0]
#define UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_1	(3)										//ライフ000の鼓動のアニメーションの切り替え時間[1]
#define UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_2	(2)										//ライフ000の鼓動のアニメーションの切り替え時間[2]
#define UI_LIFE_000_STOP_PATTERN					(3)										//ライフ000の停止のパターンNo.

//========== グローバル宣言		==========//
LPDIRECT3DTEXTURE9 g_pTextureUi_life_000 = NULL;		//ライフ000のテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi_life_000 = NULL;	//ライフ000の頂点バッファへのポインタ
Ui_life_000 g_aUi_life_000[5];							//ライフ000の情報

//========================================
//InitUi_life_000関数 - ライフ000の初期化処理 -
//========================================
void InitUi_life_000(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int nCntUi_life_000;		//侵略者000のカウント

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, UI_LIFE_000_TEXTURE_PATH, &g_pTextureUi_life_000);

	for (nCntUi_life_000 = 0; nCntUi_life_000 < 5; nCntUi_life_000++)
	{
		g_aUi_life_000[nCntUi_life_000].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置を初期化する
		g_aUi_life_000[nCntUi_life_000].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向きを初期化する
		g_aUi_life_000[nCntUi_life_000].nPattern = 0;							//パターンNo.を初期化する
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 5,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi_life_000,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_life_000->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi_life_000 = 0; nCntUi_life_000 < 5; nCntUi_life_000++, pVtx += 4)
	{
		//頂点座標を設定
		pVtx[0].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x - (UI_LIFE_000_WIDTH * 0.5f) + (nCntUi_life_000 * UI_LIFE_000_SPACE_X);
		pVtx[0].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y - UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[1].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x + (UI_LIFE_000_WIDTH * 0.5f) + (nCntUi_life_000 * UI_LIFE_000_SPACE_X);
		pVtx[1].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y - UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[2].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x - (UI_LIFE_000_WIDTH * 0.5f) + (nCntUi_life_000 * UI_LIFE_000_SPACE_X);
		pVtx[2].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y + UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[3].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x + (UI_LIFE_000_WIDTH * 0.5f) + (nCntUi_life_000 * UI_LIFE_000_SPACE_X);
		pVtx[3].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y + UI_LIFE_000_HEIGHT * 0.5f;
		
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), ((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[1].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) + (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), ((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[2].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), (((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX)) + (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[3].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) + (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), (((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX)) + (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
	}

	//頂点座標をアンロックする
	g_pVtxBuffUi_life_000->Unlock();
}

//========================================
//UninitUi_life_000関数 - ライフ000の終了処理 -
//========================================
void UninitUi_life_000(void)
{
	//テクスチャの破棄
	if (g_pTextureUi_life_000 != NULL)
	{
		g_pTextureUi_life_000->Release();
		g_pTextureUi_life_000 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffUi_life_000 != NULL)
	{
		g_pVtxBuffUi_life_000->Release();
		g_pVtxBuffUi_life_000 = NULL;
	}
}

//========================================
//UpdateUi_life_000関数 - ライフ000の更新処理 -
//========================================
void UpdateUi_life_000(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int nCntUi_life_000;		//ライフ000のカウント

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_life_000->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi_life_000 = 0; nCntUi_life_000 < 5; nCntUi_life_000++, pVtx += 4)
	{
		//アニメーションのカウンターを加算
		g_aUi_life_000[nCntUi_life_000].nCounterAnim++;

		if (((g_aUi_life_000[nCntUi_life_000].nCounterAnim >= UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_0) && g_aUi_life_000[nCntUi_life_000].nPattern == UI_LIFE_000_HEARTBEAT_ANIM_START_PATTERN)
			|| ((g_aUi_life_000[nCntUi_life_000].nCounterAnim >= UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_1) && g_aUi_life_000[nCntUi_life_000].nPattern == UI_LIFE_000_HEARTBEAT_ANIM_START_PATTERN + 1)
			|| ((g_aUi_life_000[nCntUi_life_000].nCounterAnim >= UI_LIFE_000_HEARTBEAT_ANIM_CHANGE_TIME_2) && g_aUi_life_000[nCntUi_life_000].nPattern == UI_LIFE_000_HEARTBEAT_ANIM_START_PATTERN + 2))
		{//カウンターがアニメーションの切り替え時間に達した時

			//カウンターを初期化
			g_aUi_life_000[nCntUi_life_000].nCounterAnim = 0;

			//パターンを更新
			g_aUi_life_000[nCntUi_life_000].nPattern = ++g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_HEARTBEAT_ANIM_END_PATTERN;
		}

		if (nCntUi_life_000 >= GetPlayer()->nLife)
		{//ライフ000のカウントがプレイヤーの体力以上だった時

			//パターンNo.を停止で設定
			g_aUi_life_000[nCntUi_life_000].nPattern = UI_LIFE_000_STOP_PATTERN;
		}
		else if (g_aUi_life_000[nCntUi_life_000].nPattern == UI_LIFE_000_STOP_PATTERN)
		{//パターンNo.が停止だった時

			//パターンNo.を初期化する
			g_aUi_life_000[nCntUi_life_000].nPattern = 0;
		}

		//位置を設定する
		g_aUi_life_000[nCntUi_life_000].pos = D3DXVECTOR3(UI_LIFE_000_X + (nCntUi_life_000 * UI_LIFE_000_SPACE_X), UI_LIFE_000_Y, 0.0f);

		//頂点座標を設定
		pVtx[0].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x - UI_LIFE_000_WIDTH * 0.5f;
		pVtx[0].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y - UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[1].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x + UI_LIFE_000_WIDTH * 0.5f;
		pVtx[1].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y - UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[2].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x - UI_LIFE_000_WIDTH * 0.5f;
		pVtx[2].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y + UI_LIFE_000_HEIGHT * 0.5f;
		pVtx[3].pos.x = g_aUi_life_000[nCntUi_life_000].pos.x + UI_LIFE_000_WIDTH * 0.5f;
		pVtx[3].pos.y = g_aUi_life_000[nCntUi_life_000].pos.y + UI_LIFE_000_HEIGHT * 0.5f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), ((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[1].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) + (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), ((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[2].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), (((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX)) + (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
		pVtx[3].tex = D3DXVECTOR2((g_aUi_life_000[nCntUi_life_000].nPattern % UI_LIFE_000_TEXTURE_X_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) + (1.0f / UI_LIFE_000_TEXTURE_X_PATTERN_MAX), (((g_aUi_life_000[nCntUi_life_000].nPattern / UI_LIFE_000_TEXTURE_X_PATTERN_MAX) % UI_LIFE_000_TEXTURE_Y_PATTERN_MAX) * (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX)) + (1.0f / UI_LIFE_000_TEXTURE_Y_PATTERN_MAX));
	}

	//頂点座標をアンロックする
	g_pVtxBuffUi_life_000->Unlock();
}

//========================================
//DrawUi_life_000関数 - ライフ000の描画処理 -
//========================================
void DrawUi_life_000(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int nCntUi_life_000;		//ライフ000のカウント

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi_life_000, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureUi_life_000);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi_life_000->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntUi_life_000 = 0; nCntUi_life_000 < 5; nCntUi_life_000++) 
	{
		//侵略者000の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi_life_000 * 4, 2);
	}

	//頂点座標をアンロックする
	g_pVtxBuffUi_life_000->Unlock();
}
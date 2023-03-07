//◇======================◇
//｜　　  　　　　　　　　｜
//｜　リザルト画面の処理　｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"result ver2.h"
#include "ResultSource.h"
#include"sound.h"
#include "fade.h"

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_BG (1)      //背景の数
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffResult2 = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureResult2[NUM_BG] = {};		//テクスチャへのポインタ
int g_nCntTime2;											//一定時間経過で画面推移する処理に使う

//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【リザルト画面・処理】｜―――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇======================================◇
//｜　　　 リザルト画面の初期化処理　　 　｜
//◇======================================◇
void InitResult2(void)
{
	int nCntResult2;
	g_nCntTime2 = 0;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\WireWAARRIOR　ゲームクリア.png", &g_pTextureResult2[0]);

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffResult2,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffResult2->Lock(0, 0, (void **)&pVtx, 0);

	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇

	for (nCntResult2 = 0; nCntResult2 < NUM_BG; nCntResult2++, pVtx += 4)
	{
		//　　　　　　　　　　　　 x       y     z
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f); //右回りで設定する
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); //左回りで設定する


	//◇――――――――――――◇
	//｜　　　　rhwの設定　　　 ｜
	//◇――――――――――――◇
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//◇――――――――――――◇
		//｜　　　 頂点カラー 　　　｜
		//◇――――――――――――◇
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffResult2->Unlock();

	InitResulyScore();
	UpdateResulyScore();
	PlaySound(SOUND_LABEL_BGM003); //←再生したいサウンドを指定

}
//◇====================================◇
//｜　　　 リザルト画面の終了処理　　 　｜
//◇====================================◇
void UninitResult2(void)
{
	int nCntResult2;

	//サウンドの停止
	StopSound();

	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	for (nCntResult2 = 0; nCntResult2 < NUM_BG; nCntResult2++)
	{
		if (g_pTextureResult2[nCntResult2] != NULL)
		{
			g_pTextureResult2[nCntResult2]->Release();
			g_pTextureResult2[nCntResult2] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffResult2 != NULL)
	{
		g_pVtxbuffResult2->Release();
		g_pVtxbuffResult2 = NULL;
	}
	UninitResulyScore();

}
//◇====================================◇
//｜　　　 リザルト画面の更新処理　　 　｜
//◇====================================◇
void UpdateResult2(void)
{
	//画面推移（リザルト2 → タイトル）
	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE) {// 決定キー [ ENTERキー ] が押された

	 //モードの設定
		SetFade(MODE_TITLE);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_START && GetFade() == FADE_NONE)
	{
		//モードの設定
		SetFade(MODE_TITLE);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_A&& GetFade() == FADE_NONE)
	{
		//モードの設定
		SetFade(MODE_TITLE);
	}
	else if (GetFade() == FADE_NONE && g_nCntTime2 == SWITCH_TIME)
	{//リザルト画面に来てから10秒たったらタイトル画面に切り替わる

	 //モードの設定
		SetFade(MODE_TITLE);
	}
	else if (GetFade() == FADE_NONE)
	{
		g_nCntTime2++;		//フェードが FADE_NONE の状態になったら1フレームずつカウントする
	}
}
//◇====================================◇
//｜　　　 リザルト画面の描画処理　　 　｜
//◇====================================◇
void DrawResult2(void)
{
	int nCntResult2;

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffResult2, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇

	for (nCntResult2 = 0; nCntResult2 < NUM_BG; nCntResult2++)
	{
		pDevice->SetTexture(0, g_pTextureResult2[nCntResult2]);
		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,       //プリミティブの種類
		//	2,                                               //プリミティブ(ポリゴン)数
		//	&g_aVertex[0],                                   //頂点情報の先頭アドレス
		//	sizeof(VERTEX_2D));                              //頂点情報構造体のサイズ

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult2 * 4, 2);
	}

	DrawResulyScore();

}
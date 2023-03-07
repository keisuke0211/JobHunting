//◇======================◇
//｜　　  　　　　　　　　｜
//｜　リザルト画面の処理　｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"result ver1.h"
#include"sound.h"
#include "fade.h"

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_BG (2)      //背景の数
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffResult1 = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureResult1[NUM_BG] = {};		//テクスチャへのポインタ
int g_nCntTime1;											//一定時間経過で画面推移する処理に使う
int g_nCnTFlash1;
bool bFlash1;
Result1 g_Result1[NUM_BG];
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【リザルト画面・処理】｜―――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇======================================◇
//｜　　　 リザルト画面の初期化処理　　 　｜
//◇======================================◇
void InitResult1(void)
{
	int nCntResult1;
	g_nCntTime1 = 0;				//タイマーを初期化
	g_nCnTFlash1 = 0;
	bFlash1 = false;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチャの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\WireWAARRIOR　ゲームオーバー.png", &g_pTextureResult1[0]);

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\WireWAARRIOR　ゲームオーバー1.png", &g_pTextureResult1[1]);

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffResult1,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffResult1->Lock(0, 0, (void **)&pVtx, 0);

	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇
	for (nCntResult1 = 0; nCntResult1 < NUM_BG; nCntResult1++, pVtx += 4)
	{
		g_Result1[nCntResult1].bUse;
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
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffResult1->Unlock();
	PlaySound(SOUND_LABEL_BGM004); //←再生したいサウンドを指定
}
//◇====================================◇
//｜　　　 リザルト画面の終了処理　　 　｜
//◇====================================◇
void UninitResult1(void)
{
	int nCntResult1;

	//サウンドの停止
	StopSound();

	for (nCntResult1 = 0; nCntResult1 < NUM_BG; nCntResult1++)
	{
		//◇――――――――――――◇
		//｜　　テクスチャの破棄　　｜
		//◇――――――――――――◇
		if (g_pTextureResult1[nCntResult1] != NULL)
		{
			g_pTextureResult1[nCntResult1]->Release();
			g_pTextureResult1[nCntResult1] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffResult1 != NULL)
	{
		g_pVtxbuffResult1->Release();
		g_pVtxbuffResult1 = NULL;
	}
}
//◇====================================◇
//｜　　　 リザルト画面の更新処理　　 　｜
//◇====================================◇
void UpdateResult1(void)
{
	//画面推移（リザルト1 → タイトル）

	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{// 決定キー [ ENTERキー ] が押された

		 //モードの設定
		SetFade(MODE_TITLE);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_START && GetFade() == FADE_NONE)
	{
		//モードの設定
		SetFade(MODE_TITLE);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_A && GetFade() == FADE_NONE)
	{
		//モードの設定
		SetFade(MODE_TITLE);
	}
	else if (GetFade() == FADE_NONE && g_nCntTime1 == SWITCH_TIME)
	{//リザルト画面に来てから10秒たったらタイトル画面に切り替わる

		//モードの設定
		SetFade(MODE_TITLE);
	}
	else if (GetFade() == FADE_NONE)
	{
		g_nCntTime1++;		//フェードが FADE_NONE の状態になったら1フレームずつカウントする
	}
	//点滅
	g_nCnTFlash1++;

	if (g_nCnTFlash1 >= 26 && bFlash1 == false)
	{//点滅切り替え時間になった
		g_Result1[0].bUse = g_Result1[0].bUse ? false : true;
		g_nCnTFlash1 = 0;
	}
	else if (g_nCnTFlash1 >= 6 && bFlash1 == true)
	{
		g_Result1[0].bUse = g_Result1[0].bUse ? false : true;
		g_nCnTFlash1 = 0;
	}

}
//◇====================================◇
//｜　　　 リザルト画面の描画処理　　 　｜
//◇====================================◇
void DrawResult1(void)
{
	int nCntResult1;

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffResult1, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇
	for (nCntResult1 = 0; nCntResult1 < NUM_BG; nCntResult1++)
	{
		pDevice->SetTexture(0, g_pTextureResult1[nCntResult1]);
		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult1 * 4, 2);
	}
}
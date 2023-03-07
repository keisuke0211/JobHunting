//◇======================◇
//｜　　  　　　　　　　　｜
//｜　タイトル画面の処理　｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"title.h"
#include"sound.h"
#include "fade.h"
#include "player.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_BG (4)      //背景の数
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTitle = NULL;		//頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_BG] = {};    //テクスチャ(4枚分)へのポインタ
D3DXCOLOR g_colorTitle;								//ポリゴン(フェード)の色

int g_nCnTFlash;									 //点滅時間
bool bFlash;										 //点滅切り替え
TITLE g_Title[NUM_BG];								 //敵の情報
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【タイトル画面・処理】｜―――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇======================================◇
//｜　　　 タイトル画面の初期化処理　　 　｜
//◇======================================◇
void InitTitle(void)
{
	int nCntTitle;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\bg000.jpg",
		&g_pTextureTitle[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Custom.png",
		&g_pTextureTitle[1]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Aircraft.png",
		&g_pTextureTitle[2]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\-PRESS ENTER-.png",
		&g_pTextureTitle[3]);

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	g_nCnTFlash = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTitle,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTitle->Lock(0, 0, (void **)&pVtx, 0);

	bFlash = false;
	for (nCntTitle = 0; nCntTitle < NUM_BG; nCntTitle++, pVtx += 4)
	{
		g_colorTitle = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//黒いポリゴン(不透明)にしておく
		g_Title[nCntTitle].bUse = true;
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

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

		if (nCntTitle == 1 || nCntTitle == 2)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		else if (nCntTitle == 0 || nCntTitle == 3)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	g_Title[4].state = TITLESTATE_NORMAL;
	//頂点バッファのアンロック
	g_pVtxbuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM000); //←再生したいサウンドを指定

}
//◇====================================◇
//｜　　　 タイトル画面の終了処理　　 　｜
//◇====================================◇
void UninitTitle(void)
{
	int nCntTitle;

	//サウンドの停止
	StopSound();

	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (nCntTitle = 0; nCntTitle < NUM_BG; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTitle != NULL)
	{
		g_pVtxbuffTitle->Release();
		g_pVtxbuffTitle = NULL;
	}
}
//◇====================================◇
//｜　　　 タイトル画面の更新処理　　 　｜
//◇====================================◇
void UpdateTitle(void)
{	
	XINPUT_STATE *xInput = GetXInputState();
	Player *pPlayer = Getplayer();						//プレイヤーの情報へのポインタ


	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{// 決定キー [ ENTERキー ] が押された

		pPlayer->bBph = false;												//コントローラの振動オフ
		PlaySound(SOUND_LABEL_SE_START);									//←再生したいサウンドを指定
		bFlash = true;
		//モードの設定
		SetFade(MODE_TUTORIAL);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_START && GetFade() == FADE_NONE)
	{
		pPlayer->bBph = true;												//コントローラの振動オン
		PlaySound(SOUND_LABEL_SE_START);									//←再生したいサウンドを指定
		bFlash = true;
		//モードの設定
		SetFade(MODE_TUTORIAL);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_A&& GetFade() == FADE_NONE)
	{
		pPlayer->bBph = true;												//コントローラの振動オン
		PlaySound(SOUND_LABEL_SE_START);									//←再生したいサウンドを指定
		bFlash = true;
		//モードの設定
		SetFade(MODE_TUTORIAL);
	}
	//点滅
	g_nCnTFlash++;

			//フェードアウト
			g_colorTitle.a += 0.015f; //ポリゴンを不透明にしていく

			if (g_colorTitle.a >= 1.0f)
			{
				g_colorTitle.a = 1.0f;
			}

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTitle->Lock(0, 0, (void **)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorTitle.a);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorTitle.a);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorTitle.a);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorTitle.a);

	for (int nCntTitle = 0; nCntTitle < NUM_BG; nCntTitle++, pVtx += 4)
	{
		if (nCntTitle >= 1 && nCntTitle <= 2)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorTitle.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorTitle.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorTitle.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorTitle.a);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	g_pVtxbuffTitle->Unlock();


	if (g_nCnTFlash >= 26 && bFlash == false)
	{//点滅切り替え時間になった
		g_Title[3].bUse = g_Title[3].bUse ? false : true;
		g_nCnTFlash = 0;
	}
	else if (g_nCnTFlash >= 6 && bFlash == true)
	{
		g_Title[3].bUse = g_Title[3].bUse ? false : true;
		g_nCnTFlash = 0;
	}
}
//◇====================================◇
//｜　　　 タイトル画面の描画処理　　 　｜
//◇====================================◇
void DrawTitle(void)
{
	int nCntTitle;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTitle, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	
	for (nCntTitle = 0; nCntTitle < NUM_BG; nCntTitle++)
	{
		if (g_Title[nCntTitle].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);
			//◇――――――――――――◇
			//｜　　ポリゴンの描画　　　｜
			//◇――――――――――――◇

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 4);
		}
	}
}
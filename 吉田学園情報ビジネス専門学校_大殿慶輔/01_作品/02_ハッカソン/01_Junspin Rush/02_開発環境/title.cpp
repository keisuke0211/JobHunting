//◇======================◇
//｜　　  　　　　　　　　｜
//｜　タイトル画面の処理　｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"title.h"
#include"sound.h"
#include "fade.h"
#include "bg.h"
#include "ground.h"
#include "cloud.h"
#include "ui_frame_02.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_BG (2)      //背景の数
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
	PlaySound(SOUND_LABEL_BGM_TITLE_000);

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
		".\\data\\TEXTURE\\-PRESS ENTER-.png",
		&g_pTextureTitle[1]);

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

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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

	InitBg();		// 背景
	InitGround();	// 地面
	InitCloud();	// 雲
	SetCloud2();		// 雲設定
	InitUi_frame_02();

	SetStateUi_frame_02(UI_FRAME_02_STATE_IN_POP);
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

	UninitBg();		// 背景
	UninitGround();	// 地面
	UninitCloud();	// 雲
	UninitUi_frame_02();
}
//◇====================================◇
//｜　　　 タイトル画面の更新処理　　 　｜
//◇====================================◇
void UpdateTitle(void)
{	
	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{// 決定キー [ ENTERキー ] が押された
		PlaySound(SOUND_LABEL_SE_DETERMINATION_000);
		//モードの設定
		SetFade(MODE_GAME_00);
	}
	//点滅
	g_nCnTFlash++;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTitle->Lock(0, 0, (void **)&pVtx, 0);

	g_pVtxbuffTitle->Unlock();


	if (g_nCnTFlash >= 26 && bFlash == false)
	{//点滅切り替え時間になった
		g_Title[1].bUse = g_Title[1].bUse ? false : true;
		g_nCnTFlash = 0;
	}
	else if (g_nCnTFlash >= 6 && bFlash == true)
	{
		g_Title[1].bUse = g_Title[1].bUse ? false : true;
		g_nCnTFlash = 0;
	}

	UpdateBg();		// 背景
	UpdateGround();	// 地面
	UpdateCloud();	// 雲
	UpdateUi_frame_02();
}
//◇====================================◇
//｜　　　 タイトル画面の描画処理　　 　｜
//◇====================================◇
void DrawTitle(void)
{
	DrawCloud();	// 雲
	DrawBg();		// 背景
	DrawGround();	// 地面
	DrawUi_frame_02();

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
	
	for (nCntTitle = 1; nCntTitle < NUM_BG; nCntTitle++)
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
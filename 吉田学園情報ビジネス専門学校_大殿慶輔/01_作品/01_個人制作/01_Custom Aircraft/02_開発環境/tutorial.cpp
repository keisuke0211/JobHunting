//◇============================◇
//｜　　  　　　　　	  　　　｜
//｜　チュートリアル画面の処理　｜
//｜　Author:大殿慶輔　			｜
//｜							｜
//◇============================◇
#include"tutorial.h"
#include"sound.h"
#include "fade.h"

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTutorial = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureTutorial = NULL;			//テクスチャへのポインタ

int g_nCntTime5;
//◇======================================◇
//｜　　　 リザルト画面の初期化処理　　 　｜
//◇======================================◇
void InitTutorial(void)
{
	int nCntResult1;
	g_nCntTime5 = 0;				//タイマーを初期化
									//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチャの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\bg013.png", &g_pTextureTutorial);
	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTutorial,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTutorial->Lock(0, 0, (void **)&pVtx, 0);

	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇
	for (nCntResult1 = 0; nCntResult1 < 1; nCntResult1++, pVtx += 4)
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
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffTutorial->Unlock();
	PlaySound(SOUND_LABEL_BGM000); //←再生したいサウンドを指定
}
//◇====================================◇
//｜　　　 リザルト画面の終了処理　　 　｜
//◇====================================◇
void UninitTutorial(void)
{
	//サウンドの停止
	StopSound();

	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureTutorial != NULL)
	{
		g_pTextureTutorial->Release();
		g_pTextureTutorial = NULL;
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTutorial != NULL)
	{
		g_pVtxbuffTutorial->Release();
		g_pVtxbuffTutorial = NULL;
	}
}
//◇====================================◇
//｜　　　 リザルト画面の更新処理　　 　｜
//◇====================================◇
void UpdateTutorial(void)
{
	//画面推移（リザルト1 → タイトル）

	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{// 決定キー [ ENTERキー ] が押された

	 //モードの設定
		SetFade(MODE_GAME);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_START && GetFade() == FADE_NONE)
	{
		//モードの設定
		SetFade(MODE_GAME);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_A && GetFade() == FADE_NONE)
	{
		//モードの設定
		SetFade(MODE_GAME);
	}

	else if (GetFade() == FADE_NONE && g_nCntTime5 == SWITCH_TIME * 2)
	{//リザルト画面に来てから10秒たったらタイトル画面に切り替わる

	 //モードの設定
		SetFade(MODE_GAME);
	}
	else if (GetFade() == FADE_NONE)
	{
		g_nCntTime5++;		//フェードが FADE_NONE の状態になったら1フレームずつカウントする
	}

}
//◇====================================◇
//｜　　　 リザルト画面の描画処理　　 　｜
//◇====================================◇
void DrawTutorial1(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTutorial, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇
	pDevice->SetTexture(0, g_pTextureTutorial);
	//◇――――――――――――◇
	//｜　　ポリゴンの描画　　　｜
	//◇――――――――――――◇

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
}
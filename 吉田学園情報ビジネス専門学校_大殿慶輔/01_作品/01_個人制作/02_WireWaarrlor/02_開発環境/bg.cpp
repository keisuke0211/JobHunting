//◇====================◇
//｜　　　　　　　　　　｜
//｜　 　背景の処理　　 ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "bg.h"
#include "camera.h"
//****************************************
// マクロ定義
//****************************************
#define NUM_BG	(5) //ブロックの種類
//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBg = NULL; //頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureBg[NUM_BG] = {};//テクスチャ(3枚分)へのポインタ

float g_aTexV[NUM_BG];						 //テクスチャ座標の開始位置(V値)
//****************************************
// 背景の初期化処理
//****************************************
void InitBg(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,".\\data\\TEXTURE\\Dir.png",&g_pTextureBg[0]);
	D3DXCreateTextureFromFile(pDevice,".\\data\\TEXTURE\\PixelSky_layer01.png",&g_pTextureBg[1]);
	D3DXCreateTextureFromFile(pDevice,".\\data\\TEXTURE\\PixelSky_layer02.png",&g_pTextureBg[2]);
	D3DXCreateTextureFromFile(pDevice,".\\data\\TEXTURE\\PixelSky_layer03.png",&g_pTextureBg[3]);
	D3DXCreateTextureFromFile(pDevice,".\\data\\TEXTURE\\bg_mountain_000.png",&g_pTextureBg[4]);


	//テクスチャ座標の開始位置(V値)の初期化
	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aTexV[nCntBG] = 0.0f;
	}

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBg,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBg->Lock(0, 0, (void **)&pVtx, 0);

	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇

	for (int nCntBg = 0; nCntBg < NUM_BG; nCntBg++, pVtx += 4)
	{
		if (nCntBg == 0)
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 490.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 490.0f, 0.0f); //右回りで設定する
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); //左回りで設定する
		}
		else if (nCntBg == 1)
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f); //右回りで設定する
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); //左回りで設定する
		}
		else  if(nCntBg == 2 || nCntBg == 3)
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f); //右回りで設定する
			pVtx[2].pos = D3DXVECTOR3(0.0f, 575, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 575, 0.0f); //左回りで設定する
		}
		else if (nCntBg == 4)
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 400.0f, 0.0f); //右回りで設定する
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); //左回りで設定する
		}
		
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
	g_pVtxbuffBg->Unlock();
}
//****************************************
// 背景の終了処理
//****************************************
void UninitBg(void)
{

	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	for (int nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		if (g_pTextureBg[nCnt] != NULL)
		{
			g_pTextureBg[nCnt]->Release();
			g_pTextureBg[nCnt] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffBg != NULL)
	{
		g_pVtxbuffBg->Release();
		g_pVtxbuffBg = NULL;
	}
}
//****************************************
// 背景の更新処理
//****************************************
void UpdateBg(void)
{
	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBg->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntBG = 0; nCntBG < NUM_BG; nCntBG++,pVtx += 4)
	{
		//テクスチャ座標の開始位置(V値)の更新
		g_aTexV[1] += 0.0000f;
		g_aTexV[2] += 0.0002f;
		g_aTexV[3] += 0.0001f;

		//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(g_aTexV[nCntBG],0.09f);
		pVtx[1].tex = D3DXVECTOR2(g_aTexV[nCntBG] + 1.0f,0.09f);
		pVtx[2].tex = D3DXVECTOR2(g_aTexV[nCntBG],1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aTexV[nCntBG] + 1.0f,1.0f);
	}

	//頂点バッファのアンロック
	g_pVtxbuffBg->Unlock();
}
//****************************************
// 背景の描画処理
//****************************************
void DrawBg(void)
{

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffBg, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	for (int nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureBg[nCnt]);
		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt* 4, 2);
	}
}
//◇====================◇
//｜　　　　　　　　　　｜
//｜　タイムの処理      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "time.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_PLACE (2) //スコアの桁数

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTime = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;
D3DXVECTOR3 g_posTime;			//スコアの位置
int g_nTime;					//スコアの値

//==================================================================================================================================================
//―――――――――――――――――――――――――――――｜【スコア・処理】｜――――――――――――――――――――――――――――――――――
//==================================================================================================================================================

//◇================================◇
//｜　　　 タイムの初期化処理　　 　｜
//◇================================◇
void InitTime(void)
{
	int nCntScore;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\number002.png", &g_pTextureTime);

	g_posTime = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);	//位置を初期化
	g_nTime = 90;												//値を初期化

																		//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTime,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTime->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(16.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(16.0f, 50.0f, 0.0f);
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
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//頂点バッファのアンロック
	g_pVtxbuffTime->Unlock();

	SetTime(g_nTime);
}
//◇==============================◇
//｜　　　 スコアの終了処理　　 　｜
//◇==============================◇
void UninitTime(void)
{
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTime != NULL)
	{
		g_pVtxbuffTime->Release();
		g_pVtxbuffTime = NULL;
	}
}
//◇==============================◇
//｜　　　 スコアの更新処理　　 　｜
//◇==============================◇
void UpdateTime(void)
{
	int nCntScore;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTime->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		pVtx[0].pos = D3DXVECTOR3(635.0f + (nCntScore * 25.0f), 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(660.0f + (nCntScore * 25.0f), 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(635.0f + (nCntScore * 25.0f), 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(660.0f + (nCntScore * 25.0f), 60.0f, 0.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffTime->Unlock();
}
//◇==============================◇
//｜　　　 スコアの描画処理　　 　｜
//◇==============================◇
void DrawTime(void)
{
	int nCntScore;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTime, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇

	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}
//◇==============================◇
//｜　　　 スコアの設定処理　　 　｜
//◇==============================◇
void SetTime(int nScore)
{
	int nCntScore;
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	g_nTime = nScore;

	aTexU[0] = g_nTime % 100 / 10;
	aTexU[1] = g_nTime % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffTime->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffTime->Unlock();
}
//◇==============================◇
//｜　　　 スコアの加算処理　　 　｜
//◇==============================◇
void AddTime(int nValue)
{
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	int nCntScore;

	g_nTime += nValue;

	/*if (g_nScore >= GetTopScore())
	{
	SetTopScore(g_nScore);
	}*/
	aTexU[0] = g_nTime % 100 / 10;
	aTexU[1] = g_nTime % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffTime->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffTime->Unlock();
}
//◇========================◇
//｜　　 スコアの所得　　 　｜
//◇========================◇
int GetTime(void)
{
	return g_nTime;
}
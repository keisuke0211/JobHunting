//◇====================◇
//｜　　　　　　　　　　｜
//｜　スコアの処理      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "score.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_PLACE (4) //スコアの桁数

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffScore = NULL;
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
D3DXVECTOR3 g_posScore;			//スコアの位置
int g_nScore;					//スコアの値

//==================================================================================================================================================
//―――――――――――――――――――――――――――――｜【スコア・処理】｜――――――――――――――――――――――――――――――――――
//==================================================================================================================================================

//◇================================◇
//｜　　　 スコアの初期化処理　　 　｜
//◇================================◇
void InitScore(void)
{
	int nCntScore;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\number002.png", &g_pTextureScore);

		g_posScore = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);	//位置を初期化
		g_nScore = 0;														//値を初期化

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffScore,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++,pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f , 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(16.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f , 50.0f, 0.0f);
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
	g_pVtxbuffScore->Unlock();

	SetScore(g_nScore);
}
//◇==============================◇
//｜　　　 スコアの終了処理　　 　｜
//◇==============================◇
void UninitScore(void)
{
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffScore != NULL)
	{
		g_pVtxbuffScore->Release();
		g_pVtxbuffScore = NULL;
	}
}
//◇==============================◇
//｜　　　 スコアの更新処理　　 　｜
//◇==============================◇
void UpdateScore(void)
{
	int nCntScore;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++,pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		pVtx[0].pos = D3DXVECTOR3(220.0f + (nCntScore * 25.0f), 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(245.0f + (nCntScore * 25.0f), 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(220.0f + (nCntScore * 25.0f), 70.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(245.0f + (nCntScore * 25.0f), 70.0f, 0.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffScore->Unlock();
}
//◇==============================◇
//｜　　　 スコアの描画処理　　 　｜
//◇==============================◇
void DrawScore(void)
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
	pDevice->SetStreamSource(0, g_pVtxbuffScore, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇

	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}
//◇==============================◇
//｜　　　 スコアの設定処理　　 　｜
//◇==============================◇
void SetScore(int nScore)
{
	int nCntScore;
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	g_nScore = nScore;

	aTexU[0] = g_nScore % 10000		/ 1000;
	aTexU[1] = g_nScore % 1000		/ 100;
	aTexU[2] = g_nScore % 100		/ 10;
	aTexU[3] = g_nScore % 10		/ 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffScore->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++,pVtx += 4)
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
	g_pVtxbuffScore->Unlock();
}
//◇==============================◇
//｜　　　 スコアの加算処理　　 　｜
//◇==============================◇
void AddScore(int nValue)
{
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	int nCntScore;

	g_nScore += nValue;

	/*if (g_nScore >= GetTopScore())
	{
		SetTopScore(g_nScore);
	}*/
	aTexU[0] = g_nScore % 10000 / 1000;
	aTexU[1] = g_nScore % 1000 / 100;
	aTexU[2] = g_nScore % 100 / 10;
	aTexU[3] = g_nScore % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++,pVtx += 4)
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
	g_pVtxbuffScore->Unlock();
}
//◇========================◇
//｜　　 スコアの所得　　 　｜
//◇========================◇
int GetScore(void)
{
	return g_nScore;
}
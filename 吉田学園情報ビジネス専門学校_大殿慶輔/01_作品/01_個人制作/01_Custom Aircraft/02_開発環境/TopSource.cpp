//◇====================◇
//｜　　　　　　　　　　｜
//｜　スコアの処理      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "TopSource.h"

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_PLACE (8) //スコアの桁数

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTopScore = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTopScore = NULL;
D3DXVECTOR3 g_posTopScore;							//スコアの位置
int g_nTopScore;									//スコアの値

//==================================================================================================================================================
//―――――――――――――――――――――――――――――｜【スコア・処理】｜――――――――――――――――――――――――――――――――――
//==================================================================================================================================================

//◇================================◇
//｜　　　 スコアの初期化処理　　 　｜
//◇================================◇
void InitTopScore(void)
{
	int nCntScore;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\number000.png", &g_pTextureTopScore);

	g_posTopScore = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, 0);	//位置を初期化

	FILE *pFile;	//ファイルポインタを宣言


	// ファイルを開く
	pFile = fopen("data\\ranking.bin", "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ファイルからデータ数を書き込む
		fread(&g_nTopScore, sizeof(int), 1, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		g_nTopScore = 5000;
	}
	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTopScore,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTopScore->Lock(0, 0, (void **)&pVtx, 0);

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
	g_pVtxbuffTopScore->Unlock();

	SetTopScore(g_nTopScore);
}
//◇==============================◇
//｜　　　 スコアの終了処理　　 　｜
//◇==============================◇
void UninitTopScore(void)
{
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureTopScore != NULL)
	{
		g_pTextureTopScore->Release();
		g_pTextureTopScore = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTopScore != NULL)
	{
		g_pVtxbuffTopScore->Release();
		g_pVtxbuffTopScore = NULL;
	}
}
//◇==============================◇
//｜　　　 スコアの更新処理　　 　｜
//◇==============================◇
void UpdateTopScore(void)
{
	int nCntScore;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTopScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		pVtx[0].pos = D3DXVECTOR3(1070.0f + (nCntScore * 25.0f), 90.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1090.0f + (nCntScore * 25.0f), 90.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1070.0f + (nCntScore * 25.0f), 130.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1090.0f + (nCntScore * 25.0f), 130.0f, 0.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffTopScore->Unlock();
}
//◇==============================◇
//｜　　　 スコアの描画処理　　 　｜
//◇==============================◇
void DrawTopScore(void)
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
	pDevice->SetStreamSource(0, g_pVtxbuffTopScore, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇

	pDevice->SetTexture(0, g_pTextureTopScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}
//◇==============================◇
//｜　　　 スコアの設定処理　　 　｜
//◇==============================◇
void SetTopScore(int nScore)
{
	int nCntScore;
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	g_nTopScore = nScore;

	aTexU[0] = g_nTopScore % 100000000 / 10000000;
	aTexU[1] = g_nTopScore % 10000000 / 1000000;
	aTexU[2] = g_nTopScore % 1000000 / 100000;
	aTexU[3] = g_nTopScore % 100000 / 10000;
	aTexU[4] = g_nTopScore % 10000 / 1000;
	aTexU[5] = g_nTopScore % 1000 / 100;
	aTexU[6] = g_nTopScore % 100 / 10;
	aTexU[7] = g_nTopScore % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffTopScore->Lock(0, 0, (void **)&pVtx, 0);
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
	g_pVtxbuffTopScore->Unlock();
}
//◇========================◇
//｜　　 スコアの所得　　 　｜
//◇========================◇
int GetTopScore(void)
{
	return g_nTopScore;
}
//◇==============================◇
//｜　　　 スコアの加算処理　　 　｜
//◇==============================◇

//void AddTopScore(int nValue)
//{
//	int aTexU[NUM_PLACE]; //各桁の数字を格納
//	int nCntScore;
//
//	g_nTopScore += nValue;
//
//	aTexU[0] = g_nTopScore % 100000000 / 10000000;
//	aTexU[1] = g_nTopScore % 10000000 / 1000000;
//	aTexU[2] = g_nTopScore % 1000000 / 100000;
//	aTexU[3] = g_nTopScore % 100000 / 10000;
//	aTexU[4] = g_nTopScore % 10000 / 1000;
//	aTexU[5] = g_nTopScore % 1000 / 100;
//	aTexU[6] = g_nTopScore % 100 / 10;
//	aTexU[7] = g_nTopScore % 10 / 1;
//
//	//設定用ポインタ
//	VERTEX_2D *pVtx;
//
//	//頂点バッファのロック
//	g_pVtxbuffTopScore->Lock(0, 0, (void **)&pVtx, 0);
//
//	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
//	{
//		//◇――――――――――――◇
//		//｜　テクスチュ座標の設定　｜
//		//◇――――――――――――◇
//		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 1.0f);
//	}
//	//頂点バッファのアンロック
//	g_pVtxbuffTopScore->Unlock();
//}
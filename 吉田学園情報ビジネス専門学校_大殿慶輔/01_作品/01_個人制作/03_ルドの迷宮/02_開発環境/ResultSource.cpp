//◇====================◇
//｜　　　　　　　　　　｜
//｜　タイマーの処理    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "ResultSource.h"
#include "time.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_PLACE (2) //スコアの桁数

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTime11 = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTime11 = NULL;
D3DXVECTOR3 g_posTime11;			//スコアの位置
int g_nTime11;					//スコアの値
int g_InitTime11;					//初期のスコア
//==================================================================================================================================================
//―――――――――――――――――――――――――――――｜【スコア・処理】｜――――――――――――――――――――――――――――――――――
//==================================================================================================================================================

//◇================================◇
//｜　　　 スコアの初期化処理　　 　｜
//◇================================◇
void InitResulyScore(void)
{
	int nCntScore;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\number005.png", &g_pTextureTime11);

	g_posTime11 = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, 0);	//位置を初期化
	g_nTime11 = 300;														//値を初期化
	g_InitTime11 = 300;

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTime11,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTime11->Lock(0, 0, (void **)&pVtx, 0);

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
	g_pVtxbuffTime11->Unlock();

	SetResult(g_nTime11);
}
//◇==============================◇
//｜　　　 スコアの終了処理　　 　｜
//◇==============================◇
void UninitResulyScore(void)
{
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureTime11 != NULL)
	{
		g_pTextureTime11->Release();
		g_pTextureTime11 = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTime11 != NULL)
	{
		g_pVtxbuffTime11->Release();
		g_pVtxbuffTime11 = NULL;
	}
}
//◇==============================◇
//｜　　　 スコアの更新処理　　 　｜
//◇==============================◇
void UpdateResulyScore(void)
{
	int Score;

	Score = 90 - GetTime();
	SetResult(Score);
}
//◇==============================◇
//｜　　　 スコアの描画処理　　 　｜
//◇==============================◇
void DrawResulyScore(void)
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
	pDevice->SetStreamSource(0, g_pVtxbuffTime11, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇

	pDevice->SetTexture(0, g_pTextureTime11);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}
//◇==============================◇
//｜　　　 スコアの設定処理　　 　｜
//◇==============================◇
void SetResult(int nScore)
{
	int nCntScore;
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	g_nTime11 = nScore;

	aTexU[0] = g_nTime11 % 100 / 10;
	aTexU[1] = g_nTime11 % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffTime11->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		pVtx[0].pos = D3DXVECTOR3(500.0f + (nCntScore * 65.0f), 390.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(540.0f + (nCntScore * 65.0f), 390.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(500.0f + (nCntScore * 65.0f), 440.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(540.0f + (nCntScore * 65.0f), 440.0f, 0.0f);
		
		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffTime11->Unlock();
}
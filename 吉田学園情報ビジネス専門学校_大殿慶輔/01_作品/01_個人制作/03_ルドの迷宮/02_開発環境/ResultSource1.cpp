//◇====================◇
//｜　　　　　　　　　　｜
//｜　タイマーの処理    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "ResultSource1.h"
#include "score.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_PLACE (4) //スコアの桁数

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTime111 = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTime111 = NULL;
D3DXVECTOR3 g_posTime111;			//スコアの位置
int g_nTime111;					//スコアの値
//==================================================================================================================================================
//―――――――――――――――――――――――――――――｜【スコア・処理】｜――――――――――――――――――――――――――――――――――
//==================================================================================================================================================

//◇================================◇
//｜　　　 スコアの初期化処理　　 　｜
//◇================================◇
void InitResulyScore1(void)
{
	int nCntScore;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\number005.png", &g_pTextureTime111);

	g_posTime111 = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, 0);	//位置を初期化
	g_nTime111 = 0;															//値を初期化

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTime111,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTime111->Lock(0, 0, (void **)&pVtx, 0);

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
	g_pVtxbuffTime111->Unlock();

	SetResult1(g_nTime111);
}
//◇==============================◇
//｜　　　 スコアの終了処理　　 　｜
//◇==============================◇
void UninitResulyScore1(void)
{
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureTime111 != NULL)
	{
		g_pTextureTime111->Release();
		g_pTextureTime111 = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTime111 != NULL)
	{
		g_pVtxbuffTime111->Release();
		g_pVtxbuffTime111 = NULL;
	}
}
//◇==============================◇
//｜　　　 スコアの更新処理　　 　｜
//◇==============================◇
void UpdateResulyScore1(void)
{
	int Score;

	Score = GetScore();
	SetResult1(Score);
}
//◇==============================◇
//｜　　　 スコアの描画処理　　 　｜
//◇==============================◇
void DrawResulyScore1(void)
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
	pDevice->SetStreamSource(0, g_pVtxbuffTime111, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇

	pDevice->SetTexture(0, g_pTextureTime111);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}
//◇==============================◇
//｜　　　 スコアの設定処理　　 　｜
//◇==============================◇
void SetResult1(int nScore)
{
	int nCntScore;
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	g_nTime111 = nScore;

	aTexU[0] = g_nTime111 % 10000 / 1000;
	aTexU[1] = g_nTime111 % 1000 / 100;
	aTexU[2] = g_nTime111 % 100 / 10;
	aTexU[3] = g_nTime111 % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffTime111->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		pVtx[0].pos = D3DXVECTOR3(500.0f + (nCntScore * 65.0f), 530.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(540.0f + (nCntScore * 65.0f), 530.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(500.0f + (nCntScore * 65.0f), 580.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(540.0f + (nCntScore * 65.0f), 580.0f, 0.0f);

		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntScore] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntScore] * 0.1f), 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffTime111->Unlock();
}
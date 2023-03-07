//==========================================
//
//スコア表示プログラム[Score.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "score.h"
#include "player.h"

//マクロ
#define SCORE_USEPOLY			(15)	//ポリゴン数
#define SCORE_SIZE_WIDTH		(256)	//テクスチャサイズ幅（実際の4倍）
#define SCORE_SIZE_HEIGHT		(256)	//テクスチャサイズ高さ（実際の4倍）
#define SCORE_PATT_WIDTH		(8)		//テクスチャパターン幅
#define SCORE_PATT_HEIGHT		(8)		//テクスチャパターン高さ
#define SCORE_CHARPART_NUM		(7)		//文字部分で使用するポリゴン数
#define SCORE_NUMPART_NUM		(8)		//数字部分で使用するポリゴン数

//グローバル
LPDIRECT3DTEXTURE9 g_pTextureScore;							//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffScore;					//頂点バッファポインタ
D3DXVECTOR3 g_posScore;										//スコア座標
int g_nScore2;												//最大体力反映後のスコアの値

//========================
//初期化処理
//========================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ
	int nCntScore;				//カウンタ

	pDevice = GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\txt_font_000.PNG",
		&g_pTextureScore);

	//スコア位置・値・種類初期化
	g_posScore = D3DXVECTOR3(784.0f, 16.0f, 0.0f);
	g_nScore2 = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SCORE_USEPOLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffScore,
		NULL);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScore = 0; nCntScore < SCORE_USEPOLY; nCntScore++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x + (SCORE_SIZE_WIDTH / SCORE_PATT_WIDTH * nCntScore), 
			g_posScore.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + (SCORE_SIZE_WIDTH / SCORE_PATT_WIDTH * (nCntScore + 1)),
			g_posScore.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x + (SCORE_SIZE_WIDTH / SCORE_PATT_WIDTH * nCntScore),
			g_posScore.y + (SCORE_SIZE_HEIGHT / SCORE_PATT_HEIGHT), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + (SCORE_SIZE_WIDTH / SCORE_PATT_WIDTH * (nCntScore + 1)),
			g_posScore.y + (SCORE_SIZE_HEIGHT / SCORE_PATT_HEIGHT), 0.0f);

		//座標変換用係数設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxbuffScore->Unlock();
}

//========================
//終了処理
//========================
void UninitScore(void)
{
	int nCntTexUnini;		//テクスチャ破棄カウンタ

	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxbuffScore != NULL)
	{
		g_pVtxbuffScore->Release();
		g_pVtxbuffScore = NULL;
	}
}

//========================
//更新処理
//========================
void UpdateScore(void)
{
	int aTexU[SCORE_USEPOLY];		//各桁の数字格納
	int nCntScoreCulc;			//計算カウンタ

	//文字部分のパターン番号設定
	aTexU[0] = PATT_NUM_S;
	aTexU[1] = PATT_NUM_C;
	aTexU[2] = PATT_NUM_O;
	aTexU[3] = PATT_NUM_R;
	aTexU[4] = PATT_NUM_E;
	aTexU[5] = PATT_NUM_SPACE;
	aTexU[6] = PATT_NUM_COLON;

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffScore->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntScoreCulc = 0; nCntScoreCulc < SCORE_NUMPART_NUM; nCntScoreCulc++)
	{
		//数字分割
		aTexU[nCntScoreCulc + SCORE_CHARPART_NUM] = g_nScore2 % (int)pow(10, (SCORE_NUMPART_NUM - nCntScoreCulc)) / (int)pow(10, SCORE_NUMPART_NUM - 1 - nCntScoreCulc) + SCORE_PATT_OFFSET;
	}

	for (nCntScoreCulc = 0; nCntScoreCulc < SCORE_USEPOLY; nCntScoreCulc++, pVtx += 4)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((float)(aTexU[nCntScoreCulc] % SCORE_PATT_WIDTH) / SCORE_PATT_WIDTH,
			(float)(aTexU[nCntScoreCulc] / SCORE_PATT_WIDTH) / SCORE_PATT_HEIGHT);
		pVtx[1].tex = D3DXVECTOR2((float)(aTexU[nCntScoreCulc] % SCORE_PATT_WIDTH + 1) / SCORE_PATT_WIDTH,
			(float)(aTexU[nCntScoreCulc] / SCORE_PATT_WIDTH) / SCORE_PATT_HEIGHT);
		pVtx[2].tex = D3DXVECTOR2((float)(aTexU[nCntScoreCulc] % SCORE_PATT_WIDTH) / SCORE_PATT_WIDTH,
			(float)(aTexU[nCntScoreCulc] / SCORE_PATT_WIDTH + 1) / SCORE_PATT_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2((float)(aTexU[nCntScoreCulc] % SCORE_PATT_WIDTH + 1) / SCORE_PATT_WIDTH,
			(float)(aTexU[nCntScoreCulc] / SCORE_PATT_WIDTH + 1) / SCORE_PATT_HEIGHT);
	}

	//頂点バッファをアンロック
	g_pVtxbuffScore->Unlock();
}

//========================
//描画処理
//========================
void DrawScore(void)
{
	int nCntScore;				//カウンタ
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < SCORE_USEPOLY; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//========================
//設定処理
//========================
void SetScore(int nScore)
{
	g_nScore2 = nScore;
}

//========================
//スコア加算処理
//========================
void AddScore(int nValue)
{
	g_nScore2 += nValue;
}

//========================
//スコア取得
//========================
int GetScore2(void) 
{
	return g_nScore2;
}
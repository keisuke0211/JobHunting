//◇====================◇
//｜　　　　　　　　　　｜
//｜　ライフの処理      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "life.h"

//****************************************
//マクロ定義
//****************************************
#define NUM_PLACE (2) //ライフの桁数

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffLife = NULL;
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;
D3DXVECTOR3 g_posLife;			//スコアの位置
int g_nLife;					//スコアの値

//****************************************
//ライフの初期化処理
//****************************************
void InitLife(void)
{
	int nCntLife;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\number000.png", &g_pTextureLife);

	g_posLife = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, 0);	//位置を初期化
	g_nLife = 0;														//値を初期化

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffLife,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffLife->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntLife = 0; nCntLife < NUM_PLACE; nCntLife++, pVtx += 4)
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
	g_pVtxbuffLife->Unlock();

	SetLife(g_nLife);
}
//****************************************
// ライフの終了処理
//****************************************
void UninitLife(void)
{
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffLife != NULL)
	{
		g_pVtxbuffLife->Release();
		g_pVtxbuffLife = NULL;
	}
}
//****************************************
// ライフの更新処理
//****************************************
void UpdateLife(void)
{
	int nCntLife;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffLife->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntLife = 0; nCntLife < NUM_PLACE; nCntLife++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		pVtx[0].pos = D3DXVECTOR3(1200.0f + (nCntLife * 25.0f), 350.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1220.0f + (nCntLife * 25.0f), 350.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(1200.0f + (nCntLife * 25.0f), 390.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1220.0f + (nCntLife * 25.0f), 390.0f, 0.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffLife->Unlock();
}
//****************************************
// ライフの描画処理
//****************************************
void DrawLife(void)
{
	int nCntLife;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffLife, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇

	pDevice->SetTexture(0, g_pTextureLife);

	for (nCntLife = 0; nCntLife < NUM_PLACE; nCntLife++)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);
	}
}
//****************************************
// ライフの設定処理
//****************************************
void SetLife(int nScore)
{
	int nCntLife;
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	g_nLife = nScore;

	aTexU[0] = g_nLife % 100 / 10;
	aTexU[1] = g_nLife % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffLife->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntLife = 0; nCntLife < NUM_PLACE; nCntLife++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntLife] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntLife] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntLife] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntLife] * 0.1f), 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffLife->Unlock();
}
//****************************************
// ライフの加算処理
//****************************************
void AddLife(int nValue)
{
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	int nCntLife;

	g_nLife += nValue;

	aTexU[0] = g_nLife % 100 / 10;
	aTexU[1] = g_nLife % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffLife->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntLife = 0; nCntLife < NUM_PLACE; nCntLife++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntLife] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntLife] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntLife] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntLife] * 0.1f), 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffLife->Unlock();
}
//****************************************
// ライフの減算処理
//****************************************
void DeleteLife(int nValue)
{
	int aTexU[NUM_PLACE]; //各桁の数字を格納
	int nCntLife;

	//プレイヤーの体力がなくなったら
	if (g_nLife == 0)
	{ }

	else if (g_nLife > 0)
	{
		g_nLife -= nValue;
	}

	aTexU[0] = g_nLife % 100 / 10;
	aTexU[1] = g_nLife % 10 / 1;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffLife->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntLife = 0; nCntLife < NUM_PLACE; nCntLife++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f + (aTexU[nCntLife] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (aTexU[nCntLife] * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (aTexU[nCntLife] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (aTexU[nCntLife] * 0.1f), 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffLife->Unlock();
}
//◇========================◇
//｜　　 スコアの所得　　 　｜
//◇========================◇
int GetLife(void)
{
	return g_nLife;
}
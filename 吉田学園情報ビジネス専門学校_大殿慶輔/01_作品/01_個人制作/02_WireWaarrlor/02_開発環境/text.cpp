//◇======================◇
//｜　　　　　　　　　　　｜
//｜　文字画像・表示処理  ｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"text.h"

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_MAX (1) //テキスト画像の数

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffText = NULL;
LPDIRECT3DTEXTURE9 g_pTextureText[NUM_MAX] = {};

//====================================================================================================================================================
//―――――――――――――――――――――――――――――｜【テキスト・処理】｜――――――――――――――――――――――――――――――――――
//====================================================================================================================================================

//◇==================================◇
//｜　　　 テキストの初期化処理　　 　｜
//◇==================================◇
void InitText(void)
{
	int nCntText;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\TIME.png", &g_pTextureText[0]);



	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffText,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffText->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntText = 0; nCntText < NUM_MAX; nCntText++,pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(100.0f, 50.0f, 0.0f);
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
		pVtx[0].tex = D3DXVECTOR2(0.01f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.01f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffText->Unlock();
}

//◇==============================◇
//｜　　　 テキストの終了処理　　 　｜
//◇==============================◇
void UninitText(void)
{
	int nCnttext;
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (nCnttext = 0; nCnttext < NUM_MAX; nCnttext++)
	{
		if (g_pTextureText[nCnttext] != NULL)
		{
			g_pTextureText[nCnttext]->Release();
			g_pTextureText[nCnttext] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffText != NULL)
	{
		g_pVtxbuffText->Release();
		g_pVtxbuffText = NULL;
	}
}
//◇============================◇
//｜　　　 文字の更新処理　　 　｜
//◇============================◇
void UpdateText(void)
{
	int nCntText;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffText->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntText = 0; nCntText < NUM_MAX; nCntText++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		if (nCntText == 0)
		{
			pVtx[0].pos = D3DXVECTOR3(1000.0f, 10.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1150.0f, 10.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(1000.0f, 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1150.0f, 50.0f, 0.0f);
		}
	}
	//頂点バッファのアンロック
	g_pVtxbuffText->Unlock();
}
//◇============================◇
//｜　　　 テキストの描画処理　 ｜
//◇============================◇
void DrawaText(void)
{
	int nCntText;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffText, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇
	for (nCntText = 0; nCntText < NUM_MAX; nCntText++)
	{
		pDevice->SetTexture(0, g_pTextureText[nCntText]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntText * 4, 2);
	}
}
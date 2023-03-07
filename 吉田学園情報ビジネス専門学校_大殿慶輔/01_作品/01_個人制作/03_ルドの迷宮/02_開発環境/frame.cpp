//◇======================◇
//｜　　　　　　　　　　　｜
//｜　枠・表示処理		　｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include "camera.h"
#include "frame.h"

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_FRAME (3) //テキスト画像の数

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffFrame = NULL;
LPDIRECT3DTEXTURE9 g_pTextureFrame = NULL;
Frame g_aFrame[MAX_FRAME];
//====================================================================================================================================================
//―――――――――――――――――――――――――――――｜【テキスト・処理】｜――――――――――――――――――――――――――――――――――
//====================================================================================================================================================

//◇==================================◇
//｜　　　 テキストの初期化処理　　 　｜
//◇==================================◇
void InitFrame(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\frame.png", &g_pTextureFrame);


	for (int nCntFrame = 0; nCntFrame < MAX_FRAME; nCntFrame++)
	{
		g_aFrame[nCntFrame].pos = DEFAULT_D3DXVECTOR3;
		g_aFrame[nCntFrame].fHight = 0.0f;
		g_aFrame[nCntFrame].fWidth = 0.0f;
		g_aFrame[nCntFrame].bUse = false;
	}

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FRAME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffFrame,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffFrame->Lock(0, 0, (void **)&pVtx, 0);
	for (int nCntFrame = 0; nCntFrame < MAX_FRAME; nCntFrame++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(g_aFrame[nCntFrame].pos.x, g_aFrame[nCntFrame].pos.y, g_aFrame[nCntFrame].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aFrame[nCntFrame].pos.x + g_aFrame[nCntFrame].fWidth, g_aFrame[nCntFrame].pos.y, g_aFrame[nCntFrame].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aFrame[nCntFrame].pos.x, g_aFrame[nCntFrame].pos.y + g_aFrame[nCntFrame].fHight, g_aFrame[nCntFrame].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aFrame[nCntFrame].pos.x + g_aFrame[nCntFrame].fWidth, g_aFrame[nCntFrame].pos.y + g_aFrame[nCntFrame].fHight, g_aFrame[nCntFrame].pos.z);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffFrame->Unlock();
}

//◇==============================◇
//｜　　　 テキストの終了処理　　 ｜
//◇==============================◇
void UninitFrame(void)
{
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureFrame != NULL)
	{
		g_pTextureFrame ->Release();
		g_pTextureFrame = NULL;
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffFrame != NULL)
	{
		g_pVtxbuffFrame->Release();
		g_pVtxbuffFrame = NULL;
	}
}
//◇============================◇
//｜　　　 文字の更新処理　　 　｜
//◇============================◇
void UpdateFrame(void)
{
	Camera *pCamera = Getcamara(0);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffFrame->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntFrame = 0; nCntFrame < MAX_FRAME; nCntFrame++, pCamera++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		if (pCamera->bUse == true && nCntFrame >= 1)
		{
			g_aFrame[nCntFrame].pos.x = pCamera->viewport.X;
			g_aFrame[nCntFrame].pos.y = pCamera->viewport.Y;
			g_aFrame[nCntFrame].fWidth = pCamera->viewport.Width;
			g_aFrame[nCntFrame].fHight = pCamera->viewport.Height;
			g_aFrame[nCntFrame].bUse = true;


			pVtx[0].pos = D3DXVECTOR3(g_aFrame[nCntFrame].pos.x, g_aFrame[nCntFrame].pos.y, g_aFrame[nCntFrame].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aFrame[nCntFrame].pos.x + g_aFrame[nCntFrame].fWidth, g_aFrame[nCntFrame].pos.y, g_aFrame[nCntFrame].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aFrame[nCntFrame].pos.x, g_aFrame[nCntFrame].pos.y + g_aFrame[nCntFrame].fHight, g_aFrame[nCntFrame].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aFrame[nCntFrame].pos.x + g_aFrame[nCntFrame].fWidth, g_aFrame[nCntFrame].pos.y + g_aFrame[nCntFrame].fHight, g_aFrame[nCntFrame].pos.z);
		}
		else if (pCamera->bUse == false)
		{
			g_aFrame[nCntFrame].bUse = false;
		}
	}
	//頂点バッファのアンロック
	g_pVtxbuffFrame->Unlock();
}
//◇============================◇
//｜　　　 テキストの描画処理　 ｜
//◇============================◇
void DrawaFrame(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	for (int nCntFrame = 0; nCntFrame < MAX_FRAME; nCntFrame++)
	{
		if (g_aFrame[nCntFrame].bUse == true)
		{
			//◇――――――――――――◇
			//｜データストリームの設定　｜
			//◇――――――――――――◇

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxbuffFrame, 0, sizeof(VERTEX_2D));

			//◇――――――――――――◇
			//｜頂点フォーマットの設定　｜
			//◇――――――――――――◇
			pDevice->SetFVF(FVF_VERTEX_2D);

			//◇――――――――――――◇
			//｜　　テクスチュの設定　　｜
			//◇――――――――――――◇
				pDevice->SetTexture(0, g_pTextureFrame);

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFrame * 4, 2);
		}
	}
}
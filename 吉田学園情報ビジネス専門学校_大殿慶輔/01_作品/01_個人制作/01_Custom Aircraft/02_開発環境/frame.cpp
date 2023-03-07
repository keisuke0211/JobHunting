//◇====================◇
//｜　　　　　　　　　　｜
//｜　 　枠の処理　　   ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "frame.h"

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffFrame = NULL;		 //頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureFrame = NULL;			//テクスチャ(3枚分)へのポインタ

//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【背景・処理】｜―――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇============================◇
//｜　　　 枠の初期化処理　　 　｜
//◇============================◇
void InitFrame(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\bg02.jpg",
		&g_pTextureFrame);
	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffFrame,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffFrame->Lock(0, 0, (void **)&pVtx, 0);

	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇

	//　　　　　　　　　　　　 x       y     z
	pVtx[0].pos = D3DXVECTOR3(1000.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f); //右回りで設定する
	pVtx[2].pos = D3DXVECTOR3(1000.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f); //左回りで設定する

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

	pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

	//頂点バッファのアンロック
	g_pVtxbuffFrame->Unlock();
}
//◇==============================◇
//｜　　  　背景の終了処理　　 　 ｜
//◇==============================◇
void UninitFrame(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureFrame != NULL)
	{
		g_pTextureFrame->Release();
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
//◇==============================◇
//｜　　　  背景の更新処理　 　　 ｜
//◇==============================◇
void UpdateFrame(void)
{

}
//◇==============================◇
//｜　　 　 背景の描画処理 　　　 ｜
//◇==============================◇
void DrawFrame(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

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
		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,      //プリミティブの種類
		//	2,                                               //プリミティブ(ポリゴン)数
		//	&g_aVertex[0],                                   //頂点情報の先頭アドレス
		//	sizeof(VERTEX_2D));                              //頂点情報構造体のサイズ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
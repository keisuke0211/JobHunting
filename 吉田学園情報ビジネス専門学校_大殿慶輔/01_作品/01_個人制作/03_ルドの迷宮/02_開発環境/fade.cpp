//◇====================◇
//｜　　　　　　　　　　｜
//｜　フェード処理	    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "fade.h"

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffFade = NULL;			//頂点バッファポインタ
FADE g_fade;											//フェードの状態
MODE g_modeNext;										//次の画面(モード)
D3DXCOLOR g_colorFade;									//ポリゴン(フェード)の色

//==============================================================================================================================================
//――――――――――――――――――――――――――――――｜【フェード・処理】｜―――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇======================================◇
//｜　　　 フェード処理の初期化処理　　 　｜
//◇======================================◇
void lnitFade(MODE modeNext)
{

	g_fade = FADE_IN;									//フェードイン状態に
	g_modeNext = modeNext;								//次の画面(モード)を設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒いポリゴン(不透明)にしておく

	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスの所得

	//頂点バッフの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffFade,
		NULL);
	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffFade->Lock(0, 0, (void **)&pVtx, 0);

	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇
	//　　　　　　　　　　　　 x       y     z
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f); //右回りで設定する
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); //左回りで設定する

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

	//頂点バッファのアンロック
	g_pVtxbuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);

}
//◇====================================◇
//｜　　　 フェード処理の終了処理　　 　｜
//◇====================================◇
void UninitFade(void)
{
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffFade != NULL)
	{
		g_pVtxbuffFade->Release();
		g_pVtxbuffFade = NULL;
	}
}
//◇====================================◇
//｜　　　 フェード処理の更新処理　　 　｜
//◇====================================◇
void UpdateFade(void)
{
	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffFade->Lock(0, 0, (void **)&pVtx, 0);

	if (g_fade != FADE_NONE)
	{

		if (g_fade == FADE_IN)
		{
			//フェードイン状態
			g_colorFade.a -= 0.05f;//ポリゴンを透明にしていく

			if (g_colorFade.a <= 0.0f)
			{

				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;//何もしていない状態にする
			}
		}

		else if (g_fade == FADE_OUT)
		{
			//フェードアウト
			g_colorFade.a += 0.025f; //ポリゴンを不透明にしていく

			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;//フェードイン状態

				//モード設定
				SetMode(g_modeNext);
			}

		}
	}
	//◇――――――――――――◇
	//｜　　　 頂点カラー 　　　｜
	//◇――――――――――――◇
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);

	//頂点バッファのアンロック
	g_pVtxbuffFade->Unlock();
}
//◇====================================◇
//｜　　　 フェード処理の描画処理　　 　｜
//◇====================================◇
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffFade, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
		pDevice->SetTexture(0, NULL);						//テクスチャを使用しないので NULL を指定！

	//◇――――――――――――◇
	//｜　　ポリゴンの描画　　　｜
	//◇――――――――――――◇

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 4);
}
//◇====================================◇
//｜　　　 フェード処理の設定処理　　 　｜
//◇====================================◇
void SetFade(MODE modeNext)
{
	g_fade = FADE_OUT;									//フェードアウト状態にする
	g_modeNext = modeNext;								//次の画面(モード)を設定
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//黒いポリゴンを(透明)にしておく

}
//◇====================================◇
//｜　　　 フェード処理の所得処理　　 　｜
//◇====================================◇
FADE GetFade(void)
{
		return g_fade;
}

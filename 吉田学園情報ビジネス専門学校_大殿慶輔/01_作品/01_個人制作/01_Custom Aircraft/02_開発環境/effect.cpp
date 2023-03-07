//◇====================◇
//｜　　　　　　　　　　｜
//｜　エフェクト処理    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"effect.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_EFFECT (48000) //エフェクトの最大数


//◇====================◇
//｜ 　　情報構造体 　　｜
//◇====================◇

//エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR col;		//頂点カラー
	float fRadius;		//半径（大きさ）
	int  nLife;			//寿命（表示時間）
	int nType;			//エフェクト種類
	bool bUse;			//使用しているかどうか
	float rRot;
} Effect;

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;			//テクスチャへのポインタ
Effect g_aEffect[MAX_EFFECT];

//◇==================================◇
//｜　　 エフェクトの初期化処理　　 　｜
//◇==================================◇
void InitEffect(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEffect;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチャの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);

	//爆弾の情報の初期化処理
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col	  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fRadius = 6.0f;
		g_aEffect[nCntEffect].nLife	  = 25;
		g_aEffect[nCntEffect].bUse	  = false;
		
	}

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{


		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //頂点データのポインタを４つ分進める
	}
	//頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}
//◇================================◇
//｜　　 エフェクトの終了処理　　 　｜
//◇================================◇
void UninitEffect(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇―――――――――――◇
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}
//◇================================◇
//｜　　 エフェクトの更新処理　　 　｜
//◇================================◇
void UpdateEffect(void)
{
	int nCntEffect;

	//テクスチャ座標の更新
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

			//エフェクの位置更新
			g_aEffect[nCntEffect].pos.x += g_aEffect[nCntEffect].move.x;
			g_aEffect[nCntEffect].pos.y += g_aEffect[nCntEffect].move.y;

			//サイズ変更
			g_aEffect[nCntEffect].fRadius -= g_aEffect[nCntEffect].fRadius / g_aEffect[nCntEffect].nLife;

			//頂点座標の設定
			pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;

			//不透明度更新
			g_aEffect[nCntEffect].col.a += 1.0f / g_aEffect[nCntEffect].nLife;

			//頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//寿命と大きさを減らしていく
			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife == 0)
			{//寿命が尽きた

				g_aEffect[nCntEffect].bUse = false;
			}
		}
		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	//頂点バッファのアンロック
	g_pVtxBuffEffect->Unlock();
}
//◇================================◇
//｜　　 エフェクトの描画処理　　 　｜
//◇================================◇
void DrawEffect(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEffect;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	pDevice->SetTexture(0, g_pTextureEffect);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

		 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}
	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//◇================================◇
//｜　 　エフェクトの設定処理　　 　｜
//◇================================◇
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife,int nType)
{
	int nCntEffect;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].bUse = true;

			//◇――――――――――――◇
			//｜ 　　頂点座標を設定　　 ｜
			//◇――――――――――――◇

			pVtx[0].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[0].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[1].pos.y = g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.x = g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius;
			pVtx[2].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.x = g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius;
			pVtx[3].pos.y = g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius;

			//◇――――――――――――◇
			//｜　　　 頂点カラー 　　　｜
			//◇――――――――――――◇
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;
			
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点バッファのアンロック
			g_pVtxBuffEffect->Unlock();
			break;
		}
	}
}
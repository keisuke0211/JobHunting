//◇====================◇
//｜　　　　　　　　　　｜
//｜　影の処理　　　    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "Torch.h"

//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTorch = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureTorch = NULL;			//テクスチャへのポインタ
Torch g_aTorch[NUM_TORCH];
//****************************************
// 初期化処理
//****************************************
void InitTorch(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\effect000.jpg",
		&g_pTextureTorch);

	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_TORCH,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTorch,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTorch->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < NUM_TORCH; nCntShadow++)
	{
		g_aTorch[nCntShadow].pos = DEFAULT_D3DXVECTOR3;
		g_aTorch[nCntShadow].rot = DEFAULT_D3DXVECTOR3;
		g_aTorch[nCntShadow].fRadius = 25.0f;
		g_aTorch[nCntShadow].aColor = 200;
		g_aTorch[nCntShadow].bSet = false;
		g_aTorch[nCntShadow].bUse = false;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTorch[nCntShadow].pos.x - g_aTorch[nCntShadow].fRadius, g_aTorch[nCntShadow].pos.y, g_aTorch[nCntShadow].pos.z + g_aTorch[nCntShadow].fRadius);
		pVtx[1].pos = D3DXVECTOR3(g_aTorch[nCntShadow].pos.x + g_aTorch[nCntShadow].fRadius, g_aTorch[nCntShadow].pos.y, g_aTorch[nCntShadow].pos.z + g_aTorch[nCntShadow].fRadius);
		pVtx[2].pos = D3DXVECTOR3(g_aTorch[nCntShadow].pos.x - g_aTorch[nCntShadow].fRadius, g_aTorch[nCntShadow].pos.y, g_aTorch[nCntShadow].pos.z - g_aTorch[nCntShadow].fRadius);
		pVtx[3].pos = D3DXVECTOR3(g_aTorch[nCntShadow].pos.x + g_aTorch[nCntShadow].fRadius, g_aTorch[nCntShadow].pos.y, g_aTorch[nCntShadow].pos.z - g_aTorch[nCntShadow].fRadius);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aTorch[nCntShadow].aColor);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aTorch[nCntShadow].aColor);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aTorch[nCntShadow].aColor);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aTorch[nCntShadow].aColor);

		//テクスチャ座標のの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffTorch->Unlock();
}
//****************************************
// 終了処理
//****************************************
void UninitTorch(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureTorch != NULL)
	{
		g_pTextureTorch->Release();
		g_pTextureTorch = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTorch != NULL)
	{
		g_pVtxbuffTorch->Release();
		g_pVtxbuffTorch = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdatTorch(void)
{

}
//****************************************
// 描画処理
//****************************************
void DrawTorch(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = DEFAULT_INT; nCntShadow < NUM_TORCH; nCntShadow++)
	{
		if (g_aTorch[nCntShadow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aTorch[nCntShadow].mtxWorld);

			//pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//D3DXMatrixInverse(&g_aTorch[nCntShadow].mtxWorld, NULL, &mtxView);	//逆行列を求める
			//g_aTorch[nCntShadow].mtxWorld._41 = DEFAULT_FLOA;
			//g_aTorch[nCntShadow].mtxWorld._42 = DEFAULT_FLOA;
			//g_aTorch[nCntShadow].mtxWorld._43 = DEFAULT_FLOA;

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aTorch[nCntShadow].rot.y, g_aTorch[nCntShadow].rot.x, g_aTorch[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_aTorch[nCntShadow].mtxWorld, &g_aTorch[nCntShadow].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aTorch[nCntShadow].pos.x, g_aTorch[nCntShadow].pos.y, g_aTorch[nCntShadow].pos.z);

			D3DXMatrixMultiply(&g_aTorch[nCntShadow].mtxWorld, &g_aTorch[nCntShadow].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aTorch[nCntShadow].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxbuffTorch, 0, sizeof(VERTEX_3D));

			//◇――――――――――――◇
			//｜頂点フォーマットの設定　｜
			//◇――――――――――――◇
			pDevice->SetFVF(FVF_VERTEX_3D);

			//◇――――――――――――◇
			//｜　　テクスチャの設定　　｜
			//◇――――――――――――◇

			pDevice->SetTexture(0, g_pTextureTorch);

			//◇――――――――――――◇
			//｜　　ポリゴンの描画　　　｜
			//◇――――――――――――◇

			//頂点情報構造体のサイズ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
}

//****************************************
// 設定処理
//****************************************
int SetTorch(void)
{
	int nCntShadow = -1;

	for (int mCntShadow = 0; mCntShadow < NUM_TORCH; mCntShadow++)
	{
		nCntShadow++;

		if (g_aTorch[mCntShadow].bUse == false && g_aTorch[mCntShadow].bSet == false)
		{
			g_aTorch[mCntShadow].pos = DEFAULT_D3DXVECTOR3;	//位置初期化
			g_aTorch[mCntShadow].rot = DEFAULT_D3DXVECTOR3;	//向き初期化
			g_aTorch[mCntShadow].bSet = true;		//対象が決まった
			break;
		}
	}

	return nCntShadow;
}
//****************************************
// 位置設定処理
//****************************************
void SetPositionTorch(int nIdxShadow, D3DXVECTOR3 pos, float fRadius)
{
	g_aTorch[nIdxShadow].pos = pos;
	g_aTorch[nIdxShadow].pos.y = 1;

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxbuffTorch->Lock(0, 0, (void* *)&pVtx, 0);

	pVtx += 4 * nIdxShadow;		//頂点座標をインデックス分進める

	g_aTorch[nIdxShadow].fRadius = fRadius;
	g_aTorch[nIdxShadow].aColor = 160 - (int)(pos.y * 1.3f);			//α値調整	
	g_aTorch[nIdxShadow].bUse = true;									//使用する

	pVtx[0].pos = D3DXVECTOR3(-g_aTorch[nIdxShadow].fRadius, 0.0f, +g_aTorch[nIdxShadow].fRadius);
	pVtx[1].pos = D3DXVECTOR3(+g_aTorch[nIdxShadow].fRadius, 0.0f, +g_aTorch[nIdxShadow].fRadius);
	pVtx[2].pos = D3DXVECTOR3(-g_aTorch[nIdxShadow].fRadius, 0.0f, -g_aTorch[nIdxShadow].fRadius);
	pVtx[3].pos = D3DXVECTOR3(+g_aTorch[nIdxShadow].fRadius, 0.0f, -g_aTorch[nIdxShadow].fRadius);


	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 196, 96, g_aTorch[nIdxShadow].aColor);
	pVtx[1].col = D3DCOLOR_RGBA(255, 196, 96, g_aTorch[nIdxShadow].aColor);
	pVtx[2].col = D3DCOLOR_RGBA(255, 196, 96, g_aTorch[nIdxShadow].aColor);
	pVtx[3].col = D3DCOLOR_RGBA(255, 196, 96, g_aTorch[nIdxShadow].aColor);

	//頂点バッファをロックする
	g_pVtxbuffTorch->Unlock();
}

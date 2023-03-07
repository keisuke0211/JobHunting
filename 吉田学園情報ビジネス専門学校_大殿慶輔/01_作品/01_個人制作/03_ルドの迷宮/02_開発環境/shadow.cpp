//◇====================◇
//｜　　　　　　　　　　｜
//｜　影の処理　　　    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "shadow.h"

//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffShadow = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;			//テクスチャへのポインタ
Shadow g_aShadow[MAX_SHADOW];
//****************************************
// 初期化処理
//****************************************
void InitShadow(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffShadow,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffShadow->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = DEFAULT_D3DXVECTOR3;
		g_aShadow[nCntShadow].rot = DEFAULT_D3DXVECTOR3;
		g_aShadow[nCntShadow].fWidhtX = 5.0f;
		g_aShadow[nCntShadow].fWidhtZ = 5.0f;
		g_aShadow[nCntShadow].aColor = 200;
		g_aShadow[nCntShadow].bSet = false;			
		g_aShadow[nCntShadow].bUse = false;	

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidhtX, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fWidhtZ);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidhtX, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fWidhtZ);
		pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidhtX, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fWidhtZ);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidhtX, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fWidhtZ);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aShadow[nCntShadow].aColor);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aShadow[nCntShadow].aColor);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aShadow[nCntShadow].aColor);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aShadow[nCntShadow].aColor);

		//テクスチャ座標のの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファのアンロック
	g_pVtxbuffShadow->Unlock();
}
//****************************************
// 終了処理
//****************************************
void UninitShadow(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffShadow != NULL)
	{
		g_pVtxbuffShadow->Release();
		g_pVtxbuffShadow = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdateShadow(void)
{
	
}
//****************************************
// 描画処理
//****************************************
void DrawShadow(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	
	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = DEFAULT_INT; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxbuffShadow, 0, sizeof(VERTEX_3D));

			//◇――――――――――――◇
			//｜頂点フォーマットの設定　｜
			//◇――――――――――――◇
			pDevice->SetFVF(FVF_VERTEX_3D);

			//◇――――――――――――◇
			//｜　　テクスチャの設定　　｜
			//◇――――――――――――◇

			pDevice->SetTexture(0, g_pTextureShadow);

			//◇――――――――――――◇
			//｜　　ポリゴンの描画　　　｜
			//◇――――――――――――◇

			//頂点情報構造体のサイズ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//****************************************
// 設定処理
//****************************************
int SetShadow(void)
{
	int nCntShadow = -1;

	for (int mCntShadow = 0; mCntShadow < MAX_SHADOW; mCntShadow++)
	{
		nCntShadow++;

		if (g_aShadow[mCntShadow].bUse == false && g_aShadow[mCntShadow].bSet == false)
		{
			g_aShadow[mCntShadow].pos = DEFAULT_D3DXVECTOR3;	//位置初期化
			g_aShadow[mCntShadow].rot = DEFAULT_D3DXVECTOR3;	//向き初期化
			g_aShadow[mCntShadow].bSet = true;		//対象が決まった
			break;
		}
	}

	return nCntShadow;
}
//****************************************
// 位置設定処理
//****************************************
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos,float fWidht, float fHight)
{
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].pos.y = 0;

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxbuffShadow->Lock(0, 0, (void* *)&pVtx, 0);

	pVtx += 4 * nIdxShadow;		//頂点座標をインデックス分進める
	
	g_aShadow[nIdxShadow].fWidhtX = fWidht;
	g_aShadow[nIdxShadow].fWidhtZ = fHight;
	g_aShadow[nIdxShadow].aColor = 200 - (int)(pos.y * 1.3f);			//α値調整	
	g_aShadow[nIdxShadow].bUse = true;									//使用する

	pVtx[0].pos = D3DXVECTOR3(- g_aShadow[nIdxShadow].fWidhtX, 0.0f, + g_aShadow[nIdxShadow].fWidhtZ);
	pVtx[1].pos = D3DXVECTOR3(+ g_aShadow[nIdxShadow].fWidhtX, 0.0f, + g_aShadow[nIdxShadow].fWidhtZ);
	pVtx[2].pos = D3DXVECTOR3(- g_aShadow[nIdxShadow].fWidhtX, 0.0f, - g_aShadow[nIdxShadow].fWidhtZ);
	pVtx[3].pos = D3DXVECTOR3(+ g_aShadow[nIdxShadow].fWidhtX, 0.0f, - g_aShadow[nIdxShadow].fWidhtZ);


	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aShadow[nIdxShadow].aColor);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aShadow[nIdxShadow].aColor);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aShadow[nIdxShadow].aColor);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aShadow[nIdxShadow].aColor);

	//頂点バッファをロックする
	g_pVtxbuffShadow->Unlock();
}

//****************************************
// 影を消す
//****************************************
void DeleteShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}
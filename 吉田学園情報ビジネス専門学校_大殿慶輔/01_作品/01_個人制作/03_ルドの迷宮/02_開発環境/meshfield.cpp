//◇====================================◇
//｜　　　　　　　　　　				｜
//｜　メッシュフィールドの処理          ｜
//｜　Author:大殿慶輔　					｜
//｜									｜
//◇====================================◇
#include "meshfield.h"

//****************************************
// グローバル変数
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureMeshfield = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;			//頂点バッファポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield = NULL;			//頂点バッファポインタ
D3DXVECTOR3 g_posMeshfield;//位置
D3DXVECTOR3 g_rotMeshfield;//向き
D3DXMATRIX g_mtxWorldMeshfield;//ワールドマトリックス

//****************************************
// 初期化処理
//****************************************
void InitMeshField(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\floor001.png",
		&g_pTextureMeshfield);

	g_posMeshfield = DEFAULT_D3DXVECTOR3;
	g_rotMeshfield = DEFAULT_D3DXVECTOR3;
	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffMeshfield->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-250,0,+250);
	pVtx[1].pos = D3DXVECTOR3(	 0,0,+250);
	pVtx[2].pos = D3DXVECTOR3(+250,0,+250);
	pVtx[3].pos = D3DXVECTOR3(-250,0,	0);
	pVtx[4].pos = D3DXVECTOR3(	 0,0,   0);
	pVtx[5].pos = D3DXVECTOR3(+250,0,	0);
	pVtx[6].pos = D3DXVECTOR3(-250,0,-250);
	pVtx[7].pos = D3DXVECTOR3(   0,0,-250);
	pVtx[8].pos = D3DXVECTOR3(+250,0,-250);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標のの設定

	//１枚を４分割して表示
	
	/*{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);
	}*/

	//〇×2のブロック分表示する
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(2.0f, 0.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[4].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[5].tex = D3DXVECTOR2(2.0f, 1.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 2.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 2.0f);
		pVtx[8].tex = D3DXVECTOR2(2.0f, 2.0f);
	}
	//頂点バッファのアンロック
	g_pVtxBuffMeshfield->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield,
		NULL);

	WORD *pIdx;//インデックス情報へのポインタ

	//インデックスバッファをロックしｍ頂点番号データへのポインタを所得
	g_pIdxBuffMeshfield->Lock(0, 0, (void**)&pIdx, 0);

	//頂点番号データの設定
	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshfield->Unlock();
}
//****************************************
// 終了処理
//****************************************
void UninitMeshField(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshfield != NULL)
	{
		g_pIdxBuffMeshfield->Release();
		g_pIdxBuffMeshfield = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdateMeshField(void)
{

}
//****************************************
// 描画処理
//****************************************
void DrawMeshField(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

												//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y,
		g_rotMeshfield.x, g_rotMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield, &g_mtxWorldMeshfield, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshfield);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,//用意した頂点の数
		0,
		12);//描画するプリミティブの数
}
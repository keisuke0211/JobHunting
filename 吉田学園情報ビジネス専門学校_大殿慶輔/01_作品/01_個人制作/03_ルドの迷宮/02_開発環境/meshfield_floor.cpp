//◇====================================◇
//｜　　　　　　　　　　				｜
//｜　メッシュフィールドの処理          ｜
//｜　Author:大殿慶輔　					｜
//｜									｜
//◇====================================◇
#include "meshfield_floor.h"
//****************************************
// グローバル変数
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureMeshfield_Floor = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield_Floor = NULL;			//頂点バッファポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield_Floor = NULL;			//頂点バッファポインタ
D3DXMATRIX g_mtxWorldMeshfield_Floor;//ワールドマトリックス

MeshFloor g_MeshFloor[NUM_MESHI];
//****************************************
// 初期化処理
//****************************************
void InitMeshField_Floor(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\stone brick.png",
		&g_pTextureMeshfield_Floor);

	g_MeshFloor[0].pos = D3DXVECTOR3(0.0f,0.0f,100.0f);
	g_MeshFloor[0].rot = DEFAULT_D3DXVECTOR3;
	g_MeshFloor[0].fWidhtX = 50;
	g_MeshFloor[0].fWidhtZ = 50;
	g_MeshFloor[0].nBlock_X = 30;
	g_MeshFloor[0].nBlock_Z = 30;
	g_MeshFloor[0].nVertex = (g_MeshFloor[0].nBlock_Z + 1) * (g_MeshFloor[0].nBlock_X + 1);
	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_MeshFloor[0].nVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield_Floor,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffMeshfield_Floor->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点座標/法線ベクトル/テクスチャ座標の設定	

	for (int nCntHeight = 0; nCntHeight < g_MeshFloor[0].nBlock_X + 1; nCntHeight++)
	{
		for (int nCntWidth = 0; nCntWidth < g_MeshFloor[0].nBlock_Z + 1; nCntWidth++)
		{
			pVtx[(nCntHeight * (g_MeshFloor[0].nBlock_Z + 1)) + nCntWidth].pos
				= D3DXVECTOR3(
					((float)nCntHeight - (float)g_MeshFloor[0].nBlock_X / 2) * g_MeshFloor[0].fWidhtX,
					0.0f,
					((float)nCntWidth - (float)g_MeshFloor[0].nBlock_Z / 2) * g_MeshFloor[0].fWidhtZ);

			pVtx[(nCntHeight * (g_MeshFloor[0].nBlock_Z + 1)) + nCntWidth].nor
				= D3DXVECTOR3(
					0.0f,
					1.0f,
					0.0f);

			pVtx[(nCntHeight * (g_MeshFloor[0].nBlock_Z + 1)) + nCntWidth].tex
				= D3DXVECTOR2(
					nCntHeight * ((float)g_MeshFloor[0].nBlock_Z / (float)g_MeshFloor[0].nBlock_Z),
					nCntWidth * ((float)g_MeshFloor[0].nBlock_X / (float)g_MeshFloor[0].nBlock_X));
		}
	}

	for (int nCntVtx = 0; nCntVtx < ((g_MeshFloor[0].nBlock_Z + 1) * (g_MeshFloor[0].nBlock_X + 1)); nCntVtx++)
	{
		// 頂点カラーの設定
		pVtx[nCntVtx].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//頂点バッファのアンロック
	g_pVtxBuffMeshfield_Floor->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((g_MeshFloor[0].nBlock_X * 2 * (g_MeshFloor[0].nBlock_Z + 1)) + ((g_MeshFloor[0].nBlock_X - 1) * 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield_Floor,
		NULL);

	WORD *pIdx;//インデックス情報へのポインタ

	//インデックスバッファをロックしｍ頂点番号データへのポインタを所得
	g_pIdxBuffMeshfield_Floor->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;	// インデックスのカウント

	//頂点番号データの設定
	for (int nCntDepth = 0; nCntDepth < g_MeshFloor[0].nBlock_X; nCntDepth++)
	{
		for (int nCntWidth = 0; nCntWidth < g_MeshFloor[0].nBlock_Z + 1; nCntWidth++)
		{
			pIdx[nCntIdx] = (nCntWidth + ((g_MeshFloor[0].nBlock_Z + 1) * (nCntDepth + 1)));
			pIdx[nCntIdx + 1] = nCntWidth + ((g_MeshFloor[0].nBlock_Z + 1) * nCntDepth);
			nCntIdx += 2;
		}

		if (nCntDepth < g_MeshFloor[0].nBlock_X - 1)
		{
			pIdx[nCntIdx] = ((g_MeshFloor[0].nBlock_Z + 1) * (nCntDepth + 1)) - 1;
			pIdx[nCntIdx + 1] = (g_MeshFloor[0].nBlock_Z + 1) * (nCntDepth + 2);
			nCntIdx += 2;
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshfield_Floor->Unlock();
}
//****************************************
// 終了処理
//****************************************
void UninitMeshField_Floor(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshfield_Floor != NULL)
	{
		g_pTextureMeshfield_Floor->Release();
		g_pTextureMeshfield_Floor = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshfield_Floor != NULL)
	{
		g_pVtxBuffMeshfield_Floor->Release();
		g_pVtxBuffMeshfield_Floor = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshfield_Floor != NULL)
	{
		g_pIdxBuffMeshfield_Floor->Release();
		g_pIdxBuffMeshfield_Floor = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdateMeshField_Floor(void)
{

}
//****************************************
// 描画処理
//****************************************
void DrawMeshField_Floor(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

												//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield_Floor);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshFloor[0].rot.y,
		g_MeshFloor[0].rot.x, g_MeshFloor[0].rot.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield_Floor, &g_mtxWorldMeshfield_Floor, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshFloor[0].pos.x, g_MeshFloor[0].pos.y, g_MeshFloor[0].pos.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield_Floor, &g_mtxWorldMeshfield_Floor, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield_Floor);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield_Floor, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshfield_Floor);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield_Floor);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		g_MeshFloor[0].nVertex,//用意した頂点の数
		0,
		((g_MeshFloor[0].nBlock_Z * g_MeshFloor[0].nBlock_X) * 2) + (4 * (g_MeshFloor[0].nBlock_X - 1)));//描画するプリミティブの数
}
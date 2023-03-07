//◇====================================◇
//｜　　　　　　　　　　				｜
//｜　メッシュフィールドの処理          ｜
//｜　Author:大殿慶輔　					｜
//｜									｜
//◇====================================◇
#include "meshfield_wall.h"
#include "Player_model.h"
#include "camera.h"
//****************************************
// グローバル変数
//****************************************
LPDIRECT3DTEXTURE9		g_pTextureMeshfield_Wall = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield_Wall = NULL;			//頂点バッファポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshfield_Wall = NULL;			//頂点バッファポインタ
//D3DXVECTOR3 g_posMeshfield_Wall;//位置
//D3DXVECTOR3 g_rotMeshfield_Wall;//向き
D3DXMATRIX g_mtxWorldMeshfield_Wall;//ワールドマトリックス

MeshWall g_MeshWall;
//****************************************
// 初期化処理
//****************************************
void InitMeshField_Wall(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\floor001.png",
		&g_pTextureMeshfield_Wall);

	/*g_posMeshfield_Wall = D3DXVECTOR3(0.0f, 0.0f, 750.0f);
	g_rotMeshfield_Wall = DEFAULT_D3DXVECTOR3;*/

	g_MeshWall.pos = D3DXVECTOR3(0.0f, 0.0f, 750.0f);
	g_MeshWall.rot = DEFAULT_D3DXVECTOR3;
	g_MeshWall.fWidhtX = 250;
	g_MeshWall.fWidhtZ = 500;
	g_MeshWall.nBlock_Z = g_MeshWall.nBlock_Z / 25;
	g_MeshWall.nBlock_X = g_MeshWall.nBlock_X / 25;
	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 9,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield_Wall,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffMeshfield_Wall->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-250, 500, 0);
	pVtx[1].pos = D3DXVECTOR3(0, 500, 0);
	pVtx[2].pos = D3DXVECTOR3(+250, 500, 0);
	pVtx[3].pos = D3DXVECTOR3(-250, 250, 0);
	pVtx[4].pos = D3DXVECTOR3(0, 250, 0);
	pVtx[5].pos = D3DXVECTOR3(+250, 250, 0);
	pVtx[6].pos = D3DXVECTOR3(-250, 0, 0);
	pVtx[7].pos = D3DXVECTOR3(0, 0, 0);
	pVtx[8].pos = D3DXVECTOR3(+250, 0, -0);

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
	g_pVtxBuffMeshfield_Wall->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshfield_Wall,
		NULL);

	WORD *pIdx;//インデックス情報へのポインタ

			   //インデックスバッファをロックしｍ頂点番号データへのポインタを所得
	g_pIdxBuffMeshfield_Wall->Lock(0, 0, (void**)&pIdx, 0);

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
	g_pIdxBuffMeshfield_Wall->Unlock();
}
//****************************************
// 終了処理
//****************************************
void UninitMeshField_Wall(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshfield_Wall != NULL)
	{
		g_pTextureMeshfield_Wall->Release();
		g_pTextureMeshfield_Wall = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshfield_Wall != NULL)
	{
		g_pVtxBuffMeshfield_Wall->Release();
		g_pVtxBuffMeshfield_Wall = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffMeshfield_Wall != NULL)
	{
		g_pIdxBuffMeshfield_Wall->Release();
		g_pIdxBuffMeshfield_Wall = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdateMeshField_Wall(void)
{

}
//****************************************
// 描画処理
//****************************************
void DrawMeshField_Wall(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

												//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshfield_Wall);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshWall.rot.y,
		g_MeshWall.rot.x, g_MeshWall.rot.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield_Wall, &g_mtxWorldMeshfield_Wall, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_MeshWall.pos.x, g_MeshWall.pos.y, g_MeshWall.pos.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshfield_Wall, &g_mtxWorldMeshfield_Wall, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshfield_Wall);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield_Wall, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshfield_Wall);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield_Wall);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		9,//用意した頂点の数
		0,
		12);//描画するプリミティブの数
}
//****************************************
// 当たり判定
//****************************************
void CollisionMeshField_Wall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	for (int nCntWall = 0; nCntWall < 1; nCntWall++)
	{
		if (pPos->x >= g_MeshWall.pos.x - (g_MeshWall.fWidhtX + SIZE_WIDTH) &&
			pPos->x <= g_MeshWall.pos.x + (g_MeshWall.fWidhtX + SIZE_WIDTH) &&
			pPos->y >= g_MeshWall.pos.y - (g_MeshWall.fWidhtZ + SIZE_HEIGHT) &&
			pPos->y <= g_MeshWall.pos.y + (g_MeshWall.fWidhtZ + SIZE_HEIGHT) &&
			pPos->z >= g_MeshWall.pos.z - (g_MeshWall.fWidhtX + SIZE_WIDTH) &&
			pPos->z <= g_MeshWall.pos.z + (g_MeshWall.fWidhtX + SIZE_WIDTH))
		{
			/*if (g_Wall[nCntWall].nCype == 0 || g_Wall[nCntWall].nCype == 1)
			{*/
				if (/*g_MeshWall.pos.y + g_MeshWall.fWidhtY >= pPosOld->y + SIZE_HEIGHT &&
					g_MeshWall.pos.y - g_MeshWall.fWidhtY <= pPosOld->y - SIZE_HEIGHT &&*/
					g_MeshWall.pos.z - 2 >= pPosOld->z + SIZE_WIDTH &&
					g_MeshWall.pos.z - 2 <= pPos->z + SIZE_WIDTH)
				{
					pPos->z = g_MeshWall.pos.z - (2 + SIZE_WIDTH);

					Getcamara(0)->posV.z = pPos->z - 50;
					Getcamara(0)->posR.z = pPos->z;
				}
				else if (/*g_MeshWall.pos.y + g_MeshWall.fWidhtY >= pPosOld->y + SIZE_HEIGHT &&
						 g_MeshWall.pos.y - g_MeshWall.fWidhtY <= pPosOld->y - SIZE_HEIGHT &&*/
						 g_MeshWall.pos.z + 2 <= pPosOld->z - SIZE_WIDTH &&
						 g_MeshWall.pos.z + 2 >= pPos->z - SIZE_WIDTH)
				{
					pPos->z = g_MeshWall.pos.z + (2 + SIZE_WIDTH);

					Getcamara(0)->posV.z = pPos->z - 50;
					Getcamara(0)->posR.z = pPos->z;
				}
			//}
			/*else if (g_Wall[nCntWall].nCype == 2 || g_Wall[nCntWall].nCype == 3)
			{*/
				/*if (g_MeshWall.pos.y + g_MeshWall.fWidhtY >= pPosOld->y + SIZE_HEIGHT &&
					g_MeshWall.pos.y - g_MeshWall.fWidhtY <= pPosOld->y - SIZE_HEIGHT &&
					g_MeshWall.pos.x - 2 >= pPosOld->x + SIZE_WIDTH &&
					g_MeshWall.pos.x - 2 <= pPos->x + SIZE_WIDTH)
				{
					pPos->x = g_MeshWall.pos.x - (2 + SIZE_WIDTH);

					Getcamara()->posV.x = pPos->x;
					Getcamara()->posR.x = pPos->x;
				}
				else if (g_MeshWall.pos.y + g_MeshWall.fWidhtY >= pPosOld->y + SIZE_HEIGHT &&
					g_MeshWall.pos.y - g_MeshWall.fWidhtY <= pPosOld->y - SIZE_HEIGHT &&
					g_MeshWall.pos.x + 2 <= pPosOld->x - SIZE_WIDTH &&
					g_MeshWall.pos.x + 2 >= pPos->x - SIZE_WIDTH)
				{
					pPos->x = g_MeshWall.pos.x + (2 + SIZE_WIDTH);

					Getcamara()->posV.x = pPos->x;
					Getcamara()->posR.x = pPos->x;
				}*/
			//}
		}
	}
}
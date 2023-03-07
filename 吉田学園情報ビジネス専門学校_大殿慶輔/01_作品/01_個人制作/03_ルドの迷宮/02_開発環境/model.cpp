//◇====================◇
//｜　　　　　　　　　　｜
//｜　モデル処理        ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "model.h"
//****************************************
// グローバル変数
//****************************************
LPD3DXMESH g_pMeshModel = NULL;						//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatModel = NULL;;				//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureModel[2] = {};			//テクスチャへのポインタ
DWORD g_dwNumMatModel = 0;							//マテリアルの数
D3DXVECTOR3 g_posModel;								//位置
D3DXVECTOR3 g_rotModel;								//向き
D3DXMATRIX  g_mtxWoridModel;						//ワールドマトリックス
//****************************************
// モデルの初期化処理
//****************************************
void InitModel(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_posModel = DEFAULT_D3DXVECTOR3;
	g_rotModel = DEFAULT_D3DXVECTOR3;
	//Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/Luna.x",/*読み込むファイル名*/
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_dwNumMatModel,
		&g_pMeshModel);

	D3DXMATERIAL *pMat;							//マテリアルへのポインタ

												//マテリアル情報に対するポインタを所得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する

		 //このファイル名を使用してテクスチャを読み込む
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureModel[nCntMat]);
		}
	}
}
//****************************************
// モデルの終了処理
//****************************************
void UninitModel(void)
{
	//メッシュの破棄
	if (g_pMeshModel != NULL)
	{
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatModel != NULL)
	{
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}
//****************************************
// モデルの更新処理
//****************************************
void UpdateModel(void)
{

}
//****************************************
// モデルの描画処理
//****************************************
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの所得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

												//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWoridModel);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y,g_rotModel.x, g_rotModel.z);

	D3DXMatrixMultiply(&g_mtxWoridModel, &g_mtxWoridModel, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posModel.x, g_posModel.y, g_posModel.z);

	D3DXMatrixMultiply(&g_mtxWoridModel, &g_mtxWoridModel, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWoridModel);

	//現在のマテリアルを所得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを所得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModel[nCntMat]);

		//モデル(パーツ)の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

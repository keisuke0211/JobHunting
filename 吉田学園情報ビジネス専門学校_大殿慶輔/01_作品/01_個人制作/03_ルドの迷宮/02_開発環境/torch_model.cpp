//****************************************
//
//	松明の処理
//	Author:大殿慶輔
//
//****************************************
#include "torch_model.h"
#include "torch.h"
//****************************************
// グローバル変数
//****************************************
LPD3DXMESH g_pMeshOrnamentModel_T[NUM_TORCH_M] = {};							//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatOrnamentModel_T[NUM_TORCH_M] = {};						//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureOrnamentModel_T[NUM_TORCH_M][MAX_TEXTURE] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatOrnamentModel_T[NUM_TORCH_M] = {};								//マテリアルの数
D3DXMATRIX  g_mtxWoridFigureModel_T[NUM_TORCH_M];								//ワールドマトリックス

Torch_Model g_Torch[NUM_TORCH_M];
//****************************************
// モデルの初期化処理
//****************************************
void InitTorch_Model(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pMat;							//マテリアルへのポインタ

	for (int nCntType = 0; nCntType < NUM_TORCH_M; nCntType++)
	{
		g_Torch[nCntType].pos = D3DXVECTOR3(g_Torch[nCntType].pos.x, g_Torch[nCntType].pos.y, g_Torch[nCntType].pos.z);

		if (g_Torch[nCntType].nType == 0)
		{
			g_Torch[nCntType].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		else if (g_Torch[nCntType].nType == 1)
		{
			g_Torch[nCntType].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
		}
		else if (g_Torch[nCntType].nType == 2)
		{
			g_Torch[nCntType].rot = D3DXVECTOR3(0.0f, 1.575f, 0.0f);
		}
		else if (g_Torch[nCntType].nType == 3)
		{
			g_Torch[nCntType].rot = D3DXVECTOR3(0.0f, -1.575f, 0.0f);
		}

		g_Torch[nCntType].fHight = 0.0f;
		g_Torch[nCntType].fWidth = 0.0f;
		g_Torch[nCntType].r = 1.0f;
		g_Torch[nCntType].b = 1.0f;
		g_Torch[nCntType].g = 1.0f;
		g_Torch[nCntType].a = 1.0f;
		g_Torch[nCntType].bUse = true;

		g_Torch[nCntType].nIdxTorchLight = SetTorch();
	}
	for (int nCntType = 0; nCntType < NUM_TORCH_M; nCntType++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX("data\\MODEL\\torch.x",/*読み込むファイル名*/
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatOrnamentModel_T[nCntType],
			NULL,
			&g_dwNumMatOrnamentModel_T[nCntType],
			&g_pMeshOrnamentModel_T[nCntType]);

		//マテリアル情報に対するポインタを所得
		pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentModel_T[nCntType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentModel_T[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する

			 //このファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_pTextureOrnamentModel_T[nCntType][nCntMat]);
			}
		}
	}
}
//****************************************
// モデルの終了処理
//****************************************
void UninitTorch_Model(void)
{
	for (int nCntType = 0; nCntType < NUM_TORCH_M; nCntType++)
	{
		//メッシュの破棄
		if (g_pMeshOrnamentModel_T[nCntType] != NULL)
		{
			g_pMeshOrnamentModel_T[nCntType]->Release();
			g_pMeshOrnamentModel_T[nCntType] = NULL;
		}
		//マテリアルの破棄
		if (g_pBuffMatOrnamentModel_T[nCntType] != NULL)
		{
			g_pBuffMatOrnamentModel_T[nCntType]->Release();
			g_pBuffMatOrnamentModel_T[nCntType] = NULL;
		}
	}
}
//****************************************
// モデルの更新処理
//****************************************
void UpdateTorch_Model(void)
{
	for (int nCntModel = 0; nCntModel < NUM_TORCH_M; nCntModel++)
	{
		SetPositionTorch(nCntModel, D3DXVECTOR3(g_Torch[nCntModel].pos.x + g_Torch[nCntModel].fWidth, g_Torch[nCntModel].pos.y, g_Torch[nCntModel].pos.z + g_Torch[nCntModel].fHight), 150);
	}
}
//****************************************
// モデルの描画処理
//****************************************
void DrawTorch_Model(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの所得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	Torch_Model *pTorch = g_Torch;

	for (int nCntModel = 0; nCntModel < NUM_TORCH_M; nCntModel++, pTorch++)
	{
		if (g_Torch[nCntModel].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWoridFigureModel_T[nCntModel]);
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pTorch->rot.y, pTorch->rot.x, pTorch->rot.z);

			D3DXMatrixMultiply(&g_mtxWoridFigureModel_T[nCntModel], &g_mtxWoridFigureModel_T[nCntModel], &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pTorch->pos.x, pTorch->pos.y, pTorch->pos.z);

			D3DXMatrixMultiply(&g_mtxWoridFigureModel_T[nCntModel], &g_mtxWoridFigureModel_T[nCntModel], &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWoridFigureModel_T[nCntModel]);

			//現在のマテリアルを所得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを所得
			pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentModel_T[nCntModel]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentModel_T[pTorch->nType]; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Diffuse.r = g_Torch[nCntModel].r;
				pMat[nCntMat].MatD3D.Diffuse.g = g_Torch[nCntModel].g;
				pMat[nCntMat].MatD3D.Diffuse.b = g_Torch[nCntModel].b;
				pMat[nCntMat].MatD3D.Diffuse.a = g_Torch[nCntModel].a;

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureOrnamentModel_T[pTorch->nType][nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshOrnamentModel_T[pTorch->nType]->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}

}
//****************************************
// モデルの読み込み処理
//****************************************
void LoadTorch_Model(void)
{
	int c = 0;	   //1文字ずつ確認する変数
	int column = 1;//列数を数える変数
	int row = 0;//行数を数える変数

	char aData[500];//つなげる文字数
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data\\csv\\TorchData.csv", "r");

	//ファイルから１文字ずつ読み込む
	while (fgetc(pFile) != '\n');

	while (c != EOF)
	{
		//１セル分の文字列を作る
		while (1)
		{
			c = fgetc(pFile);

			//末尾ならループを抜ける
			if (c == EOF)
				break;

			//カンマか改行でなければ、文字としてつなげる
			if (c != ',' && c != '\n')
				strcat(aData, (const char*)&c);

			//カンマが改行ならループ抜ける
			else
				break;
		}

		switch (column)
		{
			//atoi関数で数値として代入
		case 1:		g_Torch[row].nType = atoi(aData);			break;	//　列目：画像種類
		case 2:		g_Torch[row].pos.x = (float)(atoi(aData));	break;	//　列目：X座標
		case 3:		g_Torch[row].pos.y = (float)(atoi(aData));	break;	//　列目：Y座標
		case 4:		g_Torch[row].pos.z = (float)(atoi(aData));	break;	//　列目：Z座標
		}
		//バッファを初期化
		memset(aData, 0, sizeof(aData));

		//列数を足す
		column++;

		//もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
		if (c == '\n')
		{
			column = 1;
			row++;
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}
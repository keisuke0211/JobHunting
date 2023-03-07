//****************************************
//
//	石柱の処理
//	Author:大殿慶輔
//
//****************************************
#include "Player_model.h"
#include "stone_pillar.h"
#include "particle.h"
//****************************************
// グローバル変数
//****************************************
LPD3DXMESH g_pMeshOrnamentModel_P[NUM_PILLAR] = {};							//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatOrnamentModel_P[NUM_PILLAR] = {};					//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureOrnamentModel_P[NUM_PILLAR][MAX_TEXTURE] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatOrnamentModel_P[NUM_PILLAR] = {};							//マテリアルの数
D3DXMATRIX  g_mtxWoridFigureModel_P[NUM_PILLAR];							//ワールドマトリックス

Pillar g_Pillar[NUM_PILLAR];
//****************************************
// モデルの初期化処理
//****************************************
void InitPillar_Model(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pMat;							//マテリアルへのポインタ

	for (int nCntType = 0; nCntType < NUM_PILLAR; nCntType++)
	{
		g_Pillar[nCntType].pos = D3DXVECTOR3(g_Pillar[nCntType].pos.x, g_Pillar[nCntType].pos.y, g_Pillar[nCntType].pos.z);

		if (g_Pillar[nCntType].nType == 0)
		{
			g_Pillar[nCntType].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		else if (g_Pillar[nCntType].nType == 1)
		{
			g_Pillar[nCntType].rot = D3DXVECTOR3(0.0f, 0.0f, 1.575f);
		}
		else if (g_Pillar[nCntType].nType == 2)
		{
			g_Pillar[nCntType].rot = D3DXVECTOR3(0.0f, 1.575f, 1.575f);
		}
		g_Pillar[nCntType].fHight = 15.0f;
		g_Pillar[nCntType].fWidth = 15.0f;
		g_Pillar[nCntType].r = 1.0f;
		g_Pillar[nCntType].b = 1.0f;
		g_Pillar[nCntType].g = 1.0f;
		g_Pillar[nCntType].a = 1.0f;
		g_Pillar[nCntType].bUse = true;
	}
	for (int nCntType = 0; nCntType < NUM_PILLAR; nCntType++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX("data\\MODEL\\stone_pillar.x",/*読み込むファイル名*/
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatOrnamentModel_P[nCntType],
			NULL,
			&g_dwNumMatOrnamentModel_P[nCntType],
			&g_pMeshOrnamentModel_P[nCntType]);

		//マテリアル情報に対するポインタを所得
		pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentModel_P[nCntType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentModel_P[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する

			 //このファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_pTextureOrnamentModel_P[nCntType][nCntMat]);
			}
		}
	}
}
//****************************************
// モデルの終了処理
//****************************************
void UninitPillar_Model(void)
{
	for (int nCntType = 0; nCntType < NUM_PILLAR; nCntType++)
	{
		//メッシュの破棄
		if (g_pMeshOrnamentModel_P[nCntType] != NULL)
		{
			g_pMeshOrnamentModel_P[nCntType]->Release();
			g_pMeshOrnamentModel_P[nCntType] = NULL;
		}
		//マテリアルの破棄
		if (g_pBuffMatOrnamentModel_P[nCntType] != NULL)
		{
			g_pBuffMatOrnamentModel_P[nCntType]->Release();
			g_pBuffMatOrnamentModel_P[nCntType] = NULL;
		}
	}
}
//****************************************
// モデルの更新処理
//****************************************
void UpdatePillar_Model(void)
{
	
}
//****************************************
// モデルの描画処理
//****************************************
void DrawPillar_Model(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの所得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	Pillar *pPillar = g_Pillar;

	for (int nCntModel = 0; nCntModel < NUM_PILLAR; nCntModel++, pPillar++)
	{
		if (g_Pillar[nCntModel].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWoridFigureModel_P[nCntModel]);
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pPillar->rot.y, pPillar->rot.x, pPillar->rot.z);

			D3DXMatrixMultiply(&g_mtxWoridFigureModel_P[nCntModel], &g_mtxWoridFigureModel_P[nCntModel], &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pPillar->pos.x, pPillar->pos.y, pPillar->pos.z);

			D3DXMatrixMultiply(&g_mtxWoridFigureModel_P[nCntModel], &g_mtxWoridFigureModel_P[nCntModel], &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWoridFigureModel_P[nCntModel]);

			//現在のマテリアルを所得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを所得
			pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentModel_P[nCntModel]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentModel_P[pPillar->nType]; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Diffuse.r = g_Pillar[nCntModel].r;
				pMat[nCntMat].MatD3D.Diffuse.g = g_Pillar[nCntModel].g;
				pMat[nCntMat].MatD3D.Diffuse.b = g_Pillar[nCntModel].b;
				pMat[nCntMat].MatD3D.Diffuse.a = g_Pillar[nCntModel].a;

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureOrnamentModel_P[pPillar->nType][nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshOrnamentModel_P[pPillar->nType]->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}

}
//****************************************
// モデルの読み込み処理
//****************************************
void LoadPillar_Model(void)
{
	int c = 0;	   //1文字ずつ確認する変数
	int column = 1;//列数を数える変数
	int row = 0;//行数を数える変数

	char aData[500];//つなげる文字数
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data\\csv\\PillarData.csv", "r");

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
		case 1:		g_Pillar[row].nType = atoi(aData);			break;	//　列目：画像種類
		case 2:		g_Pillar[row].pos.x = (float)(atoi(aData));	break;	//　列目：X座標
		case 3:		g_Pillar[row].pos.y = (float)(atoi(aData));	break;	//　列目：Y座標
		case 4:		g_Pillar[row].pos.z = (float)(atoi(aData));	break;	//　列目：Z座標
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
//****************************************
// 当たり判定処理
//****************************************
void CollisioPillar_Model(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	for (int nCntFigure = 0; nCntFigure < NUM_PILLAR; nCntFigure++)
	{
		if (g_Pillar[nCntFigure].bUse == true)
		{
			if (g_Pillar[nCntFigure].nType == 0)
			{
				if (pPos->x >= g_Pillar[nCntFigure].pos.x - (g_Pillar[nCntFigure].fWidth + SIZE_WIDTH) &&
					pPos->x <= g_Pillar[nCntFigure].pos.x + (g_Pillar[nCntFigure].fWidth + SIZE_WIDTH) &&
					pPos->z >= g_Pillar[nCntFigure].pos.z - (g_Pillar[nCntFigure].fWidth + SIZE_WIDTH) &&
					pPos->z <= g_Pillar[nCntFigure].pos.z + (g_Pillar[nCntFigure].fWidth + SIZE_WIDTH))
				{
					if (g_Pillar[nCntFigure].pos.z - g_Pillar[nCntFigure].fWidth >= pPosOld->z + SIZE_HEIGHT &&
						g_Pillar[nCntFigure].pos.z - g_Pillar[nCntFigure].fWidth <= pPos->z + SIZE_HEIGHT)
					{
						pPos->z = g_Pillar[nCntFigure].pos.z - (g_Pillar[nCntFigure].fWidth + SIZE_HEIGHT);
					}
					else if (g_Pillar[nCntFigure].pos.z + g_Pillar[nCntFigure].fWidth <= pPosOld->z - SIZE_HEIGHT &&
						g_Pillar[nCntFigure].pos.z + g_Pillar[nCntFigure].fWidth >= pPos->z - SIZE_HEIGHT)
					{
						pPos->z = g_Pillar[nCntFigure].pos.z + (g_Pillar[nCntFigure].fWidth + SIZE_HEIGHT);
					}

					else if (g_Pillar[nCntFigure].pos.x - g_Pillar[nCntFigure].fWidth >= pPosOld->x + SIZE_WIDTH &&
						g_Pillar[nCntFigure].pos.x - g_Pillar[nCntFigure].fWidth <= pPos->x + SIZE_WIDTH)
					{
						pPos->x = g_Pillar[nCntFigure].pos.x - (g_Pillar[nCntFigure].fWidth + SIZE_WIDTH);
					}
					else if (g_Pillar[nCntFigure].pos.x + g_Pillar[nCntFigure].fWidth <= pPosOld->x - SIZE_WIDTH &&
						g_Pillar[nCntFigure].pos.x + g_Pillar[nCntFigure].fWidth >= pPos->x - SIZE_WIDTH)
					{
						pPos->x = g_Pillar[nCntFigure].pos.x + (g_Pillar[nCntFigure].fWidth + SIZE_WIDTH);
					}

					if (g_Pillar[nCntFigure].nType == 5)
					{
						g_Pillar[nCntFigure].bUse = false;
						SetParticle(g_Pillar[nCntFigure].pos, 0, 10, 5.0f, 2, 0);
					}
				}
			}
		}
	}
}
//****************************************
// 情報の取得
//****************************************
Pillar *GetPillarl(void)
{
	return &g_Pillar[0];
}
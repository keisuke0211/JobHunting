//****************************************
//
//	モデル（置物）の処理
//	Author:大殿慶輔
//
//****************************************
#include "jewel.h"
#include "Player_model.h"
#include "camera.h"
#include "particle.h"
#include "score.h"
#include "sound.h"
//****************************************
// グローバル変数
//****************************************
LPD3DXMESH g_pMeshOrnamentModel_Jewel[NUM_JEWEL] = {};					//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatOrnamentModel_Jewel[NUM_JEWEL] = {};				//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureOrnamentModel_Jewel[NUM_JEWEL][1] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatOrnamentModel_Jewel[NUM_JEWEL] = {};					//マテリアルの数
D3DXMATRIX  g_mtxWoridFigureModel_Jewel[NUM_JEWEL];						//ワールドマトリックス

Jewel g_jewel[NUM_JEWEL];
//****************************************
// モデルの初期化処理
//****************************************
void InitJewel(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pMat;							//マテリアルへのポインタ

	for (int nCntType = 0; nCntType < NUM_JEWEL; nCntType++)
	{
		g_jewel[nCntType].pos = D3DXVECTOR3(g_jewel[nCntType].pos.x, g_jewel[nCntType].pos.y, g_jewel[nCntType].pos.z);
		g_jewel[nCntType].rot = DEFAULT_D3DXVECTOR3;

		g_jewel[nCntType].r = 0.0f;
		g_jewel[nCntType].b = 0.8f;
		g_jewel[nCntType].g = 0.0f;
		g_jewel[nCntType].a = 1.0f;

		g_jewel[nCntType].fHight = 15.0f;
		g_jewel[nCntType].fWidth = 15.0f;

		g_jewel[nCntType].bUse = true;
	}

	for (int nCntType = 0; nCntType < NUM_JEWEL; nCntType++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX("data\\MODEL\\Diamondo.x",/*読み込むファイル名*/
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatOrnamentModel_Jewel[nCntType],
			NULL,
			&g_dwNumMatOrnamentModel_Jewel[nCntType],
			&g_pMeshOrnamentModel_Jewel[nCntType]);

		//マテリアル情報に対するポインタを所得
		pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentModel_Jewel[nCntType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentModel_Jewel[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する

			 //このファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_pTextureOrnamentModel_Jewel[nCntType][nCntMat]);
			}
		}
	}
}
//****************************************
// モデルの終了処理
//****************************************
void UninitJewel(void)
{
	//サウンドの停止
	StopSound();

	for (int nCntType = 0; nCntType < NUM_JEWEL; nCntType++)
	{
		//メッシュの破棄
		if (g_pMeshOrnamentModel_Jewel[nCntType] != NULL)
		{
			g_pMeshOrnamentModel_Jewel[nCntType]->Release();
			g_pMeshOrnamentModel_Jewel[nCntType] = NULL;
		}
		//マテリアルの破棄
		if (g_pBuffMatOrnamentModel_Jewel[nCntType] != NULL)
		{
			g_pBuffMatOrnamentModel_Jewel[nCntType]->Release();
			g_pBuffMatOrnamentModel_Jewel[nCntType] = NULL;
		}
	}
}
//****************************************
// モデルの更新処理
//****************************************
void UpdateJewel(void)
{
	for (int nCntModel = 0; nCntModel < NUM_JEWEL; nCntModel++)
	{
		if (g_jewel[nCntModel].bUse == true)
		{
			g_jewel[nCntModel].rot.y += 0.025f;
		}
	}
}
//****************************************
// モデルの描画処理
//****************************************
void DrawJewel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの所得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	Jewel *pFigure = g_jewel;

	for (int nCntModel = 0; nCntModel < NUM_JEWEL; nCntModel++, pFigure++)
	{
		if (g_jewel[nCntModel].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWoridFigureModel_Jewel[nCntModel]);
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pFigure->rot.y, pFigure->rot.x, pFigure->rot.z);

			D3DXMatrixMultiply(&g_mtxWoridFigureModel_Jewel[nCntModel], &g_mtxWoridFigureModel_Jewel[nCntModel], &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pFigure->pos.x, pFigure->pos.y, pFigure->pos.z);

			D3DXMatrixMultiply(&g_mtxWoridFigureModel_Jewel[nCntModel], &g_mtxWoridFigureModel_Jewel[nCntModel], &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWoridFigureModel_Jewel[nCntModel]);

			//現在のマテリアルを所得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを所得
			pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentModel_Jewel[nCntModel]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentModel_Jewel[pFigure->nType]; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Diffuse.r = g_jewel[nCntModel].r;
				pMat[nCntMat].MatD3D.Diffuse.g = g_jewel[nCntModel].g;
				pMat[nCntMat].MatD3D.Diffuse.b = g_jewel[nCntModel].b;
				pMat[nCntMat].MatD3D.Diffuse.a = g_jewel[nCntModel].a;

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureOrnamentModel_Jewel[pFigure->nType][nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshOrnamentModel_Jewel[pFigure->nType]->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}

}
//****************************************
// モデルの読み込み処理
//****************************************
void LoadJewel(void)
{
	int c = 0;	   //1文字ずつ確認する変数
	int column = 1;//列数を数える変数
	int row = 0;//行数を数える変数

	char aData[500];//つなげる文字数
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data\\csv\\JewelData.csv", "r");

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
		case 1:		g_jewel[row].nType = atoi(aData);			break;	//　列目：画像種類
		case 2:		g_jewel[row].pos.x = (float)(atoi(aData));	break;	//　列目：X座標
		case 3:		g_jewel[row].pos.y = (float)(atoi(aData));	break;	//　列目：Y座標
		case 4:		g_jewel[row].pos.z = (float)(atoi(aData));	break;	//　列目：Z座標
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
// 当たり判定
//****************************************

//プレイヤー
void CollisionJewel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	for (int nCntFigure = 0; nCntFigure < NUM_JEWEL; nCntFigure++)
	{
		if (g_jewel[nCntFigure].bUse == true)
		{
			if (pPos->x >= g_jewel[nCntFigure].pos.x - (g_jewel[nCntFigure].fWidth + SIZE_WIDTH) &&
				pPos->x <= g_jewel[nCntFigure].pos.x + (g_jewel[nCntFigure].fWidth + SIZE_WIDTH) &&
				pPos->z >= g_jewel[nCntFigure].pos.z - (g_jewel[nCntFigure].fHight + SIZE_HEIGHT) &&
				pPos->z <= g_jewel[nCntFigure].pos.z + (g_jewel[nCntFigure].fHight + SIZE_HEIGHT))
			{
				if (g_jewel[nCntFigure].pos.z - g_jewel[nCntFigure].fHight >= pPosOld->z + SIZE_HEIGHT &&
					g_jewel[nCntFigure].pos.z - g_jewel[nCntFigure].fHight <= pPos->z + SIZE_HEIGHT)
				{
					g_jewel[nCntFigure].bUse = false;
					SetParticle(g_jewel[nCntFigure].pos, 0, 10, 5.0f, 1, 2);
					PlaySound(SOUND_LABEL_JEWEL); //獲得音
					AddScore(50);
				}
				else if (g_jewel[nCntFigure].pos.z + g_jewel[nCntFigure].fHight <= pPosOld->z - SIZE_HEIGHT &&
					g_jewel[nCntFigure].pos.z + g_jewel[nCntFigure].fHight >= pPos->z - SIZE_HEIGHT)
				{
					g_jewel[nCntFigure].bUse = false;
					SetParticle(g_jewel[nCntFigure].pos, 0, 10, 5.0f, 1, 2);
					PlaySound(SOUND_LABEL_JEWEL); //獲得音
					AddScore(50);
				}

				else if (g_jewel[nCntFigure].pos.x - g_jewel[nCntFigure].fWidth >= pPosOld->x + SIZE_WIDTH &&
					g_jewel[nCntFigure].pos.x - g_jewel[nCntFigure].fWidth <= pPos->x + SIZE_WIDTH)
				{
					g_jewel[nCntFigure].bUse = false;
					SetParticle(g_jewel[nCntFigure].pos, 0, 10, 5.0f, 1, 2);
					PlaySound(SOUND_LABEL_JEWEL); //獲得音
					AddScore(50);
				}
				else if (g_jewel[nCntFigure].pos.x + g_jewel[nCntFigure].fWidth <= pPosOld->x - SIZE_WIDTH &&
					g_jewel[nCntFigure].pos.x + g_jewel[nCntFigure].fWidth >= pPos->x - SIZE_WIDTH)
				{
					g_jewel[nCntFigure].bUse = false;
					SetParticle(g_jewel[nCntFigure].pos, 0, 10, 5.0f, 1, 2);
					PlaySound(SOUND_LABEL_JEWEL); //獲得音
					AddScore(50);
				}
			}
		}
	}
}
//情報の取得
Jewel *GetJewel(void)
{
	return &g_jewel[0];
}

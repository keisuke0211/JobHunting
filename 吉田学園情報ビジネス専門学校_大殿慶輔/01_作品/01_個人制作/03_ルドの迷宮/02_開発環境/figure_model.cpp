//****************************************
//
//	モデル（置物）の処理
//	Author:大殿慶輔
//		Getcamara()->posV.x = Getcamara()->posR.x + sinf(D3DX_PI * (1.0f - Getcamara()->rot.z)) * Getcamara()->length;
//****************************************
#include "figure_model.h"
#include "Player_model.h"
#include "bullet.h"
#include "camera.h"
#include "particle.h"
#include "game.h"
//****************************************
// グローバル変数
//****************************************
LPD3DXMESH g_pMeshOrnamentModel[NUM_FIGURE_MODEL] = {};							//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatOrnamentModel[NUM_FIGURE_MODEL] = {};						//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureOrnamentModel[NUM_FIGURE_MODEL][MAX_TEXTURE] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatOrnamentModel[NUM_FIGURE_MODEL] = {};								//マテリアルの数
D3DXMATRIX  g_mtxWoridFigureModel[NUM_FIGURE_MODEL];								//ワールドマトリックス

Figure g_figure[NUM_FIGURE_MODEL];
int g_nCnt = 0;
bool bGoll;
//****************************************
// モデルの初期化処理
//****************************************
void InitFigureModel(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pMat;							//マテリアルへのポインタ

	bGoll = false;
	for (int nCntType = 0; nCntType < NUM_FIGURE_MODEL; nCntType++)
	{
		g_figure[nCntType].pos = D3DXVECTOR3(g_figure[nCntType].pos.x, g_figure[nCntType].pos.y, g_figure[nCntType].pos.z);
		g_figure[nCntType].rot = D3DXVECTOR3(g_figure[nCntType].rot.x, g_figure[nCntType].rot.y, g_figure[nCntType].rot.z);

		g_figure[nCntType].r = 1.0f;
		g_figure[nCntType].b = 1.0f;
		g_figure[nCntType].g = 1.0f;
		g_figure[nCntType].a = 1.0f;

		g_figure[nCntType].nCntDamage = 0;
		g_figure[nCntType].bUse = true;
		g_figure[nCntType].bDraw = true;

		if (g_figure[nCntType].nType == 3)
		{
			g_figure[nCntType].fHight = 5.0f;
			g_figure[nCntType].fWidth = 10.0f;
			g_figure[nCntType].nLife = 50;
		}
		else if (g_figure[nCntType].nType == 4)
		{
			g_figure[nCntType].r = 1.0f;
			g_figure[nCntType].g = 1.0f;
			g_figure[nCntType].b = 1.0f;
			g_figure[nCntType].a = 1.0f;
			g_figure[nCntType].nLife = 50;
		}
		else if (g_figure[nCntType].nType == 6)
		{
			g_figure[nCntType].fHight = 50.0f;
			g_figure[nCntType].fWidth = 50.0f;

			g_figure[nCntType].bDraw = false;
		}
		else
		{
			g_figure[nCntType].fHight = 15.0f;
			g_figure[nCntType].fWidth = 15.0f;
			g_figure[nCntType].nLife = 3;
		}
	}

	for (int nCntType = 0; nCntType < NUM_FIGURE_MODEL; nCntType++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(g_figure[nCntType].aModelPath,/*読み込むファイル名*/
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatOrnamentModel[nCntType],
			NULL,
			&g_dwNumMatOrnamentModel[nCntType],
			&g_pMeshOrnamentModel[nCntType]);

		//マテリアル情報に対するポインタを所得
		pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentModel[nCntType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentModel[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する

			 //このファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_pTextureOrnamentModel[nCntType][nCntMat]);
			}
		}
	}
}
//****************************************
// モデルの終了処理
//****************************************
void UninitFigureModel(void)
{
	for (int nCntType = 0; nCntType < NUM_FIGURE_MODEL; nCntType++)
	{
		//メッシュの破棄
		if (g_pMeshOrnamentModel[nCntType] != NULL)
		{
			g_pMeshOrnamentModel[nCntType]->Release();
			g_pMeshOrnamentModel[nCntType] = NULL;
		}
		//マテリアルの破棄
		if (g_pBuffMatOrnamentModel[nCntType] != NULL)
		{
			g_pBuffMatOrnamentModel[nCntType]->Release();
			g_pBuffMatOrnamentModel[nCntType] = NULL;
		}
	}
}
//****************************************
// モデルの更新処理
//****************************************
void UpdateFigureModel(void)
{
	for (int nCntModel = 0; nCntModel < NUM_FIGURE_MODEL; nCntModel++)
	{
		if (g_figure[nCntModel].bUse == true)
		{
			if (g_figure[nCntModel].nCntDamage <= 0)
			{
				g_figure[nCntModel].r = 1.0f;
				g_figure[nCntModel].b = 1.0f;
				g_figure[nCntModel].g = 1.0f;
				g_figure[nCntModel].a = 1.0f;
			}
			else
			{
				g_figure[nCntModel].nCntDamage -= 1;
			}

			if (g_figure[nCntModel].nType == 5)
			{
				g_figure[nCntModel].rot.y += 0.025f;
			}
			else if (g_figure[nCntModel].nType == 6)
			{
				g_nCnt++;

				if (g_nCnt >= 50)
				{
					SetParticle(g_figure[nCntModel].pos, 3, 10, 60.0f, 0, 1);
					g_nCnt = 0;
				}
			}
		}
	}
}
//****************************************
// モデルの描画処理
//****************************************
void DrawFigureModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの所得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	Figure *pFigure = g_figure;

	for (int nCntModel = 0; nCntModel < NUM_FIGURE_MODEL; nCntModel++, pFigure++)
	{
		if (g_figure[nCntModel].bDraw == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWoridFigureModel[nCntModel]);
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pFigure->rot.y, pFigure->rot.x, pFigure->rot.z);

			D3DXMatrixMultiply(&g_mtxWoridFigureModel[nCntModel], &g_mtxWoridFigureModel[nCntModel], &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pFigure->pos.x, pFigure->pos.y, pFigure->pos.z);

			D3DXMatrixMultiply(&g_mtxWoridFigureModel[nCntModel], &g_mtxWoridFigureModel[nCntModel], &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWoridFigureModel[nCntModel]);

			//現在のマテリアルを所得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを所得
			pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentModel[nCntModel]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentModel[pFigure->nType]; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Diffuse.r = g_figure[nCntModel].r;
				pMat[nCntMat].MatD3D.Diffuse.g = g_figure[nCntModel].g;
				pMat[nCntMat].MatD3D.Diffuse.b = g_figure[nCntModel].b;
				pMat[nCntMat].MatD3D.Diffuse.a = g_figure[nCntModel].a;
				
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureOrnamentModel[pFigure->nType][nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshOrnamentModel[pFigure->nType]->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
	
}
//****************************************
// モデルの読み込み処理
//****************************************
void LoadFigureModel(void)
{
	FILE *pFile;				// ファイルポインタ
	Figure *pFigure = g_figure;
	char aDataSearch[TXT_MAX];	// データ検索用

	// 種類毎の情報のデータファイルを開く
	pFile = fopen("data\\txt\\FigureData.txt", "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		// 検索
		fscanf(pFile, "%s", aDataSearch);									

		//ファイルを閉じる	
		if (strcmp(aDataSearch, "END") == 0) { fclose(pFile); break; }

		// 折り返す
		if (aDataSearch[0] == '#') { continue; }

		if (strcmp(aDataSearch, "TYPE") == 0) {
			// CHR:敵[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (strcmp(aDataSearch, "TYPE_END") == 0) { pFigure++; break; }										// 読み込みを終了
				else if (strcmp(aDataSearch, "MODEL_PATH") == 0) { fscanf(pFile, "%s", &pFigure->aModelPath); }		// モデルの相対パス
				else if (strcmp(aDataSearch, "FIGURE_TYPE") == 0) { fscanf(pFile, "%d", &pFigure->nType); }			// 敵の種類
				else if (strcmp(aDataSearch, "POS") == 0) {	// 位置
					fscanf(pFile, "%f", &pFigure->pos.x);	// X
					fscanf(pFile, "%f", &pFigure->pos.y);	// Y
					fscanf(pFile, "%f", &pFigure->pos.z);	// Z
				}
				else if (strcmp(aDataSearch, "ROT") == 0){	// 向き
					fscanf(pFile, "%f", &pFigure->rot.x);	// X
					fscanf(pFile, "%f", &pFigure->rot.y);	// Y
					fscanf(pFile, "%f", &pFigure->rot.z);	// Z
				}
			}
		}
	}
}
//****************************************
// HIT判定
//****************************************
void HitFigure(int nCntFigure, int nDamage)
{
	if (g_figure[nCntFigure].bUse == true)
	{
		g_figure[nCntFigure].nLife -= nDamage;

		if (g_figure[nCntFigure].nLife <= 0)//敵の体力がなくなった
		{
			if (g_figure[nCntFigure].nType == 6)
			{

			}
			else
			{
				g_figure[nCntFigure].bUse = false;
				g_figure[nCntFigure].bDraw = false;
				SetParticle(g_figure[nCntFigure].pos, 0, 10, 30.0f, 1, 0);
			}
		}
		else
		{
			for (int nCntModel = nCntFigure; nCntModel < (nCntFigure+1); nCntModel++)
			{

				g_figure[nCntFigure].r = 0.5f;
				g_figure[nCntFigure].g = 0.0f;
				g_figure[nCntFigure].b = 0.0f;
				g_figure[nCntFigure].a = 1.0f;

				g_figure[nCntFigure].nCntDamage = DAMEGE_COUNT;
			}
		}
	}
	SetParticle(g_figure[nCntFigure].pos, 0, 10, 5.0f, 1, 0);
}
//****************************************
// 当たり判定
//****************************************

//プレイヤー
void CollisionFigurePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	for (int nCntFigure = 0; nCntFigure < NUM_FIGURE_MODEL; nCntFigure++)
	{
		if (g_figure[nCntFigure].bUse == true)
		{
			if (pPos->x >= g_figure[nCntFigure].pos.x - (g_figure[nCntFigure].fWidth + SIZE_WIDTH) &&
				pPos->x <= g_figure[nCntFigure].pos.x + (g_figure[nCntFigure].fWidth + SIZE_WIDTH) &&
				pPos->z >= g_figure[nCntFigure].pos.z - (g_figure[nCntFigure].fHight + SIZE_HEIGHT) &&
				pPos->z <= g_figure[nCntFigure].pos.z + (g_figure[nCntFigure].fHight + SIZE_HEIGHT))
			{
				if (g_figure[nCntFigure].nType == 6)
				{
					if (bGoll == false)
					{
						SetGameState(GAMESTATE_END2, 3);
						bGoll = true;
					}
				}
				else if (g_figure[nCntFigure].pos.z - g_figure[nCntFigure].fHight >= pPosOld->z + SIZE_HEIGHT &&
					g_figure[nCntFigure].pos.z - g_figure[nCntFigure].fHight <= pPos->z + SIZE_HEIGHT)
				{
					pPos->z = g_figure[nCntFigure].pos.z - (g_figure[nCntFigure].fHight + SIZE_HEIGHT);
				}
				else if (g_figure[nCntFigure].pos.z + g_figure[nCntFigure].fHight <= pPosOld->z - SIZE_HEIGHT &&
					g_figure[nCntFigure].pos.z + g_figure[nCntFigure].fHight >= pPos->z - SIZE_HEIGHT)
				{
					pPos->z = g_figure[nCntFigure].pos.z + (g_figure[nCntFigure].fHight + SIZE_HEIGHT);
				}

				else if (g_figure[nCntFigure].pos.x - g_figure[nCntFigure].fWidth >= pPosOld->x + SIZE_WIDTH &&
					g_figure[nCntFigure].pos.x - g_figure[nCntFigure].fWidth <= pPos->x + SIZE_WIDTH)
				{
					pPos->x = g_figure[nCntFigure].pos.x - (g_figure[nCntFigure].fWidth + SIZE_WIDTH);
				}
				else if (g_figure[nCntFigure].pos.x + g_figure[nCntFigure].fWidth <= pPosOld->x - SIZE_WIDTH &&
					g_figure[nCntFigure].pos.x + g_figure[nCntFigure].fWidth >= pPos->x - SIZE_WIDTH)
				{
					pPos->x = g_figure[nCntFigure].pos.x + (g_figure[nCntFigure].fWidth + SIZE_WIDTH);
				}

				if (g_figure[nCntFigure].nType == 5)
				{
					g_figure[nCntFigure].bUse = false;
					SetParticle(g_figure[nCntFigure].pos, 0, 10, 5.0f, 2, 0);
				}
			}
		}
	}
}
//情報の取得
Figure *GetFigure(void)
{
	return &g_figure[0];
}

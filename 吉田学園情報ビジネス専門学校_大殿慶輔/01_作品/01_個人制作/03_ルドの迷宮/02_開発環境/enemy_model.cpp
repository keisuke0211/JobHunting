//****************************************
//
//	モデル（敵）の処理
//	Author:大殿慶輔
//
//****************************************
#include "enemy_model.h"
#include "Player_model.h"
#include "bullet.h"
#include "camera.h"
#include "particle.h"
#include "wall.h"
#include "score.h"
#include "sound.h"
//****************************************
// グローバル変数
//****************************************
LPD3DXMESH g_pMeshOrnamentEnemyModel[NUM_MODEL] = {};							//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatOrnamentEnemyModel[NUM_MODEL] = {};						//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureOrnamentEnemyModel[NUM_MODEL][MAX_TEXTURE] = {};	//テクスチャへのポインタ
DWORD g_dwNumMatOrnamentEnemyModel[NUM_MODEL] = {};								//マテリアルの数
D3DXMATRIX  g_mtxWoridFigureEnemyModel[NUM_MODEL];								//ワールドマトリックス

Enemy g_enemy[NUM_MODEL];
//****************************************
// モデルの初期化処理
//****************************************
void InitEnemyeModel(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL *pMat;							//マテリアルへのポインタ

	for (int nCntType = 0; nCntType < NUM_MODEL; nCntType++)
	{
		g_enemy[nCntType].pos = D3DXVECTOR3(g_enemy[nCntType].pos.x, g_enemy[nCntType].pos.y, g_enemy[nCntType].pos.z);
		g_enemy[nCntType].posOld = D3DXVECTOR3(g_enemy[nCntType].pos.x, g_enemy[nCntType].pos.y, g_enemy[nCntType].pos.z);
		g_enemy[nCntType].Initpos = D3DXVECTOR3(g_enemy[nCntType].pos.x, g_enemy[nCntType].pos.y, g_enemy[nCntType].pos.z);

		if (g_enemy[nCntType].nType_M == 0)
		{
			g_enemy[nCntType].rot = D3DXVECTOR3(DEFAULT_FLOA,DEFAULT_FLOA,DEFAULT_FLOA);
		}
		else if (g_enemy[nCntType].nType_M == 1)
		{
			g_enemy[nCntType].rot = D3DXVECTOR3(DEFAULT_FLOA,3.24f,DEFAULT_FLOA);
		}
		else if (g_enemy[nCntType].nType_M == 2)
		{
			g_enemy[nCntType].rot = D3DXVECTOR3(DEFAULT_FLOA, 1.57f, DEFAULT_FLOA);
		}
		else if (g_enemy[nCntType].nType_M == 3)
		{
			g_enemy[nCntType].rot = D3DXVECTOR3(DEFAULT_FLOA, -1.57f, DEFAULT_FLOA);
		}

		if (g_enemy[nCntType].nType == 0)
		{
			g_enemy[nCntType].nSpeed = 4;
			g_enemy[nCntType].nLife = 3;
			g_enemy[nCntType].fWidth = 15.0f;
			g_enemy[nCntType].fHight = 15.0f;
			g_enemy[nCntType].nScore = 60;
		}
		else if (g_enemy[nCntType].nType == 1)
		{
			g_enemy[nCntType].nSpeed = 6;
			g_enemy[nCntType].nLife = 2;
			g_enemy[nCntType].fWidth = 15.0f;
			g_enemy[nCntType].fHight = 15.0f;
			g_enemy[nCntType].nScore = 80;
		}
		else if (g_enemy[nCntType].nType == 2)
		{
			g_enemy[nCntType].nSpeed = 6;
			g_enemy[nCntType].nLife = 2;
			g_enemy[nCntType].fWidth = 15.0f;
			g_enemy[nCntType].fHight = 15.0f;
			g_enemy[nCntType].nScore = 80;
		}
		else
		{
			g_enemy[nCntType].nSpeed = 2;
			g_enemy[nCntType].nLife = 10;
			g_enemy[nCntType].fWidth = 50.0f;
			g_enemy[nCntType].fHight = 65.0f;
			g_enemy[nCntType].nScore = 100;
		}

		g_enemy[nCntType].r = 1.0f;
		g_enemy[nCntType].b = 1.0f;
		g_enemy[nCntType].g = 1.0f;
		g_enemy[nCntType].a = 1.0f;

		g_enemy[nCntType].nCntDamage = 0;
		g_enemy[nCntType].nCntSummon = 0;

		g_enemy[nCntType].bUse = true;
		g_enemy[nCntType].bDraw = true;
		g_enemy[nCntType].bReturn = false;
	}
	for (int nCntType = 0; nCntType < NUM_MODEL; nCntType++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(g_enemy[nCntType].aModelPath,/*読み込むファイル名*/
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatOrnamentEnemyModel[nCntType],
			NULL,
			&g_dwNumMatOrnamentEnemyModel[nCntType],
			&g_pMeshOrnamentEnemyModel[nCntType]);

		//マテリアル情報に対するポインタを所得
		pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentEnemyModel[nCntType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentEnemyModel[nCntType]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在する

			 //このファイル名を使用してテクスチャを読み込む
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_pTextureOrnamentEnemyModel[nCntType][nCntMat]);
			}
		}
	}
}
//****************************************
// モデルの終了処理
//****************************************
void UninitEnemyeModel(void)
{
	for (int nCntType = 0; nCntType < NUM_MODEL; nCntType++)
	{
		//メッシュの破棄
		if (g_pMeshOrnamentEnemyModel[nCntType] != NULL)
		{
			g_pMeshOrnamentEnemyModel[nCntType]->Release();
			g_pMeshOrnamentEnemyModel[nCntType] = NULL;
		}
		//マテリアルの破棄
		if (g_pBuffMatOrnamentEnemyModel[nCntType] != NULL)
		{
			g_pBuffMatOrnamentEnemyModel[nCntType]->Release();
			g_pBuffMatOrnamentEnemyModel[nCntType] = NULL;
		}
	}
}
//****************************************
// モデルの更新処理
//****************************************
void UpdateEnemyeModel(void)
{
	for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++)
	{
		if (g_enemy[nCntModel].bUse == true)
		{
			g_enemy[nCntModel].posOld = g_enemy[nCntModel].pos;
			if (g_enemy[nCntModel].bReturn == false)
			{
				if (g_enemy[nCntModel].nType_M == 0)
				{
					g_enemy[nCntModel].move.z = -g_enemy[nCntModel].nSpeed;
				}
				else if (g_enemy[nCntModel].nType_M == 1)
				{
					g_enemy[nCntModel].move.z = g_enemy[nCntModel].nSpeed;
				}
				else if (g_enemy[nCntModel].nType_M == 2)
				{
					g_enemy[nCntModel].move.x = -g_enemy[nCntModel].nSpeed;
				}
				else if (g_enemy[nCntModel].nType_M == 3)
				{
					g_enemy[nCntModel].move.x = g_enemy[nCntModel].nSpeed;
				}
				g_enemy[nCntModel].pos.x += g_enemy[nCntModel].move.x;
				g_enemy[nCntModel].pos.z += g_enemy[nCntModel].move.z;

				CollisionEnemy_Wall(&g_enemy[nCntModel], &g_enemy[nCntModel].pos, &g_enemy[nCntModel].posOld, &g_enemy[nCntModel].move);
			}
			else if (g_enemy[nCntModel].bReturn == true)
			{
				if (g_enemy[nCntModel].nCntSummon <= 0)
				{
					if (g_enemy[nCntModel].nType_M == 0)
					{
						g_enemy[nCntModel].rot.y = ABOVE;
					}
					else if (g_enemy[nCntModel].nType_M == 1)
					{
						g_enemy[nCntModel].rot.y = 3.24f;
					}
					else if (g_enemy[nCntModel].nType_M == 2)
					{
						g_enemy[nCntModel].rot.y = 1.57f;
					}
					else if (g_enemy[nCntModel].nType_M == 3)
					{
						g_enemy[nCntModel].rot.y = -1.57f;
					}

					g_enemy[nCntModel].bReturn = false;
				}
				else if (g_enemy[nCntModel].nCntSummon == (RETURN_COUNT - RETURN_COUNT * 0.9))
				{
					g_enemy[nCntModel].nCntSummon--;
				}
				else if (g_enemy[nCntModel].nCntSummon == RETURN_COUNT)
				{
					g_enemy[nCntModel].nCntSummon--;
				}
				else
				{
					g_enemy[nCntModel].nCntSummon--;
				}
			}

			if (g_enemy[nCntModel].nCntDamage <= 0)
			{
				g_enemy[nCntModel].r = 1.0f;
				g_enemy[nCntModel].b = 1.0f;
				g_enemy[nCntModel].g = 1.0f;
				g_enemy[nCntModel].a = 1.0f;
			}
			else
			{
				g_enemy[nCntModel].nCntDamage -= 1;
			}
		}

	}
}
//****************************************
// モデルの描画処理
//****************************************
void DrawEnemyeModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの所得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	Enemy *pEnemy = &g_enemy[0];

	for (int nCntModel = 0; nCntModel < NUM_MODEL; nCntModel++, pEnemy++)
	{
		if (g_enemy[nCntModel].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pEnemy->mtxWorld);
			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEnemy->rot.y, pEnemy->rot.x, pEnemy->rot.z);

			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z);

			D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxTrans);
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pEnemy->mtxWorld);

			//現在のマテリアルを所得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを所得
			pMat = (D3DXMATERIAL*)g_pBuffMatOrnamentEnemyModel[nCntModel]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOrnamentEnemyModel[nCntModel]; nCntMat++)
			{
				pMat[nCntMat].MatD3D.Diffuse.r = g_enemy[nCntModel].r;
				pMat[nCntMat].MatD3D.Diffuse.g = g_enemy[nCntModel].g;
				pMat[nCntMat].MatD3D.Diffuse.b = g_enemy[nCntModel].b;
				pMat[nCntMat].MatD3D.Diffuse.a = g_enemy[nCntModel].a;

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureOrnamentEnemyModel[pEnemy->nType][nCntMat]);

				//モデル(パーツ)の描画
				g_pMeshOrnamentEnemyModel[pEnemy->nType]->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}

}
//****************************************
// モデルの読み込み処理
//****************************************
void LoadEnemyModel(void)
{
	FILE *pFile;				// ファイルポインタ
	Enemy *pEnemy = g_enemy;
	char aDataSearch[TXT_MAX];	// データ検索用

								// 種類毎の情報のデータファイルを開く
	pFile = fopen("data\\txt\\EnemyData.txt", "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (strcmp(aDataSearch, "END") == 0) { fclose(pFile); break; }	//ファイルを閉じる	
		if (aDataSearch[0] == '#') { continue; }							// 折り返す

		if (strcmp(aDataSearch, "TYPE") == 0) {
			// CHR:敵[00] の種類毎の情報の読み込みを開始
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (strcmp(aDataSearch, "TYPE_END") == 0) { pEnemy++; break; }										// 読み込みを終了
				else if (strcmp(aDataSearch, "MODEL_PATH") == 0) { fscanf(pFile, "%s", &pEnemy->aModelPath); }		// モデルの相対パス
				else if (strcmp(aDataSearch, "FIGURE_TYPE") == 0) { fscanf(pFile, "%d", &pEnemy->nType); }			// 敵の種類
				else if(strcmp(aDataSearch, "MOVE_TYPE") == 0) { fscanf(pFile, "%d", &pEnemy->nType_M); }			// 敵の種類
				else if (strcmp(aDataSearch, "POS") == 0) {		// 位置
					fscanf(pFile, "%f", &pEnemy->pos.x);	// X
					fscanf(pFile, "%f", &pEnemy->pos.y);	// Y
					fscanf(pFile, "%f", &pEnemy->pos.z);	// Z
				}
				else if (strcmp(aDataSearch, "ROT") == 0) {		// 向き
					fscanf(pFile, "%f", &pEnemy->rot.x);	// X
					fscanf(pFile, "%f", &pEnemy->rot.y);	// Y
					fscanf(pFile, "%f", &pEnemy->rot.z);	// Z
				}
			}
		}
	}
}
//****************************************
// 当たり判定
//****************************************

//　壁
void CollisionEnemy_Wall(Enemy *pEnemy, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	Wall *pWall = GetWall();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
			if (pEnemy->pos.x >= pWall->pos.x - (pWall->fWidth + pEnemy->fWidth) &&
				pEnemy->pos.x <= pWall->pos.x + (pWall->fWidth + pEnemy->fWidth) &&
				pEnemy->pos.z >= pWall->pos.z - (pWall->fWidth + pEnemy->fWidth) &&
				pEnemy->pos.z <= pWall->pos.z + (pWall->fWidth + pEnemy->fWidth))
			{
				if (pWall->nType == 2)
				{
					if (pWall->nCype == 0 || pWall->nCype == 1)
					{
						if (pWall->pos.z - DEFAULT_INT >= pEnemy->posOld.z + pEnemy->fWidth &&
							pWall->pos.z - DEFAULT_INT <= pEnemy->pos.z + pEnemy->fWidth)
						{
							pEnemy->pos.z = pWall->pos.z - (DEFAULT_INT + pEnemy->fWidth);
							pEnemy->bReturn = true;
							pEnemy->nCntSummon = RETURN_COUNT;
							pEnemy->nType_M = 0;
						}
						else if (pWall->pos.z + DEFAULT_INT <= pEnemy->posOld.z - pEnemy->fWidth &&
							pWall->pos.z + DEFAULT_INT >= pEnemy->pos.z - pEnemy->fWidth)
						{
							pEnemy->pos.z = pWall->pos.z + (DEFAULT_INT + pEnemy->fWidth);
							pEnemy->bReturn = true;
							pEnemy->nCntSummon = RETURN_COUNT;
							pEnemy->nType_M = 1;
						}
					}
					if (pWall->nCype == 2 || pWall->nCype == 3)
					{
						if (pWall->pos.x - DEFAULT_INT >= pEnemy->posOld.x + pEnemy->fWidth &&
							pWall->pos.x - DEFAULT_INT <= pEnemy->pos.x + pEnemy->fWidth)
						{
							pEnemy->pos.x = pWall->pos.x - (DEFAULT_INT + pEnemy->fWidth);
							pEnemy->bReturn = true;
							pEnemy->nCntSummon = RETURN_COUNT;
							pEnemy->nType_M = 2;
						}
						else if (pWall->pos.x + DEFAULT_INT <= pEnemy->posOld.x - pEnemy->fWidth &&
							pWall->pos.x + DEFAULT_INT >= pEnemy->pos.x - pEnemy->fWidth)
						{
							pEnemy->pos.x = pWall->pos.x + (DEFAULT_INT + pEnemy->fWidth);
							pEnemy->bReturn = true;
							pEnemy->nCntSummon = RETURN_COUNT;
							pEnemy->nType_M = 3;
						}
					}
				}
				else
				{
					if (pWall->nCype == 0 || pWall->nCype == 1)
					{
						if (pWall->pos.z - DECISION >= pEnemy->posOld.z + pEnemy->fWidth &&
							pWall->pos.z - DECISION <= pEnemy->pos.z + pEnemy->fWidth)
						{
							pEnemy->pos.z = pWall->pos.z - (DECISION + pEnemy->fWidth);
							pEnemy->bReturn = true;
							pEnemy->nCntSummon = RETURN_COUNT;
							pEnemy->nType_M = 0;
						}
						else if (pWall->pos.z + DECISION <= pEnemy->posOld.z - pEnemy->fWidth &&
							pWall->pos.z + DECISION >= pEnemy->pos.z - pEnemy->fWidth)
						{
							pEnemy->pos.z = pWall->pos.z + (DECISION + pEnemy->fWidth);
							pEnemy->bReturn = true;
							pEnemy->nCntSummon = RETURN_COUNT;
							pEnemy->nType_M = 1;
						}
					}
					if (pWall->nCype == 2 || pWall->nCype == 3)
					{
						if (pWall->pos.x - DECISION >= pEnemy->posOld.x + pEnemy->fWidth &&
							pWall->pos.x - DECISION <= pEnemy->pos.x + pEnemy->fWidth)
						{
							pEnemy->pos.x = pWall->pos.x - (DECISION + pEnemy->fWidth);
							pEnemy->bReturn = true;
							pEnemy->nCntSummon = RETURN_COUNT;
							pEnemy->nType_M = 2;
						}
						else if (pWall->pos.x + DECISION <= pEnemy->posOld.x - pEnemy->fWidth &&
							pWall->pos.x + DECISION >= pEnemy->pos.x - pEnemy->fWidth)
						{
							pEnemy->pos.x = pWall->pos.x + (DECISION + pEnemy->fWidth);
							pEnemy->bReturn = true;
							pEnemy->nCntSummon = RETURN_COUNT;
							pEnemy->nType_M = 3;
						}
					}
				}
			}
		}
	}

}

// プレイヤー
void CollisionEnemy_Player(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	for (int nCntFigure = 0; nCntFigure < NUM_MODEL; nCntFigure++)
	{
		if (g_enemy[nCntFigure].bDraw == true)
		{
			if (pPos->x >= g_enemy[nCntFigure].pos.x - (g_enemy[nCntFigure].fWidth + SIZE_WIDTH) &&
				pPos->x <= g_enemy[nCntFigure].pos.x + (g_enemy[nCntFigure].fWidth + SIZE_WIDTH) &&
				pPos->z >= g_enemy[nCntFigure].pos.z - (g_enemy[nCntFigure].fHight + SIZE_HEIGHT) &&
				pPos->z <= g_enemy[nCntFigure].pos.z + (g_enemy[nCntFigure].fHight + SIZE_HEIGHT))
			{
				if (g_enemy[nCntFigure].posOld.z - g_enemy[nCntFigure].fHight >= pPosOld->z + SIZE_HEIGHT &&
					g_enemy[nCntFigure].pos.z - g_enemy[nCntFigure].fHight <= pPos->z + SIZE_HEIGHT)
				{
					pPos->z = g_enemy[nCntFigure].pos.z - (g_enemy[nCntFigure].fHight + SIZE_HEIGHT);
				}
				else if (g_enemy[nCntFigure].posOld.z + g_enemy[nCntFigure].fHight <= pPosOld->z - SIZE_HEIGHT &&
					g_enemy[nCntFigure].pos.z + g_enemy[nCntFigure].fHight >= pPos->z - SIZE_HEIGHT)
				{
					pPos->z = g_enemy[nCntFigure].pos.z + (g_enemy[nCntFigure].fHight + SIZE_HEIGHT);
				}

				else if (g_enemy[nCntFigure].posOld.x - g_enemy[nCntFigure].fWidth >= pPosOld->x + SIZE_WIDTH &&
					g_enemy[nCntFigure].pos.x - g_enemy[nCntFigure].fWidth <= pPos->x + SIZE_WIDTH)
				{
					pPos->x = g_enemy[nCntFigure].pos.x - (g_enemy[nCntFigure].fWidth + SIZE_WIDTH);
				}
				else if (g_enemy[nCntFigure].posOld.x + g_enemy[nCntFigure].fWidth <= pPosOld->x - SIZE_WIDTH &&
					g_enemy[nCntFigure].pos.x + g_enemy[nCntFigure].fWidth >= pPos->x - SIZE_WIDTH)
				{
					pPos->x = g_enemy[nCntFigure].pos.x + (g_enemy[nCntFigure].fWidth + SIZE_WIDTH);
				}
			}
		}
	}
}

//****************************************
// HIT判定
//****************************************
void HitEnemy(int nCntFigure, int nDamage)
{
	if (g_enemy[nCntFigure].bUse == true)
	{
		g_enemy[nCntFigure].nLife -= nDamage;

		if (g_enemy[nCntFigure].nLife <= 0)//敵の体力がなくなった
		{
			g_enemy[nCntFigure].bUse = false;
			g_enemy[nCntFigure].bDraw = false;
			SetParticle(g_enemy[nCntFigure].pos, 0, 10, 30.0f, 1, 0);

			AddScore(g_enemy[nCntFigure].nScore);
		}
		else
		{
			for (int nCntModel = nCntFigure; nCntModel < (nCntFigure + 1); nCntModel++)
			{

				g_enemy[nCntFigure].r = 0.5f;
				g_enemy[nCntFigure].g = 0.0f;
				g_enemy[nCntFigure].b = 0.0f;
				g_enemy[nCntFigure].a = 1.0f;

				g_enemy[nCntFigure].nCntDamage = DAMEGE_COUNT;
			}
		}
	}
	SetParticle(g_enemy[nCntFigure].pos, 0, 10, 5.0f, 1, 0);
}
//****************************************
// 情報の取得
//****************************************
Enemy *GetEnemy(void)
{
	return &g_enemy[0];
}
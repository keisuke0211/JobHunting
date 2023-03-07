//◇====================◇
//｜　　　　　　　　　　｜
//｜　壁の処理          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "game.h"
#include "wall.h"
#include "fade.h"
#include "Player_model.h"
#include "bullet.h"
#include "particle.h"
#include "camera.h"

//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffWall = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureWall[WALL_LABEL_MAX] = {};			//テクスチャへのポインタ
Wall g_Wall[MAX_WALL];
BreakWall g_BreakWall;
float g_GroundMoveX;//地面を横に動かす
float g_GroundMoveY;//地面を縦に動かす
//****************************************
//テクスチャファイル名
//****************************************
const char *c_apFllenameBlock[WALL_LABEL_MAX] =
{
	"data/TEXTURE/wall2.png",
	"data/TEXTURE/wall1.png",
	"data/TEXTURE/Transparent.png",//stone brick  Transparent
};
//****************************************
// 初期化処理
//****************************************
void InitWall(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	for (int nCntTexture = 0; nCntTexture < WALL_LABEL_MAX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFllenameBlock[nCntTexture],
			&g_pTextureWall[nCntTexture]);
	}

	g_BreakWall.Current = 0;
	g_BreakWall.Pull = 0;
	g_BreakWall.Total = 0;
	g_BreakWall.Current_F = 0.0f;
	g_BreakWall.Break = 0;

	//壁データの読み込め＆設定
	LoadWall();

	//g_BreakWall.Current_F = g_BreakWall.Current / g_BreakWall.Total;
	g_BreakWall.Current_F = (float)g_BreakWall.Break / g_BreakWall.Total;

	g_BreakWall.Pull = (float)g_BreakWall.Total - (g_BreakWall.Total * 0.8);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_Wall[nCntWall].nCype == 0)
		{
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		if (g_Wall[nCntWall].nCype == 1)
		{
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 3.1425f, 0.0f);
		}
		if (g_Wall[nCntWall].nCype == 2)
		{
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 1.57f, 0.0f);
		}
		if (g_Wall[nCntWall].nCype == 3)
		{
			g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 4.7125f, 0.0f);
		}
		if (g_Wall[nCntWall].nType == 0 || g_Wall[nCntWall].nType == 2)
		{
			g_Wall[nCntWall].nLife = 100;
		}
		else if(g_Wall[nCntWall].nType == 1)
		{
			g_Wall[nCntWall].nLife = 1;
		}

		//テクスチャの分割数を計算
		g_Wall[nCntWall].nSplitX = (int)g_Wall[nCntWall].fWidth / 50;
		g_Wall[nCntWall].nSplitY = (int)g_Wall[nCntWall].fHight / 50;
		g_Wall[nCntWall].nCntTime = 0;
		g_Wall[nCntWall].bUse = true;

		if (g_Wall[nCntWall].nType == 2)
		{
			g_Wall[nCntWall].bDraw = false;
		}
		else
		{
			g_Wall[nCntWall].bDraw = true;
		}
	}

	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffWall,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffWall->Lock(0, 0, (void **)&pVtx, 0);


	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth, +g_Wall[nCntWall].fHight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth, +g_Wall[nCntWall].fHight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_Wall[nCntWall].fWidth, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_Wall[nCntWall].fWidth, 0.0f, 0.0f);


		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標のの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_Wall[nCntWall].nSplitX, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f + g_Wall[nCntWall].nSplitY);
		pVtx[3].tex = D3DXVECTOR2(0.0f + g_Wall[nCntWall].nSplitX, 0.0f + g_Wall[nCntWall].nSplitY);
	}
	//頂点バッファのアンロック
	g_pVtxbuffWall->Unlock();

	
}
//****************************************
// 終了処理
//****************************************
void UninitWall(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	for (int nCntBlock = 0; nCntBlock < WALL_LABEL_MAX; nCntBlock++)
	{

		if (g_pTextureWall[nCntBlock] != NULL)
		{
			g_pTextureWall[nCntBlock]->Release();
			g_pTextureWall[nCntBlock] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffWall != NULL)
	{
		g_pVtxbuffWall->Release();
		g_pVtxbuffWall = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdateWall(void)
{

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffWall->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++,pVtx += 4)
	{
		if (g_Wall[nCntWall].bUse == true)
		{
			/*g_GroundMoveX += 1.0f;
			g_GroundMoveY += 1.0f;*/

			pVtx[0].tex.x = g_GroundMoveX;
			pVtx[1].tex.x = g_GroundMoveX + g_Wall[nCntWall].nSplitX;
			pVtx[2].tex.x = g_GroundMoveX;
			pVtx[3].tex.x = g_GroundMoveX + g_Wall[nCntWall].nSplitX;

			pVtx[0].tex.y = g_GroundMoveY;
			pVtx[1].tex.y = g_GroundMoveY;
			pVtx[2].tex.y = g_GroundMoveY + g_Wall[nCntWall].nSplitY;
			pVtx[3].tex.y = g_GroundMoveY + g_Wall[nCntWall].nSplitY;
		}
	}

	//頂点バッファのアンロック
	g_pVtxbuffWall->Unlock();
}
//****************************************
// 描画処理
//****************************************
void DrawWall(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_Wall[nCntWall].bDraw == true && g_Wall[nCntWall].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);

			D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);

			D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxbuffWall, 0, sizeof(VERTEX_3D));

			//◇――――――――――――◇
			//｜頂点フォーマットの設定　｜
			//◇――――――――――――◇
			pDevice->SetFVF(FVF_VERTEX_3D);

			//◇――――――――――――◇
			//｜　　テクスチャの設定　　｜
			//◇――――――――――――◇
			pDevice->SetTexture(0, g_pTextureWall[g_Wall[nCntWall].nType]);

			//◇――――――――――――◇
			//｜　　ポリゴンの描画　　　｜
			//◇――――――――――――◇

			//頂点情報構造体のサイズ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}
}
//****************************************
// 読み込み処理
//****************************************
void LoadWall(void)
{
	int c = 0;	   //1文字ずつ確認する変数
	int column = 1;//列数を数える変数
	int row = 0;//行数を数える変数

	char aData[150];//つなげる文字数
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data/csv/WallData.csv", "r");

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
		case 1:		g_Wall[row].nCype		= atoi(aData);			break;	//１列目：計算種類
		case 2:		g_Wall[row].nType		= atoi(aData);			break;	//　列目：画像種類
		case 3:		g_Wall[row].pos.x		= (float)(atoi(aData));	break;	//　列目：X座標
		case 4:		g_Wall[row].pos.y		= (float)(atoi(aData));	break;	//　列目：Y座標
		case 5:		g_Wall[row].pos.z		= (float)(atoi(aData));	break;	//　列目：Z座標
		case 6:		g_Wall[row].fHight		= (float)(atoi(aData));	break;	//　列目：高さ
		case 7:		g_Wall[row].fWidth		= (float)(atoi(aData));	break;	//　列目：横幅
		}
		//バッファを初期化
		memset(aData, 0, sizeof(aData));

		//列数を足す
		column++;

		//もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
		if (c == '\n')
		{
			if (g_Wall[row].nType == 1)
			{
				g_BreakWall.Current++;
				g_BreakWall.Total++;
			}
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

//プレイヤー
void CollisionWallPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	//線分の交差判定(失敗)t
	{
		////設定用ポインタ
		//VERTEX_3D *pVtx;

		////頂点バッファのロックと頂点情報へのポインタを取得
		//g_pVtxbuffWall->Lock(0, 0, (void **)&pVtx, 0);

		//for (int nCntWall1 = 0; nCntWall1 < MAX_WALL;nCntWall1++,pVtx += 4)
		//{
		//	//境界線を設定
		//	g_Wall[nCntWall1].vecLine = pVtx[1].pos - pVtx[0].pos;

		//	g_Wall[nCntWall1].vecToPos = *pPos - g_Wall[nCntWall1].pos;

		//	g_Wall[nCntWall1].fRate = (g_Wall[nCntWall1].vecLine.z * g_Wall[nCntWall1].vecToPos.x) - (g_Wall[nCntWall1].vecLine.x * g_Wall[nCntWall1].vecToPos.z);

		//	if (g_Wall[nCntWall1].fRate <= 0)
		//	{
		//		GetPlayer()->pos = GetPlayer()->posOld;
		//		Getcamara()->posV = Getcamara()->posOldV;
		//		Getcamara()->posR = Getcamara()->posOldR;
		//	}
		//}
		////頂点バッファのアンロック
		//g_pVtxbuffWall->Unlock();
	}

	{
		for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
		{
			if (g_Wall[nCntWall].bUse == true)
			{
				if (pPos->x >= g_Wall[nCntWall].pos.x - (g_Wall[nCntWall].fWidth + SIZE_WIDTH) &&
					pPos->x <= g_Wall[nCntWall].pos.x + (g_Wall[nCntWall].fWidth + SIZE_WIDTH) &&
					pPos->y >= g_Wall[nCntWall].pos.y - (g_Wall[nCntWall].fHight + SIZE_HEIGHT) &&
					pPos->y <= g_Wall[nCntWall].pos.y + (g_Wall[nCntWall].fHight + SIZE_HEIGHT) &&
					pPos->z >= g_Wall[nCntWall].pos.z - (g_Wall[nCntWall].fWidth + SIZE_WIDTH) &&
					pPos->z <= g_Wall[nCntWall].pos.z + (g_Wall[nCntWall].fWidth + SIZE_WIDTH))
				{
					if (g_Wall[nCntWall].nType == 2)
					{
						
					}
					else if (g_Wall[nCntWall].nCype == 0 || g_Wall[nCntWall].nCype == 1)
					{
						if (g_Wall[nCntWall].pos.y + g_Wall[nCntWall].fHight >= pPosOld->y + SIZE_HEIGHT &&
							g_Wall[nCntWall].pos.y - g_Wall[nCntWall].fHight <= pPosOld->y - SIZE_HEIGHT &&
							g_Wall[nCntWall].pos.z - 2 >= pPosOld->z + SIZE_WIDTH &&
							g_Wall[nCntWall].pos.z - 2 <= pPos->z + SIZE_WIDTH)
						{
							pPos->z = g_Wall[nCntWall].pos.z - (2 + SIZE_WIDTH);
						}
						else if (g_Wall[nCntWall].pos.y + g_Wall[nCntWall].fHight >= pPosOld->y + SIZE_HEIGHT &&
							g_Wall[nCntWall].pos.y - g_Wall[nCntWall].fHight <= pPosOld->y - SIZE_HEIGHT &&
							g_Wall[nCntWall].pos.z + 2 <= pPosOld->z - SIZE_WIDTH &&
							g_Wall[nCntWall].pos.z + 2 >= pPos->z - SIZE_WIDTH)
						{
							pPos->z = g_Wall[nCntWall].pos.z + (2 + SIZE_WIDTH);
						}
					}
					else if (g_Wall[nCntWall].nCype == 2 || g_Wall[nCntWall].nCype == 3)
					{
						if (g_Wall[nCntWall].pos.y + g_Wall[nCntWall].fHight >= pPosOld->y + SIZE_HEIGHT &&
							g_Wall[nCntWall].pos.y - g_Wall[nCntWall].fHight <= pPosOld->y - SIZE_HEIGHT &&
							g_Wall[nCntWall].pos.x - 2 >= pPosOld->x + SIZE_WIDTH &&
							g_Wall[nCntWall].pos.x - 2 <= pPos->x + SIZE_WIDTH)
						{
							pPos->x = g_Wall[nCntWall].pos.x - (2 + SIZE_WIDTH);
						}
						else if (g_Wall[nCntWall].pos.y + g_Wall[nCntWall].fHight >= pPosOld->y + SIZE_HEIGHT &&
							g_Wall[nCntWall].pos.y - g_Wall[nCntWall].fHight <= pPosOld->y - SIZE_HEIGHT &&
							g_Wall[nCntWall].pos.x + 2 <= pPosOld->x - SIZE_WIDTH &&
							g_Wall[nCntWall].pos.x + 2 >= pPos->x - SIZE_WIDTH)
						{
							pPos->x = g_Wall[nCntWall].pos.x + (2 + SIZE_WIDTH);
						}
					}
				}
			}
		}
	}
}
//****************************************
// データの所得
//****************************************
Wall *GetWall(void)
{
	return &g_Wall[0];
}
BreakWall *GetBreakWall(void)
{
	return &g_BreakWall;
}
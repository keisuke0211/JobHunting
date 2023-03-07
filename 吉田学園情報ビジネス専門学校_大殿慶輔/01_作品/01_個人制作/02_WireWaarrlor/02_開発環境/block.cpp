//◇====================◇
//｜　　　　　　　　　　｜
//｜　ブロックの処理    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "block.h"
#include "game.h"
#include "player.h"
#include "bg.h"
#include "camera.h"
#include "lure.h"
#include "sound.h"
//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9  g_pVtxbuffBlosk = NULL;						//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureBlock [BLOCK_LABEL_MAX] = {};			//テクスチャへのポインタ
Block g_aBlock[MAX_BLOCK];												//ブロックの情報
float g_GroundMoveX;													//地面を横に動かす
//****************************************
//テクスチャファイル名
//****************************************
const char *c_apFllenameBlock[BLOCK_LABEL_MAX] =
{
	"data/TEXTURE/Rocks02.png",
	"data/TEXTURE/Rocks00.png",
	"data/TEXTURE/Rocks01.png",
	"data/TEXTURE/Dir.png",
	"data/TEXTURE/block001.jpg",
	"data/TEXTURE/block002.jpg",
	"data/TEXTURE/Treasure Chest.png",
	"data/TEXTURE/Checkpoint (Flag Idle)(64x64).png",
	"data/TEXTURE/Checkpoint (Flag Idle)(64x64).png",
};
//****************************************
// ブロックの初期化処理
//****************************************
void InitBlock(void)
{
	Camera *pCamera = GetCamera();

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	for (int nCntTexture = 0; nCntTexture < BLOCK_LABEL_MAX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFllenameBlock[nCntTexture],
			&g_pTextureBlock[nCntTexture]);
	}

	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBlosk,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBlosk->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += 4)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0, 0, 0);										//位置
		g_aBlock[nCntBlock].posOld = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0);		//前回の位置
		g_aBlock[nCntBlock].fWidth = 0;														//幅
		g_aBlock[nCntBlock].fHight = 0;														//高さ
		g_aBlock[nCntBlock].nType = 0;														//種類
		g_aBlock[nCntBlock].nSpeed = 0;														//移動量
		g_aBlock[nCntBlock].nCounterAnim = 0;												//チャストのアニメーションカウント
		g_aBlock[nCntBlock].nPatternAnim = 0;												//チャストのアニメーションカウント
		g_aBlock[nCntBlock].nCounterAnim1 = 0;												//中間地点のアニメーションカウント
		g_aBlock[nCntBlock].nPatternAnim1 = 0;												//中間地点のアニメーションカウント
		g_aBlock[nCntBlock].nSplitX = 0;													//テクスチャの縦の分割数
		g_aBlock[nCntBlock].nSplitY = 0;													//テクスチャの横の分割数
		g_aBlock[nCntBlock].bUse = false;													//使用しているかどうか
		g_aBlock[nCntBlock].bChat = false;													//宝箱にふれたか
		g_aBlock[nCntBlock].bClear = false;													//クリアしたか
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇	

		pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth;
		pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;
		pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth;
		pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;
		pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight;
		pVtx[3].pos.z = 0.0f;

		//◇――――――――――――◇
		//｜　　　　rhwの設定　　　 ｜
		//◇――――――――――――◇
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//◇――――――――――――◇
		//｜　　　 頂点カラー 　　　｜
		//◇――――――――――――◇
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//◇――――――――――――◇
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇
		if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_TreasureChest)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);
		}
		else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint1)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);
		}
		else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint2)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
	//ブロックの配置
	LoadBlock();

	//頂点バッファのアンロック
	g_pVtxbuffBlosk->Unlock();
}
//****************************************
// ブロックの終了処理
//****************************************
void UninitBlock(void)
{
	//サウンドの停止
	StopSound();

	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (int nCntBlock = 0; nCntBlock < BLOCK_LABEL_MAX; nCntBlock++)
	{

		if (g_pTextureBlock[nCntBlock] != NULL)
		{
			g_pTextureBlock[nCntBlock]->Release();
			g_pTextureBlock[nCntBlock] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffBlosk != NULL)
	{
		g_pVtxbuffBlosk->Release();
		g_pVtxbuffBlosk = NULL;
	}
}
//****************************************
// ブロックの更新処理
//****************************************
void UpdateBlock(void)
{
	Player *pPlayer = Getplayer();	//プレイヤーの情報へのポインタ

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBlosk->Lock(0, 0, (void **)&pVtx, 0);
	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇	
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pVtx += 4)
	{
			D3DXVECTOR3 drawPos = g_aBlock[nCntBlock].pos;

			CameraPosition(&drawPos);

			pVtx[0].pos.x = drawPos.x - g_aBlock[nCntBlock].fWidth;
			pVtx[0].pos.y = drawPos.y - g_aBlock[nCntBlock].fHight;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = drawPos.x + g_aBlock[nCntBlock].fWidth;
			pVtx[1].pos.y = drawPos.y - g_aBlock[nCntBlock].fHight;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = drawPos.x - g_aBlock[nCntBlock].fWidth;
			pVtx[2].pos.y = drawPos.y + g_aBlock[nCntBlock].fHight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = drawPos.x + g_aBlock[nCntBlock].fWidth;
			pVtx[3].pos.y = drawPos.y + g_aBlock[nCntBlock].fHight;
			pVtx[3].pos.z = 0.0f;

			if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_TreasureChest)
			{
				if (g_aBlock[nCntBlock].bChat == true && g_aBlock[nCntBlock].bClear == false)
				{
					g_aBlock[nCntBlock].nCounterAnim++;
					if (g_aBlock[nCntBlock].nCounterAnim % 10 == 0)
					{
						if (g_aBlock[nCntBlock].nPatternAnim >= 0 && g_aBlock[nCntBlock].nPatternAnim <= 6)
						{
							if (g_aBlock[nCntBlock].nPatternAnim == 0)
							{
								PlaySound(SOUND_LABEL_SE_CHEST); //フックを戻す
							}
							g_aBlock[nCntBlock].nPatternAnim++;
						}
						else if (g_aBlock[nCntBlock].nPatternAnim == 7)
						{
							g_aBlock[nCntBlock].bClear = true;
							SetGameState(GAMESTATE_END1, 1);
						}
					}
				}
				pVtx[0].tex = D3DXVECTOR2(0.0f	+ ((g_aBlock[nCntBlock].nPatternAnim % 4)* 0.25f), 0.0f	+ ((g_aBlock[nCntBlock].nPatternAnim / 4)* 0.5));
				pVtx[1].tex = D3DXVECTOR2(0.25f + ((g_aBlock[nCntBlock].nPatternAnim % 4)* 0.25f), 0.0f	+ ((g_aBlock[nCntBlock].nPatternAnim / 4)* 0.5));
				pVtx[2].tex = D3DXVECTOR2(0.0f	+ ((g_aBlock[nCntBlock].nPatternAnim % 4)* 0.25f), 0.5f	+ ((g_aBlock[nCntBlock].nPatternAnim / 4)* 0.5));
				pVtx[3].tex = D3DXVECTOR2(0.25f + ((g_aBlock[nCntBlock].nPatternAnim % 4)* 0.25f), 0.5f	+ ((g_aBlock[nCntBlock].nPatternAnim / 4)* 0.5));
			}
			else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint1)
			{
				g_aBlock[nCntBlock].nCounterAnim1++;
				if (g_aBlock[nCntBlock].nCounterAnim1 % 10 == 0)
				{
					if (g_aBlock[nCntBlock].nPatternAnim1 >= 0 && g_aBlock[nCntBlock].nPatternAnim1 <= 9)
					{
						g_aBlock[nCntBlock].nPatternAnim1++;
					}
					else if (g_aBlock[nCntBlock].nPatternAnim1 == 10)
					{
						g_aBlock[nCntBlock].nPatternAnim1 = 1;
					}
				}
				pVtx[0].tex = D3DXVECTOR2(0.0f	+ ((g_aBlock[nCntBlock].nPatternAnim1 % 4)* 0.1f), 0.05f);
				pVtx[1].tex = D3DXVECTOR2(0.1f	+ ((g_aBlock[nCntBlock].nPatternAnim1 % 4)* 0.1f), 0.05f);
				pVtx[2].tex = D3DXVECTOR2(0.0f	+ ((g_aBlock[nCntBlock].nPatternAnim1 % 4)* 0.1f), 1.0f	);
				pVtx[3].tex = D3DXVECTOR2(0.1f	+ ((g_aBlock[nCntBlock].nPatternAnim1 % 4)* 0.1f), 1.0f	);
			}
			else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint2)
			{
				g_aBlock[nCntBlock].nCounterAnim1++;
				if (g_aBlock[nCntBlock].nCounterAnim1 % 10 == 0)
				{
					if (g_aBlock[nCntBlock].nPatternAnim1 >= 0 && g_aBlock[nCntBlock].nPatternAnim1 <= 9)
					{
						g_aBlock[nCntBlock].nPatternAnim1++;
					}
					else if (g_aBlock[nCntBlock].nPatternAnim1 == 10)
					{
						g_aBlock[nCntBlock].nPatternAnim1 = 1;
					}
				}
				pVtx[0].tex = D3DXVECTOR2(0.0f + ((g_aBlock[nCntBlock].nPatternAnim1 % 4)* 0.1f), 0.05f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + ((g_aBlock[nCntBlock].nPatternAnim1 % 4)* 0.1f), 0.05f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + ((g_aBlock[nCntBlock].nPatternAnim1 % 4)* 0.1f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + ((g_aBlock[nCntBlock].nPatternAnim1 % 4)* 0.1f), 1.0f);
			}
			else
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + g_aBlock[nCntBlock].nSplitX, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f + g_aBlock[nCntBlock].nSplitY);
				pVtx[3].tex = D3DXVECTOR2(0.0f + g_aBlock[nCntBlock].nSplitX, 0.0f + g_aBlock[nCntBlock].nSplitY);

				g_GroundMoveX = 1.0f;

				pVtx[0].tex.x = g_GroundMoveX;
				pVtx[1].tex.x = g_GroundMoveX + g_aBlock[nCntBlock].nSplitX;
				pVtx[2].tex.x = g_GroundMoveX;
				pVtx[3].tex.x = g_GroundMoveX + g_aBlock[nCntBlock].nSplitX;

				pVtx[0].tex.y = g_GroundMoveX;
				pVtx[1].tex.y = g_GroundMoveX;
				pVtx[2].tex.y = g_GroundMoveX + g_aBlock[nCntBlock].nSplitY;
				pVtx[3].tex.y = g_GroundMoveX + g_aBlock[nCntBlock].nSplitY;
			}
	}

	//頂点バッファのアンロック
	g_pVtxbuffBlosk->Unlock();
}
//****************************************
// ブロックの描画処理
//****************************************
void DrawBlock(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffBlosk, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].nType]);

			//◇――――――――――――◇
			//｜　　ポリゴンの描画　　　｜
			//◇――――――――――――◇

			//頂点情報構造体のサイズ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}
//****************************************
// ブロックの読み込み
//****************************************
void LoadBlock(void)
{
	int c = 0;		//1文字ずつ確認する変数
	int column = 1;	//行数を数える変数
	int line = 0;	//列数を数える変数

	char aData[150];//つなげる文字数
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data/csv/BlockData.csv", "r");

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
		case 1:		g_aBlock[line].nType	= atoi(aData);			break;	//１列目：種類
		case 2:		g_aBlock[line].fWidth	= (float)(atoi(aData));	break;	//  列目：幅
		case 3:		g_aBlock[line].fHight	= (float)(atoi(aData));	break;	//　列目：高さ
		case 4:		g_aBlock[line].pos.x	= (float)(atoi(aData));	break;	//　列目：X座標
		case 5:		g_aBlock[line].pos.y	= (float)(atoi(aData));	break;	//　列目：Y座標
		case 6:		g_aBlock[line].nSplitX	= atoi(aData);			break;	//　列目：縦の分割数
		case 7:		g_aBlock[line].nSplitY	= atoi(aData);			break;	//　列目：縦の分割数

		}
		//バッファを初期化
		memset(aData, 0, sizeof(aData));
		
		//列数を足す
		column++;

		//もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
		if (c == '\n')
		{
			column = 1;
			line++;
		}
		for (int nCntBlock = 0; nCntBlock < line; nCntBlock++)
		{
			//使用しているかどうか
			g_aBlock[nCntBlock].bUse = true;
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}
//****************************************
// ブロックとの当たり判定(プレイヤー)
//****************************************
bool CollisionBlockPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;//着地したかどうか 

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (pPos->x >= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH ) &&
				pPos->x <= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH	) &&
				pPos->y >= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT) &&
				pPos->y <= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT))//地面にめり込んだ
			{
				if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_TreasureChest || g_aBlock[nCntBlock].nType == BLOCK_LABEL_ROCKS02)
				{
					g_aBlock[nCntBlock].bChat = true;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint1)
				{
					Getplayer()->bCheckpoint = 1;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint2)
				{
					Getplayer()->bCheckpoint = 2;
				}
				else
				{
					if (g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight >= pPosOld->y + SIZE_HEIGHT &&
						g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight <= pPos->y + SIZE_HEIGHT)
					{
						bLand = true;
						pPos->y = g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT);	//ブロックの上に立たせる
						pMove->y = 0.0f;																		//移動量を０にする
					}
					else if (g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight <= pPosOld->y - SIZE_HEIGHT &&
						g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight >= pPos->y - SIZE_HEIGHT)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT);		//ブロックの上に立たせる
						pMove->y = 10.0f;
					}

					else if (g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth >= pPosOld->x + SIZE_WIDTH &&
						g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth <= pPos->x + SIZE_WIDTH)
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH);		//ブロックの上に立たせる
					}
					else if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= pPosOld->x - SIZE_WIDTH &&
						g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth >= pPos->x - SIZE_WIDTH)
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH);		//ブロックの上に立たせる
					}
				}
			}
		}

	}
	return bLand;
}
//****************************************
// ブロックとの当たり判定(ルアー)
//****************************************
bool CollisionBlockLure(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;//着地したかどうか 

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (pPos->x >= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_LURE_WIDTH*2) &&
				pPos->x <= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_LURE_WIDTH*2) &&
				pPos->y >= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_LURE_HEIGHT*2) &&
				pPos->y <= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_LURE_HEIGHT*2))//地面にめり込んだ
			{
				if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_TreasureChest || g_aBlock[nCntBlock].nType == BLOCK_LABEL_ROCKS02)
				{

				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint1)
				{

				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint2)
				{

				}
				else
				{
					if (g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight >= pPosOld->y + SIZE_LURE_HEIGHT &&
						g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight <= pPos->y + SIZE_LURE_HEIGHT)
					{
						bLand = true;
						pPos->y = g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_LURE_HEIGHT);	//ブロックの上に立たせる
					}
					else if (g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight <= pPosOld->y - SIZE_LURE_HEIGHT &&
						g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight >= pPos->y - SIZE_LURE_HEIGHT)
					{
						bLand = true;
						pPos->y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_LURE_HEIGHT);		//ブロックの上に立たせる
					}

					else if (g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth >= pPosOld->x + SIZE_LURE_WIDTH &&
						g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth <= pPos->x + SIZE_LURE_WIDTH)
					{
						bLand = true;
						pPos->x = g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_LURE_WIDTH);		//ブロックの上に立たせる
					}
					else if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= pPosOld->x - SIZE_LURE_WIDTH &&
						g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth >= pPos->x - SIZE_LURE_WIDTH)
					{
						bLand = true;
						pPos->x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_LURE_WIDTH);		//ブロックの上に立たせる
					}
				}
			}
		}

	}
	return bLand;
}

//****************************************
// ブロックとの当たり判定(振り子用)
//****************************************
bool CollisionBlockPendulumX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;//壁に当たったか

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (pPos->x >= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH) &&
				pPos->x <= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH) &&
				pPos->y >= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT) &&
				pPos->y <= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT))//地面にめり込んだ
			{
				if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_TreasureChest || g_aBlock[nCntBlock].nType == BLOCK_LABEL_ROCKS02)
				{
					g_aBlock[nCntBlock].bChat = true;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint1)
				{
					Getplayer()->bCheckpoint = 1;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint2)
				{
					Getplayer()->bCheckpoint = 2;
				}
				else
				{
					if (g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth >= pPosOld->x + SIZE_WIDTH &&
						g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth <= pPos->x + SIZE_WIDTH)
					{
						bLand = true;
						pPos->x = g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH);		//ブロックの上に立たせる
					}
					else if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= pPosOld->x - SIZE_WIDTH &&
						g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth >= pPos->x - SIZE_WIDTH)
					{
						bLand = true;
						pPos->x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH);		//ブロックの上に立たせる
					}
				}
			}
		}

	}
	return bLand;
}
bool CollisionBlockPendulumY(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;//壁に当たったか

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (pPos->x >= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH) &&
				pPos->x <= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH) &&
				pPos->y >= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT) &&
				pPos->y <= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT))//地面にめり込んだ
			{
				if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_TreasureChest || g_aBlock[nCntBlock].nType == BLOCK_LABEL_ROCKS02)
				{
					g_aBlock[nCntBlock].bChat = true;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint1)
				{
					Getplayer()->bCheckpoint = 1;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint2)
				{
					Getplayer()->bCheckpoint = 2;
				}
				else
				{
					if (g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight >= pPosOld->y + SIZE_HEIGHT &&
						g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight <= pPos->y + SIZE_HEIGHT)
					{
						bLand = true;
						pPos->y = g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT);	//ブロックの上に立たせる
						pMove->y = 0.0f;																		//移動量を０にする
					}
					else if (g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight <= pPosOld->y - SIZE_HEIGHT &&
						g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight >= pPos->y - SIZE_HEIGHT)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT);		//ブロックの上に立たせる
						pMove->y = 10.0f;
					}
				}
			}
		}

	}
	return bLand;
}

//****************************************
// ブロックとの当たり判定(放物線用)
//****************************************
bool CollisionBlockParabola(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;//壁に当たったか

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (pPos->x >= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH) &&
				pPos->x <= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH) &&
				pPos->y >= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT) &&
				pPos->y <= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT))//地面にめり込んだ
			{
				if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_TreasureChest || g_aBlock[nCntBlock].nType == BLOCK_LABEL_ROCKS02)
				{
					g_aBlock[nCntBlock].bChat = true;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint1)
				{
					Getplayer()->bCheckpoint = 1;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint2)
				{
					Getplayer()->bCheckpoint = 2;
				}
				else
				{
					if (g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight >= pPosOld->y + SIZE_HEIGHT &&
						g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight <= pPos->y + SIZE_HEIGHT)
					{
						bLand = true;
						pPos->y = g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT);	//ブロックの上に立たせる
						pMove->y = 0.0f;																		//移動量を０にする
					}
					else if (g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight <= pPosOld->y - SIZE_HEIGHT &&
						g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight >= pPos->y - SIZE_HEIGHT)
					{
						bLand = true;
						pPos->y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_HEIGHT);		//ブロックの上に立たせる
						pMove->y = 10.0f;
					}

					else if (g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth >= pPosOld->x + SIZE_WIDTH &&
						g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth <= pPos->x + SIZE_WIDTH)
					{
						bLand = true;
						pPos->x = g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH);		//ブロックの上に立たせる
					}
					else if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= pPosOld->x - SIZE_WIDTH &&
						g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth >= pPos->x - SIZE_WIDTH)
					{
						bLand = true;
						pPos->x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_WIDTH);		//ブロックの上に立たせる
					}
				}
			}
		}

	}
	return bLand;
}

//****************************************
// ブロックとの当たり判定(自動移動用)
//****************************************
bool CollisionBlockAutoLure(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;//壁に当たったか

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if (pPos->x >= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_LURE_WIDTH * 2) &&
				pPos->x <= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_LURE_WIDTH * 2) &&
				pPos->y >= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_LURE_HEIGHT * 2) &&
				pPos->y <= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_LURE_HEIGHT * 2))//地面にめり込んだ
			{
				if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_TreasureChest || g_aBlock[nCntBlock].nType == BLOCK_LABEL_ROCKS02)
				{
					g_aBlock[nCntBlock].bChat = true;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint1)
				{
					Getplayer()->bCheckpoint = 1;
				}
				else if (g_aBlock[nCntBlock].nType == BLOCK_LABEL_Checkpoint2)
				{
					Getplayer()->bCheckpoint = 2;
				}
				else
				{
					if (g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight >= pPosOld->y + SIZE_LURE_HEIGHT * 2 &&
						g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHight <= pPos->y + SIZE_LURE_HEIGHT * 2)
					{
						pPos->y = g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHight + SIZE_LURE_HEIGHT * 2);	//ブロックの上に立たせる
						pMove->y = 0.0f;																		//移動量を０にする
					}
					else if (g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight <= pPosOld->y - SIZE_LURE_HEIGHT * 2 &&
						g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHight >= pPos->y - SIZE_LURE_HEIGHT * 2)
					{
						bLand = true;
						pPos->y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHight + SIZE_LURE_HEIGHT * 2);		//ブロックの上に立たせる
						pMove->y = 10.0f;
					}

					else if (g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth >= pPosOld->x + SIZE_LURE_WIDTH * 2 &&
						g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth <= pPos->x + SIZE_LURE_WIDTH * 2)
					{
						bLand = true;
						pPos->x = g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth + SIZE_LURE_WIDTH * 2);		//ブロックの上に立たせる
					}
					else if (g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth <= pPosOld->x - SIZE_LURE_WIDTH * 2 &&
						g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth >= pPos->x - SIZE_LURE_WIDTH * 2)
					{
						bLand = true;
						pPos->x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth + SIZE_LURE_WIDTH * 2);		//ブロックの上に立たせる
					}
				}
			}
		}

	}
	return bLand;
}

//****************************************
// ブロックの所得
//****************************************
Block *GetBlock(void)
{
	return &g_aBlock[0];
}
/*-----------------------------------------------------------------------------------------------------

雲処理[cloud.cpp]
Author:平澤詩苑

-------------------------------------------------------------------------------------------------------*/
#include "cloud.h"
#include "input.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

//雲の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	int nType;				//種類
	bool bUse;				//使われているかどうか
}CLOUD;

//雲の種類構造体
typedef enum
{
	CLOUD_TYPE_SMALLEST = 0,		//一番小さい雲
	CLOUD_TYPE_SMALL,				//小さめな雲
	CLOUD_TYPE_MEDIUM,				//中ぐらいサイズの雲
	CLOUD_TYPE_BIG,					//入道雲(一番大きい)
	CLOUD_TYPE_MAX,
}CLOUD_TYPE;

#define MAX_CLOUD_TYPE		(4)								//雲の種類
#define MAX_CLOUD			(128)							//ライフの表示する数　= プレイヤーの最大ライフ
#define CLOUD_000			"data/TEXTURE/cloud_000.PNG"	//雲(小)の画像名
#define CLOUD_001			"data/TEXTURE/cloud_001.PNG"	//雲(小 Part2)の画像名
#define CLOUD_002			"data/TEXTURE/cloud_002.PNG"	//雲(中)の画像名
#define CLOUD_003			"data/TEXTURE/cloud_003.PNG"	//雲(大)の画像名

//グローバル変数宣言
LPDIRECT3DTEXTURE9			g_pTextureCloud[MAX_CLOUD_TYPE] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffCloud = NULL;					//頂点バッファへのポインタ
CLOUD g_Cloud[MAX_CLOUD];											//クラウドの情報
int g_nCounterAppearCloud;											//雲の発生タイミング

//雲の初期化処理
void InitCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, CLOUD_000, &g_pTextureCloud[CLOUD_TYPE_SMALLEST]);
	D3DXCreateTextureFromFile(pDevice, CLOUD_001, &g_pTextureCloud[CLOUD_TYPE_SMALL]);
	D3DXCreateTextureFromFile(pDevice, CLOUD_002, &g_pTextureCloud[CLOUD_TYPE_MEDIUM]);
	D3DXCreateTextureFromFile(pDevice, CLOUD_003, &g_pTextureCloud[CLOUD_TYPE_BIG]);

	//雲の情報初期化
	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++)
	{
		g_Cloud[nCntCloud].pos  = D3DXVECTOR3(0.0f, 0.0f, 0.0f); 
		g_Cloud[nCntCloud].move = D3DXVECTOR3(0.3f, 0.0f, 0.0f);
		g_Cloud[nCntCloud].bUse = false;
	}
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CLOUD, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffCloud, NULL);

	//ポインタを設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCloud->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++, pVtx += 4)
	{
		//頂点座標の設定
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		//rhwの設定
		{
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
		}

		//頂点カラーの設定
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//テクスチャの座標
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.00001f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.00001f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffCloud->Unlock();

	//ランダム関数のシードを時刻より設定
	srand((unsigned int)time(NULL));

	//雲が出るタイミングを決める
	g_nCounterAppearCloud = rand() % 600;

	//最初に雲を複数回出現させる
	SetCloud();
	SetCloud();
	SetCloud();
}

//雲の終了処理
void UninitCloud(void)
{
	//テクスチャ(４枚)の破棄
	for (int nCntCloud = 0; nCntCloud < CLOUD_TYPE_MAX; nCntCloud++)
	{
		if (g_pTextureCloud[nCntCloud] != NULL)
		{
			g_pTextureCloud[nCntCloud]->Release();
			g_pTextureCloud[nCntCloud] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCloud != NULL)
	{
		g_pVtxBuffCloud->Release();
		g_pVtxBuffCloud = NULL;
	}
}

//雲の更新処理
void UpdateCloud(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCloud->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++, pVtx += 4)
	{
		//雲が使用されている
		if (g_Cloud[nCntCloud].bUse == true)
		{
			g_Cloud[nCntCloud].pos.x -= g_Cloud[nCntCloud].move.x;

			//雲のサイズごとに頂点座標を大きくする
			switch (g_Cloud[nCntCloud].nType)
			{
			case CLOUD_TYPE_SMALLEST:		//一番小さいサイズと２番目に小さいサイズ
			case CLOUD_TYPE_SMALL:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				break;

			case CLOUD_TYPE_MEDIUM:			//中くらいサイズと最大サイズ
			case CLOUD_TYPE_BIG:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				break;
			}

			//雲が画面外に出てしまった
			if (g_Cloud[nCntCloud].pos.x < -200.0f)
			{
				g_Cloud[nCntCloud].bUse = false;
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffCloud->Unlock();

	if (g_nCounterAppearCloud > 0)
	{
		g_nCounterAppearCloud--;

		if (g_nCounterAppearCloud == 0)
		{
			//雲を召喚する
			SetCloud();

			//雲が出るタイミングを決める
			g_nCounterAppearCloud = rand() % 600;
		}
	}
}

//雲の描画処理
void DrawCloud(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCloud, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++)
	{
		if (g_Cloud[nCntCloud].bUse == true)
		{
			int nType = g_Cloud[nCntCloud].nType;

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCloud[nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCloud * 4, 2);
		}
	}
}

//雲の設定処理
void SetCloud(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCloud->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++, pVtx += 4)
	{
		//雲が使われていない
		if (g_Cloud[nCntCloud].bUse == false)
		{
			//雲の出現X座標を右画面端よりさらに奥に設定
			g_Cloud[nCntCloud].pos.x = (float)(rand() % 200) + 1380.0f;

			//雲のY座標をランダムで設定
			g_Cloud[nCntCloud].pos.y = (float)(rand() % (int)(SCREEN_HEIGHT * 0.5f)) + 50.0f;

			//雲の種類を決める
			g_Cloud[nCntCloud].nType = rand() % CLOUD_TYPE_MAX;

			//雲のサイズごとに頂点座標を大きくする
			switch (g_Cloud[nCntCloud].nType)
			{
			case CLOUD_TYPE_SMALLEST:		//一番小さいサイズと２番目に小さいサイズ
			case CLOUD_TYPE_SMALL:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				break;

			case CLOUD_TYPE_MEDIUM:			//中くらいサイズと最大サイズ
			case CLOUD_TYPE_BIG:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				break;
			}
	
			//使用状態にする
			g_Cloud[nCntCloud].bUse = true;			
			
			break;
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffCloud->Unlock();
}

//雲の設定処理
void SetCloud2(void)
{
	VERTEX_2D *pVtx;

	int n = 5;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCloud->Lock(0, 0, (void* *)&pVtx, 0);

	for (int nCntCloud = 0; nCntCloud < MAX_CLOUD; nCntCloud++, pVtx += 4)
	{
		//雲が使われていない
		if (g_Cloud[nCntCloud].bUse == false)
		{
			//雲の出現X座標を右画面端よりさらに奥に設定
			g_Cloud[nCntCloud].pos.x = (float)(rand() % SCREEN_WIDTH);

			//雲のY座標をランダムで設定
			g_Cloud[nCntCloud].pos.y = (float)(rand() % (int)(SCREEN_HEIGHT * 0.5f)) + 50.0f;

			//雲の種類を決める
			g_Cloud[nCntCloud].nType = rand() % CLOUD_TYPE_MAX;

			//雲のサイズごとに頂点座標を大きくする
			switch (g_Cloud[nCntCloud].nType)
			{
			case CLOUD_TYPE_SMALLEST:		//一番小さいサイズと２番目に小さいサイズ
			case CLOUD_TYPE_SMALL:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y - 32.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 64.0f, g_Cloud[nCntCloud].pos.y + 32.0f, 0.0f);
				break;

			case CLOUD_TYPE_MEDIUM:			//中くらいサイズと最大サイズ
			case CLOUD_TYPE_BIG:
				pVtx[0].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y - 64.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x - 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Cloud[nCntCloud].pos.x + 128.0f, g_Cloud[nCntCloud].pos.y + 64.0f, 0.0f);
				break;
			}

			//使用状態にする
			g_Cloud[nCntCloud].bUse = true;

			n--;

			if (n == 0) 
			{
				break;
			}
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffCloud->Unlock();
}
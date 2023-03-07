//◇======================◇
//｜　　　　　　　　　  　｜
//｜　ボスの㏋ゲージ処理  ｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"boss_hp.h"
#include"boss.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_BG (4)		//最大テクスチャ
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBosshp = NULL;				//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureBosshp[MAX_BG] = {};			//テクスチャへのポインタ
Bosshp g_Bosshp[MAX_BG];										//ボス㏋ゲージの情報

//◇============================◇
//｜　　HPゲージの初期化処理　　｜
//◇============================◇
void InitBosshp(void)
{
	Boss *pBoss = GetBoss(); //敵の情報へのポインタ

	int nCntText;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//テクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\HP.jpg", &g_pTextureBosshp[0]);
	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\HP1.jpg", &g_pTextureBosshp[1]);
	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\HP2.jpg", &g_pTextureBosshp[2]);
	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\HP_flame.png",&g_pTextureBosshp[3]);

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBosshp,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBosshp->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntText = 0; nCntText < MAX_BG; nCntText++, pVtx += 4)
	{
		g_Bosshp[nCntText].nType = 0;
		g_Bosshp[nCntText].bUse = false;
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		if (nCntText == 0)
		{
			pVtx[0].pos = D3DXVECTOR3(20.0f, 10.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(990.0f, 10.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(20.0f, 40.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(990.0f, 40.0f, 0.0f);
		}
		else if (nCntText == 1)
		{
			pVtx[0].pos = D3DXVECTOR3(20.0f, 10.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(990.0f, 10.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(20.0f, 40.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(990.0f, 40.0f, 0.0f);
		}
		else if (nCntText == 2)
		{
			pVtx[0].pos = D3DXVECTOR3(20.0f, 10.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(990.0f, 10.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(20.0f, 40.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(990.0f, 40.0f, 0.0f);
		}
		else if (nCntText == 3)
		{
			pVtx[0].pos = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1000.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(10.0f, 50.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1000.0f, 50.0f, 0.0f);
		}
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	g_Bosshp[0].curt_nLife = pBoss[0].curt_nLife;
	g_Bosshp[0].init_nLife = pBoss[0].init_nLife;

	g_Bosshp[1].curt_nLife = pBoss[1].curt_nLife;
	g_Bosshp[1].init_nLife = pBoss[1].init_nLife;

	g_Bosshp[2].curt_nLife = pBoss[2].curt_nLife;
	g_Bosshp[2].init_nLife = pBoss[2].init_nLife;
	//頂点バッファのアンロック
	g_pVtxbuffBosshp->Unlock();
}
//◇==========================◇
//｜　　HPゲージの終了処理　　｜
//◇==========================◇
void UninitBosshp(void)
{
	int nCnttext;
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (nCnttext = 0; nCnttext < MAX_BG; nCnttext++)
	{
		if (g_pTextureBosshp[nCnttext] != NULL)
		{
			g_pTextureBosshp[nCnttext]->Release();
			g_pTextureBosshp[nCnttext] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffBosshp != NULL)
	{
		g_pVtxbuffBosshp->Release();
		g_pVtxbuffBosshp = NULL;
	}
}
//◇==========================◇
//｜　　HPゲージの更新処理　　｜
//◇==========================◇
void UpdateBosshp(void)
{
	Boss *pBoss = GetBoss(); //敵の情報へのポインタ

//	int nCntText;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBosshp->Lock(0, 0, (void **)&pVtx, 0);

		//現在のステータスと減らしているステータスが違えば減らす
		//体力
		if (pBoss[0].bDrw == true)
		{
			if (pBoss[0].curt_nLife > g_Bosshp[0].curt_nLife)
			{
				g_Bosshp[0].curt_nLife = pBoss[0].curt_nLife;
			}
			else if (pBoss[0].curt_nLife < g_Bosshp[0].curt_nLife)
			{
				g_Bosshp[0].curt_nLife--;
			}

			//最大体力
			if (pBoss[0].init_nLife < g_Bosshp[0].init_nLife)
			{
				g_Bosshp[0].init_nLife--;
			}
		}
		else if (pBoss[1].bDrw == true)
		{
			if (pBoss[1].curt_nLife > g_Bosshp[1].curt_nLife)
			{
				g_Bosshp[1].curt_nLife = pBoss[1].curt_nLife;
			}
			else if (pBoss[1].curt_nLife < g_Bosshp[1].curt_nLife)
			{
				g_Bosshp[1].curt_nLife--;
			}

			//最大体力
			if (pBoss[1].init_nLife < g_Bosshp[1].init_nLife)
			{
				g_Bosshp[1].init_nLife--;
			}
		}
		if (pBoss[2].bDrw == true)
		{
			if (pBoss[2].curt_nLife > g_Bosshp[2].curt_nLife)
			{
				g_Bosshp[2].curt_nLife = pBoss[2].curt_nLife;
			}
			else if (pBoss[2].curt_nLife < g_Bosshp[2].curt_nLife)
			{
				g_Bosshp[2].curt_nLife--;
			}

			//最大体力
			if (pBoss[2].init_nLife < g_Bosshp[2].init_nLife)
			{
				g_Bosshp[2].init_nLife--;
			}
		}
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇

		if (pBoss[0].bDrw == true)
		{
			g_Bosshp[0].bUse = true;
			g_Bosshp[3].bUse = true;

			//		  //ポリゴンのサイズ変更
			pVtx[1].pos.x = g_Bosshp[0].pos.x *((float)g_Bosshp[0].curt_nLife / pBoss[0].init_nLife) * 1.3f;
			pVtx[3].pos.x = g_Bosshp[0].pos.x *((float)g_Bosshp[0].curt_nLife / pBoss[0].init_nLife) * 1.3f;

			////テクスチャサイズ変更
			pVtx[1].tex.x = 1.0f * ((float)g_Bosshp[0].curt_nLife / pBoss[0].init_nLife);
			pVtx[3].tex.x = 1.0f * ((float)g_Bosshp[0].curt_nLife / pBoss[0].init_nLife);

			if (pBoss[0].curt_nLife >= pBoss[0].init_nLife / 2)
			{
				g_Bosshp[0].nType = 0;
			}
			else if (pBoss[0].curt_nLife <= pBoss[0].init_nLife / 2 && pBoss[0].curt_nLife >= pBoss[0].init_nLife / 4)
			{
				g_Bosshp[0].nType = 1;
			}
			else if (pBoss[0].curt_nLife <= pBoss[0].init_nLife / 4)
			{
				g_Bosshp[0].nType = 2;
			}
		}

		else if (pBoss[1].bDrw == true)
		{
			g_Bosshp[1].bUse = true;

			//		  //ポリゴンのサイズ変更
			pVtx[1].pos.x = g_Bosshp[1].pos.x *((float)g_Bosshp[1].curt_nLife / pBoss[1].init_nLife) * 1;
			pVtx[3].pos.x = g_Bosshp[1].pos.x *((float)g_Bosshp[1].curt_nLife / pBoss[1].init_nLife) * 1;

			////テクスチャサイズ変更
			pVtx[1].tex.x = 1.0f * ((float)g_Bosshp[1].curt_nLife / pBoss[1].init_nLife);
			pVtx[3].tex.x = 1.0f * ((float)g_Bosshp[1].curt_nLife / pBoss[1].init_nLife);

			if (pBoss[1].curt_nLife >= pBoss[1].init_nLife / 2)
			{
				g_Bosshp[1].nType = 0;
			}
			else if (pBoss[1].curt_nLife <= pBoss[1].init_nLife / 2 && pBoss[1].curt_nLife >= pBoss[1].init_nLife / 4)
			{
				g_Bosshp[1].nType = 1;
			}
			else if (pBoss[1].curt_nLife <= pBoss[1].init_nLife / 4)
			{
				g_Bosshp[1].nType = 2;
			}
		}

		else if (pBoss[2].bDrw == true)
		{
			g_Bosshp[2].bUse = true;

			//		  //ポリゴンのサイズ変更
			pVtx[1].pos.x = g_Bosshp[2].pos.x *((float)g_Bosshp[2].curt_nLife / pBoss[2].init_nLife) * 1;
			pVtx[3].pos.x = g_Bosshp[2].pos.x *((float)g_Bosshp[2].curt_nLife / pBoss[2].init_nLife) * 1;

			////テクスチャサイズ変更
			pVtx[1].tex.x = 1.0f * ((float)g_Bosshp[2].curt_nLife / pBoss[2].init_nLife);
			pVtx[3].tex.x = 1.0f * ((float)g_Bosshp[2].curt_nLife / pBoss[2].init_nLife);

			if (pBoss[2].curt_nLife >= pBoss[2].init_nLife / 2)
			{
				g_Bosshp[2].nType = 0;
			}
			else if (pBoss[2].curt_nLife <= pBoss[2].init_nLife / 2 && pBoss[2].curt_nLife >= pBoss[2].init_nLife / 4)
			{
				g_Bosshp[2].nType = 1;
			}
			else if (pBoss[2].curt_nLife <= pBoss[2].init_nLife / 4)
			{
				g_Bosshp[2].nType = 2;
			}
		}
	//頂点バッファのアンロック
	g_pVtxbuffBosshp->Unlock();
}
//◇==========================◇
//｜　　HPゲージの描画処理　　｜
//◇==========================◇
void DrawBosshp(void)
{
	int nCntText;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffBosshp, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇
	for (nCntText = 0; nCntText < MAX_BG; nCntText++)
	{
		pDevice->SetTexture(0, g_pTextureBosshp[g_Bosshp[nCntText].nType]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntText * 4, 2);
	}
}
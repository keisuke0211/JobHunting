//◇====================◇
//｜　　　　　　　　　　｜
//｜　爆発の処理        ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"bullet.h"
#include"explosion.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_EXPLOSION (400) //爆発最大数

//◇==================◇
//｜　 構造体の定義 　｜
//◇==================◇
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXCOLOR col;//色
	int nCounterAnim;//アニメーションカウンター
	int nPatternAnim;//アニメーションパターンNo.
	bool bUse;//使用しているかどうか
}Explosion;

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;				 //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		 //頂点バッファポインタ

Explosion g_aExplosion[MAX_EXPLOSION];//爆発情報

//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【爆発・処理】｜―――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇============================◇
//｜　　 爆発の初期化処理　　 　｜
//◇============================◇
void InitExplosion(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntExplosion;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\explosion000.png",
		&g_pTextureExplosion);

	//爆弾の情報の初期化処理
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f);

		g_aExplosion[nCntExplosion].bUse = false;

		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
	}

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		

		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
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
		pVtx[0].col = D3DXCOLOR(.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4; //頂点データのポインタを４つ分進める
	}
	//頂点バッファのアンロック
	g_pVtxBuffExplosion->Unlock();
}
//◇==========================◇
//｜　　 爆発の終了処理　　 　｜
//◇==========================◇
void UninitExplosion(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇―――――――――――◇
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}
//◇==========================◇
//｜　　 爆発の更新処理　　 　｜
//◇==========================◇
void UpdateExplosion(void)
{
	int nCntExplosion;

	//テクスチャ座標の更新
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{

		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている
			g_aExplosion[nCntExplosion].nCounterAnim++;//カウンターを加算

			if ((g_aExplosion[nCntExplosion].nCounterAnim % 10) == 0)
			{
				//g_aExplosion[nCntExplosion].nPatternAnim = 0;
				g_aExplosion[nCntExplosion].nPatternAnim++;//パターンNo.を更新する

				if (g_aExplosion[nCntExplosion].nPatternAnim == 10)
				{
					g_aExplosion[nCntExplosion].bUse = false;
				}

				//・テクスチャ座標の再設定
				pVtx[0].tex = D3DXVECTOR2((0.0f + (g_aExplosion[nCntExplosion].nPatternAnim * 0.1f)), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((0.1f + (g_aExplosion[nCntExplosion].nPatternAnim * 0.1f)), 0.0f);
				pVtx[2].tex = D3DXVECTOR2((0.0f + (g_aExplosion[nCntExplosion].nPatternAnim * 0.1f)), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((0.1f + (g_aExplosion[nCntExplosion].nPatternAnim * 0.1f)), 1.0f);

			}
		}
		pVtx += 4; //該当の位置まで進める
	}
	//頂点バッファのアンロック
	g_pVtxBuffExplosion->Unlock();
}
//◇==========================◇
//｜　　 爆発の描画処理　　 　｜
//◇==========================◇
void DrawExplosion(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntExplosion;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆弾が使用されている
		 //ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//◇==========================◇
//｜　 　爆発の設定処理　　 　｜
//◇==========================◇
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col, col;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;


			//◇――――――――――――◇
			//｜ 　　頂点座標を設定　　 ｜
			//◇――――――――――――◇

			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - SIZE_WIDTH, g_aExplosion[nCntExplosion].pos.y - SIZE_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + SIZE_WIDTH, g_aExplosion[nCntExplosion].pos.y - SIZE_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - SIZE_WIDTH, g_aExplosion[nCntExplosion].pos.y + SIZE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + SIZE_WIDTH, g_aExplosion[nCntExplosion].pos.y + SIZE_HEIGHT, 0.0f);

			//◇――――――――――――◇
			//｜　　　 頂点カラー 　　　｜
			//◇――――――――――――◇
			pVtx[0].col = g_aExplosion[nCntExplosion].col;
			pVtx[1].col = g_aExplosion[nCntExplosion].col;
			pVtx[2].col = g_aExplosion[nCntExplosion].col;
			pVtx[3].col = g_aExplosion[nCntExplosion].col;
			//◇――――――――――――◇
			//｜　テクスチュ座標の設定　｜
			//◇――――――――――――◇
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);
			
			g_aExplosion[nCntExplosion].bUse = true; //弾が使用している状態にする

			break;
		}
		pVtx += 4; //該当の位置まで進める
	}
	//頂点バッファのアンロック
	g_pVtxBuffExplosion->Unlock();
}
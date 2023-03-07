//◇====================◇
//｜　　　　　　　　　　｜
//｜　エフェクト処理    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "Goal_effect.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_EFFECT (48000) //エフェクトの最大数

//◇====================◇
//｜ 　　情報構造体 　　｜
//◇====================◇

//エフェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3	rot;	// 向き
	D3DXVECTOR3 move;	//移動量
	D3DXMATRIX	mtxWorld;	//ワールドマトリックス
	D3DXCOLOR col;		//頂点カラー
	float fRadius;		//半径（大きさ）
	int  nLife;			//寿命（表示時間）
	int nType;			//エフェクト種類
	bool bUse;			//使用しているかどうか
	float rRot;
} GoalEffect;

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGoal_Effect = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureGoal_Effect = NULL;			//テクスチャへのポインタ
GoalEffect g_aGoalEffect[MAX_EFFECT];

//◇==================================◇
//｜　　 エフェクトの初期化処理　　 　｜
//◇==================================◇
void InitGoalEffect(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEffect;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチャの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureGoal_Effect);

	//爆弾の情報の初期化処理
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aGoalEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGoalEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aGoalEffect[nCntEffect].fRadius = 6.0f;
		g_aGoalEffect[nCntEffect].nLife = 25;
		g_aGoalEffect[nCntEffect].bUse = false;
	}

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGoal_Effect,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffGoal_Effect->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//◇――――――――――――◇
		//｜　　　　rhwの設定　　　 ｜
		//◇――――――――――――◇
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//◇――――――――――――◇
		//｜　　　 頂点カラー 　　　｜
		//◇――――――――――――◇
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//◇――――――――――――◇
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //頂点データのポインタを４つ分進める
	}
	//頂点バッファのアンロック
	g_pVtxBuffGoal_Effect->Unlock();
}
//◇================================◇
//｜　　 エフェクトの終了処理　　 　｜
//◇================================◇
void UninitGoalEffect(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇―――――――――――◇
	if (g_pTextureGoal_Effect != NULL)
	{
		g_pTextureGoal_Effect->Release();
		g_pTextureGoal_Effect = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffGoal_Effect != NULL)
	{
		g_pVtxBuffGoal_Effect->Release();
		g_pVtxBuffGoal_Effect = NULL;
	}
}
//◇================================◇
//｜　　 エフェクトの更新処理　　 　｜
//◇================================◇
void UpdateGoalEffect(void)
{
	int nCntEffect;

	//テクスチャ座標の更新
	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffGoal_Effect->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aGoalEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている

		 //エフェクの位置更新
			g_aGoalEffect[nCntEffect].pos.x += g_aGoalEffect[nCntEffect].move.x;
			g_aGoalEffect[nCntEffect].pos.y += g_aGoalEffect[nCntEffect].move.y;
			g_aGoalEffect[nCntEffect].pos.z += g_aGoalEffect[nCntEffect].move.z;

			//サイズ変更
			//g_aGoalEffect[nCntEffect].fRadius -= g_aGoalEffect[nCntEffect].fRadius / g_aGoalEffect[nCntEffect].nLife;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aGoalEffect[nCntEffect].fRadius, g_aGoalEffect[nCntEffect].fRadius, g_aGoalEffect[nCntEffect].fRadius);
			pVtx[1].pos = D3DXVECTOR3(g_aGoalEffect[nCntEffect].fRadius, g_aGoalEffect[nCntEffect].fRadius, g_aGoalEffect[nCntEffect].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_aGoalEffect[nCntEffect].fRadius, -g_aGoalEffect[nCntEffect].fRadius, -g_aGoalEffect[nCntEffect].fRadius);
			pVtx[3].pos = D3DXVECTOR3(g_aGoalEffect[nCntEffect].fRadius, -g_aGoalEffect[nCntEffect].fRadius, -g_aGoalEffect[nCntEffect].fRadius);

			//不透明度更新
			g_aGoalEffect[nCntEffect].col.a += 1.0f / g_aGoalEffect[nCntEffect].nLife;

			//頂点カラーの設定
			pVtx[0].col = g_aGoalEffect[nCntEffect].col;
			pVtx[1].col = g_aGoalEffect[nCntEffect].col;
			pVtx[2].col = g_aGoalEffect[nCntEffect].col;
			pVtx[3].col = g_aGoalEffect[nCntEffect].col;

			//寿命と大きさを減らしていく
			g_aGoalEffect[nCntEffect].nLife--;

			if (g_aGoalEffect[nCntEffect].nLife == 0)
			{//寿命が尽きた

				g_aGoalEffect[nCntEffect].bUse = false;
			}
		}
		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	//頂点バッファのアンロック
	g_pVtxBuffGoal_Effect->Unlock();
}
//◇================================◇
//｜　　 エフェクトの描画処理　　 　｜
//◇================================◇
void DrawGoalEffect(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用

	int nCntEffect;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGoal_Effect, 0, sizeof(VERTEX_3D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_3D);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aGoalEffect[nCntEffect].bUse == true)
		{//エフェクトが使用されている
		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aGoalEffect[nCntEffect].mtxWorld);

			//ビューマトリックスをGet
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aGoalEffect[nCntEffect].mtxWorld, NULL, &mtxView);	//逆行列を求める
			g_aGoalEffect[nCntEffect].mtxWorld._41 = DEFAULT_FLOA;
			g_aGoalEffect[nCntEffect].mtxWorld._42 = DEFAULT_FLOA;
			g_aGoalEffect[nCntEffect].mtxWorld._43 = DEFAULT_FLOA;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aGoalEffect[nCntEffect].pos.x, g_aGoalEffect[nCntEffect].pos.y, g_aGoalEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&g_aGoalEffect[nCntEffect].mtxWorld, &g_aGoalEffect[nCntEffect].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aGoalEffect[nCntEffect].mtxWorld);

			//◇――――――――――――◇
			//｜　　テクスチャの設定　　｜
			//◇――――――――――――◇
			pDevice->SetTexture(0, g_pTextureGoal_Effect);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//◇================================◇
//｜　 　エフェクトの設定処理　　 　｜
//◇================================◇
void SetGoalEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nType)
{
	int nCntEffect;

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffGoal_Effect->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4)
	{
		if (g_aGoalEffect[nCntEffect].bUse == false)
		{
			g_aGoalEffect[nCntEffect].pos = pos;
			g_aGoalEffect[nCntEffect].move = move;
			g_aGoalEffect[nCntEffect].col = col;
			g_aGoalEffect[nCntEffect].fRadius = fRadius;
			g_aGoalEffect[nCntEffect].nLife = nLife;
			g_aGoalEffect[nCntEffect].nType = nType;
			g_aGoalEffect[nCntEffect].bUse = true;

			//◇――――――――――――◇
			//｜ 　　頂点座標を設定　　 ｜
			//◇――――――――――――◇

			pVtx[0].pos = D3DXVECTOR3(-g_aGoalEffect[nCntEffect].fRadius, g_aGoalEffect[nCntEffect].fRadius, g_aGoalEffect[nCntEffect].fRadius);
			pVtx[1].pos = D3DXVECTOR3(g_aGoalEffect[nCntEffect].fRadius, g_aGoalEffect[nCntEffect].fRadius, g_aGoalEffect[nCntEffect].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_aGoalEffect[nCntEffect].fRadius, -g_aGoalEffect[nCntEffect].fRadius, -g_aGoalEffect[nCntEffect].fRadius);
			pVtx[3].pos = D3DXVECTOR3(g_aGoalEffect[nCntEffect].fRadius, -g_aGoalEffect[nCntEffect].fRadius, -g_aGoalEffect[nCntEffect].fRadius);

			//◇――――――――――――◇
			//｜　　　 頂点カラー 　　　｜
			//◇――――――――――――◇
			pVtx[0].col = g_aGoalEffect[nCntEffect].col;
			pVtx[1].col = g_aGoalEffect[nCntEffect].col;
			pVtx[2].col = g_aGoalEffect[nCntEffect].col;
			pVtx[3].col = g_aGoalEffect[nCntEffect].col;

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点バッファのアンロック
			g_pVtxBuffGoal_Effect->Unlock();
			break;
		}
	}
}
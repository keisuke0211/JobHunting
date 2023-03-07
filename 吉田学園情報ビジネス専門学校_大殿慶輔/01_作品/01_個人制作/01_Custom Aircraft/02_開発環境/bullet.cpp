//◇====================◇
//｜　　　　　　　　　　｜
//｜　　　弾の処理　　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"bullet.h"
#include"effect.h"
#include"particle.h"
#include"Summon.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_BULLET (400)  //弾の最大数

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void CollisionEnemy	(Bullet *pBullet);
void CollisionSummon(Bullet *pBullet);
void CollisionBoss(Bullet *pBullet);
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DTEXTURE9		g_pTextureBullet[2] = {};			 //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		 //頂点バッファポインタ
Bullet g_aBullet[MAX_BULLET];//弾の情報
float g_fLengthBullit;									    //対角線の長さ
float g_fAngleBullit;									    //対角線の角度

//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【弾・処理】｜―――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇============================◇
//｜　　　 弾の初期化処理　　 　｜
//◇============================◇
void InitBullet(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntBullet;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Bullet.png",
		&g_pTextureBullet[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Human soul.png",
		&g_pTextureBullet[1]);

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);

	nCntBullet = 0;

	//弾の情報の初期化処理
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = LIFE_DATA;

		g_aBullet[nCntBullet].bUse = false;
		//使用していない状態にする//対角線の長さを算出する(p幅*p幅+p高さ*p高さ);
		g_fLengthBullit = sqrtf(BULLET_WIDTH* BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT);

		//対角線の角度を算出する(p幅,p高さ)
		g_fAngleBullit = atan2f(BULLET_WIDTH, BULLET_HEIGHT);

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

		pVtx += 4; //頂点データのポインタを４つ分進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffBullet->Unlock();
}
//◇==========================◇
//｜　　　 弾の終了処理　　 　｜
//◇==========================◇
void UninitBullet(void)
{
	int nCntBullet;

	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	for (nCntBullet = 0; nCntBullet < MAX_DATA; nCntBullet++)
	{
		if (g_pTextureBullet[nCntBullet] != NULL)
		{
			g_pTextureBullet[nCntBullet]->Release();
			g_pTextureBullet[nCntBullet] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}
//◇==========================◇
//｜　　　 弾の更新処理　　 　｜
//◇==========================◇
void UpdateBullet(void)
{
	int nCntBullet;

	//テクスチャ座標の更新
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pVtx += 4)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			CollisionEnemy(&g_aBullet[nCntBullet]);
			CollisionBoss (&g_aBullet[nCntBullet]);
			CollisionSummon(&g_aBullet[nCntBullet]);

			//弾の位置更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;


			SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f,10,0);

			//頂点座標の更新

			/*pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot + (-D3DX_PI + g_fAngleBullit)) * g_fLengthBullit;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot + (-D3DX_PI + g_fAngleBullit)) * g_fLengthBullit;
			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot + (D3DX_PI - g_fAngleBullit)) * g_fLengthBullit;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot + (D3DX_PI - g_fAngleBullit)) * g_fLengthBullit;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot + (0 - g_fAngleBullit)) * g_fLengthBullit;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot + (0 - g_fAngleBullit)) * g_fLengthBullit;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot + (0 + g_fAngleBullit)) * g_fLengthBullit;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot + (0 + g_fAngleBullit)) * g_fLengthBullit;
			pVtx[3].pos.z = 0.0f;*/

			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - SIZE_ENEMY / 4;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - SIZE_ENEMY / 4;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + SIZE_ENEMY / 4;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - SIZE_ENEMY / 4;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - SIZE_ENEMY / 4; 
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + SIZE_ENEMY / 4;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + SIZE_ENEMY / 4;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + SIZE_ENEMY / 4;
			

			//SetParticle(g_aBullet[nCntBullet].pos,0);


			//弾の位置が範囲外に出た処理
			if (g_aBullet[nCntBullet].pos.x < 0 || g_aBullet[nCntBullet].pos.x > 1000)
			{//X座標が範囲外の場合
				g_aBullet[nCntBullet].bUse = false; //使用していない状態にする
			}
			if (g_aBullet[nCntBullet].pos.y < 5 || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT)
			{//Y座標が範囲外の場合
				g_aBullet[nCntBullet].bUse = false; //使用していない状態にする
			}

			//寿命のカウントダウン
			g_aBullet[nCntBullet].nLife--;
			if (g_aBullet[nCntBullet].nLife == 0)
			{
				//爆発の設定
				SetParticle(g_aBullet[nCntBullet].pos, 0, 5, 6.0f,0,0);

				//SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				/*g_aBullet[nCntBullet].pos*/
				g_aBullet[nCntBullet].bUse = false; //使用していない状態にする
			}
		}
	}

	//頂点バッファのアンロック
	g_pVtxBuffBullet->Unlock();
}
//◇==========================◇
//｜　　　 弾の描画処理　　 　｜
//◇==========================◇
void DrawBullet(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntBullet;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇
	for (nCntBullet = 0; nCntBullet < MAX_DATA; nCntBullet++)
	{
		pDevice->SetTexture(0, g_pTextureBullet[nCntBullet]);

		for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
		{
			if (g_aBullet[nCntBullet].bUse == true)
			{//弾が使用されている
				
			 //ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
			}
		}
	}
}

//◇==========================◇
//｜　　　 弾の設定処理　　 　｜
//◇==========================◇
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot,BULLETTYPE type)
{
	int nCntBullet;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffBullet->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].rot = rot;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = LIFE_DATA*2;
			//◇――――――――――――◇
			//｜ 　　頂点座標を設定　　 ｜
			//◇――――――――――――◇

				/*pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot + (-D3DX_PI + g_fAngleBullit)) * g_fLengthBullit;
				pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot + (-D3DX_PI + g_fAngleBullit)) * g_fLengthBullit;
				pVtx[0].pos.z = 0.0f;
				pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot + (D3DX_PI - g_fAngleBullit)) * g_fLengthBullit;
				pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot + (D3DX_PI - g_fAngleBullit)) * g_fLengthBullit;
				pVtx[1].pos.z = 0.0f;
				pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot + (0 - g_fAngleBullit)) * g_fLengthBullit;
				pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot + (0 - g_fAngleBullit)) * g_fLengthBullit;
				pVtx[2].pos.z = 0.0f;
				pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot + (0 + g_fAngleBullit)) * g_fLengthBullit;
				pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot + (0 + g_fAngleBullit)) * g_fLengthBullit;
				pVtx[3].pos.z = 0.0f;
*/
				pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - SIZE_ENEMY / 4;
				pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - SIZE_ENEMY / 4;
				pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + SIZE_ENEMY / 4;
				pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - SIZE_ENEMY / 4;
				pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - SIZE_ENEMY / 4;
				pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + SIZE_ENEMY / 4;
				pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + SIZE_ENEMY / 4;
				pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + SIZE_ENEMY / 4;
			g_aBullet[nCntBullet].bUse = true; //弾が使用している状態にする

			break;
		}
		pVtx += 4; //該当の位置まで進める
	}
	//頂点バッファのアンロック
	g_pVtxBuffBullet->Unlock();
}

//◇==========================◇
//｜　　　 弾の判定処理　　 　｜
//◇==========================◇

//敵の当たり判定
void CollisionEnemy(Bullet *pBullet)
{
	int nCntEnemy;

	Enemy *pEnemy = GetEnemy(); //敵の情報へのポインタ

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//敵が使用されている
			if (pBullet->pos.x >= pEnemy->pos.x - 32
				&& pBullet->pos.x <= pEnemy->pos.x + 32
				&& pBullet->pos.y >= pEnemy->pos.y - 32
				&& pBullet->pos.y <= pEnemy->pos.y + 32)
			{//敵と弾が当たった

				HitEnemy(nCntEnemy, 1);

				pBullet->bUse = false;

			}
		}
	}
}
//召喚の敵の当たり判定
void CollisionSummon(Bullet *pBullet)
{
	int nCntEnemy;

	Summon *pSummon = GetSummon(); //敵の情報へのポインタ

	for (nCntEnemy = 0; nCntEnemy < 60; nCntEnemy++, pSummon++)
	{
		if (pSummon->bUse == true)
		{//敵が使用されている

			if (pSummon->nType == 5)
			{
				if    (pBullet->pos.x >= pSummon->pos.x - 80
					&& pBullet->pos.x <= pSummon->pos.x + 80
					&& pBullet->pos.y >= pSummon->pos.y - 80
					&& pBullet->pos.y <= pSummon->pos.y + 80)
				{//敵と弾が当たった

					HitSummon(nCntEnemy, 1);

					pBullet->bUse = false;
				}
			}
			else
			{
				if (pBullet->pos.x >= pSummon->pos.x - 32
					&& pBullet->pos.x <= pSummon->pos.x + 32
					&& pBullet->pos.y >= pSummon->pos.y - 32
					&& pBullet->pos.y <= pSummon->pos.y + 32)
				{//敵と弾が当たった

					HitSummon(nCntEnemy, 1);

					pBullet->bUse = false;
				}
			}
		}
	}
}
//ボスの当たり判定
void CollisionBoss(Bullet *pBullet)
{
	int nCntEnemy;

	Boss *pBoss = GetBoss(); //敵の情報へのポインタ

	for (nCntEnemy = 0; nCntEnemy < 3; nCntEnemy++, pBoss++)
	{
		if (pBoss->bUse == true && pBoss->bHit == true)
		{//敵が使用されている
			if (pBullet->pos.x >= pBoss->pos.x - 80
				&& pBullet->pos.x <= pBoss->pos.x + 80
				&& pBullet->pos.y >= pBoss->pos.y - 80
				&& pBullet->pos.y <= pBoss->pos.y + 80)
			{//敵と弾が当たった

				HitBoss(nCntEnemy, 1);

				SetParticle(pBullet->pos, 0, 10, 10.0f, 0,0);

				pBullet->bUse = false;

			}
		}
	}
}
//◇=============◇
//｜　弾の所得　 ｜
//◇=============◇
Bullet *GetBullet(void)
{
	return &g_aBullet[0];//敵の情報の先頭アドレスを返す
}
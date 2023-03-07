//◇====================◇
//｜　　　　　　　　　　｜
//｜　　　敵の弾処理　  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "enemy_bullet.h"
#include "effect.h"
#include"particle.h"
#include"enemy.h"
#include"boss.h"
#include"Summon.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_BULLET (800)		//弾の最大数
#define BULLET_SPEED (4.0f)		//弾のスピード
#define MAX_ENEMY_BULLET (6)	//弾の種類
//◇==================◇
//｜　 構造体の定義 　｜
//◇==================◇
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	float rot;			//向き
	int nLife;			//寿命
	BULLETTYPE_E type;	//種類
	int stype;			//弾　画像
	int speed;			//弾スピード
	int time;			//時間（追跡時間）
	bool bUse;			//使用しているかどうか
}eBullet;

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void CollisionPlayer(eBullet *pBullet);

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DTEXTURE9		g_pTextureEnemyBullet[MAX_ENEMY_BULLET] = {};		 //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyBullet = NULL;		 //頂点バッファポインタ
static eBullet g_aBullet[MAX_BULLET];//弾の情報

//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【弾・処理】｜―――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇============================◇
//｜　　　 弾の初期化処理　　 　｜
//◇============================◇
void InitEnemyBullet(void)
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
		".\\data\\TEXTURE\\eff_shot_001_01_red.PNG",
		&g_pTextureEnemyBullet[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\eff_shot_001_01_blue.PNG",
		&g_pTextureEnemyBullet[1]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\eff_shot_001_01_green.PNG",
		&g_pTextureEnemyBullet[2]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\eff_shot_001_01_yellow.PNG",
		&g_pTextureEnemyBullet[3]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\eff_shot_001_01_white.PNG",
		&g_pTextureEnemyBullet[4]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\cleanness.png",
		&g_pTextureEnemyBullet[5]);

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemyBullet,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffEnemyBullet->Lock(0, 0, (void **)&pVtx, 0);

	nCntBullet = 0;

	//弾の情報の初期化処理
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = LIFE_DATA1/2;
		g_aBullet[nCntBullet].time = 10;
		g_aBullet[nCntBullet].bUse = false;


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
	g_pVtxBuffEnemyBullet->Unlock();
}
//◇==========================◇
//｜　　　 弾の終了処理　　 　｜
//◇==========================◇
void UninitEnemyBullet(void)
{
	int nCntBullet;

	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (nCntBullet = 0; nCntBullet < MAX_ENEMY_BULLET; nCntBullet++)
	{
		if (g_pTextureEnemyBullet[nCntBullet] != NULL)
		{
			g_pTextureEnemyBullet[nCntBullet]->Release();
			g_pTextureEnemyBullet[nCntBullet] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffEnemyBullet != NULL)
	{
		g_pVtxBuffEnemyBullet->Release();
		g_pVtxBuffEnemyBullet = NULL;
	}
}
//◇==========================◇
//｜　　　 弾の更新処理　　 　｜
//◇==========================◇
void UpdateEnemyBullet(void)
{
	int nCntBullet;
	//float fRotMove, fRotDest, fRotDiff, fRotTotal;//現在の移動方向・目標の移動方向・目標の移動方向までの差分・２つの角度の合計

	Enemy *pEnemy = GetEnemy();		//敵の情報へのポインタ
	Player *pPlayer = Getplayer();	//プレイヤーの情報へのポインタ

								   //テクスチャ座標の更新
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffEnemyBullet->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

		 //エネミーの弾
			CollisionPlayer(&g_aBullet[nCntBullet]);

			D3DXVECTOR3 movevec;

			switch (g_aBullet[nCntBullet].type)
			{
			case BULLETTYPE_E_LINE:

				//弾の位置更新
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				break;
			case BULLETTYPE_E_AIM:

				if (g_aBullet[nCntBullet].time >= 1 && pPlayer->bUse == true)
				{
					movevec = pPlayer->pos - g_aBullet[nCntBullet].pos;//弾からプレイヤーまでの方向ベクトルを求める

					D3DXVec3Normalize(&movevec, &movevec);				//ベクトルを正規化する(ベクトルを１にする) (正当化　, 正当化する前)

					movevec = movevec * BULLET_SPEED;					//正当化した物に移動量を与える

					g_aBullet[nCntBullet].pos += movevec;				//弾の位置を更新する
					g_aBullet[nCntBullet].move = movevec;
					g_aBullet[nCntBullet].time--;
				}
				else
				{
					//弾の位置更新
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}
				break;
			case BULLETTYPE_E_HOMING:

				//g_aBullet[nCntBullet].time--;
				//if (g_aBullet[nCntBullet].time >= 1)
				//{
				//	fRotMove = atan2f(g_aBullet[nCntBullet].pos.x,g_aBullet[nCntBullet].pos.y);	//現在の移動方向
				//	fRotDest = atan2f(g_aBullet[nCntBullet].pos.x - pPlayer->pos.x, g_aBullet[nCntBullet].pos.y - pPlayer->pos.y);	//目標の移動方向
				//	fRotDiff = fRotDest - fRotMove;																					//目標の移動方向までの差分
				//	if (fRotDiff >= D3DX_PI)
				//	{
				//		fRotDiff = fRotDiff - D3DX_PI * 2;
				//	}
				//	else if (fRotDiff <= -D3DX_PI)
				//	{
				//		fRotDiff = fRotDiff + D3DX_PI * 2;
				//	}
				//	fRotMove += fRotDiff * 0.5f;	//移動方向（角度）の補正
				//	if (fRotMove >= D3DX_PI)
				//	{
				//		fRotMove = fRotMove - D3DX_PI * 2;
				//	}
				//	else if (fRotMove <= -D3DX_PI)
				//	{
				//		fRotMove = fRotMove + D3DX_PI * 2;
				//	}
				//	g_aBullet[nCntBullet].move.x = sinf(fRotMove) * 5;
				//	g_aBullet[nCntBullet].move.y = cosf(fRotMove) * 5;
				//}
				//else if (g_aBullet[nCntBullet].time == 0)
				//{
				//	g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				//	g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				//}	


				if (g_aBullet[nCntBullet].time >= 1 && pPlayer->bUse == true)
				{
					movevec = pPlayer->pos - g_aBullet[nCntBullet].pos;//弾からプレイヤーまでの方向ベクトルを求める

					D3DXVec3Normalize(&movevec, &movevec);				//ベクトルを正規化する(ベクトルを１にする) (正当化　, 正当化する前)

					movevec = movevec * BULLET_SPEED;					//正当化した物に移動量を与える

					g_aBullet[nCntBullet].pos += movevec;				//弾の位置を更新する
					g_aBullet[nCntBullet].move = movevec;
					g_aBullet[nCntBullet].time--;
				}
				else
				{
					//弾の位置更新
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}
				break;
			case BULLETTYPE_E_3_SHOT:
				//弾の位置更新
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				break;
			case BULLETTYPE_E_MACHINEGUN:

				if (g_aBullet[nCntBullet].time >= 1 && pPlayer->bUse == true)
				{
					movevec = pPlayer->pos - g_aBullet[nCntBullet].pos;//弾からプレイヤーまでの方向ベクトルを求める

					D3DXVec3Normalize(&movevec, &movevec);				//ベクトルを正規化する(ベクトルを１にする) (正当化　, 正当化する前)

					movevec = movevec * BULLET_SPEED * 2;				//正当化した物に移動量を与える

					g_aBullet[nCntBullet].pos += movevec;				//弾の位置を更新する
					g_aBullet[nCntBullet].move = movevec;
					g_aBullet[nCntBullet].stype = 5;
					g_aBullet[nCntBullet].time--;
				}
				else
				{
					//弾の位置更新
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}
				break;
			case BULLETTYPE_E_3_SHOT2:
				g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
				g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				break;
			case BULLETTYPE_E_CHARGE_SHOT:
				if (g_aBullet[nCntBullet].time >= 1 && pPlayer->bUse == true)
				{
					movevec = pPlayer->pos - g_aBullet[nCntBullet].pos;//弾からプレイヤーまでの方向ベクトルを求める

					D3DXVec3Normalize(&movevec, &movevec);				//ベクトルを正規化する(ベクトルを１にする) (正当化　, 正当化する前)

					movevec = movevec * BULLET_SPEED * 2;				//正当化した物に移動量を与える

					g_aBullet[nCntBullet].pos += movevec;				//弾の位置を更新する
					g_aBullet[nCntBullet].move = movevec;
					g_aBullet[nCntBullet].stype = 5;
					g_aBullet[nCntBullet].time--;
				}
				else
				{
					//弾の位置更新
					g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
					g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
				}
				break;
			}
			
			//頂点座標の更新
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - 24 / 2;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - 24 / 2;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + 24 / 2;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - 24 / 2;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - 24 / 2;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + 24 / 2;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + 24 / 2;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + 24 / 2;

			//弾の軌跡を設定
			switch (g_aBullet[nCntBullet].type)
			{
			case BULLETTYPE_E_LINE:
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 12.0f, 10, 0);
				break;
			case BULLETTYPE_E_AIM:
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 12.0f, 10, 0);
				break;
			case BULLETTYPE_E_HOMING:
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 12.0f, 10, 0);
				break;
			case BULLETTYPE_E_3_SHOT:
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 12.0f, 10, 0);
				break;
			case BULLETTYPE_E_MACHINEGUN:
				SetParticle(g_aBullet[nCntBullet].pos, 4, 2, 5.0f, 1, 4);
				break;
			case BULLETTYPE_E_3_SHOT2:
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 12.0f, 10, 0);
				break;
			case BULLETTYPE_E_CHARGE_SHOT:
				SetParticle(g_aBullet[nCntBullet].pos, 4, 2, 5.0f, 1, 4);
				break;
			}
			if (g_aBullet[nCntBullet].pos.x < 0 || g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH - 310)
			{//X座標が範囲外の場合
				g_aBullet[nCntBullet].bUse = false; //使用していない状態にする
				switch (g_aBullet[nCntBullet].type)
				{
				case 0:
					//爆発の設定
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 0, 0);
					break;
				case 1:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 1, 0);
					break;
				case 2:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				case 3:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				case BULLETTYPE_E_MACHINEGUN:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				case BULLETTYPE_E_3_SHOT2:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				}
			}
			if (g_aBullet[nCntBullet].pos.y < 0 || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT)
			{//Y座標が範囲外の場合
				g_aBullet[nCntBullet].bUse = false; //使用していない状態にする
				switch (g_aBullet[nCntBullet].type)
				{
				case 0:
					//爆発の設定
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 0, 0);
					break;
				case 1:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 1, 0);
					break;
				case 2:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				case 3:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				case BULLETTYPE_E_MACHINEGUN:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				case BULLETTYPE_E_3_SHOT2:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				}
			}

			//寿命のカウントダウン
			g_aBullet[nCntBullet].nLife--;
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				switch (g_aBullet[nCntBullet].type)
				{
				case 0:
					//爆発の設定
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 0,0);
					break;
				case 1:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 1,0);
					break;
				case 2:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2,0);
					break;
				case 3:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				case BULLETTYPE_E_MACHINEGUN:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				case BULLETTYPE_E_3_SHOT2:
					SetParticle(g_aBullet[nCntBullet].pos, 0, 2, 5.0f, 2, 0);
					break;
				}
				//SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				g_aBullet[nCntBullet].bUse = false; //使用していない状態にする
			}
		}
		pVtx += 4;
	}

	//頂点バッファのアンロック
	g_pVtxBuffEnemyBullet->Unlock();
}
//◇==========================◇
//｜　　　 弾の描画処理　　 　｜
//◇==========================◇
void DrawEnemyBullet(void)
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
	pDevice->SetStreamSource(0, g_pVtxBuffEnemyBullet, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている
			pDevice->SetTexture(0, g_pTextureEnemyBullet[g_aBullet[nCntBullet].stype]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//◇==========================◇
//｜　　　 弾の設定処理　　 　｜
//◇==========================◇
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE_E type,int stype)
{
	int nCntBullet;

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffEnemyBullet->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].stype = stype;

			if (g_aBullet[nCntBullet].type == BULLETTYPE_E_MACHINEGUN)
			{
				g_aBullet[nCntBullet].nLife = LIFE_DATA1 * 4;
			}
			else
			{
				g_aBullet[nCntBullet].nLife = LIFE_DATA1 * 2;
			}

			//エイムとホーミングの追跡時間を個別に設定
			if (g_aBullet[nCntBullet].type == 1)
			{
				g_aBullet[nCntBullet].time = 1;
			}
			else if (g_aBullet[nCntBullet].type == 2)
			{
				g_aBullet[nCntBullet].time = 30;
			}
			else if (g_aBullet[nCntBullet].type == 4)
			{
				g_aBullet[nCntBullet].time = 1;
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_E_MACHINEGUN)
			{
				g_aBullet[nCntBullet].time = 1;
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_E_CHARGE_SHOT)
			{
				g_aBullet[nCntBullet].time = 1;
			}
			
			//◇――――――――――――◇
			//｜ 　　頂点座標を設定　　 ｜
			//◇――――――――――――◇

			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - SIZE_ENEMY / 3;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - SIZE_ENEMY / 3;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + SIZE_ENEMY / 3;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - SIZE_ENEMY / 3;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - SIZE_ENEMY / 3;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + SIZE_ENEMY / 3;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + SIZE_ENEMY / 3;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + SIZE_ENEMY / 3;
			g_aBullet[nCntBullet].bUse = true; //弾が使用している状態にする

			break;
		}
		pVtx += 4; //該当の位置まで進める
	}
	//頂点バッファのアンロック
	g_pVtxBuffEnemyBullet->Unlock();
}
//◇==========================◇
//｜　　　 弾の判定処理　　 　｜
//◇==========================◇

//プレイヤー当たり判定
void CollisionPlayer(eBullet *pBullet)
{
	int nCntEnemy;

	Player *pPlayer = Getplayer(); //プレイヤーの情報へのポインタ

	for (nCntEnemy = 0; nCntEnemy < MAX_BULLET; nCntEnemy++)
	{

		if (pPlayer->bUse == true && pBullet->bUse == true)
		{
			if (pBullet->pos.x >= pPlayer->pos.x - 24 &&
				pBullet->pos.x <= pPlayer->pos.x + 24 &&
				pBullet->pos.y >= pPlayer->pos.y - 24 &&
				pBullet->pos.y <= pPlayer->pos.y + 24)
			{//プレイヤーと弾が当たった

				HitPlayer(nCntEnemy, 1);

				SetParticle(pBullet->pos, 0, 10, 10.0f, 0, 0);

				pBullet->bUse = false;

			}
		}
	}
}

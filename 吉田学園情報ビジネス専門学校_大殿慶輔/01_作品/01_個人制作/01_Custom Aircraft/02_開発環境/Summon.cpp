//◇====================◇
//｜　　　　　　　　　　｜
//｜　敵の処理          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"game.h"
#include"Summon.h"
#include"bullet.h"
#include"score.h"
#include"sound.h"
#include"player.h"
#include"enemy_bullet.h"
#include"particle.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_ENEMY	  (7) //敵の種類
#define MAX_SUMMON	(128) //召喚数
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void CollisionSummon(Summon *pSummon);
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DTEXTURE9 g_pTextureSummon[NUM_ENEMY] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSummon = NULL;		//頂点バッファポインタ
Summon g_aSummon[MAX_SUMMON];							//敵の情報

int g_nCount10;											//一定時間
int g_nNumSummon;										//現在いる敵の数
int g_nTotalSummon;										//敵の総数
int g_CntTime10;
int p_summon;											//召喚種類
int stand_time;											//待機時間

int b_summon;
int nCntCharge1;										//チャージ時間
int nCharge1;											//チャージ率
int nCntCharge2;										//チャージ時間2
int nCharge2;											//チャージ率2
int nCntChargeShot;										//チャージショットのカウント
int nCntMachineGun1;									//機関銃のカウント
int m_boss;

bool bswich;
bool bswich1;
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【敵・処理】｜――――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇============================◇
//｜　　　 敵の初期化処理　　 　｜
//◇============================◇
void InitSummon(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEnemy;
	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\ghost.png",
		&g_pTextureSummon[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy_a.png",
		&g_pTextureSummon[1]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy2.png",
		&g_pTextureSummon[2]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy3b.png",
		&g_pTextureSummon[3]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy4.png",
		&g_pTextureSummon[4]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy5.png",
		&g_pTextureSummon[5]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy6.png",
		&g_pTextureSummon[6]);

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SUMMON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSummon,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffSummon->Lock(0, 0, (void **)&pVtx, 0);

	//敵の情報の初期化処理
	for (nCntEnemy = 0; nCntEnemy < MAX_SUMMON; nCntEnemy++)
	{
		g_aSummon[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSummon[nCntEnemy].move.x = 0.3f;
		g_aSummon[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きを初期化する(Z値を使用)
		g_aSummon[nCntEnemy].nBullet = 0;
		g_aSummon[nCntEnemy].nLife = 1;
		g_aSummon[nCntEnemy].time = 1;
		g_aSummon[nCntEnemy].Tracking = 90;
		g_aSummon[nCntEnemy].nCntMove = 0;
		g_aSummon[nCntEnemy].nCntStop = 0;
		g_aSummon[nCntEnemy].bUse = false;
		g_aSummon[nCntEnemy].nStop = false;
		g_aSummon[nCntEnemy].Reversal = false;
		g_aSummon[nCntEnemy].m_switch = false;
		g_aSummon[nCntEnemy].bHit = false;
		g_aSummon[nCntEnemy].bDrw = false;
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(16.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(16.0f, 50.0f, 0.0f);
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

	p_summon = 10;
	b_summon = 0;
	g_nNumSummon = 0;	//現在いる敵の数
	g_nTotalSummon = 0;	//敵の総数
	g_CntTime10 = 0;
	stand_time = 62;

	nCntMachineGun1 = 0;
	nCntCharge1 = 361;
	nCharge1 = 0;
	nCntCharge2 = 300;
	nCharge2 = 0;
	nCntChargeShot = 0;

	m_boss = 0;
	bswich = true;
	bswich1 = true;
	//頂点バッファのアンロック
	g_pVtxBuffSummon->Unlock();

	//敵情報を読み込む
	LoadSummon();
}
//◇==========================◇
//｜　　　 敵の終了処理　　 　｜
//◇==========================◇
void UninitSummon(void)
{
	int nCntEnemy;
	//サウンドの停止
	StopSound();
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_pTextureSummon[nCntEnemy] != NULL)
		{
			g_pTextureSummon[nCntEnemy]->Release();
			g_pTextureSummon[nCntEnemy] = NULL;
		}
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffSummon != NULL)
	{
		g_pVtxBuffSummon->Release();
		g_pVtxBuffSummon = NULL;
	}
}
//◇==========================◇
//｜　　　 敵の更新処理　　 　｜
//◇==========================◇
void UpdateSummon(void)
{
	int nCntEnemy;		//敵カウンタ
	Boss *pBoss = GetBoss(); //敵の情報へのポインタ

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffSummon->Lock(0, 0, (void **)&pVtx, 0);

	g_nCount10++;
	for (nCntEnemy = 0; nCntEnemy < MAX_SUMMON; nCntEnemy++, pVtx += 4)
	{
		//状態遷移処理
		if (g_aSummon[nCntEnemy].bUse == true)
		{
			CollisionSummon(&g_aSummon[nCntEnemy]);

			switch (g_aSummon[nCntEnemy].m_pattern)
			{
				//=============================
				//上から出現する移動パターン
				//=============================

			case 0:
				if (g_aSummon[nCntEnemy].nStop == false)
				{
					if (g_aSummon[nCntEnemy].Reversal == false)
					{
						g_aSummon[nCntEnemy].pos.x -= 2;
						if (g_aSummon[nCntEnemy].pos.x < 200)
						{
							g_aSummon[nCntEnemy].pos.x = 200;
							g_aSummon[nCntEnemy].nStop = true;
						}
					}
					else if (g_aSummon[nCntEnemy].Reversal == true)
					{
						g_aSummon[nCntEnemy].pos.x += 2;
						if (g_aSummon[nCntEnemy].pos.x > 800)
						{
							g_aSummon[nCntEnemy].pos.x = 800;
							g_aSummon[nCntEnemy].nStop = true;
						}
					}
				}
				else if (g_aSummon[nCntEnemy].nStop == true)
				{
					g_aSummon[nCntEnemy].nCntStop++;
					if (g_aSummon[nCntEnemy].nCntStop == 10)
					{
						if (g_aSummon[nCntEnemy].Reversal == false)
						{
							g_aSummon[nCntEnemy].Reversal = true;
						}
						else if (g_aSummon[nCntEnemy].Reversal == true)
						{
							g_aSummon[nCntEnemy].Reversal = false;
						}
						g_aSummon[nCntEnemy].nStop = false;
						g_aSummon[nCntEnemy].nCntStop = 0;
					}
				}
				break;
			case 1:
				if (g_aSummon[nCntEnemy].nStop == false)
				{
					if (g_aSummon[nCntEnemy].Reversal == false)
					{
						g_aSummon[nCntEnemy].pos.y -= 2;
						if (g_aSummon[nCntEnemy].pos.y < 100)
						{
							g_aSummon[nCntEnemy].pos.y = 102;
							g_aSummon[nCntEnemy].nStop = true;
						}
					}
					else if (g_aSummon[nCntEnemy].Reversal == true)
					{
						g_aSummon[nCntEnemy].pos.y += 2;
						if (g_aSummon[nCntEnemy].pos.y > 600)
						{
							g_aSummon[nCntEnemy].pos.y = 598;
							g_aSummon[nCntEnemy].nStop = true;
						}
					}
				}
				else if (g_aSummon[nCntEnemy].nStop == true)
				{
					g_aSummon[nCntEnemy].nCntStop++;
					if (g_aSummon[nCntEnemy].nCntStop == 10)
					{
						if (g_aSummon[nCntEnemy].Reversal == false)
						{
							g_aSummon[nCntEnemy].Reversal = true;
						}
						else if (g_aSummon[nCntEnemy].Reversal == true)
						{
							g_aSummon[nCntEnemy].Reversal = false;
						}
						g_aSummon[nCntEnemy].nStop = false;
						g_aSummon[nCntEnemy].nCntStop = 0;
					}
				}
					break;
			case 2:
				if (g_aSummon[nCntEnemy].Reversal == false)
				{
					g_aSummon[nCntEnemy].pos.x -= 2;
					if (g_aSummon[nCntEnemy].pos.x< 100)
					{
						g_aSummon[nCntEnemy].pos.x= 101;
						g_aSummon[nCntEnemy].Reversal = true;
					}
				}
				else if (g_aSummon[nCntEnemy].Reversal == true)
				{
					g_aSummon[nCntEnemy].pos.x += 2;
					if (g_aSummon[nCntEnemy].pos.x > 300)
					{
						g_aSummon[nCntEnemy].pos.x = 299;
						g_aSummon[nCntEnemy].Reversal = false;
					}
				}
				break;
			case 3:
				if (g_aSummon[nCntEnemy].Reversal == false)
				{
					g_aSummon[nCntEnemy].pos.x -= 2;
					if (g_aSummon[nCntEnemy].pos.x< 700)
					{
						g_aSummon[nCntEnemy].pos.x = 701;
						g_aSummon[nCntEnemy].Reversal = true;
					}
				}
				else if (g_aSummon[nCntEnemy].Reversal == true)
				{
					g_aSummon[nCntEnemy].pos.x += 2;
					if (g_aSummon[nCntEnemy].pos.x > 900)
					{
						g_aSummon[nCntEnemy].pos.x = 899;
						g_aSummon[nCntEnemy].Reversal = false;
					}
				}
				break;
			case 99:
				g_aSummon[nCntEnemy].pos.x += 0;
				g_aSummon[nCntEnemy].pos.y += 0;
				break;
			}

			//画面からはみ出したら敵を非表示にする
			if (g_aSummon[nCntEnemy].pos.x < -30 || g_aSummon[nCntEnemy].pos.x > SCREEN_WIDTH - 220)
			{
				//X座標が範囲外の場合
				g_aSummon[nCntEnemy].bDrw = false; //使用していない状態にする
				g_aSummon[nCntEnemy].bHit = false;
				g_aSummon[nCntEnemy].bUse = false;
			}
			if (g_aSummon[nCntEnemy].pos.y < -45 || g_aSummon[nCntEnemy].pos.y > SCREEN_HEIGHT)
			{
				//Y座標が範囲外の場合
				g_aSummon[nCntEnemy].bDrw = false; //使用していない状態にする
				g_aSummon[nCntEnemy].bHit = false;
				g_aSummon[nCntEnemy].bUse = false;
			}

			switch (g_aSummon[nCntEnemy].state)
			{
			case SUMMONSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGE:
				g_aSummon[nCntEnemy].nCounterState--;

				if (g_aSummon[nCntEnemy].nCounterState <= 0)
				{
					g_aSummon[nCntEnemy].state = SUMMONSTATE_NORMAL;

					if (g_aSummon[16].bHit == true && g_aSummon[17].bHit == true)
					{
						g_aSummon[16].bDrw = true;
						g_aSummon[17].bDrw = true;
					}
					else
					{
						g_aSummon[nCntEnemy].bDrw = true;
					}
					//◇――――――――――――◇
					//｜　　　 頂点カラー 　　　｜
					//◇――――――――――――◇
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//ポリゴンを通常色に戻す
				}
				else
				{
					if(g_aSummon[16].bHit == true && g_aSummon[17].bHit == true)
					{
						g_aSummon[16].bDrw = g_aSummon[16].bDrw ? false : true;
						g_aSummon[17].bDrw = g_aSummon[17].bDrw ? false : true;
					}
					else
					{
						g_aSummon[nCntEnemy].bDrw = g_aSummon[nCntEnemy].bDrw ? false : true;
					}
				}
				break;
			}


			//g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

			if (g_aSummon[nCntEnemy].nType == 5)
			{
				//頂点座標の設定
				pVtx[0].pos.x = g_aSummon[nCntEnemy].pos.x - SIZE_BOSS;
				pVtx[0].pos.y = g_aSummon[nCntEnemy].pos.y - SIZE_BOSS;
				pVtx[0].pos.z = 0;
				pVtx[1].pos.x = g_aSummon[nCntEnemy].pos.x + SIZE_BOSS;
				pVtx[1].pos.y = g_aSummon[nCntEnemy].pos.y - SIZE_BOSS;
				pVtx[1].pos.z = 0;
				pVtx[2].pos.x = g_aSummon[nCntEnemy].pos.x - SIZE_BOSS;
				pVtx[2].pos.y = g_aSummon[nCntEnemy].pos.y + SIZE_BOSS;
				pVtx[2].pos.z = 0;
				pVtx[3].pos.x = g_aSummon[nCntEnemy].pos.x + SIZE_BOSS;
				pVtx[3].pos.y = g_aSummon[nCntEnemy].pos.y + SIZE_BOSS;
				pVtx[3].pos.z = 0;
			}
			else if (g_aSummon[nCntEnemy].nType == 6)
			{
				pVtx[0].pos.x = g_aSummon[nCntEnemy].pos.x - SIZE_ENEMY;
				pVtx[0].pos.y = g_aSummon[nCntEnemy].pos.y - SIZE_ENEMY;
				pVtx[0].pos.z = 0;
				pVtx[1].pos.x = g_aSummon[nCntEnemy].pos.x + SIZE_ENEMY;
				pVtx[1].pos.y = g_aSummon[nCntEnemy].pos.y - SIZE_ENEMY;
				pVtx[1].pos.z = 0;
				pVtx[2].pos.x = g_aSummon[nCntEnemy].pos.x - SIZE_ENEMY;
				pVtx[2].pos.y = g_aSummon[nCntEnemy].pos.y + SIZE_ENEMY;
				pVtx[2].pos.z = 0;
				pVtx[3].pos.x = g_aSummon[nCntEnemy].pos.x + SIZE_ENEMY;
				pVtx[3].pos.y = g_aSummon[nCntEnemy].pos.y + SIZE_ENEMY;
				pVtx[3].pos.z = 0;
			}
			else
			{
				//頂点座標の設定
				pVtx[0].pos.x = g_aSummon[nCntEnemy].pos.x - SIZE_ENEMY / 2;
				pVtx[0].pos.y = g_aSummon[nCntEnemy].pos.y - SIZE_ENEMY / 2;
				pVtx[0].pos.z = 0;
				pVtx[1].pos.x = g_aSummon[nCntEnemy].pos.x + SIZE_ENEMY / 2;
				pVtx[1].pos.y = g_aSummon[nCntEnemy].pos.y - SIZE_ENEMY / 2;
				pVtx[1].pos.z = 0;
				pVtx[2].pos.x = g_aSummon[nCntEnemy].pos.x - SIZE_ENEMY / 2;
				pVtx[2].pos.y = g_aSummon[nCntEnemy].pos.y + SIZE_ENEMY / 2;
				pVtx[2].pos.z = 0;
				pVtx[3].pos.x = g_aSummon[nCntEnemy].pos.x + SIZE_ENEMY / 2;
				pVtx[3].pos.y = g_aSummon[nCntEnemy].pos.y + SIZE_ENEMY / 2;
				pVtx[3].pos.z = 0;
			}

			if (g_aSummon[nCntEnemy].bHit == true)
			{
				switch (g_aSummon[nCntEnemy].s_pattern)
				{
				case BULLETTYPE_E_LINE:
					g_aSummon[nCntEnemy].nBullet++;//カウンターを加算
					if ((g_aSummon[nCntEnemy].nBullet % 60) == 0)
					{
						SetEnemyBullet(g_aSummon[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_LINE, g_aSummon[nCntEnemy].sType);
					}
					break;

				case BULLETTYPE_E_AIM:
					g_aSummon[nCntEnemy].nBullet++;//カウンターを加算
					if ((g_aSummon[nCntEnemy].nBullet % 90) == 0)
					{
						SetEnemyBullet(g_aSummon[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_AIM, g_aSummon[nCntEnemy].sType);
					}
					break;

				case BULLETTYPE_E_HOMING:
					g_aSummon[nCntEnemy].nBullet++;//カウンターを加算
					if ((g_aSummon[nCntEnemy].nBullet % 90) == 0)
					{
						SetEnemyBullet(g_aSummon[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_HOMING, g_aSummon[nCntEnemy].sType);
					}
					break;
				case BULLETTYPE_E_CHARGE:

					//チャージ状態ならカウントを減らす
					if (nCntCharge1 <= 0)
					{
						g_aSummon[nCntEnemy].s_pattern = BULLETTYPE_E_MACHINEGUN;
						nCntMachineGun1 = 0;
					}
					else if (nCntCharge1 <= 60)
					{
						nCntCharge1--;
						nCharge1++;
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 5);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 32);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 31);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 3);
					}
					else if (nCntCharge1 <= 120 && nCntCharge1 >= 61)
					{
						nCntCharge1--;
						nCharge1++;
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 5);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 31);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 3);

					}
					else if (nCntCharge1 <= 300 && nCntCharge1 >= 121)
					{
						nCntCharge1--;
						nCharge1++;
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 5);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 3);
					}
					else
					{
						nCntCharge1--;
					}

					break;
				case BULLETTYPE_E_MACHINEGUN:

					if (nCntMachineGun1 <= 310)
					{
						nCntMachineGun1++; //カウントを加算
						nCharge1--;

						//30回に一回発射。310までなので10発発射。
						if ((nCntMachineGun1 % 30) == 0)
						{
							if (bswich == false)
							{
								bswich = true;
								SetEnemyBullet(D3DXVECTOR3(g_aSummon[16].pos.x + 0.0f, g_aSummon[16].pos.y + 60.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_MACHINEGUN, g_aSummon[16].sType);
							}
							else if (bswich == true)
							{
								bswich = false;
								SetEnemyBullet(D3DXVECTOR3(g_aSummon[17].pos.x + 0.0f, g_aSummon[17].pos.y + 60.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_MACHINEGUN, g_aSummon[17].sType);
							}
						}
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 5);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 3);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 31);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 7, 32);
					}
					else
					{
						nCntCharge1 = 461;
						nCharge1 = 0;
						g_aSummon[nCntEnemy].s_pattern = BULLETTYPE_E_CHARGE;
					}
					break;
				case BULLETTYPE_E_CHARGE1:
					if (nCntCharge2 <= 0)
					{
						g_aSummon[nCntEnemy].s_pattern = BULLETTYPE_E_CHARGE_SHOT;
						nCntChargeShot = 0;
					}
					else if (nCntCharge2 <= 30)
					{
						nCntCharge2--;
						nCharge2++;

						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 51);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 31);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 32);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 3);
					}
					else if (nCntCharge2 <= 60 && nCntCharge2 >= 31)
					{
						nCntCharge2--;
						nCharge2++;

						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 51);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 31);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 3);
					}
					else if(nCntCharge2 <= 180 && nCntCharge2 >= 61)
					{
						nCntCharge2--;
						nCharge2++;
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 51);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 3);
					}
					else
					{
						nCntCharge2--;
					}
					break;
				case BULLETTYPE_E_CHARGE_SHOT:

					//チャージ状態ならカウントを減らす
					if (nCntChargeShot <= 120)
					{
						nCharge2--;
						nCntChargeShot++;
						if ((nCntChargeShot % 30) == 0)
						{
							if (bswich1 == false)
							{
								if (g_aSummon[21].bHit == true)
								{
									bswich1 = true;
								}
								SetEnemyBullet(D3DXVECTOR3(g_aSummon[20].pos.x + 0.0f, g_aSummon[20].pos.y + 60.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_CHARGE_SHOT, g_aSummon[20].sType);
							}
							else if (bswich1 == true)
							{
								if (g_aSummon[20].bHit == true)
								{
									bswich1 = false;
								}
								SetEnemyBullet(D3DXVECTOR3(g_aSummon[21].pos.x + 0.0f, g_aSummon[21].pos.y + 60.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_CHARGE_SHOT, g_aSummon[21].sType);
							}
						}
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 51);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 3);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 31);
						SetParticle(D3DXVECTOR3(g_aSummon[nCntEnemy].pos.x + 0.0f, g_aSummon[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 1, 32);

					}
					else
					{
						nCntCharge2 = 300;
						nCharge1 = 0;
						g_aSummon[nCntEnemy].s_pattern = BULLETTYPE_E_CHARGE1;
					}
					break;
				case 99:
					break;
				}
			}
			if (g_aSummon[nCntEnemy].bTime == true)
			{
				stand_time--;
				if (stand_time <= 0)
				{
					if (p_summon == 0)
					{
						g_aSummon[0].bHit = true; g_aSummon[0].bTime = false;
						g_aSummon[1].bHit = true; g_aSummon[1].bTime = false;
						g_aSummon[2].bHit = true; g_aSummon[2].bTime = false;
						g_aSummon[3].bHit = true; g_aSummon[3].bTime = false;

						g_aSummon[0].m_pattern = 1;
						g_aSummon[1].m_pattern = 1;
						g_aSummon[2].m_pattern = 0;
						g_aSummon[3].m_pattern = 0;

					}
					else if (p_summon == 1)
					{
						g_aSummon[7]. bHit = true; g_aSummon[7]. bTime = false;
						g_aSummon[8]. bHit = true; g_aSummon[8]. bTime = false;
						g_aSummon[9]. bHit = true; g_aSummon[9]. bTime = false;
						g_aSummon[10].bHit = true; g_aSummon[10].bTime = false;
						g_aSummon[11].bHit = true; g_aSummon[11].bTime = false;
						g_aSummon[12].bHit = true; g_aSummon[12].bTime = false;
						g_aSummon[13].bHit = true; g_aSummon[13].bTime = false;
						g_aSummon[14].bHit = true; g_aSummon[14].bTime = false;
						g_aSummon[15].bHit = true; g_aSummon[15].bTime = false;


						g_aSummon[14].m_pattern = 2;
						g_aSummon[15].m_pattern = 3;
					}
					else if (p_summon == 2)
					{
						g_aSummon[7].bHit = true; g_aSummon[7].bTime = false;g_aSummon[7].nLife = 15;
						g_aSummon[8].bHit = true; g_aSummon[8].bTime = false;g_aSummon[8].nLife = 15;
						g_aSummon[9].bHit = true; g_aSummon[9].bTime = false;g_aSummon[9].nLife = 15;
						g_aSummon[18].bHit = true; g_aSummon[18].bTime = false;
						g_aSummon[19].bHit = true; g_aSummon[19].bTime = false;
						g_aSummon[20].bHit = true; g_aSummon[20].bTime = false;
						g_aSummon[21].bHit = true; g_aSummon[21].bTime = false;
					}
					else if (p_summon == 3)
					{
						g_aSummon[16].bHit = true; g_aSummon[16].bTime = false;
						g_aSummon[17].bHit = true; g_aSummon[17].bTime = false;

						nCntCharge1 = 541;
						nCharge1 = 0;
					}

				}
				else if (stand_time <= 10)
				{
					if (p_summon == 0)
					{
						g_aSummon[0].bDrw = true;
						g_aSummon[1].bDrw = true;
						g_aSummon[2].bDrw = true;
						g_aSummon[3].bDrw = true;
					}
					else if (p_summon == 1)
					{
						g_aSummon[7]. bDrw = true; 
						g_aSummon[8]. bDrw = true; 
						g_aSummon[9]. bDrw = true; 
						g_aSummon[10].bDrw = true;
						g_aSummon[11].bDrw = true;
						g_aSummon[12].bDrw = true;
						g_aSummon[13].bDrw = true;
						g_aSummon[14].bDrw = true;
						g_aSummon[15].bDrw = true;
					}
					else if (p_summon == 2)
					{
						g_aSummon[7].bDrw = true;
						g_aSummon[8].bDrw = true;
						g_aSummon[9].bDrw = true;
						g_aSummon[18].bDrw = true;
						g_aSummon[19].bDrw = true;
						g_aSummon[20].bDrw = true;
						g_aSummon[21].bDrw = true;
					}
					else if (p_summon == 3)
					{
						g_aSummon[16].bDrw = true;
						g_aSummon[17].bDrw = true;
					}
				}
				else if (stand_time == 60)
				{
					if (p_summon == 0)
					{
						SetParticle(g_aSummon[0].pos, 1, 30, 90.0f, 1, 0);
						SetParticle(g_aSummon[1].pos, 1, 30, 90.0f, 1, 0);
						SetParticle(g_aSummon[2].pos, 1, 30, 90.0f, 1, 0);
						SetParticle(g_aSummon[3].pos, 1, 30, 90.0f, 1, 0);
					}
					else if (p_summon == 1)
					{
						SetParticle(g_aSummon[7]. pos, 1, 30, 90.0f, 5, 0);
						SetParticle(g_aSummon[8]. pos, 1, 30, 90.0f, 5, 0);
						SetParticle(g_aSummon[9]. pos, 1, 30, 90.0f, 5, 0);
						SetParticle(g_aSummon[10].pos, 1, 30, 90.0f, 5, 0);
						SetParticle(g_aSummon[11].pos, 1, 30, 90.0f, 5, 0);
						SetParticle(g_aSummon[12].pos, 1, 30, 90.0f, 1, 0);
						SetParticle(g_aSummon[13].pos, 1, 30, 90.0f, 1, 0);
						SetParticle(g_aSummon[14].pos, 1, 30, 90.0f, 1, 0);
						SetParticle(g_aSummon[15].pos, 1, 30, 90.0f, 1, 0);
					}
					else if (p_summon == 2)
					{
						SetParticle(g_aSummon[7].pos, 1, 30, 90.0f, 5, 0);
						SetParticle(g_aSummon[8].pos, 1, 30, 90.0f, 5, 0);
						SetParticle(g_aSummon[9].pos, 1, 30, 90.0f, 5, 0);
						SetParticle(g_aSummon[18].pos, 1, 30, 90.0f, 2, 0);
						SetParticle(g_aSummon[19].pos, 1, 30, 90.0f, 2, 0);
						SetParticle(g_aSummon[20].pos, 1, 30, 90.0f, 1, 0);
						SetParticle(g_aSummon[21].pos, 1, 30, 90.0f, 1, 0);
					}
					else if (p_summon == 3)
					{
						SetParticle(g_aSummon[16].pos, 1, 30, 90.0f, 4, 0);
						SetParticle(g_aSummon[17].pos, 1, 30, 90.0f, 4, 0);
					}
				}
		}

			if (b_summon >= 2 && b_summon <= 6)
			{
				if (g_aSummon[10].bUse == true)
				{
					SetParticle(g_aSummon[10].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[10].bHit = false; g_aSummon[10].bTime = false; g_aSummon[10].bDrw = false; g_aSummon[10].bUse = false;
					AddScore(g_aSummon[10].score * (int)1.5f);
				}
				if (g_aSummon[11].bUse == true)
				{
					SetParticle(g_aSummon[11].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[11].bHit = false; g_aSummon[11].bTime = false; g_aSummon[11].bDrw = false; g_aSummon[11].bUse = false;
					AddScore(g_aSummon[11].score * (int)1.5f);
				}
				if (g_aSummon[12].bUse == true)
				{
					SetParticle(g_aSummon[12].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[12].bHit = false; g_aSummon[12].bTime = false; g_aSummon[12].bDrw = false; g_aSummon[12].bUse = false;
					AddScore(g_aSummon[12].score * (int)1.5f);
				}
				if (g_aSummon[13].bUse == true)
				{
					SetParticle(g_aSummon[13].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[13].bHit = false; g_aSummon[13].bTime = false; g_aSummon[13].bDrw = false; g_aSummon[13].bUse = false;
					AddScore(g_aSummon[13].score * (int)1.5f);
				}
				if (g_aSummon[14].bUse == true)
				{
					SetParticle(g_aSummon[14].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[14].bHit = false; g_aSummon[14].bTime = false; g_aSummon[14].bDrw = false; g_aSummon[14].bUse = false;
					AddScore(g_aSummon[14].score * (int)1.5f);
				}
				if (g_aSummon[15].bUse == true)
				{
					SetParticle(g_aSummon[15].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[15].bHit = false; g_aSummon[15].bTime = false; g_aSummon[15].bDrw = false; g_aSummon[15].bUse = false;
					AddScore(g_aSummon[15].score * (int)1.5f);
				}

				if (g_aSummon[10].bUse == false && g_aSummon[11].bUse == false && 
					g_aSummon[12].bUse == false && g_aSummon[13].bUse == false && 
					g_aSummon[14].bUse == false && g_aSummon[15].bUse == false)
				{
					pBoss[2].nReset = 1;
					EnemySummon(2);
					b_summon = 7;
				}
			}
			else if (b_summon >= 9 && b_summon <= 14)
			{
				if (g_aSummon[7].bUse == true)
				{
					SetParticle(g_aSummon[7].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[7].bHit = false; g_aSummon[7].bTime = false; g_aSummon[7].bDrw = false; g_aSummon[7].bUse = false;
					AddScore(g_aSummon[7].score * (int)1.5f);
				}
				if (g_aSummon[8].bUse == true)
				{
					SetParticle(g_aSummon[8].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[8].bHit = false; g_aSummon[8].bTime = false; g_aSummon[8].bDrw = false; g_aSummon[8].bUse = false;
					AddScore(g_aSummon[8].score * (int)1.5f);
				}
				if (g_aSummon[9].bUse == true)
				{
					SetParticle(g_aSummon[9].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[9].bHit = false; g_aSummon[9].bTime = false; g_aSummon[9].bDrw = false; g_aSummon[9].bUse = false;
					AddScore(g_aSummon[9].score * (int)1.5f);
				}
				if (g_aSummon[18].bUse == true)
				{
					SetParticle(g_aSummon[18].pos, 1, 30, 90.0f, 2, 0);
					g_aSummon[18].bHit = false; g_aSummon[18].bTime = false; g_aSummon[18].bDrw = false; g_aSummon[18].bUse = false;
					AddScore(g_aSummon[18].score * (int)1.5f);
				}
				if (g_aSummon[19].bUse == true)
				{
					SetParticle(g_aSummon[19].pos, 1, 30, 90.0f, 2, 0);
					g_aSummon[19].bHit = false; g_aSummon[19].bTime = false; g_aSummon[19].bDrw = false; g_aSummon[19].bUse = false;
					AddScore(g_aSummon[19].score * (int)1.5f);
				}
				if (g_aSummon[20].bUse == true)
				{
					SetParticle(g_aSummon[20].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[20].bHit = false; g_aSummon[20].bTime = false; g_aSummon[20].bDrw = false; g_aSummon[20].bUse = false;
					AddScore(g_aSummon[20].score * (int)1.5f);
				}
				if (g_aSummon[21].bUse == true)
				{
					SetParticle(g_aSummon[21].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[21].bHit = false; g_aSummon[21].bTime = false; g_aSummon[21].bDrw = false; g_aSummon[21].bUse = false;
					AddScore(g_aSummon[21].score * (int)1.5f);
				}


				if (g_aSummon[7].bUse == false && g_aSummon[8].bUse == false && 
					g_aSummon[9].bUse == false && g_aSummon[18].bUse == false &&
					g_aSummon[19].bUse == false && g_aSummon[20].bUse == false && 
					g_aSummon[21].bUse == false)
				{
					pBoss[2].nReset = 1;
					EnemySummon(3);
					b_summon = 15;
				}
			}

			if (GetNumBoss() == 1)
			{
				PlaySound(SOUND_LABEL_SE_SUMMON);

				if (g_aSummon[0].bUse == true)
				{
					SetParticle(g_aSummon[0].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[0].bHit = false; g_aSummon[0].bTime = false; g_aSummon[0].bDrw = false; g_aSummon[0].bUse = false;
					AddScore(g_aSummon[0].score * (int)1.5f);
				}
				else if (g_aSummon[1].bUse == true)
				{
					SetParticle(g_aSummon[1].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[1].bHit = false; g_aSummon[1].bTime = false; g_aSummon[1].bDrw = false; g_aSummon[1].bUse = false;
					AddScore(g_aSummon[1].score * (int)1.5f);
				}
				else if (g_aSummon[2].bUse == true)
				{
					SetParticle(g_aSummon[2].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[2].bHit = false; g_aSummon[2].bTime = false; g_aSummon[2].bDrw = false; g_aSummon[2].bUse = false;
					AddScore(g_aSummon[2].score * (int)1.5f);
				}
				else if (g_aSummon[3].bUse == true)
				{
					SetParticle(g_aSummon[3].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[3].bHit = false; g_aSummon[3].bTime = false; g_aSummon[3].bDrw = false; g_aSummon[3].bUse = false;
					AddScore(g_aSummon[3].score * (int)1.5f);
				}
				else if (g_aSummon[4].bUse == true)
				{
					SetParticle(g_aSummon[4].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[4].bHit = false; g_aSummon[4].bTime = false; g_aSummon[4].bDrw = false; g_aSummon[4].bUse = false;
					AddScore(g_aSummon[4].score * (int)1.5f);
				}
				else if (g_aSummon[5].bUse == true)
				{
					SetParticle(g_aSummon[5].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[5].bHit = false; g_aSummon[5].bTime = false; g_aSummon[5].bDrw = false; g_aSummon[5].bUse = false;
					AddScore(g_aSummon[5].score * (int)1.5f);
				}
				else if (g_aSummon[6].bUse == true)
				{
					SetParticle(g_aSummon[6].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[6].bHit = false; g_aSummon[6].bTime = false; g_aSummon[6].bDrw = false; g_aSummon[6].bUse = false;
					AddScore(g_aSummon[6].score * (int)1.5f);
				}
			}
			else if (GetNumBoss() == 0)
			{
				PlaySound(SOUND_LABEL_SE_SUMMON);

				if (g_aSummon[7].bUse == true)
				{
					SetParticle(g_aSummon[7].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[7].bHit = false; g_aSummon[7].bTime = false; g_aSummon[7].bDrw = false; g_aSummon[7].bUse = false;
					AddScore(g_aSummon[7].score * (int)1.5f);
				}
				else if (g_aSummon[8].bUse == true)
				{
					SetParticle(g_aSummon[8].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[8].bHit = false; g_aSummon[8].bTime = false; g_aSummon[8].bDrw = false; g_aSummon[8].bUse = false;
					AddScore(g_aSummon[8].score * (int)1.5f);
				}
				else if (g_aSummon[9].bUse == true)
				{
					SetParticle(g_aSummon[9].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[9].bHit = false; g_aSummon[9].bTime = false; g_aSummon[9].bDrw = false; g_aSummon[9].bUse = false;
					AddScore(g_aSummon[9].score * (int)1.5f);
				}
				else if (g_aSummon[10].bUse == true)
				{
					SetParticle(g_aSummon[10].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[10].bHit = false; g_aSummon[10].bTime = false; g_aSummon[10].bDrw = false; g_aSummon[10].bUse = false;
					AddScore(g_aSummon[10].score * (int)1.5f);
				}
				else if (g_aSummon[11].bUse == true)
				{
					SetParticle(g_aSummon[11].pos, 1, 30, 90.0f, 5, 0);
					g_aSummon[11].bHit = false; g_aSummon[11].bTime = false; g_aSummon[11].bDrw = false; g_aSummon[11].bUse = false;
					AddScore(g_aSummon[11].score * (int)1.5f);
				}
				else if (g_aSummon[12].bUse == true)
				{
					SetParticle(g_aSummon[12].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[12].bHit = false; g_aSummon[12].bTime = false; g_aSummon[12].bDrw = false; g_aSummon[12].bUse = false;
					AddScore(g_aSummon[12].score * (int)1.5f);
				}
				else if (g_aSummon[13].bUse == true)
				{
					SetParticle(g_aSummon[13].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[13].bHit = false; g_aSummon[13].bTime = false; g_aSummon[13].bDrw = false; g_aSummon[13].bUse = false;
					AddScore(g_aSummon[13].score * (int)1.5f);
				}
				else if (g_aSummon[14].bUse == true)
				{
					SetParticle(g_aSummon[14].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[14].bHit = false; g_aSummon[14].bTime = false; g_aSummon[14].bDrw = false; g_aSummon[14].bUse = false;
					AddScore(g_aSummon[14].score * (int)1.5f);
				}
				else if (g_aSummon[15].bUse == true)
				{
					SetParticle(g_aSummon[15].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[15].bHit = false; g_aSummon[15].bTime = false; g_aSummon[15].bDrw = false; g_aSummon[15].bUse = false;
					AddScore(g_aSummon[15].score * (int)1.5f);
				}
				else if (g_aSummon[16].bUse == true)
				{
					SetParticle(g_aSummon[16].pos, 1, 30, 90.0f, 4, 0);
					g_aSummon[16].bHit = false; g_aSummon[16].bTime = false; g_aSummon[16].bDrw = false; g_aSummon[16].bUse = false;
					AddScore(g_aSummon[16].score * (int)1.5f);
				}
				else if (g_aSummon[17].bUse == true)
				{
					SetParticle(g_aSummon[17].pos, 1, 30, 90.0f, 4, 0);
					g_aSummon[17].bHit = false; g_aSummon[17].bTime = false; g_aSummon[17].bDrw = false; g_aSummon[17].bUse = false;
					AddScore(g_aSummon[17].score * (int)1.5f);
				}
				else if (g_aSummon[18].bUse == true)
				{
					SetParticle(g_aSummon[18].pos, 1, 30, 90.0f, 2, 0);
					g_aSummon[18].bHit = false; g_aSummon[18].bTime = false; g_aSummon[18].bDrw = false; g_aSummon[18].bUse = false;
					AddScore(g_aSummon[18].score * (int)1.5f);
				}
				else if (g_aSummon[19].bUse == true)
				{
					SetParticle(g_aSummon[19].pos, 1, 30, 90.0f, 2, 0);
					g_aSummon[19].bHit = false; g_aSummon[19].bTime = false; g_aSummon[19].bDrw = false; g_aSummon[19].bUse = false;
					AddScore(g_aSummon[19].score * (int)1.5f);
				}
				else if (g_aSummon[20].bUse == true)
				{
					SetParticle(g_aSummon[20].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[20].bHit = false; g_aSummon[20].bTime = false; g_aSummon[20].bDrw = false; g_aSummon[20].bUse = false;
					AddScore(g_aSummon[20].score * (int)1.5f);
				}
				else if (g_aSummon[21].bUse == true)
				{
					SetParticle(g_aSummon[21].pos, 1, 30, 90.0f, 1, 0);
					g_aSummon[21].bHit = false; g_aSummon[21].bTime = false; g_aSummon[21].bDrw = false; g_aSummon[21].bUse = false;
					AddScore(g_aSummon[21].score * (int)1.5f);
				}
			}
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffSummon->Unlock();
}
//◇==========================◇
//｜　　　 敵の描画処理　　 　｜
//◇==========================◇
void DrawSummon(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEnemy;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSummon, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	for (nCntEnemy = 0; nCntEnemy < MAX_SUMMON; nCntEnemy++)
	{
		if (g_aSummon[nCntEnemy].bDrw == true)
		{//敵が使用されている

			pDevice->SetTexture(0, g_pTextureSummon[g_aSummon[nCntEnemy].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//◇==========================◇
//｜　　　 敵の判定処理　　 　｜
//◇==========================◇

//敵の当たり判定
void CollisionSummon(Summon *pSummon)
{
	int nCntEnemy;

	Player *pPlayer = Getplayer(); //プレイヤーの情報へのポインタ

	for (nCntEnemy = 0; nCntEnemy < MAX_SUMMON; nCntEnemy++, pSummon++)
	{
		if (pSummon->bHit == true)
		{//敵が使用されている
			if (pSummon->pos.x >= pPlayer->pos.x - 32
				&& pSummon->pos.x <= pPlayer->pos.x + 32
				&& pSummon->pos.y >= pPlayer->pos.y - 32
				&& pSummon->pos.y <= pPlayer->pos.y + 32)
			{//敵とプレイヤーが当たった

				HitPlayer(nCntEnemy, 1);
			}
		}
	}
}
//◇============================◇
//｜　　　 敵のヒット処理　　 　｜
//◇============================◇
void HitSummon(int nCntSummon, int nDamage)
{
	if (g_aSummon[nCntSummon].bHit == true)
	{
		if (g_aSummon[16].bHit == true && g_aSummon[17].bHit == true)
		{
			g_aSummon[16].nLife -= nDamage;
			g_aSummon[17].nLife -= nDamage;
		}
		else
		{
			g_aSummon[nCntSummon].nLife -= nDamage;
		}

		if (g_aSummon[nCntSummon].nLife <= 0)//敵の体力がなくなった
		{
				if (g_aSummon[12].bUse == true && g_aSummon[12].nLife == 0)
				{
					if (g_aSummon[14].bUse == true)
					{
						b_summon++;
					}
					g_aSummon[12].bUse = false; g_aSummon[12].bDrw = false; g_aSummon[12].bHit = false;
				}
				if (g_aSummon[14].bUse == true && g_aSummon[14].nLife == 0)
				{
					if (g_aSummon[12].bUse == true)
					{
						b_summon++;
					}
					g_aSummon[14].bUse = false; g_aSummon[14].bDrw = false; g_aSummon[14].bHit = false;
				}

				if (g_aSummon[13].bUse == true && g_aSummon[13].nLife == 0)
				{
					if (g_aSummon[15].bUse == true)
					{
						b_summon++;
					}
					g_aSummon[13].bUse = false; g_aSummon[13].bDrw = false; g_aSummon[13].bHit = false;
				}
				if (g_aSummon[15].bUse == true && g_aSummon[15].nLife == 0)
				{
					if (g_aSummon[13].bUse == true)
					{
						b_summon++;
					}
					g_aSummon[15].bUse = false; g_aSummon[15].bDrw = false; g_aSummon[15].bHit = false;
				}

				if (g_aSummon[18].bUse == true && g_aSummon[18].nLife == 0)
				{
					if (g_aSummon[20].bUse == true)
					{
						b_summon++;
					}
					g_aSummon[18].bUse = false; g_aSummon[18].bDrw = false; g_aSummon[18].bHit = false;
				}
				if (g_aSummon[20].bUse == true && g_aSummon[20].nLife == 0)
				{
					if (g_aSummon[18].bUse == true)
					{
						b_summon++;
					}
					g_aSummon[20].bUse = false; g_aSummon[20].bDrw = false; g_aSummon[20].bHit = false;
				}

				if (g_aSummon[19].bUse == true && g_aSummon[19].nLife == 0)
				{
					if (g_aSummon[21].bUse == true)
					{
						b_summon++;
					}
					g_aSummon[19].bUse = false; g_aSummon[19].bDrw = false; g_aSummon[19].bHit = false;
				}
				if (g_aSummon[21].bUse == true && g_aSummon[21].nLife == 0)
				{
					if (g_aSummon[19].bUse == true)
					{
						b_summon++;
					}
					g_aSummon[21].bUse = false; g_aSummon[21].bDrw = false; g_aSummon[21].bHit = false;
				}
			

			if (g_aSummon[16].bUse == true && g_aSummon[17].bUse == true && g_aSummon[16].nLife == 0 && g_aSummon[17].nLife == 0)
			{
				g_aSummon[16].bUse = false;g_aSummon[16].bDrw = false;g_aSummon[16].bHit = false;SetParticle(g_aSummon[16].pos, 0, 20, 10.0f, 4, 0);
				g_aSummon[17].bUse = false;g_aSummon[17].bDrw = false;g_aSummon[17].bHit = false;SetParticle(g_aSummon[16].pos, 0, 20, 10.0f, 4, 0);
			}

			//爆発の設定
			switch (g_aSummon[nCntSummon].nType)
			{
			case 0:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 0, 0);
				break;
			case 1:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 0, 0);
				break;
			case 2:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 1, 0);
				break;
			case 3:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 4, 0);
				break;
			case 4:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 5, 0);
				break;
			case 5:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 4, 0);
				break;
			case 6:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 4, 0);
				break;
			}

			//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			PlaySound(SOUND_LABEL_SE_EXPLOSION); //←再生したいサウンドを指定

			g_aSummon[nCntSummon].bUse = false;
			g_aSummon[nCntSummon].bDrw = false;
			g_aSummon[nCntSummon].bHit = false;

			g_nNumSummon--;		//現在の敵の数
			g_nTotalSummon--;	//敵の総数を減らす
			AddScore(g_aSummon[nCntSummon].score);
		}

		else
		{
			if (g_aSummon[nCntSummon].state == SUMMONSTATE_NORMAL)
			{
				g_aSummon[nCntSummon].state = SUMMONSTATE_DAMAGE;
				g_aSummon[nCntSummon].nCounterState = 10; //ダメージを保つ時間を設定
			}
			PlaySound(SOUND_LABEL_SE_HIT); //←再生したいサウンドを指定

			VERTEX_2D *pVtx;	//設定用ポインタ

								//頂点バッファのロックと頂点情報へのポインタを取得
			g_pVtxBuffSummon->Lock(0, 0, (void **)&pVtx, 0);

			switch (g_aSummon[nCntSummon].nType)
			{
			case 0:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 5, 6.0f, 0, 0);
				break;
			case 1:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 5, 6.0f, 0, 0);
				break;
			case 2:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 5, 6.0f, 1, 0);
				break;
			case 3:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 4, 0);
				break;
			case 4:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 20, 10.0f, 5, 0);
				break;
			case 5:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 5, 6.0f, 0, 0);
				break;
			case 6:
				SetParticle(g_aSummon[nCntSummon].pos, 0, 5, 6.0f, 4, 0);
				break;
			}

			pVtx += nCntSummon * 4;
			//◇――――――――――――◇
			//｜　　　 頂点カラー 　　　｜
			//◇――――――――――――◇
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //ポリゴンのダメージ色に設定

															 //頂点バッファをアンロック
			g_pVtxBuffSummon->Unlock();
		}
	}
}
//◇====================◇
//｜　 敵の総数の取得　 ｜
//◇====================◇
int GetNumSummon(void)
{
	return g_nNumSummon;
}
//◇======================◇
//｜　　　 敵の所得　　 　｜
//◇======================◇
Summon *GetSummon(void)
{
	return &g_aSummon[0];//敵の情報の先頭アドレスを返す
}
//◇====================================◇
//｜　　　 エネミーデータ読み込み　　 　｜
//◇====================================◇
void LoadSummon(void)
{
	int c = 0;	   //1文字ずつ確認する変数
	int column = 1;//列数を数える変数
	int line = 0;//行数を数える変数
	int nCntEnemy; //カウント変数

	char aData[100];//つなげる文字数
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data/csv/SummonData.csv", "r");

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
		case 1:		g_aSummon[line].nType = atoi(aData);			 break;	//１列目：敵・種類
		case 2:		g_aSummon[line].sType = atoi(aData);			 break;	//  列目：弾・種類
		case 3:		g_aSummon[line].m_pattern = atoi(aData);		 break;	//　列目：移動パターン
		case 4:		g_aSummon[line].s_pattern = atoi(aData);		 break;	//　列目：発射パターン
		case 5:		g_aSummon[line].pos.x = (float)(atoi(aData));	 break;	//　列目：X座標
		case 6:		g_aSummon[line].pos.y = (float)(atoi(aData));	 break;	//　列目：Y座標
		case 7:		g_aSummon[line].nLife = atoi(aData);			 break;	//　列目：体力
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
	}
	//ファイルを閉じる
	fclose(pFile);

	//敵クラスの生成
	for (nCntEnemy = 0; nCntEnemy < MAX_SUMMON; nCntEnemy++)
	{
		if (g_aSummon[nCntEnemy].nType == 1)
		{
			g_aSummon[nCntEnemy].score = 200;
		}
		else if (g_aSummon[nCntEnemy].nType == 2)
		{
			g_aSummon[nCntEnemy].score = 500;
		}
		else if (g_aSummon[nCntEnemy].nType == 3)
		{
			g_aSummon[nCntEnemy].score = 600;
		}
		else if (g_aSummon[nCntEnemy].nType == 4)
		{
			g_aSummon[nCntEnemy].score = 250;
		}
		else if (g_aSummon[nCntEnemy].nType == 5)
		{
			g_aSummon[nCntEnemy].score = 2000;
		}
		else if (g_aSummon[nCntEnemy].nType == 6)
		{
			g_aSummon[nCntEnemy].score = 1000;
		}
	}
}
//◇==========================◇
//｜　　　 敵の召喚処理  　 　｜
//◇==========================◇
void EnemySummon(int p_pattern)
{
	p_summon = p_pattern;

	switch (p_summon)
	{
	case 0:
		g_aSummon[0].bTime = true; g_aSummon[0].bUse = true;
		g_aSummon[1].bTime = true; g_aSummon[1].bUse = true;
		g_aSummon[2].bTime = true; g_aSummon[2].bUse = true;
		g_aSummon[3].bTime = true; g_aSummon[3].bUse = true;
		stand_time = 62;
		break;

	case 1:
		g_aSummon[7].bTime = true; g_aSummon[7].bUse = true;
		g_aSummon[8].bTime = true; g_aSummon[8].bUse = true;
		g_aSummon[9].bTime = true; g_aSummon[9].bUse = true;
		g_aSummon[10].bTime = true; g_aSummon[10].bUse = true;
		g_aSummon[11].bTime = true; g_aSummon[11].bUse = true;
		g_aSummon[12].bTime = true; g_aSummon[12].bUse = true;
		g_aSummon[13].bTime = true; g_aSummon[13].bUse = true;
		g_aSummon[14].bTime = true; g_aSummon[14].bUse = true;
		g_aSummon[15].bTime = true; g_aSummon[15].bUse = true;
		stand_time = 62;
		break;
	case 2:
		g_aSummon[7].bTime = true; g_aSummon[7].bUse = true;
		g_aSummon[8].bTime = true; g_aSummon[8].bUse = true;
		g_aSummon[9].bTime = true; g_aSummon[9].bUse = true;
		g_aSummon[18].bTime = true; g_aSummon[18].bUse = true;
		g_aSummon[19].bTime = true; g_aSummon[19].bUse = true;
		g_aSummon[20].bTime = true; g_aSummon[20].bUse = true;
		g_aSummon[21].bTime = true; g_aSummon[21].bUse = true;
		stand_time = 132;
		break;
	case 3:
		g_aSummon[16].bTime = true; g_aSummon[16].bUse = true;
		g_aSummon[17].bTime = true; g_aSummon[17].bUse = true;
		stand_time = 132;
		break;
	}
}
//◇============================◇
//｜　　 チャージ率の所得　　 　｜
//◇============================◇
int GetnCharge1(void)
{
	return nCharge1;
}
int GetnCharge2(void)
{
	return nCharge2;
}
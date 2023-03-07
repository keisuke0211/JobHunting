//◇====================◇
//｜　　　　　　　　　　｜
//｜　ボスの処理        ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"game.h"
#include"bullet.h"
#include"explosion.h"
#include"score.h"
#include"sound.h"
#include"player.h"
#include"enemy_bullet.h"
#include"boss.h"
#include"particle.h"
#include"player.h"
#include"Summon.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_LIFE (100)  //HPの最大数
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void CollisionBoss(Boss *pBoss);
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DTEXTURE9 g_pTextureBoss[NUM_BOSS] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;		//頂点バッファポインタ
Boss g_aBoss[NUM_BOSS];								//敵の情報

int g_nNumBoss;										//敵の総数
int g_Count1;										//経過時間
int g_nCntWhile;									//一定時間
int g_nCount3;										//一定時間
int nCntStop;										//停止カウント
int nCntMove;										//真ん中に戻る時間
int nCntpat;										//ワープ後のカウント
int g_inCnt;										//出現カウント
int nCntPlaterMove;									//最後のボスが死んでからプレイヤーが動くカウント
int nCntMachineGun;									//機関銃のカウント
int nCntCharge;										//チャージ時間
int nCharge;										//チャージ率

bool nStop;											//停止フラグ
bool Reversal1 = false;								//移動方向
bool bBonus = false;
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【敵・処理】｜――――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇============================◇
//｜　　　 ボスの初期化処理　　 ｜
//◇============================◇
void InitBoss(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntBoss;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチャの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\boss1.png",
		&g_pTextureBoss[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy3.png",
		&g_pTextureBoss[1]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy3-1.png",
		&g_pTextureBoss[2]);
	
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy3-2.png",
		&g_pTextureBoss[3]);

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_BOSS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void **)&pVtx, 0);

	nCntMove = 0;
	nCntpat  = 0;
	g_inCnt  = 0;
	nCntPlaterMove = 800;
	nCntMachineGun = 0;
	nCntCharge = 361;
	nCharge = 0;
	//敵の情報の初期化処理
	for (nCntBoss = 0; nCntBoss < NUM_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].move.x = 0.3f;
		g_aBoss[nCntBoss].nBullet = 0;
		g_aBoss[nCntBoss].curt_nLife = 1;
		g_aBoss[nCntBoss].state_pattern = 0;
		g_aBoss[nCntBoss].nCounterState = 0;
		g_aBoss[nCntBoss].stand_time = 150;
		g_aBoss[nCntBoss].nReset = 0;

		g_aBoss[nCntBoss].bUse = false;
		g_aBoss[nCntBoss].bDrw = false;
		g_aBoss[nCntBoss].bHit = false;
		g_aBoss[nCntBoss].bTime = false;
		g_aBoss[nCntBoss].bSummon = false;

		nStop = false;
		Reversal1 = false;

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
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //頂点データのポインタを４つ分進める
	}

	g_nNumBoss = 0;	//敵の総数をクリア
	g_Count1 = 0;	//経過時間をクリア
	g_nCntWhile = 0;//一定時間をクリア
	nCntStop = 0;	//停止時間をクリア
	//頂点バッファのアンロック
	g_pVtxBuffBoss->Unlock();

	LoadBoss();
}
//◇==========================◇
//｜　　　 ボスの終了処理　 　｜
//◇==========================◇
void UninitBoss(void)
{
	int nCntBoss;
	//サウンドの停止
	StopSound();
	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (nCntBoss = 0; nCntBoss < NUM_BOSS; nCntBoss++)
	{
		if (g_pTextureBoss[nCntBoss] != NULL)
		{
			g_pTextureBoss[nCntBoss]->Release();
			g_pTextureBoss[nCntBoss] = NULL;
		}
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}
//◇==========================◇
//｜　　　 ボスの更新処理　 　｜
//◇==========================◇
void UpdateBoss(void)
{
	int nCntEnemy;		//ボスカウンタ
	VERTEX_2D *pVtx;	//設定用ポインタ
	Player *pPlayer = Getplayer();	//プレイヤーの情報へのポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void **)&pVtx, 0);

	//g_Count1 = GetCunt();
	g_Count1++;
	for (nCntEnemy = 0; nCntEnemy < NUM_BOSS; nCntEnemy++, pVtx += 4)
	{
		//状態遷移処理
		if (g_aBoss[nCntEnemy].bUse == true)
		{
			CollisionBoss(&g_aBoss[nCntEnemy]);

			D3DXVECTOR3 movevec;

			if (g_aBoss[1].curt_nLife <= g_aBoss[1].init_nLife / 2 && g_aBoss[1].bDrw == true)
			{
				SetParticle(D3DXVECTOR3(g_aBoss[1].pos.x + 0.0f, g_aBoss[1].pos.y + 15.0f, 0.0f), 6, 2, 15.0f, 7, 6);
			}

				switch (g_aBoss[nCntEnemy].m_pattern)
				{
				case 0:
					//出て来てから止まる時間までの間なら下に移動
					if (g_aBoss[nCntEnemy].in_time < g_Count1 && g_Count1 < g_aBoss[nCntEnemy].stop_time)
					{
						g_aBoss[nCntEnemy].pos.y += 2;
					}
					else if (g_Count1 > g_aBoss[nCntEnemy].out_time)
					{
						g_aBoss[nCntEnemy].m_pattern = 91;
						g_aBoss[nCntEnemy].bHit = true;
					}
					break;
				case 91:

					if (g_aBoss[nCntEnemy].curt_nLife <= 100)
					{
						if (nStop == false)
						{
							if (Reversal1 == false)
							{
								g_aBoss[nCntEnemy].pos.x -= 1.5;
								if (g_aBoss[nCntEnemy].pos.x < 110)
								{
									g_aBoss[nCntEnemy].pos.x = 110;
									nStop = true;
								}
							}
							else if (Reversal1 == true)
							{
								g_aBoss[nCntEnemy].pos.x += 1.5;
								if (g_aBoss[nCntEnemy].pos.x > 860)
								{
									g_aBoss[nCntEnemy].pos.x = 860;
									nStop = true;
								}
							}
						}
						else if (nStop == true)
						{
							nCntStop++;
							if (nCntStop == 20)
							{
								if (Reversal1 == false)
								{
									Reversal1 = true;
								}
								else if (Reversal1 == true)
								{
									Reversal1 = false;
								}
								nStop = false;
								nCntStop = 0;
							}
						}
					}
					break;
				case 98:
					g_aBoss[nCntEnemy].pos.x += 0;
					g_aBoss[nCntEnemy].pos.y += 0;
					break;
				case 99:
					//真ん中に戻る処理

					//if (g_aBoss[nCntEnemy].pos.x >= 500.0f - 1 &&
					//	g_aBoss[nCntEnemy].pos.x <= 500.0f + 1)
					//{
					//	bMove = true;
					//	g_aBoss[nCntEnemy].m_pattern = 98;
					//}
					//else if (bMove == false)
					//{
					//	movevec = D3DXVECTOR3(500.0f, 115.0f, 0.0f) - g_aBoss[nCntEnemy].pos;//弾からプレイヤーまでの方向ベクトルを求める
					nCntMove++;
					if (g_aBoss[nCntEnemy].pos == D3DXVECTOR3(500.0f, 115.0f, 0.0f))
					{
						nCntpat++;
						if (nCntpat == 1)
						{
							if (g_aBoss[nCntEnemy].state_pattern == 1)
							{
								SetParticle(g_aBoss[0].pos, 1, 30, 90.0f, 1,0);
							}
							else if (g_aBoss[nCntEnemy].state_pattern == 2)
							{
								SetParticle(g_aBoss[0].pos, 1, 30, 90.0f, 0,0);
							}
						}
						else if (nCntpat >= 2)
						{
							g_aBoss[nCntEnemy].pos.x += 0;
							g_aBoss[nCntEnemy].pos.y += 0;
							g_aBoss[nCntEnemy].bTime = true;
						}
					}
					else
					{
						g_aBoss[nCntEnemy].pos = (g_aBoss[nCntEnemy].pos * (1.0f - (nCntMove / 60)) + (D3DXVECTOR3(500.0f, 115.0f, 0.0f) *(nCntMove / 60)));//カウントの前に(float)を入れる
					}
					break;
				}

			//画面からはみ出したら敵を非表示にする
			if (g_aBoss[nCntEnemy].pos.x < 0 || g_aBoss[nCntEnemy].pos.x > SCREEN_WIDTH - 320)
			{
				//X座標が範囲外の場合
				g_aBoss[nCntEnemy].bUse = false; //使用していない状態にする
			}
			if (g_aBoss[nCntEnemy].pos.y < -100 || g_aBoss[nCntEnemy].pos.y > SCREEN_HEIGHT)
			{
				//Y座標が範囲外の場合
				g_aBoss[nCntEnemy].bUse = false; //使用していない状態にする
			}

			//待機処理

			if (g_aBoss[nCntEnemy].bTime == true)
			{
				g_aBoss[nCntEnemy].stand_time--;
				
				if (g_aBoss[nCntEnemy].stand_time <= 0)
				{
					g_aBoss[nCntEnemy].bTime = false;
					//g_aBoss[nCntEnemy].stand_time = 30;

					if (g_inCnt == 1)
					{
						g_aBoss[1].bHit = true;	//ボスの当たり判定ON
						g_aBoss[1].m_pattern = 91;
					}
					else if (g_inCnt == 2)
					{
						g_aBoss[2].bHit = true;//ボスの当たり判定ON
						g_Count1 = 3;
					}
					else if (g_inCnt == 2 && g_aBoss[3].bTime == true && g_aBoss[4].bTime == true && g_aBoss[5].bTime == true)
					{
						g_aBoss[3].bHit = true;
						g_aBoss[4].bHit = true;
						g_aBoss[5].bHit = true;
					}
				}
				else if (g_aBoss[nCntEnemy].stand_time == 1)
				{
					if (g_inCnt == 1)
					{
						g_aBoss[1].m_pattern = 98;
					}
					else if (g_inCnt == 2)
					{
						g_aBoss[2].m_pattern = 98;
					}
				}
				else if (g_aBoss[nCntEnemy].stand_time <= 20)
				{
					if (g_inCnt == 1)
					{
						g_aBoss[1].bDrw = true;
					}
					else if (g_inCnt == 2)
					{
						g_aBoss[2].bDrw = true;
					}
					else if (g_inCnt == 2 && g_aBoss[3].bTime == true && g_aBoss[4].bTime == true && g_aBoss[5].bTime == true)
					{
						g_aBoss[3].bDrw = true;
						g_aBoss[4].bDrw = true;
						g_aBoss[5].bDrw = true;
					}
				}
				else if (g_aBoss[nCntEnemy].stand_time == 60)
				{
					if (g_inCnt == 1)
					{
						PlaySound(SOUND_LABEL_SE_SUMMON);
						SetParticle(g_aBoss[1].pos, 1, 30, 90.0f, 1,0);
					}
					else if (g_inCnt == 2)
					{
						PlaySound(SOUND_LABEL_SE_SUMMON);
						SetParticle(g_aBoss[2].pos, 1, 30, 90.0f, 0,0);
					}
					else if (g_inCnt == 2 && g_aBoss[3].bTime == true && g_aBoss[4].bTime == true && g_aBoss[5].bTime == true)
					{
						bBonus = true;
						PlaySound(SOUND_LABEL_SE_SUMMON);
						SetParticle(g_aBoss[3].pos, 1, 30, 90.0f, 0, 0);
						SetParticle(g_aBoss[4].pos, 1, 30, 90.0f, 0, 0);
						SetParticle(g_aBoss[5].pos, 1, 30, 90.0f, 0, 0);
					}
				}
			}
			if (g_aBoss[nCntEnemy].bHit == true)
			{
				switch (g_aBoss[nCntEnemy].state)
				{
				case BOSSSTATE_NORMAL:
					break;

				case BOSSSTATE_DAMAGE:
					g_aBoss[nCntEnemy].nCounterState--;


					if (g_aBoss[nCntEnemy].nCounterState <= 0)
					{
						g_aBoss[nCntEnemy].state = BOSSSTATE_NORMAL;
						g_aBoss[nCntEnemy].bDrw = true;
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
						g_aBoss[nCntEnemy].bDrw = g_aBoss[nCntEnemy].bDrw ? false : true;
					}
					break;
				}
			}

			//g_aBoss[nCntBoss].pos.y += g_aBoss[nCntBoss].move.y;

			//頂点座標の設定
			pVtx[0].pos.x = g_aBoss[nCntEnemy].pos.x - SIZE_BOSS * 1;
			pVtx[0].pos.y = g_aBoss[nCntEnemy].pos.y - SIZE_BOSS * 1;
			pVtx[0].pos.z = 0;
			pVtx[1].pos.x = g_aBoss[nCntEnemy].pos.x + SIZE_BOSS * 1;
			pVtx[1].pos.y = g_aBoss[nCntEnemy].pos.y - SIZE_BOSS * 1;
			pVtx[1].pos.z = 0;
			pVtx[2].pos.x = g_aBoss[nCntEnemy].pos.x - SIZE_BOSS * 1;
			pVtx[2].pos.y = g_aBoss[nCntEnemy].pos.y + SIZE_BOSS * 1;
			pVtx[2].pos.z = 0;
			pVtx[3].pos.x = g_aBoss[nCntEnemy].pos.x + SIZE_BOSS * 1;
			pVtx[3].pos.y = g_aBoss[nCntEnemy].pos.y + SIZE_BOSS * 1;
			pVtx[3].pos.z = 0;

			if (g_aBoss[nCntEnemy].bHit == true)
			{
				if (g_aBoss[2].nReset == 1)
				{
					nCntCharge = 361;
					nCharge = 0;
					g_aBoss[nCntEnemy].s_pattern = BULLETTYPE_E_CHARGE;

					g_aBoss[2].nReset = 0;
				}

				switch (g_aBoss[nCntEnemy].s_pattern)
				{
				case BULLETTYPE_E_LINE:
					g_aBoss[nCntEnemy].nBullet++;//カウンターを加算
					if ((g_aBoss[nCntEnemy].nBullet % 40) == 0)
					{
						g_aBoss[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
						SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_LINE, g_aBoss[nCntEnemy].sType);
					}
					break;
				case BULLETTYPE_E_AIM:
					g_aBoss[nCntEnemy].nBullet++;//カウンターを加算
					if ((g_aBoss[nCntEnemy].nBullet % 30) == 0)
					{
						g_aBoss[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
						SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(0.0f, 13.0f, 0.0f), BULLETTYPE_E_AIM, g_aBoss[nCntEnemy].sType);
					}
					break;
				case BULLETTYPE_E_HOMING:
					g_aBoss[nCntEnemy].nBullet++;//カウンターを加算
					if ((g_aBoss[nCntEnemy].nBullet % 60) == 0)
					{
						g_aBoss[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
						SetEnemyBullet(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 5.0f, 0.0f), D3DXVECTOR3(0.0f, 5, 0.0f), BULLETTYPE_E_HOMING, g_aBoss[nCntEnemy].sType);

					}
					break;
				case BULLETTYPE_E_3_SHOT:
					g_aBoss[nCntEnemy].nBullet++;//カウンターを加算

					if ((g_aBoss[nCntEnemy].nBullet % 30) == 0)
					{
						SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_3_SHOT, g_aBoss[nCntEnemy].sType);
						SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * -0.1)* SPEED, cosf(D3DX_PI * 0.1)* 5.0f, 0.0f), BULLETTYPE_E_3_SHOT, g_aBoss[nCntEnemy].sType);
						SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * 0.1)* SPEED, cosf(D3DX_PI  * 0.1)* 5.0f, 0.0f),BULLETTYPE_E_3_SHOT, g_aBoss[nCntEnemy].sType);
					}
					break;
				case BULLETTYPE_S_1_SUMMON:
					if (g_aBoss[1].bSummon == true)
					{
						EnemySummon(0);
						g_aBoss[1].s_pattern = 0;
						g_aBoss[1].sType = 0;
						g_aBoss[1].bSummon = false;
						PlaySound(SOUND_LABEL_SE_SUMMON);
					}
					break;
				case BULLETTYPE_S_2_SUMMON:
					if (g_aBoss[2].bSummon == true)
					{
						EnemySummon(1);
						g_aBoss[2].s_pattern = BULLETTYPE_E_CHARGE;
						g_aBoss[2].sType = 1;
						g_aBoss[2].bSummon = false;
						PlaySound(SOUND_LABEL_SE_SUMMON);
					}
					break;
				case BULLETTYPE_E_CHARGE:

					g_aBoss[nCntEnemy].nBullet++;//カウンターを加算

					if (g_aBoss[2].curt_nLife <= g_aBoss[2].init_nLife / 2)
					{
						if ((g_aBoss[nCntEnemy].nBullet % 60) == 0)
						{
							SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_3_SHOT2, g_aBoss[nCntEnemy].sType);
							SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * -0.1)* SPEED, cosf(D3DX_PI * 0.1)* 5.0f, 0.0f), BULLETTYPE_E_3_SHOT2, g_aBoss[nCntEnemy].sType);
							SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * 0.1)* SPEED, cosf(D3DX_PI  * 0.1)* 5.0f, 0.0f), BULLETTYPE_E_3_SHOT2, g_aBoss[nCntEnemy].sType);
						}
						//チャージ状態ならカウントを減らす
						if (nCntCharge <= 0)
						{
							g_aBoss[nCntEnemy].s_pattern = BULLETTYPE_E_MACHINEGUN;
							nCntMachineGun = 0;
						}
						else if (nCntCharge == 301)
						{
							nCntCharge--;
							PlaySound(SOUND_LABEL_SE_CHARG);
						}
						else if (nCntCharge <= 60)
						{
							nCntCharge--;
							nCharge++;
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 3);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 31);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 32);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 42);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 42);
						}
						else if (nCntCharge <= 120 && nCntCharge >= 61)
						{
							nCntCharge--;
							nCharge++;
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 3);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 31);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
						}
						else if (nCntCharge <= 300 && nCntCharge >= 121)
						{
							nCntCharge--;
							nCharge++;
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 3);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
						}
						else
						{
							nCntCharge--;
						}
					}
					else
					{
						if ((g_aBoss[nCntEnemy].nBullet % 90) == 0)
						{
							SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_3_SHOT2, g_aBoss[nCntEnemy].sType);
							SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * -0.1)* SPEED, cosf(D3DX_PI * 0.1)* 5.0f, 0.0f), BULLETTYPE_E_3_SHOT2, g_aBoss[nCntEnemy].sType);
							SetEnemyBullet(g_aBoss[nCntEnemy].pos, D3DXVECTOR3(sinf(D3DX_PI * 0.1)* SPEED, cosf(D3DX_PI  * 0.1)* 5.0f, 0.0f), BULLETTYPE_E_3_SHOT2, g_aBoss[nCntEnemy].sType);
						}
						//チャージ状態ならカウントを減らす
						if (nCntCharge <= 0)
						{
							g_aBoss[nCntEnemy].s_pattern = BULLETTYPE_E_MACHINEGUN;
							nCntMachineGun = 0;
						}
						else if (nCntCharge == 301)
						{
							nCntCharge--;
							PlaySound(SOUND_LABEL_SE_CHARG);
						}
						else if (nCntCharge <= 60)
						{
							nCntCharge--;
							nCharge++;
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 42);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 42);
						}
						else if (nCntCharge <= 300 && nCntCharge >= 60)
						{
							nCntCharge--;
							nCharge++;
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
						}
						else
						{
							nCntCharge--;
						}
					}

						break;
				case BULLETTYPE_E_MACHINEGUN:

					g_aBoss[nCntEnemy].nBullet++;//カウンターを加算

					if (g_aBoss[2].curt_nLife <= g_aBoss[2].init_nLife / 2)
					{
						if ((g_aBoss[nCntEnemy].nBullet % 60) == 0)
						{
							SetEnemyBullet(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_MACHINEGUN, g_aBoss[nCntEnemy].sType);
							SetEnemyBullet(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x - 50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_MACHINEGUN, g_aBoss[nCntEnemy].sType);
						}
						if (nCntMachineGun <= 310)
						{
							nCntMachineGun++; //カウントを加算
							nCharge--;

							//20回に一回発射。310までなので15発発射。
							if ((nCntMachineGun % 20) == 0)
							{

								SetEnemyBullet(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_MACHINEGUN, g_aBoss[nCntEnemy].sType);
							}
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 3);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 31);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 0.0f, g_aBoss[nCntEnemy].pos.y + 60.0f, 0.0f), 3, 2, 5.0f, 0, 32);

						}
						else
						{
							nCntCharge = 361;
							nCharge = 0;
							g_aBoss[nCntEnemy].s_pattern = BULLETTYPE_E_CHARGE;
						}
					}
					else
					{
						if (nCntMachineGun <= 310)
						{
							nCntMachineGun++; //カウントを加算
							nCharge--;

							//20回に一回発射。310までなので15発発射。
							if ((nCntMachineGun % 30) == 0)
							{

								SetEnemyBullet(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_MACHINEGUN, g_aBoss[nCntEnemy].sType);
								SetEnemyBullet(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x - 50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), D3DXVECTOR3(0.0f, 5.0f, 0.0f), BULLETTYPE_E_MACHINEGUN, g_aBoss[nCntEnemy].sType);
							}
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 2);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 41);

							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + 53.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 42);
							SetParticle(D3DXVECTOR3(g_aBoss[nCntEnemy].pos.x + -50.0f, g_aBoss[nCntEnemy].pos.y + 70.0f, 0.0f), 3, 2, 5.0f, 1, 42);
						}
						else
						{
							nCntCharge = 361;
							nCharge = 0;
							g_aBoss[nCntEnemy].s_pattern = BULLETTYPE_E_CHARGE;
						}
					}

					break;
				}
			}
		}
		if (g_aBoss[2].bUse == false)
		{
			nCntPlaterMove--;
			if (nCntPlaterMove == 0)
			{
				nCntPlaterMove = 0;
				pPlayer->bUse = false;
			}
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffBoss->Unlock();
}
//◇==========================◇
//｜　　 ボスの描画処理　　 　｜
//◇==========================◇
void DrawBoss(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntBoss;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	for (nCntBoss = 0; nCntBoss < NUM_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bDrw == true)
		{//敵が使用されている

			pDevice->SetTexture(0, g_pTextureBoss[g_aBoss[nCntBoss].nType]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBoss * 4, 2);

		}
	}
}

//◇==========================◇
//｜　　　ボスの判定処理　　　｜
//◇==========================◇

//ボスの当たり判定
void CollisionBoss(Boss *pBoss)
{
	int nCntEnemy;

	Player *pPlayer = Getplayer(); //プレイヤーの情報へのポインタ

	for (nCntEnemy = 0; nCntEnemy < NUM_BOSS; nCntEnemy++, pBoss++)
	{
		if (pBoss->bUse == true && pBoss->bHit == true && pPlayer->bUse == true)
		{//敵が使用されている
			if (pBoss->pos.x >= pPlayer->pos.x - 80
				&& pBoss->pos.x <= pPlayer->pos.x + 80
				&& pBoss->pos.y >= pPlayer->pos.y - 80
				&& pBoss->pos.y <= pPlayer->pos.y + 80)
			{//敵と弾が当たった
				HitPlayer(nCntEnemy, 1);
			}
		}
	}
}
//◇============================◇
//｜　　 ボスのヒット処理　　 　｜
//◇============================◇
void HitBoss(int nCntBoss, int nDamage)
{
	g_aBoss[nCntBoss].curt_nLife -= nDamage;

	if (g_aBoss[nCntBoss].curt_nLife <= 0)//敵の体力がなくなった
	{
		//爆発の設定
		if (g_aBoss[0].curt_nLife == 0 && g_aBoss[0].bUse == true)
		{
			SetParticle(g_aBoss[0].pos, 1, 30, 90.0f, 0,0);
			g_aBoss[0].bUse = false;
			g_aBoss[0].bHit = false;
			g_aBoss[0].bDrw = false;
		}
		else if (g_aBoss[1].curt_nLife == 0 && g_aBoss[1].bUse == true)
		{
			SetParticle(g_aBoss[1].pos, 1, 30, 90.0f, 0,0);
			g_aBoss[1].bUse = false;
			g_aBoss[1].bHit = false;
			g_aBoss[1].bDrw = false;
		}
		else if (g_aBoss[2].curt_nLife == 0 && g_aBoss[2].bUse == true)
		{
			SetParticle(g_aBoss[2].pos, 2, 30, 90.0f, 2,0);
			g_aBoss[2].bUse = false;
			g_aBoss[2].bHit = false;
			g_aBoss[2].bDrw = false;
		}
		//SetExplosion(g_aBoss[nCntBoss].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		PlaySound(SOUND_LABEL_SE_EXPLOSION); //←再生したいサウンドを指定

		//スコアを加算
		AddScore(g_aBoss[nCntBoss].score);

		

		g_nNumBoss--; //敵の総数を減らす

		if (g_nNumBoss <= 0)
		{
			//状態の設定
			SetGameState(GAMESTATE_END1, 450);
		}
		else if (g_nNumBoss == 2)
		{
			if (g_inCnt == 0)
			{
				g_inCnt++;
				g_aBoss[1].bTime = true;
				g_aBoss[1].bSummon = true;
			}
		}
		else if (g_nNumBoss == 1)
		{
			if (g_inCnt == 1)
			{
				g_inCnt++;
				g_aBoss[2].bTime = true;
				g_aBoss[2].bSummon = true;
			}

			//g_aBoss[2].bTime = true;

			//g_aBoss[1].bUse = true; //ボスを使用している状態にする
			//g_aBoss[1].bDrw = true;
			//g_aBoss[1].bHit = false;	//ボスの当たり判定ON
		}
	}

	else
	{
		/*if (g_aBoss[nCntBoss].state_pattern == 0)
		{
			if (g_aBoss[nCntBoss].curt_nLife <= g_aBoss[nCntBoss].init_nLife / 2 && g_aBoss[nCntBoss].curt_nLife >= g_aBoss[nCntBoss].init_nLife / 4)
			{
				if (g_aBoss[nCntBoss].state_pattern == 0)
				{
					g_aBoss[nCntBoss].bHit = false;
					g_aBoss[nCntBoss].bDrw = false;

					SetParticle(g_aBoss[nCntBoss].pos, 1, 30, 90.0f, 1);

					g_aBoss[nCntBoss].m_pattern = 99;
					g_aBoss[nCntBoss].state_pattern++;
				}
			}
		}
		else if (g_aBoss[nCntBoss].state_pattern == 1)
		{
			if (g_aBoss[nCntBoss].curt_nLife <= g_aBoss[nCntBoss].init_nLife / 4)
			{
				if (g_aBoss[nCntBoss].state_pattern == 1)
				{
					g_aBoss[nCntBoss].bHit = false;
					g_aBoss[nCntBoss].bDrw = false;

					g_nCntMove = 0;
					nCntpat = 0;
					SetParticle(g_aBoss[nCntBoss].pos, 1, 30, 90.0f, 0);

					g_aBoss[nCntBoss].m_pattern = 99;
					g_aBoss[nCntBoss].state_pattern++;
				}
			}
		}*/

		if (g_aBoss[nCntBoss].state == BOSSSTATE_NORMAL)
		{
			g_aBoss[nCntBoss].state = BOSSSTATE_DAMAGE;
			g_aBoss[nCntBoss].nCounterState = 6; //ダメージを保つ時間を設定
		}

		/*if (g_aBoss[nCntBoss].state_pattern == 0)
		{
			if (g_aBoss[nCntBoss].curt_nLife <= g_aBoss[nCntBoss].init_nLife / 2 && g_aBoss[nCntBoss].curt_nLife >= g_aBoss[nCntBoss].init_nLife / 4)
			{
				if (g_aBoss[nCntBoss].state_pattern == 0)
				{
					g_aBoss[nCntBoss].bHit = false;
					g_aBoss[nCntBoss].bDrw = false;

					SetParticle(g_aBoss[nCntBoss].pos, 1, 30, 90.0f, 1);

					g_aBoss[nCntBoss].m_pattern = 99;
					g_aBoss[nCntBoss].state_pattern++;
				}
			}
		}
		else if (g_aBoss[nCntBoss].state_pattern == 1)
		{
			if (g_aBoss[nCntBoss].curt_nLife <= g_aBoss[nCntBoss].init_nLife / 4)
			{
				if (g_aBoss[nCntBoss].state_pattern == 1)
				{
					g_aBoss[nCntBoss].bHit = false;
					g_aBoss[nCntBoss].bDrw = false;

					nCntMove = 0;
					nCntpat = 0;
					SetParticle(g_aBoss[nCntBoss].pos, 1, 30, 90.0f, 0);

					g_aBoss[nCntBoss].m_pattern = 99;
					g_aBoss[nCntBoss].state_pattern++;
				}
			}
		}*/

		if (g_aBoss[1].curt_nLife <= g_aBoss[1].init_nLife / 2)
		{
			g_aBoss[1].s_pattern = BULLETTYPE_E_3_SHOT;
			PlaySound(SOUND_LABEL_SE_SUMMON);
		}
		else if (g_aBoss[2].curt_nLife <= g_aBoss[2].init_nLife / 2)
		{
			g_aBoss[2].s_pattern = BULLETTYPE_E_CHARGE;
			PlaySound(SOUND_LABEL_SE_SUMMON);
		}
		PlaySound(SOUND_LABEL_SE_HIT); //←再生したいサウンドを指定



		VERTEX_2D *pVtx;	//設定用ポインタ

		//頂点バッファのロックと頂点情報へのポインタを取得
		g_pVtxBuffBoss->Lock(0, 0, (void **)&pVtx, 0);



		pVtx += nCntBoss * 4;

		//頂点バッファをアンロック
		g_pVtxBuffBoss->Unlock();
	}
}
//◇======================◇
//｜　　 ボスの所得　　 　｜
//◇======================◇
Boss *GetBoss(void)
{
	return &g_aBoss[0];//敵の情報の先頭アドレスを返す
}
//◇============================◇
//｜　　 ボスの総数の所得　　 　｜
//◇============================◇
int GetNumBoss(void)
{
	return g_nNumBoss;
}
//◇============================◇
//｜　　 チャージ率の所得　　 　｜
//◇============================◇
int GetnCharge(void)
{
	return nCharge;
}
//◇==============================◇
//｜　　 出現カウントの所得　　 　｜
//◇==============================◇
int GetinCnt(void)
{
	return g_inCnt;
}
//◇============================◇
//｜　　　 データ読み込み　　 　｜
//◇============================◇
void LoadBoss(void)
{
	int c = 0;
	int column = 1;//行数を数える変数
	int line = 0;//列数を数える変数
	int nCntBoss; //カウント変数

	char aData[100];
	FILE *pFile;

	memset(aData, 0, sizeof(aData));
	pFile = fopen("data/csv/boosdata.csv", "r");

	if (pFile == NULL)
	{
		/*printf("ファイルオープンエラー\n");*/
	}

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
		case 1:		g_aBoss[line].nType = atoi(aData);			break;		//１列目：敵・種類
		case 2:		g_aBoss[line].sType = atoi(aData);			break;		//  列目：弾・種類
		case 3:		g_aBoss[line].m_pattern = atoi(aData);		break;		//　列目：移動パターン
		case 4:		g_aBoss[line].s_pattern = atoi(aData);		break;		//　列目：発射パターン
		case 5:		g_aBoss[line].in_time = atoi(aData);		break;		//　列目：出現時間
		case 6:		g_aBoss[line].stop_time = atoi(aData);		break;		//　列目：停止時間
		case 7:		g_aBoss[line].shot_time = atoi(aData);		break;		//　列目：弾 発射時間
		case 8:		g_aBoss[line].out_time = atoi(aData);		break;		//停止時間(次の行動時間)
		case 9:		g_aBoss[line].pos.x = (float)(atoi(aData));	break;		//　列目：X座標
		case 10:	g_aBoss[line].pos.y = (float)(atoi(aData));	break;		//　列目：Y座標
		case 11:	g_aBoss[line].curt_nLife = atoi(aData);					//　列目：体力　（構造体　現在の体力）
					g_aBoss[line].init_nLife = atoi(aData);		break;		//				（構造体　初期体力）
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

	//ボスクラスの生成

	for (nCntBoss = 0; nCntBoss < 3; nCntBoss++)
	{
		SetBoss(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x, g_aBoss[nCntBoss].pos.y, 0), nCntBoss);
		g_aBoss[nCntBoss].bUse = true;	//ボスを使用している状態にする
		g_nNumBoss++;						//敵の総数を加算する
	}
	g_aBoss[0].bDrw = true;
}
//◇==========================◇
//｜　　　 敵の設定処理2　 　｜
//◇==========================◇
void SetBoss(D3DXVECTOR3 pos, int nCntBoss)
{
	if (g_aBoss[nCntBoss].nType == 1)
	{
		g_aBoss[nCntBoss].score = 1000;
	}
	else if (g_aBoss[nCntBoss].nType == 2)
	{
		g_aBoss[nCntBoss].score = 2000;
	}
	else if (g_aBoss[nCntBoss].nType == 3)
	{
		g_aBoss[nCntBoss].score = 3000;
	}
	else if (g_aBoss[nCntBoss].nType == 4)
	{
		g_aBoss[nCntBoss].score = 2000;
	}
	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxBuffBoss->Lock(0, 0, (void **)&pVtx, 0);

	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇

	pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - SIZE_BOSS / 2, g_aBoss[nCntBoss].pos.y - SIZE_BOSS / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + SIZE_BOSS / 2, g_aBoss[nCntBoss].pos.y - SIZE_BOSS / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - SIZE_BOSS / 2, g_aBoss[nCntBoss].pos.y + SIZE_BOSS / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + SIZE_BOSS / 2, g_aBoss[nCntBoss].pos.y + SIZE_BOSS / 2, 0.0f);

	//頂点バッファのアンロック
	g_pVtxBuffBoss->Unlock();
}
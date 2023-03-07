//◇====================◇
//｜　　　　　　　　　　｜
//｜　敵の処理          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"game.h"
#include"enemy.h"
#include"bullet.h"
#include"explosion.h"
#include"score.h"
#include"sound.h"
#include"player.h"
#include"enemy_bullet.h"
#include"particle.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define NUM_ENEMY (5) //敵の種類

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void CollisionEnemy(Enemy *pEnemy);
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DTEXTURE9 g_pTextureEnemy[NUM_ENEMY] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;		//頂点バッファポインタ
Enemy g_aEnemy[MAX_ENEMY];							//敵の情報

int g_CntTime;										//経過時間
int g_nCount;										//一定時間
int g_nCntSkip;										//スキップしてからの間の時間
int g_nNumEnemy;									//現在いる敵の数
int g_nTotalEnemy;									//敵の総数
int g_nArrivalEnemy;								//出現した敵の数

bool SkipType;										//スキップ種類
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【敵・処理】｜――――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇============================◇
//｜　　　 敵の初期化処理　　 　｜
//◇============================◇
void InitEnemy(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntEnemy;
	SkipType = false;
	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\ghost.png",
		&g_pTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy_a.png",
		&g_pTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy2.png",
		&g_pTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy3b.png",
		&g_pTextureEnemy[3]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy4.png",
		&g_pTextureEnemy[4]);

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void **)&pVtx, 0);

	//敵の情報の初期化処理
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(1000.0f, 500.0f, 0.0f);
		g_aEnemy[nCntEnemy].move.x = 0.3f;
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//向きを初期化する(Z値を使用)
		g_aEnemy[nCntEnemy].nBullet = 0;
		g_aEnemy[nCntEnemy].nLife = 1;
		g_aEnemy[nCntEnemy].time = 1;
		g_aEnemy[nCntEnemy].Tracking = 90;
		g_aEnemy[nCntEnemy].nCntMove = 0;
		g_aEnemy[nCntEnemy].stand_time = 62;
		g_aEnemy[nCntEnemy].score = 0;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].Reversal = false;
		g_aEnemy[nCntEnemy].m_switch = false;
		g_aEnemy[nCntEnemy].bHit = false;
		g_aEnemy[nCntEnemy].bDrw = false;
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

	g_CntTime = 0;		//経過時間をクリア
	g_nCount = 0;		//一定時間をクリア
	g_nCntSkip = 15;	//スキップしてからの間の時間
	g_nNumEnemy = 0;	//現在いる敵の数
	g_nTotalEnemy = 0;	//敵の総数
	g_nArrivalEnemy = 0;	//
	//頂点バッファのアンロック
	g_pVtxBuffEnemy->Unlock();

	//敵情報を読み込む
	LoadEnemy();
}
//◇==========================◇
//｜　　　 敵の終了処理　　 　｜
//◇==========================◇
void UninitEnemy(void)
{
	int nCntEnemy;
	//サウンドの停止
	StopSound();
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		if (g_pTextureEnemy[nCntEnemy] != NULL)
		{
			g_pTextureEnemy[nCntEnemy]->Release();
			g_pTextureEnemy[nCntEnemy] = NULL;
		}
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}
//◇==========================◇
//｜　　　 敵の更新処理　　 　｜
//◇==========================◇
void UpdateEnemy(void)
{
	int nCntEnemy;		//敵カウンタ
	Player *pPlayer = Getplayer();	//プレイヤーの情報へのポインタ

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void **)&pVtx, 0);

	if (g_nNumEnemy <= 0)
	{
		if (SkipType == false)
		{
			g_CntTime = g_aEnemy[0].in_time;
			SkipType = true;
		}
		else if (SkipType == true)
		{
			g_CntTime = g_aEnemy[g_nArrivalEnemy + 1].in_time;
		}
	}
	else
	{
		g_CntTime++;
	}
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pVtx += 4)
	{
		//状態遷移処理
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			CollisionEnemy(&g_aEnemy[nCntEnemy]);

			D3DXVECTOR3 movevec;
			switch (g_aEnemy[nCntEnemy].m_pattern)
			{
				//=============================
				//上から出現する移動パターン
				//=============================

			case 0:
				//出て来てから止まる時間までの間なら下に移動
				if (g_aEnemy[nCntEnemy].in_time < g_CntTime && g_CntTime < g_aEnemy[nCntEnemy].stop_time)
				{
					g_aEnemy[nCntEnemy].pos.y += 2;
				}
				else if (g_CntTime == g_aEnemy[nCntEnemy].stop_time)
				{
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5, 0.0f), BULLETTYPE_E_LINE, g_aEnemy[nCntEnemy].sType);
				}
				//帰還時間を過ぎたら戻る
				else if (g_CntTime > g_aEnemy[nCntEnemy].out_time)
				{
					g_aEnemy[nCntEnemy].pos.y -= 2;
				}
				break;

				//止まらず下にいくパターン
			case 1:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					g_aEnemy[nCntEnemy].pos.y += 2;
				}
				break;

				//少しずつ左に移動しながら消えていく
			case 2:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					g_aEnemy[nCntEnemy].pos.y += 2;
					if (g_nCount % 10 == 0)
					{
						g_aEnemy[nCntEnemy].pos.x -= 1;
					}
				}
				break;

				//少しずつ右に移動しながら消えていく
			case 3:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					g_aEnemy[nCntEnemy].pos.y += 2;
					if (g_nCount % 10 == 0)
					{
						g_aEnemy[nCntEnemy].pos.x += 1;
					}
				}
				break;

				//少しずつ左に移動し途中で帰還する
			case 4:
				if (g_aEnemy[nCntEnemy].in_time < g_CntTime && g_CntTime < g_aEnemy[nCntEnemy].stop_time)
				{
					if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
					{
						g_aEnemy[nCntEnemy].pos.y += 2;
						if (g_nCount % 10 == 0)
						{
							g_aEnemy[nCntEnemy].pos.x -= 1;
						}
					}
				}
				//少し左に移動する
				else if (g_aEnemy[nCntEnemy].stop_time < g_CntTime && g_CntTime < g_aEnemy[nCntEnemy].out_time)
				{
					g_aEnemy[nCntEnemy].pos.x += 2;
				}
				//帰還時間を過ぎたら戻る
				else if (g_CntTime > g_aEnemy[nCntEnemy].out_time)
				{
					g_aEnemy[nCntEnemy].pos.y -= 2;
					if (g_nCount % 10 == 0)
					{
						g_aEnemy[nCntEnemy].pos.x += 1;
					}
				}
				break;

				//少しずつ右に移動し途中で帰還する
			case 5:
				if (g_aEnemy[nCntEnemy].in_time < g_CntTime && g_CntTime < g_aEnemy[nCntEnemy].stop_time)
				{
					if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
					{
						g_aEnemy[nCntEnemy].pos.y += 2;
						if (g_nCount % 10 == 0)
						{
							g_aEnemy[nCntEnemy].pos.x += 1;
						}
					}
				}
				//少し左に移動する
				else if (g_aEnemy[nCntEnemy].stop_time < g_CntTime && g_CntTime < g_aEnemy[nCntEnemy].out_time)
				{
					g_aEnemy[nCntEnemy].pos.x -= 2;
				}
				//帰還時間を過ぎたら戻る
				else if (g_CntTime > g_aEnemy[nCntEnemy].out_time)
				{
					g_aEnemy[nCntEnemy].pos.y -= 2;
					if (g_nCount % 10 == 0)
					{
						g_aEnemy[nCntEnemy].pos.x -= 1;
					}
				}
				break;

				//プレイヤーの方向に移動する
			case 6:
				//出て来てから止まる時間までの間なら下に移動
				if (g_aEnemy[nCntEnemy].in_time < g_CntTime && g_CntTime < g_aEnemy[nCntEnemy].stop_time)
				{
					g_aEnemy[nCntEnemy].pos.y += 2;
				}
				//停止時間過ぎたらプレイヤーの方向に移動する
				else if (g_aEnemy[nCntEnemy].stop_time < g_CntTime)
				{
					if (g_aEnemy[nCntEnemy].time >= 1 && pPlayer->bUse == true)
					{
						movevec = pPlayer->pos - g_aEnemy[nCntEnemy].pos;	//エネミーからプレイヤーまでの方向ベクトルを求める

						D3DXVec3Normalize(&movevec, &movevec);				//ベクトルを正規化する(ベクトルを１にする) (正当化　, 正当化する前)

						movevec = movevec * 10;								//正当化した物に移動量を与える

						g_aEnemy[nCntEnemy].pos += movevec;					//弾の位置を更新する
						g_aEnemy[nCntEnemy].move = movevec;
						g_aEnemy[nCntEnemy].time--;
					}
					else if (pPlayer->bUse == false)
					{
						g_aEnemy[nCntEnemy].pos.y += 2;
					}
					else
					{
						//弾の位置更新
						g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
						g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
					}
				}
				break;
				//ジグザグに移動
			case 7:
					//出て来てから止まる時間までの間なら下に移動
					if (g_aEnemy[nCntEnemy].in_time < g_CntTime)
					{
						g_aEnemy[nCntEnemy].nCntMove++;

							if (g_aEnemy[nCntEnemy].Reversal == false)
							{
								g_aEnemy[nCntEnemy].pos.x -= 3;
								g_aEnemy[nCntEnemy].pos.y += 2;
								if (g_aEnemy[nCntEnemy].pos.x < 10)
								{
									g_aEnemy[nCntEnemy].Reversal = true;
									g_aEnemy[nCntEnemy].nCntMove = 0;

								}
								else if (g_aEnemy[nCntEnemy].nCntMove >= 120)
								{
									g_aEnemy[nCntEnemy].Reversal = true;
									g_aEnemy[nCntEnemy].nCntMove = 0;
								}
							}
							else if (g_aEnemy[nCntEnemy].Reversal == true)
							{
								g_aEnemy[nCntEnemy].pos.x += 3;
								g_aEnemy[nCntEnemy].pos.y += 2;

								if (g_aEnemy[nCntEnemy].pos.x > 950)
								{
									g_aEnemy[nCntEnemy].Reversal = false;
									g_aEnemy[nCntEnemy].nCntMove = 0;
								}
								else if (g_aEnemy[nCntEnemy].nCntMove >= 120)
								{
									g_aEnemy[nCntEnemy].Reversal = false;
									g_aEnemy[nCntEnemy].nCntMove = 0;
								}
							}
					}
					break;
			case 71:
				//出て来てから止まる時間までの間なら下に移動
				if (g_aEnemy[nCntEnemy].in_time < g_CntTime)
				{
					g_aEnemy[nCntEnemy].nCntMove++;

					if (g_aEnemy[nCntEnemy].Reversal == false)
					{
						g_aEnemy[nCntEnemy].pos.x += 3;
						g_aEnemy[nCntEnemy].pos.y += 2;
						if (g_aEnemy[nCntEnemy].pos.x < 10)
						{
							g_aEnemy[nCntEnemy].Reversal = true;
							g_aEnemy[nCntEnemy].nCntMove = 0;

						}
						else if (g_aEnemy[nCntEnemy].nCntMove >= 120)
						{
							g_aEnemy[nCntEnemy].Reversal = true;
							g_aEnemy[nCntEnemy].nCntMove = 0;
						}
					}
					else if (g_aEnemy[nCntEnemy].Reversal == true)
					{
						g_aEnemy[nCntEnemy].pos.x -= 3;
						g_aEnemy[nCntEnemy].pos.y += 2;

						if (g_aEnemy[nCntEnemy].pos.x > 950)
						{
							g_aEnemy[nCntEnemy].Reversal = false;
							g_aEnemy[nCntEnemy].nCntMove = 0;
						}
						else if (g_aEnemy[nCntEnemy].nCntMove >= 120)
						{
							g_aEnemy[nCntEnemy].Reversal = false;
							g_aEnemy[nCntEnemy].nCntMove = 0;
						}
					}
				}
				break;
				//==============================
				//横から出現する移動パターン
				//=============================

				//止まらず横に移動する(左から右)
			case 8:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					g_aEnemy[nCntEnemy].pos.x += 2;
				}
				break;
				//〃（右から左）
			case 9:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					g_aEnemy[nCntEnemy].pos.x -= 2;
				}
				break;
				//プレイヤーとX軸が一致するまで横に移動する(左)
			case 10:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					if (g_aEnemy[nCntEnemy].m_switch == false)
					{
						g_aEnemy[nCntEnemy].nBullet++;//カウンターを加算
						if ((g_aEnemy[nCntEnemy].nBullet % 60) == 0)
						{
							g_aEnemy[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
							SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5, 0.0f), BULLETTYPE_E_LINE, g_aEnemy[nCntEnemy].sType);
						}
						g_aEnemy[nCntEnemy].pos.x += 2;

						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 32 &&
							pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 32
							&& pPlayer->bUse == true)
						{
							g_aEnemy[nCntEnemy].m_switch = true;
						}
					}
					//一致したら下に移動する
					else if (g_aEnemy[nCntEnemy].m_switch == true)
					{
						g_aEnemy[nCntEnemy].pos.y += 15;
					}
				}
				break;
				//プレイヤーとX軸が一致するまで横に移動する(右)
			case 11:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					if (g_aEnemy[nCntEnemy].m_switch == false)
					{
						g_aEnemy[nCntEnemy].nBullet++;//カウンターを加算
						if ((g_aEnemy[nCntEnemy].nBullet % 60) == 0)
						{
							g_aEnemy[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
							SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5, 0.0f), BULLETTYPE_E_LINE, g_aEnemy[nCntEnemy].sType);
						}
						g_aEnemy[nCntEnemy].pos.x -= 2;

						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 32 &&
							pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 32
							&& pPlayer->bUse == true)
						{
							g_aEnemy[nCntEnemy].m_switch = true;
						}
					}
					//一致したら下に移動する
					else if (g_aEnemy[nCntEnemy].m_switch == true)
					{
						g_aEnemy[nCntEnemy].pos.y += 15;
					}
				}
				break;
				//プレイヤーとX軸が一致するまで横に移動する(左)
			case 12:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					if (g_aEnemy[nCntEnemy].m_switch == false)
					{
						g_aEnemy[nCntEnemy].nBullet++;//カウンターを加算
						if ((g_aEnemy[nCntEnemy].nBullet % 60) == 0)
						{
							g_aEnemy[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
							SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5, 0.0f), BULLETTYPE_E_LINE, g_aEnemy[nCntEnemy].sType);
						}
						g_aEnemy[nCntEnemy].pos.x += 2;

						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 64 &&
							pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 64
							&& pPlayer->bUse == true)
						{
							g_aEnemy[nCntEnemy].m_switch = true;
						}
					}
					//一致したら1分半追尾するする
					else if (g_aEnemy[nCntEnemy].m_switch == true)
					{
						if (g_aEnemy[nCntEnemy].Tracking >= 1)
						{
							movevec = pPlayer->pos - g_aEnemy[nCntEnemy].pos;	//弾からプレイヤーまでの方向ベクトルを求める

							D3DXVec3Normalize(&movevec, &movevec);				//ベクトルを正規化する(ベクトルを１にする) (正当化　, 正当化する前)

							movevec = movevec * 5;								//正当化した物に移動量を与える

							g_aEnemy[nCntEnemy].pos += movevec;					//弾の位置を更新する
							g_aEnemy[nCntEnemy].move = movevec;
							g_aEnemy[nCntEnemy].Tracking--;
						}
						//０秒になったら直進する
						else if (g_aEnemy[nCntEnemy].Tracking == 0)
						{
							//弾の位置更新
							g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
							g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
						}
					}
				}
				break;
				//プレイヤーとX軸が一致するまで横に移動する(右)
			case 13:
				if (g_aEnemy[nCntEnemy].in_time <= g_CntTime)
				{
					if (g_aEnemy[nCntEnemy].m_switch == false)
					{
						g_aEnemy[nCntEnemy].nBullet++;//カウンターを加算
						if ((g_aEnemy[nCntEnemy].nBullet % 60) == 0)
						{
							g_aEnemy[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
							SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5, 0.0f), BULLETTYPE_E_LINE, g_aEnemy[nCntEnemy].sType);
						}
						g_aEnemy[nCntEnemy].pos.x -= 2;

						if (pPlayer->pos.x >= g_aEnemy[nCntEnemy].pos.x - 64 &&
							pPlayer->pos.x <= g_aEnemy[nCntEnemy].pos.x + 64
							&& pPlayer->bUse == true)
						{
							g_aEnemy[nCntEnemy].m_switch = true;
						}
					}
					//一致したら下に移動する
					else if (g_aEnemy[nCntEnemy].m_switch == true)
					{
						if (g_aEnemy[nCntEnemy].Tracking >= 1)
						{
							movevec = pPlayer->pos - g_aEnemy[nCntEnemy].pos;	//弾からプレイヤーまでの方向ベクトルを求める

							D3DXVec3Normalize(&movevec, &movevec);				//ベクトルを正規化する(ベクトルを１にする) (正当化　, 正当化する前)

							movevec = movevec * 5;								//正当化した物に移動量を与える

							g_aEnemy[nCntEnemy].pos += movevec;					//弾の位置を更新する
							g_aEnemy[nCntEnemy].move = movevec;
							g_aEnemy[nCntEnemy].Tracking--;
						}
						else if (g_aEnemy[nCntEnemy].Tracking == 0)
						{
							//弾の位置更新
							g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
							g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
						}
					}
				}
				break;
			}

			//画面からはみ出したら敵を非表示にする
			if (g_aEnemy[nCntEnemy].pos.x < -30 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - 220)
			{
				//X座標が範囲外の場合
				g_aEnemy[nCntEnemy].bDrw = false; //使用していない状態にする
			}
			if (g_aEnemy[nCntEnemy].pos.y < -45 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT)
			{
				//Y座標が範囲外の場合
				g_aEnemy[nCntEnemy].bDrw = false; //使用していない状態にする
			}

			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
					//◇――――――――――――◇
					//｜　　　 頂点カラー 　　　｜
					//◇――――――――――――◇
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//ポリゴンを通常色に戻す
				}
				break;
			}


			//g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - SIZE_ENEMY / 2;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - SIZE_ENEMY / 2;
			pVtx[0].pos.z = 0;
			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + SIZE_ENEMY / 2;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - SIZE_ENEMY / 2;
			pVtx[1].pos.z = 0;
			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - SIZE_ENEMY / 2;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + SIZE_ENEMY / 2;
			pVtx[2].pos.z = 0;
			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + SIZE_ENEMY / 2;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + SIZE_ENEMY / 2;
			pVtx[3].pos.z = 0;

			
			switch (g_aEnemy[nCntEnemy].s_pattern)
			{
			case BULLETTYPE_E_LINE:
				g_aEnemy[nCntEnemy].nBullet++;//カウンターを加算
				if ((g_aEnemy[nCntEnemy].nBullet % 60) == 0)
				{
					g_aEnemy[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5, 0.0f), BULLETTYPE_E_LINE, g_aEnemy[nCntEnemy].sType);
				}
				break;

			case BULLETTYPE_E_AIM:
				g_aEnemy[nCntEnemy].nBullet++;//カウンターを加算
				if ((g_aEnemy[nCntEnemy].nBullet % 90) == 0)
				{
					g_aEnemy[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5., 0.0f), BULLETTYPE_E_AIM, g_aEnemy[nCntEnemy].sType);
				}
				break;

			case BULLETTYPE_E_HOMING:
				g_aEnemy[nCntEnemy].nBullet++;//カウンターを加算
				if ((g_aEnemy[nCntEnemy].nBullet % 90) == 0)
				{
					g_aEnemy[nCntEnemy].nBullet = 0;//カウンターを初期値に戻す
					SetEnemyBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 5., 0.0f), BULLETTYPE_E_HOMING, g_aEnemy[nCntEnemy].sType);
				}
				break;
			case 99:
				break;
			}
		}
		else if (g_aEnemy[nCntEnemy].bUse == false)
		{
			if (g_aEnemy[nCntEnemy].in_time == g_CntTime)
			{
				pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - SIZE_ENEMY / 2;
				pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - SIZE_ENEMY / 2;
				pVtx[0].pos.z = 0;
				pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + SIZE_ENEMY / 2;
				pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - SIZE_ENEMY / 2;
				pVtx[1].pos.z = 0;
				pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - SIZE_ENEMY / 2;
				pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + SIZE_ENEMY / 2;
				pVtx[2].pos.z = 0;
				pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + SIZE_ENEMY / 2;
				pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + SIZE_ENEMY / 2;
				pVtx[3].pos.z = 0;

				g_aEnemy[nCntEnemy].bUse = true;	//敵が使用している状態にする
				g_aEnemy[nCntEnemy].bHit = true;	//敵の当たり判定をオンにする
				g_aEnemy[nCntEnemy].bDrw = true;	//敵を表示する
				g_nNumEnemy++;						//現在いる敵の数を加算
				g_nArrivalEnemy++;					//出現した敵の数加算
			}
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}
//◇==========================◇
//｜　　　 敵の描画処理　　 　｜
//◇==========================◇
void DrawEnemy(void)
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
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bDrw == true)
		{//敵が使用されている

			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//◇==========================◇
//｜　　　 敵の判定処理　　 　｜
//◇==========================◇

//敵の当たり判定
void CollisionEnemy(Enemy *pEnemy)
{
	int nCntEnemy;

	Player *pPlayer = Getplayer(); //プレイヤーの情報へのポインタ

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bHit == true && pPlayer->bUse == true)
		{//敵が使用されている
			if (pEnemy->pos.x >= pPlayer->pos.x - 32
				&& pEnemy->pos.x <= pPlayer->pos.x + 32
				&& pEnemy->pos.y >= pPlayer->pos.y - 32
				&& pEnemy->pos.y <= pPlayer->pos.y + 32)
			{//敵とプレイヤーが当たった

				HitPlayer(nCntEnemy, 1);

			}
		}
	}
}
//◇============================◇
//｜　　　 敵のヒット処理　　 　｜
//◇============================◇
void HitEnemy(int nCntEnemy, int nDamage)
{
	if (g_aEnemy[nCntEnemy].bHit == true)
	{
		g_aEnemy[nCntEnemy].nLife -= nDamage;

		if (g_aEnemy[nCntEnemy].nLife <= 0)//敵の体力がなくなった
		{
			//爆発の設定
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case 0:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 20, 10.0f, 0,0);
				break;
			case 1:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 20, 10.0f, 0,0);
				break;
			case 2:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 20, 10.0f, 1,0);
				break;
			case 3:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 20, 10.0f, 4, 0);
				break;
			case 4:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 20, 10.0f, 5, 0);
				break;
			}

			//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			PlaySound(SOUND_LABEL_SE_EXPLOSION); //←再生したいサウンドを指定

			g_aEnemy[nCntEnemy].bUse = false;
			g_aEnemy[nCntEnemy].bDrw = false;
			g_aEnemy[nCntEnemy].bHit = false;

			g_nNumEnemy--;		//現在の敵の数
			g_nTotalEnemy--;	//敵の総数を減らす
			AddScore(g_aEnemy[nCntEnemy].score);
		}

		else
		{
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			PlaySound(SOUND_LABEL_SE_HIT); //←再生したいサウンドを指定
			g_aEnemy[nCntEnemy].nCounterState = 10; //ダメージを保つ時間を設定

			VERTEX_2D *pVtx;	//設定用ポインタ

			//頂点バッファのロックと頂点情報へのポインタを取得
			g_pVtxBuffEnemy->Lock(0, 0, (void **)&pVtx, 0);

			switch (g_aEnemy[nCntEnemy].nType)
			{
			case 0:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 5, 6.0f, 0,0);
				break;
			case 1:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 5, 6.0f, 0,0);
				break;
			case 2:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 5, 6.0f, 1,0);
				break;
			case 3:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 20, 10.0f, 4, 0);
				break;
			case 4:
				SetParticle(g_aEnemy[nCntEnemy].pos, 0, 20, 10.0f, 5, 0);
				break;
			}

			pVtx += nCntEnemy * 4;
			//◇――――――――――――◇
			//｜　　　 頂点カラー 　　　｜
			//◇――――――――――――◇
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f); //ポリゴンのダメージ色に設定

			//頂点バッファをアンロック
			g_pVtxBuffEnemy->Unlock();
		}
	}
}
//◇====================◇
//｜　 敵の総数の取得　 ｜
//◇====================◇
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
//◇======================◇
//｜　　　 敵の所得　　 　｜
//◇======================◇
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];//敵の情報の先頭アドレスを返す
}
//◇================◇
//｜ 経過時間の所得 ｜
//◇================◇
int GetCunt(void)
{
	return g_CntTime;
}
//◇====================================◇
//｜　　　 エネミーデータ読み込み　　 　｜
//◇====================================◇
void LoadEnemy(void)
{
	int c = 0;	   //1文字ずつ確認する変数
	int column = 1;//列数を数える変数
	int line   = 0;//行数を数える変数
	int nCntEnemy; //カウント変数

	char aData[100];//つなげる文字数
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data/csv/enemydata.csv", "r");

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
		case 1:		g_aEnemy[line].nType = atoi(aData);			 break;	//１列目：敵・種類
		case 2:		g_aEnemy[line].sType = atoi(aData);			 break;	//  列目：弾・種類
		case 3:		g_aEnemy[line].m_pattern = atoi(aData);		 break;	//　列目：移動パターン
		case 4:		g_aEnemy[line].s_pattern = atoi(aData);		 break;	//　列目：発射パターン
		case 5:		g_aEnemy[line].in_time = atoi(aData);		 break;	//　列目：出現時間
		case 6:		g_aEnemy[line].stop_time = atoi(aData);		 break;	//　列目：停止時間
		case 7:		g_aEnemy[line].shot_time = atoi(aData);		 break;	//　列目：弾 発射時間
		case 8:		g_aEnemy[line].out_time = atoi(aData);		 break;	//　列目：帰還時間
		case 9:		g_aEnemy[line].pos.x = (float)(atoi(aData)); break;	//　列目：X座標
		case 10:	g_aEnemy[line].pos.y = (float)(atoi(aData)); break;	//　列目：Y座標
		case 11:	g_aEnemy[line].nLife = atoi(aData);			 break;	//　列目：体力
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
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].nType == 1)
		{
			g_aEnemy[nCntEnemy].score = 100;
		}
		else if (g_aEnemy[nCntEnemy].nType == 2)
		{

			g_aEnemy[nCntEnemy].score = 200;
		}
		else if (g_aEnemy[nCntEnemy].nType == 3)
		{
			g_aEnemy[nCntEnemy].score = 300;
		}
		else if (g_aEnemy[nCntEnemy].nType == 4)
		{
			g_aEnemy[nCntEnemy].score = 50;
		}
	}
}
//◇==========================◇
//｜　　　 敵の設定処理1　 　｜
//◇==========================◇
//void SetEnemy(D3DXVECTOR3 pos, int nType)
//{
//	int nCntEnemy;
//
//	//設定用ポインタ
//	VERTEX_2D *pVtx;
//
//	//頂点バッファのロック
//	g_pVtxBuffSummon->Lock(0, 0, (void **)&pVtx, 0);
//
//	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
//	{
//		if (g_aEnemy[nCntEnemy].bUse == false)
//		{
//			g_aEnemy[nCntEnemy].pos = pos;
//			g_aEnemy[nCntEnemy].nType = nType;
//			g_aEnemy[nCntEnemy].move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
//			g_aEnemy[nCntEnemy].curt_nLife = 2;
//
//			//◇――――――――――――◇
//			//｜ 　　頂点座標を設定　　 ｜
//			//◇――――――――――――◇
//
//			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - SIZE_ENEMY / 2, g_aEnemy[nCntEnemy].pos.y - SIZE_ENEMY / 2, 0.0f);
//			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + SIZE_ENEMY / 2, g_aEnemy[nCntEnemy].pos.y - SIZE_ENEMY / 2, 0.0f);
//			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - SIZE_ENEMY / 2, g_aEnemy[nCntEnemy].pos.y + SIZE_ENEMY / 2, 0.0f);
//			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + SIZE_ENEMY / 2, g_aEnemy[nCntEnemy].pos.y + SIZE_ENEMY / 2, 0.0f);
//
//
//			g_nNumSummon++; //敵の総数を加算する
//			g_aEnemy[nCntEnemy].bUse = true; //弾が使用している状態にする
//
//			break;
//		}
//		pVtx += 4; //該当の位置まで進める
//	}
//	//頂点バッファのアンロック
//	g_pVtxBuffSummon->Unlock();
//}
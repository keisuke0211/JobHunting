//◇====================◇
//｜　　　　　　　　　　｜
//｜　弾の処理          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "bullet.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "wall.h"
#include "effect.h"
#include"figure_model.h"
#include "enemy_model.h"
#include "particle.h"
#include "fade.h"
#include "damage.h"
#include "game.h"
#include "sound.h"
//****************************************
// マクロ定義
//****************************************
#define MAX_BULLET			(128)			//ビルボードの最大数
#define BULLET_LIFE			(100)			//弾の寿命

//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBllet = NULL;		//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBllet = NULL;			//テクスチャのポインタ
Bullet g_aBullet[MAX_BULLET];

void CollisionWall(Bullet *pBullet);
void CollisionEnemy(Bullet *pBullet);
void CollisionFigure(Bullet *pBullet);
//****************************************
// 弾の初期化処理
//****************************************
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスの取得	

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shot.PNG",
		&g_pTextureBllet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = BULLET_LIFE;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].fWidht = BULLET_SIZE_WIDTH;
		g_aBullet[nCntBullet].fHight = BULLET_SIZE_HEIGHT;

		g_aBullet[nCntBullet].nIdxShadow = SetShadow();		//影の設定

	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&g_pVtxBuffBllet, NULL);

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBllet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pVtx += 4)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidht, +g_aBullet[nCntBullet].fHight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].fWidht, +g_aBullet[nCntBullet].fHight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidht, -g_aBullet[nCntBullet].fHight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aBullet[nCntBullet].fWidht, -g_aBullet[nCntBullet].fHight, 0.0f);

		//norの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffBllet->Unlock();
}

//****************************************
// 弾の終了処理
//****************************************
void UninitBullet(void)
{
	//サウンドの停止
	StopSound();

	//テクスチャの破棄
	if (g_pTextureBllet != NULL)
	{
		g_pTextureBllet->Release();
		g_pTextureBllet = NULL;
	}

	//バッファの破棄
	if (g_pVtxBuffBllet != NULL)
	{
		g_pVtxBuffBllet->Release();
		g_pVtxBuffBllet = NULL;
	}

}

//****************************************
// 弾の更新処理
//****************************************
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{

		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されてる

			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//弾の更新処理
			g_aBullet[nCntBullet].move.x = sinf(g_aBullet[nCntBullet].rot.y) * -50.0;
			g_aBullet[nCntBullet].move.z = cosf(g_aBullet[nCntBullet].rot.y) * -50.0;

			//位置を更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//エフェクト
			SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.1f, 1.0f), 15.0f, 20, 0);

			//影描画
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].fWidht, g_aBullet[nCntBullet].fHight);

			//*** 当たり判定 ***

			//	壁
			CollisionWall(&g_aBullet[nCntBullet]);
			//置物
			CollisionFigure(&g_aBullet[nCntBullet]);
			// 敵
			CollisionEnemy(&g_aBullet[nCntBullet]);

			//寿命の減少
			g_aBullet[nCntBullet].nLife -= 1;

			if (g_aBullet[nCntBullet].nLife == 0)
			{//寿命が尽きた
				g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
			}
		}
	}
}

//****************************************
// 弾の描画処理
//****************************************
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX  mtxTrans, mtxView;			//計算用マトリックス

											//デバイスの取得
	pDevice = GetDevice();

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);//逆行列を求める
																			
			g_aBullet[nCntBullet].mtxWorld._41 = DEFAULT_FLOA;
			g_aBullet[nCntBullet].mtxWorld._42 = DEFAULT_FLOA;
			g_aBullet[nCntBullet].mtxWorld._43 = DEFAULT_FLOA;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBllet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBllet);

			//描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//****************************************
// 弾の設定処理
//****************************************
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntBllet = 0; nCntBllet < MAX_BULLET; nCntBllet++)
	{
		if (g_aBullet[nCntBllet].bUse == false)
		{//弾が使用されていない
			g_aBullet[nCntBllet].pos = pos;
			g_aBullet[nCntBllet].rot = rot;
			g_aBullet[nCntBllet].nLife = BULLET_LIFE;
			g_aBullet[nCntBllet].bUse = true;//使用されている状態にする
			break;
		}
	}
}
//****************************************
// 当たり判定
//****************************************

// ***** 壁 *****
void CollisionWall(Bullet *pBullet)
{
	Wall *pWall = GetWall();
	BreakWall *pBreakWall = GetBreakWall();
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
			if (pBullet->pos.x >= pWall->pos.x - (pWall->fWidth + BULLET_SIZE_WIDTH) &&
				pBullet->pos.x <= pWall->pos.x + (pWall->fWidth + BULLET_SIZE_WIDTH) &&
				pBullet->pos.y >= pWall->pos.y - (pWall->fHight + BULLET_SIZE_HEIGHT) &&
				pBullet->pos.y <= pWall->pos.y + (pWall->fHight + BULLET_SIZE_HEIGHT) &&
				pBullet->pos.z >= pWall->pos.z - (pWall->fWidth + BULLET_SIZE_WIDTH) &&
				pBullet->pos.z <= pWall->pos.z + (pWall->fWidth + BULLET_SIZE_WIDTH))
			{
				//透明の壁の判定
				if (pWall->nType == 2)
				{

				}

				//通常と白壁の判定
				else if (pWall->nCype == 0 || pWall->nCype == 1)
				{
					if (pWall->pos.y + pWall->fHight >= pBullet->posOld.y + BULLET_SIZE_HEIGHT &&
						pWall->pos.y - pWall->fHight <= pBullet->posOld.y - BULLET_SIZE_HEIGHT &&
						pWall->pos.z - 2 >= pBullet->posOld.z + BULLET_SIZE_WIDTH &&
						pWall->pos.z - 2 <= pBullet->pos.z + BULLET_SIZE_WIDTH)
					{
						pBullet->bUse = false;

						// 影・削除
						DeleteShadow(pBullet->nIdxShadow);
						if (pWall->nType == 1)
						{
							if (pWall->nLife == 1)
							{
								pWall->nLife--;
								pWall[- 1].nLife--;
								pWall[+ 1].nLife--;
								PlaySound(SOUND_LABEL_BREAK); //壁が壊れる音
								SetParticle(D3DXVECTOR3(pWall->pos.x, pWall->pos.y + (pWall->fHight / 3), pWall->pos.z), 1, 10, 5.0f, 3, 2);
							}
						}
						else
						{
							SetParticle(pBullet->pos, 0, 10, 5.0f, 0, 0);
						}
					}
					else if (pWall->pos.y + pWall->fHight >= pBullet->posOld.y + BULLET_SIZE_HEIGHT &&
						pWall->pos.y - pWall->fHight <= pBullet->posOld.y - BULLET_SIZE_HEIGHT &&
						pWall->pos.z + 2 <= pBullet->posOld.z - BULLET_SIZE_WIDTH &&
						pWall->pos.z + 2 >= pBullet->pos.z - BULLET_SIZE_WIDTH)
					{
						pBullet->bUse = false;

						// 影・削除
						DeleteShadow(pBullet->nIdxShadow);
						if (pWall->nType == 1)
						{
							if (pWall->nLife == 1)
							{
								pWall->nLife--;
								pWall[-1].nLife--;
								pWall[+1].nLife--;
								PlaySound(SOUND_LABEL_BREAK); //壁が壊れる音
								SetParticle(D3DXVECTOR3(pWall->pos.x, pWall->pos.y + (pWall->fHight / 3), pWall->pos.z), 1, 10, 5.0f, 3, 2);
							}
						}
						else
						{
							SetParticle(pBullet->pos, 0, 10, 5.0f, 0, 0);
						}
					}
				}
				else if (pWall->nCype == 2 || pWall->nCype == 3)
				{
					if (pWall->pos.y + pWall->fHight >= pBullet->posOld.y + BULLET_SIZE_HEIGHT &&
						pWall->pos.y - pWall->fHight <= pBullet->posOld.y - BULLET_SIZE_HEIGHT &&
						pWall->pos.x - 2 >= pBullet->posOld.x + BULLET_SIZE_WIDTH &&
						pWall->pos.x - 2 <= pBullet->pos.x + BULLET_SIZE_WIDTH)
					{
						pBullet->bUse = false;

						// 影・削除
						DeleteShadow(pBullet->nIdxShadow);
						if (pWall->nType == 1)
						{
							
							if (pWall->nLife == 1)
							{
								pWall->nLife--;
								pWall[-1].nLife--;
								pWall[+1].nLife--;
								PlaySound(SOUND_LABEL_BREAK); //壁が壊れる音
								SetParticle(D3DXVECTOR3(pWall->pos.x, pWall->pos.y + (pWall->fHight / 3), pWall->pos.z), 1, 10, 5.0f, 3, 2);
							}
						}
						else
						{
							SetParticle(pBullet->pos, 0, 10, 10.0f, 0, 0);
						}
					}
					else if (pWall->pos.y + pWall->fHight >= pBullet->posOld.y + BULLET_SIZE_HEIGHT &&
						pWall->pos.y - pWall->fHight <= pBullet->posOld.y - BULLET_SIZE_HEIGHT &&
						pWall->pos.x + 2 <= pBullet->posOld.x - BULLET_SIZE_WIDTH &&
						pWall->pos.x + 2 >= pBullet->pos.x - BULLET_SIZE_WIDTH)
					{
						pBullet->bUse = false;

						// 影・削除
						DeleteShadow(pBullet->nIdxShadow);
						if (pWall->nType == 1)
						{
							
							if (pWall->nLife == 1)
							{
								pWall->nLife--;
								pWall[-1].nLife--;
								pWall[+1].nLife--;
								PlaySound(SOUND_LABEL_BREAK); //壁が壊れる音
								SetParticle(D3DXVECTOR3(pWall->pos.x, pWall->pos.y + (pWall->fHight / 3), pWall->pos.z), 1, 10, 5.0f, 3, 2);
							}
						}
						else
						{
							SetParticle(pBullet->pos, 0, 10, 10.0f, 0, 0);
						}
					}
				}
			}
		}

		if (pWall->nType == 1 && pWall->bUse == true && pWall->nLife <= 0)
		{
			pWall->bUse = false;

			pBreakWall->Break++;
			pBreakWall->Current_F = pBreakWall->Break * 100 / pBreakWall->Total;

			Fog1 *pFog = GetFog();
			pFog->End -= pBreakWall->Current_F;
			SetDamage((int)pBreakWall->Current_F);
		}
		if (pBreakWall->Current_F = pBreakWall->Break * 100 / pBreakWall->Total >= 81)
		{
			SetGameState(GAMESTATE_END1, 5);
		}
	}
}

// ***** 置物 *****
void CollisionFigure(Bullet *pBullet)
{
	Figure *pFigure = GetFigure();
	for (int nCntFigure = 0; nCntFigure < NUM_FIGURE_MODEL; nCntFigure++, pFigure++)
	{
		if (pFigure->bUse == true)
		{
			if (pBullet->pos.x >= pFigure->pos.x - (pFigure->fWidth + GetBullet()->fWidht) &&
				pBullet->pos.x <= pFigure->pos.x + (pFigure->fWidth + GetBullet()->fWidht) &&
				pBullet->pos.z >= pFigure->pos.z - (pFigure->fHight + GetBullet()->fHight) &&
				pBullet->pos.z <= pFigure->pos.z + (pFigure->fHight + GetBullet()->fHight))
			{
				pBullet->posOld.z = pFigure->pos.z - (pFigure->fHight + GetBullet()->fHight);

				HitFigure(nCntFigure, 1);

				pBullet->bUse = false;
				// 影・削除
				DeleteShadow(pBullet->nIdxShadow);
			}
		}
	}
}

// ***** 敵 *****
void CollisionEnemy(Bullet *pBullet)
{
	Enemy *pEnemy = GetEnemy();
	for (int nCntFigure = 0; nCntFigure < NUM_MODEL; nCntFigure++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bDraw == true)
		{
			if (pBullet->pos.x >= pEnemy->pos.x - (pEnemy->fWidth + GetBullet()->fWidht) &&
				pBullet->pos.x <= pEnemy->pos.x + (pEnemy->fWidth + GetBullet()->fWidht) &&
				pBullet->pos.z >= pEnemy->pos.z - (pEnemy->fHight + GetBullet()->fHight) &&
				pBullet->pos.z <= pEnemy->pos.z + (pEnemy->fHight + GetBullet()->fHight))
			{
				pBullet->posOld.z = pEnemy->pos.z - (pEnemy->fHight + GetBullet()->fHight);

				HitEnemy(nCntFigure, 1);
				PlaySound(SOUND_LABEL_DAMAGE); //ダメージ音
				pBullet->bUse = false;
				// 影・削除
				DeleteShadow(pBullet->nIdxShadow);
			}
		}
	}
}
//****************************************
// 情報の取得
//****************************************
Bullet *GetBullet(void)
{
	return &g_aBullet[0];
}
//◇======================◇
//｜　　　　　　　　　　  ｜
//｜　パーティクル処理	  ｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"particle.h"
#include"effect.h"
#include"boss.h"
#include"Summon.h"
#include"aura_effect.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_PARTICLE	(2560)	//パーティクル総数
//◇====================◇
//｜ 　　情報構造体 　　｜
//◇====================◇

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置（発生位置）
	int  nLife;			//寿命（発生時間）
	int nType;			//種類
	int cType;			//色種類
	int pType;			//パーティクル種類
	int nCntForm;		//一度に生成するパーティクル数
	float fRadius;		//半径（大きさ）
	bool bUse;			//使用しているかどうか
} Particle;

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
Particle g_aParticle[MAX_PARTICLE];
int nForm;
int nForm1;
//◇====================================◇
//｜　　 パーティクルの初期化処理　　 　｜
//◇====================================◇
void InitParticle(void)
{
	int nCntParticle;

	nForm = 0;
	nForm1 = 0;
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].nCntForm = 0;
		g_aParticle[nCntParticle].bUse = false;
	}
}
//◇==================================◇
//｜　　 パーティクルの終了処理　　 　｜
//◇==================================◇
void UninitParticle(void)
{

}
//◇==================================◇
//｜　　 パーティクルの更新処理　　 　｜
//◇==================================◇
void UpdateParticle(void)
{
	int nCntParticle;	//カウンタ

	Boss *pBoss = GetBoss(); //敵の情報へのポインタ

	//エフェクト設定用
	D3DXVECTOR3 pos;	//位置
	float fRot;			//角度
	float fSpeed;		//移動速度
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 movevec;
	D3DXCOLOR col;		//色
	float fRadius = 0;		//半径
	int nLife = 0;			//寿命
	float rRot;			//中心点から角度を求める
	//float rRange;		//中心点から距離を求める

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse)
		{
			//パーティクルの発生
			int nCntAppear;

			switch (g_aParticle[nCntParticle].pType)
			{
			case 0:
				g_aParticle[nCntParticle].nCntForm = 20;
				break;
			case 1:
				g_aParticle[nCntParticle].nCntForm = 5;
				break;
			case 2:
				nForm = GetnCharge();
				g_aParticle[nCntParticle].nCntForm = 0 + (nForm / 3);
				break;
			case 3:
				g_aParticle[nCntParticle].nCntForm = 10;
				break;
			case 4:
				g_aParticle[nCntParticle].nCntForm = 5;
				break;
			case 5:
				nForm = GetnCharge1();
				g_aParticle[nCntParticle].nCntForm = 0 + (nForm / 3);
				break;
			case 6:
				g_aParticle[nCntParticle].nCntForm = 20;
				break;

			case 31:
				g_aParticle[nCntParticle].nCntForm = 10;
				break;
			case 32:
				g_aParticle[nCntParticle].nCntForm = 10;
				break;
			case 41:
				g_aParticle[nCntParticle].nCntForm = 10;
				break;
			case 42:
				g_aParticle[nCntParticle].nCntForm = 10;
				break;
			case 51:
				nForm1 = GetnCharge2();
				g_aParticle[nCntParticle].nCntForm = 0 + (nForm / 2);
				break;
			}

			for (nCntAppear = 0; nCntAppear < g_aParticle[nCntParticle].nCntForm; nCntAppear++)
			{
				switch (g_aParticle[nCntParticle].pType)
				{
				case 0:
					//位置の設定
					pos = g_aParticle[nCntParticle].pos;
					//移動量の設定
					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					fSpeed = (float)(rand() % (2 * 100)) / 100 + 2;
					move.x = sinf(fRot) * fSpeed;
					move.y = cosf(fRot) * fSpeed;
					break;
				case 1:
					//位置の設定
					pos = g_aParticle[nCntParticle].pos;
					//移動量の設定
					fRot = 0.0f;
					fSpeed = 0.0f;
					move.x = sinf(fRot) * fSpeed;
					move.y = cosf(fRot) * fSpeed;
					break;
				case 2:
					//位置の設定
					pos = g_aParticle[nCntParticle].pos;

					//移動量の設定
					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					fSpeed = (float)(rand() % (2 * 50)) / 50 + 2;
					move.x = sinf(fRot) * fSpeed;
					move.y = cosf(fRot) * fSpeed;
					break;
				case 3:
					rRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;

					pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + sinf(rRot) * 30,
									  g_aParticle[nCntParticle].pos.y + cosf(rRot) * 30, 0.0f);

					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					move.x = sinf(fRot) * 0;
					move.y = cosf(fRot) * 0;
					break;
				case 4:
					pos = g_aParticle[nCntParticle].pos;
					fRot = 0.0f;
					fSpeed = 0.0f;
					move.x = sinf(fRot) * fSpeed;
					move.y = cosf(fRot) * fSpeed;
					break;
				case 5:
					//位置の設定
					pos = g_aParticle[nCntParticle].pos;

					//移動量の設定
					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					fSpeed = (float)(rand() % (2 * 50)) / 50 + 2;
					move.x = sinf(fRot) * fSpeed;
					move.y = cosf(fRot) * fSpeed;
					break;
				case 6:

					pos = D3DXVECTOR3(pBoss->pos.x + 0.0f, pBoss->pos.y + 15.0f, 0.0f);

					rRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;

					pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + sinf(rRot) * 90,
						g_aParticle[nCntParticle].pos.y + cosf(rRot) * 90, 0.0f);

					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					move.x = sinf(3) * 3;
					move.y = cosf(3) * 3;
					break;

				case 31:
					rRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;

					pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + sinf(rRot) * 40,
						g_aParticle[nCntParticle].pos.y + cosf(rRot) * 40, 0.0f);

					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					move.x = sinf(fRot) * 0;
					move.y = cosf(fRot) * 0;
					break;
				case 32:
					rRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;

					pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + sinf(rRot) * 50,
						g_aParticle[nCntParticle].pos.y + cosf(rRot) * 50, 0.0f);

					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					move.x = sinf(fRot) * 0;
					move.y = cosf(fRot) * 0;
					break;
				case 41:
					rRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;

					pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + sinf(rRot) * 20,
						g_aParticle[nCntParticle].pos.y + cosf(rRot) * 20, 0.0f);

					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					move.x = sinf(fRot) * 0;
					move.y = cosf(fRot) * 0;
					break;
				case 42:
					rRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;

					pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + sinf(rRot) * 30,
						g_aParticle[nCntParticle].pos.y + cosf(rRot) * 30, 0.0f);

					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					move.x = sinf(fRot) * 0;
					move.y = cosf(fRot) * 0;
					break;
				case 51:
					pos = g_aParticle[nCntParticle].pos;

					//移動量の設定
					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					fSpeed = (float)(rand() % (2 * 50)) / 50 + 2;
					move.x = sinf(fRot) * fSpeed;
					move.y = cosf(fRot) * fSpeed;
					break;
				}

				switch (g_aParticle[nCntParticle].cType)
				{
				case 0:
					//色
					col = D3DXCOLOR(1.0f, 0.2f, 0.3f, 1.0f);
					break;
				case 1:
					col = D3DXCOLOR(0.2f, 0.5f, 1.0f, 1.0f);
					break;
				case 2:
					col = D3DXCOLOR(0.5f, 0.5f, 0.2f, 1.0f);
					break;
				case 3:
					col = D3DXCOLOR(0.5f, 0.2f, 0.3f, 1.0f);
					break;
				case 4:
					col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
					break;
				case 5:
					col = D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f);
					break;
				case 6:
					col = D3DXCOLOR(0.1f, 1.0f, 0.3f, 1.0f);
					break;
				case 7:
					col = D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f);
					break;
				}

				switch (g_aParticle[nCntParticle].nType)
				{
				case 0:
					//半径
					fRadius = 5;
					break;
				case 1:
					fRadius = 30;
					break;
				case 2:
					fRadius = 90;
					break;
				case 3:
					fRadius = 5;
					break;
				case 4:
					fRadius = 12;
					break;
				case 6:
					fRadius = 10;
					break;
				}

				switch (g_aParticle[nCntParticle].nType)
				{
				case 0:
					//寿命
					nLife = 30;
					break;
				case 1:
					//寿命
					nLife = 30;
					break;
				case 2:
					//寿命
					nLife = 30;
					break;
				case 3:
					//寿命
					nLife = 10;
					break;
				case 4:
					nLife = 10;
					break;
				case 6:
					nLife = 15;
					break;
				}
				
				if (g_aParticle[nCntParticle].pType == 6 && g_aParticle[nCntParticle].nType == 6)
				{
					//エフェクト設定
					SetAuraEffect(pos, move, col, fRadius, nLife, 0);
				}
				else
				{
					//エフェクト設定
					SetEffect(pos, move, col, fRadius, nLife, 0);
				}
			}
				

			//寿命の減少
			g_aParticle[nCntParticle].nLife--;
			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}
//◇==================================◇
//｜　　 パーティクルの描画処理　　 　｜
//◇==================================◇
void DrawParticle(void)
{
	
}

//◇==================================◇
//｜　　 パーティクルの設定処理　　 　｜
//◇==================================◇
void SetParticle(D3DXVECTOR3 pos, int nType, int nLife, float fRadius,int cType,int pType)
{
	int nCntParticle;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].nType = nType;
			g_aParticle[nCntParticle].cType = cType;
			g_aParticle[nCntParticle].pType = pType;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_aParticle[nCntParticle].bUse = true;
			break;
		}
	}
}
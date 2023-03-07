//◇======================◇
//｜　　　　　　　　　　  ｜
//｜　パーティクル処理	  ｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"particle.h"
#include"effect.h"
#include "Goal_effect.h"
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

	//エフェクト設定用
	D3DXVECTOR3 pos;	//位置
	float fRot;			//角度
	float fSpeed;		//移動速度
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 movevec;
	D3DXCOLOR col;		//色
	float fRadius = 0;	//半径
	int nLife = 0;		//寿命

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
				g_aParticle[nCntParticle].nCntForm = 1;
				break;
			case 2:
				g_aParticle[nCntParticle].nCntForm = 5;
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
					move.z = sinf(fRot) * fSpeed;
					break;
				case 1:
					//位置の設定
					pos = g_aParticle[nCntParticle].pos;
					//移動量の設定
					fRot = 0.0f;
					fSpeed = 0.0f;
					move.x = sinf(fRot) * fSpeed;
					move.y = cosf(fRot) * fSpeed;
					move.z = sinf(fRot) * fSpeed;
					break;
				case 2:
					//位置の設定
					pos = g_aParticle[nCntParticle].pos;
					//移動量の設定
					fRot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
					fSpeed = (float)(rand() % (2 * 100)) / 100 + 2;
					move.x = sinf(fRot) * fSpeed;
					move.y = cosf(fRot) * fSpeed;
					move.z = sinf(fRot) * fSpeed;
					break;
				}

				switch (g_aParticle[nCntParticle].cType)
				{
				case 0:
					//色
					col = D3DXCOLOR(0.0f, 1.0f, 0.1f, 1.0f);
					break;
				case 1:
					col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
					break;
				case 2:
					col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				case 3:
					col = D3DXCOLOR(0.1f, 0.3f, 0.1f, 1.0f);
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
					fRadius = 20;
					break;
				case 3:
					fRadius = 80;
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
					nLife = 20;
					break;
				case 3:
					nLife = 50;
					break;
				}

				//エフェクト設定
				if (g_aParticle[nCntParticle].pType == 1)
				{
					SetGoalEffect(pos, move, col, fRadius, nLife, g_aParticle[nCntParticle].pType);
				}
				else
				{
					SetEffect(pos, move, col, fRadius, nLife, g_aParticle[nCntParticle].pType);
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
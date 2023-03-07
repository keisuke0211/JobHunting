//◇====================◇
//｜　　  　　　　　　　｜
//｜　ゲーム画面の処理　｜
//｜　Author:大殿慶輔   ｜
//｜                    ｜
//◇====================◇
#include "game.h"
#include "fade.h"

//UI
#include "frame.h"
#include "score.h"
#include "text.h"
#include "time.h"
#include "damage.h"
// ライト
#include "PointLight.h"
// モデル
#include "stone_pillar.h"
#include "Player_model.h"
#include "torch_model.h"
#include "enemy_model.h"
#include "figure_model.h"
#include "jewel.h"
// 演出
#include "camera.h"
#include "effect.h"
#include "Goal_effect.h"
#include "particle.h"
#include "shadow.h"
#include "sound.h"
#include "torch.h"

// 地形
#include "floor.h"
#include "meshfield_floor.h"
#include "meshfield_wall.h"
#include "wall.h"

// その他
#include "bullet.h"
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffGame = NULL;			// 頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureGame = NULL;			// テクスチャへのポインタ

GAMESTATE g_gameState = GAMESTATE_NONE;					// ゲームの状態
Fog1 g_aFog;											// 霧の状態
int g_nCounterGameState = 0;							// 状態管理カウンター
bool g_bPause;											// ポーズ状態のON/OFF
bool g_dMap;											// ミニマップのＯＮ・ＯＦＦ
int g_nTimeCnt;
int nCnt99;
int nScore;

//==============================================================================================================================================
//――――――――――――――――――――――――――――――｜【ゲーム・処理】｜―――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇====================================◇
//｜　　　 ゲーム画面の初期化処理　　 　｜
//◇====================================◇
void InitGame(void)
{
	Camera *pCamera = Getcamara(0);

	g_aFog.Start = 1.0f;
	g_aFog.End = 2500.0f;
	g_aFog.bUse = true;

	g_gameState = GAMESTATE_NONE;
	g_nCounterGameState = 0;
	g_bPause = false;//ポーズ解除
	g_dMap = true;
	nCnt99 = 0;

	//エネミー　読み込み処理
	LoadEnemyModel();
	//松明　読み込み処理
	LoadTorch_Model();
	//モデル・置物　読み込み処理
	LoadFigureModel();
	//石柱
	LoadPillar_Model();
	//宝石
	LoadJewel();

	//カメラ
	InitCamera();
	//床
	InitFloor();
	//壁
	InitWall();

	//モデル
	InitPlayerModel();
	//エネミー
	InitEnemyeModel();
	//モデル・置物
	InitFigureModel();
	//松明・モデル
	InitTorch_Model();
	//石柱
	InitPillar_Model();
	//宝石
	InitJewel();
	//弾
	InitBullet();
	//松明
	InitTorch();
	//エフェクト
	InitEffect();
	//ゴールエフェクト
	InitGoalEffect();
	//パーティクル
	InitParticle();
	//影
	InitShadow();
	//枠
	InitFrame();
	//SCORE
	InitScore();
	//テキスト
	InitText();
	//タイム
	InitTime();
	//ダメージ
	InitDamage();

	pCamera[1].bUse = true;

	PlaySound(SOUND_LABEL_BGM002); //洞窟のBJM
}
//◇==================================◇
//｜　　　 ゲーム画面の終了処理　　 　｜
//◇==================================◇
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//ポイントライト
	UninitPointLight();
	//モデル
	UninitPlayerModel();
	//エネミー
	UninitEnemyeModel();
	//モデル・置物
	UninitFigureModel();
	//松明・モデル
	UninitTorch_Model();
	//石柱
	UninitPillar_Model();
	//宝石
	UninitJewel();
	//弾
	UninitBullet();
	//松明
	UninitTorch();
	//エフェクト
	UninitEffect();
	//ゴールエフェクト
	UninitGoalEffect();
	//パーティクル
	UninitParticle();
	//影
	UninitShadow();
	//枠
	UninitFrame();
	//SCORE
	UninitScore();
	//テキスト
	UninitText();
	//タイム
	UninitTime();
	//ダメージ
	UninitDamage();
	//床
	UninitFloor();
	//壁
	UninitWall();
	//メッシュフィールド・床
	UninitMeshField_Floor();
	//メッシュフィールド・壁
	UninitMeshField_Wall();

}
//◇==================================◇
//｜　　　 ゲーム画面の更新処理　　 　｜
//◇==================================◇
void UpdateGame(void)
{
	Camera *pCamera = Getcamara(0);
	Player *pPlayer = GetPlayer();

	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

#ifdef _DEBUG
	//　ミニマップのＯＮ・ＯＦＦ
	if (GetKeyboardTrigger(DIK_I) == true)
	{
		if (pCamera[2].bUse == false)
		{
			if (pCamera[1].bUse == true)
			{
				pCamera[1].bUse = false;
				g_dMap = false;
			}
			else if (pCamera[1].bUse == false)
			{
				pCamera[1].bUse = true;
				g_dMap = true;
			}
		}
	}
#endif // _DEBUG

	//　全体マップのＯＮ・ＯＦＦ
	if (GetKeyboardTrigger(DIK_M) == true)
	{
		if (pCamera[2].bUse == true)
		{
			if (g_dMap == true)
			{
				pCamera[1].bUse = true;
			}
			pCamera[2].bUse = false;
			g_aFog.bUse = true;
			pPlayer->bAction = true;

		}
		else if (pCamera[2].bUse == false)
		{
			if (g_dMap == true)
			{
				pCamera[1].bUse = false;
			}
			pCamera[2].bUse = true;
			g_aFog.bUse = false;
			pPlayer->bAction = false;
		}
	}

#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P) == true && GetFade() == FADE_NONE)
	{//ポーズキー(Pキー)が押された
		g_bPause = g_bPause ? false : true;
	}
#endif // _DEBUG

	if (g_bPause == false)
	{
		if (pCamera[2].bUse == false)
		{
			if (g_gameState == GAMESTATE_NONE)
			{
				if (GetTime() == 0 && nCnt99 == 0)
				{
					SetGameState(GAMESTATE_END1, 1);
					nCnt99++;
				}
				else if (GetTime() > 0)
				{
					g_nTimeCnt++;
					if (g_nTimeCnt >= 60)
					{
						g_nTimeCnt = 0;
						AddTime(-1);
					}
				}
			}

			//モデル
			UpdatPlayerModel();
			//エネミー
			UpdateEnemyeModel();
			//モデル・置物
			UpdateFigureModel();
			//松明・モデル
			UpdateTorch_Model();
			//石柱
			UpdatePillar_Model();
			//宝石
			UpdateJewel();
			//弾
			UpdateBullet();
			//松明
			UpdatTorch();
			//エフェクト
			UpdateEffect();
			//ゴールエフェクト
			UpdateGoalEffect();
			//パーティクル
			UpdateParticle();
			//影
			UpdateShadow();
			
			//SCORE
			UpdateScore();
			//タイム
			UpdateTime();
			//テキスト
			UpdateText();
			//ダメージ
			UpdateDamage();
			//床
			UpdateFloor();
			//壁
			UpdateWall();

			switch (g_gameState)
			{
			case GAMESTATE_NORMAL://通常状態
				break;
			case GAMESTATE_END1://終了状態・敵全滅
				g_nCounterGameState--;
				if (g_nCounterGameState == 0)
				{
					//モードの設定(リザルト画面に移行)
					SetFade(MODE_RESULY1);

					g_gameState = GAMESTATE_NONE;//何もしていない状態に設定
					pCamera[1].bUse = false;
					pCamera[2].bUse = false;
					g_aFog.End = 2500.0f;
				}
				break;
			case GAMESTATE_END2://ゲームクリア
				g_nCounterGameState--;
				if (g_nCounterGameState == 0)
				{
					nScore = 90 - (90 - GetTime());

					nScore = nScore * 50;

					AddScore(nScore);

					//モードの設定(リザルト画面に移行)
					SetFade(MODE_RESULY2);

					g_gameState = GAMESTATE_NONE;//何もしていない状態に設定

					pCamera[1].bUse = false;
					pCamera[2].bUse = false;
					g_aFog.End = 2500.0f;
				}
				break;
			}
		}
		//枠
		UpdateFrame();
	}
}
//◇==================================◇
//｜　　　 ゲーム画面の描画処理　　 　｜
//◇==================================◇
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	Camera *pCamera = Getcamara(0);
	Fog1 *pFog = GetFog();

	pDevice->SetRenderState(D3DRS_FOGENABLE, pFog->bUse);				// 霧の有効化
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0, 0, 0, 0));	// 霧の色
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);			// 頂点モード
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&pFog->Start));	// 開始位置
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&pFog->End));		// 終了位置

	//床
	DrawFloor();
	//壁
	DrawWall();
	//弾
	DrawBullet();
	//影
	DrawShadow();
	//松明
	DrawTorch();
	//エネミー
	DrawEnemyeModel();
	//モデル・置物
	DrawFigureModel();
	//松明・モデル
	DrawTorch_Model();
	//石柱
	DrawPillar_Model();
	//宝石
	DrawJewel();
	if (pCamera[2].bUse == false)
	{
		//エフェクト
		DrawEffect();
	}
	//ゴールエフェクト
	DrawGoalEffect();
	//パーティクル
	DrawParticle();
	//モデル
	DrawPlayerModel();

	if (pCamera[2].bUse == false)
	{
		//SCORE
		DrawScore();
		//テキスト
		DrawaText();
		//タイム
		DrawTime();
		//ダメージ
		DrawDamage();
	}
	//枠
	DrawaFrame();
}
//◇================================◇
//｜　　　 ゲームの状態の設定　　 　｜
//◇================================◇
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}
//◇================================◇
//｜　　　 ゲームの状態の所得　　 　｜
//◇================================◇
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
int GetTimeCnt(void)
{
	return g_nTimeCnt;
}

Fog1 *GetFog(void)
{
	return &g_aFog;
}
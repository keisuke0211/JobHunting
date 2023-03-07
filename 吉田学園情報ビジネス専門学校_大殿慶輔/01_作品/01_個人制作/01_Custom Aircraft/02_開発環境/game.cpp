//◇====================◇
//｜　　  　　　　　　　｜
//｜　ゲーム画面の処理　｜
//｜　Author:大殿慶輔   ｜
//｜                    ｜
//◇====================◇
#include "game.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "text.h"
#include "enemy_bullet.h"
#include "boss.h"
#include "fade.h"
#include "effect.h"
#include "pause.h"
#include "particle.h"
#include "frame.h"
#include "life.h"
#include "ranking_bg.h"
#include "boss_hp.h"
#include "Summon.h"
#include "aura_effect.h"
#include "TopSource.h"
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffGame = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureGame = NULL;			//テクスチャへのポインタ

GAMESTATE g_gameState = GAMESTATE_NONE;					//ゲームの状態
int g_nCounterGameState = 0;							//状態管理カウンター
bool g_bPause = false;									//ポーズ状態のON/OFF
bool bswitch = true;									//曲切り替え
//==============================================================================================================================================
//――――――――――――――――――――――――――――――｜【ゲーム・処理】｜―――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇====================================◇
//｜　　　 ゲーム画面の初期化処理　　 　｜
//◇====================================◇
void InitGame(void)
{

	g_gameState = GAMESTATE_NONE;
	g_nCounterGameState = 0;
	g_bPause = false;//ポーズ解除

     //背景の初期化処理
	InitBg();

	//スコアの初期化処理
	InitScore();

	//トップスコアの初期化処理
	InitTopScore();

	//ライフの初期化処理
	InitLife();

	//テキストの初期化処理
	InitText();

	//弾の初期化処理
	InitBullet();

	//エフェクトの初期化処理
	InitEffect();

	//オーラエフェクト初期化処理
	InitAuraEffect();

	//パーティクルの初期化処理
	InitParticle();

	//敵の弾初期化処理
	InitEnemyBullet();

	//爆発の初期化処理
	InitExplosion();

	//プレイヤーの初期化処理
	InitPlayer();

	//敵の初期化処理
	InitEnemy();

	//召喚の初期化処理
	InitSummon();

	//ボスの初期化処理
	InitBoss();

	//HPゲージの初期化処理
	InitBosshp();

	//枠の初期化処理
	InitFrame();

	//ポーズの初期化処理
	InitPause();

	PlaySound(SOUND_LABEL_BGM001); //←再生したいサウンドを指定

	//SetEnemy(D3DXVECTOR3(150, 0, 0), 2);//【位置,種類】任意の数だけ設定する
	//SetEnemy(D3DXVECTOR3(150, -70, 0), 1);//【位置,種類】任意の数だけ設定する
	//SetEnemy(D3DXVECTOR3(150, -140, 0), 2);//【位置,種類】任意の数だけ設定する
	//SetEnemy(D3DXVECTOR3(150, -210, 0), 1);//【位置,種類】任意の数だけ設定する
	//SetEnemy(D3DXVECTOR3(150, -280, 0), 2);//【位置,種類】任意の数だけ設定する

	
}
//◇==================================◇
//｜　　　 ゲーム画面の終了処理　　 　｜
//◇==================================◇
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//ポリゴンの終了処理
	UninitBg();

	//パーティクルの終了処理
	UninitParticle();

	//エフェクトの終了処理
	UninitEffect();

	//オーラエフェクトの終了処理
	UninitAuraEffect();

	//弾の終了処理
	UninitBullet();

	//敵の弾終了処理
	UninitEnemyBullet();

	//爆発の終了処理
	UninitExplosion();

	//テキストの終了処理
	UninitText();

	//スコアの終了処理
	UninitScore();

	//トップスコアの終了処理
	UninitTopScore();

	//ライフの終了処理
	UninitLife();

	//プレイヤーの終了処理
	UninitPlayer();

	//敵の終了処理
	UninitEnemy();

	//召喚の終了処理
	UninitSummon();

	//ボスの終了処理
	UninitBoss();

	//HPゲージの初期化処理
	UninitBosshp();

	//枠の終了処理
	UninitFrame();

	//ポーズの終了処理
	UninitPause();

}
//◇==================================◇
//｜　　　 ゲーム画面の更新処理　　 　｜
//◇==================================◇
void UpdateGame(void)
{
	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ
	Player *pPlayer = Getplayer();	//プレイヤーの情報へのポインタ

	if (GetKeyboardTrigger(DIK_P) == true && pPlayer->bUse == true && GetFade() == FADE_NONE)
	{//ポーズキー(Pキー)が押された
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true)
	{//ポーズ中
		//ポーズの更新処理
		UpdatePause();
	}
	else
	{//ポーズ中でなければ
	 //背景の更新処理
		UpdateBg();

		//テキストの更新処理
		UpdateText();

		//スコアの更新処理
		UpdateScore();

		//トップスコアの更新処理
		UpdateTopScore();

		//ライフの更新処理
		UpdateLife();

		//弾の更新処理
		UpdateBullet();

		//爆発の更新処理
		UpdateExplosion();

		//敵の弾更新処理
		UpdateEnemyBullet();

		//プレイヤーの更新処理
		UpdatePlayer();

		//敵の更新処理
		UpdateEnemy();

		//ボスの更新処理
		UpdateBoss();

		//召喚の更新処理
		UpdateSummon();

		//パーティクルの更新処理
		UpdateParticle();

		//エフェクトの更新処理
		UpdateEffect();

		//オーラエフェクトの更新処理
		UpdateAuraEffect();

		//HPゲージの更新処理
		UpdateBosshp();

		//枠の更新処理
		UpdateFrame();

		switch (g_gameState)
		{
		case GAMESTATE_NORMAL://通常状態
			break;
		case GAMESTATE_END1://終了状態・敵全滅
			g_nCounterGameState--;
			if (g_nCounterGameState == 0)
			{
				g_gameState = GAMESTATE_NONE;//何もしていない状態に設定

				//モードの設定(リザルト画面に移行)
				SetFade(MODE_GAME_CLEAR);

			}
			break;
		case GAMESTATE_END2://終了状態・プレイヤー滅
			g_nCounterGameState--;
			if (g_nCounterGameState == 0)
			{
				g_gameState = GAMESTATE_NONE;//何もしていない状態に設定

				 //モードの設定(リザルト画面に移行)
				SetFade(MODE_GAME_OVER);

			}
			break;
		}
	}
}
//◇==================================◇
//｜　　　 ゲーム画面の描画処理　　 　｜
//◇==================================◇
void DrawGame(void)
{
	//背景の描画処理
	DrawBg();
	
	//敵の描画処理
	DrawEnemy();

	//オーラエフェクトの描画処理
	DrawAuraEffect();

	//ボスの描画処理
	DrawBoss();

	//召喚の描画処理
	DrawSummon();
	
	//パーティクルの描画処理
	DrawParticle();

	//エフェクトの描画処理
	DrawEffect();

	//プレイヤーの描画処理
	DrawPlayer();

	//弾の描画処理
	DrawBullet();

	//敵の弾描画処理
	DrawEnemyBullet();

	//爆発の描画処理
  	DrawExplosion();

	//HPゲージの描画処理
	//DrawBosshp();

	//枠の描画処理
	DrawFrame();

	//テキストの初期化処理
	DrawaText();

	//スコアの描画処理
	DrawScore();

	//トップスコアの描画処理
	DrawTopScore();

	//ライフの描画処理
	DrawLife();

	if (g_bPause == true)
	{//ポーズ中
		//ポーズの描画処理
		UpdatePause();
	}

}

//ポーズの有効無効設定
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
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
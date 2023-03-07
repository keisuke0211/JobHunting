//◇====================◇
//｜　　  　　　　　　　｜
//｜　ゲーム画面の処理　｜
//｜　Author:大殿慶輔   ｜
//｜                    ｜
//◇====================◇
#include "game.h"
#include "fade.h"
#include "player.h"
#include "bg.h"
#include "block.h"
#include "camera.h"
#include "lure.h"
#include "rope.h"
#include "sound.h"
#include "time.h"
#include "text.h"
#include "result ver2.h"
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffGame = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureGame = NULL;			//テクスチャへのポインタ

GAMESTATE g_gameState = GAMESTATE_NONE;					//ゲームの状態
int g_nCounterGameState = 0;							//状態管理カウンター
bool g_bPause = false;									//ポーズ状態のON/OFF
bool bswitch = true;									//曲切り替え
int g_nTimeCnt;
int nCnt99;
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
	nCnt99 = 0;
	//背景の初期化処理
	InitBg();

	//プレイヤーの初期化処理
	InitPlayer();

	//ルアーの初期化処理
	InitLure();

	//ロープの初期化処理
	InitRope();
	//ブロックの初期化処理
	InitBlock();

	//タイマーの初期化処理
	InitTime();
	//テキストの初期化処理
	InitText();
	//カメラの初期化処理
	InitCamera();

	PlaySound(SOUND_LABEL_BGM001); //洞窟のBJM

	
}
//◇==================================◇
//｜　　　 ゲーム画面の終了処理　　 　｜
//◇==================================◇
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//背景の終了処理
	UninitBg();

	//プレイヤーの終了処理
	UninitPlayer();

	//ルアーの終了処理
	UninitLure();
	//ロープの終了処理
	UninitRope();
	//ブロックの終了処理
	UninitBlock();
	//タイマーの終了処理
	UninitTime();
	//テキストの終了処理
	UninitText();
	//カメラの終了処理
	UninitCamera();

}
//◇==================================◇
//｜　　　 ゲーム画面の更新処理　　 　｜
//◇==================================◇
void UpdateGame(void)
{
	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

	if (GetTime() == 0 && nCnt99 == 0)
	{
		SetGameState(GAMESTATE_END2, 1);
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

	if (GetKeyboardTrigger(DIK_P) == true && GetFade() == FADE_NONE)
	{//ポーズキー(Pキー)が押された
		g_bPause = g_bPause ? false : true;
	}

	//背景の更新処理
	UpdateBg();
	//プレイヤーの更新処理
	UpdatePlayer();
	//ルアーの更新処理
	UpdateLure();
	//ロープの更新処理
	UpdateRope();
	//ブロックの更新処理
	UpdateBlock();
	//タイマーの更新処理
	UpdateTime();
	//テキストの更新処理
	UpdateText();
	//カメラの更新処理
	UpdateCamera();

	GetCamera()->bCamera_Y = false;
		switch (g_gameState)
		{
		case GAMESTATE_NORMAL://通常状態
			break;
		case GAMESTATE_END1://終了状態・敵全滅
			g_nCounterGameState--;
			if (g_nCounterGameState == 0)
			{
				//モードの設定(リザルト画面に移行)
				SetFade(MODE_RESULY2);

				g_gameState = GAMESTATE_NONE;//何もしていない状態に設定
			}
			break;
		case GAMESTATE_END2://ゲームオーバー
			g_nCounterGameState--;
			if (g_nCounterGameState == 0)
			{
				//モードの設定(リザルト画面に移行)
				SetFade(MODE_RESULY1);

				g_gameState = GAMESTATE_NONE;//何もしていない状態に設定
			}
			break;
		}
}
//◇==================================◇
//｜　　　 ゲーム画面の描画処理　　 　｜
//◇==================================◇
void DrawGame(void)
{
	//背景の描画処理
	DrawBg();
	//ブロックの描画処理
	DrawBlock();
	//ロープの描画処理
	DrawRope();
	//ルアーの描画処理
	DrawLure();
	//プレイヤーの描画処理
	DrawPlayer();
	//テキストの描画処理
	DrawaText();
	//タイマーの描画処理
	DrawTime();
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
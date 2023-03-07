//========================================
// 
// MD:ゲーム画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_game_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "text.h"
#include "bg_game_00.h"			// BG :ゲーム		[00]
#include "chr_enemy_00.h"		// CHR:敵			[00]
#include "md_game_00.h"			// MD :ゲーム画面	[00]
#include "md_result_00.h"		// MD :リザルト画面	[00]
#include "obj_castle_00.h"		// OBJ:城			[00]
#include "obj_castle_01.h"		// OBJ:城			[01]
#include "ui_move-text_00.h"	// UI :移動テキスト	[00]
#include "ui_life_000.h"		// UI :ライフ000
#include "eff_light_00.h"		// EFF : ライト00
#include "eff_particle_00.h"	// EFF : パーティクル00
#include "player.h"				// プレイヤー
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
#define MD_GAME_00_LIMIT_TIME	(60*120)	// 制限時間

//****************************************
// プロトタイプ宣言
//****************************************
// MD:ゲーム画面[00] のパラメーターの初期化処理
// MD:ゲーム画面[00] の状態に応じた更新処理
// MD:ゲーム画面[00] の状態に応じた開始処理
// MD:ゲーム画面[00] の状態に応じた終了処理
void InitParameterMd_game_00(void);
void UpdateMd_game_00State(void);
void StartMd_game_00State(void);
void EndMd_game_00State(void);

//****************************************
// グローバル宣言
//****************************************
Md_game_00	g_md_game_00;	// MD:ゲーム画面[00] の情報
bool		g_bPause		// ポーズフラグ
			= false;
int			g_nCounterFrame;		// フレームカウンター
int			g_nCounterLimitTime;	// 制限時間カウンター
bool		g_bGameEnd;				// ゲーム終了フラグ
int			g_nTemp;				// 記憶用

//========== *** MD:ゲーム画面[00] の情報を取得 ***
Md_game_00 *GetMd_game_00(void) 
{
	return &g_md_game_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterMd_game_00関数 - MD:ゲーム画面[00] のパラメーターの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMd_game_00(void)
{
	Md_game_00	*pMd =	// MD:ゲーム画面[00] の情報
				&g_md_game_00;

	pMd->state	= MD_GAME_00_STATE_NONE;	// 状態
}

//========================================
// StartMd_game_00State関数 - MD:ゲーム画面[00] の状態に応じた開始処理 -
// Author:RIKU NISHIMURA
//========================================
void StartMd_game_00State(void)
{
	Md_game_00	*pMd	// MD:ゲーム画面[00] の情報
				= &g_md_game_00;

	// 状態カウンターを初期化
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	}
}

//========================================
// EndMd_game_00State関数 - MD:ゲーム画面[00] の状態に応じた終了処理 -
// Author:RIKU NISHIMURA
//========================================
void EndMd_game_00State(void) 
{
	Md_game_00	*pMd	// MD:ゲーム画面[00] の情報
				= &g_md_game_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	}
}

//========================================
// UpdateMd_game_00State関数 - MD:ゲーム画面[00] の状態に応じた更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_game_00State(void)
{
	Md_game_00	*pMd	// MD:ゲーム画面[00] の情報
				= &g_md_game_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_GAME_00_STATE_NORMAL: {

		break;
	}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitMd_game_00関数 - MD:ゲーム画面[00]の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitMd_game_00(void)
{
	// パラメーターの初期化処理
	InitParameterMd_game_00();

	// 初期化処理
	InitParameterCamera3D();	// カメラ(3D)のパラメーター
	InitBg_game_00();			// BG :ゲーム[00]
	InitChr_enemy_00();			// CHR:敵[00]
	InitObj_castle_00();		// OBJ:城[00]
	InitObj_castle_01();		// OBJ:城[01]
	InitUi_moveText_00();		// UI :移動テキスト[00]
	InitUi_life_000();			// UI :ライフ[00]
	InitEff_light_00();			// EFF : ライト00
	InitEff_particle_00();		// EFF : パーティクル00
	InitPlayer();				// プレイヤー

	// 状態を通常に設定
	SetMd_game_00State(MD_GAME_00_STATE_NORMAL);

	PlaySound(2);

	// フレームカウンターを初期化
	g_nCounterFrame = 0;

	// 制限時間カウンターを初期化
	g_nCounterLimitTime = MD_GAME_00_LIMIT_TIME;

	// ゲーム終了フラグを偽にする
	g_bGameEnd = false;
}

//========================================
// UninitMd_game_00関数 - MD:ゲーム画面[00]の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_game_00(void)
{
	UninitPlayer();			//プレイヤー
	UninitEff_light_00();	// EFF : ライト00
	UninitBg_game_00();		// BG :ゲーム[00]
	UninitChr_enemy_00();	// CHR:敵[00]
	UninitObj_castle_00();	// OBJ:城[00]
	UninitObj_castle_01();	// OBJ:城[01]
	UninitUi_moveText_00();	// UI :移動テキスト[00]
	UninitUi_life_000();	// UI :ライフ[00]
}

//========================================
// UpdateMd_game_00関数 - MD:ゲーム画面[00]の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_game_00(void)
{
	// MD:ゲーム画面[00] の状態に応じた更新処理
	UpdateMd_game_00State();

#if _DEBUG
	{
		g_nCounterFrame += g_bPause ^ 1;
		char aString[TXT_MAX];
		sprintf(aString, "FRAME:%d", g_nCounterFrame);
		SetText2D(
			aString,
			2,
			DISPLAY_LEFT,
			D3DXVECTOR3(16.0f, 16.0f, 0.0f), { 255,255,255,255 }, 1.0f, 1.0f);
		if (GetKeyboardTrigger(DIK_P)) 
		{
			g_bPause ^= 1;

			if (g_bPause) 
			{
				PauseSound(2);
			}
			else 
			{
				RestartSound(2);
			}
		}

		if (GetKeyboardTrigger(DIK_T)) 
		{
			g_nTemp = g_nCounterFrame;
		}

		sprintf(aString, "TEMP:%d", g_nTemp);
		SetText2D(
			aString,
			2,
			DISPLAY_LEFT,
			D3DXVECTOR3(16.0f, 40.0f, 0.0f), { 255,255,255,255 }, 1.0f, 1.0f);
	}
#endif

	if (!g_bPause)
	{
		bool bLimitEnd = g_nCounterLimitTime <= 0;	// 制限時間が尽きたフラグ
		bool bLifeDown = GetPlayer()->nLife <= 0;	// ライフが尽きたフラグ

		if (((bLimitEnd) || (bLifeDown)) && (!g_bGameEnd))
		{// 制限時間カウンターが結果0以下 or プレイヤーの体力が0以下の時、
			g_bGameEnd = true;				// ゲーム終了フラグを真にする
			SetFade(MODE_RESULT_00);		// モードをMD:リザルト画面[00] に設定
			if (bLimitEnd) 
			{// 制限時間が尽きた
				SetBgMd_result_00(0);
			}
			else 
			{
				SetBgMd_result_00(1);
			}
		}
		else
		{
			g_nCounterLimitTime--;
		}
	}

	{
		char aString[TXT_MAX];
		sprintf(aString, "TIME:%d", g_nCounterLimitTime / 60);
		SetText2D(
			aString,
			0,
			DISPLAY_CENTER,
			D3DXVECTOR3(SCREEN_WIDTH*0.5f, 40.0f, 0.0f), { 255,255,255,255 }, 1.0f, 1.0f);
	}

	if (!g_bPause)
	{
		if (!g_bGameEnd)
		{
			UpdateBg_game_00();		// BG :ゲーム[00]
			UpdateChr_enemy_00();	// CHR:敵[00]
		}
		UpdateEff_particle_00();
		UpdateEff_light_00();
		UpdateObj_castle_00();	// OBJ:城[00]
		UpdateObj_castle_01();	// OBJ:城[01]
		UpdatePlayer();			// プレイヤー
		UpdateUi_moveText_00();	// UI :移動テキスト[00]
		UpdateUi_life_000();	// UI :ライフ[00]
	}
}

//========================================
// DrawMd_game_00関数 - MD:ゲーム画面[00]の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_game_00(void)
{
	// 描画処理
	DrawBg_game_00();		// BG :ゲーム[00]
	DrawObj_castle_00();	// OBJ:城[00]
	DrawObj_castle_01();	// OBJ:城[01]
	DrawPlayer();			// プレイヤー
	DrawChr_enemy_00();		// CHR:敵[00]
	DrawEff_light_00();		
	DrawUi_life_000();		// UI :ライフ[00]
	DrawText_();			// テキスト
}

//========================================
// SetMd_game_00State関数 - MD:ゲーム画面[00] の状態を設定 -
// Author:RIKU NISHIMURA
//========================================
void SetMd_game_00State(MD_GAME_00_STATE state)
{
	// MD:ゲーム画面[00] の状態に応じた終了処理
	EndMd_game_00State();

	// MD:ゲーム画面[00] の状態を代入
	g_md_game_00.state = state;

	// MD:ゲーム画面[00] の状態に応じた開始処理
	StartMd_game_00State();
}
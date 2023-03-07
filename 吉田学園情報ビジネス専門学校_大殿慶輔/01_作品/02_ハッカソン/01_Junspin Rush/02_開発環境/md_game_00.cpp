//========================================
// 
// ゲーム画面[00]の処理
// Author:西村 吏功
// 
//========================================
// *** md_game_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "md_game_00.h"
#include "bg.h"
#include "life.h"
#include "ground.h"
#include "player.h"
#include "score.h"
#include "cloud.h"
#include "combo.h"
#include <stdio.h>

//****************************************
// マクロ定義 
//****************************************

// 隠しコマンドの有無
#define SECRET_COMMAND	(true)

// 出現待ちの時間
// 死亡後の余韻の時間
#define POP_WAIT_TIME		(120)
#define DIED_AFTERGLOW_TIME	(120)

// ウェーブ待ちの時間
// ウェーブ待ちの余韻
// ウェーブテキストの中間までの移動にかかる時間
// ウェーブテキストの終了までの移動を開始する時間
// ウェーブテキストの初期の位置
// ウェーブテキストの中間の位置
// ウェーブテキストの最後の位置
// ウェーブテキストの大きさにかかる倍率
// ウェーブテキストの色
#define WAVE_WAIT_TIME						(60)
#define WAVE_WAIT_AFTERGLOW					(20)
#define WAVE_TEXT_INTERMEDIATE_MOVE_TIME	(20)
#define WAVE_TEXT_END_MOVE_START_TIME		(40)
#define WAVE_TEXT_FIRST_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_INTERMEDIATE_POS			(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_LAST_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_SCALE						(ScaleDiameter{1.6f, 1.6f})
#define WAVE_TEXT_COLOR						(Color{255, 255, 255, 255})

// スタート待ちの時間
// スタートテキストの位置
// スタートテキストの中間までにかかる時間
// スタートテキストの終了までの推移を開始する時間
// スタートテキストの初期の大きさにかかる倍率
// スタートテキストの中間の大きさにかかる倍率
// スタートテキストの最後の大きさにかかる倍率
// スタートテキストの色
#define START_WAIT_TIME					(60)
#define START_TEXT_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define START_TEXT_INTERMEDIATE_TIME	(20)
#define START_TEXT_END_START_TIME		(40)
#define START_TEXT_FIRST_SCALE			(ScaleDiameter{8.0f, 8.0f})
#define START_TEXT_INTERMEDIATE_SCALE	(ScaleDiameter{2.0f, 2.0f})
#define START_TEXT_LAST_SCALE			(ScaleDiameter{8.0f, 0.0f})
#define START_TEXT_COLOR				(Color{243, 191, 63, 255})

// リザルトスコアの加算にかかる時間
// リザルトスコアの加算回数
// リザルトスコアの表示にかかる時間
// リザルトスコアの表示完了時の色
#define RESULT_SCORE_ADD_TIME			(3)
#define RESULT_SCORE_ADD_NUM			(17)
#define RESULT_SCORE_SHOW_TIME			(8)
#define RESULT_SCORE_SHOW_COMP_COLOR	(Color{243, 191, 63, 255})

// ランキングテキストの開始位置
// ランキングテキストの終了位置
// ランキングテキストの移動にかかる時間
// ランキングテキストの配置間隔X
// ランキングテキストの点滅色
// ランキングテキストの点滅にかかる時間
// ランキングフレームの位置
#define RANKING_TEXT_FIRST_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.4f) - (PIXEL * 8 * 9), PIXEL * 140, 0.0f))
#define RANKING_TEXT_LAST_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (PIXEL * 8 * 9), PIXEL * 140, 0.0f))
#define RANKING_TEXT_MOVE_TIME		(10)
#define RANKING_TEXT_SPACE_Y		(PIXEL * 16)
#define RANKING_TEXT_BRINK_COLOR	(Color{243, 191, 63, 255})
#define RANKING_TEXT_BRINK_TIME		(4)
#define RANKING_FRAME_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 16), 0.0f))

// ポーズテキストの点滅にかかる時間
#define PAUSE_TEXT_BRINK_TIME	(16)

// ポーズメニューの位置
// リザルトメニューの位置
#define MD_GAME_00_PAUSE_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 80, 0.0f))
#define MD_GAME_00_RESULT_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 64, 0.0f))

// コンティニューのカウント回数
// コンティニューのカウントにかかる時間
// コンティニューの余韻にかかる時間
// コンティニュー押下催促の点滅にかかる時間
#define CONTINUE_COUNT_NUM			(10)
#define CONTINUE_COUNT_TIME			(60)
#define CONTINUE_AFTERGLOW_TIME		(60)
#define PUSH_CONTINUE_BRINK_TIME	(30)

// ゲームクリアにかかる時間
// ゲームオーバーにかかる時間
// ゲームクリアのテキストにかかる時間
// ゲームオーバーのテキストにかかる時間
#define GAME_CLEAR_TIME				(300)
#define GAME_OVER_TIME				(300)
#define GAME_CLEAR_TEXT_TIME		(12)
#define GAME_OVER_TEXT_TIME			(12)

// ノーダメージボーナススコア
// ノーダメージボーナススコアの被弾毎の減少量
#define NO_DAMAGE_BONUS_SCORE			(100000)
#define NO_DAMAGE_BONUS_SCORE_DECREASE	(20000)

// 矢印の相対位置
#define ALLOW_RELATIVE_POSITION	(D3DXVECTOR3(PIXEL * 8 * 6.5f,PIXEL * 0,0.0f))

// 敵の設定情報の最大
#define ENEMY_SET_MAX	(256)
#define TIME			(90)

//****************************************
// 列挙型の定義
//****************************************

// タイトル画面[00] のリザルトメニュー
typedef enum
{
	MD_GAME_00_RESULT_MENU_RETRY = 0,		// リトライ
	MD_GAME_00_RESULT_MENU_BACK_TO_TITLE,	// タイトルに戻る
	MD_GAME_00_RESULT_MENU_MAX,
}MD_GAME_00_RESULT_MENU;

// タイトル画面[00] のポーズメニュー
typedef enum
{
	MD_GAME_00_PAUSE_MENU_RESUME = 0,		// 再開
	MD_GAME_00_PAUSE_MENU_RETRY,			// リトライ
	MD_GAME_00_PAUSE_MENU_SETTING,			// 設定
	MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE,	// タイトルに戻る
	MD_GAME_00_PAUSE_MENU_MAX,
}MD_GAME_00_PAUSE_MENU;

//****************************************
// 構造体の定義
//****************************************

// 合図テキストの情報構造体
typedef struct 
{
	char			aString				// 
					[TXT_MAX];			// 文字列
	D3DXVECTOR3		pos;				// 位置
	ScaleDiameter	scaleDiameter;		// 大きさにかかる倍率
	Color			col;				// 色
}SignalText;

// ランキングテキストの情報構造体
typedef struct 
{
	D3DXVECTOR3 pos;			// 位置
	int			nCounterMove;	// 移動カウンター
	Color		col;			// 色
}RankingText;

//****************************************
// プロトタイプ宣言
//****************************************

// テキスト内容の更新処理
void UpdateTxtMd_game_00(void);

// ゲームの状態に応じた開始処理
void StartGameState(MD_GAME_00_STATE state);

// ゲームの状態に応じた終了処理
void EndGameState(MD_GAME_00_STATE state);

// ゲームの状態に応じた更新処理
void UpdateGameState(void);

//****************************************
// グローバル宣言
//****************************************
MD_GAME_00_STATE	g_gameState				//
					= MD_GAME_00_STATE_N1;	// ゲームの状態
int					g_nStateCounter;		// 状態のカウンター
SignalText			g_signalText;			// 合図テキストの情報
bool				g_bResultMenu;			// リザルトメニューの出現フラグ
int					g_nEnemyNum = 0;		// 敵の数

EnemySet	g_aEnemySet[ENEMY_SET_MAX];	// 敵の設定情報
int			g_nCountertime;				// 時間のカウンター
int			g_nTime;					// 時間
int			g_nScore;					// スコア

// ゲーム画面[00] のテキストの情報
Text g_aMd_game_00Text[MD_GAME_00_TEXT_MAX] =
{
	{ ""			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)									, {255, 255, 255, 255} , FONT_001 },	// 合図
	{ "TIME  :%d"	, D3DXVECTOR3(PIXEL * 8 * 24, PIXEL * 8 * 2.5f, 0.0f)			, {255, 255, 255, 255} , FONT_000 },	// 時間
	{ "SCORE :%08d"	, D3DXVECTOR3(PIXEL * 8 * 24, PIXEL * 8 * 1.5f, 0.0f)			, {255, 255, 255, 255} , FONT_000 },	// スコア
	{ "STAGE CLEAR"	, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)	, {243, 191, 63 , 255} , FONT_001 },	// ゲームクリア
	{ "GAME OVER"	, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)	, {88 , 248, 152, 255} , FONT_001 },	// ゲームオーバー
};

// スコアを取得
int *GetScore(void) 
{
	return &g_nScore;
}

//========== *** ゲーム画面[00]のテキストの情報を取得 ***
Text GetMd_game_00Text(MD_GAME_00_TEXT text)
{
	return g_aMd_game_00Text[text];
}

//========== *** ゲームの状態の取得	***
MD_GAME_00_STATE GetGameState(void)
{
	return g_gameState;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateTxtMd_game_00関数 - テキスト内容の更新処理 -
//========================================
void UpdateTxtMd_game_00(void)
{
	char			aString					//
					[TXT_MAX];				// 文字列代入用
		
	//========== *** テキスト[スコア] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].aDisplay,g_nScore);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_SCORE,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[タイム] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_TIME].aDisplay, g_nTime);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_TIME,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_TIME].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_TIME].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_TIME].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[合図] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_SIGNAL,
			g_signalText.aString,
			g_signalText.pos,
			g_signalText.col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SIGNAL].font,
			g_signalText.scaleDiameter.fWidth,
			g_signalText.scaleDiameter.fHeight);
	}
	//========== *** テキスト[ゲームクリア] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay);
		int nText = (g_nStateCounter / GAME_CLEAR_TEXT_TIME) + 1;
		IntControl(&nText, strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay), 0);
		aString[(g_nStateCounter / GAME_CLEAR_TEXT_TIME) + 1] = '\0';
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_GAME_CLEAR,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].font,
			1.0f,
			1.0f);
	}
	//========== *** テキスト[ゲームオーバー] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay);
		int nText = (g_nStateCounter / GAME_OVER_TEXT_TIME) + 1;
		IntControl(&nText, strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay), 0);
		aString[(g_nStateCounter / GAME_OVER_TEXT_TIME) + 1] = '\0';
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_GAME_OVER,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].font,
			1.0f,
			1.0f);
	}
}

//========================================
// StartGameState関数 - ゲームの状態に応じた開始処理 -
//========================================
void StartGameState(MD_GAME_00_STATE state)
{
	// 状態のカウンターを初期化する
	g_nStateCounter = 0;

	// 開始処理
	switch (g_gameState)
	{
		//========== *** ポーズ ***
	case MD_GAME_00_STATE_PAUSE:
	{
		
	}
	break;
		//========== *** スタート待ち ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// 合図テキストの色を設定
		g_signalText.col = START_TEXT_COLOR;

		// 透明度を0にする
		g_signalText.col.nA = 0;

		// 位置を設定
		g_signalText.pos = START_TEXT_POS;

		// 大きさにかかる倍率を設定
		g_signalText.scaleDiameter = START_TEXT_FIRST_SCALE;

		// 文字列を設定する
		sprintf(g_signalText.aString, "START");
	}
	break;
		//========== *** 通常 ***
	case MD_GAME_00_STATE_NORMAL:
	{

	}
	break;
		//========== *** ゲームクリア ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		// テキスト[ゲームクリア]	を表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_CLEAR, true);

		// ゲームクリアSE[000] を再生
		PlaySound(SOUND_LABEL_SE_GAME_CLEAR_000);
	}
	break;
		//========== *** ゲームオーバー ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		// テキスト[ゲームオーバー]	を表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_OVER, true);

		// ゲームオーバーSE[000] を再生
		PlaySound(SOUND_LABEL_SE_GAME_OVER_000);
	}
		break;
		//========== *** 死亡後の余韻 ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{
		
	}
	break;
		//========= *** 終了 ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
	}
}

//========================================
// EndGameState関数 - ゲームの状態に応じた終了処理 -
//========================================
void EndGameState(MD_GAME_00_STATE state)
{
	switch (g_gameState)
	{
		//========== *** ポーズ ***
	case MD_GAME_00_STATE_PAUSE:
	{
		
	}
	break;
		//========== *** スタート待ち ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// テキスト[合図] を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SIGNAL, false);
	}
	break;
		//========== *** 通常 ***
	case MD_GAME_00_STATE_NORMAL:
	{

	}
	break;
		//========== *** ゲームクリア ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		// テキスト[ゲームクリア]	を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_CLEAR, false);
	}
	break;
		//========== *** ゲームオーバー ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		// テキスト[ゲームオーバー]	を非表示
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_OVER, false);
	}
	break;
		//========== *** 終了 ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
		//========== *** 死亡後の余韻 ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{

	}
	break;
	}
}

//========================================
// UpdateGameState関数 - ゲームの状態に応じた更新処理 -
//========================================
void UpdateGameState(void)
{
	switch (g_gameState)
	{
		//========== *** ポーズ ***
	case MD_GAME_00_STATE_PAUSE:
	{
		
	}
	break;
		//========== *** 通常 ***
	case MD_GAME_00_STATE_NORMAL:
	{
		
	}
	break;
		//========== *** スタート待ち ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// 状態のカウンターを加算
		g_nStateCounter++;

		if (g_nStateCounter <= START_TEXT_INTERMEDIATE_TIME)
		{// 状態のカウンターが中間までにかかる時間以下の時、
			// 大きさにかかる倍率を更新
			g_signalText.scaleDiameter.fWidth
				= (START_TEXT_FIRST_SCALE.fWidth * (1.0f - ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME)))
				+ (START_TEXT_INTERMEDIATE_SCALE.fWidth * ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME));
			g_signalText.scaleDiameter.fHeight
				= (START_TEXT_FIRST_SCALE.fHeight * (1.0f - ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME)))
				+ (START_TEXT_INTERMEDIATE_SCALE.fHeight * ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME));

			// 透明度を更新
			g_signalText.col.nA = 255
				* ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME);
		}

		if (g_nStateCounter >= START_TEXT_END_START_TIME)
		{// 状態のカウンターが終了までの移動を開始する時間以上の時、
			// 大きさにかかる倍率を更新
			g_signalText.scaleDiameter.fWidth
				= (START_TEXT_INTERMEDIATE_SCALE.fWidth
					* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME))))
				+ (START_TEXT_LAST_SCALE.fWidth * ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));
			g_signalText.scaleDiameter.fHeight
				= (START_TEXT_INTERMEDIATE_SCALE.fHeight
					* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME))))
				+ (START_TEXT_LAST_SCALE.fHeight * ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));

			// 透明度を更新
			g_signalText.col.nA = 255
				* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));
		}

		if ((g_nStateCounter >= START_WAIT_TIME))
		{//	状態のカウンターがスタート待ちの時間に達した時、
			// ゲームの状態を通常に移行
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}
	}
	break;
		//========== *** ゲームクリア ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		if (g_nStateCounter >= GAME_CLEAR_TIME) 
		{// 状態のカウンターがゲームクリアにかかる時間に達した時、
			// 状態をリザルトに移行
			
		}
		else if ((g_nStateCounter % GAME_CLEAR_TEXT_TIME == 0)
			&&((g_nStateCounter / GAME_CLEAR_TEXT_TIME) < strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay)))
		{// 状態のカウンターがゲームクリアのテキストにかかる時間で割り切れる時、
			
		}

		// 状態のカウンターを加算
		g_nStateCounter++;
	}
	break;
		//========== *** ゲームオーバー ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		if (g_nStateCounter >= GAME_OVER_TIME)
		{// 状態のカウンターがゲームオーバーにかかる時間に達した時、
			// 状態をリザルトに移行
			
		}
		else if ((g_nStateCounter % GAME_OVER_TEXT_TIME == 0)
			&& ((g_nStateCounter / GAME_OVER_TEXT_TIME) < strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay)))
		{// 状態のカウンターがゲームオーバーのテキストにかかる時間で割り切れる時、
			
		}

		// 状態のカウンターを加算
		g_nStateCounter++;
	}
	break;
		//========== *** 死亡後の余韻 ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{
		// 状態のカウンターを加算
		g_nStateCounter++;

		if (g_nStateCounter >= DIED_AFTERGLOW_TIME)
		{// 状態のカウンターが死亡後の余韻にかかる時間に達した時、
			
		}
	}
	break;
		//========== *** 終了 ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitMd_game_00関数 - ゲーム画面[00]の初期化処理 -
//========================================
void InitMd_game_00(void)
{
	g_gameState		= MD_GAME_00_STATE_N1;	// ゲームの状態を初期化
	g_nCountertime	= 0;						// 時間のカウンターを初期化
	g_nTime			= TIME;						// 時間を初期化
	g_nScore		= 0;						// スコアの初期化

	// 初期化処理
	InitTxt_00();			// テキスト
	InitChr_enemy_00();		// CHR:敵					[00]
	InitGround();			// 背景
	InitPlayer();			// プレイヤー
	InitAtk_bullet_00();	// 弾
	InitAtk_explosion_00();	// 爆発
	InitBg();				// 背景
	InitLife();				// 体力
	InitCloud();			// 雲
	InitCombo();			// コンボ

	// ゲームの状態を開始待ちに設定
	SetMd_game_00State(MD_GAME_00_STATE_START_WAIT);

	// テキスト[スコア]	を表示
	ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE, true);
	ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_TIME, true);

	PlaySound(SOUND_LABEL_BGM_STAGE_000);

	SetCloud2();		// 雲設定
}

//========================================
// UninitMd_game_00関数 - ゲーム画面[00]の終了処理 -
//========================================
void UninitMd_game_00(void)
{
	// 終了処理
	UninitTxt_00();				// テキスト
	UninitChr_enemy_00();		// CHR:敵	[00]
	UninitGround();				// 背景
	UninitPlayer();				// プレイヤー
	UninitAtk_bullet_00();		// 弾
	UninitAtk_explosion_00();	// 爆発
	UninitBg();					// 背景
	UninitLife();				// 体力
	UninitCloud();			// 雲
	UninitCombo();			// コンボ

}

//========================================
// UpdateMd_game_00関数 - ゲーム画面[00]の更新処理 -
//========================================
void UpdateMd_game_00(void)
{
	if (++g_nCountertime % 60 == 0
		&&
		GetFade() == FADE_NONE)
	{// 時間カウンターが60に達した時、
		if (--g_nTime <= 0)
		{// 時間を減算した結果0以下になった時、
			// リザルトに以降
			SetFade(MODE_RESULT_00);
		}
		else 
		{
			for (int nCntSetEnemy = 0; nCntSetEnemy < g_nEnemyNum; nCntSetEnemy++) 
			{
				if (TIME - g_aEnemySet[nCntSetEnemy].in_time == g_nTime + 1)
				{// 出現時間が一致した時、
					// 設定
					SetChr_enemy_00(
						g_aEnemySet[nCntSetEnemy].pos,
						D3DXVECTOR3(0.0f, 0.0f, g_aEnemySet[nCntSetEnemy].nAngle),
						g_aEnemySet[nCntSetEnemy].nType,
						false,
						false,
						-1, 
						-1);
				}
			}
		}
	}

	if ((g_gameState != MD_GAME_00_STATE_PAUSE)
		&& (g_gameState != MD_GAME_00_STATE_N1))
	{// 状態がポーズ/無し/設定でない時、
		// 更新処理
		UpdateChr_enemy_00();	// CHR:敵	[00]
		UpdateGround();			// 背景
		UpdatePlayer();			// プレイヤー
		UpdateAtk_bullet_00();	// 弾
		UpdateAtk_explosion_00();	// 爆発
		UpdateBg();					// 背景
		UpdateLife();				// 体力
		UpdateBg();					// 背景
		UpdateLife();				// 体力
		UpdateCloud();			// 雲
		UpdateCombo();			// コンボ

	}

	// ゲームの状態に応じた更新処理
	UpdateGameState();

	// テキスト内容の更新処理
	UpdateTxtMd_game_00();
}

//========================================
// DrawMd_game_00関数 - ゲーム画面[00]の描画処理 -
//========================================
void DrawMd_game_00(void)
{
	// 描画処理
	DrawCloud();			// 雲
	DrawBg();				// 背景
	DrawChr_enemy_00();		// CHR:敵	[00]
	DrawPlayer();			// プレイヤー
	DrawTxt_00();
	DrawAtk_bullet_00();	// 弾
	DrawAtk_explosion_00();	// 爆発
	DrawGround();			// 背景

	DrawLife();				// 体力
	DrawCombo();			// コンボ
}

//========================================
// SetGameState関数 - ゲームの状態の設定 -
//========================================
void SetMd_game_00State(MD_GAME_00_STATE state) 
{
	MD_GAME_00_STATE	stateTemp		// 
						= g_gameState;	// 元の状態を一時的に格納

	// ゲームの状態に応じた終了処理
	EndGameState(state);

	// ゲームの状態を反映
	g_gameState = state;

	// ゲームの状態に応じた開始処理
	StartGameState(stateTemp);
}

//◇====================================◇
//｜　　　 エネミーデータ読み込み　　 　｜
//◇====================================◇
void LoadEnemy(void)
{
	int c = 0;	   //1文字ずつ確認する変数
	int column = 1;//列数を数える変数
	int line = 0;//行数を数える変数
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
		case 1:	g_aEnemySet[line].nType = atoi(aData);				break;	//１列目：敵・種類
		case 2:	g_aEnemySet[line].in_time = atoi(aData);			break;	//　列目：出現時間
		case 3:	g_aEnemySet[line].pos.x = (float)(atoi(aData));	break;	//　列目：X座標
		case 4:	g_aEnemySet[line].pos.y = (float)(atoi(aData));	break;	//　列目：Y座標
		case 5:	g_aEnemySet[line].nAngle = (float)(atoi(aData));	break;	//　列目：角度
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
			g_nEnemyNum++;
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}
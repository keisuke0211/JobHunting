//========================================
// 
// MD:チュートリアル画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_tutorial_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "color.h"
#include "fade.h"
#include "input.h"
#include "physics.h"
#include "sound.h"
#include "text.h"
#include "bg_tutorial_00.h"		// BG :チュートリアル		[00]
#include "md_tutorial_00.h"		// MD :チュートリアル画面	[00]
#include <stdio.h>	

//****************************************
// マクロ定義
//****************************************
// MD:チュートリアル画面[00] のデータファイルの相対パス
#define BG_TUTORIAL_00_DATA_FILE_PATH	"data\\GAMEDATA\\MODE\\MD_TUTORIAL_00_DATA.txt"

//****************************************
// プロトタイプ宣言
//****************************************
// MD:チュートリアル画面[00] のパラメーターの初期化処理
// MD:チュートリアル画面[00] の状態に応じた更新処理
// MD:チュートリアル画面[00] の状態に応じた開始処理
// MD:チュートリアル画面[00] の状態に応じた終了処理
void InitParameterMd_tutorial_00(void);
void UpdateMd_tutorial_00State(void);
void StartMd_tutorial_00State(void);
void EndMd_tutorial_00State(void);

//****************************************
// グローバル宣言
//****************************************
Md_tutorial_00	g_md_tutorial_00;	// MD:チュートリアル画面[00] の情報

//========== *** MD:チュートリアル画面[00] の情報を取得 ***
Md_tutorial_00 *GetMd_tutorial_00(void) 
{
	return &g_md_tutorial_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMd_tutorial_00関数 - MD:チュートリアル画面[00] の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMd_tutorial_00(void) 
{
	FILE			*pFile;		// ファイルポインタ
	char			aDataSearch	// データ検索用
					[TXT_MAX];
	Md_tutorial_00	*pMd		// MD:チュートリアル画面[00] の情報のポインタ
					= &g_md_tutorial_00;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(BG_TUTORIAL_00_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if		(!strcmp(aDataSearch, "END"))					{ fclose(pFile); break; }							// 読み込みを終了
		else if (!strcmp(aDataSearch, "PROMPT_TEXT_FONT:"))		{ fscanf(pFile, "%d", &pMd->nPromptTextFont); }		// 入力催促テキストのフォント
		else if (!strcmp(aDataSearch, "PROMPT_TEXT_COLOR:"))	{ LoadColor(pFile, &pMd->promptTextCol); }			// 入力催促テキストのフォント
		else if (!strcmp(aDataSearch, "PROMPT_TEXT_POS:"))		{ LoadVector(pFile, &pMd->promptTextPos); }			// 入力催促テキストの位置
		else if (!strcmp(aDataSearch, "WAIT_BLINK_TIME:"))		{ fscanf(pFile, "%d", &pMd->nWaitBlinkTime); }		// 待機時の点滅にかかる時間
		else if (!strcmp(aDataSearch, "DECISION_BLINK_TIME:"))	{ fscanf(pFile, "%d", &pMd->nDecisionBlinkTime); }	// 決定時の点滅にかかる時間
		else if (!strcmp(aDataSearch, "BGM:"))					{ fscanf(pFile, "%d", &pMd->nBGM); }				// BGM
		else if (!strcmp(aDataSearch, "DECITION_SE:"))			{ fscanf(pFile, "%d", &pMd->nDecisionSE); }			// 決定SE
	}
}

//========================================
// InitParameterMd_tutorial_00関数 - MD:チュートリアル画面[00] のパラメーターの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMd_tutorial_00(void)
{
	Md_tutorial_00	*pMd	// MD:チュートリアル画面[00] の情報
				= &g_md_tutorial_00;

	pMd->state				= MD_TUTORIAL_00_STATE_NONE;	// 状態
	pMd->nCounterState		= 0;						// 状態カウンター
	pMd->nCounterPromptText = 0;						// 入力催促テキストの汎用カウンター
}

//========================================
// StartMd_tutorial_00State関数 - MD:チュートリアル画面[00] の状態に応じた開始処理 -
// Author:RIKU NISHIMURA
//========================================
void StartMd_tutorial_00State(void)
{
	Md_tutorial_00	*pMd	// MD:チュートリアル画面[00] の情報
				= &g_md_tutorial_00;

	// 状態カウンターを初期化
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TUTORIAL_00_STATE_NORMAL: {

		break;
	}
	}
}

//========================================
// EndMd_tutorial_00State関数 - MD:チュートリアル画面[00] の状態に応じた終了処理 -
// Author:RIKU NISHIMURA
//========================================
void EndMd_tutorial_00State(void) 
{
	Md_tutorial_00	*pMd	// MD:チュートリアル画面[00] の情報
				= &g_md_tutorial_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TUTORIAL_00_STATE_NORMAL: {

		break;
	}
	}
}

//========================================
// UpdateMd_tutorial_00State関数 - MD:チュートリアル画面[00] の状態に応じた更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_tutorial_00State(void)
{
	Md_tutorial_00	*pMd	// MD:チュートリアル画面[00] の情報
				= &g_md_tutorial_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TUTORIAL_00_STATE_NORMAL: {
		if (((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A))) && (GetFade() == FADE_NONE))
		{// ENTERキーorAボタンが押された & フェードが無しの時、
			PlaySound(pMd->nDecisionSE);	// 決定SEを再生
			SetFade(MODE_GAME_00);			// モードをMD:ゲーム画面[00] に移行
		}
		
		if (GetFade() == FADE_OUT) 
		{// フェードアウト状態の時、入力催促テキストの汎用カウンターを加算&制御
			++pMd->nCounterPromptText %= (pMd->nDecisionBlinkTime * 2);
		}

		if ((pMd->nCounterPromptText / pMd->nDecisionBlinkTime) % 2 == 0)
		{// 入力催促テキストの汎用カウンターの進行度に応じてテキストの設定
			SetText2D(
				"PRESS A BUTTON",
				0,
				DISPLAY_CENTER,
				pMd->promptTextPos,
				pMd->promptTextCol,
				1.0f, 1.0f);
		}
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
// InitMd_tutorial_00関数 - MD:チュートリアル画面[00]の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitMd_tutorial_00(void)
{
	Md_tutorial_00	*pMd	// MD:チュートリアル画面[00] の情報
				= &g_md_tutorial_00;

	// パラメーターの初期化処理
	InitParameterMd_tutorial_00();

	// 初期化処理
	InitParameterCamera3D();	// カメラ(3D)のパラメーター
	InitBg_tutorial_00();		// BG :チュートリアル	[00]
	
	// 状態をロゴ待ちに設定
	SetMd_tutorial_00State(MD_TUTORIAL_00_STATE_NORMAL);

	// BGMを再生
	PlaySound(7);
}

//========================================
// UninitMd_tutorial_00関数 - MD:チュートリアル画面[00]の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_tutorial_00(void)
{
	// 終了処理
	UninitBg_tutorial_00();	// BG :チュートリアル	[00]
}

//========================================
// UpdateMd_tutorial_00関数 - MD:チュートリアル画面[00]の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_tutorial_00(void)
{
	// MD:チュートリアル画面[00] の状態に応じた更新処理
	UpdateMd_tutorial_00State();

	// 更新処理
	UpdateBg_tutorial_00();	// BG :チュートリアル	[00]
}

//========================================
// DrawMd_tutorial_00関数 - MD:チュートリアル画面[00]の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_tutorial_00(void)
{
	// 描画処理
	DrawBg_tutorial_00();	// BG :チュートリアル	[00]
	DrawText_();			// テキスト
}

//========================================
// SetMd_tutorial_00State関数 - MD:チュートリアル画面[00] の状態を設定 -
// Author:RIKU NISHIMURA
//========================================
void SetMd_tutorial_00State(MD_TUTORIAL_00_STATE state)
{
	// MD:チュートリアル画面[00] の状態に応じた終了処理
	EndMd_tutorial_00State();

	// MD:チュートリアル画面[00] の状態を代入
	g_md_tutorial_00.state = state;

	// MD:チュートリアル画面[00] の状態に応じた開始処理
	StartMd_tutorial_00State();
}
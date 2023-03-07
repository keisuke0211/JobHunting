//========================================
// 
// MD:タイトル画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_title_00.cpp ***
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
#include "bg_title_00.h"		// BG :タイトル		[00]
#include "md_title_00.h"		// MD :タイトル画面	[00]
#include "ui_title-logo_00.h"	// UI :タイトルロゴ	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// MD:タイトル画面[00] のデータファイルの相対パス
#define BG_TITLE_00_DATA_FILE_PATH	"data\\GAMEDATA\\MODE\\MD_TITLE_00_DATA.txt"

//****************************************
// プロトタイプ宣言
//****************************************
// MD:タイトル画面[00] のパラメーターの初期化処理
// MD:タイトル画面[00] の状態に応じた更新処理
// MD:タイトル画面[00] の状態に応じた開始処理
// MD:タイトル画面[00] の状態に応じた終了処理
void InitParameterMd_title_00(void);
void UpdateMd_title_00State(void);
void StartMd_title_00State(void);
void EndMd_title_00State(void);

//****************************************
// グローバル宣言
//****************************************
Md_title_00	g_md_title_00;	// MD:タイトル画面[00] の情報

//========== *** MD:タイトル画面[00] の情報を取得 ***
Md_title_00 *GetMd_title_00(void) 
{
	return &g_md_title_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMd_title_00関数 - MD:タイトル画面[00] の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMd_title_00(void) 
{
	FILE		*pFile;		// ファイルポインタ
	char		aDataSearch	// データ検索用
				[TXT_MAX];
	Md_title_00	*pMd		// MD:タイトル画面[00] の情報のポインタ
				= &g_md_title_00;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(BG_TITLE_00_DATA_FILE_PATH, "r");

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
// InitParameterMd_title_00関数 - MD:タイトル画面[00] のパラメーターの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMd_title_00(void)
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	pMd->state				= MD_TITLE_00_STATE_NONE;	// 状態
	pMd->nCounterState		= 0;						// 状態カウンター
	pMd->nCounterPromptText = 0;						// 入力催促テキストの汎用カウンター
}

//========================================
// StartMd_title_00State関数 - MD:タイトル画面[00] の状態に応じた開始処理 -
// Author:RIKU NISHIMURA
//========================================
void StartMd_title_00State(void)
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	// 状態カウンターを初期化
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TITLE_00_STATE_NORMAL: {

		break;
	}
	case /*/ ロゴ待ち /*/MD_TITLE_00_STATE_LOGOWAIT: {

		break;
	}
	}
}

//========================================
// EndMd_title_00State関数 - MD:タイトル画面[00] の状態に応じた終了処理 -
// Author:RIKU NISHIMURA
//========================================
void EndMd_title_00State(void) 
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TITLE_00_STATE_NORMAL: {

		break;
	}
	case /*/ ロゴ待ち /*/MD_TITLE_00_STATE_LOGOWAIT: {

		break;
	}
	}
}

//========================================
// UpdateMd_title_00State関数 - MD:タイトル画面[00] の状態に応じた更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_title_00State(void)
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_TITLE_00_STATE_NORMAL: {
		if (((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A))) && (GetFade()==FADE_NONE)) 
		{// ENTERキーorAボタンが押された & フェードが無しの時、
			PlaySound(pMd->nDecisionSE);	// 決定SEを再生
			SetFade(MODE_TUTORIAL_00);		// モードをMD:チュートリアル画面[00] に移行
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
	case /*/ ロゴ待ち /*/MD_TITLE_00_STATE_LOGOWAIT: {
		if (GetUi_titleLogo_00()->nCounterArrival >= GetUi_titleLogo_00()->nArrivalTime) 
		{// UI:タイトルロゴ[00] の到着カウンターが到着カウンターに達している時、
			SetMd_title_00State(MD_TITLE_00_STATE_NORMAL);	// 状態を通常に設定
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
// InitMd_title_00関数 - MD:タイトル画面[00]の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitMd_title_00(void)
{
	Md_title_00	*pMd	// MD:タイトル画面[00] の情報
				= &g_md_title_00;

	// パラメーターの初期化処理
	InitParameterMd_title_00();

	// 初期化処理
	InitParameterCamera3D();	// カメラ(3D)のパラメーター
	InitBg_title_00();			// BG :タイトル		[00]
	InitUi_titleLogo_00();		// UI :タイトルロゴ	[00]

	// 状態をロゴ待ちに設定
	SetMd_title_00State(MD_TITLE_00_STATE_LOGOWAIT);

	// BGMを再生
	PlaySound(pMd->nBGM);
}

//========================================
// UninitMd_title_00関数 - MD:タイトル画面[00]の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_title_00(void)
{
	// 終了処理
	UninitBg_title_00();		// BG :タイトル		[00]
	UninitUi_titleLogo_00();	// UI :タイトルロゴ	[00]
}

//========================================
// UpdateMd_title_00関数 - MD:タイトル画面[00]の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_title_00(void)
{
	// MD:タイトル画面[00] の状態に応じた更新処理
	UpdateMd_title_00State();

	// 更新処理
	UpdateBg_title_00();		// BG :タイトル		[00]
	UpdateUi_titleLogo_00();	// UI :タイトルロゴ	[00]
}

//========================================
// DrawMd_title_00関数 - MD:タイトル画面[00]の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_title_00(void)
{
	// 描画処理
	DrawBg_title_00();		// BG :タイトル		[00]
	DrawUi_titleLogo_00();	// UI :タイトルロゴ	[00]
	DrawText_();			// テキスト
}

//========================================
// SetMd_title_00State関数 - MD:タイトル画面[00] の状態を設定 -
// Author:RIKU NISHIMURA
//========================================
void SetMd_title_00State(MD_TITLE_00_STATE state)
{
	// MD:タイトル画面[00] の状態に応じた終了処理
	EndMd_title_00State();

	// MD:タイトル画面[00] の状態を代入
	g_md_title_00.state = state;

	// MD:タイトル画面[00] の状態に応じた開始処理
	StartMd_title_00State();
}
//========================================
// 
// MD:リザルト画面[00] の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** md_result_00.cpp ***
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
#include "player.h"
#include "bg_result_00.h"	// BG :リザルト		[00]
#include "bg_result_01.h"	// BG :リザルト		[01]
#include "md_result_00.h"	// MD :リザルト画面	[00]
#include "sys_ranking_00.h"	// SYS:ランキング	[00]
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// MD:リザルト画面[00] のデータファイルの相対パス
#define BG_RESULT_00_DATA_FILE_PATH	"data\\GAMEDATA\\MODE\\MD_RESULT_00_DATA.txt"

// ランキングテキストの位置
// ランキングテキストの開始位置X
// ランキングテキストの終了位置X
// ランキングテキストの位置Y
// ランキングテキストの間隔Y
// ランキングテキストセットにかかる時間
#define RANKING_TEXT_POS			D3DXVECTOR3(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f,0.0f)
#define RANKING_TEXT_START_POS_X	(PIXEL*-40)
#define RANKING_TEXT_END_POS_X		(PIXEL*0)
#define RANKING_TEXT_POS_Y			(PIXEL*-64)
#define RANKING_TEXT_SPACE_Y		(PIXEL*16)
#define RANKING_TEXT_SET_TIME		(90)

// ランキングテキストの点滅にかかる時間
// ランキングテキストの点滅色
#define RANKING_TEXT_BLINK_TIME		(4)
#define RANKING_TEXT_BLINK_COLOR	Color{243,189,63,255}

//****************************************
// プロトタイプ宣言
//****************************************
// MD:リザルト画面[00] のパラメーターの初期化処理
// MD:リザルト画面[00] の状態に応じた更新処理
// MD:リザルト画面[00] の状態に応じた開始処理
// MD:リザルト画面[00] の状態に応じた終了処理
void InitParameterMd_result_00(void);
void UpdateMd_result_00State(void);
void StartMd_result_00State(void);
void EndMd_result_00State(void);

//****************************************
// グローバル宣言
//****************************************
Md_result_00	g_md_result_00;	// MD:リザルト画面[00] の情報
int				g_nResultBg;	// MD:リザルト画面[00] の背景番号

//========== *** MD:リザルト画面[00] の情報を取得 ***
Md_result_00 *GetMd_result_00(void) 
{
	return &g_md_result_00;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadMd_result_00関数 - MD:リザルト画面[00] の読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadMd_result_00(void) 
{
	FILE			*pFile;		// ファイルポインタ
	char			aDataSearch	// データ検索用
					[TXT_MAX];
	Md_result_00	*pMd		// MD:リザルト画面[00] の情報のポインタ
					= &g_md_result_00;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(BG_RESULT_00_DATA_FILE_PATH, "r");

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
		else if (!strcmp(aDataSearch, "RANKINGSET_SE:"))		{ fscanf(pFile, "%d", &pMd->nRankingSetSE); }		// ランキングセットSE
		else if (!strcmp(aDataSearch, "DECITION_SE:"))			{ fscanf(pFile, "%d", &pMd->nDecisionSE); }			// 決定SE
	}
}

//========================================
// InitParameterMd_result_00関数 - MD:リザルト画面[00] のパラメーターの初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterMd_result_00(void)
{
	Md_result_00	*pMd	// MD:リザルト画面[00] の情報
					= &g_md_result_00;

	pMd->state				= MD_RESULT_00_STATE_NONE;	// 状態
	pMd->nCounterState		= 0;						// 状態カウンター
	pMd->nCounterPromptText = 0;						// 入力催促テキストの汎用カウンター
	pMd->nUpdateRank		= -1;						// 更新順位
}

//========================================
// StartMd_result_00State関数 - MD:リザルト画面[00] の状態に応じた開始処理 -
// Author:RIKU NISHIMURA
//========================================
void StartMd_result_00State(void)
{
	Md_result_00	*pMd	// MD:リザルト画面[00] の情報
					= &g_md_result_00;

	// 状態カウンターを初期化
	pMd->nCounterState = 0;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_RESULT_00_STATE_NORMAL: {

		break;
	}
	case /*/ ランキング待ち /*/MD_RESULT_00_STATE_RANKINGWAIT: {

		break;
	}
	}
}

//========================================
// EndMd_result_00State関数 - MD:リザルト画面[00] の状態に応じた終了処理 -
// Author:RIKU NISHIMURA
//========================================
void EndMd_result_00State(void) 
{
	Md_result_00	*pMd	// MD:リザルト画面[00] の情報
					= &g_md_result_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_RESULT_00_STATE_NORMAL: {

		break;
	}
	case /*/ ランキング待ち /*/MD_RESULT_00_STATE_RANKINGWAIT: {

		break;
	}
	}
}

//========================================
// UpdateMd_result_00State関数 - MD:リザルト画面[00] の状態に応じた更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_result_00State(void)
{
	Md_result_00	*pMd	// MD:リザルト画面[00] の情報
					= &g_md_result_00;

	switch (pMd->state)
	{
	case /*/ 通常 /*/MD_RESULT_00_STATE_NORMAL: {
		if (((GetKeyboardTrigger(DIK_RETURN)) || (GetButtonTrigger(BUTTON_A))) && (GetFade() == FADE_NONE))
		{// ENTERキーorAボタンが押された & フェードが無しの時、
			PlaySound(pMd->nDecisionSE);	// 決定SEを再生
			SetFade(MODE_TITLE_00);			// モードをMD:タイトル画面[00] に移行
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

		Sys_ranking_00	*pSys	// ランキング[00] の情報のポインタ
						= GetSys_ranking_00();

		++pMd->nCounterBlink %= (RANKING_TEXT_BLINK_TIME * 2);	// 点滅カウンターを加算&制御

		for (int nCntRanking = 0; nCntRanking < RANKING_NUM; nCntRanking++, pSys++)
		{
			char aString[TXT_MAX];
			sprintf(aString, "%s %8d", GetRankText(nCntRanking), pSys->nScore);
			Color setColor =
				pMd->nUpdateRank == nCntRanking ?
				!(pMd->nCounterBlink / RANKING_TEXT_BLINK_TIME) ?
				INITCOLOR : RANKING_TEXT_BLINK_COLOR :
				INITCOLOR;
			SetText2D(
				aString,
				0,
				DISPLAY_CENTER,
				RANKING_TEXT_POS +
				D3DXVECTOR3(
					RANKING_TEXT_END_POS_X,
					RANKING_TEXT_POS_Y + (RANKING_TEXT_SPACE_Y * nCntRanking),
					0.0f),
				setColor,
				1.0f,
				1.0f);
		}

		break;
	}
	case /*/ ランキング待ち /*/MD_RESULT_00_STATE_RANKINGWAIT: {
		// 状態カウンターを加算
		pMd->nCounterState++;

		// 状態カウンターを制御
		IntControl(&pMd->nCounterState, RANKING_TEXT_SET_TIME, 0);

		Sys_ranking_00	*pSys	// ランキング[00] の情報のポインタ
						= GetSys_ranking_00();

		const int	nTime	// 1つのランキング当たりにかかる時間
					= RANKING_TEXT_SET_TIME / RANKING_NUM;

		if ((pMd->nCounterState - 1) % nTime == 0)
		{// 状態カウンターが1つのランキング当たりにかかる時間で割り切れる時、
			PlaySound(pMd->nRankingSetSE);	// ランキングセットSEを再生
		}

		for (int nCntRanking = 0; nCntRanking < IntControlReturn((pMd->nCounterState / nTime) + 1, RANKING_NUM, 0); nCntRanking++, pSys++)
		{
			char aString[TXT_MAX];
			sprintf(aString, "%s %8d", GetRankText(nCntRanking), pSys->nScore);
			float fRate =	// 進行割合
				(float)(IntControlReturn(pMd->nCounterState, nTime * (nCntRanking + 1), 0) -
				(nTime * nCntRanking)) / (float)nTime;
			SetText2D(
				aString,
				0,
				DISPLAY_CENTER,
				RANKING_TEXT_POS +
				D3DXVECTOR3(
				(RANKING_TEXT_START_POS_X * (1.0f - fRate)) + (RANKING_TEXT_END_POS_X * fRate),
					RANKING_TEXT_POS_Y + (RANKING_TEXT_SPACE_Y * nCntRanking),
					0.0f),
					{ 255,255,255,(int)(255 * fRate) },
				1.0f,
				1.0f);
		}

		if (pMd->nCounterState >= RANKING_TEXT_SET_TIME)
		{// 状態カウンターがテキストセットにかかる時間に達した時、
			SetMd_result_00State(MD_RESULT_00_STATE_NORMAL);	// 状態を通常にする
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
// InitMd_result_00関数 - MD:リザルト画面[00]の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitMd_result_00(void)
{
	Md_result_00	*pMd	// MD:リザルト画面[00] の情報
					= &g_md_result_00;

	// パラメーターの初期化処理
	InitParameterMd_result_00();

	// 初期化処理
	InitParameterCamera3D();	// カメラ(3D)のパラメーター
	if (g_nResultBg == 0) { InitBg_result_00(); }	// BG :リザルト		[00]
	if (g_nResultBg == 1) { InitBg_result_01(); }	// BG :リザルト		[01]

	// 状態をランキング待ちに設定
	SetMd_result_00State(MD_RESULT_00_STATE_RANKINGWAIT);

	// BGMを再生
	PlaySound(8);

	// スコア設定
	pMd->nUpdateRank = SetScore(GetPlayer()->nScore);
}

//========================================
// UninitMd_result_00関数 - MD:リザルト画面[00]の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitMd_result_00(void)
{
	// 終了処理
	if (g_nResultBg == 0) { UninitBg_result_00(); }	// BG :リザルト		[00]
	if (g_nResultBg == 1) { UninitBg_result_01(); }	// BG :リザルト		[01]
}

//========================================
// UpdateMd_result_00関数 - MD:リザルト画面[00]の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateMd_result_00(void)
{
	// MD:リザルト画面[00] の状態に応じた更新処理
	UpdateMd_result_00State();

	// 更新処理
	if (g_nResultBg == 0) { UpdateBg_result_00(); }	// BG :リザルト		[00]
	if (g_nResultBg == 1) { UpdateBg_result_01(); }	// BG :リザルト		[01]
}

//========================================
// DrawMd_result_00関数 - MD:リザルト画面[00]の描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawMd_result_00(void)
{
	// 描画処理
	if (g_nResultBg == 0) { DrawBg_result_00(); }	// BG :リザルト		[00]
	if (g_nResultBg == 1) { DrawBg_result_01(); }	// BG :リザルト		[01]
	DrawText_();		// テキスト
}

//========================================
// SetMd_result_00State関数 - MD:リザルト画面[00] の状態を設定 -
// Author:RIKU NISHIMURA
//========================================
void SetMd_result_00State(MD_RESULT_00_STATE state)
{
	// MD:リザルト画面[00] の状態に応じた終了処理
	EndMd_result_00State();

	// MD:リザルト画面[00] の状態を代入
	g_md_result_00.state = state;

	// MD:リザルト画面[00] の状態に応じた開始処理
	StartMd_result_00State();
}

//========================================
// SetBgMd_result_00関数 - MD:リザルト画面[00] の背景を設定 -
// Author:RIKU NISHIMURA
//========================================
void SetBgMd_result_00(int nBg) 
{
	g_nResultBg = nBg;
}
//========================================
// 
// 設定の処理
// Author:西村 吏功
// 
//========================================
// *** setting.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "input.h"
#include "sound.h"
#include "setting.h"
#include "md_game_00.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************

// 設定の情報のデータファイルの相対パス
#define SETTING_DATA_FILE_PATH	"data\\SAVEDATA\\setting.bin"

// 音量の表示倍率
#define VOLUME_DISPLAY_DIAMETER	(5)

// 画面モードの切り替えにかかる時間
#define WINDOW_MODE_CHANGE_TIME	(20)

// 設定メニューの位置
#define SETTING_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 64, 0.0f))

//****************************************
// 列挙型の定義
//****************************************

// 設定メニュー
typedef enum
{
	SETTING_MENU_BGM_VOLUME = 0,	// BGM音量
	SETTING_MENU_SE_VOLUME,			// SE音量
	SETTING_MENU_SCREEN_MODE,		// 画面モード
	SETTING_MENU_VIBRATION,			// 振動
	SETTING_MENU_LASER,				// 光線
	SETTING_MENU_OK,				// 了解
	SETTING_MENU_MAX,
}SETTING_MENU;

//****************************************
// グローバル宣言
//****************************************
MODE	g_sourceMode;						// 元のモード
int		g_nCounterScreenModeChange = -1;	// 画面モード切り替えカウンター

// 設定の情報
Setting g_setting =
{// 初期設定
	VOLUME_STAGE / 2,
	VOLUME_STAGE / 2,
	false,
	true,
	true,
};

// 画面モードの表示形式
char g_aScreenModeText[2][TXT_MAX] =
{
	"FULL:OFF ",
	"FULL:ON  ",
};

// 振動の表示形式
char g_aVibrationText[2][TXT_MAX] =
{
	"VIB :OFF ",
	"VIB :ON  ",
};

// 光線の表示形式
char g_aLaserText[2][TXT_MAX] =
{
	"BEAM:OFF ",
	"BEAM:ON  ",
};

//========== *** 設定の情報を取得 ***
Setting *GetSetting(void) 
{
	return &g_setting;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitSetting関数 - 設定の初期化処理 -
//========================================
void InitSetting(void)
{

}

//========================================
// UninitSetting関数 - 設定の終了処理 -
//========================================
void UninitSetting(void)
{

}

//========================================
// UpdateSetting関数 - 設定の更新処理 -
//========================================
void UpdateSetting(void)
{
	if (g_nCounterScreenModeChange >= 0) 
	{// 画面モード切り替えカウンターが0以上の時、
		// 画面モード切り替えカウンターを減算
		g_nCounterScreenModeChange--;

		if (g_nCounterScreenModeChange == 0) 
		{// 画面モード切り替えカウンターが0の時、
			// ウインドウのモードを切り替える
			WindowModeChange(g_setting.bFullScreen);
		}
	}

	if (GetKeyboardTrigger(DIK_F11))
	{// F11キーが入力された時、
		// 全画面フラグを切り替える
		g_setting.bFullScreen ^= 1;

		// 画面モード切り替えカウンターを設定
		g_nCounterScreenModeChange = WINDOW_MODE_CHANGE_TIME;
	}
}

//========================================
// LoadSetting関数 - 設定の読み込み処理 -
//========================================
void LoadSetting(void)
{
	FILE *pFile;	//ファイルのポインタ

	//ファイルを開く
	pFile = fopen(SETTING_DATA_FILE_PATH, "rb");

	if (pFile != NULL)
	{//ファイルが開けた時、
		//ファイルから設定を読み込む
		fread(&g_setting, sizeof(Setting), 1, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}

#if _DEBUG
	// デバッグ時、全画面フラグを偽にする。(フリーズ対策)
	g_setting.bFullScreen = false;
#endif 

}

//========================================
// SaveSetting関数 - 設定の書き込み処理 -
//========================================
void SaveSetting(void)
{
	FILE *pFile;	//ファイルのポインタ

	//ファイルを開く
	pFile = fopen(SETTING_DATA_FILE_PATH, "wb");

	if (pFile != NULL)
	{//ファイルが開けた時、
		//ファイルに設定を書き出し
		fwrite(&g_setting, sizeof(Setting), 1, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
}
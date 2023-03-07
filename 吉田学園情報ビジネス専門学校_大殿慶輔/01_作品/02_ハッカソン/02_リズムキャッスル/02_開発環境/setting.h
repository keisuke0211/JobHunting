//========================================
//  *** setting.h ***
//========================================
#ifndef _SETTING_H_	// このマクロ定義がされていなかった時
#define _SETTING_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************

// 音量の段階
#define VOLUME_STAGE	(20)

//****************************************
// 構造体の定義
//****************************************

// 設定の情報構造体
typedef struct
{
	int nBGMVolume;		// BGM音量
	int nSEVolume;		// SE音量
	bool bVibration;	// 振動フラグ
}Setting;

//****************************************
// プロトタイプ宣言
//****************************************

// 設定の情報を取得
Setting *GetSetting(void);

// 設定の初期化処理
// 設定の終了処理
// 設定の更新処理
void InitSetting(void);
void UninitSetting(void);
void UpdateSetting(void);

// 設定の読み込み処理
// 設定の書き込み処理
void LoadSetting(void);
void SaveSetting(void);

#endif
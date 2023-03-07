//========================================
// 
// サウンド処理のヘッダファイル
// Author : AKIRA TANAKA
// |
// Arrange : 西村 吏功
// 
//========================================
// *** sys_stage_00.h ***
//========================================
#ifndef _SOUND_H_	// このマクロ定義がされていなかった時
#define _SOUND_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// 列挙型の定義
//****************************************

// サウンドの種類
typedef enum 
{
	SOUND_TYPE_BGM = 0,	// 音楽
	SOUND_TYPE_SE,		// 効果音
	SOUND_TYPE_MAX,
}SOUND_TYPE;

// サウンド一覧
typedef enum
{
	SOUND_LABEL_BGM_TITLE_000 = 0,		// タイトルBGM			[000]
	SOUND_LABEL_BGM_STAGE_000,			// ステージBGM			[000]
	SOUND_LABEL_BGM_RESULT_000,			// リザルトBGM			[000]
	SOUND_LABEL_SE_GAME_CLEAR_000,		// ゲームクリアSE		[000]
	SOUND_LABEL_SE_GAME_OVER_000,		// ゲームオーバーSE		[000]
	SOUND_LABEL_SE_JUMP_000,			// ジャンプSE			[000]
	SOUND_LABEL_SE_LANDING_000,			// 着地SE				[000]
	SOUND_LABEL_SE_DAMAGE_000,			// ダメージSE			[000]
	SOUND_LABEL_SE_SLASH_000,			// スラッシュSE			[000]
	SOUND_LABEL_SE_DETERMINATION_000,	// 決定SE				[000]
	SOUND_LABEL_SE_METAL_000,			// 金属SE				[000]
	SOUND_LABEL_MAX
}SOUND_LABEL;

//****************************************
// 構造体の定義
//****************************************

// サウンド情報の構造体定義
typedef struct
{
	char		*pFilename;	// ファイル名
	char		*pLoadName;	// 読み込み名
	int			nCntLoop;	// ループカウント
	float		fVolume;	// 音量
	SOUND_TYPE	type;		// サウンドの種類
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

// サウンドの情報を取得
SOUNDINFO *GetSoundInfo(void);

// サウンドの種類を文字列から読み込む
void StringLoadSound(char aString[TXT_MAX], SOUND_LABEL *pLabel);

// サウンドの初期化処理
HRESULT InitSound(HWND hWnd);

// サウンドの終了処理
void UninitSound(void);

// セグメント再生(再生中なら停止)
HRESULT PlaySound(SOUND_LABEL label);

// セグメント停止(ラベル指定)
void StopSound(SOUND_LABEL label);

// セグメント停止(全て)
void StopSound(void);

// 種類毎に音量変更
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);

// 設定音量を設定
void SetSettingVolume(SOUND_TYPE type);

// 種類毎に再生停止
void StopSoundType(SOUND_TYPE type);

#endif

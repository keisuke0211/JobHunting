//========================================
// 
// サウンドのヘッダファイル
// Author : AKIRA TANAKA
// |
// Arrange : RIKU NISHIMURA
//			 石原颯馬
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
	SOUND_TYPE_BGM,	// 音楽
	SOUND_TYPE_SE,	// 効果音
	SOUND_TYPE_MAX,
}SOUND_TYPE;

//****************************************
// 構造体の定義
//****************************************
// サウンド情報の構造体定義
typedef struct
{
	char		aFileName	// ファイル名
				[TXT_MAX];
	int			nCntLoop;	// ループカウント
	float		fVolume;	// 音量
	SOUND_TYPE	type;		// サウンドの種類
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
// サウンドの情報を取得
SOUNDINFO *GetSoundInfo(void);

// サウンドの読み込み処理
void LoadSound(void);

// サウンドの初期化処理
// サウンドの終了処理
HRESULT InitSound(HWND hWnd);
void UninitSound(void);

// セグメント再生(再生中なら停止)
// セグメント停止(ラベル指定)
// セグメント停止(全て)
HRESULT PlaySound(int nSoundIdx);
void StopSound(int nSoundIdx);
void StopSound(void);

// 設定音量を設定
void SetSettingVolume(SOUND_TYPE type);

// 種類毎に音量変更
// 種類毎に再生停止
void SoundTypeVolumeChange(SOUND_TYPE type, float fVolume);
void StopSoundType(SOUND_TYPE type);

//種類枚毎に一時停止・再開
void PauseSound(int nSoundIdx);
void RestartSound(int nSoundIdx);

#endif

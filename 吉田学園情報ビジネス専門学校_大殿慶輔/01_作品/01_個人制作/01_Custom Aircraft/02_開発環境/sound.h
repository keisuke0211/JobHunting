//=============================================================================
//
// サウンド処理 [sound.h]
// Author :大殿慶輔
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM0・オープニング
	SOUND_LABEL_BGM001,			// BGM1・ステージ１
	SOUND_LABEL_BGM002,			// BGM2・ステージ２
	SOUND_LABEL_BGM003,			// BGM3・エンディング（CLEAR）
	SOUND_LABEL_BGM004,			// BGM4・エンディング（OVER）
	SOUND_LABEL_SE_SHOT,		// 弾発射音
	SOUND_LABEL_SE_HIT,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION,	// 爆発音
	SOUND_LABEL_SE_START,		//ゲームスタート
	SOUND_LABEL_SE_CHARG,		//チャージ
	SOUND_LABEL_SE_SUMMON,		//召喚
	SOUND_LABEL_MAX,
}SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif

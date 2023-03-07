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
	SOUND_LABEL_BGM001,			// BGM1・外
	SOUND_LABEL_BGM002,			// BGM2・洞窟内
	SOUND_LABEL_BGM003,			// BGM3・エンディング（CLEAR）
	SOUND_LABEL_BGM004,			// BGM4・エンディング（OVER）
	SOUND_LABEL_SHOT,			//弾の発射音
	SOUND_LABEL_DAMAGE,			//敵のダメージ音
	SOUND_LABEL_BREAK,			//壁が壊れる音
	SOUND_LABEL_JEWEL,			//クリスタルの獲得音
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

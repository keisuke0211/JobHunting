//========================================
//  *** eff_light_00.h ***
//========================================
#ifndef _EFF_LIGHT_00_H_	// このマクロ定義がされていなかった時
#define _EFF_LIGHT_00_H_	// 二重インクルード防止のマクロを定義する

//========== 列挙型の定義		==========// 

// ライト[00]の状態
typedef enum
{
	EFF_LIGHT_00_STATE_NORMAL = 0,	// 通常
	EFF_LIGHT_00_STATE_MAX
}EFF_LIGHT_00_STATE;

// ライトの種類
typedef enum 
{
	EFF_LIGHT_00_TYPE_00 = 0,
	EFF_LIGHT_00_TYPE_01,
	EFF_LIGHT_00_TYPE_MAX
}EFF_LIGHT_00_TYPE;

//========== プロトタイプ宣言	==========// 
void InitEff_light_00(void);																													// ライト[00] の初期化処理
void UninitEff_light_00(void);																													// ライト[00] の終了処理
void UpdateEff_light_00(void);																													// ライト[00] の更新処理
void DrawEff_light_00(void);																													// ライト[00] の描画処理
void SetEff_light_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fMove, Color col, float fWidth, float fHeight, int nLife, EFF_LIGHT_00_TYPE type);	// ライト[00] の設定処理

#endif
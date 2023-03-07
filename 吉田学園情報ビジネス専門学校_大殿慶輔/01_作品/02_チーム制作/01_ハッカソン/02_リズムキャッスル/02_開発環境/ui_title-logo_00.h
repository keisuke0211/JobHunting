//========================================
// 
// UI:タイトルロゴ[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** ui_title-logo_00.h ***
//========================================
#ifndef _UI_TITLELOGO_00_H_	// このマクロ定義がされていなかった時
#define _UI_TITLELOGO_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// 構造体の定義
//****************************************
// UI:タイトルロゴ[00] の情報
typedef struct
{
	// 読み込み情報
	char		aTexturePath	// テクスチャの相対パス
				[TXT_MAX];
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	D3DXVECTOR3	startPos;		// 開始位置
	D3DXVECTOR3	endPos;			// 終了位置
	int			nArrivalTime;	// 到着にかかる時間

	// 状態関連
	int nCounterArrival;	// 到着カウンター
}Ui_titleLogo_00;

//****************************************
// プロトタイプ宣言
//****************************************
// UI:タイトルロゴ[00] の読み込み処理
void LoadUi_titleLogo_00(void);

// UI:タイトルロゴ[00] の情報を取得
Ui_titleLogo_00 *GetUi_titleLogo_00(void);

// UI:タイトルロゴ[00] の初期化処理
// UI:タイトルロゴ[00] の終了処理
// UI:タイトルロゴ[00] の更新処理
// UI:タイトルロゴ[00] の描画処理
void InitUi_titleLogo_00(void);
void UninitUi_titleLogo_00(void);
void UpdateUi_titleLogo_00(void);
void DrawUi_titleLogo_00(void);

#endif
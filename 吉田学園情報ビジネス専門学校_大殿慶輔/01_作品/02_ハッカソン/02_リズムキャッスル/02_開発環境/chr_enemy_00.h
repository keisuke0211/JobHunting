//========================================
// 
// CHR:敵[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** chr_enemy_00.h ***
//========================================
#ifndef _CHR_ENEMY_00_H_	// このマクロ定義がされていなかった時
#define _CHR_ENEMY_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// 列挙型の定義
//****************************************
// CHR:敵[00] の属性情報
typedef enum 
{
	CHR_ENEMY_00_ELEM_A,
	CHR_ENEMY_00_ELEM_B,
	CHR_ENEMY_00_ELEM_X,
	CHR_ENEMY_00_ELEM_Y,
}CHR_ENEMY_00_ELEM;

//****************************************
// 構造体定義
//****************************************
// CHR:敵[00] の情報
typedef struct
{
	int		nType;			// 種類

	D3DXVECTOR3 pos;		// 位置
	bool	bUse;			// 使用されているかフラグ
	int		nCounterMove;	// 移動カウンター
	int		nPtn;			// パターンNo.
	int		nCounterAnim;	// アニメーションカウンター
}Chr_enemy_00;

// CHR:敵[00] の種類毎の情報
typedef struct
{
	char				aTexturePath	// テクスチャの相対パス
						[TXT_MAX];
	float				fWidth;			// 幅
	float				fHeight;		// 高さ
	int					nPtnWidth;		// パターン幅
	int					nPtnHeight;		// パターン高さ
	int					nMoveTime;		// 移動にかかる時間
	CHR_ENEMY_00_ELEM	elem;			// 属性
}Chr_enemy_00Type;

//****************************************
// マクロ定義
//****************************************
//敵の最大数
#define CHR_ENEMY_00_MAX	(128)

//****************************************
// プロトタイプ宣言
//****************************************
// CHR:敵[00] の情報の取得
// CHR:敵[00] の種類毎の情報の取得
Chr_enemy_00 *GetChr_enemy_00(void);
Chr_enemy_00Type *GetChr_enemy_00Type(void);

void LoadChr_enemy_00(void);

// CHR:敵[00] の初期化処理
// CHR:敵[00] の終了処理
// CHR:敵[00] の更新処理
// CHR:敵[00] の描画処理
void InitChr_enemy_00(void);
void UninitChr_enemy_00(void);
void UpdateChr_enemy_00(void);
void DrawChr_enemy_00(void);

// CHR:敵[00] の設定処理
void SetChr_enemy_00(int nType);

#endif
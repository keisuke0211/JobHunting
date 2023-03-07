//========================================
// 
// OBJ:城[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** obj_castle_00.h ***
//========================================
#ifndef _OBJ_CASTLE_00_H_	// このマクロ定義がされていなかった時
#define _OBJ_CASTLE_00_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"

//****************************************
// 構造体の定義
//****************************************
// OBJ:城[00] の情報
typedef struct
{
	// 状態関連
	int nCounterDamage;	// ダメージカウンター
}Obj_castle_00;

//****************************************
// プロトタイプ宣言
//****************************************
// OBJ:城[00] の情報を取得
Obj_castle_00 *GetObj_castle_00(void);

// OBJ:城[00] の初期化処理
// OBJ:城[00] の終了処理
// OBJ:城[00] の更新処理
// OBJ:城[00] の描画処理
void InitObj_castle_00(void);
void UninitObj_castle_00(void);
void UpdateObj_castle_00(void);
void DrawObj_castle_00(void);

void SetDamageObj_castle_00(void);

#endif
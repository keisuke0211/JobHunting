//========================================
// 
// SYS:ランキング[00] のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** sys_ranking_00.h ***
//========================================
#ifndef _SYS_RANKING_00_H_	// このマクロ定義がされていなかった時
#define _SYS_RANKING_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
// ランキングの順位数
#define RANKING_NUM	(8)

//****************************************
// 構造体の定義
//****************************************
// ランキング構造体
typedef struct
{
	int	nScore;	// スコア
}Sys_ranking_00;

//****************************************
// プロトタイプ宣言
//****************************************
// 順位の表示形式を取得
char *GetRankText(int nRank);

// SYS:ランキング[00] の情報を取得
Sys_ranking_00 *GetSys_ranking_00(void);

// SYS:ランキング[00] の初期化処理
// SYS:ランキング[00] の終了処理
// SYS:ランキング[00] の更新処理
void InitSys_ranking_00(void);
void UninitSys_ranking_00(void);
void UpdateSys_ranking_00(void);

// SYS:ランキング[00] の読み込み処理
// SYS:ランキング[00] の書き込み処理
void LoadSys_ranking_00(void);
void SaveSys_ranking_00(void);

// ランキングにスコアを挿入する
int SetScore(int nScore);

#endif
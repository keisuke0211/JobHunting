//==========================================
//
//プレイヤー関係プログラムのヘッダ[player.h]
//Author:石原颯馬
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//マクロ
//ほげ

//判定列挙
typedef enum
{
	JUDGE_EXELENT = 0,
	JUDGE_GOOD,
	JUDGE_MISS,
	JUDGE_MAX
} JUDGE;

//プレイヤー型構造体定義
typedef struct
{
	//見た目関係
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 move;					//移動量	
	D3DXVECTOR3 rot;					//向き
	int nCounterAnim;					//アニメーションカウンタ
	int nPatternAnim;					//アニメーションパターンNo.

	//ステータス関係
	int nLife;							//体力
	int nCounterAttack;					//攻撃アニメーション開始から経過した時間
	int nScore;							//スコア
	int nCombo;							//コンボ
	int nCounterComboMoveTime;			//コンボ表示開始から経過した時間

	//使用有無
	bool bDisp;							//生きているかどうか
} Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void SetPlayer(void);

#endif // !_PLAYER_H_

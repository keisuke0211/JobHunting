//========================================
//  *** eff_particle_00.h ***
//========================================
#ifndef _EFF_PARTICLE_00_H_	// このマクロ定義がされていなかった時
#define _EFF_PARTICLE_00_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// 弾の種類
typedef enum 
{
	EFF_PARTICLE_00_TYPE_SPARK_00 = 0,	// 火花[00]
	EFF_PARTICLE_00_TYPE_SPARK_01,		// 火花[01]
	EFF_PARTICLE_00_TYPE_01,			// 弾の破片(プレイヤー)
	EFF_PARTICLE_00_TYPE_02,			// 弾の破片(敵)
	EFF_PARTICLE_00_TYPE_MAX,
	EFF_PARTICLE_00_TYPE_N1
}EFF_PARTICLE_00_TYPE;

//****************************************
// 構造体の定義
//****************************************

// 弾[00]構造体の定義
typedef struct
{
								//========== *** 位置関連 ***
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	float fMove;				// 移動力
	float fRotation;			// 回転力
	D3DXVECTOR3 rot;			// 向き
	float fWidth;				// 幅
	float fHeight;				// 高さ
	float fHitTestRadius;		// 当たり判定の半径
								//========== *** 分類情報 ***
	EFF_PARTICLE_00_TYPE type;	// 種類
	bool bUse;					// 使用されているかフラグ
								//========== *** 状態値 ***
	int nLife;					// 寿命
}Eff_particle_00;

//****************************************
// プロトタイプ宣言
//****************************************
void InitEff_particle_00	(void);															// 弾[00] の初期化処理
void UpdateEff_particle_00	(void);															// 弾[00] の更新処理
void SetEff_particle_00		(D3DXVECTOR3 pos, D3DXVECTOR3 rot, EFF_PARTICLE_00_TYPE type);	// 弾[00] の設定処理

#endif
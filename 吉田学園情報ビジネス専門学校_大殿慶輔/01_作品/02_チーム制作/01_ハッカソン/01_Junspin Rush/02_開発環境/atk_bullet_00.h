//========================================
//  *** atk_bullet_00.h ***
//========================================
#ifndef _ATK_BULLET_00_H_	// このマクロ定義がされていなかった時
#define _ATK_BULLET_00_H_	// 二重インクルード防止のマクロを定義する

#include "attack.h"
#include "polygon.h"
#include "atk_explosion_00.h"
#include "eff_particle_00.h"

//****************************************
// マクロ定義
//****************************************
#define ATK_BULLET_00_MAX		(256)	// 弾[00] の最大数
#define ATK_BULLET_00_TYPE_MAX	(80)	// 弾[00] の種類の最大

//****************************************
// 構造体の定義
//****************************************

// 弾[00]構造体の定義
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	float		fMove;			// 移動力
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 drawRot;		// +描画向き
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	float		fHitTestWidth;	// 当たり判定の幅
	float		fHitTestHeight;	// 当たり判定の高さ

	// 分類情報
	int				nType;		// 種類
	ATK_PARENT_TYPE	parentType;	// 親の種類

	// 状態値
	bool	bUse;				// 使用されているかフラグ
	int		nLife;				// 寿命
	int		nDamage;			// ダメージ
	int		nPenetrationNum;	// 残りの貫通回数
	int		nPattern;			// パターンNo.
	int		nCounterAnim;		// アニメーションのカウンター
	int		nCounterRotation;	// 回転カウンター
}Atk_bullet_00;

// 弾[00] の種類毎の情報
typedef struct
{
	// 分類情報
	char	aLoadName	// 読み込み名
			[TXT_MAX];

	// 見た目関連
	char		aTexturPath						// テクスチャの相対パス
				[ATK_PARENT_TYPE_MAX][TXT_MAX];	//
	int			nPtnMaxX;						// パターンの最大数(横)
	int			nPtnMaxY;						// パターンの最大数(縦)
	int			nAnimPtnMin;					// アニメパターンNo.の下限
	int			nAnimPtnMax;					// アニメパターンNo.の上限
	int			nAnimTime;						// アニメーションの切り替え時間
	ANIM_TYPE	animType;						// アニメーションの種類
	float		fWidth;							// 幅
	float		fHeight;						// 高さ
	ANGLE_TYPE	angleType;						// 向きの種類
	float		fRotation;						// 回転力

	// 性能関連5
	float			fHitTestWidth;	// 当たり判定の幅
	float			fHitTestHeight;	// 当たり判定の高さ
	bool			bHitAlly;		// 味方にも当たるかフラグ
	float			fMove;			// 移動力
	int				nLife;			// 寿命
	int				nPenetration;	// 貫通回数
	int				nDamage;		// ダメージ

	// 演出関連
	SOUND_LABEL				soundSet;				// 設定時のサウンド
	ATK_EXPLOSION_00_TYPE	exType;					// 爆発の種類
	bool					bRotation;				// 回転フラグ
	Color					aLightColor				// ライトの色
							[ATK_PARENT_TYPE_MAX];	// 
	float					fLightWidth;			// ライトの幅
	float					fLightHeight;			// ライトの高さ
	int						nLightLife;				// ライトの寿命
	float					fLightMove;				// ライトの移動量
	EFF_PARTICLE_00_TYPE	aPtclType				// パーティクルの種類
							[ATK_PARENT_TYPE_MAX];	//
	int						nPtclNum;				// パーティクルの発生数
}Atk_bullet_00Type;

//****************************************
// プロトタイプ宣言
//****************************************

// 弾[00] の情報を取得
Atk_bullet_00 *GetAtk_bullet_00(void);

// 弾[00] の種類毎の情報を取得
Atk_bullet_00Type *GetAtk_bullet_00Type(void);

// 弾[00] の読み込み処理
void LoadAtk_bullet_00(void);

// 弾[00] の種類を文字列から読み込む
void StringLoadAtk_bullet_00(char aString[TXT_MAX], int *pType);

// 弾[00] の初期化処理
void InitAtk_bullet_00(void);

// 弾[00] の終了処理
void UninitAtk_bullet_00(void);

// 弾[00] の更新処理
void UpdateAtk_bullet_00(void);

// 弾[00] の描画処理
void DrawAtk_bullet_00(void);

// 弾[00] の設定処理
void SetAtk_bullet_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, ATK_PARENT_TYPE lookType);

// 弾[00] の破壊処理
void DestructionAtk_bullet_00(Atk_bullet_00 *pAtk);

// 弾[00] の全破壊処理
void AllDestructionAtk_bullet_00(ATK_PARENT_TYPE parentType);

#endif
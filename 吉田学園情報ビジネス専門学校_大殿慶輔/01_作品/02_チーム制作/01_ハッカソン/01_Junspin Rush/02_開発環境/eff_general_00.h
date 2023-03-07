//========================================
//  *** eff_general_00.h ***
//========================================
#ifndef _EFF_GENERAL_00_H_	// このマクロ定義がされていなかった時
#define _EFF_GENERAL_00_H_	// 二重インクルード防止のマクロを定義する

#include "polygon.h"
#include "sound.h"

//****************************************
// マクロ定義
//****************************************
#define EFF_GENERAL_00_TYPE_MAX				(80)	// エフェクト全般[00] の種類の最大数
#define EFF_GENERAL_00_MAX					(256)	// エフェクト全般[00] の最大数
#define EFF_GENERAL_00_SOUND_MEMORY_MAX		(128)	// エフェクト全般[00] のサウンド情報の最大記憶数
#define EFF_GENERAL_00_SOUND_PATTERN_MAX	(128)	// エフェクト全般[00] のパターンNo.情報の最大記憶数
#define EFF_GENERAL_00_SUMMON_MEMORY_MAX	(128)	// エフェクト全般[00] の召喚情報の最大記憶数

//****************************************
// 構造体の定義
//****************************************

// エフェクト全般[00] のサウンド情報構造体
typedef struct
{
	int			nTime;		// サウンドを鳴らす時間
	SOUND_LABEL	soundLabel;	// 鳴らすサウンドの種類
}Eff_general_00Sound;

// エフェクト全般[00] のパターンNo.情報構造体
typedef struct
{
	int	nTime;		// パターンNo.を変える時間
	int	nPattern;	// パターンNo.
}Eff_general_00Pattern;

// エフェクト全般[00] の召喚情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 召喚する位置
	float		fAngle;		// 召喚する角度
	int			nTime;		// 召喚する時間
	int			nEnemyType;	// 召喚するエフェクト全般の種類
	int			nParentNum;	// 召喚するエフェクト全般の親番号
	int			nChildNum;	// 召喚するエフェクト全般の子供番号
}Eff_general_00Summon;

// エフェクト全般[00] 構造体の定義
typedef struct
{
	// 位置関連
	D3DXVECTOR3	pos;			// 位置
	D3DXVECTOR3	rot;			// 向き
	D3DXVECTOR3	targetRot;		// 目標向き
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	D3DXVECTOR3	*pParentPos;	// 親の位置のポインタ
	D3DXVECTOR3	parentPosTemp;	// 親の位置を保存
	D3DXVECTOR3	*pParentRot;	// 親の向きのポインタ
	D3DXVECTOR3	parentRotTemp;	// 親の向きを保存
	
	// 分類情報
	int	nType;	// 種類
	
	// 状態値
	int		nLife;				// 寿命
	bool	bUse;				// 使用されているかフラグ
	bool	bChild;				// 子フラグ
	int		nPattern;			// テクスチャのパターンNo.
	int		nCounterAnim;		// アニメーションのカウンター
	int		nCounterSound;		// サウンドカウンター
	int		nCounterPattern;	// パターンNo.カウンター
	int		nCounterSummon;		// 召喚カウンター
	bool	bDraw;				// 描画フラグ
}Eff_general_00;

// エフェクト全般[00] の種類毎の情報
typedef struct
{
	// 分類情報
	char	aLoadName	// 読み込み名
			[TXT_MAX];	//

	// 見た目関連
	char		aTexturPath		// テクスチャの相対パス
				[TXT_MAX];		//
	int			nPtnMaxX;		// パターンの最大数(横)
	int			nPtnMaxY;		// パターンの最大数(縦)
	int			nAnimPtnMin;	// アニメパターンNo.の下限
	int			nAnimPtnMax;	// アニメパターンNo.の上限
	int			nAnimTime;		// アニメーションにかかる時間
	ANIM_TYPE	animType;		// アニメーションの種類
	float		fWidth;			// 幅
	float		fHeight;		// 高さ
	ANGLE_TYPE	angleType;		// 向きの種類

	// 性能関連
	int						nLife;								// 寿命
	int						nSoundNum;							// サウンドの数
	Eff_general_00Sound		aSound								//
							[EFF_GENERAL_00_SOUND_MEMORY_MAX];	// サウンドの情報
	int						nSoundLoopTime;						// サウンドのループ時間
	int						nPatternNum;						// パターンNo.の数
	Eff_general_00Pattern	aPattern							//
							[EFF_GENERAL_00_SOUND_PATTERN_MAX];	// パターンNo.の情報
	int						nPatternLoopTime;					// パターンNo.のループ時間
	int						nSummonNum;							// 召喚の数
	Eff_general_00Summon	aSummon								//
							[EFF_GENERAL_00_SUMMON_MEMORY_MAX];	// 召喚の情報
	int						nSummonLoopTime;					// 召喚のループ時間
}Eff_general_00Type;

//****************************************
// プロトタイプ宣言
//****************************************

// エフェクト全般[00] の読み込み処理
void LoadEff_general_00(void);

// エフェクト全般[00] の種類を文字列から読み込む
void StringLoadEff_general_00Type(char aString[TXT_MAX], int *pType);

// エフェクト全般[00] の初期化処理
void InitEff_general_00(void);

// エフェクト全般[00] の終了処理
void UninitEff_general_00(void);

// エフェクト全般[00] の更新処理
void UpdateEff_general_00(void);

// エフェクト全般[00] の描画処理
void DrawEff_general_00(void);

// エフェクト全般[00] の設定処理
void SetEff_general_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bParent, D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pParentRot);

#endif
//========================================
//  *** chr_enemy_00.h ***
//========================================
#ifndef _CHR_ENEMY_00_H_	// このマクロ定義がされていなかった時
#define _CHR_ENEMY_00_H_	// 二重インクルード防止のマクロを定義する

#include "polygon.h"
#include "sound.h"
#include "atk_explosion_00.h"

//****************************************
// マクロ定義
//****************************************
// 敵[00] の種類の最大数
// 敵[00] の最大数
// 敵[00] の行動情報の最大記憶数
// 敵[00] のサウンド情報の最大記憶数
// 敵[00] のパターンNo.情報の最大記憶数
// 敵[00] のエフェクト情報の最大記憶数
// 敵[00] の発射情報の最大記憶数
// 敵[00] の召喚情報の最大記憶数
#define CHR_ENEMY_00_TYPE_MAX			(80)
#define CHR_ENEMY_00_MAX				(256)
#define CHR_ENEMY_00_ACT_MEMORY_MAX		(128)
#define CHR_ENEMY_00_SOUND_MEMORY_MAX	(128)
#define CHR_ENEMY_00_PATTERN_MEMORY_MAX	(128)
#define CHR_ENEMY_00_EFFECT_MEMORY_MAX	(128)
#define CHR_ENEMY_00_SHOT_MEMORY_MAX	(128)
#define CHR_ENEMY_00_SUMMON_MEMORY_MAX	(128)

// 敵[00] の膨らみ待ちにかかる時間の(爆心地からの距離→)変換倍率
// 敵[00] の膨らみ待ちにかかる時間の(親からの距離→)変換倍率
#define CHR_ENEMY_00_SWELL_WAIT_TIME_DIAMETER_EXPLOSION	(0.1f)
#define CHR_ENEMY_00_SWELL_WAIT_TIME_DIAMETER_PARENT	(0.2f)

//****************************************
// 列挙型の定義
//****************************************

// 敵[00] の状態
typedef enum
{
	CHR_ENEMY_00_STATE_NORMAL = 0,				// 通常
	CHR_ENEMY_00_STATE_IN_POP,					// 出現中
	CHR_ENEMY_00_STATE_IN_CLEAR,				// 消え中
	CHR_ENEMY_00_STATE_DAMAGE,					// ダメージ
	CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE,	// 爆発耐性
	CHR_ENEMY_00_STATE_SWELL,					// 膨らみ
	CHR_ENEMY_00_STATE_SWELL_WAIT,				// 膨らみ待ち
	CHR_ENEMY_00_STATE_MAX
}CHR_ENEMY_00_STATE;

// 敵[00] の行動の読み込みの種類
typedef enum
{
	CHR_ENEMY_00_ACT_LOAD_PROGRESS_START = 0 ,			// 前進開始
	CHR_ENEMY_00_ACT_LOAD_PROGRESS_END,					// 前進終了
	CHR_ENEMY_00_ACT_LOAD_STRONG_PROGRESS_START,		// 強前進開始
	CHR_ENEMY_00_ACT_LOAD_STRONG_PROGRESS_END,			// 強前進終了
	CHR_ENEMY_00_ACT_LOAD_LIGHT_PROGRESS_START,			// 弱前進開始
	CHR_ENEMY_00_ACT_LOAD_LIGHT_PROGRESS_END,			// 弱前進終了
	CHR_ENEMY_00_ACT_LOAD_BACKWARD_START,				// 後進開始
	CHR_ENEMY_00_ACT_LOAD_BACKWARD_END,					// 後進終了
	CHR_ENEMY_00_ACT_LOAD_STRONG_BACKWARD_START,		// 強後進開始
	CHR_ENEMY_00_ACT_LOAD_STRONG_BACKWARD_END,			// 強後進終了
	CHR_ENEMY_00_ACT_LOAD_LIGHT_BACKWARD_START,			// 弱後進開始
	CHR_ENEMY_00_ACT_LOAD_LIGHT_BACKWARD_END,			// 弱後進終了
	CHR_ENEMY_00_ACT_LOAD_DRIVING,						// 推進
	CHR_ENEMY_00_ACT_LOAD_SLIDE_LEFT_START,				// 左スライド開始
	CHR_ENEMY_00_ACT_LOAD_SLIDE_LEFT_END,				// 左スライド終了
	CHR_ENEMY_00_ACT_LOAD_SLIDE_RIGHT_START,			// 右スライド開始
	CHR_ENEMY_00_ACT_LOAD_SLIDE_RIGHT_END,				// 右スライド終了
	CHR_ENEMY_00_ACT_LOAD_TURN_AROUND_TO_PLAYER_START,	// プレイヤー方向転換開始
	CHR_ENEMY_00_ACT_LOAD_TURN_AROUND_TO_PLAYER_END,	// プレイヤー方向転換終了
	CHR_ENEMY_00_ACT_LOAD_TRACKING_PLAYER_START,		// プレイヤー追尾開始
	CHR_ENEMY_00_ACT_LOAD_TRACKING_PLAYER_END,			// プレイヤー追尾終了
	CHR_ENEMY_00_ACT_LOAD_STARE_PLAYER_START,			// プレイヤー凝視開始
	CHR_ENEMY_00_ACT_LOAD_STARE_PLAYER_END,				// プレイヤー凝視終了
	CHR_ENEMY_00_ACT_LOAD_STARE_CENTER_START,			// 中心凝視開始
	CHR_ENEMY_00_ACT_LOAD_STARE_CENTER_END,				// 中心凝視終了
	CHR_ENEMY_00_ACT_LOAD_GHOST_START,					// すり抜け開始
	CHR_ENEMY_00_ACT_LOAD_GHOST_END,					// すり抜け終了
	CHR_ENEMY_00_ACT_LOAD_BULLET_GHOST_START,			// 弾すり抜け開始
	CHR_ENEMY_00_ACT_LOAD_BULLET_GHOST_END,				// 弾すり抜け終了
	CHR_ENEMY_00_ACT_LOAD_INVINCIBLE_START,				// 無敵開始
	CHR_ENEMY_00_ACT_LOAD_INVINCIBLE_END,				// 無敵終了
	CHR_ENEMY_00_ACT_LOAD_KNOCK_BACK_START,				// ノックバック開始
	CHR_ENEMY_00_ACT_LOAD_KNOCK_BACK_END,				// ノックバック終了
	CHR_ENEMY_00_ACT_LOAD_SCALE_DOWN_START,				// 縮小開始
	CHR_ENEMY_00_ACT_LOAD_SCALE_DOWN_END,				// 縮小終了
	CHR_ENEMY_00_ACT_LOAD_SUMMONING_ON_DIED,			// 死亡時召喚
	CHR_ENEMY_00_ACT_LOAD_CHILD_RELEASE,				// 親離れ
	CHR_ENEMY_00_ACT_LOAD_IMMEDIATE_POP,				// 即出現
	CHR_ENEMY_00_ACT_LOAD_IMMEDIATE_CLEAR,				// 即消える
	CHR_ENEMY_00_ACT_LOAD_CLEAR,						// 消える
	CHR_ENEMY_00_ACT_LOAD_LOOP,							// ループ
	CHR_ENEMY_00_ACT_LOAD_MAX
}CHR_ENEMY_00_ACT_LOAD;

// 敵[00] の行動の種類
typedef enum
{
	CHR_ENEMY_00_ACT_PROGRESS = 0,		// 前進
	CHR_ENEMY_00_ACT_STRONG_PROGRESS,	// 強前進
	CHR_ENEMY_00_ACT_LIGHT_PROGRESS,	// 弱前進
	CHR_ENEMY_00_ACT_BACKWARD,			// 後進
	CHR_ENEMY_00_ACT_STRONG_BACKWARD,	// 強後進
	CHR_ENEMY_00_ACT_LIGHT_BACKWARD,	// 弱後進
	CHR_ENEMY_00_ACT_SLIDE_LEFT,		// 左スライド
	CHR_ENEMY_00_ACT_SLIDE_RIGHT,		// 右スライド
	CHR_ENEMY_00_ACT_TURN_AROUND,		// 方向転換
	CHR_ENEMY_00_ACT_TRACKING,			// 追尾
	CHR_ENEMY_00_ACT_STARE,				// 凝視
	CHR_ENEMY_00_ACT_GHOST,				// すり抜け
	CHR_ENEMY_00_ACT_BULLET_GHOST,		// 弾すり抜け
	CHR_ENEMY_00_ACT_INVINCIBLE,		// 無敵
	CHR_ENEMY_00_ACT_KNOCK_BACK,		// ノックバック
	CHR_ENEMY_00_ACT_SCALE_DOWN,		// 縮小
	CHR_ENEMY_00_ACT_SUMMONING_ON_DIED,	// 死亡時召喚
	CHR_ENEMY_00_ACT_MAX
}CHR_ENEMY_00_ACT;

//****************************************
// 構造体の定義
//****************************************

// 敵[00] の行動情報構造体
typedef struct
{
	int						nTime;	// 行動する時間
	CHR_ENEMY_00_ACT_LOAD	type;	// 行動の種類
}Chr_enemy_00Act;

// 敵[00] のサウンド情報構造体
typedef struct
{
	int			nTime;		// サウンドを鳴らす時間
	SOUND_LABEL	soundLabel;	// 鳴らすサウンドの種類
}Chr_enemy_00Sound;

// 敵[00] のパターンNo.情報構造体
typedef struct
{
	int	nTime;		// パターンNo.を変える時間
	int	nPattern;	// パターンNo.
}Chr_enemy_00Pattern;

typedef struct 
{
	D3DXVECTOR3 pos;			// 配置する位置
	float		fAngle;			// 配置する角度
	int			nTime;			// 配置する時間
	int			nEffectType;	// 配置するエフェクトの種類
	bool		bChild;			// 子フラグ
}Chr_enemy_00Effect;

// 敵[00] の発射情報構造体
typedef struct
{
	D3DXVECTOR3 pos;			// 発射する位置
	float		fAngle;			// 発射する角度
	int			nTime;			// 発射する時間
	int			nBulletType;	// 発射する弾の種類
}Chr_enemy_00Shot;

// 敵[00] の召喚情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 召喚する位置
	float		fAngle;		// 召喚する角度
	int			nTime;		// 召喚する時間
	int			nEnemyType;	// 召喚する敵の種類
	int			nParentNum;	// 召喚する敵の親番号
	int			nChildNum;	// 召喚する敵の子供番号
}Chr_enemy_00Summon;

// 敵[00] 構造体の定義
typedef struct
{
	// 位置関連
	D3DXVECTOR3		pos;				// 位置
	D3DXVECTOR3		move;				// 移動量
	float			fMove;				// 移動力
	float			fRotation;			// 回転力
	D3DXVECTOR3		rot;				// 向き
	D3DXVECTOR3		targetRot;			// 目標向き
	float			fWidth;				// 幅
	float			fHeight;			// 高さ
	float			fHitTestWidth;		// 当たり判定の幅
	float			fHitTestHeight;		// 当たり判定の高さ
	ScaleDiameter	scale;				// 大きさの倍率
	D3DXVECTOR3		*pTurnAroundPos;	// 方向転換位置
	D3DXVECTOR3		*pTrackingPos;		// 追尾位置
	D3DXVECTOR3		*pStarePos;			// 凝視位置
	
	// 分類情報
	int		nType;		// 種類
	int		nParentNum;	// 親番号
	int		nChildNum;	// 子供番号
	
	// 状態値
	bool	bUse;						// 使用されているかフラグ
	bool	bBomb;						// 爆弾所持フラグ
	bool	bReceivedExplosion;			// 爆発を受けたフラグ
	int		nSwellWaitTime;				// 膨らみ待ちにかかる時間
	int		nLife;						// 体力
	bool	aState						// 状態フラグ
			[CHR_ENEMY_00_STATE_MAX];	//
	int		aCounterState				// 状態管理カウンター
			[CHR_ENEMY_00_STATE_MAX];	//
	int		nPattern;					// テクスチャのパターンNo.
	int		nCounterAnim;				// アニメーションのカウンター
	int		nCounterAct;				// 行動カウンター
	bool	aAct						// 行動フラグ
			[CHR_ENEMY_00_ACT_MAX];		//
	int		nCounterSound;				// サウンドカウンター
	int		nCounterPattern;			// パターンNo.カウンター
	int		nCounterEffect;				// エフェクトカウンター
	int		nCounterShot;				// 発射カウンター
	int		nCounterSummon;				// 召喚カウンター
	int		nCntChain;					// 連鎖のカウント
}Chr_enemy_00;

// 敵[00] の種類毎の情報
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
	float					fHitTestWidth;						// 当たり判定の幅
	float					fHitTestHeight;						// 当たり判定の高さ
	int						nLife;								// 体力
	float					fMove;								// 移動力
	float					fRotation;							// 回転力
	int						nHitDamage;							// 衝突ダメージ
	bool					bHitBreak;							// 衝突時に破壊するか
	ATK_EXPLOSION_00_TYPE	explosionType;						// 爆発タイプ
	int						nScore;								// スコア
	int						nActNum;							// 行動の数
	Chr_enemy_00Act			aAct								// 
							[CHR_ENEMY_00_ACT_MEMORY_MAX];		// 行動の情報
	int						nSoundNum;							// サウンドの数
	Chr_enemy_00Sound		aSound								//
							[CHR_ENEMY_00_SOUND_MEMORY_MAX];	// サウンドの情報
	int						nSoundLoopTime;						// サウンドのループ時間
	int						nPatternNum;						// パターンNo.の数
	Chr_enemy_00Pattern		aPattern							//
							[CHR_ENEMY_00_PATTERN_MEMORY_MAX];	// パターンNo.の情報
	int						nPatternLoopTime;					// パターンNo.のループ時間
	int						nEffectNum;							// エフェクトの数
	Chr_enemy_00Effect		aEffect								//
							[CHR_ENEMY_00_EFFECT_MEMORY_MAX];	// エフェクトの情報
	int						nEffectLoopTime;					// エフェクトのループ時間
	int						nShotNum;							// 発射の数
	Chr_enemy_00Shot		aShot								//
							[CHR_ENEMY_00_SHOT_MEMORY_MAX];		// 発射の情報
	int						nShotLoopTime;						// 発射のループ時間
	int						nSummonNum;							// 召喚の数
	Chr_enemy_00Summon		aSummon								//
							[CHR_ENEMY_00_SUMMON_MEMORY_MAX];	// 召喚の情報
	int						nSummonLoopTime;					// 召喚のループ時間

	// 演出関連
	bool		bBonusSound;	// ボーナスサウンドフラグ
	SOUND_LABEL	damageSound;	// ダメージ時のサウンドの種類
}Chr_enemy_00Type;

//****************************************
// プロトタイプ宣言
//****************************************

// 敵[00] の情報を取得
Chr_enemy_00 *GetChr_enemy_00(void);

// 敵[00] の種類毎の情報を取得
Chr_enemy_00Type *GetChr_enemy_00Type(void);

// 敵[00] の読み込み処理
void LoadChr_enemy_00(void);

// 敵[00] の種類を文字列から読み込む
void StringLoadChr_enemy_00Type(char aString[TXT_MAX], int *pType);

// 敵[00] の初期化処理
void InitChr_enemy_00(void);

// 敵[00] の終了処理
void UninitChr_enemy_00(void);

// 敵[00] の更新処理
void UpdateChr_enemy_00(void);

// 敵[00] の描画処理
void DrawChr_enemy_00(void);

// 敵[00] の設定処理
void SetChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bAreaBoss, bool bBomb, int nParentNum, int nChildNum);

// 敵[00] のヒット処理
void HitChr_enemy_00(Chr_enemy_00 *pChr, int nDamage);

// 爆発[00] 用の敵[00] のヒット処理
bool HitChr_enemy_00ForAtk_explosion_00(Chr_enemy_00 *pChr, int nDamage, int nCntChain);

// 敵[00] の破壊処理
void DestructionChr_enemy_00(Chr_enemy_00 *pChr);

// 敵[00] を全て消し去る
void RemoveAllChr_enemy_00(void);

#endif
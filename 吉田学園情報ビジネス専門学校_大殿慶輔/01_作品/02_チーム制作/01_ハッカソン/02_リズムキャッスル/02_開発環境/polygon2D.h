//========================================
// 
// ポリゴン(2D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon2D.h ***
//========================================
#ifndef _POLYGON2D_H_	// このマクロ定義がされていなかった時
#define _POLYGON2D_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
// 部品(2D)の最大数
// モーション(2D)の最大数
// モーション(2D)のパターンNo.	のアニメーション情報の最大数
// モーション(2D)の位置			のアニメーション情報の最大数
// モーション(2D)の角度			のアニメーション情報の最大数
// モーション(2D)の色			のアニメーション情報の最大数
// モーション(2D)の光			のアニメーション情報の最大数
// モーション(2D)のサウンド		のアニメーション情報の最大数
#define PARTS_2D_MAX				(24)
#define MOTION_2D_MAX				(16)
#define MOTION_2D_PATTERN_ANIM_MAX	(8)
#define MOTION_2D_POSITION_ANIM_MAX	(8)
#define MOTION_2D_ROTATION_ANIM_MAX	(8)
#define MOTION_2D_COLOR_ANIM_MAX	(8)
#define MOTION_2D_LIGHT_ANIM_MAX	(8)
#define MOTION_2D_SOUND_ANIM_MAX	(8)
//****************************************
// 列挙型の定義
//****************************************
// 角度の種類
typedef enum
{
	ANGLE_TYPE_FREE,	// 自由
	ANGLE_TYPE_FIXED,	// 固定
	ANGLE_TYPE_MAX,
}ANGLE_TYPE;
//****************************************
// 構造体の定義
//****************************************
// ポリゴン毎の情報(2D)
typedef struct 
{
	char	aTexturePath	// テクスチャの相対パス
			[TXT_MAX];
	float	fWidth;			// 幅
	float	fHeight;		// 高さ
	int		nMax;			// 最大数
}Polygon2D;
//====================
// *** 部品関連(2D)
//====================
// 部品(2D)構造体
typedef struct 
{
	// 位置関連
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 relativePos;	// 相対位置
	D3DXVECTOR3 rot;			// 角度
	D3DXVECTOR3 relativeRot;	// 相対角度
	D3DXVECTOR3 scale			// 拡大倍率
				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// テクスチャ関連
	int		nPattern;	// パターンNo.

	// 状態関連
	Color	col;			// 色
	Color	lightColor;		// 光の色
	float	fLightLength;	// 光の長さ

	// モーション関連
	bool		bPosAnim;			// 位置アニメーションフラグ
	int			nCounterPosAnim;	// 位置アニメーションカウンター
	int			nPosAnimTime;		// 位置アニメーションにかかる時間
	D3DXVECTOR3 posOld;				// 元の位置
	D3DXVECTOR3 targetPos;			// 目標位置

	bool		bRotAnim;			// 角度アニメーションフラグ
	int			nCounterRotAnim;	// 角度アニメーションカウンター
	int			nRotAnimTime;		// 角度アニメーションにかかる時間
	D3DXVECTOR3 rotOld;				// 元の角度
	D3DXVECTOR3 targetRot;			// 目標角度
	
	bool		bColorAnim;			// 色アニメーションフラグ
	int			nCounterColorAnim;	// 色アニメーションカウンター
	int			nColorAnimTime;		// 色アニメーションにかかる時間
	Color		colOld;				// 元の色
	Color		targetCol;			// 目標色
	
	bool		bLightAnim;			// 光アニメーションフラグ
	int			nCounterLightAnim;	// 光アニメーションカウンター
	int			nLightAnimTime;		// 光アニメーションにかかる時間
	Color		lightColorOld;		// 元の光の色
	Color		targetLightColor;	// 目標光の色
	float		fLightLengthOld;	// 元の光の長さ
	float		fTargetLightLength;	// 目標の光の長さ
}Parts2D;
// 部品(2D)管理構造体
typedef struct 
{
	int		nMotion;		// モーション番号
	int		nCounterMotion;	// モーションカウンター
	Parts2D	aParts			// 部品情報
			[PARTS_2D_MAX];
}Parts2DInfo;
// 部品(2D)の種類毎の情報構造体
typedef struct 
{
	// 読み込み情報
	char		aTexturePath		// テクスチャの相対パス
				[TXT_MAX];
	float		fWidth;				// 幅
	float		fHeight;			// 高さ
	int			nPtnMaxX;			// パターン上限X
	int			nPtnMaxY;			// パターン上限Y
	int			nParent;			// 親番号
	D3DXVECTOR3	fixedRelativePos;	// 固定の相対位置
	D3DXVECTOR3 fixedRelativeRot;	// 固定の相対角度
	D3DXVECTOR3	rotRelativePos;		// 回転軸の相対位置
	ANGLE_TYPE	angleType;			// 角度の種類
	bool		bIlluminate;		// 光の影響を受けるフラグ
	Color		initColor;			// 初期カラー
	int			nInitPtn;			// 初期パターンNo.
}Parts2DType;

// 部品設定情報構造体
typedef struct 
{
	int			nPartsNum;	// 部品数
	Parts2DType	aPartsType	// 部品の種類毎の情報
				[PARTS_2D_MAX];
}Parts2DSet;
//====================
// *** モーション関連(3D)
//====================
// パターンNo.のアニメーション情報構造体
typedef struct 
{
	int	nPattern;	// パターンNo.
	int	nTime;		// 切り替え時間
}PatternAnim;
// 位置のアニメーション情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int			nMoveTime;	// 到達にかかる時間
	int			nTime;		// 切り替え時間
}PositionAnim;
// 角度のアニメーション情報構造体
typedef struct
{
	D3DXVECTOR3 rot;		// 角度
	int			nMoveTime;	// 到達にかかる時間
	int			nTime;		// 切り替え時間
}RotationAnim;
// 色のアニメーション情報構造体
typedef struct
{
	Color	col;		// 色
	int		nMoveTime;	// 到達にかかる時間
	int		nTime;		// 切り替え時間
}ColorAnim;
// 光のアニメーション情報構造体
typedef struct
{
	Color	col;		// 色
	float	fLength;	// 長さ
	int		nMoveTime;	// 到達にかかる時間
	int		nTime;		// 切り替え時間
}LightAnim;
// サウンドのアニメーション情報構造体
typedef struct
{
	int	nSound;	// サウンド番号
	int	nTime;	// 切り替え時間
}SoundAnim;
// 部品毎のモーション情報構造体
typedef struct 
{
	int				nPtnAnimNum;	// パターンNo.のアニメーション数
	PatternAnim		aPtnAnim		// パターンNo.のアニメーション情報
					[MOTION_2D_PATTERN_ANIM_MAX];
	int				nPosAnimNum;	// 位置(相対)のアニメーション数
	PositionAnim	aPosAnim		// 位置(相対)のアニメーション情報
					[MOTION_2D_POSITION_ANIM_MAX];
	int				nRotAnimNum;	// 角度(相対)のアニメーション数
	RotationAnim	aRotAnim		// 角度(相対)のアニメーション情報
					[MOTION_2D_ROTATION_ANIM_MAX];
	int				nColorAnimNum;	// 色のアニメーション数
	ColorAnim		aColorAnim		// 色のアニメーション情報
					[MOTION_2D_COLOR_ANIM_MAX];
	int				nLightAnimNum;	// 光のアニメーション数
	LightAnim		aLightAnim		// 光のアニメーション情報
					[MOTION_2D_LIGHT_ANIM_MAX];
}PartsMotion;
// モーション情報構造体
typedef struct
{
	int			nLoopTime;			// ループ時間
	bool		bLoop;				// ループするかフラグ
	PartsMotion	aPartsMotion		// 部品毎のモーション情報
				[PARTS_2D_MAX];		
	bool		bFlipHorizontal;	// 左右反転フラグ
	int			nSoundAnimNum;		// サウンドのアニメーション数
	SoundAnim	aSoundAnim			// サウンドのアニメーション情報
				[MOTION_2D_SOUND_ANIM_MAX];
}Motion;
// モーション設定情報構造体
typedef struct
{
	int		nMotionNum;	// モーション数
	Motion	aMotion		// モーションの情報
			[MOTION_2D_MAX];
}MotionSet;
//****************************************
// プロトタイプ宣言
//****************************************
// ポリゴン(2D)の必要数を返す
// 現在のカウントのポリゴン(2D)の番号を返す
// ポリゴン(2D)のテクスチャの読み込み
int GetPolygon2DNum(const Polygon2D *pPoly2D, int nPolyMax);
int GetPolygon2DIndex(const Polygon2D *pPoly2D, int nPolyMax, int nCount);
void LoadPolygon2DTexture(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 *pTextureUi_map_00, const Polygon2D *pPoly2D, int nPolyMax);

// 部品設定情報の読み込み処理
// 部品管理の初期化処理
// 部品の頂点座標の設定処理
// 部品の頂点カラーの設定処理
void LoadParts2DSet(FILE *pFile, Parts2DSet *pPartsSet);
void InitParts2DInfo(Parts2DInfo *pPartsInfo, Parts2DSet partsSet);
void SetParts2DVertexPos(VERTEX_2D *pVtxTemp, VERTEX_2D *pVtx, int nPartsNum, Parts2D *pParts, Parts2DInfo *pPartsInfo, Parts2DType *pPartsType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, bool bFlipHorizontal);
void SetParts2DVertexColor(VERTEX_2D *pVtx, Parts2D parts, bool bIlluminate);

// モーション設定情報の読み込み処理
// モーションの更新処理
// モーションの設定処理
void LoadMotionSet(FILE *pFile, MotionSet *pMotionSet);
bool UpdateMotion(int nMotion, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet);
void SetMotion(int nMotion, MotionSet motionSet, Parts2DInfo *pPartsInfo, Parts2DSet partsSet);

// 頂点座標の設定処理(2D)
// rhwの設定処理(2D)
// 頂点色の設定処理(2D)
// テクスチャ座標の設定処理(2D)
// テクスチャ座標の設定処理(2D)(切り取りver)
void SetVertexPos2D(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType);
void SetRHW2D(VERTEX_2D *pVtx);
void SetVertexColor2D(VERTEX_2D *pVtx, Color col);
void SetTexturePos2D(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);
void SetTexturePos2DCut(VERTEX_2D *pVtx, Scale cutScale, D3DXVECTOR3 pos);

// 描画位置を基準に合わせる
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos);

#endif
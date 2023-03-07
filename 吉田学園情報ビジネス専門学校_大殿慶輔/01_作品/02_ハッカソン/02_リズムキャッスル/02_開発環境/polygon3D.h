//========================================
// 
// ポリゴン(3D)関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
//  *** polygon3D.h ***
//========================================
#ifndef _POLYGON3D_H_	// このマクロ定義がされていなかった時
#define _POLYGON3D_H_	// 二重インクルード防止のマクロを定義する

#include "main.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
#define MATERIAL_3D_MAX				(10)	// マテリアル(3D)の上限
#define PARTS_3D_MAX				(8)		// 部品(3D)の最大数
#define MOTION_3D_MAX				(16)	// モーション(3D)の最大数
#define MOTION_3D_POSITION_ANIM_MAX	(8)		// モーション(3D)の位置			のアニメーション情報の最大数
#define MOTION_3D_ROTATION_ANIM_MAX	(8)		// モーション(3D)の角度			のアニメーション情報の最大数
#define MOTION_3D_SOUND_ANIM_MAX	(8)		// モーション(3D)のサウンド		のアニメーション情報の最大数

//****************************************
// 構造体の定義
//****************************************
//====================
// *** 部品関連(3D)
//====================
// 部品(3D)構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 posResult;	// 位置(結果)
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rotResult;	// 向き(結果)
	D3DXVECTOR3 rot;		// 向き

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
}Parts3D;

// 部品(3D)管理構造体
typedef struct
{
	int		nMotion;		// モーション番号
	int		nCounterMotion;	// モーションカウンター
	Parts3D	aParts			// 部品情報
			[PARTS_3D_MAX];
}Parts3DInfo;

// 部品(3D)の種類毎の情報構造体
typedef struct
{
	// 読み込み情報
	char		aModelPath			// モデルの相対パス
				[TXT_MAX];
	int			nParent;			// 親番号
	D3DXVECTOR3	fixedRelativePos;	// 固定の相対位置
	D3DXVECTOR3 fixedRelativeRot;	// 固定の相対角度
	D3DXVECTOR3	rotRelativePos;		// 回転軸の相対位置
}Parts3DType;

// 部品設定情報構造体
typedef struct
{
	int			nPartsNum;	// 部品数
	Parts3DType	aPartsType	// 部品の種類毎の情報
				[PARTS_3D_MAX];
}Parts3DSet;

//====================
// *** モーション関連(3D)
//====================
// 位置のアニメーション(3D)情報構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int			nMoveTime;	// 到達にかかる時間
	int			nTime;		// 切り替え時間
}PositionAnim3D;

// 角度のアニメーション(3D)情報構造体
typedef struct
{
	D3DXVECTOR3 rot;		// 角度
	int			nMoveTime;	// 到達にかかる時間
	int			nTime;		// 切り替え時間
}RotationAnim3D;

// サウンドのアニメーション(3D)情報構造体
typedef struct
{
	int	nSound;	// サウンド番号
	int	nTime;	// 切り替え時間
}SoundAnim3D;

// 部品毎のモーション(3D)情報構造体
typedef struct
{
	int				nPosAnimNum;	// 位置(相対)のアニメーション数
	PositionAnim3D	aPosAnim		// 位置(相対)のアニメーション情報
		[MOTION_3D_POSITION_ANIM_MAX];
	int				nRotAnimNum;	// 角度(相対)のアニメーション数
	RotationAnim3D	aRotAnim		// 角度(相対)のアニメーション情報
		[MOTION_3D_ROTATION_ANIM_MAX];
}PartsMotion3D;

// モーション(3D)情報構造体
typedef struct
{
	int				nLoopTime;		// ループ時間
	bool			bLoop;			// ループするかフラグ
	PartsMotion3D	aPartsMotion	// 部品毎のモーション情報
					[PARTS_3D_MAX];
	int				nSoundAnimNum;	// サウンドのアニメーション数
	SoundAnim3D		aSoundAnim		// サウンドのアニメーション情報
					[MOTION_3D_SOUND_ANIM_MAX];
}Motion3D;

// モーション(3D)設定情報構造体
typedef struct
{
	int			nMotionNum;	// モーション数
	Motion3D	aMotion		// モーションの情報
				[MOTION_3D_MAX];
}Motion3DSet;

//****************************************
// プロトタイプ宣言
//****************************************
// 部品設定情報(3D)の読み込み処理
// 部品(3D)の初期化処理
// 部品(3D)のトランスフォームの初期化処理
// 部品(3D)のトランスフォームの取得処理
void LoadParts3DSet(FILE *pFile, Parts3DSet *pPartsSet);
void InitParts3DInfo(Parts3DInfo *pPartsInfo);
void InitParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);
void GetParts3DTransform(Parts3DInfo *pPartsInfo, Parts3DSet *pPartsSet);

// モーション(3D)設定情報の読み込み処理
// モーション(3D)の更新処理
// モーション(3D)の設定処理
void LoadMotion3DSet(FILE *pFile, Motion3DSet *pMotionSet);
bool UpdateMotion3D(int nMotion, Motion3DSet motionSet, Parts3DInfo *pPartsInfo, Parts3DSet partsSet);
void SetMotion3D(Parts3DInfo *pPartsInfo, int nMotion);

// 頂点座標の設定処理(3D)
// 法線ベクトルの設定処理(3D)
// 頂点色の設定処理(3D)
// テクスチャ座標の設定処理(3D)
void SetVertexPos3D(VERTEX_3D *pVtx, float fWidth, float fHeight);
void SetNormalLine3D(VERTEX_3D *pVtx, D3DXVECTOR3 nor);
void SetVertexColor3D(VERTEX_3D *pVtx, Color col);
void SetTexturePos3D(VERTEX_3D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);

// マテリアルの設定
void SetMaterial(LPDIRECT3DDEVICE9 pDevice, D3DMATERIAL9 *pMat, Color col);

#endif
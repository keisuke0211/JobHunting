//========================================
// 
// カメラのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** camera.h ***
//========================================
#ifndef _CAMERA_H_	// このマクロ定義がされていなかった時
#define _CAMERA_H_	// 二重インクルード防止のマクロを定義する

#include "d3dx9.h"

//****************************************
// 列挙型の定義
//****************************************
// カメラの状態
typedef enum
{
	CAMERA_STATE_VIBRATION,	// 振動
	CAMERA_STATE_MAX,
}CAMERA_STATE;

//****************************************
// 構造体の定義
//****************************************
// カメラ(2D)の情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posOld;				// 元の位置
	D3DXVECTOR3 vibrationPos;		// 振動位置
	D3DXVECTOR3 vibrationTargetPos;	// 振動の目標位置
	D3DXVECTOR3 *pTargetPos;		// 目標位置のポインタ
	D3DXVECTOR3 targetRelativePos;	// 目標位置のポインタ

	// 状態関連
	bool		aState				// 状態フラグ
				[CAMERA_STATE_MAX];
	int			aCounterState		// 状態管理カウンター
				[CAMERA_STATE_MAX];
	float		fVibration;			// 振動の強さ
	bool		bTemp;				// フラグ保存
	bool		bMove;				// 移動フラグ
	int			nCounterMove;		// 移動カウンター
}Camera2D;

// カメラ(3D)の情報構造体
typedef struct 
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 posU;			// 上方向ベクトル
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 spin;			// 回転量
	float		fLength;		// 距離
	float		fHeight;		// 高さ
	float		fVerticalMove;	// 縦方向の移動量
	D3DXMATRIX	mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	mtxView;		// ビューマトリックス
	float		fVibration;		// 振動の強さ
	int			nCounterVib;	// 振動カウンター
	D3DXVECTOR3 vibrationPos;	// 振動位置
}Camera3D;

//****************************************
// プロトタイプ宣言
//****************************************
// カメラ(2D)の情報を取得
// カメラ(3D)の情報を取得
Camera2D *GetCamera2D(void);
Camera3D *GetCamera3D(void);

// カメラ(3D)のパラメーター初期化処理
void InitParameterCamera3D(void);

// カメラの初期化処理
// カメラの終了処理
// カメラの更新処理
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

// 描画位置をカメラ(2D)の位置に合わせる
// カメラ(2D)の振動を設定
void MatchCamera2DPosition(D3DXVECTOR3 *pDrawPos);
void SetCamera2DVibration(float fVibration);

// カメラ(3D)の設定処理
// カメラ(3D)の視点の設定処理
// カメラ(3D)の注視点の設定処理
void SetCamera3D(void);
void SetCamera3DPosV(D3DXVECTOR3 pos);
void SetCamera3DPosR(D3DXVECTOR3 pos);

// カメラ(3D)の振動を設定
void SetCamera3DVibration(float fVibration);

#endif
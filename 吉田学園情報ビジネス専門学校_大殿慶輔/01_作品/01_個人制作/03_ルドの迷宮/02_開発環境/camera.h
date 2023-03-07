//****************************************
//	camera.h
//	Author:大殿慶輔
//****************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include"main.h"

//****************************************
// 構造体の定義
//****************************************

// カメラの情報構造体
typedef struct
{
	D3DXVECTOR3 posV;				// 現在の視点
	D3DXVECTOR3 posR;				// 現在の注視点
	D3DXVECTOR3 posOldV;			// 前回の視点
	D3DXVECTOR3 posOldR;			// 前回の注視点
	D3DXVECTOR3 vecU;				// 上方向ベクトル
	D3DXMATRIX mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX mtxView;				// ビューマトリックス
	D3DXVECTOR3 rot;				// 向き
	D3DXVECTOR3 spin;				// 回転量
	D3DVIEWPORT9 viewport;			// ビューポート
									   
	float length;					// 距離
									   
	bool bUse;						// 使われているか
	bool bFog;						// 霧の有無
}Camera;
//****************************************
// マクロ定義
//****************************************
#define CAMERA_SPEED			(5)			// 移動速度（X・Z座標）
#define CAMERA_SPEED1			(2)			// 移動速度（Y座標）
#define ROT_SPEED				(0.015f)	// 回転スピード
#define MAX_CAMERA				(3)			// 最大カメラ数
#define SPIN_DAMP				(0.9f)		// 回転の減衰量
#define ROT_CURSOR	(D3DXVECTOR3(0.0005f,0.00075f,0.0f))// カメラのカーソルの回転力
//****************************************
// プロトタイプ宣言
//****************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int nldx); 
Camera *Getcamara(int nCntCamara);

#endif
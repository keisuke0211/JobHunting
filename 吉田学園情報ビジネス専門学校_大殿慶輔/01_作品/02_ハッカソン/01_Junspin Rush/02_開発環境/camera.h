//========================================
// *** camera.h ***
//========================================
#ifndef _CAMERA_H_	// このマクロ定義がされていなかった時
#define _CAMERA_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// カメラの状態
typedef enum
{
	CAMERA_STATE_VIBRATION = 0,	// 振動
	CAMERA_STATE_MAX,
}CAMERA_STATE;

//****************************************
// 構造体の定義
//****************************************

// カメラの情報構造体
typedef struct 
{
	// 位置関連
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 vibrationPos;		// 振動位置
	D3DXVECTOR3 vibrationTargetPos;	// 振動の目標位置

	// 状態関連
	bool	aState[CAMERA_STATE_MAX];			// 状態フラグ
	int		aCounterState[CAMERA_STATE_MAX];	// 状態管理カウンター
	float	fVibration;							// 振動の強さ
}Camera;

//****************************************
// プロトタイプ宣言
//****************************************

// カメラの初期化処理
void InitCamera(void);

// カメラの終了処理
void UninitCamera(void);

// カメラの更新処理
void UpdateCamera(void);

// カメラの情報を取得
Camera *GetCamera(void);

// カメラの位置に合わせる
void MatchCameraPosition(D3DXVECTOR3 *pDrawPos);

// カメラの振動を設定
void SetCameraVibration	(float fVibration);

#endif
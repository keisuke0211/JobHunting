//========================================
// 
// カメラの処理
// Author:西村 吏功
// 
//========================================
//  *** camera.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "input.h"

//****************************************
// マクロ定義
//****************************************

// カメラの振動にかかる時間
// カメラの1度の振動にかかる時間
// カメラの振動の減衰倍率
// カメラの振動をコントローラーの振動の時間に変換する際の倍率
// カメラの振動がコントローラーの振動の最大値に達する値
#define CAMERA_VIBRATION_TIME				(20)
#define CAMERA_ONCE_VIBRATION_TIME			(2)
#define CAMERA_VIBRATION_DAMPING			(0.75f)
#define CONTROLLER_VIBRATION_TIME_DIAMETER	(5)
#define CONTROLLER_VIBRATION_MAX_VIBRATION	(16.0f)

//****************************************
// プロトタイプ宣言
//****************************************
void StateProcessCamera(void);	// カメラの状態処理

//****************************************
// グローバル変数
//****************************************
Camera g_camera;	// カメラの情報

//========== *** カメラの情報を取得 ***
Camera	*GetCamera(void) 
{
	return &g_camera;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StateProcessCamera関数 - カメラの状態処理 -
//========================================
void StateProcessCamera(void)
{
	for (int nCntState = 0; nCntState < CAMERA_STATE_MAX; nCntState++)
	{
		if (g_camera.aState[nCntState])
		{// その状態が真の時、
			switch (nCntState)
			{
				//========== *** 振動 ***
			case CAMERA_STATE_VIBRATION:

				if (g_camera.aCounterState[CAMERA_STATE_VIBRATION] < CAMERA_VIBRATION_TIME)
				{// 振動カウンターが出現にかかる時間を下回っている時、
					// 振動カウンターを加算
					g_camera.aCounterState[CAMERA_STATE_VIBRATION]++;

					if ((g_camera.aCounterState[CAMERA_STATE_VIBRATION] % CAMERA_ONCE_VIBRATION_TIME) == 0)
					{// 振動カウンターが1度の振動にかかる時間に達した時、
						float	fAngle	// 角度を乱数で設定
								= -D3DX_PI + fRand(D3DX_PI * 2);

						// 振動の目標位置を設定
						g_camera.vibrationTargetPos.x = sinf(fAngle) * g_camera.fVibration;
						g_camera.vibrationTargetPos.y = cosf(fAngle) * g_camera.fVibration;

						// 振動の強さを減衰させる
						g_camera.fVibration *= CAMERA_VIBRATION_DAMPING;
					}
				}
				else
				{// 振動カウンターが出現にかかる時間に達した時、
					// 振動フラグを偽にする
					g_camera.aState[CAMERA_STATE_VIBRATION] = false;
				}

				break;
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitCamera関数 - カメラの初期化処理 -
//========================================
void InitCamera(void)
{
	
}

//========================================
// UninitCamera関数 - カメラの終了処理 -
//========================================
void UninitCamera(void)
{
	
}

//========================================
// UpdateCamera関数 - カメラの更新処理 -
//========================================
void UpdateCamera(void)
{
	// 状態処理
	StateProcessCamera();

	// 振動位置を設定
	g_camera.vibrationPos = (g_camera.vibrationPos + g_camera.vibrationTargetPos) * 0.5f;
}

//========================================
// MatchCameraPosition関数 - 描画位置をカメラに合わせる -
//========================================
void MatchCameraPosition(D3DXVECTOR3 *pDrawPos)
{
	// 描画位置をカメラに合わせる
	MatchPosition(pDrawPos, g_camera.pos);

	// 描画位置に振動位置を更新する
	*pDrawPos += g_camera.vibrationPos;
}

//========================================
// SetCameraVibration関数 - カメラの振動を設定 -
//========================================
void SetCameraVibration(float fVibration)
{
	// 状態[振動]を真にする
	g_camera.aState[CAMERA_STATE_VIBRATION] = true;
	g_camera.aCounterState[CAMERA_STATE_VIBRATION] = 0;

	// 振動の強さを設定
	g_camera.fVibration += fVibration;

	// コントローラーの振動を設定
	SetVibration(
		fVibration / CONTROLLER_VIBRATION_MAX_VIBRATION, 
		fVibration * CONTROLLER_VIBRATION_TIME_DIAMETER);
}
//◇====================◇
//｜　　　　　　　　　　｜
//｜　camera.h          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _CAMERA_H_  //このマクロ定義がされてなかったら
#define _CAMERA_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

//****************************************
// 構造体の定義
//****************************************

// カメラの情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;				// 位置
	float fWidth;					// 横の中心点
	float fHeighy;					// 縦の中心点

	bool bCamera_Y;
}Camera;


//****************************************
// プロトタイプ宣言
//****************************************

void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

// カメラの情報を取得
Camera *GetCamera(void);

// カメラの位置に合わせる
void CameraPosition(D3DXVECTOR3 *pDrawPos);

#endif
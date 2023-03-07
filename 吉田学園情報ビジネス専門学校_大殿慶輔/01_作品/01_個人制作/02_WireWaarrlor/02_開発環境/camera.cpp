//◇====================◇
//｜　　　　　　　　　　｜
//｜　 カメラの処理　　 ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"camera.h"
#include"player.h"

Camera g_camera;
//========================================
//カメラの初期化処理 -
//========================================
void InitCamera(void)
{
	Player *pPlayer = Getplayer();
	g_camera.fWidth = 0.3f;
	g_camera.fHeighy = 0.9f;
	g_camera.bCamera_Y = true;
	g_camera.pos = D3DXVECTOR3(100, SCREEN_HEIGHT - 150, 0);
}

//========================================
// カメラの終了処理 -
//========================================
void UninitCamera(void)
{

}

//========================================
//カメラの更新処理 -
//========================================
void UpdateCamera(void)
{
	Player *pPlayer = Getplayer();	//プレイヤーの情報へのポインタ

	if (pPlayer->bCamera == true)
	{
		//プレイヤーの位置をカメラの位置情報に代入する
		g_camera.pos = Getplayer()->pos;
	}
}

//========================================
// 描画位置をカメラに合わせる -
//========================================
void CameraPosition(D3DXVECTOR3 *pDrawPos)
{
	// 描画位置をカメラに合わせる
	//Position(pDrawPos, g_camera.pos);


	// 位置をプレイヤーの位置に合わせる
	pDrawPos->x = pDrawPos->x - g_camera.pos.x + (SCREEN_WIDTH * g_camera.fWidth);

	if (g_camera.bCamera_Y == true)
	{
		pDrawPos->y = pDrawPos->y - g_camera.pos.y + (SCREEN_HEIGHT * g_camera.fHeighy);
	}
}
//========================================
// カメラ情報の取得
//========================================
Camera *GetCamera(void)
{
	return &g_camera;
}
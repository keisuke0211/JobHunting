//****************************************
//	Play_model.h
//	Author:大殿慶輔
//****************************************
#ifndef _PLAYER_MODEL_H_
#define _PLAYER_MODEL_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define MODEL_SPEED			(10)	//移動速度
#define SIZE_WIDTH			(20)	//横のサイズ
#define SIZE_HEIGHT			(20)	//縦のサイズ
//****************************************
// 構造体の定義
//****************************************

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3		pos;					// 現在の位置
	D3DXVECTOR3		posOld;					// 前回の位置
	D3DXVECTOR3		move;					// 位置の移動量
	D3DXVECTOR3		moveRot;				// 向きの移動量
	D3DXVECTOR3		vecMove;				// 移動ベクトル
	D3DXVECTOR3		relativePosR;			// 注視点の相対位置
	D3DXVECTOR3		targetPosR;				// 目標の注視点の相対位置

	D3DXVECTOR3		rot;					// 向き
	D3DXMATRIX		mtxWorld;				// ワールドマトリックス
	int				nIdxShadow = -1;		// 対象の影のインデックス(番号)
	int				nIdxTorch = 1;			// 対象の影のインデックス(番号)
	int nCntAction;							// 自動移動の移動時間
	float fWidht;

	bool bAction;							// プレイヤーの操作の可能か不可能か
}Player;
//****************************************
// プロトタイプ宣言
//****************************************
void InitPlayerModel(void);
void UninitPlayerModel(void);
void UpdatPlayerModel(void);
void DrawPlayerModel(void);
Player *GetPlayer(void);

#endif
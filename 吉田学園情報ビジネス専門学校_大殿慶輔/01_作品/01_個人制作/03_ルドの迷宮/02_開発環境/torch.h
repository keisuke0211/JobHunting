//****************************************
//	Play_model.h
//	Author:大殿慶輔
//****************************************
#ifndef _TORCH_H_
#define _TORCH_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define NUM_TORCH			(100)	//灯りの最大数
//****************************************
// 構造体の定義
//****************************************

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 現在の位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	int				aColor;		//影のα値
	float			fRadius;	//半径
	bool			bSet;		// 松明の設定が出来たか
	bool			bUse;		// 使用しているかどうか
}Torch;
//****************************************
// プロトタイプ宣言
//****************************************
void InitTorch(void);
void UninitTorch(void);
void UpdatTorch(void);
void DrawTorch(void);

int SetTorch(void);
void SetPositionTorch(int nIdxTorch, D3DXVECTOR3 pos, float fRadius);
//void DeleteTorch(int nIdxTorch);
#endif
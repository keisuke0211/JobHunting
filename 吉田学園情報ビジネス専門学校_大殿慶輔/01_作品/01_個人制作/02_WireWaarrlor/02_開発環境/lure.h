//◇====================◇
//｜　　　　　　　　　　｜
//｜　lure.h            ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _LURE_H_  //このマクロ定義がされてなかったら
#define _LURE_H_  //２重インクルード防止のマクロを定義する

#include"main.h"
//****************************************
// ルアーの構造体の定義
//****************************************

typedef struct
{
	D3DXVECTOR3 pos;	//現在の位置
	D3DXVECTOR3 posOld;	//前回の位置
	D3DXVECTOR3 move;   //移動量
	D3DXVECTOR3 rot;    //向き

	int nCntReturn;		//プレイヤーの場所に戻るカウント
	int nLife;			//寿命

	float Length;
	float Angle;
	bool bUse;			//使用しているかどうか
	bool bReturn;		//ルアーを戻す
	bool bStab;			//壁に当たったかどうか
	bool bWireA;		//ワイヤアクションA　フックの所に移動する・しない
	bool bRope;			//ロープの長さ設定　固定or解除
}Lure;
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitLure(void);
void UninitLure(void);
void UpdateLure(void);
void DrawLure(void);
void SetLure(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot);
Lure *GetLure(void);
//****************************************
// マクロ定義
//****************************************
#define SIZE_LURE_WIDTH  (5.0f)
#define SIZE_LURE_HEIGHT (5.0f)

#endif
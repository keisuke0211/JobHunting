//****************************************
//	bullet.h
//	Author:大殿慶輔
//****************************************
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"
//****************************************
// マクロ定義
//****************************************
#define BULLET_SIZE_WIDTH			(10)	//横のサイズ
#define BULLET_SIZE_HEIGHT			(15)	//縦のサイズ
//****************************************
// 情報構造体
//****************************************
//ビルボードの構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;
	int nLife;
	int nIdxShadow;//対象の影のインデックス(番号)

	float fWidht;
	float fHight;

	bool bUse;
}Bullet;

//****************************************
// プロトタイプ宣言
//****************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
Bullet *GetBullet(void);

#endif 
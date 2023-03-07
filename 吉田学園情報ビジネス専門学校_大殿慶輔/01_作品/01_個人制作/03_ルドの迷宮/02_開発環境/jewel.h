//****************************************
//	figure_model.h
//	Author:大殿慶輔
//****************************************
#ifndef _JEWEL_H_
#define _JEWEL_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define NUM_JEWEL	(116)			//モデルの配置数
//****************************************
// 構造体の定義
//****************************************

//モデル（置物）の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	int nIdxModelParent;		//親モデルのインデックス

	int nLife;
	int nType;
	int nCntDamage;
	int	nIdxShadow = -1;		//対象の影のインデックス(番号)

	float fHight;
	float fWidth;
	float r, g, b, a;

	bool bUse;
}Jewel;
//****************************************
// プロトタイプ宣言
//****************************************
void InitJewel(void);
void UninitJewel(void);
void UpdateJewel(void);
void DrawJewel(void);
void LoadJewel(void);
void CollisionJewel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
Jewel *GetJewel(void);

#endif
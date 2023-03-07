//****************************************
//	meshfield_wall.h
//	Author:大殿慶輔
//****************************************
#ifndef _MESHFIELD_WALL_H_
#define _MESHFIELD_WALL_H_
#include "main.h"

//****************************************
// 構造体の定義
//****************************************

//メッシュフィールド・壁の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	int	nBlock_Z;				//横のブロック数
	int nBlock_X;				//縦のブロック数

	float			fWidhtX;	//縦幅
	float			fWidhtZ;	//横幅
}MeshWall;

//****************************************
// プロトタイプ宣言
//****************************************
void InitMeshField_Wall(void);
void UninitMeshField_Wall(void);
void UpdateMeshField_Wall(void);
void DrawMeshField_Wall(void);
void CollisionMeshField_Wall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

#endif 
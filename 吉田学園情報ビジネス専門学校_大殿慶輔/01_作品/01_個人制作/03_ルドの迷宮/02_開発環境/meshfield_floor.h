//****************************************
//	meshfield_floor.h
//	Author:大殿慶輔
//****************************************
#ifndef _MESHFIELD_FLOOR_H_
#define _MESHFIELD_FLOOR_H_
#include "main.h"

//****************************************
// マクロ定義
//****************************************
#define NUM_MESHI			(3)			//メッシュの配置数
//****************************************
// 構造体の定義
//****************************************

//メッシュフィールド・床の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	int	nBlock_Z;				//横のブロック数
	int nBlock_X;				//縦のブロック数
	int nVertex;				//頂点数
	int nIdx;					//インデックス数
	int nPolygon;				//ポリゴン数

	float			fWidhtX;	//縦幅
	float			fWidhtZ;	//横幅
}MeshFloor;

//****************************************
// プロトタイプ宣言
//****************************************
void InitMeshField_Floor(void);
void UninitMeshField_Floor(void);
void UpdateMeshField_Floor(void);
void DrawMeshField_Floor(void);

#endif 
//◇====================◇
//｜　　　　　　　　　　｜
//｜　sgadow.h          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define MAX_SHADOW		(512)							//影の最大数

//****************************************
// 構造体の定義
//****************************************
//影の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	int				aColor;		//影のα値
	float			fWidhtX;	//縦幅
	float			fWidhtZ;	//横幅
	bool			bSet;		//影の設定が出来たか
	bool			bUse;		//使用しているかどうか
}Shadow;
//****************************************
// プロトタイプ宣言
//****************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(void);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fWidht,float fHight);
void DeleteShadow(int nIdxShadow);

#endif
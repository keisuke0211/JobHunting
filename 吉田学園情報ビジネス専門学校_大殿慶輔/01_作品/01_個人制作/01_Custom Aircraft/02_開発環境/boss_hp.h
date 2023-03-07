//◇====================◇
//｜　　　　　　　　　　｜
//｜　boss_hp.h         ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _BOSS_HP_H_
#define _BOSS_HP_H_

#include"main.h"

//◇==================◇
//｜　 構造体の定義 　｜
//◇==================◇
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//頂点カラー
	int nType;
	int init_nLife;		//初期体力
	int curt_nLife;		//現在の体力
	bool bUse;			//使用しているかどうか
}Bosshp;

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitBosshp(void);
void UninitBosshp(void);
void UpdateBosshp(void);
void DrawBosshp(void);

#endif
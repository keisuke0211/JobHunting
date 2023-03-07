//****************************************
//	wall.h
//	Author:大殿慶輔
//****************************************
#ifndef _WALL_H_
#define _WALL_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define MAX_WALL (150)			//壁の最大数
//*****************************************************************************
// 壁一覧
//*****************************************************************************
typedef enum
{
	WALL_LABEL_ROCKS02 = 0,		// 壊せない壁
	WALL_LABEL_ROCKS00,			// 白い壁
	WALL_LABEL_AIR,				// 空気( クリアの当たり判定用 )
	WALL_LABEL_MAX
}WALL_LABEL;
//****************************************
// 構造体の定義
//****************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3	vecLine;	//境界線ベクトル
	D3DXVECTOR3	vecToPos;	//境界線ベクトル

	int nCype;				//計算方法
	int nType;				//画像種類
	int nSplitX;			//横の分割数
	int nSplitY;			//縦の分割数
	int nLife;				//耐久力
	int nCntTime;			//切り替え時間

	float fWidth;			//横幅
	float fHight;			//高さ
	float fRate;			//pos0から交点までの距離
	bool bUse;				//使用しているかどう
	bool bDraw;
}Wall;

typedef struct
{
	int Total;		//壊せる壁の合計
	int Break;		//壊した数
	int Current;	//現在の数
	float Current_F;	//現在壊した壁のパーセント表示
	int Pull;		//合計値から8割引いた値
}BreakWall;


//****************************************
// プロトタイプ宣言
//****************************************
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void LoadWall(void);
void CollisionWallPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

Wall *GetWall(void);
BreakWall *GetBreakWall(void);


#endif

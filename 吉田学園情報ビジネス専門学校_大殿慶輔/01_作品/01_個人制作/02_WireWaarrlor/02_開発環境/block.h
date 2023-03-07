//◇====================◇
//｜　　　　　　　　　　｜
//｜　block.h           ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _BLOCK_H_  //このマクロ定義がされてなかったら
#define _BLOCK_H_  //２重インクルード防止のマクロを定義する

#include"main.h"

//*****************************************************************************
// ブロック一覧
//*****************************************************************************
typedef enum
{
	BLOCK_LABEL_ROCKS02 = 0,		// 暗い石（背景）
	BLOCK_LABEL_ROCKS00,			// 灰色の石
	BLOCK_LABEL_ROCKS01,			// 茶色の石
	BLOCK_LABEL_DIR,				// 土
	BLOCK_LABEL_BLOCK001,			// ブロック1
	BLOCK_LABEL_BLOCK002,			// ブロック2
	BLOCK_LABEL_TreasureChest,		// 宝箱
	BLOCK_LABEL_Checkpoint1,		// 中間地点1
	BLOCK_LABEL_Checkpoint2,		// 中間地点2
	BLOCK_LABEL_MAX
}BLOCK_LABEL;

//****************************************
// ブロックの構造体の定義
//****************************************

//ブロック構造体
typedef struct
{
	D3DXVECTOR3 pos;	//現在の位置
	D3DXVECTOR3 posOld;	//前回の位置
	int nType;			//種類
	int nSpeed;			//移動量
	int nSplitX;		//横の分割数
	int nSplitY;		//縦の分割数
	int nCounterAnim;	//チャストのアニメーションカウント
	int nPatternAnim;	//チャストのアニメーションパターン
	int nCounterAnim1;	//チャストのアニメーションカウント
	int nPatternAnim1;	//チャストのアニメーションパターン

	float fWidth;		//幅
	float fHight;		//高さ

	bool bUse;			//使用しているかどうか
	bool bClear;		//ゴールしたかどうか
	bool bChat;			//宝箱にふれたかどうか
}Block;
//****************************************
// マクロ定義
//****************************************
#define MAX_BLOCK (256)	//ブロックの最大数
#define NUM_BLOCK	(5) //ブロックの種類
//****************************************
// プロトタイプ宣言
//****************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void LoadBlock(void);

bool CollisionBlockPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
bool CollisionBlockLure(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
bool CollisionBlockAutoLure(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

bool CollisionBlockPendulumX(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
bool CollisionBlockPendulumY(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

bool CollisionBlockParabola(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);



Block *GetBlock(void);

#endif
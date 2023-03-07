//****************************************
//	figure_model.h
//	Author:大殿慶輔
//****************************************
#ifndef _FIGURE_MODEL_H_
#define _FIGURE_MODEL_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define NUM_FIGURE_MODEL	(7)			//モデルの配置数
#define TXT_MAX				(500)		//読み込む文字の最大数
#define MAX_TEXTURE			(10)		//テクスチャの最大数
#define DAMEGE_COUNT		(10)		//ダメージのくらってから元の色に戻るカウント
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
	float r,g,b,a;

	char aModelPath[TXT_MAX];	// モデルの相対パス
	
	bool bUse;
	bool bDraw;
}Figure;
//****************************************
// プロトタイプ宣言
//****************************************
void InitFigureModel(void);
void UninitFigureModel(void);
void UpdateFigureModel(void);
void DrawFigureModel(void);
void LoadFigureModel(void);
void HitFigure(int nCntFigure, int nDamage);
void CollisionFigurePlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
Figure *GetFigure(void);

#endif
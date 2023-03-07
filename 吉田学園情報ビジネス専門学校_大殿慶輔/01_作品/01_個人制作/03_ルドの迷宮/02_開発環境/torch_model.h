#pragma once
//****************************************
//	figure_model.h
//	Author:大殿慶輔
//****************************************
#ifndef _TORCH_MODEL_H_
#define _TORCH_MODEL_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define NUM_TORCH_M			(66)			//モデルの配置数
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

	int nType;
	int	nIdxTorchLight = -1;		//対象の照明のインデックス(番号)

	float fHight;
	float fWidth;
	float r, g, b, a;

	char aModelPath[TXT_MAX];	// モデルの相対パス

	bool bUse;
}Torch_Model;
//****************************************
// プロトタイプ宣言
//****************************************
void InitTorch_Model(void);
void UninitTorch_Model(void);
void UpdateTorch_Model(void);
void DrawTorch_Model(void);
void LoadTorch_Model(void);
void HitFigure(int nCntTorch, int nDamage);
//Torch_Model *GetTorch_Model(void);

#endif
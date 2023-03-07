#pragma once
//****************************************
//	stone_pillar_model.h
//	Author:大殿慶輔
//****************************************
#ifndef _PILLAR_MODEL_H_
#define _PILLAR_MODEL_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define NUM_PILLAR			(421)		//モデルの配置数
#define TXT_MAX				(500)		//読み込む文字の最大数
#define MAX_TEXTURE			(10)		//テクスチャの最大数
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

	float fHight;
	float fWidth;
	float r, g, b, a;

	char aModelPath[TXT_MAX];	// モデルの相対パス

	bool bUse;
}Pillar;
//****************************************
// プロトタイプ宣言
//****************************************
void InitPillar_Model(void);
void UninitPillar_Model(void);
void UpdatePillar_Model(void);
void DrawPillar_Model(void);
void LoadPillar_Model(void);
void CollisioPillar_Model(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);

Pillar *GetPillarl(void);

#endif
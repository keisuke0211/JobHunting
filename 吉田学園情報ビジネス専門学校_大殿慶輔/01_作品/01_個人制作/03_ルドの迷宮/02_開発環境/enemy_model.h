//****************************************
//	enemy_model.h
//	Author:大殿慶輔
//****************************************
#ifndef _ENEMY_MODEL_H_
#define _ENEMY_MODEL_H_

#include"main.h"
//****************************************
// マクロ定義
//****************************************
#define NUM_MODEL			(50)		//モデルの配置数
#define TXT_MAX				(500)		//読み込む文字の最大数
#define MAX_TEXTURE			(10)		//テクスチャの最大数
#define DAMEGE_COUNT		(10)		//ダメージのくらってから元の色に戻るカウント
#define DECISION			(50)		//当たり判定の追加数値
#define RETURN_COUNT		(40)		//敵のリスポーンする時間
//****************************************
// 構造体の定義
//****************************************

//モデル（置物）の構造体
typedef struct
{
	D3DXVECTOR3		pos;		// 位置
	D3DXVECTOR3		posOld;		// 前回の位置
	D3DXVECTOR3		Initpos;	// 初期位置
	D3DXVECTOR3		move;		// 移動量
	D3DXVECTOR3		rot;		// 向き
	D3DXMATRIX		mtxWorld;	// ワールドマトリックス
	int nIdxModelParent;		// 親モデルのインデックス

	int nLife;
	int nSpeed;
	int nScore;
	int nType;
	int nType_M;				//移動パターン
	int nCntSummon;
	int nCntDamage;
	int	nIdxShadow = -1;		//対象の影のインデックス(番号)

	float fHight;
	float fWidth;

	float r, g, b, a;

	char aModelPath[TXT_MAX];	// モデルの相対パス

	bool bUse;
	bool bDraw;
	bool bMove;
	bool bReturn;
}Enemy;
//****************************************
// プロトタイプ宣言
//****************************************
void InitEnemyeModel(void);
void UninitEnemyeModel(void);
void UpdateEnemyeModel(void);
void DrawEnemyeModel(void);
void LoadEnemyModel(void);
void ActionMode(Enemy *pEnemy);
void HitEnemy(int nCntFigure, int nDamage);
void CollisionEnemy_Wall(Enemy *pEnemy,D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
void CollisionEnemy_Player(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove);
Enemy *GetEnemy(void);

#endif

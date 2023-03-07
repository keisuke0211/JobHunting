//========================================
//  *** polygon.h ***
//========================================
#ifndef _POLYGON_H_	// このマクロ定義がされていなかった時
#define _POLYGON_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// 列挙型の定義
//****************************************

// 向きの種類
typedef enum
{
	ANGLE_TYPE_FREE = 0,	// 自由
	ANGLE_TYPE_FIXED,		// 固定
	ANGLE_TYPE_MAX,
}ANGLE_TYPE;

// アニメーションの種類
typedef enum
{
	ANIM_TYPE_NORMAL = 0,	// 通常
	ANIM_TYPE_TURN,			// 折り返し
	ANIM_TYPE_MAX,
}ANIM_TYPE;

//****************************************
// 構造体の定義
//****************************************

// 向きの種類毎の情報
typedef struct 
{
	// 分類関連
	char aLoadName[TXT_MAX];	// 読み込み名
}AngleType;

// アニメーションの種類毎の情報
typedef struct
{
	// 分類関連
	char aLoadName[TXT_MAX];	// 読み込み名
}AnimType;

//****************************************
// プロトタイプ宣言
//****************************************

// 向きの種類毎の情報を取得
AngleType *GetAngleType(void);

// アニメーションの種類毎の情報を取得
AnimType *GetAnimType(void);

// 向きの種類を文字列から読み込む
void StringLoadAngleType(char aString[TXT_MAX], ANGLE_TYPE *pType);

// アニメーションの種類を文字列から読み込む
void StringLoadAnimType(char aString[TXT_MAX], ANIM_TYPE *pType);

// 頂点座標の設定処理
void SetVertexPosition(VERTEX_2D *pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bMatchCamera, float fWidth, float fHeight, ANGLE_TYPE angleType);

// rhwの設定処理
void SetRHW(VERTEX_2D *pVtx);

// 頂点色の設定処理
void SetVertexColor(VERTEX_2D *pVtx, Color col);

// テクスチャ座標の設定処理
void SetTexturePosition(VERTEX_2D *pVtx, int nPtn, int nPtnMaxX, int nPtnMaxY, D3DXVECTOR3 *pPos);

// アニメーションのカウント処理
int AnimationCount(int *pPtn, int nPtnMin, int nPtnMax, int *pCounterAnim, int nAnimTime, ANIM_TYPE animType);

#endif
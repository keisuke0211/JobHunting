//========================================
// *** ui_life_000.h ***
//========================================
#ifndef _UI_LIFE_000_H_	//このマクロ定義がされていなかった時
#define _UI_LIFE_000_H_	//二重インクルード防止のマクロを定義する

//========== 構造体の定義		==========//

//ライフ000構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	float fLength;		//対角線の長さ
	float fAngle;		//対角線の角度
	int nCounterAnim;	//アニメーションカウンター
	int nPattern;		//パターンNo.
}Ui_life_000;

//========== プロトタイプ宣言	==========//
void InitUi_life_000(void);		//ライフ000の初期化処理
void UninitUi_life_000(void);	//ライフ000の終了処理
void UpdateUi_life_000(void);	//ライフ000の更新処理
void DrawUi_life_000(void);		//ライフ000の描画処理

#endif
//========================================
// 
// パーティクル[00] の処理
// Author:西村 吏功
// 
//========================================
// *** eff_particle_00.cpp ***
//========================================
#include "main.h"
#include "sound.h"
#include "eff_particle_00.h"
#include "eff_light_00.h"
#include "md_game_00.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
#define EFF_PARTICLE_00_MAX						(256)	// パーティクル[00] の最大数
#define EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MIN	(0.2f)	// パーティクル[00] の移動力にかかる乱数倍率の下限
#define EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MAX	(2.0f)	// パーティクル[00] の移動力にかかる乱数倍率の上限
#define EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MIN	(0.5f)	// パーティクル[00] の大きさにかかる乱数倍率の下限
#define EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MAX	(2.0f)	// パーティクル[00] の大きさにかかる乱数倍率の上限

// パーティクル[00] の種類毎の情報
typedef struct
{
	float fWidth;	// 幅
	float fHeight;	// 高さ
	float fMove;	// 移動力
	int nLife;		// 寿命
	int nLightLife;	// ライトの寿命
	Color col;		// 色
}Eff_particle_00Type;

//****************************************
// プロトタイプ宣言 
//****************************************
void SetParameterEff_particle_00	(Eff_particle_00 *pEff);	// パーティクル[00] のパラメーターを種類に応じて設定

//****************************************
// グローバル宣言
//****************************************
Eff_particle_00 g_aEff_particle_00[EFF_PARTICLE_00_MAX];	// パーティクル[00] の情報

// 爆発[00] の種類の情報
Eff_particle_00Type g_aEff_particle_00Type[EFF_PARTICLE_00_TYPE_MAX] =
{//   幅        , 高さ      , 移動力 , 寿命 , ライトの寿命 , 色
	{ PIXEL * 3 , PIXEL * 6 , 14.0f  , 35   , 5           , { 255, 155, 59 , 255 } },
	{ PIXEL * 6 , PIXEL * 8 , 20.0f  , 35   , 10          , { 255, 155, 59 , 255 } },
	{ PIXEL * 3 , PIXEL * 8 , 14.0f  , 25   , 5           , { 171, 231, 255, 255 } },
	{ PIXEL * 3 , PIXEL * 8 , 14.0f  , 25   , 5           , { 255, 191, 179, 255 } },
};

//========== *** パーティクル[00] の情報を取得 ***
Eff_particle_00 *GetEff_particle_00(void)
{
	return g_aEff_particle_00;
}

//========================================
// InitEff_particle_00関数 - パーティクル[00] の初期化処理 -
//========================================
void InitEff_particle_00(void)
{
	Eff_particle_00 *pEff = g_aEff_particle_00;	// パーティクル[00] の情報のポインタ

	for (int nCntEff_particle_00 = 0; nCntEff_particle_00 < EFF_PARTICLE_00_MAX; nCntEff_particle_00++, pEff++)
	{
		pEff->bUse = false;						// 使用されているかフラグを初期化する
		pEff->type = (EFF_PARTICLE_00_TYPE)0;	// 種類を設定する

		// パラメーターを種類に応じて設定
		SetParameterEff_particle_00(pEff);
	}
}

//========================================
// UpdateEff_particle_00関数 - パーティクル[00] の更新処理 -
//========================================
void UpdateEff_particle_00(void)
{
	Eff_particle_00 *pEff = g_aEff_particle_00;	// パーティクル[00] の情報のポインタ

	for (int nCntEff_particle_00 = 0; nCntEff_particle_00 < EFF_PARTICLE_00_MAX; nCntEff_particle_00++, pEff++)
	{
		if (!g_aEff_particle_00[nCntEff_particle_00].bUse)
		{// パーティクル[00] が使用されていない時、
			// 繰り返し処理を折り返す
			continue;
		}

		// 位置を更新
		pEff->pos.x += pEff->move.x;
		pEff->pos.y += pEff->move.y;

		// 移動量を減衰
		pEff->move.x += (0.0f - pEff->move.x) * 0.1f;
		pEff->move.y += (0.0f - pEff->move.y) * 0.1f;

		// 寿命のカウント を減算
		pEff->nLife--;

		if (pEff->nLife <= 0)
		{// 寿命が尽きた時、
			// 使用していない状態にする
			pEff->bUse = false;
		}

		// ライト[00] を設定
		SetEff_light_00(pEff->pos, pEff->rot, 0.0f,
			g_aEff_particle_00Type[pEff->type].col,
			g_aEff_particle_00Type[pEff->type].fWidth
			* ((float)pEff->nLife / (float)g_aEff_particle_00Type[pEff->type].nLife),
			g_aEff_particle_00Type[pEff->type].fHeight
			* ((float)pEff->nLife / (float)g_aEff_particle_00Type[pEff->type].nLife),
			g_aEff_particle_00Type[pEff->type].nLightLife
			* ((float)pEff->nLife / (float)g_aEff_particle_00Type[pEff->type].nLife),
			EFF_LIGHT_00_TYPE_00);
	}
}

//========================================
// SetEff_particle_00関数 - パーティクル[00]の設定処理 -
//========================================
void SetEff_particle_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, EFF_PARTICLE_00_TYPE type)
{
	if (type == EFF_PARTICLE_00_TYPE_N1) 
	{// 種類が無しの時、
		// 処理を終了する
		return;
	}

	Eff_particle_00 *pEff = g_aEff_particle_00;	// パーティクル[00] の情報のポインタ

	for (int nCntEff_particle_00 = 0; nCntEff_particle_00 < EFF_PARTICLE_00_MAX; nCntEff_particle_00++, pEff++)
	{
		if (g_aEff_particle_00[nCntEff_particle_00].bUse)
		{// パーティクル[00]が使用されている時、
			// 繰り返し処理を折り返す
			continue;
		}

		// 種類を代入
		pEff->type = type;

		// パラメーターを種類に応じて設定
		SetParameterEff_particle_00(&g_aEff_particle_00[nCntEff_particle_00]);

		// 位置/角度 を代入
		pEff->pos = pos;
		pEff->rot = rot;

		// 向いている方向に移動量を加算
		pEff->move.x = sinf(rot.z) * pEff->fMove;
		pEff->move.y = cosf(rot.z) * pEff->fMove;

		// 使用している状態にする
		pEff->bUse = true;

		break;
	}
}

//========================================
// SetState関数 - パーティクル[00] のパラメーターを種類に応じて設定 -
//========================================
void SetParameterEff_particle_00(Eff_particle_00 *pEff) 
{
	pEff->fWidth	= g_aEff_particle_00Type[pEff->type].fWidth;	// 幅
	pEff->fHeight	= g_aEff_particle_00Type[pEff->type].fHeight;	// 高さ
	pEff->fMove		= g_aEff_particle_00Type[pEff->type].fMove;		// 移動力
	pEff->nLife		= g_aEff_particle_00Type[pEff->type].nLife;		// 寿命

	// 移動力に乱数を発生させる
	pEff->fMove *=
		EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MIN + fRand(EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MAX - EFF_PARTICLE_00_MOVE_RAND_DIAMETER_MIN);

	// 大きさに乱数を発生させる
	{
		float fScaleDiameter =	// 大きさの倍率
			EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MIN + fRand(EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MAX - EFF_PARTICLE_00_SCALE_RAND_DIAMETER_MIN);

		// 幅と高さに倍率を適用させる
		pEff->fWidth *= fScaleDiameter;
		pEff->fHeight *= fScaleDiameter;
	}
}
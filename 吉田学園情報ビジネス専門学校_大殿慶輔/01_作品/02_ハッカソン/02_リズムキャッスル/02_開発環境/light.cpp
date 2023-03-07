//========================================
// 
// 光の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** light.cpp ***
//========================================
#include "chunk.h"
#include "color.h"
#include "main.h"
#include "physics.h"
#include "light.h"

//****************************************
// マクロ定義
//****************************************
// 光(2D)の最大数
// 光(2D)の明るさの最低値
#define LIGHT2D_MAX				(64)
#define LIGHT2D_BRIGHTNESS_MIN	(25)

// 光(3D)の最大数
#define LIGHT3D_MAX	(8)

//****************************************
// 構造体の定義
//****************************************
// 光(2D)の予約情報構造体
typedef struct
{
	// 位置関連
	D3DXVECTOR3 pos;		// 位置
	float		fLength;	// 距離

	// 状態関連
	bool	bUse;		// 使用されているかフラグ
	Color	col;		// 色
	float	fDensity;	// 濃さ
}ReserveInfoLight2D;

// 光(3D)の設定情報
typedef struct 
{
	// 位置関連
	D3DXVECTOR3	rot;			// 向き
	float		fBrightness;	// 明るさ
}Light3DSettingInfo;

//****************************************
// プロトタイプ宣言
//****************************************
// 光(2D)の初期化処理
// 光(2D)の終了処理
// 光(2D)の更新処理
void InitLight2D(void);
void UninitLight2D(void);
void UpdateLight2D(void);

// 光(2D)の一括設定処理
void BulkSetLight2D(void);

// 光(3D)の管理情報のパラメーター初期化処理
void InitParameterLight3D(void);

// 光(3D)の初期化処理
// 光(3D)の終了処理
// 光(3D)の更新処理
void InitLight3D(void);
void UninitLight3D(void);
void UpdateLight3D(void);

//****************************************
// グローバル変数
//****************************************
Light2D				g_aLight2D				// 光(2D)の情報
					[LIGHT2D_MAX];
ReserveInfoLight2D	g_aReserveInfoLight2D	// 光(2D)の予約情報
					[LIGHT2D_MAX];
D3DLIGHT9			g_light3D;				// 光(3D)の情報
Light3DControl		g_light3DControl;		// 光(3D)の管理情報

// 光(3D)の設定情報
const Light3DSettingInfo g_aLight3DSettingInfo[LIGHT3D_MAX]=
{
	{ D3DXVECTOR3( 0.0f , -0.5f , 0.5f),1.0f  },	// 前下
	{ D3DXVECTOR3(-0.5f , -0.5f , 0.5f),0.75f },	// 左前下
	{ D3DXVECTOR3( 0.5f , -0.5f , 0.5f),0.75f },	// 右前下
	{ D3DXVECTOR3(-0.9f , -0.9f , 0.0f),0.5f  },	// 左
	{ D3DXVECTOR3( 0.9f , -0.9f , 0.0f),0.5f  },	// 右
	{ D3DXVECTOR3(-0.5f , -0.5f ,-0.5f),0.25f },	// 左後下
	{ D3DXVECTOR3( 0.5f , -0.5f ,-0.5f),0.25f },	// 右後下
	{ D3DXVECTOR3( 0.0f , -0.5f ,-0.5f),0.25f },	// 後下
};

//========== *** 光(2D)の情報を取得 ***
Light2D *GetLight2D(void) 
{
	return g_aLight2D;
}

//========== *** 光(3D)の管理情報を取得 ***
Light3DControl *GetLight3DControl(void) 
{
	return &g_light3DControl;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitLight2D関数 - 光(2D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitLight2D(void) 
{
	Light2D				*pSys			// 光の情報のポインタ
						= g_aLight2D;
	ReserveInfoLight2D	*pSysReserve	// 光の予約情報のポインタ
						= g_aReserveInfoLight2D;

	for (int nCntSys = 0; nCntSys < LIGHT2D_MAX; nCntSys++, pSys++, pSysReserve++)
	{
		pSys->bUse			= false;	// 使用されていない状態にする
		pSysReserve->bUse	= false;	// 予約情報
	}
}

//========================================
// UninitLight2D関数 - 光(2D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitLight2D(void) 
{

}

//========================================
// UpdateLight2D関数 - 光(2D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateLight2D(void)
{
	BulkSetLight2D();	// 光の一括設定処理
}

//========================================
// InitParameterLight3D関数 - 光(3D)の管理情報のパラメーター初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitParameterLight3D(void)
{
	g_light3DControl.col			= INITCOLOR;	// 色
	g_light3DControl.fBrightness	= 1.0f;			// 明るさ
}

//========================================
// InitLight3D関数 - 光(3D)の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitLight3D(void)
{
	// 管理情報のパラメーター初期化処理
	InitParameterLight3D();

	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXVECTOR3			vecDir;	// 設定用方向ベクトル

	// 光(3D)の情報をクリアする
	ZeroMemory(&g_light3D, sizeof(D3DLIGHT9));

	// 光(3D)の種類を設定
	g_light3D.Type = D3DLIGHT_DIRECTIONAL;

	for (int nCntLight3D = 0; nCntLight3D < LIGHT3D_MAX; nCntLight3D++)
	{
		// 光(3D)の拡散光を設定
		g_light3D.Diffuse = ColorToD3DXCOLOR(BrightnessToColor(g_light3DControl.col, g_aLight3DSettingInfo[nCntLight3D].fBrightness));

		// 光(3D)の方向を設定
		vecDir = g_aLight3DSettingInfo[nCntLight3D].rot;
		D3DXVec3Normalize(&vecDir, &vecDir);

		// ベクトルを正規化する
		g_light3D.Direction = vecDir;

		// 光(3D)を設定する
		pDevice->SetLight(nCntLight3D, &g_light3D);

		// 光(3D)を有効にする
		pDevice->LightEnable(nCntLight3D, TRUE);
	}
}

//========================================
// UninitLight3D関数 - 光(3D)の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitLight3D(void)
{

}

//========================================
// UpdateLight3D関数 - 光(3D)の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateLight3D(void)
{

}

//========================================
// BulkSetLight2D関数 - 光(2D)の一括設定処理 -
// Author:RIKU NISHIMURA
//========================================
void BulkSetLight2D(void) 
{
	Light2D				*pSys			// 光の情報のポインタ
						= g_aLight2D;
	ReserveInfoLight2D	*pSysReserve	// 光の予約情報のポインタ
						= g_aReserveInfoLight2D;

	for (int nCntSys = 0; nCntSys < LIGHT2D_MAX; nCntSys++, pSys++)
	{
		pSys->bUse = false;	// 使用されていない状態にする

		if (!pSysReserve->bUse) 
		{// カウントの予約情報が使用されている状態でない時、
			// 繰り返し処理を折り返す
			continue;
		}

		pSys->pos			= pSysReserve->pos;			// 位置を代入
		pSys->col			= pSysReserve->col;			// 色を代入
		pSys->fLength		= pSysReserve->fLength;		// 距離を代入
		pSys->bUse			= true;						// 使用されている状態にする
		pSysReserve->bUse	= false;					// 予約情報を使用されていない状態にする
		pSysReserve++;									// 予約情報のポインタを進める
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitLight関数 - 光の初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void InitLight(void)
{
	InitLight2D();	// 光(2D)の初期化処理
	InitLight3D();	// 光(3D)の初期化処理
}

//========================================
// UninitLight関数 - 光の終了処理 -
// Author:RIKU NISHIMURA
//========================================
void UninitLight(void)
{
	UninitLight2D();	// 光(2D)の終了処理
	UninitLight3D();	// 光(3D)の終了処理
}

//========================================
// UpdateLight関数 - 光の更新処理 -
// Author:RIKU NISHIMURA
//========================================
void UpdateLight(void)
{
	UpdateLight2D();	// 光(2D)の更新処理
	UpdateLight3D();	// 光(3D)の更新処理
}

//========================================
// ReserveLight2D関数 - 光(2D)の予約処理 -
// Author:RIKU NISHIMURA
//========================================
void ReserveLight2D(D3DXVECTOR3 pos, Color col, float fLength)
{
	ReserveInfoLight2D	*pSys	// 光の予約情報のポインタ
						= g_aReserveInfoLight2D;

	for (int nCntSys = 0; nCntSys < LIGHT2D_MAX; nCntSys++, pSys++)
	{
		if (pSys->bUse) 
		{// カウントの予約情報が使用されている状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		pSys->pos		= pos;		// 位置を代入
		pSys->col		= col;		// 色を代入
		pSys->fLength	= fLength;	// 距離を代入
		pSys->bUse		= true;		// 使用されている状態にする

		// 繰り返し処理を抜ける
		break;
	}
}

//========================================
// GetColorForLight2D関数 - 光(2D)との距離から色を取得 -
// Author:RIKU NISHIMURA
//========================================
Color GetColorForLight2D(D3DXVECTOR3 pos, Color myCol)
{
	Light2D	*pLight	// 光の情報のポインタ
			= g_aLight2D;
	Color	col		// 色
			= { LIGHT2D_BRIGHTNESS_MIN,LIGHT2D_BRIGHTNESS_MIN,LIGHT2D_BRIGHTNESS_MIN,255 };
	float	fBright	// 明るさ
			= 1.0f;

	for (int nCntSys = 0; nCntSys < LIGHT2D_MAX; nCntSys++, pLight++)
	{
		float	fDistance	// 距離
				= FindDistance(pos, pLight->pos);

		if ((pLight->bUse) && fDistance <= pLight->fLength)
		{// 使用されているか、距離が光の長さ内側の時、
			// 色を距離の割合分加算
			col.r += pLight->col.r * (1.0f - (fDistance / pLight->fLength)) * fBright;
			col.g += pLight->col.g * (1.0f - (fDistance / pLight->fLength)) * fBright;
			col.b += pLight->col.b * (1.0f - (fDistance / pLight->fLength)) * fBright;

			// 色を制御
			IntControl(&col.r, 255, 0);
			IntControl(&col.g, 255, 0);
			IntControl(&col.b, 255, 0);
		}
	}

	// 自分の色を適用
	col.r *= ((float)myCol.r / (float)(255));
	col.g *= ((float)myCol.g / (float)(255));
	col.b *= ((float)myCol.b / (float)(255));
	col.a *= ((float)myCol.a / (float)(255));
	
	// 色を返す
	return col;
}

//========================================
// SetBrightnessLight3D関数 - 光(3D)の明るさ設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetBrightnessLight3D(float fBrightness)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXVECTOR3			vecDir;	// 設定用方向ベクトル

	// 光(3D)の情報をクリアする
	ZeroMemory(&g_light3D, sizeof(D3DLIGHT9));

	// 光(3D)の種類を設定
	g_light3D.Type = D3DLIGHT_DIRECTIONAL;

	for (int nCntLight3D = 0; nCntLight3D < LIGHT3D_MAX; nCntLight3D++)
	{
		// 光(3D)の拡散光を設定
		g_light3D.Diffuse = ColorToD3DXCOLOR(BrightnessToColor(g_light3DControl.col, g_aLight3DSettingInfo[nCntLight3D].fBrightness)) * fBrightness;

		// 光(3D)の方向を設定
		vecDir = g_aLight3DSettingInfo[nCntLight3D].rot;
		D3DXVec3Normalize(&vecDir, &vecDir);

		// ベクトルを正規化する
		g_light3D.Direction = vecDir;

		// 光(3D)を設定する
		pDevice->SetLight(nCntLight3D, &g_light3D);

		// 光(3D)を有効にする
		pDevice->LightEnable(nCntLight3D, TRUE);
	}

	g_light3DControl.fBrightness = fBrightness;	// 明るさを保存
}

//========================================
// SetColorLight3D関数 - 光(3D)の色設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetColorLight3D(Color col)
{
	LPDIRECT3DDEVICE9	pDevice	// デバイス取得
						= GetDevice();
	D3DXVECTOR3			vecDir;	// 設定用方向ベクトル

	// 光(3D)の情報をクリアする
	ZeroMemory(&g_light3D, sizeof(D3DLIGHT9));

	// 光(3D)の種類を設定
	g_light3D.Type = D3DLIGHT_DIRECTIONAL;

	// 光(3D)の色を設定
	g_light3DControl.col = col;

	for (int nCntLight3D = 0; nCntLight3D < LIGHT3D_MAX; nCntLight3D++)
	{
		// 光(3D)の拡散光を設定
		g_light3D.Diffuse = ColorToD3DXCOLOR(BrightnessToColor(g_light3DControl.col, g_aLight3DSettingInfo[nCntLight3D].fBrightness)) * g_light3DControl.fBrightness;

		// 光(3D)の方向を設定
		vecDir = g_aLight3DSettingInfo[nCntLight3D].rot;
		D3DXVec3Normalize(&vecDir, &vecDir);

		// ベクトルを正規化する
		g_light3D.Direction = vecDir;

		// 光(3D)を設定する
		pDevice->SetLight(nCntLight3D, &g_light3D);

		// 光(3D)を有効にする
		pDevice->LightEnable(nCntLight3D, TRUE);
	}
}
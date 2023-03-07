//========================================
// 
// 爆発[00] の処理
// Author:西村 吏功
// 
//========================================
// *** atk_explosion_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "sound.h"
#include "chr_enemy_00.h"
#include "eff_particle_00.h"
#include "atk_explosion_00.h"
#include "input.h"
#include <stdio.h>

//****************************************
// マクロ定義
//****************************************
#define ATK_EXPLOSION_00_MAX	(256)	// 爆発[00] の最大数

//****************************************
// 列挙型の定義
//****************************************

// 標的タイプ
typedef enum
{
	TARGET_TYPE_ENEMY_ONRY
}TARGET_TYPE;

//****************************************
// 構造体の定義
//****************************************

// 爆発[00] 構造体の定義
typedef struct 
{
	// 位置関連
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	float fWidth;				// 幅
	float fHeight;				// 高さ
	float fHitTestRadius;		// 当たり判定の半径
	
	// 分類情報
	ATK_EXPLOSION_00_TYPE type;	// 種類
	bool bUse;					// 使用されているかフラグ

	// 状態値
	int nDamage;				// ダメージ
	int nPattern;				// パターンNo.
	int nCounterAnim;			// アニメーションのカウンター
	int nCntChain;				// 連鎖のカウント
}Atk_explosion_00;

// 爆発[00] の種類毎の情報
typedef struct
{
	char aTexturPath[TXT_MAX];		// テクスチャの相対パス
	int nPtnMaxX;					// パターンの最大数(横)
	int nPtnMaxY;					// パターンの最大数(縦)
	int nAnimChangeTime;			// アニメーションの切り替え時間
	float fWidth;					// 幅
	float fHeight;					// 高さ
	float fHitRadius;				// 当たり判定の半径
	int nDamage;					// ダメージ
	SOUND_LABEL soundSet;			// 設定時のサウンド
	EFF_PARTICLE_00_TYPE ptclType;	// パーティクル[00] の種類
	int nPtclNum;					// パーティクルの発生数
	float fVibration;				// 振動の強さ
	int nStopTime;					// ヒットストップの時間
	TARGET_TYPE targetType;			// 目標タイプ
	bool bChain;					// 連爆フラグ
}Atk_explosion_00Type;

//****************************************
// プロトタイプ宣言
//****************************************
void CollisionChr_fighter_00	(Atk_explosion_00 *pAtk);	// 機体[00] との当たり判定処理
void CollisionChr_enemy_00		(Atk_explosion_00 *pAtk);	// 敵[00] との当たり判定
void SetState					(Atk_explosion_00 *pAtk);	// 爆発[00] のステータスを種類に応じて設定

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9 g_aTextureAtk_explosion_00[ATK_EXPLOSION_00_TYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAtk_explosion_00 = NULL;						// 頂点バッファへのポインタ
Atk_explosion_00 g_aAtk_explosion_00[ATK_EXPLOSION_00_MAX];						// 爆発[00] の情報

// 爆発[00] の種類の情報
Atk_explosion_00Type g_aAtk_explosion_00Type[ATK_EXPLOSION_00_TYPE_MAX] =
{//   テクスチャの相対パス                          , パターンの最大X, Y, アニメーションにかかる時間, 幅         , 高さ       , 当たり判定の半径, ダメージ, SE                           , パーティクルの種類           , パーティクルの発生数 , 振動の強さ, ストップ時間, 標的タイプ
	{ "data\\TEXTURE\\eff_explosion_002.png"		, 4              , 1, 4                         , PIXEL * 16 , PIXEL * 16 , PIXEL * 8       , 1       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 10                   , 4.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_001.png"		, 4              , 2, 4                         , PIXEL * 32 , PIXEL * 32 , PIXEL * 16      , 2       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 30                   , 4.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_000_medium.png" , 4              , 2, 4                         , PIXEL * 32 , PIXEL * 32 , PIXEL * 16      , 2       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_01, 40                   , 16.0f     , 40           , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_000_lange.png"	, 4              , 2, 4                         , PIXEL * 64 , PIXEL * 64 , PIXEL * 32      , 3       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_01, 50                   , 64.0f     , 80           , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_shot_break_001.png"	    , 4              , 1, 4                         , PIXEL * 16 , PIXEL * 16 , PIXEL * 0       , 0       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_N1    , 0                    , 0.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , false},
	{ "data\\TEXTURE\\eff_explosion_002.png"	    , 4              , 1, 4                         , PIXEL * 16 , PIXEL * 16 , PIXEL * 0       , 100     , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_N1    , 0                    , 0.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_001.png"		, 4              , 2, 4                         , PIXEL * 32 , PIXEL * 32 , PIXEL * 0       , 0       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 30                   , 0.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , true},
	{ "data\\TEXTURE\\eff_explosion_002.png"	    , 4              , 1, 4                         , PIXEL * 16 , PIXEL * 16 , PIXEL * 8       , 100     , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 10                   , 2.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , false},
	{ "data\\TEXTURE\\eff_explosion_001.png"		, 4              , 2, 4                         , PIXEL * 32 , PIXEL * 32 , PIXEL * 16      , 2       , (SOUND_LABEL)0 , EFF_PARTICLE_00_TYPE_SPARK_00, 30                   , 4.0f      , 0            , TARGET_TYPE_ENEMY_ONRY , false},
};

//========================================
// InitAtk_explosion_00関数 - 爆発[00] の初期化処理 -
//========================================
void InitAtk_explosion_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntAtk_explosion_00_type = 0; nCntAtk_explosion_00_type < ATK_EXPLOSION_00_TYPE_MAX; nCntAtk_explosion_00_type++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aAtk_explosion_00Type[nCntAtk_explosion_00_type].aTexturPath, &g_aTextureAtk_explosion_00[nCntAtk_explosion_00_type]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ATK_EXPLOSION_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAtk_explosion_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_explosion_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_MAX; nCntAtk_explosion_00++, pVtx += 4)
	{
		Atk_explosion_00 *pAtk = &g_aAtk_explosion_00[nCntAtk_explosion_00];	// 爆発[00] の情報のポインタ

		pAtk->nCounterAnim	= 0;								// カウンターXを初期化する
		pAtk->nPattern		= 0;								// パターンNo.を初期化する
		pAtk->pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置を初期化する
		pAtk->rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きを初期化する
		pAtk->bUse			= false;							// 使用されているかフラグを初期化する
		pAtk->type			= ATK_EXPLOSION_00_TYPE_00;			// 種類を初期化する			

		// ステータスを種類に応じて設定
		SetState(pAtk);

		{
			D3DXVECTOR3 pos = pAtk->pos;												// 位置
			D3DXVECTOR3 rot = pAtk->rot;												// 向き
			float fWidth	= pAtk->fWidth;												// 幅
			float fHeight	= pAtk->fHeight;											// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

			// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		{
			int nPtn		= pAtk->nPattern;								// テクスチャのパターンNo.
			int nPtnMaxX	= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxX;	// テクスチャのパターン数(横)
			int nPtnMaxY	= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxY;	// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffAtk_explosion_00->Unlock();
}

//========================================
// UninitAtk_explosion_00関数 - 爆発[00] の終了処理 -
//========================================
void UninitAtk_explosion_00(void)
{
	// テクスチャの破棄
	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_TYPE_MAX; nCntAtk_explosion_00++)
	{
		if (g_aTextureAtk_explosion_00[nCntAtk_explosion_00] != NULL)
		{
			g_aTextureAtk_explosion_00[nCntAtk_explosion_00]->Release();
			g_aTextureAtk_explosion_00[nCntAtk_explosion_00] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffAtk_explosion_00 != NULL) 
	{
		g_pVtxBuffAtk_explosion_00->Release();
		g_pVtxBuffAtk_explosion_00 = NULL;
	}
}

//========================================
// UpdateAtk_explosion_00関数 - 爆発[00] の更新処理 -
//========================================
void UpdateAtk_explosion_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_explosion_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_MAX; nCntAtk_explosion_00++, pVtx += 4)
	{
		if (!g_aAtk_explosion_00[nCntAtk_explosion_00].bUse)
		{// 爆発[00] が使用されている状態でない時、
			// 繰り返しをスキップ
			continue;
		}

		Atk_explosion_00 *pAtk = &g_aAtk_explosion_00[nCntAtk_explosion_00];	// 爆発[00] の情報のポインタ

		D3DXVECTOR3 pos		= pAtk->pos;											// 位置
		D3DXVECTOR3 rot		= pAtk->rot;											// 向き
		int nPtn			= pAtk->nPattern;										// テクスチャのパターンNo.
		float fWidth		= pAtk->fWidth;											// 幅
		float fHeight		= pAtk->fHeight;										// 高さ
		int nAnimChangeTime = g_aAtk_explosion_00Type[pAtk->type].nAnimChangeTime;	// アニメーションの切り替え時間
		int nPtnMaxX		= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxX;			// テクスチャのパターン数(横)
		int nPtnMaxY		= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxY;			// テクスチャのパターン数(縦)

		// アニメーションのカウンターを加算
		pAtk->nCounterAnim++;

		if (pAtk->nCounterAnim == nAnimChangeTime)
		{// アニメーションのカウンターが切り替え時間に達した時、
			// アニメーションのカウンターを初期化
			pAtk->nCounterAnim = 0;

			// パターンを更新
			pAtk->nPattern++;
		}

		if (pAtk->nPattern == nPtnMaxX * nPtnMaxY)
		{// 爆発[00] のパターンNo.がパターンの上限を上回った時、
			pAtk->bUse = false; // 使用していない状態にする
		}

		{
			D3DXVECTOR3 drawPos = pos;													// 描画位置
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

			// 描画位置を設定
			MatchCameraPosition(&drawPos);

			// 頂点座標を設定
			pVtx[0].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = drawPos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = drawPos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		{
			float fAlpha = 1.0f - ((float)((pAtk->nPattern * nAnimChangeTime) + pAtk->nCounterAnim) 
				/ (float)(nPtnMaxX * nPtnMaxY * nAnimChangeTime));	// 透明度

			// 頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * fAlpha));
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * fAlpha));
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * fAlpha));
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, (int)(255 * fAlpha));
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
	}

	// 頂点座標をアンロックする
	g_pVtxBuffAtk_explosion_00->Unlock();
}

//========================================
// DrawAtk_explosion_00関数 - 爆発[00] の描画処理 -
//========================================
void DrawAtk_explosion_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAtk_explosion_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_MAX; nCntAtk_explosion_00++)
	{
		if (g_aAtk_explosion_00[nCntAtk_explosion_00].bUse)
		{// 使用されている時、
			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureAtk_explosion_00[g_aAtk_explosion_00[nCntAtk_explosion_00].type]);

			// 機体[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAtk_explosion_00 * 4, 2);
		}
	}
}

//========================================
// Atk_explosion_00KeyInput関数 - 爆発[00] の設定処理 -
//========================================
void SetAtk_explosion_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ATK_EXPLOSION_00_TYPE type, int nCntChain)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffAtk_explosion_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAtk_explosion_00 = 0; nCntAtk_explosion_00 < ATK_EXPLOSION_00_MAX; nCntAtk_explosion_00++, pVtx += 4)
	{
		if (g_aAtk_explosion_00[nCntAtk_explosion_00].bUse)
		{// 爆発[00] が使用されている時、
			// 繰り返し処理を折り返す
			continue;
		}

		// 時間待ち
		Sleep(g_aAtk_explosion_00Type[type].nStopTime);

		Atk_explosion_00 *pAtk = &g_aAtk_explosion_00[nCntAtk_explosion_00];	// 爆発[00] の情報のポインタ

		// 振動を設定
		SetCameraVibration(g_aAtk_explosion_00Type[type].fVibration);

		// 爆発[00] の種類を設定する
		pAtk->type = type;

		// 連鎖のカウントを代入
		pAtk->nCntChain = nCntChain - (g_aAtk_explosion_00Type[type].bChain ^ 1);

		// ステータスを種類に応じて設定
		SetState(pAtk);

		// 効果音を再生
		PlaySound(g_aAtk_explosion_00Type[type].soundSet);

		// 位置/角度を代入
		pAtk->pos = pos;
		pAtk->rot = rot;

		{
			D3DXVECTOR3 drawPos = pAtk->pos;												// 描画位置
			D3DXVECTOR3 rot		= pAtk->rot;												// 向き
			float fWidth		= pAtk->fWidth;												// 幅
			float fHeight		= pAtk->fHeight;											// 高さ
			float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle		= atan2f(fWidth, fHeight);									// 対角線の角度

			// 描画位置を設定
			MatchCameraPosition(&drawPos);

			// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf((rot.z + D3DX_PI) - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf((rot.z + D3DX_PI) + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf((rot.z + D3DX_PI) - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf((rot.z + D3DX_PI) + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf((rot.z + D3DX_PI) + fAngle) * fLength;
		}

		pAtk->nPattern		= 0;	// パターンNo.を初期化する
		pAtk->nCounterAnim	= 0;	// アニメーションのカウンターを初期化する

		{
			int nPtn			= pAtk->nPattern;								// テクスチャのパターンNo.
			int nPtnMaxX		= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxX;	// テクスチャのパターン数(横)
			int nPtnMaxY		= g_aAtk_explosion_00Type[pAtk->type].nPtnMaxY;	// テクスチャのパターン数(縦)

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, ((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX)						, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((nPtn % nPtnMaxX) * (1.0f / nPtnMaxX) + (1.0f / nPtnMaxX)	, (((nPtn / nPtnMaxX) % nPtnMaxY) * (1.0f / nPtnMaxY)) + (1.0f / nPtnMaxY));
		}

		// 目標タイプに応じた当たり判定処理
		switch (g_aAtk_explosion_00Type[pAtk->type].targetType)
		{
			//========== *** 敵のみ ***
		case TARGET_TYPE_ENEMY_ONRY:

			// 敵[00]との当たり判定処理
			CollisionChr_enemy_00(pAtk);

			break;
		}

		// 使用している状態にする
		pAtk->bUse = true;

		// パーティクルの設定処理
		for (int nCntPtcl = 0; nCntPtcl < g_aAtk_explosion_00Type[pAtk->type].nPtclNum; nCntPtcl++) 
		{
			D3DXVECTOR3 setRot;	// 設定する向きを設定
			setRot.z = -D3DX_PI + fRand(D3DX_PI * 2);

			// パーティクル[00] を設定
			SetEff_particle_00(pos, setRot, g_aAtk_explosion_00Type[pAtk->type].ptclType);
		}

		break;
	}

	// 頂点座標をアンロックする
	g_pVtxBuffAtk_explosion_00->Unlock();
}

//========================================
// SetState関数 - 爆発[00] のステータスを種類に応じて設定 -
//========================================
void SetState(Atk_explosion_00 *pAtk_explosion_00)
{
	pAtk_explosion_00->fWidth				= g_aAtk_explosion_00Type[pAtk_explosion_00->type].fWidth;				// 幅
	pAtk_explosion_00->fHeight				= g_aAtk_explosion_00Type[pAtk_explosion_00->type].fHeight;				// 高さ
	pAtk_explosion_00->fHitTestRadius		= g_aAtk_explosion_00Type[pAtk_explosion_00->type].fHitRadius;			// 当たり判定の半径
	pAtk_explosion_00->nDamage				= g_aAtk_explosion_00Type[pAtk_explosion_00->type].nDamage;				// ダメージ
}

//========================================
// CollisionChr_fighter_00関数 - 機体[00]との当たり判定処理 -
//========================================
void CollisionChr_fighter_00(Atk_explosion_00 *pAtk)
{
	
}

//========================================
// CollisionChr_enemy_00関数 - 敵[00]との当たり判定処理 -
//========================================
void CollisionChr_enemy_00(Atk_explosion_00 *pAtk)
{
	Chr_enemy_00 *pChr = GetChr_enemy_00();	// 敵[00] の情報のポインタ

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if ((!pChr->bUse)
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_POP])
			|| (pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
			|| (pChr->aState[CHR_ENEMY_00_STATE_SWELL])
			|| (pChr->aAct[CHR_ENEMY_00_ACT_GHOST]))
		{// 使用されていない状態、もしくは出現中か消え中か膨らみかすり抜けの時、
			// 繰り返し処理を折り返す
			continue;
		}

		
	}
}
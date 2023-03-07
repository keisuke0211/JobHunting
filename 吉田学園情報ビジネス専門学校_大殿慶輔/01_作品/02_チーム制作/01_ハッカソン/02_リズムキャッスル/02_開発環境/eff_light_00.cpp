//========================================
// 
// ライト[00]の処理
// Author:西村 吏功
// 
//========================================
// *** eff_light_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "eff_light_00.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//****************************************
// マクロ定義
//****************************************
#define EFF_LIGHT_00_MAX	(1024)	// ライト[00] の最大数

//****************************************
// 構造体の定義
//****************************************

// ライト[00]構造体の定義
typedef struct
{
												//========== *** 位置関連 ***
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 move;							// 移動量
	float fMove;								// 移動力
	D3DXVECTOR3 rot;							// 向き
	float fWidth;								// 幅
	float fHeight;								// 高さ
												//========== *** 分類情報 ***
	int nLife;									// 寿命
	int nLifeMax;								// 寿命の最大値
	EFF_LIGHT_00_TYPE type;						// 種類
	bool bUse;									// 使用されているかフラグ
												//========== *** 状態値 ***
	Color col;									// 色
	bool aState[EFF_LIGHT_00_STATE_MAX];		// 状態フラグ
	int aCounterState[EFF_LIGHT_00_STATE_MAX];	// 状態管理カウンター
}Eff_light_00;

// ライト[00] の種類毎の情報
typedef struct
{
	char aTexturPath[TXT_MAX];	// テクスチャの相対パス
}Eff_light_00Type;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9 g_aTextureEff_light_00[EFF_LIGHT_00_TYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEff_light_00 = NULL;					// 頂点バッファへのポインタ
Eff_light_00 g_aEff_light_00[EFF_LIGHT_00_MAX];							// ライト[00] の情報

// ライト[00] の種類の情報
Eff_light_00Type g_aEff_light_00Type[EFF_LIGHT_00_TYPE_MAX] =
{//   テクスチャの相対パス
	{ "data\\TEXTURE\\Effect\\effect000.jpg"},
};

//========================================
// InitEff_light_00関数 - ライト[00]の初期化処理 -
//========================================
void InitEff_light_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntEff_light_00_type = 0; nCntEff_light_00_type < EFF_LIGHT_00_TYPE_MAX; nCntEff_light_00_type++)
	{
		D3DXCreateTextureFromFile(pDevice, g_aEff_light_00Type[nCntEff_light_00_type].aTexturPath, &g_aTextureEff_light_00[nCntEff_light_00_type]);
	}

	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++) 
	{
		g_aEff_light_00[nCntEff_light_00].bUse	= false;				// 使用されているかフラグを初期化する
		g_aEff_light_00[nCntEff_light_00].type	= EFF_LIGHT_00_TYPE_00;	// 種類を初期化
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * EFF_LIGHT_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEff_light_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++, pVtx += 4)
	{
		{
			D3DXVECTOR3 pos = g_aEff_light_00[nCntEff_light_00].pos;					// 位置
			D3DXVECTOR3 rot = g_aEff_light_00[nCntEff_light_00].rot;					// 向き
			float fWidth	= g_aEff_light_00[nCntEff_light_00].fWidth;					// 幅
			float fHeight	= g_aEff_light_00[nCntEff_light_00].fHeight;				// 高さ
			float fLength	= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
			float fAngle	= atan2f(fWidth, fHeight);									// 対角線の角度

			// 頂点座標を設定
			pVtx[0].pos.x = pos.x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[0].pos.y = pos.y + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
			pVtx[2].pos.x = pos.x + sinf(rot.z - fAngle) * fLength;
			pVtx[2].pos.y = pos.y + cosf(rot.z - fAngle) * fLength;
			pVtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
			pVtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
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

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	
	// 頂点座標をアンロックする
	g_pVtxBuffEff_light_00->Unlock();
}

//========================================
// UninitEff_light_00関数 - ライト[00]の終了処理 -
//========================================
void UninitEff_light_00(void)
{
	// テクスチャの破棄
	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_TYPE_MAX; nCntEff_light_00++)
	{
		if (g_aTextureEff_light_00[nCntEff_light_00] != NULL)
		{
			g_aTextureEff_light_00[nCntEff_light_00]->Release();
			g_aTextureEff_light_00[nCntEff_light_00] = NULL;
		}
	}
	
	// 頂点バッファの破棄
	if (g_pVtxBuffEff_light_00 != NULL) 
	{
		g_pVtxBuffEff_light_00->Release();
		g_pVtxBuffEff_light_00 = NULL;
	}
}

//========================================
// UpdateEff_light_00関数 - ライト[00]の更新処理 -
//========================================
void UpdateEff_light_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	POINT vertexPos[4];			// 四角形の頂点の座標
	
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++, pVtx += 4)
	{
		if (g_aEff_light_00[nCntEff_light_00].bUse)
		{// ライト[00]が使用されている時

			// 寿命を減算する
			g_aEff_light_00[nCntEff_light_00].nLife--;

			if (g_aEff_light_00[nCntEff_light_00].nLife <= 0) 
			{// 寿命が尽きた時、
				// 使用してない状態にする
				g_aEff_light_00[nCntEff_light_00].bUse = false;
			}

			D3DXVECTOR3 *move	= &g_aEff_light_00[nCntEff_light_00].move;		// 移動量
			D3DXVECTOR3 rot		= g_aEff_light_00[nCntEff_light_00].rot;		// 向き
			float fMove			= g_aEff_light_00[nCntEff_light_00].fMove;		// 移動力
			float fWidth		= g_aEff_light_00[nCntEff_light_00].fWidth;		// 幅
			float fHeight		= g_aEff_light_00[nCntEff_light_00].fHeight;	// 高さ

			// 状態処理
			for (int nCntState = 0; nCntState < EFF_LIGHT_00_STATE_MAX; nCntState++)
			{
				if (g_aEff_light_00[nCntEff_light_00].aState[nCntState])
				{// その状態が真の時

					switch (nCntState)
					{
					
					}
				}
			}

			// 位置/大きさを更新
			g_aEff_light_00[nCntEff_light_00].pos.x	+= move->x;
			g_aEff_light_00[nCntEff_light_00].pos.y	+= move->y;

			// 移動量を更新(減衰)
			move->x += (0.0f - move->x) * 0.1;
			move->y += (0.0f - move->y) * 0.1;

			{
				D3DXVECTOR3 drawPos = g_aEff_light_00[nCntEff_light_00].pos;					// 描画位置
				float fLength		= sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
				float fAngle		= atan2f(fWidth, fHeight);									// 対角線の角度

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
				int nR = g_aEff_light_00[nCntEff_light_00].col.r;	// 赤の強さ
				int nG = g_aEff_light_00[nCntEff_light_00].col.g;	// 緑の強さ
				int nB = g_aEff_light_00[nCntEff_light_00].col.b;	// 青の強さ
				int nA = g_aEff_light_00[nCntEff_light_00].col.a;	// 透明度

				// 透明度を寿命に応じて設定
				nA *= ((float)g_aEff_light_00[nCntEff_light_00].nLife / (float)g_aEff_light_00[nCntEff_light_00].nLifeMax);

				// 頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(nR, nG, nB, nA);
				pVtx[1].col = D3DCOLOR_RGBA(nR, nG, nB, nA);
				pVtx[2].col = D3DCOLOR_RGBA(nR, nG, nB, nA);
				pVtx[3].col = D3DCOLOR_RGBA(nR, nG, nB, nA);
			}
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffEff_light_00->Unlock();
}

//========================================
// DrawEff_light_00関数 - ライト[00]の描画処理 -
//========================================
void DrawEff_light_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEff_light_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++)
	{
		if (g_aEff_light_00[nCntEff_light_00].bUse)
		{// ライト[00]が使用されている時

			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureEff_light_00[g_aEff_light_00[nCntEff_light_00].type]);

			// ライト[00]の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEff_light_00 * 4, 2);
		}
	}

	// アルファブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
// SetEff_light_00関数 - ライト[00]の設定処理 -
//========================================
void SetEff_light_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fMove, Color col, float fWidth, float fHeight, int nLife, EFF_LIGHT_00_TYPE type)
{
	VERTEX_2D *pVtx;		// 頂点情報へのポインタ
	int nCntEff_light_00;	// ライト[00] のカウント

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEff_light_00->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEff_light_00 = 0; nCntEff_light_00 < EFF_LIGHT_00_MAX; nCntEff_light_00++)
	{
		if (g_aEff_light_00[nCntEff_light_00].bUse == false)
		{// ライト[00]が使用されていない時

			// 寿命を設定
			g_aEff_light_00[nCntEff_light_00].nLife = nLife;
			g_aEff_light_00[nCntEff_light_00].nLifeMax = nLife;

			// 位置/角度を代入
			g_aEff_light_00[nCntEff_light_00].pos = pos;
			g_aEff_light_00[nCntEff_light_00].rot = rot;

			// 移動量を設定
			g_aEff_light_00[nCntEff_light_00].move.x = sinf(rot.z) * fMove;
			g_aEff_light_00[nCntEff_light_00].move.y = cosf(rot.z) * fMove;

			// 種類を代入
			g_aEff_light_00[nCntEff_light_00].type = type;

			// 色を代入
			g_aEff_light_00[nCntEff_light_00].col = col;

			// 幅と高さを設定
			g_aEff_light_00[nCntEff_light_00].fWidth = fWidth;
			g_aEff_light_00[nCntEff_light_00].fHeight = fHeight;

			for (int nCntState = 0; nCntState < EFF_LIGHT_00_STATE_MAX; nCntState++)
			{
				// 状態と状態カウンターを初期化
				g_aEff_light_00[nCntEff_light_00].aState[nCntState] = false;
				g_aEff_light_00[nCntEff_light_00].aCounterState[nCntState] = 0;
			}

			{
				D3DXVECTOR3 pos = g_aEff_light_00[nCntEff_light_00].pos;					// 位置
				D3DXVECTOR3 rot = g_aEff_light_00[nCntEff_light_00].rot;					// 向き
				float fWidth = g_aEff_light_00[nCntEff_light_00].fWidth;					// 幅
				float fHeight = g_aEff_light_00[nCntEff_light_00].fHeight;				// 高さ
				float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) * 0.5f;	// 対角線の長さ
				float fAngle = atan2f(fWidth, fHeight);									// 対角線の角度

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

			// 使用している状態にする
			g_aEff_light_00[nCntEff_light_00].bUse = true;

			break;
		}
	}

	// 頂点座標をアンロックする
	g_pVtxBuffEff_light_00->Unlock();
}
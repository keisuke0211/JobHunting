//========================================
// 
// テキストの処理
// Author:西村 吏功
// 
//========================================
//  *** text.cpp ***
//========================================
#include "main.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// マクロ定義
//****************************************
#define TEXT_DIGIT_MAX	(128)	// テキストの最大文字数

//****************************************
// 構造体の定義
//****************************************

// テキスト情報の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	bool bUse;				// 使われているかフラグ
	char aString[TXT_MAX];	// 表示する文字列
	TEXT_MODE mode;			// モード
	FONT font;				// フォント
}Txt_00;

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9 g_pTextureTxt_00[FONT_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 pVtxBuffTxt_00 = NULL;		// 頂点バッファへのポインタ
Txt_00 g_text[TEXT_MAX];							// テキストの情報

// フォントの情報
Font g_aFont[FONT_MAX] =
{
	{ "data\\Texture\\txt_font_000.png"		,PIXEL * 8	,PIXEL * 8	,PIXEL * 8	,PIXEL * 8	, '!' , 8 , 8 },	// フォント[00]
	{ "data\\Texture\\txt_font_001.png"		,PIXEL * 16	,PIXEL * 16 ,PIXEL * 16 ,PIXEL * 16	, '!' , 8 , 8 },	// フォント[01]
	{ "data\\Texture\\txt_font_002.png"		,PIXEL * 4	,PIXEL * 8  ,PIXEL * 4  ,PIXEL * 8	, '!' , 8 , 8 },	// フォント[02]
	{ "data\\Texture\\txt_font_003.png"		,PIXEL * 16	,PIXEL * 16 ,PIXEL * 16 ,PIXEL * 16 , '!' , 8 , 8 },	// フォント[03]
	{ "data\\Texture\\txt_number_000.png"	,PIXEL * 64	,PIXEL * 64 ,PIXEL * 64 ,PIXEL * 64 , '0' , 8 , 2 },	// フォント[04]
};

// テキストの使用領域の値
int g_aTextField[TEXT_FIELD_MAX] = { 20,88,20 };

//========== *** フォントの情報を取得 ***
Font *GetFont(void) 
{
	return g_aFont;
}

//========================================
// InitTxt_00関数 - テキストの初期化処理 -
//========================================
void InitTxt_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	POINT vertexPos[4];			// 四角形の頂点の座標
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	
	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++) 
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, g_aFont[nCntFont].aPath, &g_pTextureTxt_00[nCntFont]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TEXT_MAX * TEXT_DIGIT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuffTxt_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuffTxt_00->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt_00 = 0; nCntTxt_00 < TEXT_MAX; nCntTxt_00++) 
	{
		g_text[nCntTxt_00].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置を初期化する
		g_text[nCntTxt_00].bUse = false;						// 使用されていない状態にする
		sprintf(g_text[nCntTxt_00].aString, "0");				// 表示する文字列を初期化

		for (int nCntDigit = 0; nCntDigit < TEXT_DIGIT_MAX; nCntDigit++, pVtx += 4)
		{
			float fWidth	= g_aFont[0].fWidth;	// 幅
			float fHeight	= g_aFont[0].fHeight;	// 高さ
			float fSpaceX	= g_aFont[0].fSpaceX;	// 配置間隔X
			float fSpaceY	= g_aFont[0].fSpaceY;	// 配置間隔Y

			// 頂点の座標を代入
			vertexPos[0].x = g_text[nCntTxt_00].pos.x - (fWidth * 0.5f) + (nCntDigit * fSpaceX);
			vertexPos[0].y = g_text[nCntTxt_00].pos.y - fHeight * 0.5f;
			vertexPos[1].x = g_text[nCntTxt_00].pos.x + (fWidth * 0.5f) + (nCntDigit * fSpaceX);
			vertexPos[1].y = g_text[nCntTxt_00].pos.y - fHeight * 0.5f;
			vertexPos[2].x = g_text[nCntTxt_00].pos.x - (fWidth * 0.5f) + (nCntDigit * fSpaceX);
			vertexPos[2].y = g_text[nCntTxt_00].pos.y + fHeight * 0.5f;
			vertexPos[3].x = g_text[nCntTxt_00].pos.x + (fWidth * 0.5f) + (nCntDigit * fSpaceX);
			vertexPos[3].y = g_text[nCntTxt_00].pos.y + fHeight * 0.5f;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(vertexPos[0].x, vertexPos[0].y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(vertexPos[1].x, vertexPos[1].y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(vertexPos[2].x, vertexPos[2].y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(vertexPos[3].x, vertexPos[3].y, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2((g_text[nCntTxt_00].aString[0] % g_aFont[0].nPtnMaxX) * (1.0f / g_aFont[0].nPtnMaxX), ((g_text[nCntTxt_00].aString[0] / g_aFont[0].nPtnMaxX)) * (1.0f / g_aFont[0].nPtnMaxY));
			pVtx[1].tex = D3DXVECTOR2((g_text[nCntTxt_00].aString[0] % g_aFont[0].nPtnMaxX) * (1.0f / g_aFont[0].nPtnMaxX) + (1.0f / g_aFont[0].nPtnMaxX), ((g_text[nCntTxt_00].aString[0] / g_aFont[0].nPtnMaxX)) * (1.0f / g_aFont[0].nPtnMaxY));
			pVtx[2].tex = D3DXVECTOR2((g_text[nCntTxt_00].aString[0] % g_aFont[0].nPtnMaxX) * (1.0f / g_aFont[0].nPtnMaxX), (((g_text[nCntTxt_00].aString[0] / g_aFont[0].nPtnMaxX)) * (1.0f / g_aFont[0].nPtnMaxY)) + (1.0f / g_aFont[0].nPtnMaxY));
			pVtx[3].tex = D3DXVECTOR2((g_text[nCntTxt_00].aString[0] % g_aFont[0].nPtnMaxX) * (1.0f / g_aFont[0].nPtnMaxX) + (1.0f / g_aFont[0].nPtnMaxX), (((g_text[nCntTxt_00].aString[0] / g_aFont[0].nPtnMaxX)) * (1.0f / g_aFont[0].nPtnMaxY)) + (1.0f / g_aFont[0].nPtnMaxY));
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuffTxt_00->Unlock();
}

//========================================
// UninitTxt_00関数 - テキストの終了処理 -
//========================================
void UninitTxt_00(void)
{
	int nCntTxt_00;	// テキストのカウント
	int nCntDigit;	// 桁のカウント

	for (int nCntFont = 0; nCntFont < FONT_MAX; nCntFont++)
	{
		// テクスチャの破棄
		if (g_pTextureTxt_00[nCntFont] != NULL)
		{
			g_pTextureTxt_00[nCntFont]->Release();
			g_pTextureTxt_00[nCntFont] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (pVtxBuffTxt_00 != NULL)
	{
		pVtxBuffTxt_00->Release();
		pVtxBuffTxt_00 = NULL;
	}
}

//========================================
// UpdateTxt_00関数 - テキストの更新処理 -
//========================================
void UpdateTxt_00(TEXT_FIELD textField, int nNumber, char aString[TXT_MAX], D3DXVECTOR3 pos, Color col, TEXT_MODE mode, FONT font, float fTextWidth, float fTextHeight)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ
	int nPattern;				// パターンNo.
	int nStartNumber = 0;		// 開始番号

	for (int nCntField = 0; nCntField < textField; nCntField++)
	{
		// 開始番号に領域の値を加算
		nStartNumber += g_aTextField[nCntField];
	}

	// テキスト番号を合わせる
	nNumber += nStartNumber;

	// フォントを代入
	g_text[nNumber].font = font;

	// 表示する文字列を代入
	sprintf(g_text[nNumber].aString, "%s", aString);

	// 位置を代入
	g_text[nNumber].pos = pos;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuffTxt_00->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報のポインタを該当の位置まで進める
	pVtx += nNumber * TEXT_DIGIT_MAX * 4;

	for (int nCntDigit = 0; nCntDigit < strlen(aString); nCntDigit++, pVtx += 4)
	{
		D3DXVECTOR3 pos = g_text[nNumber].pos;					// 位置
		float fWidth	= g_aFont[font].fWidth * fTextWidth;	// 幅
		float fHeight	= g_aFont[font].fHeight * fTextHeight;	// 高さ
		float fSpaceX	= g_aFont[font].fSpaceX * fTextWidth;	// 配置間隔X
		float fSpaceY	= g_aFont[font].fSpaceY * fTextHeight;	// 配置間隔Y

		switch (mode)
		{
		case TEXT_MODE_LEFT: // 左詰め

			// 頂点の座標を代入
			pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth, pos.y - fHeight * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX), pos.y + fHeight * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth, pos.y + fHeight * 0.5f, 0.0f);

			break;
		case TEXT_MODE_RIGHT: // 右詰め

			// 頂点の座標を代入
			pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) - (strlen(aString) * fSpaceX), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth - (strlen(aString) * fSpaceX), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) - (strlen(aString) * fSpaceX), pos.y + fHeight * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth - (strlen(aString) * fSpaceX), pos.y + fHeight * 0.5f, 0.0f);

			break;
		case TEXT_MODE_CENTER: // 中心

			// 頂点の座標を代入
			pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) - (strlen(aString) * fSpaceX * 0.5f), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth - (strlen(aString) * fSpaceX * 0.5f), pos.y - fHeight * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) - (strlen(aString) * fSpaceX * 0.5f), pos.y + fHeight * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + (nCntDigit * fSpaceX) + fWidth - (strlen(aString) * fSpaceX * 0.5f), pos.y + fHeight * 0.5f, 0.0f);

			break;
		}

		// 頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
		pVtx[1].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
		pVtx[2].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);
		pVtx[3].col = D3DCOLOR_RGBA(col.nR, col.nG, col.nB, col.nA);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(((aString[nCntDigit] - g_aFont[font].cStartChar) % g_aFont[font].nPtnMaxX) * (1.0f / g_aFont[font].nPtnMaxX), 
			(((aString[nCntDigit] - g_aFont[font].cStartChar) / g_aFont[font].nPtnMaxX)) * (1.0f / g_aFont[font].nPtnMaxY));
		pVtx[1].tex = D3DXVECTOR2(((aString[nCntDigit] - g_aFont[font].cStartChar) % g_aFont[font].nPtnMaxX) * (1.0f / g_aFont[font].nPtnMaxX) + (1.0f / g_aFont[font].nPtnMaxX), 
			(((aString[nCntDigit] - g_aFont[font].cStartChar) / g_aFont[font].nPtnMaxX)) * (1.0f / g_aFont[font].nPtnMaxY));
		pVtx[2].tex = D3DXVECTOR2(((aString[nCntDigit] - g_aFont[font].cStartChar) % g_aFont[font].nPtnMaxX) * (1.0f / g_aFont[font].nPtnMaxX), 
			((((aString[nCntDigit] - g_aFont[font].cStartChar) / g_aFont[font].nPtnMaxX)) * (1.0f / g_aFont[font].nPtnMaxY)) + (1.0f / g_aFont[font].nPtnMaxY));
		pVtx[3].tex = D3DXVECTOR2(((aString[nCntDigit] - g_aFont[font].cStartChar) % g_aFont[font].nPtnMaxX) * (1.0f / g_aFont[font].nPtnMaxX) + (1.0f / g_aFont[font].nPtnMaxX), 
			((((aString[nCntDigit] - g_aFont[font].cStartChar) / g_aFont[font].nPtnMaxX)) * (1.0f / g_aFont[font].nPtnMaxY)) + (1.0f / g_aFont[font].nPtnMaxY));
	}

	// 頂点座標をアンロックする
	pVtxBuffTxt_00->Unlock();
}

//========================================
// DrawTxt_00関数 - テキストの描画処理 -
//========================================
void DrawTxt_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	int nCntTxt_00;				// テキストのカウント
	int nCntDigit;				// 桁のカウント
	char aString[TXT_MAX];		// テキストの文字列を格納

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, pVtxBuffTxt_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTxt_00 = 0; nCntTxt_00 < TEXT_MAX; nCntTxt_00++) 
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTxt_00[g_text[nCntTxt_00].font]);

		for (nCntDigit = 0; nCntDigit < TEXT_DIGIT_MAX; nCntDigit++) 
		{
			if ((g_text[nCntTxt_00].bUse && nCntDigit < strlen(g_text[nCntTxt_00].aString)) && g_text[nCntTxt_00].aString[nCntDigit] != ' ')
			{// 使用されている状態で、かつ桁のカウントが表示する値の桁数を下回っている時

				// テキストの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntTxt_00 * TEXT_DIGIT_MAX) + nCntDigit) * 4, 2);
			}
		}
	}
}

//========================================
// ShowTxt_00関数 - テキストの表示/非表示を切り替える -
//========================================
void ShowTxt_00(TEXT_FIELD textField, int nNumber, bool bShow)
{
	int nStartNumber = 0;	// 開始番号

	for (int nCntField = 0; nCntField < textField; nCntField++) 
	{
		// 開始番号に領域の値を加算
		nStartNumber += g_aTextField[nCntField];
	}

	// テキスト番号を合わせる
	nNumber += nStartNumber;

	if (g_text[nNumber].bUse != bShow)
	{
		g_text[nNumber].bUse = bShow;
	}
}
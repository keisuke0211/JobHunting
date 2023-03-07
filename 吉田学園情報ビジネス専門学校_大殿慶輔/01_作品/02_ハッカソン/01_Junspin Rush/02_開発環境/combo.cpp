//==========================================
//
//コンボ表示プログラム[combo.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "combo.h"

//マクロ
#define MAX_COMBOMASS				(64)	//コンボ塊の最大数
#define COMBO_1COLUMN_USEPOLY		(8)		//コンボ塊で使うポリゴン数（6～8）
#define COMBOPOLY_SIZE_WIDTH		(128)	//テクスチャサイズ幅（実際の4倍）
#define COMBOPOLY_SIZE_HEIGHT		(128)	//テクスチャサイズ高さ（実際の4倍）
#define COMBOPOLY_PATT_WIDTH		(8)		//テクスチャパターン幅
#define COMBOPOLY_PATT_HEIGHT		(8)		//テクスチャパターン高さ
#define MAX_COMBO					(999)	//最大コンボ数
#define COMBO_STR_NUM				(5)		//数字の後に表示するCOMBOの文字数

//コンボポリゴン構造体の定義
typedef struct
{
	D3DXCOLOR col;		//色
	bool bUse;			//使用しているか
} ComboPoly;

//コンボ塊構造体
typedef struct
{
	ComboPoly cpData[COMBO_1COLUMN_USEPOLY];	//コンボポリゴン
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 move;							//移動量
	int nLife;									//寿命
	bool bUse;									//使用しているか
} ComboMass;

LPDIRECT3DTEXTURE9 g_pTextureCombo;			//テクスチャポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffCombo;	//頂点バッファポインタ
ComboMass g_aComboMass[MAX_COMBOMASS];			//コンボ塊構造体
int g_nComboNum;							//コンボ数

//========================
//コンボ初期化処理
//========================
void InitCombo(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスのポインタ
	int nCntComboMass,nCntComboPoly;				//カウンタ

	pDevice = GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\txt_font_000.PNG",
		&g_pTextureCombo);

	//コンボ構造体の中身初期化
	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		g_aComboMass[nCntComboMass].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//位置
		g_aComboMass[nCntComboMass].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//移動量
		g_aComboMass[nCntComboMass].nLife = 0;													//寿命
		for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++)			//コンボポリゴン
		{
			g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
			g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse = false;								//使用しているか
		}
		g_aComboMass[nCntComboMass].bUse = false;														//使用しているか
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COMBOMASS * COMBO_1COLUMN_USEPOLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffCombo,
		NULL);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffCombo->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++, pVtx += 4)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//座標変換用係数設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	//頂点バッファをアンロック
	g_pVtxbuffCombo->Unlock();
}

//========================
//コンボ終了処理
//========================
void UninitCombo(void)
{
	//テクスチャの破棄
	if (g_pTextureCombo != NULL)
	{
		g_pTextureCombo->Release();
		g_pTextureCombo = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxbuffCombo != NULL)
	{
		g_pVtxbuffCombo->Release();
		g_pVtxbuffCombo = NULL;
	}
}

//========================
//コンボ更新処理
//========================
void UpdateCombo(void)
{
	int nCntComboMass, nCntComboPoly;		//カウンタ
	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffCombo->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		if (g_aComboMass[nCntComboMass].bUse)
		{//コンボが使用されている
			//位置移動
			g_aComboMass[nCntComboMass].pos -= g_aComboMass[nCntComboMass].move;

			for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++, pVtx += 4)
			{
				if (g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse == true)
				{
					//不透明度更新
					g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col.a -= 1.0f / (float)g_aComboMass[nCntComboMass].nLife;

					//座標更新
					pVtx[0].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * nCntComboPoly;
					pVtx[0].pos.y = g_aComboMass[nCntComboMass].pos.y;
					pVtx[1].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * (nCntComboPoly + 1);
					pVtx[1].pos.y = g_aComboMass[nCntComboMass].pos.y;
					pVtx[2].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * nCntComboPoly;
					pVtx[2].pos.y = g_aComboMass[nCntComboMass].pos.y + COMBOPOLY_SIZE_HEIGHT / COMBOPOLY_PATT_HEIGHT;
					pVtx[3].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * (nCntComboPoly + 1);
					pVtx[3].pos.y = g_aComboMass[nCntComboMass].pos.y + COMBOPOLY_SIZE_HEIGHT / COMBOPOLY_PATT_HEIGHT;

					//色（不透明度）更新
					pVtx[0].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
					pVtx[1].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
					pVtx[2].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
					pVtx[3].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
				}
			}
			
			//寿命が尽きたら消す
			g_aComboMass[nCntComboMass].nLife--;
			if (g_aComboMass[nCntComboMass].nLife <= 0)
			{
				g_aComboMass[nCntComboMass].bUse = false;
			}
		}
	}

	//頂点バッファをアンロック
	g_pVtxbuffCombo->Unlock();
}

//========================
//コンボ描画処理
//========================
void DrawCombo(void)
{
	int nCntComboMass, nCntComboPoly;				//カウンタ
	LPDIRECT3DDEVICE9 pDevice;						//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffCombo, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTextureCombo);

	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		if (g_aComboMass[nCntComboMass].bUse == true)
		{
			for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++)
			{
				if (g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse == true)
				{
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntComboMass * COMBO_1COLUMN_USEPOLY + nCntComboPoly) * 4, 2);
				}
			}
		}
	}
}

//========================
//コンボ設定処理
//========================
void SetCombo(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife)
{
	return;

	const int nStrNumCombo[COMBO_STR_NUM] = { PATT_NUM_C ,PATT_NUM_O ,PATT_NUM_M ,PATT_NUM_B ,PATT_NUM_O };
	int aTexUV[COMBO_1COLUMN_USEPOLY];					//文字格納
	int nCntComboMass, nCntComboPoly ,nCntComboStr;		//カウンタ

	//コンボ数を増やす
	g_nComboNum++;
	if (g_nComboNum > MAX_COMBO)
	{
		g_nComboNum = MAX_COMBO;
	}

	for (nCntComboMass = 0; nCntComboMass < MAX_COMBOMASS; nCntComboMass++)
	{
		if (!g_aComboMass[nCntComboMass].bUse)
		{
			//設定
			g_aComboMass[nCntComboMass].pos = pos;
			g_aComboMass[nCntComboMass].move = move;
			g_aComboMass[nCntComboMass].nLife = nLife;

			//頂点座標設定
			VERTEX_2D *pVtx;	//設定用ポインタ

			g_pVtxbuffCombo->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += 4 * nCntComboMass * COMBO_1COLUMN_USEPOLY;

			nCntComboPoly = 0;	//カウンタ初期化

			do 
			{
				//数字分割
				aTexUV[nCntComboMass] = g_nComboNum % (int)pow(10, (nCntComboPoly + 1)) / (int)pow(10, nCntComboPoly) + SCORE_PATT_OFFSET;
				g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse = true;
				nCntComboPoly++;
			} while (g_nComboNum / (int)pow(10, nCntComboPoly) != 0);

			//COMBO設定
			for (nCntComboStr = 0; nCntComboStr < COMBO_STR_NUM; nCntComboStr++, nCntComboPoly++)
			{
				aTexUV[nCntComboPoly] = nStrNumCombo[nCntComboStr];
				g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse = true;
			}

			for (nCntComboPoly = 0; nCntComboPoly < COMBO_1COLUMN_USEPOLY; nCntComboPoly++, pVtx += 4)
			{
				g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col = col;		//色変更
				//g_aComboMass[nCntComboMass].cpData[nCntComboPoly].bUse = true;		//使用している状態にする

																					//頂点座標
				pVtx[0].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * nCntComboPoly;
				pVtx[0].pos.y = g_aComboMass[nCntComboMass].pos.y;
				pVtx[1].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * (nCntComboPoly + 1);
				pVtx[1].pos.y = g_aComboMass[nCntComboMass].pos.y;
				pVtx[2].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * nCntComboPoly;
				pVtx[2].pos.y = g_aComboMass[nCntComboMass].pos.y + COMBOPOLY_SIZE_HEIGHT / COMBOPOLY_PATT_HEIGHT;
				pVtx[3].pos.x = g_aComboMass[nCntComboMass].pos.x + COMBOPOLY_SIZE_WIDTH / COMBOPOLY_PATT_WIDTH * (nCntComboPoly + 1);
				pVtx[3].pos.y = g_aComboMass[nCntComboMass].pos.y + COMBOPOLY_SIZE_HEIGHT / COMBOPOLY_PATT_HEIGHT;

				//頂点カラー
				pVtx[0].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
				pVtx[1].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
				pVtx[2].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;
				pVtx[3].col = g_aComboMass[nCntComboMass].cpData[nCntComboPoly].col;

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((float)(aTexUV[nCntComboPoly] % COMBOPOLY_PATT_WIDTH) / COMBOPOLY_PATT_WIDTH,
					(float)(aTexUV[nCntComboPoly] / COMBOPOLY_PATT_WIDTH) / COMBOPOLY_PATT_HEIGHT);
				pVtx[1].tex = D3DXVECTOR2((float)(aTexUV[nCntComboPoly] % COMBOPOLY_PATT_WIDTH + 1) / COMBOPOLY_PATT_WIDTH,
					(float)(aTexUV[nCntComboPoly] / COMBOPOLY_PATT_WIDTH) / COMBOPOLY_PATT_HEIGHT);
				pVtx[2].tex = D3DXVECTOR2((float)(aTexUV[nCntComboPoly] % COMBOPOLY_PATT_WIDTH) / COMBOPOLY_PATT_WIDTH,
					(float)(aTexUV[nCntComboPoly] / COMBOPOLY_PATT_WIDTH + 1) / COMBOPOLY_PATT_HEIGHT);
				pVtx[3].tex = D3DXVECTOR2((float)(aTexUV[nCntComboPoly] % COMBOPOLY_PATT_WIDTH + 1) / COMBOPOLY_PATT_WIDTH,
					(float)(aTexUV[nCntComboPoly] / COMBOPOLY_PATT_WIDTH + 1) / COMBOPOLY_PATT_HEIGHT);
			}

			g_pVtxbuffCombo->Unlock();
		}
		//使用していることにする
		g_aComboMass[nCntComboMass].bUse = true;

		//抜ける
		break;
	}
}

//========================
//コンボリセット
//========================
void ResetCombo(void)
{
	g_nComboNum = 0;
}
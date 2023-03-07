//◇====================◇
//｜　　　　　　　　　　｜
//｜　ポーズの処理      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include"pause.h"
#include"input.h"
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DTEXTURE9 g_pTexturePause[3] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;		//頂点バッファポインタ
PAUSE_MENU g_pauseMenu;								//ポーズメニュー

int g_PauseSelect;
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【ポーズ・処理】｜――――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇==============================◇
//｜　　　 ポーズの初期化処理　　 ｜
//◇==============================◇
void InitPause(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntPause;
	g_PauseSelect = 0;
	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチャの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\boss1.png",
		&g_pTexturePause[0]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy_a.png",
		&g_pTexturePause[1]);

	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Enemy_a.png",
		&g_pTexturePause[2]);

	//頂点バッファの生成                           ↓必要な頂点数
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void **)&pVtx, 0);

	//ポーズ情報の初期化処理
	for (nCntPause = 0; nCntPause < 3; nCntPause++)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(16.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(16.0f, 50.0f, 0.0f);
		//◇――――――――――――◇
		//｜　　　　rhwの設定　　　 ｜
		//◇――――――――――――◇
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//◇――――――――――――◇
		//｜　　　 頂点カラー 　　　｜
		//◇――――――――――――◇
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//◇――――――――――――◇
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //頂点データのポインタを４つ分進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffPause->Unlock();
}
//◇============================◇
//｜　　　 ポーズの終了処理　　 ｜
//◇============================◇
void UninitPause(void)
{
	int nCntPause;

	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (nCntPause = 0; nCntPause < 3; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//◇============================◇
//｜　　　 ポーズの更新処理　　 ｜
//◇============================◇
void UpdatePause(void)
{
	if (GetKeyboardTrigger(DIK_W) == true)
	{
		g_PauseSelect++;
	}

	else if (GetKeyboardTrigger(DIK_S) == true)
	{
		g_PauseSelect--;
	}
	
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//決定キーが押せれた

		//メニューに合わせてモードの切り替え
	}
}
//◇============================◇
//｜　　　 ポーズの描画処理　　 ｜
//◇============================◇
void DrawPause(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	int nCntPause;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	for (nCntPause = 0; nCntPause < 3; nCntPause++)
	{

		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);

	}
}
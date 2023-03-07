//◇======================◇
//｜　　　　　　　  　　　｜
//｜　ランキングの処理    ｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"ranking_bg.h"
#include"ranking_score.h"
#include"input.h"
#include"fade.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffRankingBG;		//背景の頂点バッフ

LPDIRECT3DTEXTURE9 g_apTextureRankingBG;			//スコアのテクスチャポインタ

int g_nTimerRanking;			//ランキング画面表示タイマー

int g_nCntTime3;				//一定時間経過で画面推移する処理に使う

//◇============================◇
//｜　 ランキングの初期化処理　 ｜
//◇============================◇
void InitRanking(void)
{
	g_nCntTime3 = 0;//タイマーを初期化
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//背景のテクスチャの読み込み

	D3DXCreateTextureFromFile(pDevice, ".\\data\\TEXTURE\\bg010.png", &g_apTextureRankingBG);

	//頂点バッファの生成(背景)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffRankingBG,
		NULL);
	

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffRankingBG->Lock(0, 0, (void **)&pVtx, 0);

		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
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

		//頂点バッファのアンロック
		g_pVtxbuffRankingBG->Unlock();

		//スコアの初期化処理
		InitRankingScore();
		//スコアの更新処理
		UpdateRankingScore();
}
//◇==========================◇
//｜　 ランキングの終了処理　 ｜
//◇==========================◇
void UninitRanking(void)
{

	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	
	//背景
	if (g_apTextureRankingBG != NULL)
	{
		g_apTextureRankingBG->Release();
		g_apTextureRankingBG = NULL;
	}

	
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇

	//背景
	if (g_pVtxbuffRankingBG != NULL)
	{
		g_pVtxbuffRankingBG->Release();
		g_pVtxbuffRankingBG = NULL;
	}

	//スコアの終了処理
	UninitRankingScore();

}
//◇==========================◇
//｜　 ランキングの更新処理　 ｜
//◇==========================◇
void UpdateRanking(void)
{
	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

	//画面推移（ランキング → タイトル）
	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{// 決定キー [ ENTERキー ] が押された

	 //モードの設定
		SetFade(MODE_TITLE);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_START && GetFade() == FADE_NONE)
	{
		//モードの設定
		SetFade(MODE_TITLE);
	}
	else if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_A&& GetFade() == FADE_NONE)
	{
		//モードの設定
		SetFade(MODE_TITLE);
	}
	else if (GetFade() == FADE_NONE && g_nCntTime3 == SWITCH_TIME)
	{//リザルト画面に来てから10秒たったらタイトル画面に切り替わる

	 //モードの設定
		SetFade(MODE_TITLE);
	}
	else if (GetFade() == FADE_NONE)
	{
		g_nCntTime3++;		//フェードが FADE_NONE の状態になったら1フレームずつカウントする
	}
}
//◇==========================◇
//｜　 ランキングの描画処理　 ｜
//◇==========================◇
void DraeRanking(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffRankingBG, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇

	pDevice->SetTexture(0, g_apTextureRankingBG);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//スコアの描画処理
	DrawRankingScore();
}
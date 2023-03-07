//◇============================◇
//｜　　  　　　　　	  　　　｜
//｜　チュートリアル画面の処理　｜
//｜　Author:大殿慶輔　			｜
//｜							｜
//◇============================◇
#include"tutorial.h"
#include"sound.h"
#include "fade.h"

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTutorial = NULL;							//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureTutorial [TUTORIAL_LABEL_MAX] = {};			//テクスチャへのポインタ

int g_nCntTime5;
int nType;
//****************************************
//テクスチャファイル名
//****************************************
const char *c_apFllenameTUTORIAL[TUTORIAL_LABEL_MAX] =
{
	"data/TEXTURE/チュートリアル画面1-1.png",
	"data/TEXTURE/チュートリアル画面1-2.png",
};
//◇======================================◇
//｜　　　 リザルト画面の初期化処理　　 　｜
//◇======================================◇
void InitTutorial(void)
{
	g_nCntTime5 = 0;				//タイマーを初期化
	nType = 0;
									//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチャの読み込み　｜
	//◇――――――――――――◇

	for (int nCntTexture = 0; nCntTexture < TUTORIAL_LABEL_MAX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFllenameTUTORIAL[nCntTexture],
			&g_pTextureTutorial[nCntTexture]);
	}
	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTutorial,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTutorial->Lock(0, 0, (void **)&pVtx, 0);

	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇
		//　　　　　　　　　　　　 x       y     z
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f); //右回りで設定する
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f); //左回りで設定する


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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//◇――――――――――――◇
		//｜　テクスチャ座標の設定　｜
		//◇――――――――――――◇

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		PlaySound(SOUND_LABEL_BGM000); //洞窟のBJM

}
//◇====================================◇
//｜　　　 リザルト画面の終了処理　　 　｜
//◇====================================◇
void UninitTutorial(void)
{
	//サウンドの停止
	StopSound();

	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (int nCntBlock = 0; nCntBlock < TUTORIAL_LABEL_MAX; nCntBlock++)
	{

		if (g_pTextureTutorial[nCntBlock] != NULL)
		{
			g_pTextureTutorial[nCntBlock]->Release();
			g_pTextureTutorial[nCntBlock] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTutorial != NULL)
	{
		g_pVtxbuffTutorial->Release();
		g_pVtxbuffTutorial = NULL;
	}
}
//◇====================================◇
//｜　　　 リザルト画面の更新処理　　 　｜
//◇====================================◇
void UpdateTutorial(void)
{
	//画面推移（リザルト1 → タイトル）

	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

	if (GetFade() == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_D) == true || GetKeyboardTrigger(DIK_RIGHT) == true || xInput->Gamepad.wButtons & XINPUT_GAMEPAD_START||
			xInput->Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{// 決定キー [ ENTERキー ] が押された

			if (nType < TUTORIAL_LABEL_DIR)
			{
				nType += 1;
			}
			else if (nType == TUTORIAL_LABEL_DIR)
			{
				//モードの設定
				SetFade(MODE_GAME);
			}
		}
		else if (GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_LEFT) == true)
		{// 決定キー [ Aキー ] が押された

			if (nType >= 1 && nType <= TUTORIAL_LABEL_DIR)
			{
				nType -= 1;
			}
		}
	}
}
//◇====================================◇
//｜　　　 リザルト画面の描画処理　　 　｜
//◇====================================◇
void DrawTutorial1(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffTutorial, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチュの設定　　｜
	//◇――――――――――――◇
	pDevice->SetTexture(0, g_pTextureTutorial[nType]);
	//◇――――――――――――◇
	//｜　　ポリゴンの描画　　　｜
	//◇――――――――――――◇

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
}
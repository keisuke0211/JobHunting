//◇======================◇
//｜　　  　　　　　　　　｜
//｜　タイトル画面の処理　｜
//｜　Author:大殿慶輔　   ｜
//｜                      ｜
//◇======================◇
#include"title.h"
#include "camera.h"
#include "floor.h"
#include "wall.h"
#include "torch.h"
#include "figure_model.h"
#include "enemy_model.h"
#include "torch_model.h"
#include "stone_pillar.h"
#include "jewel.h"
#include "effect.h"
#include "particle.h"
#include"sound.h"
#include "fade.h"
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffTitle = NULL;		//頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_MAX] = {}; //テクスチャ(4枚分)へのポインタ
D3DXCOLOR g_colorTitle;								//ポリゴン(フェード)の色

int g_nCnTFlash;									//点滅時間
bool bFlash;										//点滅切り替え
TITLE g_Title[TITLE_MAX];							//タイトルの情報
float g_aTexV1[TITLE_MAX];							//テクスチャ座標の開始位置(V値)

float length_x;
float length_y;
//****************************************
//テクスチャファイル名
//****************************************
const char *c_apFllenameTitle[TITLE_MAX] =
{
	"data/TEXTURE/ルドの迷宮.png",
	"data/TEXTURE/PressEnter.png",
};
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【タイトル画面・処理】｜―――――――――――――――――――――――――――――
//==============================================================================================================================================


//◇======================================◇
//｜　　　 タイトル画面の初期化処理　　 　｜
//◇======================================◇
void InitTitle(void)
{

	int nCntTitle;
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	for (int nCntTexture = 0; nCntTexture < TITLE_MAX; nCntTexture++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFllenameTitle[nCntTexture],
			&g_pTextureTitle[nCntTexture]);
	}

	//データの初期化
	for (nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		if (nCntTitle == 0)
		{

			g_Title[nCntTitle].pos = D3DXVECTOR3(0.0f, 50.0f, 150.0f);
			g_Title[nCntTitle].fHight = 80.0f;
			g_Title[nCntTitle].fWidth = 160.0f;
		}
		else if (nCntTitle == 1)
		{
			g_Title[nCntTitle].pos = D3DXVECTOR3(0.0f, -30.0f, 150.0f);
			g_Title[nCntTitle].fHight = 30.0f;
			g_Title[nCntTitle].fWidth = 120.0f;
		}
		g_Title[nCntTitle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Title[nCntTitle].bUse = true;
	}

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	g_nCnTFlash = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * TITLE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffTitle,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffTitle->Lock(0, 0, (void **)&pVtx, 0);

	bFlash = false;
	for (nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++, pVtx += 4)
	{
		//◇――――――――――――◇
		//｜ 　　頂点座標を設定　　 ｜
		//◇――――――――――――◇
		pVtx[0].pos = D3DXVECTOR3(-g_Title[nCntTitle].fWidth, +g_Title[nCntTitle].fHight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_Title[nCntTitle].fWidth, +g_Title[nCntTitle].fHight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_Title[nCntTitle].fWidth, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_Title[nCntTitle].fWidth, 0.0f, 0.0f);
		
		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, -0.0f);

		//◇――――――――――――◇
		//｜　　　 頂点カラー 　　　｜
		//◇――――――――――――◇
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//◇――――――――――――◇
		//｜　テクスチュ座標の設定　｜
		//◇――――――――――――◇

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.01f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.01f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	g_Title[TITLE_BG001].state = TITLESTATE_NORMAL;
	//頂点バッファのアンロック
	g_pVtxbuffTitle->Unlock();

	PlaySound(SOUND_LABEL_BGM000); //迷宮BJM

	//床
	InitFloor();
	//壁
	InitWall();
	//エネミー
	InitEnemyeModel();
	//モデル・置物
	InitFigureModel();
	//松明・モデル
	InitTorch_Model();
	//石柱
	InitPillar_Model();
	//松明
	InitTorch();
}
//◇====================================◇
//｜　　　 タイトル画面の終了処理　　 　｜
//◇====================================◇
void UninitTitle(void)
{
	int nCntTitle;

	//サウンドの停止
	StopSound();

	//◇――――――――――――◇
	//｜　　テクスチャの破棄　　｜
	//◇――――――――――――◇
	for (nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}
	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffTitle != NULL)
	{
		g_pVtxbuffTitle->Release();
		g_pVtxbuffTitle = NULL;
	}

	//床
	UninitFloor();
	//壁
	UninitWall();
	//松明
	UninitTorch();
	//エネミー
	UninitEnemyeModel();
	//モデル・置物
	UninitFigureModel();
	//松明・モデル
	UninitTorch_Model();
	//石柱
	UninitPillar_Model();
}
//◇====================================◇
//｜　　　 タイトル画面の更新処理　　 　｜
//◇====================================◇
void UpdateTitle(void)
{	

	XINPUT_STATE *xInput = GetXInputState();

	//開始する前のカメラの動き
	if (GetFade() == FADE_NONE)
	{
		Getcamara(0)->rot.z += 0.001f;

		Getcamara(0)->posR.x = g_Title[0].pos.x;
		Getcamara(0)->posR.z = g_Title[0].pos.z;

		Getcamara(0)->posV.x = Getcamara(0)->posR.x + sinf(D3DX_PI * (1.0f - Getcamara(0)->rot.z)) * 300;
		Getcamara(0)->posV.z = Getcamara(0)->posR.z + cosf(D3DX_PI * (1.0f - Getcamara(0)->rot.z)) * 300;
	}

	if (GetFade() == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || xInput->Gamepad.wButtons & XINPUT_GAMEPAD_START ||
			xInput->Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{// 決定キー [ ENTERキー ] が押された

			bFlash = true;
			//モードの設定

			SetFade(MODE_TUTORIAL);
		}
	}
	//点滅
	g_nCnTFlash++;

	if (g_nCnTFlash >= 30 && bFlash == false)
	{//点滅切り替え時間になった
		g_Title[TITLE_BG001].bUse = g_Title[TITLE_BG001].bUse ? false : true;
		g_nCnTFlash = 0;
	}
	else if (g_nCnTFlash >= 6 && bFlash == true)
	{
		g_Title[TITLE_BG001].bUse = g_Title[TITLE_BG001].bUse ? false : true;
		g_nCnTFlash = 0;
	}

	//エネミー
	UpdateEnemyeModel();
	//モデル・置物
	UpdateFigureModel();
	//松明・モデル
	UpdateTorch_Model();
	//石柱
	UpdatePillar_Model();
	//松明
	UpdatTorch();
	//床
	UpdateFloor();
	//壁
	UpdateWall();
}
//◇====================================◇
//｜　　　 タイトル画面の描画処理　　 　｜
//◇====================================◇
void DrawTitle(void)
{
	//床
	DrawFloor();
	//壁
	DrawWall();
	//松明
	DrawTorch();
	//エネミー
	DrawEnemyeModel();
	//モデル・置物
	DrawFigureModel();
	//松明・モデル
	DrawTorch_Model();
	//石柱
	DrawPillar_Model();

	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;				//計算用マトリックス
	D3DXMATRIX mtxView;					//ビューマトリックス取得用

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);//Zテストの設定,比較方法
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);//Zバッファ更新の有効無効設定,有効か無効

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);//アルファテストの有効無効設定、有効・無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);//アルファテストの設定,比較方法
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);//アルファテストの参照値設定（0～255）

	for (int nCntTitle = 0; nCntTitle < TITLE_MAX; nCntTitle++)
	{
		if (g_Title[nCntTitle].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Title[nCntTitle].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_Title[nCntTitle].mtxWorld, NULL, &mtxView);//逆行列を求める
			g_Title[nCntTitle].mtxWorld._41 = 0.0f;
			g_Title[nCntTitle].mtxWorld._42 = 0.0f;
			g_Title[nCntTitle].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Title[nCntTitle].pos.x, g_Title[nCntTitle].pos.y, g_Title[nCntTitle].pos.z);

			D3DXMatrixMultiply(&g_Title[nCntTitle].mtxWorld, &g_Title[nCntTitle].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Title[nCntTitle].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxbuffTitle, 0, sizeof(VERTEX_3D));


			//◇――――――――――――◇
			//｜頂点フォーマットの設定　｜
			//◇――――――――――――◇
			pDevice->SetFVF(FVF_VERTEX_3D);

			//◇――――――――――――◇
			//｜　　テクスチャの設定　　｜
			//◇――――――――――――◇

			pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

			//◇――――――――――――◇
			//｜　　ポリゴンの描画　　　｜
			//◇――――――――――――◇
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
		}
	}
	
	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);//Zテストの設定,比較方法
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);//Zバッファ更新の有効無効設定,有効か無効

	 //アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);//アルファテストの有効無効設定、有効・無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);//アルファテストの設定,比較方法
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);//アルファテストの参照値設定（0～255）
}
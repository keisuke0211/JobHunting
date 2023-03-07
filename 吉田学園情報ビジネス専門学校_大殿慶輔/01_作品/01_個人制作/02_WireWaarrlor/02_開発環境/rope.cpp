//◇====================◇
//｜　　　　　　　　　　｜
//｜　 ロープの処理　　 ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "lure.h"
#include "rope.h"
#include "camera.h"
#include "player.h"
//****************************************
// グローバル変数
//****************************************

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffRope = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureRope = NULL;			//テクスチャへのポインタ

Rope g_Rope;											//プレイヤーの情報
//****************************************
// ロープの初期化処理
//****************************************
void InitRope(void)
{
	//対角線の長さを算出する(p幅*p幅+p高さ*p高さ);
	g_Rope.Length = sqrtf(SIZE_LURE_WIDTH * SIZE_LURE_WIDTH + SIZE_LURE_HEIGHT * SIZE_LURE_HEIGHT);

	//対角線の角度を算出する(p幅,p高さ)
	g_Rope.Angle = atan2f(SIZE_LURE_WIDTH, SIZE_LURE_HEIGHT);

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Rope.png",
		&g_pTextureRope);

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	g_Rope.posA	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//現在の位置
	g_Rope.posB	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//現在の位置

	g_Rope.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//前回の位置
	g_Rope.rot	  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//向き

	g_Rope.bUse = false;												//使用されているかどうか
	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffRope,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffRope->Lock(0, 0, (void **)&pVtx, 0);
	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇	

	pVtx[0].pos.x = g_Rope.posB.x - SIZE_LURE_WIDTH;
	pVtx[0].pos.y = g_Rope.posB.y - SIZE_LURE_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Rope.posB.x + SIZE_LURE_WIDTH;
	pVtx[1].pos.y = g_Rope.posB.y - SIZE_LURE_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Rope.posA.x - SIZE_LURE_WIDTH;
	pVtx[2].pos.y = g_Rope.posA.y + SIZE_LURE_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Rope.posA.x + SIZE_LURE_WIDTH;
	pVtx[3].pos.y = g_Rope.posA.y + SIZE_LURE_HEIGHT;
	pVtx[3].pos.z = 0.0f;

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
	pVtx[2].tex = D3DXVECTOR2(0.0f, 50.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 50.0f);

	//頂点バッファのアンロック
	g_pVtxbuffRope->Unlock();
}
//****************************************
// ロープの終了処理
//****************************************
void UninitRope(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureRope != NULL)
	{
		g_pTextureRope->Release();
		g_pTextureRope = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffRope != NULL)
	{
		g_pVtxbuffRope->Release();
		g_pVtxbuffRope = NULL;
	}
}
//****************************************
// ロープの更新処理
//****************************************
void UpdateRope(void)
{
	Camera *pCamera = GetCamera();
	Player *pPlayer = Getplayer();
	Lure   *pLure = GetLure();
	if (g_Rope.bUse == true)
	{
		//設定用ポインタ
		VERTEX_2D *pVtx;

		//頂点バッファのロックと頂点情報へのポインタを取得
		g_pVtxbuffRope->Lock(0, 0, (void **)&pVtx, 0);

		g_Rope.posOld = g_Rope.posA;		//前回の位置を保存
		g_Rope.posA	  = D3DXVECTOR3(pPlayer->pos.x,pPlayer->pos.y + 10.0f, 0.0f);
		g_Rope.posB   = D3DXVECTOR3(pLure->pos.x,pLure->pos.y, 0.0f);

		//プレイヤーを中心に画面を動かす

		D3DXVECTOR3 drawPosA = g_Rope.posA;
		D3DXVECTOR3 drawPosB = g_Rope.posB;

		CameraPosition(&drawPosA);
		CameraPosition(&drawPosB);

		/*pVtx[0].pos.x = drawPosB.x - SIZE_LURE_WIDTH;
		pVtx[1].pos.x = drawPosB.x + SIZE_LURE_WIDTH;
		pVtx[2].pos.x = drawPosA.x - SIZE_LURE_WIDTH;
		pVtx[3].pos.x = drawPosA.x + SIZE_LURE_WIDTH;

		pVtx[0].pos.y = drawPosB.y - SIZE_LURE_HEIGHT;
		pVtx[1].pos.y = drawPosB.y - SIZE_LURE_HEIGHT;
		pVtx[2].pos.y = drawPosA.y + SIZE_LURE_HEIGHT;
		pVtx[3].pos.y = drawPosA.y + SIZE_LURE_HEIGHT;*/

		pVtx[0].pos.x = drawPosB.x + sinf(pLure->rot.z + (-D3DX_PI + g_Rope.Angle)) * g_Rope.Length;
		pVtx[0].pos.y = drawPosB.y + cosf(pLure->rot.z + (-D3DX_PI + g_Rope.Angle)) * g_Rope.Length;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = drawPosB.x + sinf(pLure->rot.z + (D3DX_PI - g_Rope.Angle)) * g_Rope.Length;
		pVtx[1].pos.y = drawPosB.y + cosf(pLure->rot.z + (D3DX_PI - g_Rope.Angle)) * g_Rope.Length;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = drawPosA.x + sinf(pLure->rot.z + (0 - g_Rope.Angle)) * g_Rope.Length;
		pVtx[2].pos.y = drawPosA.y + cosf(pLure->rot.z + (0 - g_Rope.Angle)) * g_Rope.Length;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = drawPosA.x + sinf(pLure->rot.z + (0 + g_Rope.Angle)) * g_Rope.Length;
		pVtx[3].pos.y = drawPosA.y + cosf(pLure->rot.z + (0 + g_Rope.Angle)) * g_Rope.Length;
		pVtx[3].pos.z = 0.0f;

		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.z = 0.0f;

		//頂点バッファのアンロック
		g_pVtxbuffRope->Unlock();
	}
}
//****************************************
// ロープの描画処理
//****************************************
void DrawRope(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffRope, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	if (g_Rope.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureRope);

		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		//頂点情報構造体のサイズ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
//****************************************
// ロープの情報の取得
//****************************************
Rope *GetRope(void)
{
	return &g_Rope;
}

//◇====================◇
//｜　　　　　　　　　　｜
//｜　 ルアーの処理　　 ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "lure.h"
#include "block.h"
#include "camera.h"
#include "player.h"
#include "rope.h"
//****************************************
// グローバル変数
//****************************************

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffLure = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureLure = NULL;			//テクスチャへのポインタ

Lure g_Lure;											//プレイヤーの情報

//****************************************
// ルアーの初期化処理
//****************************************
void InitLure(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//対角線の長さを算出する(p幅*p幅+p高さ*p高さ);
	g_Lure.Length = sqrtf(SIZE_LURE_WIDTH * SIZE_LURE_WIDTH + SIZE_LURE_HEIGHT * SIZE_LURE_HEIGHT);

	//対角線の角度を算出する(p幅,p高さ)
	g_Lure.Angle = atan2f(SIZE_LURE_WIDTH, SIZE_LURE_HEIGHT);

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Hook.png",
		&g_pTextureLure);

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	g_Lure.pos = D3DXVECTOR3(0.0f,0.0f,0.0f);							//現在の位置
	g_Lure.posOld = D3DXVECTOR3(0.0f,0.0f,0.0f);						//前回の位置
	g_Lure.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//移動量
	g_Lure.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//向き

	g_Lure.nCntReturn = 0;												//戻る時間
	g_Lure.nLife = 0;													//寿命

	g_Lure.bUse	 = false;												//使用されているかどうか
	g_Lure.bStab = false;												//壁に当たったかどうか
	g_Lure.bRope = false;
	g_Lure.bWireA = false;
	g_Lure.bReturn = false;
	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffLure,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffLure->Lock(0, 0, (void **)&pVtx, 0);
	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇	

	pVtx[0].pos.x = g_Lure.pos.x - SIZE_LURE_WIDTH;
	pVtx[0].pos.y = g_Lure.pos.y - SIZE_LURE_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Lure.pos.x + SIZE_LURE_WIDTH;
	pVtx[1].pos.y = g_Lure.pos.y - SIZE_LURE_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Lure.pos.x - SIZE_LURE_WIDTH;
	pVtx[2].pos.y = g_Lure.pos.y + SIZE_LURE_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Lure.pos.x + SIZE_LURE_WIDTH;
	pVtx[3].pos.y = g_Lure.pos.y + SIZE_LURE_HEIGHT;
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
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	g_pVtxbuffLure->Unlock();
}
//****************************************
// ルアーの終了処理
//****************************************
void UninitLure(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureLure != NULL)
	{
		g_pTextureLure->Release();
		g_pTextureLure = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffLure != NULL)
	{
		g_pVtxbuffLure->Release();
		g_pVtxbuffLure = NULL;
	}
}
//****************************************
// ルアーの更新処理
//****************************************
void UpdateLure(void)
{
	Camera *pCamera = GetCamera();
	Player *pPlayer = Getplayer();
	Rope   *pRope   = GetRope();

		if (g_Lure.bUse == true)
		{
			//設定用ポインタ
			VERTEX_2D *pVtx;

			//頂点バッファのロックと頂点情報へのポインタを取得
			g_pVtxbuffLure->Lock(0, 0, (void **)&pVtx, 0);

			//プレイヤーとフックの距離を調べる
			pPlayer->lengthFixed = sqrtf(pow(fabs(g_Lure.pos.x - pPlayer->posOld.x), 2)
				+ pow(fabs(g_Lure.pos.y - pPlayer->posOld.y), 2));

			g_Lure.posOld = g_Lure.pos;		//前回の位置を保存

			//弾の位置更新
			g_Lure.pos += g_Lure.move;

			//プレイヤーを中心に画面を動かす

			D3DXVECTOR3 drawPos = g_Lure.pos;
			CameraPosition(&drawPos);

			/*pVtx[0].pos.x = drawPos.x - SIZE_LURE_WIDTH;
			pVtx[1].pos.x = drawPos.x + SIZE_LURE_WIDTH;
			pVtx[2].pos.x = drawPos.x - SIZE_LURE_WIDTH;
			pVtx[3].pos.x = drawPos.x + SIZE_LURE_WIDTH;
			pVtx[0].pos.y = drawPos.y - SIZE_LURE_HEIGHT;
			pVtx[1].pos.y = drawPos.y - SIZE_LURE_HEIGHT;
			pVtx[2].pos.y = drawPos.y + SIZE_LURE_HEIGHT;
			pVtx[3].pos.y = drawPos.y + SIZE_LURE_HEIGHT;

			pVtx[0].pos.z = 0.0f;
			pVtx[1].pos.z = 0.0f;
			pVtx[2].pos.z = 0.0f;
			pVtx[3].pos.z = 0.0f;*/

			pVtx[0].pos.x = drawPos.x + sinf(g_Lure.rot.z + (-D3DX_PI + g_Lure.Angle)) * g_Lure.Length;
			pVtx[0].pos.y = drawPos.y + cosf(g_Lure.rot.z + (-D3DX_PI + g_Lure.Angle)) * g_Lure.Length;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = drawPos.x + sinf(g_Lure.rot.z + (D3DX_PI - g_Lure.Angle)) * g_Lure.Length;
			pVtx[1].pos.y = drawPos.y + cosf(g_Lure.rot.z +	(D3DX_PI - g_Lure.Angle)) * g_Lure.Length;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = drawPos.x + sinf(g_Lure.rot.z + (0 - g_Lure.Angle)) * g_Lure.Length;
			pVtx[2].pos.y = drawPos.y + cosf(g_Lure.rot.z + (0 - g_Lure.Angle)) * g_Lure.Length;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = drawPos.x + sinf(g_Lure.rot.z + (0 + g_Lure.Angle)) * g_Lure.Length;
			pVtx[3].pos.y = drawPos.y + cosf(g_Lure.rot.z + (0 + g_Lure.Angle)) * g_Lure.Length;
			pVtx[3].pos.z = 0.0f;

			if (g_Lure.bStab == false)
			{
				//糸の限界地を設定
				if (pPlayer->lengthFixed >= MAX_STRING)
				{
					pPlayer->lengthFixed = MAX_STRING;
					g_Lure.nLife = 0;
				}

				//寿命のカウントダウン
				g_Lure.nLife--;
				D3DXVECTOR3 movevec;

				if (g_Lure.nLife <= 0)
				{
					/*D3DXVECTOR3 drawPos1 = pPlayer->Draepos;
					CameraPosition(&drawPos1);*/

					//g_Lure.nCntReturn++;
					//g_Lure.pos = (g_Lure.pos * (1.0f - (float)(g_Lure.nCntReturn / (float)50)) + ((Getplayer()->pos) *(float)(g_Lure.nCntReturn / (float)50)));//カウントの前に(float)を入れる

					//if (pPlayer->pos.x >= g_Lure.pos.x - SIZE_LURE_WIDTH * 2 &&
					//	pPlayer->pos.x <= g_Lure.pos.x + SIZE_LURE_WIDTH * 2 &&
					//	pPlayer->pos.y >= g_Lure.pos.y - SIZE_LURE_HEIGHT * 2 &&
					//	pPlayer->pos.y <= g_Lure.pos.y + SIZE_LURE_HEIGHT * 2)
					//{
					//	g_Lure.bUse = false;
					//	pRope->bUse = false;
					//	g_Lure.nCntReturn = 0;
					//}
					if (g_Lure.bReturn == false)
					{
						g_Lure.bReturn = true;
					}
				}
			}

			if (g_Lure.bReturn == false)
			{
				//ブロックとの当たり判定
				if (CollisionBlockLure(&g_Lure.pos, &g_Lure.posOld, &g_Lure.move) == true)
				{
					g_Lure.bStab = true;
					g_Lure.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			//頂点バッファのアンロック
			g_pVtxbuffLure->Unlock();
		}
}
//****************************************
// ルアーの描画処理
//****************************************
void DrawLure(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffLure, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	if (g_Lure.bUse == true)
	{
		pDevice->SetTexture(0, g_pTextureLure);

		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		//頂点情報構造体のサイズ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
//****************************************
// ルアーの設定処理
//****************************************
void SetLure(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot)
{
	Rope *pRope = GetRope();
	if (g_Lure.bUse == false)
	{
		g_Lure.pos = pos;
		g_Lure.move = move;
		g_Lure.rot = rot;
		g_Lure.nLife = 30;
		g_Lure.bUse = true;
		g_Lure.bRope = false;
		g_Lure.bWireA = false;
		pRope->bUse = true;
	}
}
//****************************************
// ルアーの情報の取得
//****************************************
Lure *GetLure(void)
{
	return &g_Lure;
}

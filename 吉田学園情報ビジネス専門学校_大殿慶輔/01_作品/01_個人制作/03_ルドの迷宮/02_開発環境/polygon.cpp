//◇====================◇
//｜　　　　　　　　　　｜
//｜　ポリゴンの処理    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "polygon.h"

//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffPolygon = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTexturePolygon = NULL;			//テクスチャへのポインタ
D3DXVECTOR3 g_posPolygon;//位置
D3DXVECTOR3 g_rotPolygon;//向き
D3DXMATRIX g_mtxWorldPolygon;//ワールドマトリックス

//****************************************
// 初期化処理
//****************************************
void InitPolygon(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Rocks00.png",
		&g_pTexturePolygon);

	g_posPolygon = DEFAULT_D3DXVECTOR3;
	g_rotPolygon = DEFAULT_D3DXVECTOR3;
	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffPolygon,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffPolygon->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posPolygon.x - 250, g_posPolygon.y, g_posPolygon.z + 250);
	pVtx[1].pos = D3DXVECTOR3(g_posPolygon.x + 250, g_posPolygon.y, g_posPolygon.z + 250);
	pVtx[2].pos = D3DXVECTOR3(g_posPolygon.x - 250, g_posPolygon.y, g_posPolygon.z - 250);
	pVtx[3].pos = D3DXVECTOR3(g_posPolygon.x + 250, g_posPolygon.y, g_posPolygon.z - 250);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標のの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 10.0f);
	pVtx[3].tex = D3DXVECTOR2(10.0f, 10.0f);

	//頂点バッファのアンロック
	g_pVtxbuffPolygon->Unlock();
}
//****************************************
// 終了処理
//****************************************
void UninitPolygon(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffPolygon != NULL)
	{
		g_pVtxbuffPolygon->Release();
		g_pVtxbuffPolygon = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdatePolygon(void)
{

}
//****************************************
// 描画処理
//****************************************
void DrawPolygon(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y,
		g_rotPolygon.x, g_rotPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);

	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffPolygon, 0, sizeof(VERTEX_3D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_3D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	
	pDevice->SetTexture(0, g_pTexturePolygon);

	//◇――――――――――――◇
	//｜　　ポリゴンの描画　　　｜
	//◇――――――――――――◇

	//頂点情報構造体のサイズ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
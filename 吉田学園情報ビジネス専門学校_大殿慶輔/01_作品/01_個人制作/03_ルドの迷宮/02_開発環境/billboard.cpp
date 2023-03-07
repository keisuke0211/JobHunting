//◇====================◇
//｜　　　　　　　　　　｜
//｜　ビルボードの処理  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "billboard.h"
//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffBillboard = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureBillboard = NULL;			//テクスチャへのポインタ
D3DXVECTOR3 g_posBillboard;//位置
D3DXMATRIX g_mtxWorldBillboard;//ワールドマトリックス

//****************************************
// 初期化処理
//****************************************
void InitBilldoard(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\Rocks00.png",
		&g_pTextureBillboard);

	g_posBillboard = D3DXVECTOR3(50, 50, 50);
	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffBillboard,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posBillboard.x - 50, g_posBillboard.y+50, g_posBillboard.z);
	pVtx[1].pos = D3DXVECTOR3(g_posBillboard.x + 50, g_posBillboard.y+50, g_posBillboard.z);
	pVtx[2].pos = D3DXVECTOR3(g_posBillboard.x - 50, g_posBillboard.y-50, g_posBillboard.z);
	pVtx[3].pos = D3DXVECTOR3(g_posBillboard.x + 50, g_posBillboard.y-50, g_posBillboard.z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標のの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファのアンロック
	g_pVtxbuffBillboard->Unlock();
}
//****************************************
// 終了処理
//****************************************
void UninitBilldoard(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffBillboard != NULL)
	{
		g_pVtxbuffBillboard->Release();
		g_pVtxbuffBillboard = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdateBilldoard(void)
{

}
//****************************************
// 描画処理
//****************************************
void DrawBilldoard(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;				//計算用マトリックス
	D3DXMATRIX mtxView;					//ビューマトリックス取得用

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);//Zテストの設定,比較方法
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);//Zバッファ更新の有効無効設定,有効か無効

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);//アルファテストの有効無効設定、有効・無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);//アルファテストの設定,比較方法
	pDevice->SetRenderState(D3DRS_ALPHAREF,0);//アルファテストの参照値設定（0～255）

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBillboard);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);//逆行列を求める
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);

	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffBillboard, 0, sizeof(VERTEX_3D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_3D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇

	pDevice->SetTexture(0, g_pTextureBillboard);

	//◇――――――――――――◇
	//｜　　ポリゴンの描画　　　｜
	//◇――――――――――――◇

	//頂点情報構造体のサイズ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);//Zテストの設定,比較方法
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);//Zバッファ更新の有効無効設定,有効か無効

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);//アルファテストの有効無効設定、有効・無効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_ALWAYS);//アルファテストの設定,比較方法
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);//アルファテストの参照値設定（0～255）
}
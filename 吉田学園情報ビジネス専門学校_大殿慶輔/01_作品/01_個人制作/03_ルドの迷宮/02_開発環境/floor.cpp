//◇====================◇
//｜　　　　　　　　　　｜
//｜　床の処理		    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "floor.h"
//****************************************
// グローバル変数
//****************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffFloor = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureFloor = NULL;			//テクスチャへのポインタ
Floor g_floor[MAX_FLOOR];
float g_GroundMoveX1;
float g_GroundMoveY1;
//****************************************
// 初期化処理
//****************************************
void InitFloor(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\floor002.png",
		&g_pTextureFloor);

	LoadFloor();

	for (int nCntFloar = 0; nCntFloar < MAX_FLOOR; nCntFloar++)
	{
		if (g_floor[nCntFloar].nCype == 0)
		{
			g_floor[nCntFloar].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		else if (g_floor[nCntFloar].nCype == 1)
		{
			g_floor[nCntFloar].rot = D3DXVECTOR3(0.0f, 0.0f, 3.141592f);
		}

		//テクスチャの分割数を計算
		g_floor[nCntFloar].nSplitX = g_floor[nCntFloar].fWidth / 25;
		g_floor[nCntFloar].nSplitY = g_floor[nCntFloar].fHight / 25;
	}
	//◇――――――――――――◇	
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FLOOR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffFloor,
		NULL);

	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffFloor->Lock(0, 0, (void **)&pVtx, 0);

	for (int nCntFloor = 0; nCntFloor < MAX_FLOOR; nCntFloor++, pVtx += 4)
	{
		if (g_floor[nCntFloor].nCype == 0)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_floor[nCntFloor].fWidth, g_floor[nCntFloor].pos.y, +g_floor[nCntFloor].fHight);
			pVtx[1].pos = D3DXVECTOR3(+g_floor[nCntFloor].fWidth, g_floor[nCntFloor].pos.y, +g_floor[nCntFloor].fHight);
			pVtx[2].pos = D3DXVECTOR3(-g_floor[nCntFloor].fWidth, g_floor[nCntFloor].pos.y, -g_floor[nCntFloor].fHight);
			pVtx[3].pos = D3DXVECTOR3(+g_floor[nCntFloor].fWidth, g_floor[nCntFloor].pos.y, -g_floor[nCntFloor].fHight);
		}
		else if (g_floor[nCntFloor].nCype == 1)
		{
			pVtx[0].pos = D3DXVECTOR3(-g_floor[nCntFloor].fWidth, -g_floor[nCntFloor].pos.y, +g_floor[nCntFloor].fHight);
			pVtx[1].pos = D3DXVECTOR3(+g_floor[nCntFloor].fWidth, -g_floor[nCntFloor].pos.y, +g_floor[nCntFloor].fHight);
			pVtx[2].pos = D3DXVECTOR3(-g_floor[nCntFloor].fWidth, -g_floor[nCntFloor].pos.y, -g_floor[nCntFloor].fHight);
			pVtx[3].pos = D3DXVECTOR3(+g_floor[nCntFloor].fWidth, -g_floor[nCntFloor].pos.y, -g_floor[nCntFloor].fHight);
		}
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
		pVtx[1].tex = D3DXVECTOR2(0.0f + g_floor[nCntFloor].nSplitX, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f + g_floor[nCntFloor].nSplitY);
		pVtx[3].tex = D3DXVECTOR2(0.0f + g_floor[nCntFloor].nSplitX, 0.0f + g_floor[nCntFloor].nSplitY);
	}
	//頂点バッファのアンロック
	g_pVtxbuffFloor->Unlock();
}
//****************************************
// 終了処理
//****************************************
void UninitFloor(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTextureFloor != NULL)
	{
		g_pTextureFloor->Release();
		g_pTextureFloor = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffFloor != NULL)
	{
		g_pVtxbuffFloor->Release();
		g_pVtxbuffFloor = NULL;
	}
}
//****************************************
// 更新処理
//****************************************
void UpdateFloor(void)
{
	//設定用ポインタ
	VERTEX_3D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffFloor->Lock(0, 0, (void **)&pVtx, 0);

	g_GroundMoveX1 += 1.0f;
	g_GroundMoveY1 += 1.0f;

	for (int nCntFloor = 0; nCntFloor < MAX_FLOOR; nCntFloor++, pVtx += 4)
	{
		pVtx[0].tex.x = g_GroundMoveX1;
		pVtx[1].tex.x = g_GroundMoveX1 + g_floor[nCntFloor].nSplitX;
		pVtx[2].tex.x = g_GroundMoveX1;
		pVtx[3].tex.x = g_GroundMoveX1 + g_floor[nCntFloor].nSplitX;

		pVtx[0].tex.y = g_GroundMoveY1;
		pVtx[1].tex.y = g_GroundMoveY1;
		pVtx[2].tex.y = g_GroundMoveY1 + g_floor[nCntFloor].nSplitY;
		pVtx[3].tex.y = g_GroundMoveY1 + g_floor[nCntFloor].nSplitY;
	}

	//頂点バッファのアンロック
	g_pVtxbuffFloor->Unlock();
}
//****************************************
// 描画処理
//****************************************
void DrawFloor(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntFloor = 0; nCntFloor < MAX_FLOOR; nCntFloor++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_floor[nCntFloor].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_floor[nCntFloor].rot.y,g_floor[nCntFloor].rot.x, g_floor[nCntFloor].rot.z);

		D3DXMatrixMultiply(&g_floor[nCntFloor].mtxWorld, &g_floor[nCntFloor].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_floor[nCntFloor].pos.x, g_floor[nCntFloor].pos.y, g_floor[nCntFloor].pos.z);

		D3DXMatrixMultiply(&g_floor[nCntFloor].mtxWorld, &g_floor[nCntFloor].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_floor[nCntFloor].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxbuffFloor, 0, sizeof(VERTEX_3D));

		//◇――――――――――――◇
		//｜頂点フォーマットの設定　｜
		//◇――――――――――――◇
		pDevice->SetFVF(FVF_VERTEX_3D);


		//◇――――――――――――◇
		//｜　　テクスチャの設定　　｜
		//◇――――――――――――◇

		pDevice->SetTexture(0, g_pTextureFloor);

		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		//頂点情報構造体のサイズ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFloor * 4, 2);
	}
}
//****************************************
// 設定処理
//****************************************
void LoadFloor(void)
{
	int c = 0;	   //1文字ずつ確認する変数
	int column = 1;//列数を数える変数
	int row = 0;//行数を数える変数

	char aData[300];//つなげる文字数
	FILE *pFile;

	memset(aData, 0, sizeof(aData));

	//ファイルを開く
	pFile = fopen("data/csv/FloorData.csv", "r");

	//ファイルから１文字ずつ読み込む
	while (fgetc(pFile) != '\n');
	
	while (c != EOF)
	{
		//１セル分の文字列を作る
		while (1)
		{
			c = fgetc(pFile);

			//末尾ならループを抜ける
			if (c == EOF)
				break;

			//カンマか改行でなければ、文字としてつなげる
			if (c != ',' && c != '\n')
				strcat(aData, (const char*)&c);

			//カンマが改行ならループ抜ける
			else
				break;
		}

		switch (column)
		{
			//atoi関数で数値として代入
		case 1:		g_floor[row].nCype  = atoi(aData);			break;	//１列目：種類
		case 2:		g_floor[row].pos.x  = (float)(atoi(aData));	break;	//　列目：X座標
		case 3:		g_floor[row].pos.y  = (float)(atoi(aData));	break;	//　列目：Y座標
		case 4:		g_floor[row].pos.z  = (float)(atoi(aData));	break;	//　列目：Z座標
		case 5:		g_floor[row].fHight = (float)(atoi(aData));	break;	//　列目：高さ
		case 6:		g_floor[row].fWidth = (float)(atoi(aData));	break;	//　列目：横幅
		}
		//バッファを初期化
		memset(aData, 0, sizeof(aData));

		//列数を足す
		column++;

		//もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
		if (c == '\n')
		{
			column = 1;
			row++;
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}
//****************************************
// 情報の取得
//****************************************
Floor *GetFloor(void)
{
	return &g_floor[0];
}

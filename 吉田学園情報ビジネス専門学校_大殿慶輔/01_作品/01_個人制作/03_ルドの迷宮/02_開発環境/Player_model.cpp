//◇====================◇
//｜　　　　　　　　　　｜
//｜　モデル処理        ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "Player_model.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "wall.h"
#include "effect.h"
#include "torch.h"
#include "figure_model.h"
#include "stone_pillar.h"
#include "meshfield_wall.h"
#include "enemy_model.h"
#include "jewel.h"
#include "sound.h"
//****************************************
// グローバル変数
//****************************************
LPD3DXMESH g_pMeshHammerModel = NULL;						//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER g_pBuffMatHammerModel = NULL;					//マテリアルへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureHammerModel[2] = {};			//テクスチャへのポインタ
DWORD g_dwNumHammerMatModel = 0;							//マテリアルの数
Player		g_player;										//プレイヤーの情報
//****************************************
// モデルの初期化処理
//****************************************
void InitPlayerModel(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_player.pos = D3DXVECTOR3(Getcamara(0)->posV.x,5.0f, Getcamara(0)->posV.z); //x,y,z+250
	g_player.posOld = DEFAULT_D3DXVECTOR3;
	g_player.move = DEFAULT_D3DXVECTOR3;
	g_player.vecMove = DEFAULT_D3DXVECTOR3;
	g_player.relativePosR = D3DXVECTOR3(0.0f,15.0f,0.0f);
	g_player.targetPosR = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 3.125f, 0.0f);
	g_player.nCntAction = 30;
	g_player.fWidht = 25.0f;
	g_player.bAction = true;
	//Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/Luna.x",/*読み込むファイル名*/
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatHammerModel,
		NULL,
		&g_dwNumHammerMatModel,
		&g_pMeshHammerModel);

	D3DXMATERIAL *pMat;							//マテリアルへのポインタ

	//マテリアル情報に対するポインタを所得
	pMat = (D3DXMATERIAL*)g_pBuffMatHammerModel->GetBufferPointer();

	for (int nCntMat = 0;nCntMat < (int)g_dwNumHammerMatModel;nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する

			//このファイル名を使用してテクスチャを読み込む
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureHammerModel[nCntMat]);
		}
	}

	//影の設定
	g_player.nIdxShadow = SetShadow();
	//g_player.nIdxShadow = SetTorch();
}
//****************************************
// モデルの終了処理
//****************************************
void UninitPlayerModel(void)
{
	//サウンドの停止
	StopSound();

	//メッシュの破棄
	if (g_pMeshHammerModel != NULL)
	{
		g_pMeshHammerModel->Release();
		g_pMeshHammerModel = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatHammerModel != NULL)
	{
		g_pBuffMatHammerModel->Release();
		g_pBuffMatHammerModel = NULL;
	}
}
//****************************************
// モデルの更新処理
//****************************************
void UpdatPlayerModel(void)
{
	//前回の位置を更新する
	g_player.posOld = g_player.pos;

	if(g_player.bAction == true)
	{
		//git少し使えるようになった
		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーが押された

			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された
				g_player.moveRot.y = -0.75 - (Getcamara(0)->rot.z * D3DX_PI);
				g_player.rot.y = 2.25f - (Getcamara(0)->rot.z * D3DX_PI);

				g_player.move.x += sinf(g_player.moveRot.y) * MODEL_SPEED;
				g_player.move.z += cosf(g_player.moveRot.y) * MODEL_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された
				g_player.moveRot.y = -2.25 - (Getcamara(0)->rot.z * D3DX_PI);
				g_player.rot.y = 0.75f - (Getcamara(0)->rot.z * D3DX_PI);

				g_player.move.x += sinf(g_player.moveRot.y) * MODEL_SPEED;
				g_player.move.z += cosf(g_player.moveRot.y) * MODEL_SPEED;
			}
			else
			{
				g_player.moveRot.y = -1.570796f - (Getcamara(0)->rot.z * D3DX_PI);
				g_player.rot.y = 1.570796f - (Getcamara(0)->rot.z * D3DX_PI);


				g_player.move.x += sinf(g_player.moveRot.y) * MODEL_SPEED;
				g_player.move.z += cosf(g_player.moveRot.y) * MODEL_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された

			if (GetKeyboardPress(DIK_W) == true)
			{//Wキーが押された
				g_player.moveRot.y = 0.75 - (Getcamara(0)->rot.z * D3DX_PI);
				g_player.rot.y = -2.25f - (Getcamara(0)->rot.z * D3DX_PI);

				g_player.move.x += sinf(g_player.moveRot.y) * MODEL_SPEED;
				g_player.move.z += cosf(g_player.moveRot.y) * MODEL_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//Sキーが押された
				g_player.moveRot.y = 2.25 - (Getcamara(0)->rot.z * D3DX_PI);
				g_player.rot.y = -7.5 - (Getcamara(0)->rot.z * D3DX_PI);


				g_player.move.x += sinf(g_player.moveRot.y) * MODEL_SPEED;
				g_player.move.z += cosf(g_player.moveRot.y) * MODEL_SPEED;
			}
			else
			{
				g_player.moveRot.y = 1.570796f - (Getcamara(0)->rot.z * D3DX_PI);
				g_player.rot.y = -1.570796f - (Getcamara(0)->rot.z * D3DX_PI);

				g_player.move.x += sinf(g_player.moveRot.y) * MODEL_SPEED;
				g_player.move.z += cosf(g_player.moveRot.y) * MODEL_SPEED;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された
			g_player.moveRot.y = 0.0f - (Getcamara(0)->rot.z * D3DX_PI);
			g_player.rot.y = 3.1425f - (Getcamara(0)->rot.z * D3DX_PI);

			// 移動量を更新
			g_player.move.x += sinf(g_player.moveRot.y) * MODEL_SPEED;
			g_player.move.z += cosf(g_player.moveRot.y) * MODEL_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された
			g_player.moveRot.y = 3.1425f - (Getcamara(0)->rot.z * D3DX_PI);
			g_player.rot.y = 0.0f - (Getcamara(0)->rot.z * D3DX_PI);

			g_player.move.x += sinf(g_player.moveRot.y) * MODEL_SPEED;
			g_player.move.z += cosf(g_player.moveRot.y) * MODEL_SPEED;
		}

		if (GetKeyboardPress(DIK_J) == true)
		{//DIK_LSHIFT キーが押された

			//g_player.rot.y -= 0.03125f;
			Getcamara(0)->rot.z += ROT_SPEED;
		}
		else if (GetKeyboardPress(DIK_L) == true)
		{//DIK_RSHIFT キーが押された

			//g_player.rot.y += 0.03125f;
			Getcamara(0)->rot.z -= ROT_SPEED;
		}

		
		for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
		{
			if (nCntCamera < 2)
			{
				Getcamara(nCntCamera)->posR.x = g_player.pos.x + sinf(D3DX_PI * (1.0f - g_player.rot.y));
				Getcamara(nCntCamera)->posR.z = g_player.pos.z + cosf(D3DX_PI * (1.0f - g_player.rot.y));

				Getcamara(nCntCamera)->posV.x = Getcamara(nCntCamera)->posR.x + sinf(D3DX_PI * (1.0f - Getcamara(nCntCamera)->rot.z)) * Getcamara(nCntCamera)->length;
				Getcamara(nCntCamera)->posV.z = Getcamara(nCntCamera)->posR.z + cosf(D3DX_PI * (1.0f - Getcamara(nCntCamera)->rot.z)) * Getcamara(nCntCamera)->length;
			}
		}

		//移動量を更新

		//// 視点から注視点までの角度を求め、移動向きに加算
		//g_player.moveRot.y += FindAngle(
		//	D3DXVECTOR3(posV.x, posV.z, 0.0f),
		//	D3DXVECTOR3(posR.x, posR.z, 0.0f));

		//// 向き/移動向き/目標向きを制御
		//RotControl(&g_player.rot);
		//RotControl(&g_player.moveRot);
		//RotControl(&g_player.targetRot);

		//// 角度を目標角度に向けて推移する
		//g_player.rot.y += AngleDifference(g_player.rot.y, g_player.targetRot.y) * CHR_PLAYER_00_ROT_DIAMETER;

		//// 移動量を更新
		//g_player.move.x += sinf(g_player.moveRot.y) * fMove;
		//g_player.move.z += cosf(g_player.moveRot.y) * fMove;

		g_player.move.x += (0.0f - g_player.move.x) * 0.5f;
		g_player.move.z += (0.0f - g_player.move.z) * 0.5f;

		//位置を更新
		g_player.pos.x += g_player.move.x;
		g_player.pos.y += g_player.move.y;
		g_player.pos.z += g_player.move.z;

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 25.0f, g_player.pos.z), g_player.rot);
			PlaySound(SOUND_LABEL_SHOT); //弾の発射音
		}
	}
	else if (g_player.bAction == false)
	{

	}

	//壁との当たり判定（ポリゴン）
	CollisionWallPlayer(&g_player.pos, &g_player.posOld, &g_player.move);

	//壁との当たり判定（メッシュ）
	//CollisionMeshField_Wall(&g_player.pos, &g_player.posOld, &g_player.move);

	//置物との当たり判定
	CollisionFigurePlayer(&g_player.pos,&g_player.posOld,&g_player.move);
	
	//敵との当たり判定
	CollisionEnemy_Player(&g_player.pos, &g_player.posOld, &g_player.move);

	//石柱との当たり判定
	CollisioPillar_Model(&g_player.pos, &g_player.posOld, &g_player.move);

	//宝石との当たり判定
	CollisionJewel(&g_player.pos, &g_player.posOld, &g_player.move);

	//プレイヤーの影描画
	SetPositionShadow(g_player.nIdxShadow, g_player.pos,g_player.fWidht,g_player.fWidht);
}
//****************************************
// モデルの描画処理
//****************************************
void DrawPlayerModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの所得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアルの保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	
	//D3DXMATRIX mtxShadow;						//シャドウマトリックス
	//D3DLIGHT9 light;							//ライト情報
	//D3DXVECTOR4 posLight;						//ライトの位置
	//D3DXVECTOR3 pos, normal;					//平面上の任意の点、法線ベクトル
	//D3DXPLANE plane;							//平面情報

	////ライトの位置を特定
	//pDevice->GetLight(0, &light);
	//posLight = D3DXVECTOR4(0, 100, 100, 0.0f);

	////平面情報を生成
	//pos = D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z);
	//normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	////シャドウマトリックスの初期化
	//D3DXMatrixIdentity(&mtxShadow);

	////シャドウマトリックスの作成
	//D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	//D3DXMatrixMultiply(&mtxShadow, &g_player.mtxWorld, &mtxShadow);

	////シャドウマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y,
		g_player.rot.x, g_player.rot.z);

	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);

	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//現在のマテリアルを所得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを所得
	pMat = (D3DXMATERIAL*)g_pBuffMatHammerModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumHammerMatModel; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
		pMat[nCntMat].MatD3D.Diffuse.b = 1.0f;
		pMat[nCntMat].MatD3D.Diffuse.g = 1.0f;
		pMat[nCntMat].MatD3D.Diffuse.r = 1.0f;

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureHammerModel[nCntMat]);

		//モデル(パーツ)の描画
		g_pMeshHammerModel->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//****************************************
// プレイヤーの情報
//****************************************
Player *GetPlayer(void)
{
	return &g_player;
}
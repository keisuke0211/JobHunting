//◇====================◇
//｜　　　　　　　　　　｜
//｜　ライトの処理      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "light.h"
#include "PointLight.h"
#include "Player_model.h"
//****************************************
// グローバル変数
//****************************************
D3DLIGHT9 g_PointLihgt[NUM_POINTLIGHT];//ライトの情報
				  //ライトの情報
static PointLight g_lihgtInformation[NUM_POINTLIGHT] =
{
	//{ D3DXVECTOR3(0.2f , -0.9f , 0.6f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) },	// 前
	//{ D3DXVECTOR3(0.2f , 0.9f , -0.5f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) },	// 後ろ
	//{ D3DXVECTOR3(0.9f , -0.9f , 0.5f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) },	// 右

	{D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXCOLOR(0.1f, 0.1f, 0.0f, 1.0f)},	// 上・前
};

//***************************************
// 初期化処理
//****************************************
void InitPointLight(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;//設定用方向ベクトル

	 //ライトの情報をクリアする
	ZeroMemory(&g_PointLihgt, sizeof(D3DLIGHT9));

	for (int nCntLight = NUM_LIGHT; nCntLight < NUM_POINTLIGHT + NUM_LIGHT; nCntLight++)
	{
		//ライトの種類を設定
		g_PointLihgt[nCntLight].Type = D3DLIGHT_POINT;

		//ライトの拡散光を設定g_lihgtInformation[nCntLight - NUM_LIGHT].col
		g_PointLihgt[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライトの方向を設定
		vecDir = g_lihgtInformation[nCntLight - NUM_LIGHT].rot;

		//ベクトルを正規化
		D3DXVec3Normalize(&vecDir, &vecDir);

		g_PointLihgt[nCntLight].Direction = vecDir;

		g_PointLihgt[nCntLight].Position = D3DXVECTOR3(0.0f,20.0f, 100.0f);

		/*g_PointLihgt[1].Position = D3DXVECTOR3(-99.0f, 170.0f, 450.0f);
		g_PointLihgt[2].Position = D3DXVECTOR3(0.0f, 170.0f, 1049.0f);*/

		g_PointLihgt[nCntLight].Range = 150.0f;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_PointLihgt[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}
//****************************************
// 終了処理
//****************************************
void UninitPointLight(void)
{

}
//****************************************
// 更新処理
//****************************************
void UpdatePointLight(void)
{

}
//◇====================◇
//｜　　　　　　　　　　｜
//｜　ライトの処理      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "light.h"
#include "Player_model.h"
//****************************************
// グローバル変数
//****************************************
D3DLIGHT9 g_lihgt;//ライトの情報
//ライトの情報
static Light g_lihgtInformation[NUM_LIGHT] =
{
 //{ D3DXVECTOR3(0.2f , -0.9f , 0.6f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) },	// 前
 //{ D3DXVECTOR3(0.2f , 0.9f , -0.5f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) },	// 後ろ
 //{ D3DXVECTOR3(0.9f , -0.9f , 0.5f),D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) },	// 右

 { D3DXVECTOR3(-0.9f , -0.9f , 0.5f),D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// 上・前
 { D3DXVECTOR3(0.2f , 0.9f , -0.2f)	,D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// 下
 { D3DXVECTOR3(0.9f , -0.9f , -0.5f),D3DXCOLOR(LIGHT_CLOR, LIGHT_CLOR, LIGHT_CLOR, 1.0f) },	// 右・後
};

//***************************************
// 初期化処理
//****************************************
void InitLight(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;//設定用方向ベクトル

	//ライトの情報をクリアする
	ZeroMemory(&g_lihgt, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	g_lihgt.Type = D3DLIGHT_DIRECTIONAL;

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの拡散光を設定
		g_lihgt.Diffuse = g_lihgtInformation[nCntLight].col;

		//ライトの方向を設定
		vecDir = g_lihgtInformation[nCntLight].rot;

		//ベクトルを正規化
		D3DXVec3Normalize(&vecDir, &vecDir);

		g_lihgt.Direction = vecDir;
		
		g_lihgt.Range = 300.0f;
		g_lihgt.Position = D3DXVECTOR3(30.0f, 70.0f, 500.0f);
		/*g_lihgt.Ambient = D3DXCOLOR(1.0f, 1.f,1.0f,1.0f);
		g_lihgt.Specular = D3DXCOLOR(1.0f,1.0f,1.0f, 1.0f);
		g_lihgt.Attenuation0 = 0.0f;
		g_lihgt.Attenuation1 = 0.0f;
		g_lihgt.Attenuation2 = 0.0f;*/

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_lihgt);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}
//****************************************
// 終了処理
//****************************************
void UninitLight(void)
{

}
//****************************************
// 更新処理
//****************************************
void UpdateLight(void)
{

}
//◇====================◇
//｜　　　　　　　　　　｜
//｜　プレイヤーの処理  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "game.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "sound.h"
#include "life.h"
#include "particle.h"
#include "effect.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define MAX_SPEED (65535)		//振動の最大値
#define MIN_SPEED	  (0)		//振動の最小値

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffPlayer = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;			//テクスチャへのポインタ
int g_nCounterAnimPlayer;									//アニメーションカウンター座標
int g_nPatternAnimPlayer;									//アニメーションパターン No.
Player g_Player;											//プレイヤーの情報
float g_fLengthPlayer;									    //対角線の長さ
float g_fAnglePlayer;									    //対角線の角度
int ShootMode;												//射撃切り替え　　0:単射　1:連射
int DirectionMode;											//弾数
int nCount;
int g_nCntMove;
//==============================================================================================================================================
//―――――――――――――――――――――――――――――｜【プレイヤー・処理】｜――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇==============================◇
//｜　　プレイヤーの初期化処理　　｜
//◇==============================◇
void InitPlayer(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜　テクスチュの読み込み　｜
	//◇――――――――――――◇
	D3DXCreateTextureFromFile(pDevice,
		".\\data\\TEXTURE\\fighter_a.png",
		&g_pTexturePlayer);

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	g_Player.pos = D3DXVECTOR3((SCREEN_WIDTH -300) / 2, SCREEN_HEIGHT -200,0);		//位置
	g_Player.move = D3DXVECTOR3(0.0f,0.0f,0.0f);									//移動量
    //                                   ↓-3.14f～3.14f
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//向きを初期化する(Z値を使用)
	g_Player.nLife = 5;																//ライフの値を設定
	g_Player.state = PLAYERSTATE_NORMAL;											//プレイヤーの状態を初期化
	g_Player.nCounterState = 0;														//プレイヤーの状態カウントを初期化
	g_Player.bUse = true;
	g_Player.bDrw = true;
	g_Player.bBeing = true;
	g_nCntMove = 0;
	//対角線の長さを算出する(p幅*p幅+p高さ*p高さ);
	g_fLengthPlayer = sqrtf(64*64+64*64);

	//対角線の角度を算出する(p幅,p高さ)
	g_fAnglePlayer = atan2f(64,64);


	//g_nCounterAnimPlayer = 1;					//カウンターを初期化する
	//g_nPatternAnimPlayer = 0;					//パターンNo.を初期

	ShootMode     = 0;							//射撃モードを初期化
	DirectionMode = 0;							//弾数を初期化する
	nCount = 0;
	//◇――――――――――――◇
	//｜　　  頂点バッファ  　　｜
	//◇――――――――――――◇
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffPlayer,
		NULL);

	//設定用ポインタ
	VERTEX_2D *pVtx;

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);
	//◇――――――――――――◇
	//｜ 　　頂点座標を設定　　 ｜
	//◇――――――――――――◇

	//　　　　x                               y                     z
	//pVtx[0].pos = D3DXVECTOR3(g_posPlayer.x - SIZE_WIDTH / 2, g_posPlayer.y - SIZE_HEIGHT / 2, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(g_posPlayer.x + SIZE_WIDTH / 2, g_posPlayer.y - SIZE_HEIGHT / 2, 0.0f); //右回りで設定する
	//pVtx[2].pos = D3DXVECTOR3(g_posPlayer.x - SIZE_WIDTH / 2, g_posPlayer.y + SIZE_HEIGHT / 2, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(g_posPlayer.x + SIZE_WIDTH / 2, g_posPlayer.y + SIZE_HEIGHT / 2, 0.0f); //左回りで設定する

	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 - g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 + g_fAnglePlayer)) * g_fLengthPlayer;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 + g_fAnglePlayer)) * g_fLengthPlayer;
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
	g_pVtxbuffPlayer->Unlock();

	//ライフの値を設定
	AddLife(g_Player.nLife);
}
//◇==============================◇
//｜　　 プレイヤーの終了処理　　 ｜
//◇==============================◇
void UninitPlayer(void)
{
	//◇――――――――――――◇
	//｜　　テクスチュの破棄　　｜
	//◇――――――――――――◇
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//◇―――――――――――――◇
	//｜　　頂点バッファの破棄　　｜
	//◇―――――――――――――◇
	if (g_pVtxbuffPlayer != NULL)
	{
		g_pVtxbuffPlayer->Release();
		g_pVtxbuffPlayer = NULL;
	}
}
//◇==============================◇
//｜　　 プレイヤーの更新処理　　 ｜
//◇==============================◇
void UpdatePlayer(void)
{
	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ
	
	XINPUT_STATE state;							//XINPUT_STATE型 stateを作成
	XInputGetState(0, &state);					//stateに習得したコントローラの状態を格納
	XINPUT_VIBRATION vibration;					//XINPUT_VIBRATION vibrationを作成

	if (g_Player.bUse == true)
	{ 
		//コントローラ・スティック操作

		if (GetStick(STICK_TYPE_LEFT).fTplDiameter > 0)
		{// 左スティックが倒れている時、
			g_Player.move.x += sinf(GetStick(STICK_TYPE_LEFT).fAngle) * PLAYER_SPEED;
			g_Player.move.y += cosf(GetStick(STICK_TYPE_LEFT).fAngle) * PLAYER_SPEED;
		}
		//else if (GetStick(STICK_TYPE_RIGHT).fTplDiameter > 0)
		//{// 右スティックが倒れている時、
		//	g_Player.move.x += sinf(GetStick(STICK_TYPE_RIGHT).fAngle) * PLAYER_SPEED;
		//	g_Player.move.y += cosf(GetStick(STICK_TYPE_RIGHT).fAngle) * PLAYER_SPEED;
		//}

		//キーボード&コントローラ・ボタン　操作

		else if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true || 
			xInput->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{//Aキーが押された

			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true ||
				xInput->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{//左上移動
				g_Player.move.x += sinf(D3DX_PI * -0.75f) * PLAYER_SPEED;
				g_Player.move.y += cosf(D3DX_PI * -0.75f) * PLAYER_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true || 
				xInput->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{//左下移動
				g_Player.move.x += sinf(D3DX_PI * -0.25f) * PLAYER_SPEED;
				g_Player.move.y += cosf(D3DX_PI * -0.25f) * PLAYER_SPEED;
			}
			else
			{//左移動
				g_Player.move.x += sinf(D3DX_PI * -0.5f) * PLAYER_SPEED;
				g_Player.move.y += cosf(D3DX_PI * -0.5f) * PLAYER_SPEED;

			}
		}
		else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true || 
			xInput->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{//Dキーが押された
			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true || 
				xInput->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{//右上移動
				g_Player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
				g_Player.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true || 
				xInput->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{//右下移動
				g_Player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED;
				g_Player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED;
			}
			else
			{//右移動
				g_Player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_SPEED;
				g_Player.move.y += cosf(D3DX_PI * 0.5f) * PLAYER_SPEED;

			}
		}
		else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true || 
			xInput->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
		{//Wキーが押された
			g_Player.move.x += sinf(D3DX_PI * 1.0f) * PLAYER_SPEED;
			g_Player.move.y += cosf(D3DX_PI * 1.0f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true ||
			xInput->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{//Sキーが押された
			g_Player.move.x += sinf(D3DX_PI * 0.0f) * PLAYER_SPEED;
			g_Player.move.y += cosf(D3DX_PI * 0.0f) * PLAYER_SPEED;
		}

		
		if (ShootMode == 0)
		{
			if (xInput->Gamepad.wButtons & XINPUT_GAMEPAD_A )
			{
				nCount++;

				if (nCount % 12 == 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT); //←再生したいサウンドを指定
					// A ボタンが押された
					SetBullet(D3DXVECTOR3(g_Player.pos.x + 2.0f, g_Player.pos.y - 65.0f, 0.0f), D3DXVECTOR3(0.0f, -SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				}
			}
			if (GetKeyboardPress(DIK_SPACE) == true)
			{

				nCount++;
				if (DirectionMode == 0 && nCount % 12 == 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT); //←再生したいサウンドを指定
					SetBullet(D3DXVECTOR3(g_Player.pos.x + 2.0f, g_Player.pos.y - 65.0f, 0.0f), D3DXVECTOR3(0.0f, -SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				}
			}
		}
		else if (ShootMode == 1)
		{
			nCount++;
			if (GetKeyboardPress(DIK_SPACE) == true && nCount % 7 == 0)
			{
				PlaySound(SOUND_LABEL_SE_SHOT); //←再生したいサウンドを指定

				if (DirectionMode == 0)
				{
					SetBullet(D3DXVECTOR3(g_Player.pos.x + 0.0f, g_Player.pos.y - 65.0f, 0.0f), D3DXVECTOR3(0.0f, -SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				}
				/*else if (DirectionMode == 1)
				{
				SetBullet(g_Player.pos, D3DXVECTOR3(0.0f, -SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(sinf(D3DX_PI * -UP)* SPEED, cosf(D3DX_PI * -UP)* SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(sinf(D3DX_PI * UP)* SPEED, cosf(D3DX_PI * -UP)* SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(SPEED, 0.0f, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(-SPEED, 0.0f, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(0.0f, SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(sinf(D3DX_PI * -DOWN)* SPEED, cosf(D3DX_PI * DOWN)* SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				SetBullet(g_Player.pos, D3DXVECTOR3(sinf(D3DX_PI * DOWN)* SPEED, cosf(D3DX_PI  * DOWN)* SPEED, 0.0f), g_Player.rot.z, BULLETTYPE_PLAYER);
				}*/
			}
		}
		//回転

		//if (GetKeyboardPress(DIK_LEFT) == true)
		//{
		//	g_Player.rot.z = (float)fmod(g_Player.rot.z + D3DX_PI + (D3DX_PI * 2) + 0.1f, D3DX_PI * 2) - D3DX_PI;	//時計周り
		//}
		//else if (GetKeyboardPress(DIK_RIGHT) == true)
		//{
		//	g_Player.rot.z = (float)fmod(g_Player.rot.z + D3DX_PI + (D3DX_PI * 2) - 0.1f, D3DX_PI * 2) - D3DX_PI;	//反時計周り
		//}

		//拡大・縮小

		//if (GetKeyboardPress(DIK_UP) == true)
		//{
		//	g_fLengthPlayer += 0.5f;
		//}
		//else if (GetKeyboardPress(DIK_DOWN) == true)
		//{
		//	g_fLengthPlayer -= 0.5f;
		//}
		else if (GetKeyboardPress(DIK_F2))
		{
			if (ShootMode == 0)
			{
				ShootMode++;
			}
			else if (ShootMode == 1)
			{
				ShootMode--;
			}
		}
		//else if (GetKeyboardPress(DIK_F3))
		//{
		//	if (DirectionMode == 0)
		//	{
		//		DirectionMode++;
		//	}
		//	else if (DirectionMode == 1)
		//	{
		//		DirectionMode--;
		//	}
		//}

		//位置を更新
		g_Player.pos.x += g_Player.move.x;
		g_Player.pos.y += g_Player.move.y;

		//プレイヤーの位置が範囲外に出た処理
		if (g_Player.pos.x > 960.0f)
		{//X座標が範囲外の場合
			g_Player.pos.x = 960.0f;
		}
		if (g_Player.pos.x < 50.0f)
		{//X座標が範囲外の場合
			g_Player.pos.x = 50.0f;
		}
		if (g_Player.pos.y > 680.0f)
		{//Y座標が範囲外の場合
			g_Player.pos.y = 680.0f;
		}
		if (g_Player.pos.y < 45.0f)
		{//Y座標が範囲外の場合
			g_Player.pos.y = 45.0f;
		}
		//移動量を更新(減衰させる)
		g_Player.move.x += (0.0f - g_Player.move.x) * 0.25f;
		g_Player.move.y += (0.0f - g_Player.move.y) * 0.25f;

		////頂点座標の再設定
		//pVtx[0].pos = D3DXVECTOR3(g_posPlayer.x - SIZE_WIDTH / 2, g_posPlayer.y - SIZE_HEIGHT / 2, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(g_posPlayer.x + SIZE_WIDTH / 2, g_posPlayer.y - SIZE_HEIGHT / 2, 0.0f); //右回りで設定する
		//pVtx[2].pos = D3DXVECTOR3(g_posPlayer.x - SIZE_WIDTH / 2, g_posPlayer.y + SIZE_HEIGHT / 2, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(g_posPlayer.x + SIZE_WIDTH / 2, g_posPlayer.y + SIZE_HEIGHT / 2, 0.0f); //左回りで設定する
		//テクスチャ座標の更新
		VERTEX_2D *pVtx;

		//頂点バッファのロック
		g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

		switch (g_Player.state)
		{
		case PLAYERSTATE_NORMAL:
			break;

		case PLAYERSTATE_DAMAGE:

			if (g_Player.nCounterState == 0)
			{
				if (g_Player.bBph == true)
				{
					vibration.wLeftMotorSpeed = MIN_SPEED;		//vibration　メンバー　wLeftMotorSpeed　に値を代入　モータースピード100%
					vibration.wRightMotorSpeed = MIN_SPEED;		//　　　　 〃　    　　wRightMotorSpeed に値を代入  モータースピード0%
					XInputSetState(0, &vibration);				//ジョイスティックにバイブレーターデータを送信
				}

				g_Player.nCounterState = 20;
				g_Player.bDrw = true;
				g_Player.state = PLAYERSTATE_WAIT;
				//◇――――――――――――◇
				//｜　　　 頂点カラー 　　　｜
				//◇――――――――――――◇
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//ポリゴンを通常色に戻す
			}
			else
			{
				g_Player.nCounterState--;
				g_Player.bDrw = g_Player.bDrw ? false : true;
			}
			break;
		case PLAYERSTATE_WAIT:
			if (g_Player.nCounterState == 0)
			{
				g_Player.bDrw = true;
				g_Player.state = PLAYERSTATE_NORMAL;
			}
			else
			{
				g_Player.nCounterState--;
				g_Player.bDrw = g_Player.bDrw ? false : true;
			}
			break;
		case PLAYERSTATE_DEATH:
			return;
			break;
		}


		pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[3].pos.z = 0.0f;
		
		//・頂点バッファをアンロック
		g_pVtxbuffPlayer->Unlock();
	}
	else if (g_Player.bUse == false && g_Player.bBeing == true)
	{
		g_nCntMove++;
		g_Player.pos = (g_Player.pos * (1.0f - (float)(g_nCntMove / (float)2000)) + (D3DXVECTOR3(g_Player.pos.x, -200.0f, 0.0f) * (float)(g_nCntMove / (float)2000)));

		SetParticle(D3DXVECTOR3(g_Player.pos.x + 20, g_Player.pos.y + 0.0f, 0.0f), 3, 40, 30.0f, 3,1);
		SetParticle(D3DXVECTOR3(g_Player.pos.x - 20, g_Player.pos.y + 0.0f, 0.0f), 3, 40, 30.0f, 3,1);


		VERTEX_2D *pVtx;

		//頂点バッファのロック
		g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[3].pos.z = 0.0f;

		//・頂点バッファをアンロック
		g_pVtxbuffPlayer->Unlock();
	}
}
//◇==============================◇
//｜　　 プレイヤーの描画処理　　 ｜
//◇==============================◇
void DrawPlayer(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの所得
	pDevice = GetDevice();

	//◇――――――――――――◇
	//｜データストリームの設定　｜
	//◇――――――――――――◇

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffPlayer, 0, sizeof(VERTEX_2D));

	//◇――――――――――――◇
	//｜頂点フォーマットの設定　｜
	//◇――――――――――――◇
	pDevice->SetFVF(FVF_VERTEX_2D);

	//◇――――――――――――◇
	//｜　　テクスチャの設定　　｜
	//◇――――――――――――◇
	if (g_Player.bDrw == true)
	{
		pDevice->SetTexture(0, g_pTexturePlayer);

		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		//頂点情報構造体のサイズ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
//◇================================◇
//｜　　 プレイヤーのヒット処理　　 ｜
//◇================================◇
void HitPlayer(int nCntPlayer, int nDamage)
{
	XINPUT_STATE state;							//XINPUT_STATE型 stateを作成
	XInputGetState(0, &state);					//stateに習得したコントローラの状態を格納
	XINPUT_VIBRATION vibration;					//XINPUT_VIBRATION vibrationを作成

		if (g_Player.nCounterState > 0)
		{}
		else if (g_Player.state == PLAYERSTATE_NORMAL)
		{
			g_Player.nLife -= nDamage;
			DeleteLife(nDamage);
		}

	if (g_Player.nLife == 0)//プレイヤーの体力がなくなった
	{
		g_Player.state = PLAYERSTATE_DEATH;

		//爆発の設定
		SetParticle(g_Player.pos, 0, 40, 30.0f,0,0);

		//SetExplosion(g_Player.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		PlaySound(SOUND_LABEL_SE_EXPLOSION); //←再生したいサウンドを指定

		vibration.wLeftMotorSpeed = MIN_SPEED;		//vibration　メンバー　wLeftMotorSpeed　に値を代入　モータースピード100%
		vibration.wRightMotorSpeed = MIN_SPEED;		//　　　　 〃　    　　wRightMotorSpeed に値を代入  モータースピード0%
		XInputSetState(0, &vibration);				//ジョイスティックにバイブレーターデータを送信

		g_Player.bUse = false;
		g_Player.bDrw = false;
		g_Player.bBeing = false;

		//状態の設定
		SetGameState(GAMESTATE_END2, 160);
	}

	else if (g_Player.nLife > 0 && g_Player.state == PLAYERSTATE_NORMAL)
	{
		if (g_Player.state == PLAYERSTATE_NORMAL)
		{
			g_Player.state = PLAYERSTATE_DAMAGE;
			g_Player.nCounterState = 10; //ダメージを保つ時間を設定
		}
		PlaySound(SOUND_LABEL_SE_HIT); //HIT音を流す

		if (g_Player.bBph == true)
		{
			vibration.wLeftMotorSpeed = MAX_SPEED;		//vibration　メンバー　wLeftMotorSpeed　に値を代入　モータースピード100%
			vibration.wRightMotorSpeed = MAX_SPEED;		//　　　　 〃　    　　wRightMotorSpeed に値を代入  モータースピード0%
			XInputSetState(0, &vibration);				//ジョイスティックにバイブレーターデータを送信
		}
		//XInputEnable(false);						//XInputを閉じる。		※これがないとずっと振動しっぱなしになる。

	}

}
//◇======================◇
//｜　 プレイヤーの所得　 ｜
//◇======================◇
Player *Getplayer(void)
{
	return &g_Player;//敵の情報の先頭アドレスを返す
}

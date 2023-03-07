//◇====================◇
//｜　　　　　　　　　　｜
//｜　プレイヤーの処理  ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#include "player.h"
#include "game.h"
#include "input.h"
#include "camera.h"
#include "lure.h"
#include "rope.h"
#include "sound.h"
#include "time.h"
//****************************************
// グローバル変数
//****************************************

LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffPlayer = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;			//テクスチャへのポインタ

int nCnt;
Player g_Player;											//プレイヤーの情報
//****************************************
// プレイヤーの初期化処理
//****************************************
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
		".\\data\\TEXTURE\\player01.png",
		&g_pTexturePlayer);

	//◇――――――――――――◇
	//｜　　 アニメーション 　　｜
	//◇――――――――――――◇

	g_Player.pos			= D3DXVECTOR3(100, SCREEN_HEIGHT - 150,0);						//現在の位置
	g_Player.posOld			= D3DXVECTOR3(100, SCREEN_HEIGHT - 150, 0);						//前回の位置
	g_Player.move			= D3DXVECTOR3(0.0f,0.0f,0.0f);									//移動量
	g_Player.Draepos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//描画位置
	g_Player.rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//向きを初期化する(Z値を使用)
	g_Player.fLength	    = sqrtf(SIZE_WIDTH*SIZE_WIDTH+SIZE_HEIGHT*SIZE_HEIGHT);			//プレイヤーとフックとの距離
	g_Player.fAngle			= atan2f(SIZE_WIDTH, SIZE_HEIGHT);								//現在のプレイヤーとフックとの角度
	g_Player.fTwo_Range		= sqrtf(0*0 + 0*0);												//プレイヤーとフックとの距離
	g_Player.fTwo_Angle		= atan2f(0, 0);													//現在のプレイヤーとフックとの角度
	g_Player.nCounterAnim	= DEFAULT_INT;													//アニメーションカウンターを初級する
	g_Player.nPatternAnim	= DEFAULT_INT;													//アニメーションパターンを初期化する
	g_Player.nCntReturn		= DEFAULT_INT;													//プレイヤーがフックの位置に移動するカウント
	g_Player.nCntFriction	= DEFAULT_INT;													//摩擦の減少カウント
	g_Player.bCheckpoint	= DEFAULT_INT;

	//振り子に使う変数
	g_Player.fx				= DEFAULT_FLOAT;
	g_Player.fy				= DEFAULT_FLOAT;
	g_Player.rot1			= DEFAULT_FLOAT;
	g_Player.rot_spd		= DEFAULT_FLOAT;
	g_Player.lengthFixed	= DEFAULT_FLOAT;
	g_Player.g				= DEFAULT_FLOAT;
	g_Player.px				= DEFAULT_FLOAT;
	g_Player.py				= DEFAULT_FLOAT;
	g_Player.vx				= DEFAULT_FLOAT;
	g_Player.vy				= DEFAULT_FLOAT;
	g_Player.t				= DEFAULT_FLOAT;
	g_Player.gx				= DEFAULT_FLOAT;
	g_Player.gy				= DEFAULT_FLOAT;
	g_Player.r				= DEFAULT_FLOAT;
	g_Player.sub			= DEFAULT_FLOAT;
	g_Player.friction		= MAX_FRICTION;
	g_Player.length			= DEFAULT_FLOAT;

	//放物線運動に使う変数
	g_Player.px1			= DEFAULT_FLOAT;
	g_Player.py1			= DEFAULT_FLOAT;
	g_Player.dx				= DEFAULT_FLOAT;
	g_Player.dy				= DEFAULT_FLOAT;
	g_Player.gx1			= DEFAULT_FLOAT;
	g_Player.gy1			= DEFAULT_FLOAT;

	//bool型
	g_Player.bJump		= false;
	g_Player.bDraw		= true;
	g_Player.bCamera	= true;
	g_Player.bLure		= false;
	g_Player.bSpace		= false;
	g_Player.bIdle		= false;
	g_Player.bMeasure	= false;
	g_Player.bMeasure2  = false;
	g_Player.bPendulum	= false;
	g_Player.bInertia	= false;
	g_Player.bAutoLure	= true;
	g_Player.bOperation = true;
	g_Player.bOnce		= false;
	g_Player.bDirection = false;

	nCnt = 0;
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

	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;

	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;

	pVtx[0].pos.z = DEFAULT_FLOAT;
	pVtx[1].pos.z = DEFAULT_FLOAT;
	pVtx[2].pos.z = DEFAULT_FLOAT;
	pVtx[3].pos.z = DEFAULT_FLOAT;

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
	pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.2f, 0.5f);

	//頂点バッファのアンロック
	g_pVtxbuffPlayer->Unlock();
}
//****************************************
// プレイヤーの終了処理
//****************************************
void UninitPlayer(void)
{
	//サウンドの停止
	StopSound();

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
//****************************************
// プレイヤーの	更新処理
//****************************************
void UpdatePlayer(void)
{
	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

	XINPUT_STATE state;							//XINPUT_STATE型 stateを作成
	XInputGetState(0, &state);					//stateに習得したコントローラの状態を格納

	Lure *pLure = GetLure();
	Rope *pRope = GetRope();
	Block *pBlock = GetBlock();
	//テクスチャ座標の更新
	VERTEX_2D *pVtx;

	//頂点バッファのロック
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	g_Player.posOld = g_Player.pos;		//前回の位置を保存

	//コントローラ・スティック操作
	if (GetStick(STICK_TYPE_LEFT).fTplDiameter > 0)
	{// 左スティックが倒れている時、
		g_Player.move.x += sinf(GetStick(STICK_TYPE_LEFT).fAngle) * PLAYER_SPEED;
		g_Player.move.y += cosf(GetStick(STICK_TYPE_LEFT).fAngle) * PLAYER_SPEED;
	}
	//操作出来るか
	if (g_Player.bOperation == true)
	{
		//プレイヤーの操作・移動処理
		PlayerAction();

		//ルアーの操作・移動処理
		LureAction();

	}
		//位置を更新
		g_Player.pos.x += g_Player.move.x;
		g_Player.pos.y += g_Player.move.y;

	//重力処理
	if (g_Player.bLure == false)
	{
		if (g_Player.bJump == false)
		{
			if (g_Player.move.y <= 0)
			{
				g_Player.move.y += MAX_GRAVITY;
			}
			else if (g_Player.move.y >= 1)
			{
				g_Player.move.y += (MAX_GRAVITY + MIN_GRAVITY);
			}
		}
		else if (g_Player.bJump == true)
		{
			if (g_Player.move.y <= 0)
			{
				if (g_Player.bPendulum == false)
				{
					g_Player.move.y += MAX_GRAVITY;
				}
				else if (g_Player.bPendulum == true && g_Player.bPendulum == true && g_Player.pos.y >= pLure->pos.y)
				{
					g_Player.move.y = MAX_GRAVITY;
				}
				else if (g_Player.bPendulum == true && g_Player.bPendulum == true && g_Player.pos.y <= pLure->pos.y)
				{
					g_Player.move.y += MAX_GRAVITY;
				}
			}
			else if (g_Player.move.y >= 1)
			{
				if (GetLure()->bStab == true)
				{
					g_Player.bPendulum = true;
				}

				if (g_Player.bPendulum == false)
				{
					g_Player.move.y += (MAX_GRAVITY + MIN_GRAVITY);
				}
				else if (g_Player.bPendulum == true && g_Player.bPendulum == true && g_Player.pos.y >= pLure->pos.y)
				{
					g_Player.move.y = MAX_GRAVITY;
				}
				else if (g_Player.bPendulum == true && g_Player.bPendulum == true && g_Player.pos.y <= pLure->pos.y)
				{
					g_Player.move.y += (MAX_GRAVITY + MIN_GRAVITY);
				}
			}
		}
	}
	else if (g_Player.bLure == true)
	{
		if (g_Player.bJump == false)
		{
			if (g_Player.move.y <= 0)
			{
				g_Player.move.y += MAX_GRAVITY;
			}
			else if (g_Player.move.y >= 1)
			{
				g_Player.move.y += (MAX_GRAVITY + MIN_GRAVITY);
			}
		}
		else if (g_Player.bJump == true)
		{
			if (g_Player.bPendulum == true && g_Player.pos.y <= pLure->pos.y)
			{
				if (g_Player.move.y <= 0)
				{
					g_Player.move.y += MAX_GRAVITY;
				}
				else if (g_Player.move.y >= 1)
				{
					g_Player.move.y += (MAX_GRAVITY + MIN_GRAVITY);
				}
			}
			else if (pLure->bWireA == true)
			{

			}
			/*else
			{
				if (g_Player.move.y <= 0)
				{
					g_Player.move.y += MAX_GRAVITY;
				}
				else if (g_Player.move.y >= 1)
				{
					g_Player.move.y += (MAX_GRAVITY + MIN_GRAVITY);
				}
			}*/
		}
	}
	//プレイヤーの位置が範囲外に出た処理
	if (g_Player.pos.x >11500.0f)
	{//X座標が範囲外の場合
		g_Player.pos.x = 24800.0f;
	}
	else if (g_Player.pos.x > 90.0f && g_Player.pos.x < 10050.0f)
	{
		g_Player.bCamera = true;
	}
	else if (g_Player.pos.x > -359.0f && g_Player.pos.x < 90.0f)
	{
		g_Player.bCamera = false;
	}
	else if (g_Player.pos.x < -360.0f)
	{//X座標が範囲外の場合
		g_Player.pos.x = -360.0f;
	}
	else if (g_Player.pos.x > 10050.0f)
	{
		g_Player.bCamera = false;
	}
	
	if (g_Player.pos.y >= 720.0f &&g_Player.pos.y <= 1199.0f)
	{
		pLure->bUse			= false;
		pRope->bUse			= false;
		g_Player.bOperation = false;
		g_Player.bCamera	= false;
	}
	else if (g_Player.pos.y > 1200.0f)
	{//Y座標が範囲外の場合
		if (nCnt == 5)
		{
			if (g_Player.bCheckpoint == 0)
			{
				g_Player.pos = D3DXVECTOR3(100, 615, 0);
			}
			if (g_Player.bCheckpoint == 1)
			{
				g_Player.pos = D3DXVECTOR3(4230, 515, 0);
			}
			else if (g_Player.bCheckpoint == 2)
			{
				g_Player.pos = D3DXVECTOR3(7600, 565, 0);
			}
			g_Player.nCntFriction	= DEFAULT_INT;
			g_Player.nCntReturn		= DEFAULT_INT;
			pLure->nLife			= DEFAULT_INT;
			pLure->nCntReturn		= DEFAULT_INT;

			g_Player.fx				= DEFAULT_FLOAT;
			g_Player.fy				= DEFAULT_FLOAT;
			g_Player.rot1			= DEFAULT_FLOAT;
			g_Player.rot_spd		= DEFAULT_FLOAT;
			g_Player.lengthFixed	= DEFAULT_FLOAT;
			g_Player.g				= DEFAULT_FLOAT;
			g_Player.vx				= DEFAULT_FLOAT;
			g_Player.vy				= DEFAULT_FLOAT;
			g_Player.t				= DEFAULT_FLOAT;
			g_Player.gx				= DEFAULT_FLOAT;
			g_Player.gy				= DEFAULT_FLOAT;
			g_Player.r				= DEFAULT_FLOAT;
			g_Player.sub			= DEFAULT_FLOAT;
			g_Player.px1			= DEFAULT_FLOAT;
			g_Player.py1			= DEFAULT_FLOAT;
			g_Player.dx				= DEFAULT_FLOAT;
			g_Player.dy				= DEFAULT_FLOAT;
			g_Player.gx1			= DEFAULT_FLOAT;
			g_Player.gy1			= DEFAULT_FLOAT;

			g_Player.friction		= MAX_FRICTION;

			g_Player.bCamera	= true;
			g_Player.bMeasure	= false;
			g_Player.bMeasure2	= false;
			g_Player.bOnce		= false;
			g_Player.bPendulum	= false;
			g_Player.bLure		= false;
			g_Player.bSpace		= false;
			g_Player.bAutoLure	= true;
			g_Player.bOperation = true;
			g_Player.bInertia	= false;
			g_Player.bDirection = false;
			pLure->bReturn		= false;
			pLure->bStab		= false;
			pLure->bWireA		= false;

			nCnt = 0;
		}
		else
		{
			nCnt++;
		}
	}

	if (g_Player.pos.y < -100.0f)
	{//Y座標が範囲外の場合
		g_Player.pos.y = -100.0f;
	}

	//移動量を更新(減衰させる)
	g_Player.move.x += (0.0f - g_Player.move.x) * 0.25f;


	//ブロックとの当たり判定
	if (CollisionBlockPlayer(&g_Player.pos, &g_Player.posOld, &g_Player.move) == true)
	{
		g_Player.bJump = false;
		g_Player.bPendulum = false;
		g_Player.bInertia = false;
	}

	//放物線運動していなかったらデータを初期化する
	if (g_Player.bInertia == false)
	{
		g_Player.px1 = DEFAULT_FLOAT;
		g_Player.py1 = DEFAULT_FLOAT;
		g_Player.dx	 = DEFAULT_FLOAT;
		g_Player.dy	 = DEFAULT_FLOAT;
		g_Player.gx1 = DEFAULT_FLOAT;
		g_Player.gy1 = DEFAULT_FLOAT;
		g_Player.bOnce = false;
	}
	//プレイヤーを中心に画面を動かす
	D3DXVECTOR3 drawPos = g_Player.pos;

	CameraPosition(&drawPos);

	pVtx[0].pos.x = drawPos.x - SIZE_WIDTH;
	pVtx[0].pos.y = drawPos.y - SIZE_HEIGHT;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = drawPos.x + SIZE_WIDTH;
	pVtx[1].pos.y = drawPos.y - SIZE_HEIGHT;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = drawPos.x - SIZE_WIDTH;
	pVtx[2].pos.y = drawPos.y + SIZE_HEIGHT;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = drawPos.x + SIZE_WIDTH;
	pVtx[3].pos.y = drawPos.y + SIZE_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//◇――――――――――――◇
	//｜　テクスチャ座標の設定　｜
	//◇――――――――――――◇
	pVtx[0].tex = D3DXVECTOR2(0.0f + ((g_Player.nPatternAnim % 5)* 0.2f), 0.0f	 + ((g_Player.nPatternAnim / 5)* 0.125f));
	pVtx[1].tex = D3DXVECTOR2(0.2f + ((g_Player.nPatternAnim % 5)* 0.2f), 0.0f	 + ((g_Player.nPatternAnim / 5)* 0.125f));
	pVtx[2].tex = D3DXVECTOR2(0.0f + ((g_Player.nPatternAnim % 5)* 0.2f), 0.125f + ((g_Player.nPatternAnim / 5)* 0.125f));
	pVtx[3].tex = D3DXVECTOR2(0.2f + ((g_Player.nPatternAnim % 5)* 0.2f), 0.125f + ((g_Player.nPatternAnim / 5)* 0.125f));

	//頂点バッファをアンロック
	g_pVtxbuffPlayer->Unlock();
}
//****************************************
// プレイヤーの描画処理
//****************************************
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
	if (g_Player.bDraw == true)
	{
		pDevice->SetTexture(0, g_pTexturePlayer);

		//◇――――――――――――◇
		//｜　　ポリゴンの描画　　　｜
		//◇――――――――――――◇

		//頂点情報構造体のサイズ
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}
//****************************************
// プレイヤーの操作・移動処理
//****************************************
void PlayerAction(void)
{
	//キーボード&コントローラ・ボタン　操作
	if (g_Player.bPendulum == false)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//Aキーが押された

			g_Player.move.x += sinf(D3DX_PI * -0.5f) * PLAYER_SPEED;

			if (g_Player.bJump == false)
			{
				if (g_Player.nPatternAnim >= 0 && g_Player.nPatternAnim <= 10)
				{
					g_Player.nPatternAnim = 11;
				}
				if (g_Player.nPatternAnim >= 24 && g_Player.nPatternAnim <= 35)
				{
					g_Player.nPatternAnim = 11;
				}

				g_Player.nCounterAnim++;

				if (g_Player.nCounterAnim % 4 == 0)
				{
					if (g_Player.nPatternAnim >= 11 && g_Player.nPatternAnim <= 20)
					{
						g_Player.nPatternAnim++;
					}
					else if (g_Player.nPatternAnim == 21)
					{
						g_Player.nPatternAnim = 12;
					}
				}
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//Dキーが押された
			g_Player.move.x += sinf(D3DX_PI * 0.5f) * PLAYER_SPEED;

			if (g_Player.bJump == false)
			{
				if (g_Player.nPatternAnim >= 11 && g_Player.nPatternAnim <= 21)
				{
					g_Player.nPatternAnim = 0;
				}
				if (g_Player.nPatternAnim >= 24 && g_Player.nPatternAnim <= 35)
				{
					g_Player.nPatternAnim = 0;
				}

				g_Player.nCounterAnim++;

				if (g_Player.nCounterAnim % 4 == 0)
				{
					if (g_Player.nPatternAnim >= 0 && g_Player.nPatternAnim <= 9)
					{
						g_Player.nPatternAnim++;
					}
					else if (g_Player.nPatternAnim == 10)
					{
						g_Player.nPatternAnim = 1;
					}
				}
			}
		}

		else if (GetKeyboardPress(DIK_A) == false && GetKeyboardPress(DIK_D) == false)
		{
			if (g_Player.nPatternAnim >= 11 && g_Player.nPatternAnim <= 21)
			{
				g_Player.nPatternAnim = 30;
			}
			else if (g_Player.nPatternAnim >= 0 && g_Player.nPatternAnim <= 10)
			{
				g_Player.nPatternAnim = 24;
			}
		}

		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//ＳＰＡＣＥが押された/\

			if (g_Player.bJump == false && g_Player.bLure == false)
			{
				PlaySound(SOUND_LABEL_SE_JUMP); //ジャンプの効果音

				g_Player.bJump = true;
				g_Player.move.y = -PLAYER_JUMP;
			}
		}
	}

	if (g_Player.bJump == true)
	{
		//プレイヤーが前向いている時
		if (g_Player.nPatternAnim >= 0 && g_Player.nPatternAnim <= 10)
		{
			g_Player.nPatternAnim = 22;
		}
		else if (g_Player.nPatternAnim >= 24 && g_Player.nPatternAnim <= 29)
		{
			g_Player.nPatternAnim = 22;
		}

		//プレイヤーが後ろ向いている時
		else if (g_Player.nPatternAnim >= 11 && g_Player.nPatternAnim <= 21)
		{
			g_Player.nPatternAnim = 23;
		}
		else if (g_Player.nPatternAnim >= 30 && g_Player.nPatternAnim <= 35)
		{
			g_Player.nPatternAnim = 23;
		}
	}
	else if (g_Player.bJump == false)
	{
		//ジャンプモーション　前方
		if (g_Player.nPatternAnim == 22)
		{
			g_Player.nPatternAnim = 24;
		}
		//ジャンプモーション　後方
		else if (g_Player.nPatternAnim == 23)
		{
			g_Player.nPatternAnim = 30;
		}
		//待機モーション前方

		if (g_Player.nPatternAnim >= 24 && g_Player.nPatternAnim <= 35)
		{
			g_Player.nCounterAnim++;

			if (g_Player.nCounterAnim % 12 == 0)
			{
				if (g_Player.nPatternAnim >= 24 && g_Player.nPatternAnim <= 28)
				{
					g_Player.nPatternAnim++;
				}
				else if (g_Player.nPatternAnim == 29)
				{
					g_Player.nPatternAnim = 25;
				}
				else if (g_Player.nPatternAnim >= 30 && g_Player.nPatternAnim <= 34)
				{
					g_Player.nPatternAnim++;
				}
				else if (g_Player.nPatternAnim == 35)
				{
					g_Player.nPatternAnim = 30;
				}
			}
		}
	}

	if (g_Player.bPendulum == true && GetLure()->bStab == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_Player.length -= 5.0f;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_Player.length += 5.0f;
		}

		//糸の長さの限界値を設定
		if (g_Player.length <= MIN_STRING)
		{
			g_Player.length = MIN_STRING;
		}
		else if (g_Player.length >= MAX_STRING)
		{
			g_Player.length = MAX_STRING;
		}
	}
}
//****************************************
// ルアーの操作・移動処理
//****************************************
void LureAction(void)
{
	Lure *pLure = GetLure();
	Rope *pRope = GetRope();

	if (pLure->bStab == false)
	{
		//フックを出す
		if (pLure->bUse == false)
		{
			//指定した方向にフックを撃つ
			if (GetKeyboardPress(DIK_W) == true && GetKeyboardPress(DIK_D) == true && GetKeyboardTrigger(DIK_P) == true)
			{
				g_Player.rot.z = -45.0f;
				SetLure(g_Player.pos, D3DXVECTOR3(20.0f, -20.0f, 0.0f), g_Player.rot);
			}
			else if (GetKeyboardPress(DIK_W) == true && GetKeyboardPress(DIK_A) == true && GetKeyboardTrigger(DIK_P) == true)
			{
				g_Player.rot.z = 45.0f;
				SetLure(g_Player.pos, D3DXVECTOR3(-20.0f, -20.0f, 0.0f), g_Player.rot);
			}
			else if (GetKeyboardPress(DIK_W) == true && GetKeyboardTrigger(DIK_P) == true)
			{
				g_Player.rot.z = 0.0f;
				SetLure(g_Player.pos, D3DXVECTOR3(0.0f, -20.0f, 0.0f), g_Player.rot);
			}
			else if (GetKeyboardPress(DIK_A) == true && GetKeyboardTrigger(DIK_P) == true)
			{

				g_Player.rot.z = 190.0f;
				SetLure(g_Player.pos, D3DXVECTOR3(-20.0f, 0.0f, 0.0f), g_Player.rot);
			}
			else if (GetKeyboardPress(DIK_D) == true && GetKeyboardTrigger(DIK_P) == true)
			{
				g_Player.rot.z = -190.0f;
				SetLure(g_Player.pos, D3DXVECTOR3(20.0f, 0.0f, 0.0f), g_Player.rot);
			}
			else if (GetKeyboardTrigger(DIK_P) == true)
			{
				if (g_Player.nPatternAnim >= 24 && g_Player.nPatternAnim <= 28)
				{
					g_Player.rot.z = -190.0f;
					SetLure(g_Player.pos, D3DXVECTOR3(20.0f, 0.0f, 0.0f), g_Player.rot);
				}
				else if (g_Player.nPatternAnim >= 30 && g_Player.nPatternAnim <= 34)
				{
					g_Player.rot.z = 190.0f;
					SetLure(g_Player.pos, D3DXVECTOR3(-20.0f, 0.0f, 0.0f), g_Player.rot);
				}
			}
			PlaySound(SOUND_LABEL_SE_EXPLOSION); //フック発射音
		}
	}

	if (pLure->bStab == true && g_Player.bAutoLure == true)
	{
		pLure->bWireA = true;
		g_Player.bJump = true;
		if (g_Player.bJump == true)
		{
			g_Player.bLure = true;
		}
		//フックが壁に当たったら自動でその場所に移動する
		if (g_Player.bLure == true && pLure->bWireA == true && g_Player.bAutoLure == true)
		{
			g_Player.pos = (g_Player.pos * (1.0f - (float)(g_Player.nCntReturn / (float)75)) + ((pLure->pos) *(float)(g_Player.nCntReturn / (float)75)));//カウントの前に(float)を入れる

			if (g_Player.pos.x >= pLure->pos.x - SIZE_LURE_WIDTH	* 8 &&
				g_Player.pos.x <= pLure->pos.x + SIZE_LURE_WIDTH	* 8 &&
				g_Player.pos.y >= pLure->pos.y - SIZE_LURE_HEIGHT	* 8 &&
				g_Player.pos.y <= pLure->pos.y + SIZE_LURE_HEIGHT	* 8)
			{
				StopSound(SOUND_LABEL_SE_EXPLOSION); //フックを戻す

				//pLure->bUse = false;
				g_Player.bAutoLure = false;
				g_Player.bPendulum = true;
				pLure->bWireA = false;
			}
			else
			{
				g_Player.nCntReturn++;

				if (CollisionBlockAutoLure(&g_Player.pos, &g_Player.posOld, &g_Player.move) == true)
				{
					pLure->bReturn = true;
				}
			}
		}
	}

	if (pLure->bStab == true && g_Player.bAutoLure == false)
	{
		if (pLure->bStab == true && g_Player.bJump == false)
		{
			//プレイヤーとフックの距離を調べる
			g_Player.length = sqrtf(pow(fabs(pLure->pos.x - g_Player.posOld.x), 2)
				+ pow(fabs(pLure->pos.y - g_Player.posOld.y), 2));

			//糸の限界地を設定
			if (g_Player.length >= MAX_STRING)
			{
				g_Player.length = MAX_STRING;
				g_Player.nCntReturn = 500;
				pLure->bReturn = true;
			}

			g_Player.bCamera = true;

			if (pLure->bWireA == false)
			{
				g_Player.bLure = false;
			}

			//プレイヤーとフックの角度を求める
			/*g_Player.fTwo_Angle = atan2f(((pLure->pos.x - SIZE_LURE_WIDTH) - (g_Player.pos.x - SIZE_WIDTH))
				+ ((pLure->pos.x - SIZE_LURE_WIDTH) + (g_Player.pos.x - SIZE_WIDTH))
				, ((pLure->pos.y - SIZE_LURE_HEIGHT) - (g_Player.pos.y - SIZE_HEIGHT))
				+ ((pLure->pos.y - SIZE_LURE_HEIGHT) + (g_Player.pos.y - SIZE_HEIGHT)));*/

		}

		//振り子
		Pendulum();

		//フックをしまう
		if (GetKeyboardTrigger(DIK_P) == true && pLure->bReturn == false)
		{
			PlaySound(SOUND_LABEL_SE_START1); //フックを戻す

			g_Player.nCntReturn = 30;
			pLure->bReturn = true;
			g_Player.bPendulum = false;

		}

		if (GetKeyboardTrigger(DIK_SPACE) == true && pLure->bReturn == false)
		{
			g_Player.nCntReturn = 30;
			pLure->bReturn = true;
			g_Player.bPendulum = false;

			if (g_Player.rot_spd >= 0.02f || g_Player.rot_spd <= -0.02f)
			{
				//放物線運動を始める
				if (g_Player.bOnce == false)
				{
					g_Player.bInertia = true;
					g_Player.bMeasure2 = false;
					g_Player.px1 = g_Player.px;
					g_Player.py1 = g_Player.py;

					g_Player.bOnce = true;
				}
			}
		}

		//振り子中の当たり判定
		if (CollisionBlockPendulumX(&g_Player.pos, &g_Player.posOld, &g_Player.move) == true)
		{
			g_Player.rot_spd = DEFAULT_FLOAT;
		}
		if (CollisionBlockPendulumY(&g_Player.pos, &g_Player.posOld, &g_Player.move) == true)
		{
			pLure->bReturn = true;
		}
	}

	//フックが帰還する処理
	if (pLure->bReturn == true)
	{
		pLure->nCntReturn++;
		pLure->pos = (pLure->pos * (1.0f - (float)(pLure->nCntReturn / (float)25)) + ((Getplayer()->pos) *(float)(pLure->nCntReturn / (float)25)));//カウントの前に(float)を入れる

		if (g_Player.pos.x >= pLure->pos.x - SIZE_LURE_WIDTH  * 2 &&
			g_Player.pos.x <= pLure->pos.x + SIZE_LURE_WIDTH  * 2 &&
			g_Player.pos.y >= pLure->pos.y - SIZE_LURE_HEIGHT * 2 &&
			g_Player.pos.y <= pLure->pos.y + SIZE_LURE_HEIGHT * 2)
		{
			pLure->bUse = false;
		}
	}

	//ルアーを離した時の処理
	if (g_Player.bInertia == true)
	{
		PendulumEnd();
	}

	//フックが使用されていない状態になったら　ワイヤー関連の情報を全て初期化する
	if (pLure->bUse == false && pLure->bReturn == true)
	{
		StopSound(SOUND_LABEL_SE_START1); //フックを戻す

		g_Player.nCntFriction		= DEFAULT_INT;
		g_Player.nCntReturn			= DEFAULT_INT;
		pLure->nLife				= DEFAULT_INT;
		pLure->nCntReturn			= DEFAULT_INT;

		g_Player.fx					= DEFAULT_FLOAT;
		g_Player.fy					= DEFAULT_FLOAT;
		g_Player.rot1				= DEFAULT_FLOAT;
		g_Player.rot_spd			= DEFAULT_FLOAT;
		g_Player.lengthFixed		= DEFAULT_FLOAT;
		g_Player.g					= DEFAULT_FLOAT;
		g_Player.vx					= DEFAULT_FLOAT;
		g_Player.vy					= DEFAULT_FLOAT;
		g_Player.t					= DEFAULT_FLOAT;
		g_Player.gx					= DEFAULT_FLOAT;
		g_Player.gy					= DEFAULT_FLOAT;
		g_Player.r					= DEFAULT_FLOAT;
		g_Player.sub				= DEFAULT_FLOAT;

		g_Player.friction			= MAX_FRICTION;
		
		g_Player.bCamera	= true;
		g_Player.bMeasure	= false;
		g_Player.bPendulum	= false;
		g_Player.bLure		= false;
		g_Player.bSpace		= false;
		g_Player.bAutoLure	= true;
		pLure->bReturn		= false;
		pLure->bStab		= false;
		pLure->bWireA		= false;
		pRope->bUse			= false;
	}
}
//****************************************
// 振り子の処理
//****************************************
void Pendulum(void)
{
	Lure *pLure = GetLure();

	if (g_Player.bPendulum == true && g_Player.pos.y >= pLure->pos.y)
	{
		//g_Player.bCamera = false;
		//if (g_Player.fTwo_Angle <= 1.6f)
		//{
		//	g_Player.bIdle = false;
		//}
		//else if (g_Player.fTwo_Angle >= 2.4f)
		//{
		//	g_Player.bIdle = true;
		//}
		////fAngle（角度）に移動量をたす
		//if (g_Player.bIdle == false)
		//{
		//	g_Player.fTwo_Angle += 0.02f;
		//}
		//if (g_Player.bIdle == true)
		//{
		//	g_Player.fTwo_Angle -= 0.02f;
		//}
		////１回フックの位置にプレイヤーを動かす
		//g_Player.pos = pLure->pos;
		//g_Player.fTwo_Range -= 0.5;
		//g_Player.move.x = sinf(D3DX_PI * g_Player.fTwo_Angle) * g_Player.lengthFixed;
		//g_Player.move.y = cosf(D3DX_PI * g_Player.fTwo_Angle) * g_Player.lengthFixed;
		//

		//１回だけ計算する
		if (g_Player.bMeasure == false)
		{
			GetLure()->rot.z = 0.0f;

			g_Player.g = 1.0f;

			//プレイヤーとフックの距離を調べる
			g_Player.length = sqrtf(pow(fabs(pLure->pos.x - g_Player.posOld.x), 2)
				+ pow(fabs(pLure->pos.y - g_Player.posOld.y), 2));

			//プレイヤーとフックの距離を調べる
			g_Player.rot1 = atan2f(g_Player.pos.y - pLure->pos.y, g_Player.pos.x - pLure->pos.x) * 180 / D3DX_PI;

			//フックの座標を代入する
			g_Player.fx = pLure->pos.x;
			g_Player.fy = pLure->pos.y;

			//糸の限界地を設定
			if (g_Player.length >= MAX_STRING)
			{
				g_Player.length = MAX_STRING;
			}
			g_Player.bMeasure = true;
		}
		//糸の長さを固定
		g_Player.lengthFixed = g_Player.length;

		//　現在の重りの位置
		g_Player.rad = g_Player.rot1 * D3DX_PI / 180;
		g_Player.px = g_Player.fx + cosf(g_Player.rad) * g_Player.lengthFixed;
		g_Player.py = g_Player.fy + sinf(g_Player.rad) * g_Player.lengthFixed;

		//　重力の移動量を反映した重りの位置
		g_Player.vx = g_Player.px - g_Player.fx;
		g_Player.vy = g_Player.py - g_Player.fy;
		g_Player.t = -(g_Player.vy * g_Player.g) / (g_Player.vx + g_Player.vy * g_Player.vy);
		g_Player.gx = g_Player.px + g_Player.t * g_Player.vx;
		g_Player.gy = g_Player.py + g_Player.g + g_Player.t * g_Player.vy;

		//２つの重りの位置の角度差
		g_Player.r = atan2f(g_Player.gy - g_Player.fy, g_Player.gx - g_Player.fx) * 180 / D3DX_PI;

		//角度差を加速度に加算
		g_Player.sub = g_Player.r - g_Player.rot1;
		g_Player.sub -= floorf(g_Player.sub / 360.0) * 360.0;
		if (g_Player.sub < -180.0) g_Player.sub += 360.0;
		if (g_Player.sub > 180.0) g_Player.sub -= 360.0;
		g_Player.rot_spd += g_Player.sub;

		//摩擦(値を増やすと回転スピードが上がる)
		g_Player.rot_spd += AMOUNT_OF_FRICTION;

		//振り子状態の時　回転スピードを A & D で上げる
		if (GetKeyboardPress(DIK_A) == true)
		{
			if (g_Player.rot_spd >= MIN_FRICTION && g_Player.rot_spd <= MAX_FRICTION / 2)
			{
				g_Player.rot_spd += AMOUNT_OF_FRICTION * 100;
				g_Player.friction += 0.1f;
			}
		}
		if (GetKeyboardPress(DIK_D) == true)
		{
			if (g_Player.rot_spd <= MIN_FRICTION && g_Player.rot_spd >= -MAX_FRICTION / 2)
			{
				g_Player.rot_spd -= AMOUNT_OF_FRICTION * 100;
				g_Player.friction += 0.1f;
			}
		}

		//振り幅の限界値の設定
		if (g_Player.friction >= MAX_FRICTION)
		{
			g_Player.friction = MAX_FRICTION;
		}
		else if (g_Player.friction <= -MAX_FRICTION)
		{
			g_Player.friction = -MAX_FRICTION;
		}

		//摩擦の限界値を設定
		if (g_Player.rot_spd >= g_Player.friction)
		{
			g_Player.rot_spd = g_Player.friction;
		}
		else if (g_Player.rot_spd <= -g_Player.friction)
		{
			g_Player.rot_spd = -g_Player.friction;
		}

		//摩擦の限界値を徐々に減らす
		g_Player.nCntFriction++;

		if (g_Player.nCntFriction % 10 == 0)
		{
			g_Player.friction -= 0.1f;
		}

		//限界値が0以下に行かないようにする
		if (g_Player.friction <= 0.0f)
		{
			g_Player.friction = 0.0f;
		}

		//角度に角速度を加算
		g_Player.rot1 += g_Player.rot_spd;

		//新しい重りの位置
		g_Player.rad = g_Player.rot1 * D3DX_PI / 180;
		g_Player.px = g_Player.fx + cosf(g_Player.rad) * g_Player.lengthFixed;
		g_Player.py = g_Player.fy + sinf(g_Player.rad) * g_Player.lengthFixed;

		//重りの座標
		g_Player.pos.x = g_Player.px;
		g_Player.pos.y = g_Player.py;
	}

}
//****************************************
// 放物線運動
//****************************************
void PendulumEnd(void)
{
	Lure *pLure = GetLure();

	if (g_Player.bMeasure2 == false)
	{
		// X 座標の速度を設定
		g_Player.dx = ((g_Player.pos.x - pLure->pos.x) / 12.5);

		// Ｙ座標の速度を設定
		g_Player.dy = ((pLure->pos.y - g_Player.pos.y) / 12.5);

		//重力加速度の設定　X座標
		g_Player.gx1 = ((g_Player.pos.x - pLure->pos.x) / 1000);

		//重力加速度の設定
		g_Player.gy1 = 1.0f;
		
		g_Player.bMeasure2 = true;
	}

	// X 方向速度に重力加速度を加算
	g_Player.dx -= g_Player.gx1;
	// Y 方向速度に重力加速度を加算
	g_Player.dy += g_Player.gy1;

	// 座標に速度を加算
	g_Player.px1 += g_Player.dx;
	g_Player.py1 += g_Player.dy;

	// 座標をインスタンスの座標に反映
	g_Player.pos.x = g_Player.px1;
	g_Player.pos.y = g_Player.py1;

	// 地面・壁に当たったら放物線運動をやめる
	if (CollisionBlockParabola(&g_Player.pos, &g_Player.posOld, &g_Player.move) == true)
	{
		g_Player.bInertia = false;
	}
}
//****************************************
// プレイヤーの所得
//****************************************
Player *Getplayer(void)
{
	return &g_Player;
}
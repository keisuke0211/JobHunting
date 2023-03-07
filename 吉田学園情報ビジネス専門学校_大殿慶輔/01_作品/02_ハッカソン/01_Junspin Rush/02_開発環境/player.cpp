//==========================================
//
//プレイヤー関係プログラム[player.cpp]
//Author:石原颯馬
//
//==========================================

//==========================================
//
//メモ
//・テクスチャ読み込み部分の "ほげほげ" を使うテクスチャに直す
//・ANIM_PATT_JUMPとANIM_PATT_ROTATEの番号をテクスチャに応じて直す
//
//==========================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "chr_enemy_00.h"
#include "physics.h"
#include "sound.h"

//マクロ
//運動関係
#define ACCELERATION_GRAVITY	(20.8f)				//重力加速度
#define INITIAL_VELOCITY		(12)				//初速
#define ROT_SPEED				(D3DX_PI * 6.0f)	//回転速度
#define TURN_TIME				(45)				//回転時間（フレーム）
#define ATTACK_HIT_TEST_RADIUS	(32.0f)				//攻撃の当たり判定の半径
#define INVINCIBLE_TIME			(60)				//無敵時間

//アニメーション関係
#define PLAYER_PATTERN_WIDTH	(4)					//パターン幅
#define PLAYER_PATTERN_HEIGHT	(2)					//パターン高さ
#define ANIM_SPEED				(8)					//アニメーションパターン切り替え時間
#define ANIM_PATT_JUMP			(1)					//ジャンプパターン番号
#define ANIM_PATT_ROTATE		(4)					//回転パターン番号

//ボタン
#define PUSH_KEY				DIK_RETURN			//押すボタン

//クールタイム
#define CT_TIME					(0)				//クールタイム時間（フレーム)

// 敵[00] との当たり判定
bool CollisionChr_enemy_00(void);

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffPlayer;	//頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayer;		//テクスチャポインタ
Player g_Player;
LPD3DXFONT g_pFontPlayer = NULL;			//フォントへのポインタ

//========================
//プレイヤー初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

#if 1
	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\chr_knight_000.png",
		&g_pTexturePlayer);
#endif

	//移動関係初期化
	g_Player.pos = D3DXVECTOR3(100.0f, 656.0f, 0.0f);

	//回転関係初期化
	//向き初期化
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	//対角線の長さ算出
	g_Player.fLength = sqrtf(PLAYER_SIZE_WIDTH * PLAYER_SIZE_WIDTH + PLAYER_SIZE_HEIGHT * PLAYER_SIZE_HEIGHT) * 0.5f;

	//対角線の角度算出
	g_Player.fAngle = atan2f(PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT);

	//開店時間初期化
	g_Player.nCounterTurn = 0;

	//サイズ初期化
	g_Player.fRotSpeed = 0.0f;

	//体力初期化
	g_Player.nLife = DEFAULT_LIFE;

	//ダメージカウンター初期化
	g_Player.nDamageCounter = 0;

	//アニメーション関係初期化
	g_Player.nCounterAnim = 0;		//カウンタ初期化
	g_Player.nPatternAnim = 0;		//パターンNo.初期化

	//状態初期化
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.bTurn = false;

	//運動関係初期化
	g_Player.fSpeed = 0.0f;
	g_Player.dwExecSkyLastTime = timeGetTime();

	//クールタイム初期化
	g_Player.nCoolTime = 0;

	//生きていることにする
	g_Player.bDisp = true;

	//着地フラグ初期化
	g_Player.bLanding = false;
	g_Player.bChainJump = false;

	//デバッグ表示用フォント生成
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFontPlayer);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffPlayer,
		NULL);

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	//座標変換用係数設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxbuffPlayer->Unlock();
}

//========================
//プレイヤー終了処理
//========================
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxbuffPlayer != NULL)
	{
		g_pVtxbuffPlayer->Release();
		g_pVtxbuffPlayer = NULL;
	}
}

//========================
//プレイヤー更新処理
//========================
void UpdatePlayer(void)
{
	DWORD dwCurrentTime = timeGetTime();

	//カウンタ減算
	g_Player.nCounterAnim++;
	g_Player.nCoolTime--;
	if (g_Player.nDamageCounter > 0) {
		g_Player.nDamageCounter--;
	}


	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:	//通常時
		if (GetKeyboardTrigger(PUSH_KEY) == true)
		{//スペースキーが押された
			g_Player.nPatternAnim = ANIM_PATT_JUMP;		//アニメーションパターンをジャンプ用のものにする
			g_Player.state = PLAYERSTATE_JUMP;			//ジャンプしている状態にする
			g_Player.dwExecSkyLastTime = dwCurrentTime;	//ジャンプ開始の時間取得
			g_Player.bLanding = false;					//着地フラグを偽
			PlaySound(SOUND_LABEL_SE_JUMP_000);
		}

		//アニメーション
		if ((g_Player.nCounterAnim % ANIM_SPEED) == 0)
		{
			g_Player.nCounterAnim = 0;	//カウンタ初期値に戻す

			//パターンNo更新
			g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % 4;
		}

		//自由落下
		g_Player.fSpeed = -ACCELERATION_GRAVITY * (dwCurrentTime - g_Player.dwExecSkyLastTime) / 1000;
		break;
	case PLAYERSTATE_DAMAGE:	//ダメージ時
		break;
	case PLAYERSTATE_JUMP:
		//速度設定
		g_Player.fSpeed = -ACCELERATION_GRAVITY * (dwCurrentTime - g_Player.dwExecSkyLastTime) / 1000 + INITIAL_VELOCITY;

		if (GetKeyboardTrigger(PUSH_KEY) == true &&
			!g_Player.bTurn &&
			(g_Player.nCoolTime <= 0
				||
				g_Player.bChainJump))
		{//スペースキーが押された
			g_Player.nPatternAnim = ANIM_PATT_ROTATE;		//アニメーションパターンを回転用のものにする
			g_Player.bTurn = true;							//回転している状態にする
			g_Player.bChainJump = false;					//2度目以降のジャンプフラグ
			PlaySound(SOUND_LABEL_SE_SLASH_000);
		}

		if (g_Player.fSpeed <= 0.0f)
		{//速度が0になった
			g_Player.state = PLAYERSTATE_FREEFALL;			//落下している状態にする
			g_Player.dwExecSkyLastTime = dwCurrentTime;		//落下開始の時間取得
		}

		break;
	case PLAYERSTATE_FREEFALL:
		if (GetKeyboardTrigger(PUSH_KEY) == true && 
			!g_Player.bTurn && 
			(g_Player.nCoolTime <= 0
				||
				g_Player.bChainJump))
		{//スペースキーが押された
			g_Player.nPatternAnim = ANIM_PATT_ROTATE;		//アニメーションパターンを回転用のものにする
			g_Player.bTurn = true;							//回転している状態にする
			g_Player.bChainJump = false;					//2度目以降のジャンプフラグ
			PlaySound(SOUND_LABEL_SE_SLASH_000);
		}

		//自由落下
		g_Player.fSpeed = -ACCELERATION_GRAVITY * (dwCurrentTime - g_Player.dwExecSkyLastTime) / 1000;
		break;
	}

	//回転
	if (g_Player.bTurn)
	{
		g_Player.nCounterTurn++;
		//回転
		g_Player.rot.z -= ROT_SPEED / TURN_TIME;

		if (g_Player.nCounterTurn >= TURN_TIME
			||
			g_Player.bLanding)
		{//時間経過
			g_Player.rot.z = 0.0f;
			g_Player.bTurn = false;
			g_Player.nCounterTurn = 0;
			g_Player.nCoolTime = CT_TIME;
			g_Player.bChainJump = false;
		}

		if (CollisionChr_enemy_00()) 
		{// 敵との当たり判定が真の時、
			g_Player.state = PLAYERSTATE_JUMP;			//ジャンプしている状態にする
			g_Player.dwExecSkyLastTime = dwCurrentTime;	//ジャンプ開始の時間取得
			g_Player.bChainJump = true;					//2度目以降のジャンプフラグ
		}
	}

	//当たり判定
	if (g_Player.pos.y + ((PLAYER_SIZE_HEIGHT / 2 - PLAYER_SIZE_HEIGHT / 2) * fabsf(cosf(g_Player.rot.z))) + PLAYER_SIZE_HEIGHT / 2 >= SCREEN_HEIGHT - GROUND_HEIGHT
		&& g_Player.state != PLAYERSTATE_JUMP)
	{//地面に着地
		g_Player.fSpeed = 0.0f;					//速度0にする
		g_Player.state = PLAYERSTATE_NORMAL;	//通常状態に戻す

		if (!g_Player.bLanding) 
		{// 着地フラグが偽の時、
			// 着地を真にする
			g_Player.bLanding = true;

			//パターン通常用に戻す
			g_Player.nPatternAnim = 0;
		}

		//位置を地面に合わせる
		g_Player.pos.y = SCREEN_HEIGHT - GROUND_HEIGHT - ((PLAYER_SIZE_HEIGHT / 2 - PLAYER_SIZE_HEIGHT / 2) * fabsf(cosf(g_Player.rot.z)) + PLAYER_SIZE_HEIGHT / 2);
	}

	//位置更新
	g_Player.pos.y -= g_Player.fSpeed;

	//回転慣性
	g_Player.rot.z = (float)fmod(g_Player.rot.z + D3DX_PI + (D3DX_PI * 2) + g_Player.fRotSpeed, D3DX_PI * 2) - D3DX_PI;

#if 0
	//拡縮
	if (GetKeyboardPress(DIK_UP) == true)
	{
		g_Player.move.z += LENGTH_AD;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{
		g_Player.move.z -= LENGTH_AD;
	}

	//拡縮反映
	g_Player.fLength += g_Player.move.z;

	//サイズ減衰
	g_Player.move.z += (0 - g_Player.move.z) * DUMP_COEF;
#endif

	//テクスチャ座標の更新
	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((float)(g_Player.nPatternAnim % PLAYER_PATTERN_WIDTH) / PLAYER_PATTERN_WIDTH,
		(float)(g_Player.nPatternAnim / PLAYER_PATTERN_WIDTH) / PLAYER_PATTERN_HEIGHT);
	pVtx[1].tex = D3DXVECTOR2((float)(g_Player.nPatternAnim % PLAYER_PATTERN_WIDTH + 1) / PLAYER_PATTERN_WIDTH,
		(float)(g_Player.nPatternAnim / PLAYER_PATTERN_WIDTH) / PLAYER_PATTERN_HEIGHT);
	pVtx[2].tex = D3DXVECTOR2((float)(g_Player.nPatternAnim % PLAYER_PATTERN_WIDTH) / PLAYER_PATTERN_WIDTH,
		(float)(g_Player.nPatternAnim / PLAYER_PATTERN_WIDTH + 1) / PLAYER_PATTERN_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2((float)(g_Player.nPatternAnim % PLAYER_PATTERN_WIDTH + 1) / PLAYER_PATTERN_WIDTH,
		(float)(g_Player.nPatternAnim / PLAYER_PATTERN_WIDTH + 1) / PLAYER_PATTERN_HEIGHT);

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f);

	//頂点バッファをアンロック
	g_pVtxbuffPlayer->Unlock();
}

//========================
//プレイヤー描画処理
//========================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxbuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, g_pTexturePlayer);	//g_pTexturePlayer

	if (g_Player.bDisp)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//========================
// 敵[00] との当たり判定
//========================
bool CollisionChr_enemy_00(void) 
{
	Chr_enemy_00 *pChr = GetChr_enemy_00();	// 敵[00] の情報の取得

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++) 
	{
		if (!pChr->bUse) 
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (CheckHit(
			pChr->pos,
			pChr->rot,
			pChr->fHitTestWidth * pChr->scale.fWidth,
			pChr->fHitTestHeight * pChr->scale.fHeight,
			HIT_TEST_TYPE_CIRCLE,
			GetPlayer()->pos,
			GetPlayer()->rot,
			ATTACK_HIT_TEST_RADIUS,
			0.0f,
			HIT_TEST_TYPE_CIRCLE))
		{// 当たり判定内に入った時、
			// ダメージを与える
			HitChr_enemy_00(pChr, 1);

			return true;
		}
	}
	
	return false;
}

//========================
//プレイヤー情報取得
//========================
Player *GetPlayer(void)
{
	return &g_Player;
}
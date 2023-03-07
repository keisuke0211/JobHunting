//==========================================
//
//プレイヤー関係プログラム[player.cpp]
//Author:石原颯馬
//
//==========================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "polygon2D.h"
#include "chr_enemy_00.h"
#include "text.h"
#include "sound.h"
#include "ui_move-text_00.h"
#include "obj_castle_00.h"
#include "eff_particle_00.h"

//マクロ
#define PLAYER_TEXTURE_PATH		"data\\TEXTURE\\Player\\Player-Motion.png"
#define LINE_TEXTURE_PATH		"data\\TEXTURE\\Player\\Player_JudgeLinekari.png"
#define LINE_SIZE_WIDTH			(64)	//ラインのテクスチャサイズ幅
#define LINE_SIZE_HEIGHT		(64)	//ラインのテクスチャサイズ高さ
#define PLAYER_SIZE_WIDTH		(200)	//プレイヤーテクスチャの幅
#define PLAYER_SIZE_HEIGHT		(200)	//プレイヤーテクスチャの高さ
#define PLAYER_PATTERN_WIDTH	(6)		//プレイヤーテクスチャのパターン幅
#define PLAYER_PATTERN_HEIGHT	(1)		//プレイヤーテクスチャのパターン高さ
#define PLAYER_ANIM_COUNT		(30)	//プレイヤーの表示切り替え時間
#define PLAYER_ATK_CENTER		(300)	//判定の中心位置
#define PLAYER_TEXNUM			(2)		//テクスチャ数
#define LINE_POS				D3DXVECTOR3(380.0f, 600.0f, 0.0f)

//判定
#define JUDGE_MOVE_SPEED		(3.0f)	//判定表示の移動量

//スコア関係
#define SCORE_DIGIT				(8)		//スコア桁
#define SCORE_BASE				(100)	//割合100%の時のスコア
#define SCORE_TEXT_NUM			(6 + SCORE_DIGIT + 1)	//スコア表示に使うテキストの表示文字数（"SCORE " + スコア桁 + \n）
#define SCORE_POS				D3DXVECTOR3(SCREEN_WIDTH - 40.0f, 40.0f, 0.0f)	//スコア表示始点

//コンボ関係
#define COMBO_MOVETIME			(30)	//コンボ表示が所定の位置に移動するまでの時間
#define COMBO_DIGIT				(3)		//コンボ桁
#define COMBO_TEXT_NUM			(6 + COMBO_DIGIT + 1)	//コンボ表示に使うテキストの表示文字数（"COMBO " + コンボ桁 + \n）
#define COMBO_TEXT_LENGTH		(40.0f * COMBO_TEXT_NUM)
#define COMBO_POS				SCORE_POS + D3DXVECTOR3(COMBO_TEXT_LENGTH, 40.0f, 0.0f)	//コンボ表示始点の目標位置
#define COMBO_FLASH_PULSE		(4)		//点滅の間隔
#define COMBO_LIGHTING_TIME		(120)	//点滅から点灯に変わる時間

//色関係
#define COLOR_WHITE				{255,255,255,255}	//白
#define COLOR_YELLOW			{255,255,0,255}		//黄色

//アニメーションパターン列挙
typedef enum
{
	ANIMPATT_NONE = 0,
	ANIMPATT_ATK_A,
	ANIMPATT_ATK_B,
	ANIMPATT_ATK_X,
	ANIMPATT_ATK_Y,
	ANIMPATT_CLEAR,
	ANIMPATT_MAX
} ANIMPATT;

//プロト
void AttackPlayer(Chr_enemy_00 *pEnemy);

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffPlayer;	//頂点バッファポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayer[PLAYER_TEXNUM];		//テクスチャポインタ
Player g_Player;
LPD3DXFONT g_pFontPlayer = NULL;			//フォントへのポインタ
int g_nCounterAppearTime = 0;				//出現時の点滅時間
int g_nCounterFlashTime = 0;				//点滅時間

//判定範囲とスコア割合
const int c_aJudgeWidth[JUDGE_MAX] = { 10, 20, 30 };				//判定範囲（判定きついとかゆるいとかあったらここいじって）
const float c_aJudgeScoreRatio[JUDGE_MAX] = { 1.0f, 0.5f, 0.0f };	//判定に対するスコア割合（1.0f = 100%）
const int c_aAnimPattern[ANIMPATT_MAX] = { 0,3,4,1,2,5 };			//アニメーションパターン番号

//========================
//プレイヤー初期化処理
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ

	pDevice = GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEXTURE_PATH,
		&g_pTexturePlayer[0]);

	D3DXCreateTextureFromFile(pDevice,
		LINE_TEXTURE_PATH,
		&g_pTexturePlayer[1]);

	//頂点バッファの生成（頂点数*プレイヤーと線）
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffPlayer,
		NULL);

	//プレーヤー召喚
	SetPlayer();
}

//========================
//プレイヤー終了処理
//========================
void UninitPlayer(void)
{
	for (int nCntUninit = 0; nCntUninit < PLAYER_TEXNUM; nCntUninit++)
	{
		//テクスチャの破棄
		if (g_pTexturePlayer[nCntUninit] != NULL)
		{
			g_pTexturePlayer[nCntUninit]->Release();
			g_pTexturePlayer[nCntUninit] = NULL;
		}
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
	Chr_enemy_00 *pEnemy = GetChr_enemy_00();
	char aDispScoreText[SCORE_TEXT_NUM] = "SCORE ";
	char aDispComboText[COMBO_TEXT_NUM] = "COMBO ";
	char aScoreNumText[SCORE_DIGIT];
	char aComboNumText[COMBO_DIGIT];
	bool bPushSameButton = false;

	//アニメーションカウンタ増やす
	g_Player.nCounterAnim++;

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//カウンタが一定の値に達したら表示を戻す
	if (g_Player.nCounterAnim >= PLAYER_ANIM_COUNT)
	{//戻す
		//テクスチャ設定
		SetTexturePos2D(
			pVtx,
			c_aAnimPattern[ANIMPATT_NONE],
			PLAYER_PATTERN_WIDTH,
			PLAYER_PATTERN_HEIGHT,
			NULL);
	}

	for (int nCntEnemy = 0; nCntEnemy < CHR_ENEMY_00_MAX; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->nCounterMove >= PLAYER_ATK_CENTER - c_aJudgeWidth[JUDGE_MISS] && pEnemy->nCounterMove <= PLAYER_ATK_CENTER + c_aJudgeWidth[JUDGE_MISS])
			{//プレイヤーの攻撃範囲内に敵がいる（ミス判定の範囲）
				Chr_enemy_00Type *pEnemyType = &GetChr_enemy_00Type()[pEnemy->nType];

				//ボタン（ABXY）の情報取得
				bPushSameButton = GetButtonTrigger(BUTTON_A + pEnemyType->elem);	//Aボタンを基点として属性と対応するボタンを取得

				//属性と対応するボタンが押されたら倒す
				if (bPushSameButton == true)
				{//倒す処理
					AttackPlayer(pEnemy);	//攻撃（判定）処理

					//テクスチャ設定
					SetTexturePos2D(
						pVtx,
						c_aAnimPattern[ANIMPATT_ATK_A + pEnemyType->elem],
						PLAYER_PATTERN_WIDTH,
						PLAYER_PATTERN_HEIGHT,
						NULL);

					//カウンターリセット
					g_Player.nCounterAnim = 0;
					g_Player.nCounterComboMoveTime = 0;

					//点滅時間リセット
					g_nCounterFlashTime = 0;
				}
			}
		}
	}

	//スコアテキスト設定
	snprintf(&aScoreNumText[0], SCORE_DIGIT * 4, "%d", g_Player.nScore);
	strcat(&aDispScoreText[0], &aScoreNumText[0]);
	SetText2D(&aDispScoreText[0], 0, DISPLAY_RIGHT, D3DXVECTOR3(SCREEN_WIDTH - 40.0f, 40.0f, 0.0f), { 255,255,255,255 }, 1.0f, 1.0f);

	//コンボテキスト設定
	if (g_Player.nCombo > 0)
	{//1コンボ以上なら動かす
		g_Player.nCounterComboMoveTime++;	//カウント増やす
		g_nCounterFlashTime++;				//点滅カウント増やす

		float fRate = (float)g_Player.nCounterComboMoveTime / COMBO_MOVETIME;	//割合計算
		if (fRate > 1.0f)
		{//所定の位置を超えないように修正
			fRate = 1.0f;
		}

		//表示
		snprintf(&aComboNumText[0], COMBO_DIGIT * 4, "%d", g_Player.nCombo);
		strcat(&aDispComboText[0], &aComboNumText[0]);

		//色調整
		Color color = COLOR_WHITE;
		if (g_nCounterFlashTime / COMBO_FLASH_PULSE % 2 == 0 || g_nCounterFlashTime >= COMBO_LIGHTING_TIME)	//2フレームに1回または60フレーム後
		{
			color = COLOR_YELLOW;
		}

		//色調整後に文字表示
		SetText2D(&aDispComboText[0], 0, DISPLAY_RIGHT, COMBO_POS + D3DXVECTOR3(-COMBO_TEXT_LENGTH * fRate, 0.0f, 0.0f), color, 1.0f, 1.0f);
	}

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

	//テクスチャ設定と描画
	for (int nCntDraw = 0; nCntDraw < PLAYER_TEXNUM; nCntDraw++)
	{
		//テクスチャ設定
		pDevice->SetTexture(0, g_pTexturePlayer[nCntDraw]);

		if (g_Player.bDisp)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntDraw, 2);
		}
	}
}

//========================
//プレイヤー情報取得
//========================
Player *GetPlayer(void)
{
	return &g_Player;
}

//========================
//プレイヤー召喚処理
//========================
void SetPlayer(void)
{
	//アニメーション関係初期化
	g_Player.nCounterAnim = 0;		//カウンタ初期化
	g_Player.nPatternAnim = 0;		//パターンNo.初期化

	//移動関係初期化
	g_Player.pos = D3DXVECTOR3(340.0f, 520.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//回転関係初期化
	//向き初期化
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//状態初期化
	g_nCounterAppearTime = 0;

	//体力・スコア初期化
	g_Player.nLife = 5;
	g_Player.nScore = 0;
	g_Player.nCombo = 0;

	//状態カウンタ初期化
	g_Player.nCounterAttack = 0;

	//生きていることにする
	g_Player.bDisp = true;

	VERTEX_2D *pVtx;	//設定用ポインタ

	//頂点バッファのロックと頂点情報へのポインタを取得
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//プレイヤーの情報設定
	//頂点座標の設定
	SetVertexPos2D(
		pVtx,
		g_Player.pos,
		INITD3DXVECTOR3,
		false,
		PLAYER_SIZE_WIDTH,
		PLAYER_SIZE_HEIGHT,
		ANGLE_TYPE_FIXED);

	//座標変換用係数設定
	SetRHW2D(pVtx);

	//頂点カラー
	Color playerColor = { 255,255,255,255 };
	SetVertexColor2D(pVtx, playerColor);

	//テクスチャ設定
	SetTexturePos2D(
		pVtx,
		ANIMPATT_NONE,
		PLAYER_PATTERN_WIDTH,
		PLAYER_PATTERN_HEIGHT,
		NULL);

	//線の情報設定
	pVtx += 4;
	//頂点座標の設定
	SetVertexPos2D(
		pVtx,
		LINE_POS,
		INITD3DXVECTOR3,
		false,
		LINE_SIZE_WIDTH,
		LINE_SIZE_HEIGHT,
		ANGLE_TYPE_FIXED);

	//座標変換用係数設定
	SetRHW2D(pVtx);

	//頂点カラー
	SetVertexColor2D(pVtx, playerColor);

	//テクスチャ設定
	SetTexturePos2D(
		pVtx,
		0,
		1,
		1,
		NULL);

	//頂点バッファをアンロック
	g_pVtxbuffPlayer->Unlock();

	//グローバルの点滅時間リセット
	g_nCounterFlashTime = 0;
}

//========================
//プレイヤー攻撃処理（もとい判定とスコア加算・ダメージ）
//========================
void AttackPlayer(Chr_enemy_00 *pEnemy)
{
	PlaySound(5);

	int nDistEnemy = pEnemy->nCounterMove - PLAYER_ATK_CENTER;

	if (abs(nDistEnemy) <= c_aJudgeWidth[JUDGE_EXELENT])
	{//EXELENTの時の処理
		g_Player.nScore += SCORE_BASE * c_aJudgeScoreRatio[JUDGE_EXELENT];	//スコア加算
		g_Player.nCombo++;
		SetUi_moveText_00("EXELENT!", 1, g_Player.pos, D3DXVECTOR3(0.0f, -3.0f, 0.0f), { 0,0,0,255 }, 60);	//動く文字設定
		SetEff_particle_00(pEnemy->pos, INITD3DXVECTOR3, (EFF_PARTICLE_00_TYPE)pEnemy->nType);
	}
	else if (abs(nDistEnemy) <= c_aJudgeWidth[JUDGE_GOOD])
	{//GOODの時の処理
		g_Player.nScore += SCORE_BASE * c_aJudgeScoreRatio[JUDGE_GOOD];		//スコア加算
		g_Player.nCombo++;
		SetUi_moveText_00("GOOD", 1, g_Player.pos, D3DXVECTOR3(0.0f, -3.0f, 0.0f), { 0,0,0,255 }, 60);		//動く文字設定
		SetEff_particle_00(pEnemy->pos, INITD3DXVECTOR3, (EFF_PARTICLE_00_TYPE)pEnemy->nType);
	}
	else
	{//MISSの時の処理
		g_Player.nLife--;		//体力減らす
		g_Player.nCombo = 0;	//コンボリセット
		g_Player.nCounterComboMoveTime = 0;
		SetUi_moveText_00("MISS...", 1, g_Player.pos, D3DXVECTOR3(0.0f, -3.0f, 0.0f), { 0,0,0,255 }, 60);	//動く文字設定
		SetDamageObj_castle_00();	// 城のダメージ設定
	}
	pEnemy->bUse = false;
}
//========================================
// 
// 敵[00] の処理
// Author:西村 吏功
// 
//========================================
// *** chr_enemy_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "sound.h"
#include "physics.h"
#include "polygon.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "chr_enemy_00.h"
#include "eff_general_00.h"
#include "md_game_00.h"
#include "player.h"
#include "attack.h"
#include "fade.h"
#include "score.h"
#include "combo.h"
#include <stdio.h>
#include <float.h>

//****************************************
// マクロ定義
//****************************************
// 敵[00] のダメージ状態の時間
// 敵[00] の出現にかかる時間
// 敵[00] の消えるのにかかる時間
// 敵[00] の爆発耐性にかかる時間
// 敵[00] の膨らみにかかる時間
// 敵[00] の膨らみにかかる倍率
// 敵[00] の種類毎の情報のデータファイルの相対パス
// 敵[00] の強前進時の移動量にかかる倍率
// 敵[00] の弱前進時の移動量にかかる倍率
// 敵[00] のノックバック力
// 敵[00] の縮小量
#define CHR_ENEMY_00_DAMAGE_TIME					(20)
#define CHR_ENEMY_00_POP_TIME						(40)
#define CHR_ENEMY_00_CLEAR_TIME						(20)
#define CHR_ENEMY_00_EXPLOSION_RESISTANCE_TIME		(60)
#define CHR_ENEMY_00_SWELL_TIME						(8)
#define CHR_ENEMY_00_SWELL_DIAMETER					(1.4f)
#define CHR_ENEMY_00_TYPE_DATA_FILE_PATH			"data\\GAMEDATA\\CHR_ENEMY_00_TYPE_DATA.txt"
#define CHR_ENEMY_00_STRONG_MOVE_DIAMETER			(2.0f)
#define CHR_ENEMY_00_LIGHT_MOVE_DIAMETER			(0.1f)
#define CHR_ENEMY_00_KNOCK_BACK_POWER				(20.0f)
#define CHR_ENEMY_00_SCALE_DOWN						(0.01f)
#define CHR_ENEMY_00_SCORE_DIAMETER_ADDEND_CHAIN	(0.5f)

//****************************************
// プロトタイプ宣言
//****************************************

// 文字列から行動を読み込む
void StringLoadActLoad(char aString[TXT_MAX], CHR_ENEMY_00_ACT_LOAD *pActLoad);

// パラメーターの初期化処理
void InitParameter(Chr_enemy_00 *pChr);

// パラメーターを種類に応じて設定
void SetParameter(Chr_enemy_00 *pChr);

// 色の設定処理
void SetColor(VERTEX_2D *pVtx, Chr_enemy_00 *pChr);

// 行動読み込み処理
void ActLoading(Chr_enemy_00 *pChr);

// 行動処理
void ActProcess(Chr_enemy_00 *pChr);

// サウンド読み込み処理
void SoundLoading(Chr_enemy_00 *pChr);

// パターンNo.読み込み処理
void PatternLoading(Chr_enemy_00 *pChr);

// エフェクト読み込み処理
void EffectLoading(Chr_enemy_00 *pChr);

// 発射読み込み処理
void ShotLoading(Chr_enemy_00 *pChr);

// 召喚読み込み処理
void SummonLoading(Chr_enemy_00 *pChr);

// 例外召喚処理
void SummonException(Chr_enemy_00 *pChr);

// 召喚処理
void Summon(Chr_enemy_00 *pChr, int nCntSummon);

// 状態処理
void StateProcess(Chr_enemy_00 *pChr);

// 子の移動処理
void ChildMoveProcess(Chr_enemy_00 *pChr, D3DXVECTOR3 posTemp, D3DXVECTOR3 rotTemp);

// 子の破壊処理
void ChildDestruction(Chr_enemy_00 *pChr);

// 子をダメージ状態にする
void ChildDamageState(Chr_enemy_00 *pChr);

// 機体[00] との当たり判定処理
void CollisionChr_fighter_00(Chr_enemy_00 *pChr);

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureChr_enemy_00		// テクスチャへのポインタ
						[CHR_ENEMY_00_TYPE_MAX]		//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChr_enemy_00		// 頂点バッファへのポインタ
						= NULL;						//
Chr_enemy_00			g_aChr_enemy_00				// 敵[00] の情報
						[CHR_ENEMY_00_MAX];			//
Chr_enemy_00Type		g_aChr_enemy_00Type			// 敵[00] の種類毎の情報
						[CHR_ENEMY_00_TYPE_MAX];	//

// 敵[00] の行動読み込みテキスト
char g_aActText[CHR_ENEMY_00_ACT_LOAD_MAX][TXT_MAX] =
{
	"前進開始",
	"前進終了",
	"強前進開始",
	"強前進終了",
	"弱前進開始",
	"弱前進終了",
	"後進開始",
	"後進終了",
	"強後進開始",
	"強後進終了",
	"弱後進開始",
	"弱後進終了",
	"推進",
	"左スライド開始",
	"左スライド終了",
	"右スライド開始",
	"右スライド終了",
	"プレイヤー方向転換開始",
	"プレイヤー方向転換終了",
	"プレイヤー追尾開始",
	"プレイヤー追尾終了",
	"プレイヤー凝視開始",
	"プレイヤー凝視終了",
	"中心凝視開始",
	"中心凝視終了",
	"すり抜け開始",
	"すり抜け終了",
	"弾すり抜け開始",
	"弾すり抜け終了",
	"無敵開始",
	"無敵終了",
	"ノックバック開始",
	"ノックバック終了",
	"縮小開始",
	"縮小終了",
	"死亡時召喚",
	"親離れ",
	"即出現",
	"即消える",
	"消える",
	"ループ",
};

//========== *** 敵[00] の情報を取得 ***
Chr_enemy_00 *GetChr_enemy_00(void)
{
	return g_aChr_enemy_00;
}

//========== *** 敵[00] の種類毎の情報を取得 ***
Chr_enemy_00Type *GetChr_enemy_00Type(void)
{
	return g_aChr_enemy_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StringLoadActLoad関数 - 文字列から行動の読み込みを読み込む -
//========================================
void StringLoadActLoad(char aString[TXT_MAX], CHR_ENEMY_00_ACT_LOAD *pActLoad)
{
	for (int nCntAct = 0; nCntAct < CHR_ENEMY_00_ACT_LOAD_MAX; nCntAct++)
	{
		if (strcmp(aString, g_aActText[nCntAct]) == 0)
		{// 行動の読み込みテキストと一致した時、
			// 行動の読み込みの種類を設定
			*pActLoad = (CHR_ENEMY_00_ACT_LOAD)nCntAct;
		}
	}
}

//========================================
// InitParameter関数 - パラメーターの初期化処理 -
//========================================
void InitParameter(Chr_enemy_00 *pChr) 
{
	pChr->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	pChr->move					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
	pChr->fMove					= 0.0f;								// 移動力
	pChr->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	pChr->targetRot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標向き
	pChr->fWidth				= 0.0f;								// 幅
	pChr->fHeight				= 0.0f;								// 高さ
	pChr->fHitTestWidth			= 0.0f;								// 当たり判定の幅
	pChr->fHitTestHeight		= 0.0f;								// 当たり判定の高さ
	pChr->scale					= { 1.0f,1.0f };					// 大きさの倍率
	pChr->pTrackingPos			= NULL;								// 追尾位置
	pChr->pStarePos				= NULL;								// 疑視位置
	pChr->nType					= 0;								// 種類
	pChr->bUse					= false;							// 使用されているかフラグ
	pChr->bBomb					= false;							// 爆弾所持フラグ
	pChr->bReceivedExplosion	= false;							// 爆発を受けたフラグ
	pChr->nLife					= 0;								// 体力

	for (int nCntState = 0; nCntState < CHR_ENEMY_00_STATE_MAX; nCntState++)
	{
		pChr->aState[nCntState]			= false;	// 状態フラグ
		pChr->aCounterState[nCntState]	= 0;		// 状態管理カウンター
	}

	pChr->nPattern		= 0;	// パターンNo.
	pChr->nCounterAnim	= 0;	// アニメーションのカウンター
	pChr->nCounterAct	= 0;	// 行動カウンター

	for (int nCntAct = 0; nCntAct < CHR_ENEMY_00_ACT_MAX; nCntAct++) 
	{
		pChr->aAct[nCntAct] = false;	// 行動フラグ
	}

	pChr->nCounterSound		= 0;	// サウンドカウンター
	pChr->nCounterPattern	= 0;	// パターンNo.カウンター
	pChr->nCounterEffect	= 0;	// エフェクトカウンター
	pChr->nCounterShot		= 0;	// 発射カウンター
	pChr->nCounterSummon	= 0;	// 召喚カウンター
	pChr->nCntChain			= 0;	// 連鎖のカウント
}

//========================================
// SetParameter関数 - パラメーターを種類に応じて設定 -
//========================================
void SetParameter(Chr_enemy_00 *pChr)
{
	pChr->fWidth			= g_aChr_enemy_00Type[pChr->nType].fWidth;			// 幅
	pChr->fHeight			= g_aChr_enemy_00Type[pChr->nType].fHeight;			// 高さ
	pChr->fHitTestWidth		= g_aChr_enemy_00Type[pChr->nType].fHitTestWidth;	// 当たり判定の幅
	pChr->fHitTestHeight	= g_aChr_enemy_00Type[pChr->nType].fHitTestHeight;	// 当たり判定の高さ
	pChr->fMove				= g_aChr_enemy_00Type[pChr->nType].fMove;			// 移動力
	pChr->fRotation			= g_aChr_enemy_00Type[pChr->nType].fRotation;		// 回転力
	pChr->nLife				= g_aChr_enemy_00Type[pChr->nType].nLife;			// 体力
}

//========================================
// SetColor関数 - 色の設定処理 -
//========================================
void SetColor(VERTEX_2D *pVtx, Chr_enemy_00 *pChr)
{
	float	fRed	// 不赤度
			= 1.0f;

	if (pChr->aState[CHR_ENEMY_00_STATE_DAMAGE])
	{// ダメージ状態の時、
		// 不赤度を設定
		fRed = (float)pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] / (float)CHR_ENEMY_00_DAMAGE_TIME;
	}
	else if (pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
	{// 消え中状態の時、
		// 不赤度を設定
		fRed = (float)pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR] / (float)CHR_ENEMY_00_CLEAR_TIME;
	}

	// 頂点色の設定
	SetVertexColor(pVtx,
	{
		(int)(255),
		(int)(255 * fRed),
		(int)(255 * fRed),
		(int)(255 * ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP] / (float)CHR_ENEMY_00_POP_TIME)
		* (1.0f - ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR] / (float)CHR_ENEMY_00_CLEAR_TIME)))
	});
}

//========================================
// ActLoading関数 - 行動読み込み処理 -
//========================================
void ActLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT]))
	{// 膨らみ状態/膨らみ待ち状態のいずれかの時、
		// 処理を終了する
		return;
	}

	for (int nCntAct = 0; nCntAct < g_aChr_enemy_00Type[pChr->nType].nActNum; nCntAct++)
	{
		if (pChr->nCounterAct == g_aChr_enemy_00Type[pChr->nType].aAct[nCntAct].nTime)
		{// 行動カウンター行動する時間と一致した時、
			switch (g_aChr_enemy_00Type[pChr->nType].aAct[nCntAct].type)
			{
				//========== *** 前進開始 ***
			case CHR_ENEMY_00_ACT_LOAD_PROGRESS_START:

				// 前進フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_PROGRESS] = true;

				break;
				//========== *** 前進終了 ***
			case CHR_ENEMY_00_ACT_LOAD_PROGRESS_END:

				// 前進フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_PROGRESS] = false;

				break;
				//========== *** 強前進開始 ***
			case CHR_ENEMY_00_ACT_LOAD_STRONG_PROGRESS_START:

				// 強前進フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_STRONG_PROGRESS] = true;

				break;
				//========== *** 強前進終了 ***
			case CHR_ENEMY_00_ACT_LOAD_STRONG_PROGRESS_END:

				// 強前進フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_STRONG_PROGRESS] = false;

				break;
				//========== *** 弱前進開始 ***
			case CHR_ENEMY_00_ACT_LOAD_LIGHT_PROGRESS_START:

				// 弱前進フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_LIGHT_PROGRESS] = true;

				break;
				//========== *** 弱前進終了 ***
			case CHR_ENEMY_00_ACT_LOAD_LIGHT_PROGRESS_END:

				// 弱前進フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_LIGHT_PROGRESS] = false;

				break;
				//========== *** 後進開始 ***
			case CHR_ENEMY_00_ACT_LOAD_BACKWARD_START:

				// 後進フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_BACKWARD] = true;

				break;
				//========== *** 後進終了 ***
			case CHR_ENEMY_00_ACT_LOAD_BACKWARD_END:

				// 後進フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_BACKWARD] = false;

				break;
				//========== *** 強後進開始 ***
			case CHR_ENEMY_00_ACT_LOAD_STRONG_BACKWARD_START:

				// 強後進フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_STRONG_BACKWARD] = true;

				break;
				//========== *** 強後進終了 ***
			case CHR_ENEMY_00_ACT_LOAD_STRONG_BACKWARD_END:

				// 強後進フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_STRONG_BACKWARD] = false;

				break;
				//========== *** 弱後進開始 ***
			case CHR_ENEMY_00_ACT_LOAD_LIGHT_BACKWARD_START:

				// 弱後進フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_LIGHT_BACKWARD] = true;

				break;
				//========== *** 弱後進終了 ***
			case CHR_ENEMY_00_ACT_LOAD_LIGHT_BACKWARD_END:

				// 弱後進フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_LIGHT_BACKWARD] = false;

				break;
				//========== *** 推進 ***
			case CHR_ENEMY_00_ACT_LOAD_DRIVING:

				// 移動量を更新
				pChr->move.x += sinf(pChr->rot.z) * pChr->fMove;
				pChr->move.y += cosf(pChr->rot.z) * pChr->fMove;

				break;
				//========== *** 左スライド開始 ***
			case CHR_ENEMY_00_ACT_LOAD_SLIDE_LEFT_START:

				// 左スライドフラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_SLIDE_LEFT] = true;

				break;
				//========== *** 左スライド終了 ***
			case CHR_ENEMY_00_ACT_LOAD_SLIDE_LEFT_END:

				// 左スライドフラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_SLIDE_LEFT] = false;

				break;
				//========== *** 右スライド開始 ***
			case CHR_ENEMY_00_ACT_LOAD_SLIDE_RIGHT_START:

				// 右スライドフラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_SLIDE_RIGHT] = true;

				break;
				//========== *** 右スライド終了 ***
			case CHR_ENEMY_00_ACT_LOAD_SLIDE_RIGHT_END:

				// 右スライドフラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_SLIDE_RIGHT] = false;

				break;
				//========== *** プレイヤー方向転換開始 ***
			case CHR_ENEMY_00_ACT_LOAD_TURN_AROUND_TO_PLAYER_START:

				// 方向転換位置を設定
				pChr->pTurnAroundPos = &GetPlayer()->pos;

				// 方向転換フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_TURN_AROUND] = true;

				break;
				//========== *** プレイヤー方向転換終了 ***
			case CHR_ENEMY_00_ACT_LOAD_TURN_AROUND_TO_PLAYER_END:

				// 方向転換フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_TURN_AROUND] = false;

				break;
				//========== *** プレイヤー追尾開始 ***
			case CHR_ENEMY_00_ACT_LOAD_TRACKING_PLAYER_START:

				// 追尾位置を設定
				pChr->pTrackingPos = &GetPlayer()->pos;

				// 追尾フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_TRACKING] = true;

				break;
				//========== *** プレイヤー追尾終了 ***
			case CHR_ENEMY_00_ACT_LOAD_TRACKING_PLAYER_END:

				// 追尾フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_TRACKING] = false;

				break;
				//========== *** プレイヤー凝視開始 ***
			case CHR_ENEMY_00_ACT_LOAD_STARE_PLAYER_START:

				// 凝視位置を設定
				// ★

				// 凝視フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_STARE] = true;

				break;
				//========== *** プレイヤー凝視終了 ***
			case CHR_ENEMY_00_ACT_LOAD_STARE_PLAYER_END:

				// 凝視フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_STARE] = false;

				break;
				//========== *** 中心凝視開始 ***
			case CHR_ENEMY_00_ACT_LOAD_STARE_CENTER_START:

				// 凝視位置を設定
				// ★

				// 凝視フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_STARE] = true;

				break;
				//========== *** 中心凝視終了 ***
			case CHR_ENEMY_00_ACT_LOAD_STARE_CENTER_END:

				// 凝視フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_STARE] = false;

				break;
				//========== *** すり抜け開始 ***
			case CHR_ENEMY_00_ACT_LOAD_GHOST_START:

				// すり抜けフラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_GHOST] = true;

				break;
				//========== *** すり抜け終了 ***
			case CHR_ENEMY_00_ACT_LOAD_GHOST_END:

				// すり抜けフラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_GHOST] = false;

				break;
				//========== *** 弾すり抜け開始 ***
			case CHR_ENEMY_00_ACT_LOAD_BULLET_GHOST_START:

				// 弾すり抜けフラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_BULLET_GHOST] = true;

				break;
				//========== *** 弾すり抜け終了 ***
			case CHR_ENEMY_00_ACT_LOAD_BULLET_GHOST_END:

				// 弾すり抜けフラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_BULLET_GHOST] = false;

				break;
				//========== *** 無敵開始 ***
			case CHR_ENEMY_00_ACT_LOAD_INVINCIBLE_START:

				// 無敵フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE] = true;

				break;
				//========== *** 無敵終了 ***
			case CHR_ENEMY_00_ACT_LOAD_INVINCIBLE_END:

				// 無敵フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE] = false;

				break;
				//========== *** ノックバック開始 ***
			case CHR_ENEMY_00_ACT_LOAD_KNOCK_BACK_START:

				// ノックバックフラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_KNOCK_BACK] = true;

				break;
				//========== *** ノックバック終了 ***
			case CHR_ENEMY_00_ACT_LOAD_KNOCK_BACK_END:

				// ノックバックフラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_KNOCK_BACK] = false;

				break;
				//========== *** 縮小開始 ***
			case CHR_ENEMY_00_ACT_LOAD_SCALE_DOWN_START:

				// 縮小フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_SCALE_DOWN] = true;

				break;
				//========== *** 縮小終了 ***
			case CHR_ENEMY_00_ACT_LOAD_SCALE_DOWN_END:

				// 縮小フラグを偽にする
				pChr->aAct[CHR_ENEMY_00_ACT_SCALE_DOWN] = false;

				break;
				//========== *** 死亡時召喚 ***
			case CHR_ENEMY_00_ACT_LOAD_SUMMONING_ON_DIED:

				// 死亡時召喚フラグを真にする
				pChr->aAct[CHR_ENEMY_00_ACT_SUMMONING_ON_DIED] = true;

				break;
				//========== *** 親離れ ***
			case CHR_ENEMY_00_ACT_LOAD_CHILD_RELEASE:

				// 子番号を-1にする
				pChr->nChildNum = -1;

				break;
				//========== *** 即出現 ***
			case CHR_ENEMY_00_ACT_LOAD_IMMEDIATE_POP:

				// 出現中にする
				pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP] = CHR_ENEMY_00_POP_TIME;
				pChr->aState[CHR_ENEMY_00_STATE_IN_POP] = false;

				break;
				//========== *** 即消える ***
			case CHR_ENEMY_00_ACT_LOAD_IMMEDIATE_CLEAR:

				// 使用されていない状態にする
				pChr->bUse = false;

				break;
				//========== *** 消える ***
			case CHR_ENEMY_00_ACT_LOAD_CLEAR:

				if (!pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]) 
				{// 消え中でない時、
					// 消え中にする
					pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]			= true;
					pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR]	= 0;
				}

				break;
				//========== *** ループ ***
			case CHR_ENEMY_00_ACT_LOAD_LOOP:

				// 行動の時間を初期化する
				pChr->nCounterAct = -1;

				break;
			}
		}
	}

	// 行動カウンターを加算
	pChr->nCounterAct++;

	// 行動処理
	ActProcess(pChr);
}

//========================================
// ActProcess関数 - 行動処理 -
//========================================
void ActProcess(Chr_enemy_00 *pChr)
{
	for (int nCntAct = 0; nCntAct < CHR_ENEMY_00_ACT_MAX; nCntAct++)
	{
		if (pChr->aAct[nCntAct])
		{// その状態が真の時

			switch (nCntAct)
			{
				//========== *** 前進 ***
			case CHR_ENEMY_00_ACT_PROGRESS:

				// 位置を更新
				pChr->pos.x += sinf(pChr->rot.z) * pChr->fMove;
				pChr->pos.y += cosf(pChr->rot.z) * pChr->fMove;

				break;
				//========== *** 強前進 ***
			case CHR_ENEMY_00_ACT_STRONG_PROGRESS:

				// 位置を更新
				pChr->pos.x += sinf(pChr->rot.z) * pChr->fMove * CHR_ENEMY_00_STRONG_MOVE_DIAMETER;
				pChr->pos.y += cosf(pChr->rot.z) * pChr->fMove * CHR_ENEMY_00_STRONG_MOVE_DIAMETER;

				break;
				//========== *** 弱前進 ***
			case CHR_ENEMY_00_ACT_LIGHT_PROGRESS:

				// 位置を更新
				pChr->pos.x += sinf(pChr->rot.z) * pChr->fMove * CHR_ENEMY_00_LIGHT_MOVE_DIAMETER;
				pChr->pos.y += cosf(pChr->rot.z) * pChr->fMove * CHR_ENEMY_00_LIGHT_MOVE_DIAMETER;

				break;
				//========== *** 後進 ***
			case CHR_ENEMY_00_ACT_BACKWARD:

				// 位置を更新
				pChr->pos.x += sinf(pChr->rot.z + D3DX_PI) * pChr->fMove;
				pChr->pos.y += cosf(pChr->rot.z + D3DX_PI) * pChr->fMove;

				break;
				//========== *** 強後進 ***
			case CHR_ENEMY_00_ACT_STRONG_BACKWARD:

				// 位置を更新
				pChr->pos.x += sinf(pChr->rot.z + D3DX_PI) * pChr->fMove * CHR_ENEMY_00_STRONG_MOVE_DIAMETER;
				pChr->pos.y += cosf(pChr->rot.z + D3DX_PI) * pChr->fMove * CHR_ENEMY_00_STRONG_MOVE_DIAMETER;

				break;
				//========== *** 弱後進 ***
			case CHR_ENEMY_00_ACT_LIGHT_BACKWARD:

				// 位置を更新
				pChr->pos.x += sinf(pChr->rot.z + D3DX_PI) * pChr->fMove * CHR_ENEMY_00_LIGHT_MOVE_DIAMETER;
				pChr->pos.y += cosf(pChr->rot.z + D3DX_PI) * pChr->fMove * CHR_ENEMY_00_LIGHT_MOVE_DIAMETER;

				break;
				//========== *** 左スライド ***
			case CHR_ENEMY_00_ACT_SLIDE_LEFT:

				// 位置を更新
				pChr->pos.x += sinf(D3DX_PI * -0.5f) * pChr->fMove;
				pChr->pos.y += cosf(D3DX_PI * -0.5f) * pChr->fMove;

				break;
				//========== *** 右スライド ***
			case CHR_ENEMY_00_ACT_SLIDE_RIGHT:

				// 位置を更新
				pChr->pos.x += sinf(pChr->rot.z + (D3DX_PI * -0.5f)) * pChr->fMove;
				pChr->pos.y += cosf(pChr->rot.z + (D3DX_PI * -0.5f)) * pChr->fMove;

				break;
				//========== *** 方向転換 ***
			case CHR_ENEMY_00_ACT_TURN_AROUND:

				// 方向転換処理
				TurnAround(
					&pChr->rot,
					pChr->pos,
					*pChr->pTurnAroundPos,
					pChr->fRotation);

				break;
				//========== *** 追尾 ***
			case CHR_ENEMY_00_ACT_TRACKING:

				// 位置を更新
				pChr->pos.x += sinf(FindAngle(pChr->pos, *pChr->pTrackingPos)) * pChr->fMove;
				pChr->pos.y += cosf(FindAngle(pChr->pos, *pChr->pTrackingPos)) * pChr->fMove;

				break;
				//========== *** 凝視 ***
			case CHR_ENEMY_00_ACT_STARE:

				// 向きを更新
				pChr->rot.z = FindAngle(pChr->pos, *pChr->pStarePos);

				break;
				//========== *** 縮小 ***
			case CHR_ENEMY_00_ACT_SCALE_DOWN:

				// 幅と高さの倍率を更新
				pChr->scale.fWidth	-= CHR_ENEMY_00_SCALE_DOWN;
				pChr->scale.fHeight	-= CHR_ENEMY_00_SCALE_DOWN;

				if ((pChr->scale.fWidth <= 0.0f)
					|| (pChr->scale.fHeight <= 0.0f)) 
				{// 幅か高さが0以下になった時、
					// 使用していない状態にする
					pChr->bUse = false;
				}

				break;
			}
		}
	}
}

//========================================
// SoundLoading関数 - サウンド読み込み処理 -
//========================================
void SoundLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		|| 
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// 膨らみ状態/膨らみ待ち状態/消え中のいずれかの時、
		// 処理を終了する
		return;
	}

	for (int nCntSound = 0; nCntSound < g_aChr_enemy_00Type[pChr->nType].nSoundNum; nCntSound++)
	{
		if (pChr->nCounterSound == g_aChr_enemy_00Type[pChr->nType].aSound[nCntSound].nTime)
		{// サウンドカウンターがサウンドを鳴らす時間と一致した時、
			// サウンドを再生
			PlaySound(g_aChr_enemy_00Type[pChr->nType].aSound[nCntSound].soundLabel);
		}
	}

	if ((pChr->nCounterSound >= g_aChr_enemy_00Type[pChr->nType].nSoundLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nSoundLoopTime != -1))
	{// サウンドカウンターがサウンドのループ時間に達した時、かつサウンドのループ時間が-1でない時、
		// サウンドカウンターを初期化
		pChr->nCounterSound = -1;
	}

	// サウンドカウンターを加算
	pChr->nCounterSound++;
}

//========================================
// PatternLoading関数 - パターンNo.読み込み処理 -
//========================================
void PatternLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// 膨らみ状態/膨らみ待ち状態/消え中のいずれかの時、
		// 処理を終了する
		return;
	}

	for (int nCntPattern = 0; nCntPattern < g_aChr_enemy_00Type[pChr->nType].nPatternNum; nCntPattern++)
	{
		if (pChr->nCounterPattern == g_aChr_enemy_00Type[pChr->nType].aPattern[nCntPattern].nTime)
		{// パターンNo.カウンターがパターンNo.を変更する時間と一致した時、
			// パターンNo.を更新
			pChr->nPattern = g_aChr_enemy_00Type[pChr->nType].aPattern[nCntPattern].nPattern;
		}
	}

	if ((pChr->nCounterPattern >= g_aChr_enemy_00Type[pChr->nType].nPatternLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nPatternLoopTime != -1))
	{// パターンNo.カウンターがパターンNo.のループ時間に達した時、かつパターンNo.のループ時間が-1でない時、
		// パターンNo.カウンターを初期化
		pChr->nCounterPattern = -1;
	}

	// パターンNo.カウンターを加算
	pChr->nCounterPattern++;
}

//========================================
// EffectLoading関数 - エフェクト読み込み処理 -
//========================================
void EffectLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// 膨らみ状態/膨らみ待ち状態/消え中のいずれかの時、
		// 処理を終了する
		return;
	}

	for (int nCntEffect = 0; nCntEffect < g_aChr_enemy_00Type[pChr->nType].nEffectNum; nCntEffect++)
	{
		if (pChr->nCounterEffect == g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].nTime)
		{// エフェクトカウンターがエフェクトを配置する時間と一致した時、
			D3DXVECTOR3 setPos			//
						= pChr->pos;	// 設定位置

			// 設定位置を設定
			setPos.x += sinf(pChr->rot.z - D3DX_PI * 0.5f)
				* g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].pos.x;
			setPos.y += cosf(pChr->rot.z - D3DX_PI * 0.5f)
				* g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].pos.x;
			setPos.x += sinf(pChr->rot.z + D3DX_PI)
				* g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].pos.y;
			setPos.y += cosf(pChr->rot.z + D3DX_PI)
				* g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].pos.y;

			// エフェクト全般[00] の設定処理
			SetEff_general_00(
				setPos,
				D3DXVECTOR3(
					0.0f,
					0.0f,
					pChr->rot.z + g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].fAngle),
				g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].nEffectType,
				g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].bChild,
				&pChr->pos,
				&pChr->rot);
		}
	}

	if ((pChr->nCounterEffect >= g_aChr_enemy_00Type[pChr->nType].nEffectLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nEffectLoopTime != -1))
	{// エフェクトカウンターがエフェクトのループ時間に達した時、かつエフェクトのループ時間が-1でない時、
		// エフェクトカウンターを初期化
		pChr->nCounterEffect = -1;
	}

	// エフェクトカウンターを加算
	pChr->nCounterEffect++;
}

//========================================
// ShotLoading関数 - 発射読み込み処理 -
//========================================
void ShotLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// 膨らみ状態/膨らみ待ち状態/消え中のいずれかの時、
		// 処理を終了する
		return;
	}

	for (int nCntShot = 0; nCntShot < g_aChr_enemy_00Type[pChr->nType].nShotNum; nCntShot++)
	{
		if (pChr->nCounterShot == g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].nTime)
		{// 発射カウンターが発射する時間と一致した時、
			D3DXVECTOR3 setPos	// 設定位置
						= pChr->pos;

			// 設定位置を設定
			setPos.x	+= sinf(pChr->rot.z - D3DX_PI * 0.5f) 
						* g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].pos.x;
			setPos.y	+= cosf(pChr->rot.z - D3DX_PI * 0.5f)
						* g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].pos.x;
			setPos.x	+= sinf(pChr->rot.z + D3DX_PI)
						* g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].pos.y;
			setPos.y	+= cosf(pChr->rot.z + D3DX_PI)
						* g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].pos.y;

			// 弾[00] の設定処理
			SetAtk_bullet_00(
				setPos,
				D3DXVECTOR3(
					0.0f,
					0.0f,
					pChr->rot.z + g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].fAngle),
				g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].nBulletType,
				ATK_PARENT_TYPE_ENEMY);
		}
	}

	if ((pChr->nCounterShot >= g_aChr_enemy_00Type[pChr->nType].nShotLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nShotLoopTime != -1))
	{// 発射カウンターが発射のループ時間に達した時、かつ発射のループ時間が-1でない時、
		// 発射カウンターを初期化
		pChr->nCounterShot = -1;
	}

	// 発射カウンターを加算
	pChr->nCounterShot++;
}

//========================================
// SummonLoading関数 - 召喚読み込み処理 -
//========================================
void SummonLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// 膨らみ状態/膨らみ待ち状態/消え中のいずれかの時、
		// 処理を終了する
		return;
	}

	for (int nCntSummon = 0; nCntSummon < g_aChr_enemy_00Type[pChr->nType].nSummonNum; nCntSummon++)
	{
		if (pChr->nCounterSummon == g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nTime)
		{// 召喚カウンターが召喚する時間と一致した時、
			// 召喚処理
			Summon(pChr, nCntSummon);
		}
	}

	if ((pChr->nCounterSummon >= g_aChr_enemy_00Type[pChr->nType].nSummonLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nSummonLoopTime != -1))
	{// 召喚カウンターが召喚のループ時間に達した時、かつ召喚のループ時間が-1でない時、
		// 召喚カウンターを初期化
		pChr->nCounterSummon = -1;
	}

	// 召喚カウンターを加算
	pChr->nCounterSummon++;
}

//========================================
// SummonException関数 - 例外召喚処理 -
//========================================
void SummonException(Chr_enemy_00 *pChr) 
{
	for (int nCntSummon = 0; nCntSummon < g_aChr_enemy_00Type[pChr->nType].nSummonNum; nCntSummon++)
	{
		int n = 0;

		if (g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nTime == -1)
		{// 召喚する時間が-1の時、
			// 召喚処理
			Summon(pChr, nCntSummon);
		}
	}
}

//========================================
// Summon関数 - 召喚処理 -
//========================================
void Summon(Chr_enemy_00 *pChr, int nCntSummon) 
{
	D3DXVECTOR3 setPos	// 設定位置
				= pChr->pos;

	// 設定位置を設定
	setPos.x += sinf(pChr->rot.z - D3DX_PI * 0.5f)
		* g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].pos.x;
	setPos.y += cosf(pChr->rot.z - D3DX_PI * 0.5f)
		* g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].pos.x;
	setPos.x += sinf(pChr->rot.z + D3DX_PI)
		* g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].pos.y;
	setPos.y += cosf(pChr->rot.z + D3DX_PI)
		* g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].pos.y;

	// 敵[00] の設定処理
	SetChr_enemy_00(
		setPos,
		D3DXVECTOR3(
			0.0f,
			0.0f,
			pChr->rot.z + g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].fAngle),
		g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nEnemyType,
		false, 
		false,
		g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nParentNum, 
		g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nChildNum);
}

//========================================
// StateProcess関数 - 状態処理 -
//========================================
void StateProcess(Chr_enemy_00 *pChr)
{
	for (int nCntState = 0; nCntState < CHR_ENEMY_00_STATE_MAX; nCntState++)
	{
		if (pChr->aState[nCntState])
		{// その状態が真の時

			switch (nCntState)
			{
				//========== *** 出現中 ***
			case CHR_ENEMY_00_STATE_IN_POP:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP] < CHR_ENEMY_00_POP_TIME)
				{// 状態管理カウンター[出現中] が出現にかかる時間を下回っている時、
					// 状態管理カウンター[出現中] を加算
					pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP]++;
				}
				else
				{// 状態管理カウンター[出現中] が出現にかかる時間に達した時、
					// 状態[出現中]を偽にする
					pChr->aState[CHR_ENEMY_00_STATE_IN_POP] = false;
				}

				break;
				//========== *** 消え中 ***
			case CHR_ENEMY_00_STATE_IN_CLEAR:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR] < CHR_ENEMY_00_CLEAR_TIME)
				{// 状態管理カウンター[消え中] が消えるのにかかる時間を下回っている時、
					// 状態管理カウンター[消え中] を加算
					pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR]++;
				}
				else
				{// 状態管理カウンター[消え中] が消えるのにかかる時間に達した時、
					// 状態[消え中]を偽にする
					pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR] = false;

					// 使用していない状態にする
					pChr->bUse = false;

					// スコアを加算
					*GetScore() += g_aChr_enemy_00Type[pChr->nType].nScore;
				}

				break;
				//========== *** ダメージ ***
			case CHR_ENEMY_00_STATE_DAMAGE:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] < CHR_ENEMY_00_DAMAGE_TIME)
				{// 状態管理カウンター[ダメージ] にかかる時間を下回っている時、
					// 状態管理カウンター[ダメージ] を加算
					pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE]++;
				}
				else
				{// 状態管理カウンター[ダメージ] が消えるのにかかる時間に達した時、
					// 状態[ダメージ]を偽にする
					pChr->aState[CHR_ENEMY_00_STATE_DAMAGE] = false;
				}

				break;
				//========== *** 爆発耐性 ***
			case CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE] < CHR_ENEMY_00_EXPLOSION_RESISTANCE_TIME)
				{// 状態管理カウンター[爆発耐性] にかかる時間を下回っている時、
					// 状態管理カウンター[爆発耐性] を加算
					pChr->aCounterState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE]++;
				}
				else
				{// 状態管理カウンター[爆発耐性] が消えるのにかかる時間に達した時、
					// 状態[爆発耐性]を偽にする
					pChr->aState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE] = false;
				}

				break;
				//========== *** 膨らみ ***
			case CHR_ENEMY_00_STATE_SWELL:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] < CHR_ENEMY_00_SWELL_TIME)
				{// 状態管理カウンター[膨らみ] にかかる時間を下回っている時、
					// 状態管理カウンター[膨らみ] を加算
					pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL]++;
				}
				else
				{// 状態管理カウンター[膨らみ] が消えるのにかかる時間に達した時、
					// 状態[膨らみ]を偽にする
					pChr->aState[CHR_ENEMY_00_STATE_SWELL] = false;

					// 敵[00] の破壊処理
					DestructionChr_enemy_00(pChr);
				}

				break;
				//========== *** 膨らみ待ち ***
			case CHR_ENEMY_00_STATE_SWELL_WAIT:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL_WAIT] < pChr->nSwellWaitTime)
				{// 状態管理カウンター[膨らみ待ち] にかかる時間を下回っている時、
					// 状態管理カウンター[膨らみ待ち] を加算
					pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL_WAIT]++;
				}
				else
				{// 状態管理カウンター[膨らみ待ち] が消えるのにかかる時間に達した時、
					// 状態[膨らみ待ち]を偽にする
					pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT] = false;

					// 膨らみ状態にする
					pChr->aState[CHR_ENEMY_00_STATE_SWELL] = true;
					pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] = 0;
				}

				break;
			}
		}
	}
}

//========================================
// ChildMoveProcess関数- 子の移動処理 -
//========================================
void ChildMoveProcess(Chr_enemy_00 *pChr, D3DXVECTOR3 posTemp, D3DXVECTOR3 rotTemp)
{
	if (pChr->nParentNum == -1) 
	{// 親番号が-1の時、
		// 処理を終了する
		return;
	}

	D3DXVECTOR3		move					// 移動量
					= pChr->pos - posTemp;	//
	Chr_enemy_00	*pChildChr				// 敵[00] の情報のポインタ(子)
					= g_aChr_enemy_00;		//

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((pChildChr->bUse)
			&& (pChr->nParentNum == pChildChr->nChildNum))
		{// 使用されていて、かつ親番号と子番号が一致している時、
			// 位置を更新
			pChildChr->pos += move;
			
			D3DXVECTOR3 rot;										// 親から子への向き
			float		fDistance									// 
						= FindDistance(pChr->pos, pChildChr->pos);	// 親と子の距離

			// 親から子への向きを設定
			rot.z = FindAngle(pChr->pos, pChildChr->pos) + (pChr->rot.z - rotTemp.z);		

			// 子に向きの差分を適用
			pChildChr->rot.z += (pChr->rot.z - rotTemp.z);

			// 位置を親の所に戻した後、
			// 正しい角度に距離を加算する
			pChildChr->pos = pChr->pos;
			pChildChr->pos.x += sinf(rot.z) * fDistance;
			pChildChr->pos.y += cosf(rot.z) * fDistance;
		}
	}
}

//========================================
// ChildDestruction関数 - 子の破壊処理 -
//========================================
void ChildDestruction(Chr_enemy_00 *pChr) 
{
	if (pChr->nParentNum == -1)
	{// 親番号が-1の時、
		// 処理を終了する
		return;
	}

	Chr_enemy_00	*pChildChr			// 敵[00] の情報のポインタ(子)
					= g_aChr_enemy_00;	//

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((pChildChr->bUse)
			&& (pChr->nParentNum == pChildChr->nChildNum))
		{// 使用されていて、かつ親番号と子番号が一致している時、
			// 連鎖のカウントを加算
			pChildChr->nCntChain++;

			// 破壊処理
			DestructionChr_enemy_00(pChildChr);
		}
	}
}

//========================================
// ChildDamageState関数 - 子をダメージ状態にする -
//========================================
void ChildDamageState(Chr_enemy_00 *pChr)
{
	if (pChr->nParentNum == -1)
	{// 親番号が-1の時、
		// 処理を終了する
		return;
	}

	Chr_enemy_00	*pChildChr			// 
					= g_aChr_enemy_00;	// 敵[00] の情報のポインタ(子)

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((pChildChr->bUse)
			&& (pChr->nParentNum == pChildChr->nChildNum))
		{// 使用されていて、かつ親番号と子番号が一致している時、
			// 半ダメージ状態にする
			pChildChr->aState[CHR_ENEMY_00_STATE_DAMAGE] = true;
			pChildChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] = CHR_ENEMY_00_DAMAGE_TIME * 0.5f;
		}
	}
}

//========================================
// CollisionChr_fighter_00関数 - 機体[00]との当たり判定処理 -
//========================================
void CollisionChr_fighter_00(Chr_enemy_00 *pChr)
{
	if (!GetPlayer()->bDisp
		||
		GetPlayer()->bTurn
		||
		GetPlayer()->nDamageCounter > 0)
	{// 機体[00] が使用されていない状態か無敵状態、もしくは敵[00] が出現中か消え中の時、
		// 処理を終了する
		return;
	}

	if (CheckHit(
		pChr->pos,
		pChr->rot,
		pChr->fHitTestWidth * pChr->scale.fWidth,
		pChr->fHitTestHeight * pChr->scale.fHeight,
		HIT_TEST_TYPE_CIRCLE,
		GetPlayer()->pos,
		GetPlayer()->rot,
		PLAYER_HIT_TEST_RADIUS,
		0.0f,
		HIT_TEST_TYPE_CIRCLE))
	{// 敵[00] の座標が機体[00] の当たり判定内に入った時、

		// プレイヤーのダメージカウンターを初期化
		GetPlayer()->nDamageCounter = DAMAGE_TIME;

		PlaySound(SOUND_LABEL_SE_DAMAGE_000);

		if (--GetPlayer()->nLife <= 0) 
		{// 減算した体力が尽きた時、
			// 死んでいる
			GetPlayer()->bDisp = false;

			SetFade(MODE_RESULT_00);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadChr_enemy_00関数 - 敵[00] の読み込み処理 -
//========================================
void LoadChr_enemy_00(void)
{
	FILE *pFile;				// ファイルポインタ
	char aDataSearch[TXT_MAX];	// データ検索用

	Chr_enemy_00Type	*pChrType	// 敵[00] の種類毎の情報
						= g_aChr_enemy_00Type;

	// 種類毎の情報のデータファイルを開く
	pFile = fopen(CHR_ENEMY_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
		//処理を終了する
		return;
	}

	// 文末識別子が見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (strcmp(aDataSearch, "END") == 0)
		{// ファイルを閉じる
			fclose(pFile);
			break;
		}

		if (aDataSearch[0] == '#')
		{// コメント印と一致した時、
			// 繰り返し処理を折り返す
			continue;
		}

		if (strcmp(aDataSearch, "CHR_ENEMY_00_TYPE") == 0)
		{// 敵[00] の種類毎の情報の読み込みを開始

			// 行動数を初期化する
			pChrType->nActNum = 0;

			// 発射数を初期化する
			pChrType->nShotNum = 0;

			// 召喚数を初期化する
			pChrType->nSummonNum = 0;

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // 検索

				if (strcmp(aDataSearch, "CHR_ENEMY_00_TYPE_END") == 0)
				{// 敵[00] の種類毎の情報の読み込みを終了
					pChrType++;
					break;
				}//====================//
				else if (strcmp(aDataSearch, "読み込み名:") == 0)
				{// 読み込み名を読み込む
					fscanf(pFile, "%s", pChrType->aLoadName);
				}//====================//
				else if (strcmp(aDataSearch, "テクスチャの相対パス:") == 0)
				{// テクスチャの相対パスを読み込む
					fscanf(pFile, "%s", pChrType->aTexturPath);
				}//====================//
				else if (strcmp(aDataSearch, "パターンの上限X:") == 0)
				{// パターン上限Xを読み込む
					fscanf(pFile, "%d", &pChrType->nPtnMaxX);
				}//====================//
				else if (strcmp(aDataSearch, "アニメパターンの下限:") == 0)
				{// アニメパターンの下限を読み込む
					fscanf(pFile, "%d", &pChrType->nAnimPtnMin);
				}//====================//
				else if (strcmp(aDataSearch, "アニメパターンの上限:") == 0)
				{// アニメパターンの上限を読み込む
					fscanf(pFile, "%d", &pChrType->nAnimPtnMax);
				}//====================//
				else if (strcmp(aDataSearch, "パターンの上限Y:") == 0)
				{// パターン上限Yを読み込む
					fscanf(pFile, "%d", &pChrType->nPtnMaxY);
				}//====================//
				else if (strcmp(aDataSearch, "アニメーションの時間:") == 0)
				{// アニメーションにかかる時間を読み込む
					fscanf(pFile, "%d", &pChrType->nAnimTime);
				}//====================//
				else if (strcmp(aDataSearch, "アニメーションの種類:") == 0)
				{// アニメーションの種類を読み込む
					fscanf(pFile, "%s", aDataSearch); // 検索

					// アニメーションの種類を文字列から読み込む
					StringLoadAnimType(aDataSearch, &pChrType->animType);
				}//====================//
				else if (strcmp(aDataSearch, "幅:") == 0)
				{// 幅を読み込む
					fscanf(pFile, "%f", &pChrType->fWidth);

					// 幅をピクセル単位で修正

					pChrType->fWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "高さ:") == 0)
				{// 高さを読み込む
					fscanf(pFile, "%f", &pChrType->fHeight);

					// 高さをピクセル単位で修正
					pChrType->fHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "向きの種類:") == 0)
				{// 向きの種類を読み込む
					fscanf(pFile, "%s", aDataSearch); // 検索

					// 向きの種類を文字列から読み込む
					StringLoadAngleType(aDataSearch, &pChrType->angleType);
				}//====================//
				else if (strcmp(aDataSearch, "当たり判定の幅:") == 0)
				{// 当たり判定の幅を読み込む
					fscanf(pFile, "%f", &pChrType->fHitTestWidth);

					// 当たり判定をピクセル単位で修正
					pChrType->fHitTestWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "当たり判定の高さ:") == 0)
				{// 当たり判定の幅を読み込む
					fscanf(pFile, "%f", &pChrType->fHitTestHeight);

					// 当たり判定をピクセル単位で修正
					pChrType->fHitTestHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "体力:") == 0)
				{// 体力を読み込む
					fscanf(pFile, "%d", &pChrType->nLife);
				}//====================//
				else if (strcmp(aDataSearch, "移動力:") == 0)
				{// 移動力を読み込む
					fscanf(pFile, "%f", &pChrType->fMove);
				}//====================//
				else if (strcmp(aDataSearch, "回転力:") == 0)
				{// 回転力を読み込む
					fscanf(pFile, "%f", &pChrType->fRotation);
				}//====================//
				else if (strcmp(aDataSearch, "衝突ダメージ:") == 0)
				{// 衝突ダメージを読み込む
					fscanf(pFile, "%d", &pChrType->nHitDamage);
				}//====================//
				else if (strcmp(aDataSearch, "衝突時の破壊:") == 0)
				{// 衝突時の破壊処理を読み込む
					fscanf(pFile, "%d", &pChrType->bHitBreak);
				}//====================//
				else if (strcmp(aDataSearch, "爆発タイプ:") == 0)
				{// 爆発の種類を読み込む
					fscanf(pFile, "%d", &pChrType->explosionType);
				}//====================//
				else if (strcmp(aDataSearch, "ダメージサウンド:") == 0)
				{// ダメージサウンドの種類を読み込む
					fscanf(pFile, "%s", aDataSearch); // 検索

					// ダメージサウンドの種類を文字列から読み込む
					StringLoadSound(aDataSearch, &pChrType->damageSound);
				}//====================//
				else if (strcmp(aDataSearch, "ボーナスサウンド:") == 0)
				{// ボーナスサウンドフラグを読み込む
					fscanf(pFile, "%d", &pChrType->bBonusSound);
				}//====================//
				else if (strcmp(aDataSearch, "スコア:") == 0)
				{// 弾の発射にかかる時間を読み込む
					fscanf(pFile, "%d", &pChrType->nScore);
				}//====================//
				else if (strcmp(aDataSearch, "ACT") == 0)
				{// 行動の情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "ACT_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}
						else if (strcmp(aDataSearch, "行動:") == 0)
						{// 行動する時間を読み込む
							fscanf(pFile, "%d", &pChrType->aAct[pChrType->nActNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // 検索

							// 文字列から行動の読み込みを読み込む
							StringLoadActLoad(aDataSearch, &pChrType->aAct[pChrType->nActNum].type);

							// 行動数を加算
							pChrType->nActNum++;
						}
					}
				}//====================//
				else if (strcmp(aDataSearch, "SHOT") == 0)
				{// 発射の情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "SHOT_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}//====================//
						else if (strcmp(aDataSearch, "ループ時間:") == 0) 
						{// 発射のループ時間を読み込む
							fscanf(pFile, "%d", &pChrType->nShotLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "発射:") == 0)
						{// 発射する時間を読み込む
							fscanf(pFile, "%d", &pChrType->aShot[pChrType->nShotNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // 検索

							if (strcmp(aDataSearch, "[") == 0) 
							{
								while (1)
								{
									fscanf(pFile, "%s", aDataSearch); // 検索

									if (strcmp(aDataSearch, "]") == 0)
									{// 繰り返し処理を抜ける
										break;
									}//====================//
									else if (strcmp(aDataSearch, "種類:") == 0) 
									{// 弾の種類を読み込む
										fscanf(pFile, "%s", aDataSearch); // 検索

										// 弾[00] の種類を文字列から読み込む
										StringLoadAtk_bullet_00(aDataSearch, &pChrType->aShot[pChrType->nShotNum].nBulletType);
									}//====================//
									else if (strcmp(aDataSearch, "X:") == 0)
									{// 発射する位置Xを読み込む
										fscanf(pFile, "%f", &pChrType->aShot[pChrType->nShotNum].pos.x);
									}//====================//
									else if (strcmp(aDataSearch, "Y:") == 0)
									{// 発射する位置Yを読み込む
										fscanf(pFile, "%f", &pChrType->aShot[pChrType->nShotNum].pos.y);
									}//====================//
									else if (strcmp(aDataSearch, "角度:") == 0)
									{// 発射する角度を読み込む
										fscanf(pFile, "%f", &pChrType->aShot[pChrType->nShotNum].fAngle);

										// 角度を正しい値に変換
										pChrType->aShot[pChrType->nShotNum].fAngle *= D3DX_PI;
									}//====================//
								}
							}

							// 発射数を加算
							pChrType->nShotNum++;
						}
					}
				}//====================//
				else if (strcmp(aDataSearch, "SOUND") == 0)
				{// サウンドの情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "SOUND_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}//====================//
						else if (strcmp(aDataSearch, "ループ時間:") == 0)
						{// サウンドのループ時間を読み込む
							fscanf(pFile, "%d", &pChrType->nSoundLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "サウンド:") == 0)
						{// サウンドを鳴らす時間を読み込む
							fscanf(pFile, "%d", &pChrType->aSound[pChrType->nSoundNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // 検索

							// サウンドの種類を文字列から読み込む
							StringLoadSound(aDataSearch, &pChrType->aSound[pChrType->nSoundNum].soundLabel);

							// サウンド数を加算
							pChrType->nSoundNum++;
						}//====================//
					}
				}//====================//
				else if (strcmp(aDataSearch, "PATTERN") == 0)
				{// パターンNo.の情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "PATTERN_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}//====================//
						else if (strcmp(aDataSearch, "ループ時間:") == 0)
						{// パターンNo.のループ時間を読み込む
							fscanf(pFile, "%d", &pChrType->nPatternLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "パターン:") == 0)
						{// パターンNo.を変更する時間を読み込む
							fscanf(pFile, "%d", &pChrType->aPattern[pChrType->nPatternNum].nTime);

							// パターンNo.を読み込む
							fscanf(pFile, "%d", &pChrType->aPattern[pChrType->nPatternNum].nPattern);

							// パターンNo.数を加算
							pChrType->nPatternNum++;
						}//====================//
					}
				}//====================//
				else if (strcmp(aDataSearch, "EFFECT") == 0)
				{// エフェクトの情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "EFFECT_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}//====================//
						else if (strcmp(aDataSearch, "ループ時間:") == 0)
						{// エフェクトのループ時間を読み込む
							fscanf(pFile, "%d", &pChrType->nEffectLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "エフェクト:") == 0)
						{// エフェクトする時間を読み込む
							fscanf(pFile, "%d", &pChrType->aEffect[pChrType->nEffectNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // 検索

							if (strcmp(aDataSearch, "[") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", aDataSearch); // 検索

									if (strcmp(aDataSearch, "]") == 0)
									{// 繰り返し処理を抜ける
										break;
									}//====================//
									else if (strcmp(aDataSearch, "種類:") == 0)
									{// 弾の種類を読み込む
										fscanf(pFile, "%s", aDataSearch); // 検索

										// エフェクト全般[00] の種類を文字列から読み込む
										StringLoadEff_general_00Type(aDataSearch, &pChrType->aEffect[pChrType->nEffectNum].nEffectType);
									}//====================//
									else if (strcmp(aDataSearch, "X:") == 0)
									{// エフェクトを配置する位置Xを読み込む
										fscanf(pFile, "%f", &pChrType->aEffect[pChrType->nEffectNum].pos.x);
									}//====================//
									else if (strcmp(aDataSearch, "Y:") == 0)
									{// エフェクトを配置する位置Yを読み込む
										fscanf(pFile, "%f", &pChrType->aEffect[pChrType->nEffectNum].pos.y);
									}//====================//
									else if (strcmp(aDataSearch, "角度:") == 0)
									{// エフェクトを配置する角度を読み込む
										fscanf(pFile, "%f", &pChrType->aEffect[pChrType->nEffectNum].fAngle);

										// 角度を正しい値に変換
										pChrType->aEffect[pChrType->nEffectNum].fAngle *= D3DX_PI;
									}//====================//
									else if (strcmp(aDataSearch, "親子関係:") == 0)
									{// 子フラグを読み込む
										fscanf(pFile, "%d", &pChrType->aEffect[pChrType->nEffectNum].bChild);
									}//====================//
								}
							}

							// エフェクト数を加算
							pChrType->nEffectNum++;
						}
					}
				}//====================//
				else if (strcmp(aDataSearch, "SUMMON") == 0)
				{// 発射の情報の読み込みを開始する
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // 検索

						if (strcmp(aDataSearch, "SUMMON_END") == 0)
						{// 繰り返し処理を抜ける
							break;
						}//====================//
						else if (strcmp(aDataSearch, "ループ時間:") == 0)
						{// 召喚のループ時間を読み込む
							fscanf(pFile, "%d", &pChrType->nSummonLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "召喚:") == 0)
						{// 召喚する時間を読み込む
							fscanf(pFile, "%d", &pChrType->aSummon[pChrType->nSummonNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // 検索

							if (strcmp(aDataSearch, "[") == 0)
							{
								// 召喚する親番号と子番号を初期化
								pChrType->aSummon[pChrType->nSummonNum].nParentNum	= -1;
								pChrType->aSummon[pChrType->nSummonNum].nChildNum	= -1;

								while (1)
								{
									fscanf(pFile, "%s", aDataSearch); // 検索

									if (strcmp(aDataSearch, "]") == 0)
									{// 繰り返し処理を抜ける
										break;
									}//====================//
									else if (strcmp(aDataSearch, "種類:") == 0)
									{// 敵の種類を読み込む
										fscanf(pFile, "%s", aDataSearch); // 検索

										// 敵の種類を文字列から読み込む
										StringLoadChr_enemy_00Type(aDataSearch, &pChrType->aSummon[pChrType->nSummonNum].nEnemyType);
									}//====================//
									else if (strcmp(aDataSearch, "X:") == 0)
									{// 召喚する位置Xを読み込む
										fscanf(pFile, "%f", &pChrType->aSummon[pChrType->nSummonNum].pos.x);
									}//====================//
									else if (strcmp(aDataSearch, "Y:") == 0)
									{// 召喚する位置Yを読み込む
										fscanf(pFile, "%f", &pChrType->aSummon[pChrType->nSummonNum].pos.y);
									}//====================//
									else if (strcmp(aDataSearch, "角度:") == 0)
									{// 召喚する角度を読み込む
										fscanf(pFile, "%f", &pChrType->aSummon[pChrType->nSummonNum].fAngle);

										// 角度を正しい値に変換
										pChrType->aSummon[pChrType->nSummonNum].fAngle *= D3DX_PI;
									}//====================//
									else if (strcmp(aDataSearch, "親:") == 0)
									{// 親番号を読み込む
										fscanf(pFile, "%d", &pChrType->aSummon[pChrType->nSummonNum].nParentNum);
									}//====================//
									else if (strcmp(aDataSearch, "子:") == 0)
									{// 子供番号を読み込む
										fscanf(pFile, "%d", &pChrType->aSummon[pChrType->nSummonNum].nChildNum);
									}//====================//
								}
							}

							// 召喚数を加算
							pChrType->nSummonNum+=1;
						}
					}
				}//====================//
			}
		}
	}
}

//========================================
// StringLoadChr_enemy_00Type関数 - 敵[00] の種類を文字列から読み込む -
//========================================
void StringLoadChr_enemy_00Type(char aString[TXT_MAX], int *pType)
{
	Chr_enemy_00Type	*pChrType	// 敵[00] の種類毎の情報
						= GetChr_enemy_00Type();

	for (int nCntChrType = 0; nCntChrType < CHR_ENEMY_00_TYPE_MAX; nCntChrType++, pChrType++)
	{
		if (strcmp(aString, pChrType->aLoadName) == 0)
		{// 読み込み名が一致した時、
			// 敵の種類を代入
			*pType = nCntChrType;
			break;
		}
	}
}

//========================================
// InitChr_enemy_00関数 - 敵[00] の初期化処理 -
//========================================
void InitChr_enemy_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	for (int nCntChr_enemy_00Type = 0; nCntChr_enemy_00Type < CHR_ENEMY_00_TYPE_MAX; nCntChr_enemy_00Type++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aChr_enemy_00Type[nCntChr_enemy_00Type].aTexturPath, &g_aTextureChr_enemy_00[nCntChr_enemy_00Type]);
	}
	
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CHR_ENEMY_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChr_enemy_00,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00	*pChr	// 敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pVtx+=4, pChr++)
	{
		// パラメーターを初期化し、種類に応じて設定
		InitParameter(pChr);
		SetParameter(pChr);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pChr->pos,
			pChr->rot,
			false,
			pChr->fWidth,
			pChr->fHeight,
			g_aChr_enemy_00Type[pChr->nType].angleType);

		// rhwの設定
		SetRHW(pVtx);

		// 頂点カラーの設定
		SetVertexColor(pVtx, { 255,255,255,255 });

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			pChr->nPattern,
			g_aChr_enemy_00Type[pChr->nType].nPtnMaxX,
			g_aChr_enemy_00Type[pChr->nType].nPtnMaxY,
			NULL);
	}

	// 頂点座標をアンロックする
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// UninitChr_enemy_00関数 - 敵[00] の終了処理 -
//========================================
void UninitChr_enemy_00(void)
{
	// テクスチャの破棄
	for (int nCntChr_enemy_00 = 0; nCntChr_enemy_00 < CHR_ENEMY_00_TYPE_MAX; nCntChr_enemy_00++)
	{
		if (g_aTextureChr_enemy_00[nCntChr_enemy_00] != NULL)
		{
			g_aTextureChr_enemy_00[nCntChr_enemy_00]->Release();
			g_aTextureChr_enemy_00[nCntChr_enemy_00] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffChr_enemy_00 != NULL) 
	{
		g_pVtxBuffChr_enemy_00->Release();
		g_pVtxBuffChr_enemy_00 = NULL;
	}
}

//========================================
// UpdateChr_enemy_00関数 - 敵[00] の更新処理 -
//========================================
void UpdateChr_enemy_00(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00	*pChr	// 敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++, pVtx+=4)
	{
		if (!pChr->bUse)
		{// 使用されていない時、
			// 繰り返し処理を折り返す
			continue;
		}

		D3DXVECTOR3 posTemp			// 位置を保存
					= pChr->pos;	//
		D3DXVECTOR3 rotTemp			// 向きを保存
					= pChr->rot;	//

		// 行動読み込み処理
		ActLoading(pChr);

		// サウンド読み込み処理
		SoundLoading(pChr);

		// パターンNo.読み込み処理
		PatternLoading(pChr);

		// エフェクト読み込み処理
		EffectLoading(pChr);

		// 発射読み込み処理
		ShotLoading(pChr);

		// 召喚読み込み処理
		SummonLoading(pChr);

		// 位置を更新
		PositionUpdate(&pChr->pos, &pChr->move);

		// 子の移動処理
		ChildMoveProcess(pChr, posTemp, rotTemp);

		// 機体[00] との当たり判定処理
		CollisionChr_fighter_00(pChr);

		// 状態処理
		StateProcess(pChr);

		// テクスチャ座標の設定
		SetTexturePosition(pVtx,
			AnimationCount(
				&pChr->nPattern,
				g_aChr_enemy_00Type[pChr->nType].nAnimPtnMin,
				g_aChr_enemy_00Type[pChr->nType].nAnimPtnMax,
				&pChr->nCounterAnim,
				g_aChr_enemy_00Type[pChr->nType].nAnimTime,
				g_aChr_enemy_00Type[pChr->nType].animType),
			g_aChr_enemy_00Type[pChr->nType].nPtnMaxX,
			g_aChr_enemy_00Type[pChr->nType].nPtnMaxY,
			NULL);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pChr->pos,
			pChr->rot,
			false,
			pChr->fWidth * (1.0f + ((CHR_ENEMY_00_SWELL_DIAMETER - 1.0f)
				* ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] / (float)CHR_ENEMY_00_SWELL_TIME)))
			* pChr->scale.fWidth,
			pChr->fHeight * (1.0f + ((CHR_ENEMY_00_SWELL_DIAMETER - 1.0f)
				* ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] / (float)CHR_ENEMY_00_SWELL_TIME)))
			* pChr->scale.fHeight,
			g_aChr_enemy_00Type[pChr->nType].angleType);

		// 色の設定
		SetColor(pVtx, pChr);
	}

	// 頂点座標をアンロックする
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// DrawChr_enemy_00関数 - 敵[00] の描画処理 -
//========================================
void DrawChr_enemy_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffChr_enemy_00, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	Chr_enemy_00	*pChr	// 敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{// 使用されている状態の時、
			// テクスチャの設定
			pDevice->SetTexture(0, g_aTextureChr_enemy_00[pChr->nType]);

			// 敵[00] の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntChr * 4, 2);
		}
	}
}

//========================================
// SetChr_enemy_00関数 - 敵[00] の設定処理 -
//========================================
void SetChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bAreaBoss, bool bBomb, int nParentNum, int nChildNum)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00	*pChr	// 敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pVtx += 4, pChr++)
	{
		if (pChr->bUse)
		{// 使用されている状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		// パラメータを初期化する
		InitParameter(pChr);

		pChr->pos			= pos;			// 位置を代入
		pChr->rot			= rot;			// 角度を代入
		pChr->nType			= nType;		// 種類を代入
		pChr->bBomb			= bBomb;		// 爆弾所持フラグを代入
		pChr->nParentNum	= nParentNum;	// 親番号
		pChr->nChildNum		= nChildNum;	// 子供番号

		// パラメータを種類に応じて設定
		SetParameter(pChr);

		// 頂点座標の設定
		SetVertexPosition(pVtx,
			pChr->pos,
			pChr->rot,
			true,
			pChr->fWidth * (1.0f + ((CHR_ENEMY_00_SWELL_DIAMETER - 1.0f)
				* ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] / (float)CHR_ENEMY_00_SWELL_TIME))),
			pChr->fHeight * (1.0f + ((CHR_ENEMY_00_SWELL_DIAMETER - 1.0f)
				* ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] / (float)CHR_ENEMY_00_SWELL_TIME))),
			g_aChr_enemy_00Type[pChr->nType].angleType);

		// 色の設定
		SetColor(pVtx, pChr);

		// 出現中の状態にする
		pChr->aState[CHR_ENEMY_00_STATE_IN_POP] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP] = 0;

		// 使用している状態にする
		pChr->bUse = true;

		break;
	}

	// 頂点座標をアンロックする
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// HitChr_enemy_00関数 - 敵[00] のヒット処理 -
//========================================
void HitChr_enemy_00(Chr_enemy_00 *pChr, int nDamage)
{
	if ((pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// 無敵状態の時、
		if (pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE])
		{
			// 金属SE
			PlaySound(SOUND_LABEL_SE_METAL_000);
		}

		// 処理を終了する
		return;
	}

	// 体力からダメージを減算
	pChr->nLife -= nDamage;

	SetCombo(pChr->pos, D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR((float)245 / (float)255, (float)191 / (float)255, (float)63 / (float)255, 1.0f), 60);

	PlaySound(SOUND_LABEL_SE_DAMAGE_000);

	if (pChr->nLife <= 0)
	{// 体力が0以下の時、
		// 体力を0にする
		pChr->nLife = 0;
		
		// 消え中状態にする
		pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR] = 0;
	}
	else if (nDamage > 0)
	{// 体力が0を上回っている時、
		if (pChr->bReceivedExplosion)
		{// 爆発を受けたフラグが真の時、
			// 爆発耐性状態にする
			pChr->aState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE] = true;
			pChr->aCounterState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE] = 0;
		}

		// 爆発を受けたフラグを偽にする
		pChr->bReceivedExplosion = false;

		// ダメージ状態にする
		pChr->aState[CHR_ENEMY_00_STATE_DAMAGE] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] = 0;

		// 子をダメージ状態にする
		ChildDamageState(pChr);

		// ダメージサウンドを鳴らす
		PlaySound(g_aChr_enemy_00Type[pChr->nType].damageSound);
	}
}

//========================================
// HitChr_enemy_00ForAtk_explosion_00関数 - 爆発[00] 用の敵[00] のヒット処理 -
//========================================
bool HitChr_enemy_00ForAtk_explosion_00(Chr_enemy_00 *pChr, int nDamage, int nCntChain)
{
	if ((pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE])
		|| (nDamage <= 0))
	{// 無敵状態の時、もしくはダメージが0以下の時、
		// 処理を終了する
		return false;
	}

	// 体力からダメージを減算
	pChr->nLife -= nDamage;

	if (pChr->nLife <= 0)
	{// 体力が0以下の時、
		// 体力を0にする
		pChr->nLife = 0;

		// 膨らみ状態にする
		pChr->aState[CHR_ENEMY_00_STATE_SWELL] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] = 0;

		// 連鎖のカウントを代入
		pChr->nCntChain = nCntChain;

		return true;
	}
	else if (nDamage > 0)
	{// 体力が0を上回っている時、
		// ダメージ状態にする
		pChr->aState[CHR_ENEMY_00_STATE_DAMAGE] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] = 0;

		// 子をダメージ状態にする
		ChildDamageState(pChr);

		// ダメージサウンドを鳴らす
		PlaySound(g_aChr_enemy_00Type[pChr->nType].damageSound);
	}

	return false;
}

//========================================
// DestructionChr_enemy_00関数 - 敵[00] の破壊処理 -
//========================================
void DestructionChr_enemy_00(Chr_enemy_00 *pChr)
{
	// 子の破壊処理
	ChildDestruction(pChr);

	if (g_aChr_enemy_00Type[pChr->nType].nScore > 0)
	{// スコアが0を上回っている時、
		int nScore	// スコア表示
			= g_aChr_enemy_00Type[pChr->nType].nScore
			* (1.0f + (CHR_ENEMY_00_SCORE_DIAMETER_ADDEND_CHAIN * pChr->nCntChain));

		// スコアを加算
		//GetChr_fighter_00()->nScore += nScore;
		// ★

		// スコア[00] の設定処理
		//SetUi_score_00(pChr->pos, nScore);
		// ★
	}

	if (pChr->aAct[CHR_ENEMY_00_ACT_SUMMONING_ON_DIED])
	{// 死亡時召喚フラグが真の時、
		// 例外召喚処理
		SummonException(pChr);
	}

	// 敵[00]を使用していない状態にする
	pChr->bUse = false;

	// 爆発[00] の設定処理
	SetAtk_explosion_00(pChr->pos, pChr->rot, g_aChr_enemy_00Type[pChr->nType].explosionType, pChr->nCntChain + 1);
	
	if (g_aChr_enemy_00Type[pChr->nType].bBonusSound)
	{// ボーナスサウンドフラグが真の時、
		
	}
}

//========================================
// RemoveAllChr_enemy_00関数 - 敵[00] を全て消し去る -
//========================================
void RemoveAllChr_enemy_00(void) 
{
	Chr_enemy_00	*pChr	// 敵[00] の情報のポインタ
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse)
		{// 使用されていない状態の時、
			// 繰り返し処理を折り返す
			continue;
		}

		// 出現中でなくする
		pChr->aState[CHR_ENEMY_00_STATE_IN_POP] = false;

		if (!pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
		{// 消え中でない時、
			// 消え中にする
			pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]			= true;
			pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR]	= 0;
		}

		if (pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		{// 膨らみ状態の時、
			pChr->aState[CHR_ENEMY_00_STATE_SWELL] = false;
			// 破壊処理
			DestructionChr_enemy_00(pChr);
		}
	}
}
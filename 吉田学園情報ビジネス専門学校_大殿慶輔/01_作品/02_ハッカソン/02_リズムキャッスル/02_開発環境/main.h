//========================================
// 
// メインのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.h ***
//========================================
#ifndef _MAIN_H_	// このマクロ定義がされていなかった時
#define _MAIN_H_	// 二重インクルード防止のマクロを定義する

#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"						// 入力処理に必要
#include "Xinput.h"						// 入力処理に必要
#include "xaudio2.h"					// サウンド処理に必要

//****************************************
// ライブラリのリンク
//****************************************
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネントを使用するのに必要
#pragma comment(lib,"dsound.lib")	// DirectSoundを使用するのに必要
#pragma comment(lib,"winmm.lib")	// システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	// 入力処理に必要
#pragma comment(lib,"xinput.lib")	// 入力処理に必要
#pragma warning(disable:4091)		// 警告:C4091を無視する
#pragma warning(disable:4244)		// 警告:C4244を無視する

//****************************************
// マクロ定義
//****************************************
// ウインドウクラス
// ウインドウの幅
// ウインドウの高さ
// 頂点フォーマット(2D)
// 頂点フォーマット(3D)
// 文字列の最大数(汎用)
// デバッグコメント使用フラグ
#define CLASS_NAME		"WindowClass"
#define SCREEN_WIDTH	(1280)	// NORMAL=1280 4:3=960
#define SCREEN_HEIGHT	(720)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define TXT_MAX			(256)
#define _DEBUG_COMMENT	(false)

// 1ピクセルの大きさ
// 1ピクセルの大きさ(3D)
#define PIXEL	(3.0f)
#define PIXEL3D	(0.4f)

// 内側スクリーンの左端
// 内側スクリーンの右端
#define INSIDE_SCREEN_LEFTMOST	(160)
#define INSIDE_SCREEN_RIGHTMOST	(1120)

// D3DXVECTOR3の初期化値
// Colorの初期化値
#define INITD3DXVECTOR3	D3DXVECTOR3(0.0f,0.0f,0.0f)
#define INITCOLOR		Color{255,255,255,255}

//****************************************
// 列挙型の定義
//****************************************
// 画面(モード)の種類
typedef enum
{
	MODE_NONE,			// 無し
	MODE_GAME_00,		// ゲーム画面			[00]
	MODE_TITLE_00,		// タイトル画面			[00]
	MODE_RESULT_00,		// リザルト画面			[00]
	MODE_TUTORIAL_00,	// チュートリアル画面	[00]
	MODE_MAX
}MODE;

//****************************************
// 構造体の定義
//****************************************
// 頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float		rhw;	// 座標変換用係数(1.0fで固定)
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_2D;

// 頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_3D;

// 拡大倍率構造体
typedef struct 
{
	float fWidth;	// 幅
	float fHeight;	// 高さ
	float fDepth;	// 奥行き
}Scale;

// 色構造体
typedef struct 
{
	int r;
	int g;
	int b;
	int a;
}Color;

// メイン処理の情報構造体
typedef struct 
{
	LPDIRECT3D9			pD3D		// Direct3Dオブジェクトへのポインタ
						= NULL;
	LPDIRECT3DDEVICE9	pD3DDevice	// Direct3Dデバイスへのポインタ
						= NULL;
	HWND				hWnd;		// ウインドウハンドル(識別子)
	MODE				mode		// 現在のモード
						= MODE_NONE;
	D3DXVECTOR3			windowPos;	// ウインドウの位置
	bool				bPause		// ポーズフラグ
						= false;
	Color				bgCol;		// 背景色
#if _DEBUG_COMMENT
	LPD3DXFONT			pFont		// フォントへのポインタ
						= NULL;
	int					nCountFPS	// FPSのカウンター
						= 0;
	bool				bDebug		// デバッグモードのON/OFF
						= true;
	int					nCommentY;	// コメントのY座標
	Color				commentCol;	// コメント色
#endif
}Main;

//****************************************
// プロトタイプ宣言
//****************************************
// メイン処理の情報の取得
// デバイスを取得
// ウインドウハンドルを取得
// ウインドウの位置を取得
// モードの取得
Main *GetMain(void);
LPDIRECT3DDEVICE9 GetDevice(void);
HWND *GetWindowHandle(void);
D3DXVECTOR3 GetWindowPos(void);
MODE GetMode(void);

// モードの設定
void SetMode(MODE mode);

// ウインドウのモードを切り替える
void WindowModeChange(bool bFullScreen);

#if _DEBUG_COMMENT
// コメントの描画処理
// コメントの色変更処理
void DrawComment(char *pString);
void CommentColorChange(Color col);
#endif

// メッセージ処理
void Message(char *pText, char *pCaption);

#endif
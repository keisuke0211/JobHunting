//========================================
// 
// メイン処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "color.h"
#include "fade.h"
#include "input.h"
#include "light.h"
#include "text.h"
#include "sound.h"
#include "attack.h"			// ATTACK
#include "back-ground.h"	// BACKGROUND
#include "character.h"		// CHARACTER
#include "effect.h"			// EFFECT
#include "item.h"			// ITEM
#include "mode.h"			// MODE
#include "system.h"			// SYSTEM
#include "object.h"			// OBJECT
#include "user-interface.h"	// USERINTERFACE
#include "md_game_00.h"		// MD :ゲーム			[00]
#include "md_result_00.h"	// MD :リザルト			[00]
#include "md_title_00.h"	// MD :タイトル			[00]
#include "md_tutorial_00.h"	// MD :チュートリアル	[00]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// マクロ定義
//****************************************
// ウインドウの名前
#define WINDOW_NAME	""

// 初期のモード
#define INIT_MODE	(MODE_TITLE_00)

// ポーズキー
// デバッグキー
#define PAUSE_KEY	(DIK_P)
#define DEBUG_KEY	(DIK_F1)

//****************************************
// プロトタイプ宣言
//****************************************
// ウインドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 初期化処理
// 終了処理
// 更新処理
// 描画処理
// 読み込み処理
// 書き込み処理
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void Load(void);
void Save(void);

// モードに応じた初期化処理
// モードに応じた終了処理
// モードに応じた更新処理
// モードに応じた描画処理
void ModeInit(MODE mode);
void ModeUninit(MODE mode);
void ModeUpdate(MODE mode);
void ModeDraw(MODE mode);

// デバッグコメントの描画処理
void DrawDebugComment(void);

// ウインドウの表示/非表示を切り替える
void ShowTaskBar(bool bShow);

//****************************************
// グローバル宣言
//****************************************
Main	g_main;	// メイン処理の情報構造体

//========== *** メイン処理の情報の取得 ***
Main *GetMain(void) 
{
	return &g_main;
}

//========== *** デバイスの取得 ***
LPDIRECT3DDEVICE9 GetDevice(void) 
{
	return g_main.pD3DDevice;
}

//========== *** ウインドウハンドルを取得 ***
HWND *GetWindowHandle(void) 
{
	return &g_main.hWnd;
}

//========== *** ウインドウの位置を取得 ***
D3DXVECTOR3 GetWindowPos(void)
{
	return g_main.windowPos;
}

//========== *** モードの取得 ***
MODE GetMode(void)
{
	return g_main.mode;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cppファイルの占有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// WinMain関数
// Author:RIKU NISHIMURA
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	MSG msg;				// メッセージを格納する変数
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
#if _DEBUG_COMMENT
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻
#endif

	srand((unsigned int)time(0)); // 乱数の種を取得

	// ウインドウクラスの構造体
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						// ウインドウのスタイル
		WindowProc,						// ウインドウプロシージャ
		0,								// 0にする
		0,								// 0にする
		hInstance,						// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		NULL,							// メニューバー
		CLASS_NAME,						// ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// ファイルのアイコン
	};

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// ウインドウを生成
	g_main.hWnd = CreateWindowEx(
		0,								// 拡張ウインドウスタイル
		CLASS_NAME,						// ウインドウクラスの名前
		WINDOW_NAME,					// ウインドウの名前
		WS_SYSMENU | WS_MINIMIZEBOX,	// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上X座標
		CW_USEDEFAULT,					// ウインドウの左上Y座標
		SCREEN_WIDTH,					// ウインドウの幅
		SCREEN_HEIGHT,					// ウインドウの高さ
		NULL,							// 親ウインドウのハンドル
		NULL,							// メニューハンドルまたは
		hInstance,						// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	// 初期化処理
	if (FAILED(Init(hInstance, g_main.hWnd, TRUE)))
	{// 初期化処理が失敗した場合

		return -1;
	}

	{
		bool bFull = true;	// フルスクフラグ

#if _DEBUG
		bFull = false;
#endif
		// ウインドウの表示
		WindowModeChange(bFull);
	}

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0; // 現在時刻を初期化する
	dwExecLastTime = timeGetTime(); // 現在時刻を取得(保存)

#if _DEBUG_COMMENT
	dwFrameCount = 0;	// フレームカウントを初期化する
	dwFPSLastTime = timeGetTime();	// 現在時刻を取得(保存)
#endif

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらメッセージループを抜ける

				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime(); // 現在時刻を取得

#if _DEBUG_COMMENT
			if ((dwCurrentTime - dwFPSLastTime) >= 500) 
			{// 0.5秒経過
				// FPSを計測
				g_main.nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime; // FPSを測定した時刻を保存
				dwFrameCount = 0; // フレームカウントをクリア
			}
#endif

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) 
			{// 60/1秒経過
				dwExecLastTime = dwCurrentTime; // 処理開始の時刻[現在時刻]を保存

				{
					RECT rec;	// 大きさを受け取るRECT構造体

					// ウインドウの情報を取得
					GetWindowRect(g_main.hWnd, &rec);

					// ウインドウの位置を設定
					g_main.windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);
				}

				// 更新処理
				Update();

				// 描画処理
				Draw();

#if _DEBUG_COMMENT
				dwFrameCount++; // フレームカウントを加算
#endif
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================================
// ShowTaskBar関数 - タスクバーの表示/非表示を切り替える -
// Author:RIKU NISHIMURA
// 参考: https://dixq.net/forum/viewtopic.php?t=12505
//========================================
void ShowTaskBar(bool bShow)
{
	HWND hTask;	// タスクバーのハンドル

	// タスクバーのハンドルを取得
	hTask = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (bShow)
	{// 表示
		// タスクバーを表示
		ShowWindow(hTask, SW_SHOW);

		// ウインドウをアクティブにする
		SetFocus(g_main.hWnd);
	}
	else 
	{// 非表示
		// タスクバーを非表示
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================================
// WindowProc関数
// Author:RIKU NISHIMURA
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	// 返り値を格納

	switch (uMsg)
	{
	case WM_DESTROY: // ウインドウ破棄のメッセージ

		PostQuitMessage(0); // WM_QUITメッセージを送る

		break;
	case WM_KEYDOWN: // キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]キーが押された

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO); // ウインドウを破棄する(WM_DESTROYメッセージを送る)

			if (nID == IDYES)
			{
				DestroyWindow(hWnd); // ウインドウを破棄する
			}

			break;
		}

		break;
	case WM_CLOSE: // 終了ボタンが押された時

		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO); // ウインドウを破棄する(WM_DESTROYメッセージを送る)

		if (nID == IDYES)
		{
			DestroyWindow(hWnd); // ウインドウを破棄する
		}
		else
		{
			return 0; // (※0を返さないと終了してしまう)
		}

		break;
	case WM_LBUTTONDOWN: // マウスクリックのメッセージ

		// 対象ウインドウにフォーカスを合わせる
		SetFocus(hWnd); // マウスを左クリックしたウインドウをアクティブにする
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を返す
}

//========================================
// Init関数 - 初期化処理 -
// Author:RIKU NISHIMURA
//========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// 背景色を初期化
	g_main.bgCol = INITCOLOR;

	// Direct3Dオブジェクトの生成
	g_main.pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_main.pD3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;	// ディスプレイモード

	// 現在のディスプレイモードを取得
	if (FAILED(g_main.pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// デバイスのプレゼンテーションパラメータを設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // パラメータのゼロクリア

	d3dpp.BackBufferWidth				= SCREEN_WIDTH;					// ゲームの画面サイズ(幅)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;				// ゲームの画面サイズ(高さ)
	d3dpp.BackBufferFormat				= d3ddm.Format;					// バックバッファの形式
	d3dpp.BackBufferCount				= 1;							// バックバッファの数
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil		= TRUE;							// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed						= bWindow;						// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_main.pD3DDevice)))
	{
		if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_main.pD3DDevice)))
		{
			if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_main.pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

#if _DEBUG_COMMENT
	// フォントの生成
	D3DXCreateFont(g_main.pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_main.pFont);
#endif

	// レンダーステートの設定
	g_main.pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_main.pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_main.pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_main.pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステート設定
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// アルファブレンドの設定
	g_main.pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_main.pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_main.pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 読み込み処理
	Load();

	// 初期化処理
	InitCamera();			// カメラ
	InitSound(hWnd);		// サウンド
	InitLight();			// 光
	InitText();				// テキスト
	InitFade(INIT_MODE);	// フェード

	// 入力関連の初期化処理
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// Uninit関数 - 終了処理 -
// Author:RIKU NISHIMURA
//========================================
void Uninit(void)
{
	// 書き込み処理
	Save();

	// 終了処理
	UninitInput();			// 入力関連
	UninitLight();			// 光
	UninitText();			// テキスト
	UninitSound();			// サウンド
	UninitFade();			// フェード

#if _DEBUG_COMMENT
	// フォントの破棄
	if (g_main.pFont != NULL)
	{
		g_main.pFont->Release();
		g_main.pFont = NULL;
	}
#endif

	// Direct3Dデバイスの破棄
	if (g_main.pD3DDevice != NULL)
	{
		g_main.pD3DDevice->Release();
		g_main.pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_main.pD3D != NULL)
	{
		g_main.pD3D->Release();
		g_main.pD3D = NULL;
	}

	// タスクバーを表示
	ShowTaskBar(true);
}

//========================================
// Update関数 - 更新処理 - 
// Author:RIKU NISHIMURA
//========================================
void Update(void)
{
	// 更新処理
	UpdateInput(g_main.bPause);	// 入力関連
	ModeUpdate(g_main.mode);	// モード
	UpdateCamera();				// カメラ
	UpdateLight();				// 光
	UpdateFade();				// フェード

#if _DEBUG_COMMENT
	if (GetKeyboardTrigger(DEBUG_KEY))
	{// デバッグキーを押した時、
		// デバッグ表示のON/OFFを切り替える
		g_main.bDebug ^= 1;
	}
	if (GetKeyboardTrigger(PAUSE_KEY))
	{// ポーズキーを押した時、
		// ポーズフラグのON/OFFを切り替える
		g_main.bPause ^= 1;
	}
#endif
}

//========================================
// Draw関数 - 描画処理 -
// Author:RIKU NISHIMURA
//========================================
void Draw(void)
{
	// 画面クリア
	g_main.pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ColorToD3DCOLOR(g_main.bgCol), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_main.pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合、
		SetCamera3D();			// カメラ(3D)の設定処理
		ModeDraw(g_main.mode);	// モード
		DrawFade();				// フェード

#if _DEBUG_COMMENT
		if (g_main.bDebug)
		{// デバッグ表示がONの時、
			// デバッグコメントの描画処理
			DrawDebugComment();
		}
#endif

		// 描画終了
		g_main.pD3DDevice->EndScene();
	}

	// バックバッファをフロントバッファの入れ替え
	g_main.pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// Load関数 - 読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void Load(void)
{
	LoadSound();			// サウンド
	LoadFont();				// フォント
	LoadAttack();			// ATTACK
	LoadBackGround();		// BACKGROUND
	LoadCharacter();		// CHARACTER
	LoadEffect();			// EFFECT
	LoadSystem();			// SYSTEM
	LoadItem();				// ITEM
	LoadMode();				// MODE
	LoadObject();			// OBJECT
	LoadUserInterface();	// USERINTERFACE
}

//========================================
// Save関数 - 書き込み処理 -
// Author:RIKU NISHIMURA
//========================================
void Save(void)
{
	SaveSystem();	// SYSTEM
}

//========================================
// ModeInit関数 - モードに応じた初期化処理 -
// Author:RIKU NISHIMURA
//========================================
void ModeInit(MODE mode) 
{
	switch (mode)
	{
	case MODE_GAME_00		:InitMd_game_00()		; break;	// MD:ゲーム画面			[00]
	case MODE_TITLE_00		:InitMd_title_00()		; break;	// MD:タイトル画面			[00]
	case MODE_RESULT_00		:InitMd_result_00()		; break;	// MD:リザルト画面			[00]
	case MODE_TUTORIAL_00	:InitMd_tutorial_00()	; break;	// MD:チュートリアル画面	[00]
	}
}

//========================================
// ModeUninit関数 - モードに応じた終了処理 -
// Author:RIKU NISHIMURA
//========================================
void ModeUninit(MODE mode)
{
	switch (mode)
	{
	case MODE_GAME_00		:UninitMd_game_00()		; break;	// MD:ゲーム画面			[00]
	case MODE_TITLE_00		:UninitMd_title_00()	; break;	// MD:タイトル画面			[00]
	case MODE_RESULT_00		:UninitMd_result_00()	; break;	// MD:リザルト画面			[00]
	case MODE_TUTORIAL_00	:UninitMd_tutorial_00()	; break;	// MD:チュートリアル画面	[00]
	}
}

//========================================
// ModeUpdate関数 - モードに応じた更新処理 -
// Author:RIKU NISHIMURA
//========================================
void ModeUpdate(MODE mode)
{
	switch (mode)
	{
	case MODE_GAME_00		:UpdateMd_game_00()		; break;	// MD:ゲーム画面			[00]
	case MODE_TITLE_00		:UpdateMd_title_00()	; break;	// MD:タイトル画面			[00]
	case MODE_RESULT_00		:UpdateMd_result_00()	; break;	// MD:リザルト画面			[00]
	case MODE_TUTORIAL_00	:UpdateMd_tutorial_00()	; break;	// MD:チュートリアル画面	[00]
	}
}

//========================================
// ModeDraw関数 - モードに応じた描画処理 -
// Author:RIKU NISHIMURA
//========================================
void ModeDraw(MODE mode)
{
	switch (mode)
	{
	case MODE_GAME_00		:DrawMd_game_00()		; break;	// MD:ゲーム画面			[00]
	case MODE_TITLE_00		:DrawMd_title_00()		; break;	// MD:タイトル画面			[00]
	case MODE_RESULT_00		:DrawMd_result_00()		; break;	// MD:リザルト画面			[00]
	case MODE_TUTORIAL_00	:DrawMd_tutorial_00()	; break;	// MD:チュートリアル画面	[00]
	}
}

#if _DEBUG_COMMENT
//========================================
// DrawDebugComment関数 - デバッグコメントの描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawDebugComment(void)
{
	char	aString	// 文字列
			[TXT_MAX];

	// コメントのY座標を初期化
	g_main.nCommentY = 0;

	CommentColorChange({ 0,235,219 });	// コメント色設定
	sprintf(aString, "FPS：%d", g_main.nCountFPS); DrawComment(aString);
	sprintf(aString, " デバッグ表示のON/OFF　:[ F1 ]"); DrawComment(aString);
	sprintf(aString, " "); DrawComment(aString);
	sprintf(aString, " 視点操作　　　　　　　:[ 方向キー ] or [ 右ボタン + カーソル移動 ] or [ 左スティック ]"); DrawComment(aString);
	sprintf(aString, " プレイヤー操作　　　　:[ WASDキー ] or [ 右スティック ]"); DrawComment(aString);
	sprintf(aString, " アイテム選択　　　　　:[ Q / Eキー ]"); DrawComment(aString);
	sprintf(aString, " アイテム使用　　　　　:[ SPACEキー ]"); DrawComment(aString);

	// コメント描画処理
	DrawCommentChr_player_00();	// CHR:プレイヤー	[00]
}
#endif

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetMode関数 - モードの設定処理 -
// Author:RIKU NISHIMURA
//========================================
void SetMode(MODE mode) 
{
	// 現在のモードの終了処理
	ModeUninit(g_main.mode);

	// 現在の画面(モード)を切り替える
	g_main.mode = mode;

	// 新しいモードの初期化処理
	ModeInit(mode);
}

//========================================
// WindowModeChange関数 - ウインドウのモードを切り替える -
// Author:RIKU NISHIMURA
//========================================
void WindowModeChange(bool bFullScreen)
{
	HDC hDC			// デスクトップのハンドル
		= GetDC(GetDesktopWindow());
	int nScrSizeX	// デスクトップの画面サイズX
		= GetDeviceCaps(hDC, HORZRES);
	int nScrSizeY	// デスクトップの画面サイズY
		= GetDeviceCaps(hDC, VERTRES);

	// デスクトップのハンドルを手放す
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bFullScreen)
	{// 全画面フラグが真の時、
		// ウインドウの位置を設定
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// 全画面フラグが偽の時、
		// ウインドウの位置を設定
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(g_main.hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(g_main.hWnd);

	// タスクバーを表示/非表示
	ShowTaskBar(bFullScreen ^ 1);
}

#if _DEBUG_COMMENT
//========================================
// DrawComment関数 - コメントの描画処理 -
// Author:RIKU NISHIMURA
//========================================
void DrawComment(char *pString)
{
	RECT	rect;	// 書式設定する四角形

	// テキストの描画
	rect = { 0, g_main.nCommentY , SCREEN_WIDTH, SCREEN_HEIGHT };
	g_main.nCommentY += g_main.pFont->DrawText(NULL, pString, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(g_main.commentCol.r, g_main.commentCol.g, g_main.commentCol.b, 255));
}
#endif

#if _DEBUG_COMMENT
//========================================
// DrawComment関数 - コメントの色変更処理 -
// Author:RIKU NISHIMURA
//========================================
void CommentColorChange(Color col) 
{
	// コメント色を代入
	g_main.commentCol = col;
}
#endif

//========================================
// Message関数 - メッセージ処理 -
// Author:RIKU NISHIMURA
//========================================
void Message(char *pText, char *pCaption)
{
	static int	bMessage	// メッセージ表示完了フラグ(無限ループ防止)
				= false;

	if (bMessage) 
	{// メッセージ表示完了フラグ
		bMessage = false;	// メッセージ表示完了フラグを偽にする
		return;				// 処理を終了する
	}

	if (MessageBox(*GetWindowHandle(), pText, pCaption, MB_OK) == IDOK)
	{// メッセージボックスの返り値がOKの時、
		// メッセージ表示完了フラグを真にする
		bMessage = true;
	}
}
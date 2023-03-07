//========================================
// 
// メイン処理
// Author:西村 吏功
//  
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "title.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "bg_color.h"
#include "md_game_00.h"
#include "md_game_00.h"
#include "chr_enemy_00.h"
#include "atk_bullet_00.h"
#include "ranking.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// マクロ定義
//****************************************
#define WINDOW_NAME		"Jumspin Rush"		// ウインドウの名前
#define INIT_MODE		(MODE_TITLE_00)

//****************************************
// プロトタイプ宣言
//****************************************
LRESULT CALLBACK WindowProc	(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウインドウプロシージャ
HRESULT Init				(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);			// 初期化処理
void Uninit					(void);													// 終了処理
void Update					(void);													// 更新処理
void Draw					(void);													// 描画処理
void Load					(void);													// 読み込み処理
void Save					(void);													// 書き込み処理
void DrawDebugComment		(void);													// デバッグコメントの描画処理
void ShowTaskBar			(bool bShow);											// ウインドウの表示/非表示を切り替える

//****************************************
// グローバル宣言
//****************************************
LPDIRECT3D9 g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
HWND g_hWnd;							// ウインドウハンドル(識別子)
LPD3DXFONT g_pFont = NULL;				// フォントへのポインタ
int g_nCountFPS = 0;					// FPSのカウンター
bool g_bDebug = true;					// デバッグ表示のON/OFF
MODE g_mode = MODE_TITLE_N1;			// 現在のモード
D3DXVECTOR3 g_windowPos;				// ウインドウの位置
bool g_bPause = false;					// ポーズフラグ

//========== *** デバイスの取得 ***
LPDIRECT3DDEVICE9 GetDevice(void) 
{
	return g_pD3DDevice;
}

//========== *** ウインドウハンドルを取得 ***
HWND *GetWindowHandle(void) 
{
	return &g_hWnd;
}

//========== *** ウインドウの位置を取得 ***
D3DXVECTOR3 GetWindowPos(void)
{
	return g_windowPos;
}

//========== *** モードの取得 ***
MODE GetMode(void)
{
	return g_mode;
}

//========================================
// WinMain関数
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	MSG msg;				// メッセージを格納する変数
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻

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
	g_hWnd = CreateWindowEx(
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
	if (FAILED(Init(hInstance, g_hWnd, TRUE)))
	{// 初期化処理が失敗した場合

		return -1;
	}

	// ウインドウの表示
	WindowModeChange(false);

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0; // 現在時刻を初期化する
	dwExecLastTime = timeGetTime(); // 現在時刻を取得(保存)

	dwFrameCount = 0;	// フレームカウントを初期化する
	dwFPSLastTime = timeGetTime();	// 現在時刻を取得(保存)

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

			if ((dwCurrentTime - dwFPSLastTime) >= 500) 
			{// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime; // FPSを測定した時刻を保存
				dwFrameCount = 0; // フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) 
			{// 60/1秒経過
				dwExecLastTime = dwCurrentTime; // 処理開始の時刻[現在時刻]を保存

				{
					RECT rec;	// 大きさを受け取るRECT構造体

					// ウインドウの情報を取得
					GetWindowRect(g_hWnd, &rec);

					// ウインドウの位置を設定
					g_windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);
				}

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++; // フレームカウントを加算
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
// WindowModeChange関数 - ウインドウのモードを切り替える -
//========================================
void WindowModeChange(bool bFullScreen) 
{
	HDC hDC								// 
		= GetDC(GetDesktopWindow());	// デスクトップのハンドル
	int nScrSizeX						// 
		= GetDeviceCaps(hDC, HORZRES);	// デスクトップの画面サイズX
	int nScrSizeY						// 
		= GetDeviceCaps(hDC, VERTRES);	// デスクトップの画面サイズY

	// デスクトップのハンドルを手放す
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bFullScreen) 
	{// 全画面フラグが真の時、
		// ウインドウの位置を設定
		SetWindowPos(
			g_hWnd,
			HWND_TOPMOST,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// 全画面フラグが偽の時、
		// ウインドウの位置を設定
		SetWindowPos(
			g_hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// ウインドウスタイルを変更
		SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	// ウインドウの表示状態を設定
	ShowWindow(g_hWnd, SW_NORMAL);

	// クライアント領域を更新
	UpdateWindow(g_hWnd);

	// タスクバーを表示/非表示
}

//========================================
// ShowTaskBar関数 - タスクバーの表示/非表示を切り替える -
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
		SetFocus(g_hWnd);
	}
	else 
	{// 非表示
		// タスクバーを非表示
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================================
// WindowProc関数
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
//========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;	// ディスプレイモード

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
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
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 読み込み処理
	Load();

	// 初期化処理
	InitCamera();		// カメラ
	InitBg_color();		// BG :背景色
	InitSound(hWnd);	// サウンド

	ResetRanking();

	// フェードの設定
	InitFade(INIT_MODE);

	// キーボードの初期化処理
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// Uninit関数 - 終了処理 -
//========================================
void Uninit(void)
{
	// 書き込み処理
	Save();

	// 終了処理
	UninitBg_color();	// BG :背景色
	UninitMd_game_00();	// MD :ゲーム画面	[00]
	UninitFade();		// フェード
	UninitInput();		// キーボード

	// サウンドの停止
	StopSound();
	
	// サウンドの終了処理
	UninitSound();

	// フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// タスクバーを表示
	ShowTaskBar(true);
}

//========================================
// Update関数 - 更新処理 - 
//========================================
void Update(void)
{
	UpdateKeyboard();		// キーボード
	UpdateButton();			// ボタン

	if (!g_bPause)
	{// ポーズフラグが偽の時、
		// 更新処理
		UpdateCamera();			// カメラ
		UpdateCursor();			// カーソル
		UpdateFade();			// フェード
		UpdateMouse();			// マウス
		UpdateStick();			// スティック
		UpdateVibration();		// コントローラーの振動
		UpdateBg_color();		// BG :背景色

		// 現在の画面(モード)の更新処理
		switch (g_mode)
		{
			//========== *** タイトル画面[00] ***
		case MODE_TITLE_00:
		{
			
			UpdateTitle();

		}
		break;
			//========== *** ゲーム画面[00] ***
		case MODE_GAME_00:
		{
			UpdateMd_game_00();
		}
		break;
			//========== *** リザルト画面[00] ***
		case MODE_RESULT_00:

			UpdateResult();

			break;
		}
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{// F1キーを押した時、
		// デバッグ表示のON/OFFを切り替える
		g_bDebug ^= 1;
	}
	if (GetKeyboardTrigger(DIK_F2)
		||
		GetButtonTrigger(BUTTON_BACK))
	{// F2キーを押した時、
		// ポーズフラグのON/OFFを切り替える
		g_bPause ^= 1;
	}
#endif
}

//========================================
// Draw関数 - 描画処理 -
//========================================
void Draw(void)
{
	{
		Color	bgCol					//
				= GetBg_color()->col;	// 背景色

		// 画面クリア
		g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(bgCol.nR, bgCol.nG, bgCol.nB, bgCol.nA), 1.0f, 0);
	}

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合、
		// 現在の画面(モード)の描画処理
		switch (g_mode)
		{
			//========== *** タイトル画面[00] ***
		case MODE_TITLE_00:

			DrawTitle();

			break;
			//========== *** ゲーム画面[00] ***
		case MODE_GAME_00:

			DrawMd_game_00();

			break;
			//========== *** リザルト画面[00] ***
		case MODE_RESULT_00:

			DrawResult();

			break;
		}

		// 描画処理
		DrawFade();			// フェード

#ifdef _DEBUG
		if (g_bDebug)
		{// デバッグ表示がONの時、
			// デバッグコメントの描画処理
			DrawDebugComment();
		}
#endif

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファをフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// Load関数 - 読み込み処理 -
//========================================
void Load(void)
{
	// 読み込み処理
	LoadAtk_bullet_00();
	LoadChr_enemy_00();		// CHR:敵				[00]
	
	// 敵の配置情報の読み込み
	LoadEnemy();
}

//========================================
// Save関数 - 書き込み処理 -
//========================================
void Save(void)
{

}

//========================================
// DrawComment関数 - デバッグコメントの描画処理 -
//========================================
void DrawDebugComment(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aString[TXT_MAX];
	
	sprintf(aString, "FPS：%d\n", g_nCountFPS);

	g_pFont->DrawText(NULL, aString, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
// SetMode関数 - モードの設定 -
//========================================
void SetMode(MODE mode) 
{
	// 現在の画面(モード)の終了処理
	switch (g_mode)
	{
		//========== *** タイトル画面[00] ***
	case MODE_TITLE_00:

		UninitTitle();
		
		break;
		//========== *** ゲーム画面[00] ***
	case MODE_GAME_00:
		
		UninitMd_game_00();
		
		break;
		//========== *** リザルト画面[00] ***
	case MODE_RESULT_00:

		UninitResult();

		break;
	}

	// 新しい画面(モード)の初期化処理
	switch (mode)
	{
		//========== *** タイトル画面[00] ***
	case MODE_TITLE_00:

		InitTitle();

		break;
		//========== *** ゲーム画面[00] ***
	case MODE_GAME_00:

		InitMd_game_00();

		break;
		//========== *** リザルト画面[00] ***
	case MODE_RESULT_00:

		InitResult();

		break;
	}

	// 現在の画面(モード)を切り替える
	g_mode = mode;
}

//========================================
// Lottery関数 - 抽選処理 -
//========================================
bool Lottery(float fProb)
{
	if (fProb > (0.1f * (rand() % 1000)))
	{// 当選した時、
		// 真を返す
		return true;
	}
	else 
	{// 当選しなかった時、
		// 偽を返す
		return false;
	}
}

//========================================
// fRand関数 - 小数の乱数を返す -
//========================================
float fRand(float fMax) 
{
	float fNum;	// 小数の値

	fMax *= 100;

	fNum = rand() % (int)fMax;

	return fNum * 0.01f;
}

//========================================
// MatchPosition関数 - 描画位置を基準に合わせる -
//========================================
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos)
{
	// 位置をプレイヤーの位置に合わせる
	pDrawPos->x = pDrawPos->x - basePos.x + (SCREEN_WIDTH * 0.5f);
	pDrawPos->y = pDrawPos->y - basePos.y + (SCREEN_HEIGHT * 0.5f);
}

//========================================
// PrusMinus関数 - 与えられた値に応じて1を符号付きで返す -
//========================================
int PrusMinus(float fNum)
{
	if (fNum == 0) 
	{// 引数が0だった時、
		// 0を返す
		return 0;
	}

	// 1を符号付きで返す
	return (fabsf(fNum) / fNum);
}

//========================================
// strinit関数 - 空白埋め -
//========================================
void strinit(char *pData, int nNum) 
{
	for (int nCnt = 0; nCnt < nNum; nCnt++) 
	{
		pData[nCnt] = '\0';
	}
}
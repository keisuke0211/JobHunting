//◇====================◇
//｜　　　　　　　　　　｜
//｜　メインの処理      ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇

//ゲーム名 
//Custom Aircraft(カスタム　エアクラフト)

//◇========================◇
//｜　インクルードファイル　｜
//◇========================◇
#include "main.h"			//作成した　main.h をインクルードする
#include "sound.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result ver1.h"
#include "result ver2.h"
#include "ranking_bg.h"
#include "ranking_score.h"
#include "fade.h"
#include "enemy.h"
#include <stdio.h>
#include "tutorial.h"
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define CLASS_NAME    "WindowClass"        //ウインドウクラスの名前
#define WINDOW_NAME   "Custom Aircraft"  //ウインドウの名前(キャプションに表示)
#define IDI_ICON1 (126)
#define IDI_ICON2 (125)

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hlnstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3D9		  g_pD3D = NULL;			    //Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		    //Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;						//フォントへのポインタ
int g_nCountFPS = 0;							//FPSカウンタ
int DisplayMode = 0;							//操作方法表示　ON/OFF

MODE g_mode = MODE_TITLE;						//現在のモード


//==============================================================================================================================================
//――――――――――――――――――――――――――――――｜【メイン・処理】｜―――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇==========================◇
//｜　　　　メイン関数　　　　｜
//◇==========================◇
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{

	//◇――――――――――――――◇
	//｜　ウインドウクラスの構造体　｜
	//◇――――――――――――――◇

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						  //WNDCLASSEXのメモリサイズ
		CS_CLASSDC,								  //ウインドウのスタイル
		WindowProc,								  //ウインドウプロシージャ
		0,										  //0にする(通常は使用しない)
		0,										  //0にする(通常は使用しない)
		hinstance,								  //インスタンスハンドル
		LoadIcon(NULL,((LPCSTR)IDI_ICON2)),		  //タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),				  //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				  //クライアント領域の背景色
		NULL,									  //メニューバー
		CLASS_NAME,								  //ウインドウクラスの名前
		LoadIcon(NULL,((LPCSTR)IDI_ICON2)),		  //ファイルのアイコン
	};
	HWND hWnd;									  //ウインドウハンドル(識別子)
	MSG msg;									  //メッセージを格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };//画面サイズの構造体
	//◇――――――――――――――◇
	//｜　 ウインドウクラスの登録　 ｜
	//◇――――――――――――――◇

	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	//◇――――――――――――――◇
	//｜　　　ウインドウの生成　　　｜
	//◇――――――――――――――◇
	hWnd = CreateWindowEx(0, //拡張ウィンドウスタイル
		CLASS_NAME,          //ウインドウクラスの名前
		WINDOW_NAME,         //ウインドウの名前
		WS_OVERLAPPEDWINDOW, //ウインドウスタイル
		CW_USEDEFAULT,       //ウインドウの左上X座標
		CW_USEDEFAULT,       //ウインドウの左上Y座標
		SCREEN_WIDTH,        //ウインドウの幅
		SCREEN_HEIGHT,       //ウインドウの高さ
		NULL,                //親ウインドウのハンドル
		NULL,                //メニューハンドルまたは子ウインドウID
		hinstance,           //インスタンスハンドル
		NULL);               //ウインドウ作成デー

	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	//◇――――――――――――――◇
	//｜　　　 　初期化処理 　　　　｜
	//◇――――――――――――――◇
	if (FAILED(Init(hinstance, hWnd, TRUE)))
	{//初期化処理に失敗した場合
		return -1;
	}
	//分解能を設定
	timeBeginPeriod(1);

	 dwCurrentTime = 0;					//初期化する
	 dwExecLastTime = timeGetTime();	//現在時刻を所得
	 dwFrameCount = 0;					//初期化する
	 dwFPSLastTime = timeGetTime();		//現在時刻を所持
	//◇――――――――――――――◇
	//｜　　　ウインドウの表示　　　｜
	//◇――――――――――――――◇
	ShowWindow(hWnd, nCmdShow);  //ウインドウの表示状態を設定
	UpdateWindow(hWnd);          //クライアント領域を更新

	 //メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
		 
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg); //仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);  //ウインドウプロシージャへメッセージを送出
			}

		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();//現在時刻を所得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//005秒経過
			//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;//FPSを測定した時刻を保存
				dwFrameCount = 0;//フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過したら

				dwExecLastTime = dwCurrentTime;//処理開始の時刻(現在時刻)を保存

			    //更新処理
				Update();

				//描画処理
				Draw();

				dwFrameCount++;//フレームカウントを加算
			}
		}
	}
	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}
//◇==========================◇
//｜　ウインドウプロシージャ　｜
//◇==========================◇
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;


	XINPUT_STATE *xInput = GetXInputState();	//プレイヤーの情報へのポインタ

	

	switch (uMsg)
	{
	case WM_DESTROY: //ウインドウ破棄もメッセージ

		 //WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);	//Destroyメッセージを送る
			}
			break;
		}
		break;
	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			DestroyWindow(hWnd);	//Destroyメッセージを送る
		}
		else
		{
			return 0;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);//既定の処理を返す
}
//◇==========================◇
//｜　　　デバイスの所得　　　｜
//◇==========================◇
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//==============================================================================================================================================
//――――――――――――――――――――――――――――――｜【Direct3D・処理】｜――――――――――――――――――――――――――――――
//==============================================================================================================================================

//◇==================◇
//｜　　初期化処理　　｜
//◇==================◇
HRESULT Init(HINSTANCE hlnstance, HWND hWnd, BOOL bWindow)//TRUE:ウインドウ/FALSE:フルスクリーン
{
	D3DDISPLAYMODE d3ddm;        //ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp; //プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));   //パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;                          //ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;                        //ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;                         //バックバッファの形式
	d3dpp.BackBufferCount = 1;                                     //バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                      //ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;                           //デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                     //デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;                                      //ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;    //リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;      //インターバル

	//Direct3Dデバイスも生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,                     //描画処理
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,//頂点処理
		&d3dpp,
		&g_pD3DDevice)))                    //描画処理と頂点処理をハードウェアで行う
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL                     ,//描画処理
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,//頂点処理
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,                     //描画処理
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,//頂点処理
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PALETTE,
		"Terminal", &g_pFont);

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステート設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//キーボードの初期化処理
	if (FAILED(lnitKeyboard(hlnstance, hWnd)))
	{
		return E_FAIL;
	}

	//サウンドの初期化処理
	InitSound(hWnd);

	//フェードの設定
	lnitFade(g_mode);

	//モードの設定
	SetMode(g_mode);

	//各種オブジェクトの初期化処理
	return S_OK;
}
//◇==================◇
//｜　 　終了処理　 　｜
//◇==================◇
void Uninit(void)
{
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　　各種オブジェクトの終了処理
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	//◇――――――――――――――◇
	//｜　　　　　終了処理　　　　　｜
	//◇――――――――――――――◇

	//サウンドの終了処理
	StopSound();

	UninitSound();

	//キーボードの終了処理
	UninitKeyboard();

	//タイトル画面の終了処理
	UninitTitle();

	//チュートリアル画面の終了処理
	UninitTutorial();

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面１の終了処理
	UninitResult1();

	//リザルト画面２の終了処理
	UninitResult2();

	//ランキング画面の終了処理
	UninitRanking();

	//Xinputを閉じる
	XInputEnable(false);

	//フェードの終了処理
	UninitFade();

	//デバック表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	//◇――――――――――――――◇
	//｜　　　 デバイスの破棄　 　　｜
	//◇――――――――――――――◇
	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//◇――――――――――――――◇
	//｜　　　オブジェクトの破棄　　｜
	//◇――――――――――――――◇
	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//◇==================◇
//｜　　 更新処理　　 ｜
//◇==================◇
void Update(void)
{
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	//　　各種オブジェクトの更新処理
	//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

	//キーボードの更新処理
	UpdateKeyboard();

	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UpdateTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		UpdateGame();
		break;

	case MODE_GAME_OVER:	//リザルト画面・ゲームオーバー
		UpdateResult1();
		break;

	case MODE_GAME_CLEAR:	//リザルト画面・ゲームクリア
		UpdateResult2();
		break;

	case MODE_RANKING:		//ランキング画面
		UpdateRanking();
		break;
	}
	//フェードの更新処理
	UpdateFade();
}

//◇==================◇
//｜　　 描画処理　　 ｜
//◇==================◇
void Draw(void)
{
	//画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	
	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

	 //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	 //　　各種オブジェクトの描画処理
	 //＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

		switch (g_mode)
		{
		case MODE_TITLE:		//タイトル画面
			DrawTitle();
			break;

		case MODE_TUTORIAL:		//チュートリアル画面
			DrawTutorial1();
			break;

		case MODE_GAME:			//ゲーム画面
			DrawGame();
			break;

		case MODE_GAME_OVER:	//リザルト画面・ゲームオーバー
			DrawResult1();
			break;

		case MODE_GAME_CLEAR:	//リザルト画面・ゲームクリア
			DrawResult2();
			break;

		case MODE_RANKING:		//ランキング画面
			DraeRanking();
			break;
		}

		//フェードの描画処理
		DrawFade();

		//テキストの描画処理
		RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
		RECT rect1 = { 0,250,SCREEN_WIDTH,SCREEN_HEIGHT };

		char aStr[500];
		char aStr2[500];

		if (GetKeyboardTrigger(DIK_F1) == true)
		{
			if (DisplayMode == 0)
			{
				DisplayMode++;
			}
			else if (DisplayMode == 1)
			{
				DisplayMode--;
			}
		}
		if (DisplayMode == 1)
		{
			//文字列に代入
			wsprintf(&aStr[0], "\n 【 操作方法 】 \n◇―――――――◇\n｜上移動：  [W] ｜\n｜下移動：  [S] ｜\n｜左移動：  [A] ｜\n｜右移動：  [D] ｜\n｜左回転：  [←]｜\n｜右回転：  [→]｜\n｜拡大　：  [↑]｜\n｜縮小　：  [↓]｜\n｜弾　　:[SPACE]｜\n◇―――――――◇\n");
			wsprintf(&aStr2[0], "\n   【 攻撃 】   \n◇―――――――◇\n｜切換　： [F2] ｜\n｜弾数  ： [F3] ｜\n◇―――――――◇\n");

			//テキストの描画
			g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
			g_pFont->DrawText(NULL, &aStr2[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
		}
#ifdef _DEBUG

		//FPSの表示
		DrawFPS();

#endif // _DEBUG

	    //描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//◇==================◇
//｜　　 FPS処理　 　 ｜
//◇==================◇
void DrawFPS(void)
{
	RECT rect = { 0,250,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	sprintf(&aStr[0], "FPS:%d\n角度:%f\n出現時間:%d", g_nCountFPS, GetStick(STICK_TYPE_LEFT).fAngle, GetCunt());

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}
//◇=================◇
//｜　モードの設定　 ｜
//◇=================◇
void SetMode(MODE mode)
{
	//現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:		//タイトル画面
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UninitTitle();
		break;

	case MODE_GAME:			//ゲーム画面
		UninitGame();
		break;

	case MODE_GAME_OVER:	//リザルト画面・ゲームオーバー
		UninitResult1();
		break;

	case MODE_GAME_CLEAR:	//リザルト画面・ゲームクリア
		UninitResult2();
		break;

	case MODE_RANKING:		//ランキング画面
		UninitRanking();
		break;
	}

	//新しい画面の初期化処理
	switch (mode)
	{
	case MODE_TITLE:		//タイトル画面
		InitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		InitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		InitGame();
		break;

	case MODE_GAME_OVER:	//リザルト画面・ゲームオーバー
		InitResult1();
		break;

	case MODE_GAME_CLEAR:	//リザルト画面・ゲームクリア
		InitResult2();
		break;

	case MODE_RANKING:		//ランキング画面
		InitRanking();
		break;
	}

	g_mode = mode; //現在の画面に切り替える
}

//◇=================◇
//｜　モードの所得　 ｜
//◇=================◇
MODE GetMode(void)
{
	return g_mode;
}
//◇====================◇
//｜　　　　　　　　　　｜
//｜　me\ain.h          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _MAIN_H_			         									    //このマクロ定義がされてなかったら
#define _MAIN_H_             
//◇========================◇
//｜　インクルードファイル　｜
//◇========================◇
#include<windows.h>//main.cppから移動
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include"XInput.h"
#include"d3dx9.h"  //描画処理に必要
#define DIRECTINPUT_VERSION (0X0800) //ビルド時の警告対処用のマクロ
#include"dinput.h"//入力処理に必要//２重インクルード帽子のマクロを定義する
//◇====================◇
//｜　　ライブラリイ　　｜
//◇====================◇
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント（部品）使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻所得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
#pragma comment(lib,"xinput.lib")	//入力処理に必要
#include"xaudio2.h"					//サウンド処理に必要
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define SCREEN_WIDTH     (1280.0f)											    //ウインドウの幅
#define SCREEN_HEIGHT    (720.0f)												//ウインドウの高さ　←main.cppから移動
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)			//頂点フォーマット【2D】
#define SWITCH_TIME		 (600)													//切り替わる時間(1秒 = 60)　※一部の画面切り替えに仕様
//◇====================◇
//｜ 　　情報構造体 　　｜
//◇====================◇

//画面(モード)の種類
typedef enum
{
	MODE_TITLE = 0,	//タイトル画面
	MODE_TUTORIAL,	//チュートリアル画面
	MODE_GAME,		//ゲーム画面
	MODE_RESULY1,	//ゲームオーバー
	MODE_RESULY2,	//ゲームクリア
	MODE_RANKING,	//ランキング画面
	MODE_MAX
} MODE;
//頂点情報【2D】の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換係数（1.0fで固定）
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
} VERTEX_2D;
//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
LPDIRECT3DDEVICE9 GetDevice(void);
void Position(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos);	// 描画位置を基準に合わせる
void SetMode(MODE mode);
MODE GetMode(void);
#endif
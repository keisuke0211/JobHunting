//========================================
// *** text.h ***
//========================================
#ifndef _TEXT_H_	// このマクロ定義がされていなかった時
#define _TEXT_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// マクロ定義
//****************************************
#define TEXT_MAX	(128)	// テキストの最大数

//****************************************
// 列挙型の定義
//****************************************

// テキストの使用領域
typedef enum
{
	TEXT_FIELD_UI = 0,	// UI
	TEXT_FIELD_DAMAGE,	// ダメージ表示
	TEXT_FIELD_MENU,	// メニュー
	TEXT_FIELD_MAX,
}TEXT_FIELD;

// テキストのモード
typedef enum 
{
	TEXT_MODE_LEFT = 0,	// 右詰め
	TEXT_MODE_RIGHT,	// 左詰め
	TEXT_MODE_CENTER,	// 中心
	TEXT_MODE_MAX
}TEXT_MODE;

// テキストのフォント
typedef enum
{
	FONT_000 = 0,	// フォント[000]
	FONT_001,		// フォント[001]
	FONT_002,		// フォント[002]
	FONT_003,		// フォント[003]
	FONT_004,		// フォント[004]
	FONT_MAX
}FONT;

//****************************************
// 構造体の定義
//****************************************

// テキストの情報
typedef struct 
{
	char		aDisplay[TXT_MAX];	// 表示形式
	D3DXVECTOR3 pos;				// 位置
	Color		col;				// 色
	FONT		font;				// フォント
}Text;

// フォント情報の構造体を定義
typedef struct
{
	char aPath[TXT_MAX];	// テクスチャファイルの相対パス
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fSpaceX;			// 配置間隔X
	float fSpaceY;			// 配置間隔Y
	char cStartChar;		// 開始文字
	int nPtnMaxX;			// テキストのパターン数(横)
	int nPtnMaxY;			// テキストのパターン数(縦)
}Font;

//****************************************
// プロトタイプ宣言
//****************************************
Font *GetFont(void);	// フォントの情報を取得
void InitTxt_00		(void);																																			// テキストの初期化処理
void UninitTxt_00	(void);																																			// テキストの終了処理
void UpdateTxt_00	(TEXT_FIELD textField, int nNumber, char aString[TXT_MAX], D3DXVECTOR3 pos, Color col, TEXT_MODE mode, FONT font, float fWidth, float fHeight);	// テキストの更新処理
void DrawTxt_00		(void);																																			// テキストの描画処理
void ShowTxt_00		(TEXT_FIELD textField, int nNumber, bool bShow);																								// テキストの表示/非表示を切り替える

#endif
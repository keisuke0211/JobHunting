//◇=====================◇
//｜　　　　　　　　     ｜
//｜　HPゲージの枠の処理 ｜
//｜　Author:大殿慶輔　  ｜
//｜                     ｜
//◇=====================◇
#include"boss_hp_flame.h"
#include"boss.h"

//◇==================◇
//｜　グローバル変数　｜
//◇==================◇
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffHpflame = NULL;			//頂点バッファポインタ
LPDIRECT3DTEXTURE9		g_pTextureHpflame = NULL;			//テクスチャへのポインタ

//◇====================◇
//｜　　　　　　　　　　｜
//｜　bullet.h          ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"
#include"explosion.h"
#include"enemy.h"
#include"player.h"
#include"boss.h"

//◇==================◇
//｜　 列挙型構造体 　｜
//◇==================◇
//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,//プレイヤーの弾
	BULLETTYPE_ENEMY,     //敵の弾
	BULLETTYPE_MAX
}BULLETTYPE;

//◇==================◇
//｜　 構造体の定義 　｜
//◇==================◇
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	float rot;			//向き
	int nLife;			//寿命
	BULLETTYPE type;	//種類
	bool bUse;			//使用しているかどうか
}Bullet;

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float rot ,BULLETTYPE type);//pos 発射位置：move 移動量
Bullet *GetBullet(void);
//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define BULLET_WIDTH  (16.0f)
#define BULLET_HEIGHT (16.0f)
#define MAX_DATA		(2)
#define LIFE_DATA     (60)
#endif


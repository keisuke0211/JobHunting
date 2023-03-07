//◇====================◇
//｜　　　　　　　　　　｜
//｜　enemy_bullet.h    ｜
//｜　Author:大殿慶輔　 ｜
//｜                    ｜
//◇====================◇
#ifndef _ENEMY_BULLET_H_
#define _ENEMY_BULLET_H_

#include"main.h"
#include"explosion.h"
#include"enemy.h"
#include"player.h"


//◇==================◇
//｜　 列挙型構造体 　｜
//◇==================◇
//弾の種類
typedef enum
{
	BULLETTYPE_E_LINE = 0,		//直線
	BULLETTYPE_E_AIM,			//エイム（プレイヤーに向かって撃つ）
	BULLETTYPE_E_HOMING,		//ホーミング
	BULLETTYPE_E_3_SHOT,		//３方向攻撃
	BULLETTYPE_S_1_SUMMON,		//召喚（ボス第二段階）
	BULLETTYPE_E_CHARGE,		//チャージ
	BULLETTYPE_E_MACHINEGUN,	//機関銃
	BULLETTYPE_S_2_SUMMON,		//召喚（ボス第三段階）
	BULLETTYPE_E_CHARGE1,		//チャージ1
	BULLETTYPE_E_3_SHOT2,		//３方向攻撃
	BULLETTYPE_E_CHARGE_SHOT,	//チャージショット
	BULLETTYPE_E_MAX
}BULLETTYPE_E;

//◇====================◇
//｜　プロトタイプ宣言　｜
//◇====================◇
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE_E type,int stype);//pos 発射位置：move 移動量

//◇==============◇
//｜　マクロ定義　｜
//◇==============◇
#define BULLET_WIDTH1  (16.0f
#define BULLET_HEIGHT1 (16.0f)
#define MAX_BULLET1		(3)
#define BULLET_SPEED1 (2.5f)
#define LIFE_DATA1     (60)
#endif


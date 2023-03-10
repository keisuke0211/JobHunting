//====================
//b@@@@@@@@@@b
//b@enemy_bullet.h    b
//b@Author:åacã@ b
//b                    b
//====================
#ifndef _ENEMY_BULLET_H_
#define _ENEMY_BULLET_H_

#include"main.h"
#include"explosion.h"
#include"enemy.h"
#include"player.h"


//==================
//b@ ñ^\¢Ì @b
//==================
//eÌíÞ
typedef enum
{
	BULLETTYPE_E_LINE = 0,		//¼ü
	BULLETTYPE_E_AIM,			//GCivC[Éü©ÁÄÂj
	BULLETTYPE_E_HOMING,		//z[~O
	BULLETTYPE_E_3_SHOT,		//RûüU
	BULLETTYPE_S_1_SUMMON,		//¢«i{XæñiKj
	BULLETTYPE_E_CHARGE,		//`[W
	BULLETTYPE_E_MACHINEGUN,	//@Öe
	BULLETTYPE_S_2_SUMMON,		//¢«i{XæOiKj
	BULLETTYPE_E_CHARGE1,		//`[W1
	BULLETTYPE_E_3_SHOT2,		//RûüU
	BULLETTYPE_E_CHARGE_SHOT,	//`[WVbg
	BULLETTYPE_E_MAX
}BULLETTYPE_E;

//====================
//b@vg^Cvé¾@b
//====================
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE_E type,int stype);//pos ­ËÊuFmove Ú®Ê

//==============
//b@}Nè`@b
//==============
#define BULLET_WIDTH1  (16.0f
#define BULLET_HEIGHT1 (16.0f)
#define MAX_BULLET1		(3)
#define BULLET_SPEED1 (2.5f)
#define LIFE_DATA1     (60)
#endif


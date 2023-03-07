//========================================
// 
// キャラクターの処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** character.cpp ***
//========================================
#include "main.h"
#include "character.h"
#include "chr_enemy_00.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadCharacter関数 - キャラクターの読み込み処理 -
// Author:RIKU NISHIMURA
//========================================
void LoadCharacter(void) 
{
	LoadChr_enemy_00();
}
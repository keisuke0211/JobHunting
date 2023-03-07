//====================
//b@@@@@@@@@@b
//b@pause.h           b
//b@Author:åacã@ b
//b                    b
//====================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include"main.h"

//==================
//b@ ñ^\¢Ì @b
//==================

//po|[Yj[
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	//Q[Éßé
	PAUSE_MENU_RETRY,			//Q[ðâè¼·
	PAUSE_MENU_QUIT,			//^CgæÊÉßé
	PAUSE_MENU_MAX
}PAUSE_MENU;

//====================
//b@vg^Cvé¾@b
//====================
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
#endif
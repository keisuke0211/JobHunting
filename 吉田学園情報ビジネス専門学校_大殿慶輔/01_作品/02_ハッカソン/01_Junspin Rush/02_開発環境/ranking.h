//==========================================
//
//�����L���O�\���v���O�����̃w�b�_[ranking.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//�}�N��
#define MAX_RANK	(5)		//�����L���O�\����

//�����L���O�\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int nScore;			//�X�R�A
	bool bDisp;			//�\���E��\��
} Ranking;

//�v���g�^�C�v�錾
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(int nScore);

#endif // !_RANKING_H_

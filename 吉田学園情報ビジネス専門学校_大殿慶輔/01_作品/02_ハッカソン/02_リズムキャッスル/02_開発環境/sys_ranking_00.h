//========================================
// 
// SYS:�����L���O[00] �̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** sys_ranking_00.h ***
//========================================
#ifndef _SYS_RANKING_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _SYS_RANKING_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************
// �����L���O�̏��ʐ�
#define RANKING_NUM	(8)

//****************************************
// �\���̂̒�`
//****************************************
// �����L���O�\����
typedef struct
{
	int	nScore;	// �X�R�A
}Sys_ranking_00;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ���ʂ̕\���`�����擾
char *GetRankText(int nRank);

// SYS:�����L���O[00] �̏����擾
Sys_ranking_00 *GetSys_ranking_00(void);

// SYS:�����L���O[00] �̏���������
// SYS:�����L���O[00] �̏I������
// SYS:�����L���O[00] �̍X�V����
void InitSys_ranking_00(void);
void UninitSys_ranking_00(void);
void UpdateSys_ranking_00(void);

// SYS:�����L���O[00] �̓ǂݍ��ݏ���
// SYS:�����L���O[00] �̏������ݏ���
void LoadSys_ranking_00(void);
void SaveSys_ranking_00(void);

// �����L���O�ɃX�R�A��}������
int SetScore(int nScore);

#endif
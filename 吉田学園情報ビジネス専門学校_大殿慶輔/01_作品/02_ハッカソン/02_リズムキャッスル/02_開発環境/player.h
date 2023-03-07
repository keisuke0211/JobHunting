//==========================================
//
//�v���C���[�֌W�v���O�����̃w�b�_[player.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//�}�N��
//�ق�

//�����
typedef enum
{
	JUDGE_EXELENT = 0,
	JUDGE_GOOD,
	JUDGE_MISS,
	JUDGE_MAX
} JUDGE;

//�v���C���[�^�\���̒�`
typedef struct
{
	//�����ڊ֌W
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 move;					//�ړ���	
	D3DXVECTOR3 rot;					//����
	int nCounterAnim;					//�A�j���[�V�����J�E���^
	int nPatternAnim;					//�A�j���[�V�����p�^�[��No.

	//�X�e�[�^�X�֌W
	int nLife;							//�̗�
	int nCounterAttack;					//�U���A�j���[�V�����J�n����o�߂�������
	int nScore;							//�X�R�A
	int nCombo;							//�R���{
	int nCounterComboMoveTime;			//�R���{�\���J�n����o�߂�������

	//�g�p�L��
	bool bDisp;							//�����Ă��邩�ǂ���
} Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void SetPlayer(void);

#endif // !_PLAYER_H_

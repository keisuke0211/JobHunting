//========================================
// *** ui_life_000.h ***
//========================================
#ifndef _UI_LIFE_000_H_	//���̃}�N����`������Ă��Ȃ�������
#define _UI_LIFE_000_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

//========== �\���̂̒�`		==========//

//���C�t000�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	float fLength;		//�Ίp���̒���
	float fAngle;		//�Ίp���̊p�x
	int nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int nPattern;		//�p�^�[��No.
}Ui_life_000;

//========== �v���g�^�C�v�錾	==========//
void InitUi_life_000(void);		//���C�t000�̏���������
void UninitUi_life_000(void);	//���C�t000�̏I������
void UpdateUi_life_000(void);	//���C�t000�̍X�V����
void DrawUi_life_000(void);		//���C�t000�̕`�揈��

#endif
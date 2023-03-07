//========================================
// *** bg_color.h ***
//========================================
#ifndef _BG_COLOR_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _BG_COLOR_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �}�N����`
//****************************************

// �w�i�F�̏����l
#define BG_START_COLOR	{77, 207, 217, 255}

//****************************************
// �\���̂̒�`
//****************************************

// �w�i�F�̏��\����
typedef struct 
{
	Color	col;					// �F
	Color	oldColor;				// �Â��F
	Color	newColor;				// �V�����F
	bool	bChangeColor;			// �ϐF�t���O
	int		nCounterChangeColor;	// �ϐF�J�E���^�[
	int		nTimeChangeColor;		// �ϐF�ɂ����鎞��
}Bg_color;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �w�i�F�̏����擾
Bg_color *GetBg_color(void);

// �w�i�F�̏���������
void InitBg_color(void);

// �w�i�F�̏I������
void UninitBg_color(void);

// �w�i�F�̍X�V����
void UpdateBg_color(void);

// �w�i�F�̐ݒ菈��
void SetBg_color(Color newColor, int nTime);

#endif
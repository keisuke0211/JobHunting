//========================================
// *** ui_frame_02.h ***
//========================================
#ifndef _UI_FRAME_02_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _UI_FRAME_02_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �t���[��[02] �̏��
typedef enum
{
	UI_FRAME_02_STATE_POP = 0,	// �o��
	UI_FRAME_02_STATE_IN_POP,	// �o����
	UI_FRAME_02_STATE_CLEAR,	// �����Ă���
	UI_FRAME_02_STATE_IN_CLEAR,	// ������
	UI_FRAME_02_STATE_MAX,
}UI_FRAME_02_STATE;

//****************************************
// �\���̂̒�`
//****************************************

// �t���[��[02] �̏��\����
typedef struct
{
	UI_FRAME_02_STATE	state;			// ���
	int					nCounterState;	// ��ԃJ�E���^�[
	float				fAlpha;			// �S�̂̓����x
	D3DXVECTOR3			pos;			// �ʒu
}Ui_frame_02;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �t���[��[02] �̏����擾
Ui_frame_02 *GetUi_frame_02(void);

// �t���[��[02] �̏���������
void InitUi_frame_02(void);

// �t���[��[02] �̏I������
void UninitUi_frame_02(void);

// �t���[��[02] �̍X�V����
void UpdateUi_frame_02(void);

// �t���[��[02] �̕`�揈��
void DrawUi_frame_02(void);

// �t���[��[02] �̏�Ԑݒ菈��
void SetStateUi_frame_02(UI_FRAME_02_STATE state);

// �t���[��[02] �̐ݒ菈��
void SetUi_frame_02(D3DXVECTOR3 pos);

#endif
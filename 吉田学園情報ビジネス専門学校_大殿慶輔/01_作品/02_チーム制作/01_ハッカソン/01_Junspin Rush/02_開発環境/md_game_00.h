//========================================
// *** md_game_00.h ***
//========================================
#ifndef _MD_GAME_00_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MD_GAME_00_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "text.h"

//****************************************
// �񋓌^�̒�`
//****************************************

// �Q�[�����[00] �̏��
typedef enum 
{
	MD_GAME_00_STATE_N1 = 0,			// �������Ă��Ȃ�
	MD_GAME_00_STATE_PAUSE,				// �|�[�Y
	MD_GAME_00_STATE_START_WAIT,		// �X�^�[�g�҂�
	MD_GAME_00_STATE_CLEAR_WAIT,		// �N���A�҂�
	MD_GAME_00_STATE_NORMAL,			// �ʏ�
	MD_GAME_00_STATE_GAME_CLEAR,		// �Q�[���N���A
	MD_GAME_00_STATE_GAME_OVER,			// �Q�[���I�[�o�[
	MD_GAME_00_STATE_DIED_AFTERGLOW,	// ���S��̗]�C
	MD_GAME_00_STATE_END,				// �I��
	MD_GAME_00_STATE_MAX
}MD_GAME_00_STATE;

// �Q�[�����[00] �̃e�L�X�g�ԍ�
typedef enum
{
	MD_GAME_00_TEXT_SIGNAL,			// ���}
	MD_GAME_00_TEXT_TIME,			// ����
	MD_GAME_00_TEXT_SCORE,			// �X�R�A
	MD_GAME_00_TEXT_GAME_CLEAR,		// �Q�[���N���A
	MD_GAME_00_TEXT_GAME_OVER,		// �Q�[���I�[�o�[
	MD_GAME_00_TEXT_MAX,
}MD_GAME_00_TEXT;

//****************************************
// �\���̂̒�`
//****************************************

//�G�\���̂̒�`
typedef struct
{
	int nType;			//�G�E���
	int in_time;		//�o������
	float nAngle;		//����
	D3DXVECTOR3 pos;	//�ʒu
}EnemySet;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �X�R�A���擾
int *GetScore(void);

// �Q�[�����[00]�̏���������
void InitMd_game_00(void);

// �Q�[�����[00]�̏I������
void UninitMd_game_00(void);

// �Q�[�����[00]�̍X�V����
void UpdateMd_game_00(void);

// �Q�[�����[00]�̕`�揈��
void DrawMd_game_00(void);

// �Q�[�����[00]�̃e�L�X�g�̏����擾
Text GetMd_game_00Text(MD_GAME_00_TEXT text);

// �Q�[���̏�Ԃ̐ݒ�
void SetMd_game_00State(MD_GAME_00_STATE state);

// �Q�[���̏�Ԃ̎擾
MD_GAME_00_STATE GetGameState(void);

// �G�̔z�u���̓ǂݍ���
void LoadEnemy(void);

#endif
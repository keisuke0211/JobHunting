//==========================================
//
//�v���C���[�֌W�v���O�����̃w�b�_[player.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//�}�N��
#define PLAYER_SIZE_WIDTH		(128)	//�v���C���[�̃|���S���T�C�Y�i���j
#define PLAYER_SIZE_HEIGHT		(64)	//�v���C���[�̃|���S���T�C�Y�i����)
#define PLAYER_HIT_TEST_RADIUS	(16.0f)	//�v���C���[�̓����蔻��̔��a
#define DEFAULT_LIFE			(3)		//�����̗�
#define GROUND_HEIGHT			(64.0f)	//�n�ʂ̍���
#define DAMAGE_TIME				(60)	//�_���[�W����

//�v���C���[��ԁi�񋓁j
typedef enum
{
	PLAYERSTATE_FREEFALL = 0,	//����
	PLAYERSTATE_NORMAL,			//�ʏ�
	PLAYERSTATE_DAMAGE,			//�_���[�W
	PLAYERSTATE_JUMP,			//�W�����v
	PLAYERSTATE_MAX
} PLAYERSTATE;

//�v���C���[�^�\���̒�`
typedef struct
{
	//�ʒu
	D3DXVECTOR3 pos;					//�ʒu

	//��]�֌W
	D3DXVECTOR3 rot;					//����
	float fLength;						//�Ίp���̒���
	float fAngle;						//�Ίp���̊p�x
	float fRotSpeed;					//��]���x

	//�A�j���[�V�����֌W
	int nCounterAnim;					//�A�j���[�V�����J�E���^
	int nDamageCounter;					//�_���[�W�J�E���^�[
	int nPatternAnim;					//�A�j���[�V�����p�^�[��No.
	bool bLanding;						//���n�t���O

	//���
	PLAYERSTATE state;					//���
	bool bTurn;							//����Ă��邩
	bool bChainJump;					//2�x�ڈȍ~�̃W�����v���̃t���O
	int nLife;							//�̗�

	//�^���֌W
	float fSpeed;						//���x�iu�j
	DWORD dwExecSkyLastTime;			//�W�����v�E���R�������n�߂�����
	int nCounterTurn;					//��]����

	//�N�[���^�C��
	int nCoolTime;						//�N�[���^�C��

	//�\���E��\��
	bool bDisp;							//�����Ă��邩�ǂ���
} Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

#endif // !_PLAYER_H_

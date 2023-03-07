//========================================
// *** camera.h ***
//========================================
#ifndef _CAMERA_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

//****************************************
// �񋓌^�̒�`
//****************************************

// �J�����̏��
typedef enum
{
	CAMERA_STATE_VIBRATION = 0,	// �U��
	CAMERA_STATE_MAX,
}CAMERA_STATE;

//****************************************
// �\���̂̒�`
//****************************************

// �J�����̏��\����
typedef struct 
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 vibrationPos;		// �U���ʒu
	D3DXVECTOR3 vibrationTargetPos;	// �U���̖ڕW�ʒu

	// ��Ԋ֘A
	bool	aState[CAMERA_STATE_MAX];			// ��ԃt���O
	int		aCounterState[CAMERA_STATE_MAX];	// ��ԊǗ��J�E���^�[
	float	fVibration;							// �U���̋���
}Camera;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �J�����̏���������
void InitCamera(void);

// �J�����̏I������
void UninitCamera(void);

// �J�����̍X�V����
void UpdateCamera(void);

// �J�����̏����擾
Camera *GetCamera(void);

// �J�����̈ʒu�ɍ��킹��
void MatchCameraPosition(D3DXVECTOR3 *pDrawPos);

// �J�����̐U����ݒ�
void SetCameraVibration	(float fVibration);

#endif
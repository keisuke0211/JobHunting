//========================================
// 
// �J�����̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
// *** camera.h ***
//========================================
#ifndef _CAMERA_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "d3dx9.h"

//****************************************
// �񋓌^�̒�`
//****************************************
// �J�����̏��
typedef enum
{
	CAMERA_STATE_VIBRATION,	// �U��
	CAMERA_STATE_MAX,
}CAMERA_STATE;

//****************************************
// �\���̂̒�`
//****************************************
// �J����(2D)�̏��\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 posOld;				// ���̈ʒu
	D3DXVECTOR3 vibrationPos;		// �U���ʒu
	D3DXVECTOR3 vibrationTargetPos;	// �U���̖ڕW�ʒu
	D3DXVECTOR3 *pTargetPos;		// �ڕW�ʒu�̃|�C���^
	D3DXVECTOR3 targetRelativePos;	// �ڕW�ʒu�̃|�C���^

	// ��Ԋ֘A
	bool		aState				// ��ԃt���O
				[CAMERA_STATE_MAX];
	int			aCounterState		// ��ԊǗ��J�E���^�[
				[CAMERA_STATE_MAX];
	float		fVibration;			// �U���̋���
	bool		bTemp;				// �t���O�ۑ�
	bool		bMove;				// �ړ��t���O
	int			nCounterMove;		// �ړ��J�E���^�[
}Camera2D;

// �J����(3D)�̏��\����
typedef struct 
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 posU;			// ������x�N�g��
	D3DXVECTOR3 move;			// �ړ���
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 spin;			// ��]��
	float		fLength;		// ����
	float		fHeight;		// ����
	float		fVerticalMove;	// �c�����̈ړ���
	D3DXMATRIX	mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	mtxView;		// �r���[�}�g���b�N�X
	float		fVibration;		// �U���̋���
	int			nCounterVib;	// �U���J�E���^�[
	D3DXVECTOR3 vibrationPos;	// �U���ʒu
}Camera3D;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �J����(2D)�̏����擾
// �J����(3D)�̏����擾
Camera2D *GetCamera2D(void);
Camera3D *GetCamera3D(void);

// �J����(3D)�̃p�����[�^�[����������
void InitParameterCamera3D(void);

// �J�����̏���������
// �J�����̏I������
// �J�����̍X�V����
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

// �`��ʒu���J����(2D)�̈ʒu�ɍ��킹��
// �J����(2D)�̐U����ݒ�
void MatchCamera2DPosition(D3DXVECTOR3 *pDrawPos);
void SetCamera2DVibration(float fVibration);

// �J����(3D)�̐ݒ菈��
// �J����(3D)�̎��_�̐ݒ菈��
// �J����(3D)�̒����_�̐ݒ菈��
void SetCamera3D(void);
void SetCamera3DPosV(D3DXVECTOR3 pos);
void SetCamera3DPosR(D3DXVECTOR3 pos);

// �J����(3D)�̐U����ݒ�
void SetCamera3DVibration(float fVibration);

#endif
//========================================
// 
// �J�����̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** camera.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "input.h"
#include "physics.h"
#include "polygon2D.h"

//****************************************
// �}�N����`
//****************************************
// �J����(2D)�̐U���ɂ����鎞��
// �J����(2D)��1�x�̐U���ɂ����鎞��
// �J����(2D)�̐U���̌����{��
#define CAMERA2D_VIBRATION_TIME			(20)
#define CAMERA2D_ONCE_VIBRATION_TIME	(2)
#define CAMERA2D_VIBRATION_DAMPING		(0.75f)

// �J����(3D)��1�x�̐U���ɂ����鎞��
// �J����(3D)�̐U���̌����{��
// �J����(3D)�̐U���̋����̉���
#define CAMERA3D_ONCE_VIBRATION_TIME	(2)
#define CAMERA3D_VIBRATION_DAMPING		(0.75f)
#define CAMERA3D_VIBRATION_MIN			(0.01f)

// �J����(2D)�̐U�����R���g���[���[�̐U���̎��Ԃɕϊ�����ۂ̔{��
// �J����(2D)�̐U�����R���g���[���[�̐U���̍ő�l�ɒB����l
// �J����(3D)�̐U�����R���g���[���[�̐U���̎��Ԃɕϊ�����ۂ̔{��
// �J����(3D)�̐U�����R���g���[���[�̐U���̍ő�l�ɒB����l
#define CONTROLLER_CAMERA2D_VIBRATION_TIME_DIAMETER	(5)
#define CONTROLLER_CAMERA2D_VIBRATION_MAX_VIBRATION	(16.0f)
#define CONTROLLER_CAMERA3D_VIBRATION_TIME_DIAMETER	(5)
#define CONTROLLER_CAMERA3D_VIBRATION_MAX_VIBRATION	(4.0f)

// �J����(3D)�̏����̎��_
// �J����(3D)�̏����̒����_
// �J����(3D)�̏����̏�����x�N�g��
// �J����(3D)�̏����̋�������
// �J����(3D)�̏����̋���
// �J����(3D)�̏����̍���
// �J����(3D)�̏����̍����̏��
// �J����(3D)�̏����̍����̉���
#define CAMERA3D_INIT_POS_V			(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define CAMERA3D_INIT_POS_R			(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define CAMERA3D_INIT_POS_U			(D3DXVECTOR3(0.0f,1.0f,0.0f))
#define CAMERA3D_INIT_ROT			(D3DXVECTOR3(0.0f,0.0f,0.0f))
#define CAMERA3D_INIT_LENGTH		(80.0f)
#define CAMERA3D_INIT_HEIGHT		(0.25f)
#define CAMERA3D_INIT_HEIGHT_MAX	(0.99f)
#define CAMERA3D_INIT_HEIGHT_MIN	(0.0f)

// �J����(3D)�̉�]��
// �J����(3D)�̃J�[�\���̉�]��
// �J����(3D)�̃X�e�B�b�N�̉�]��
// �J����(3D)�̉�]�̌����l
#define CAMERA3D_ROT_FORCE				(D3DXVECTOR3(0.004f,-0.006f,0.0f))
#define CAMERA3D_ROT_FORCE_BY_CURSOR	(D3DXVECTOR3(0.0005f,0.00075f,0.0f))
#define CAMERA3D_ROT_FORCE_BY_STICK		(D3DXVECTOR3(0.004f,-0.006f,0.0f))
#define CAMERA3D_SPIN_DAMP				(0.9f)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �J����(2D)�̃p�����[�^�[����������
// �J����(2D)�̏�ԏ���
void InitParameterCamera2D(void);
void StateProcessCamera2D(void);

// �J����(2D)�̏���������
// �J����(2D)�̏I������
// �J����(2D)�̍X�V����
void InitCamera2D(void);
void UninitCamera2D(void);
void UpdateCamera2D(void);

// �J����(3D)�̏���������
// �J����(3D)�̏I������
// �J����(3D)�̍X�V����
void InitCamera3D(void);
void UninitCamera3D(void);
void UpdateCamera3D(void);

// �J����(3D)�̈ړ�����
// �J����(3D)�̎���]����
// �J����(3D)�̉�]����
void MoveCamera3D(DIRECTION drct, float fMove);
void AxisRotationCamera3D(DIRECTION drct, float fRot);
void RotationCamera3D(DIRECTION drct, float fRot);

//****************************************
// �O���[�o���ϐ�
//****************************************
Camera2D g_camera2D;	// �J����(2D)�̏��
Camera3D g_camera3D;	// �J����(3D)�̏��

//========== *** �J����2D�̏����擾 ***
Camera2D *GetCamera2D(void)
{
	return &g_camera2D;
}

//========== *** �J����3D�̏����擾 ***
Camera3D *GetCamera3D(void)
{
	return &g_camera3D;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitParameterCamera2D�֐� - �J����(2D)�̃p�����[�^�[���������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterCamera2D(void)
{
	g_camera2D.bTemp = false;	// �t���O�ۑ�
	g_camera2D.bMove = false;	// �ړ��t���O
}

//========================================
// StateProcessCamera2D�֐� - �J����(2D)�̏�ԏ��� -
// Author:RIKU NISHIMURA
//========================================
void StateProcessCamera2D(void)
{
	for (int nCntState = 0; nCntState < CAMERA_STATE_MAX; nCntState++)
	{
		if (g_camera2D.aState[nCntState])
		{// ���̏�Ԃ��^�̎��A
			switch (nCntState)
			{
				//========== *** �U�� ***
			case CAMERA_STATE_VIBRATION:

				if (g_camera2D.aCounterState[CAMERA_STATE_VIBRATION] < CAMERA2D_VIBRATION_TIME)
				{// �U���J�E���^�[���U���ɂ����鎞�Ԃ�������Ă��鎞�A
					// �U���J�E���^�[�����Z
					g_camera2D.aCounterState[CAMERA_STATE_VIBRATION]++;

					if ((g_camera2D.aCounterState[CAMERA_STATE_VIBRATION] % CAMERA2D_ONCE_VIBRATION_TIME) == 0)
					{// �U���J�E���^�[��1�x�̐U���ɂ����鎞�ԂɒB�������A
						float	fAngle	// �p�x�𗐐��Őݒ�
								= -D3DX_PI + fRand(D3DX_PI * 2);

						// �U���̖ڕW�ʒu��ݒ�
						g_camera2D.vibrationTargetPos.x = sinf(fAngle) * g_camera2D.fVibration;
						g_camera2D.vibrationTargetPos.y = cosf(fAngle) * g_camera2D.fVibration;

						// �U���̋���������������
						g_camera2D.fVibration *= CAMERA2D_VIBRATION_DAMPING;
					}
				}
				else
				{// �U���J�E���^�[���o���ɂ����鎞�ԂɒB�������A
					// �U���t���O���U�ɂ���
					g_camera2D.aState[CAMERA_STATE_VIBRATION] = false;
				}

				break;
			}
		}
	}
}

//========================================
// InitCamera2D�֐� - �J����(2D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitCamera2D(void)
{

}

//========================================
// UninitCamera2D�֐� - �J����(2D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitCamera2D(void)
{

}

//========================================
// UpdateCamera2D�֐� - �J����(2D)�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateCamera2D(void)
{
	
}

//========================================
// InitCamera3D�֐� - �J����(3D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitCamera3D(void)
{
	// �p�����[�^�[�̏���������
	InitParameterCamera3D();
}

//========================================
// UninitCamera3D�֐� - �J����(3D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitCamera3D(void)
{

}

//========================================
// UpdateCamera3D�֐� - �J����(3D)�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateCamera3D(void)
{
	if (g_camera3D.fVibration >= CAMERA3D_VIBRATION_MIN)
	{// �U���̋������U���̋����̉����ȏ�̎��A
		if (--g_camera3D.nCounterVib <= 0) 
		{// �U���J�E���^�[�����Z��������0�ȉ��̎��A
			g_camera3D.nCounterVib = CAMERA3D_ONCE_VIBRATION_TIME;	// �U���J�E���^�[��ݒ�
			float fAngle = -D3DX_PI + fRand(D3DX_PI * 2);			// �����𗐐��Őݒ�
			g_camera3D.vibrationPos.x = sinf(fAngle) * g_camera3D.fVibration;
			g_camera3D.vibrationPos.y = cosf(fAngle) * g_camera3D.fVibration;
			g_camera3D.fVibration *= CAMERA3D_VIBRATION_DAMPING;	// �U���̋���������
		}
	}
	else 
	{// �U���̋������U���̋����̉��������̎��A�U���ʒu��������
		g_camera3D.vibrationPos = INITD3DXVECTOR3;
	}
}

//========================================
// MoveCamera3D�֐� - �J����(3D)�̈ړ����� -
// Author:RIKU NISHIMURA
//========================================
void MoveCamera3D(DIRECTION drct, float fMove)
{
	// �����ɉ����Ď��_�ʒu���X�V
	switch (drct)
	{
	case DIRECTION_UP		:g_camera3D.posV.z += fMove; break;	// ��
	case DIRECTION_DOWN		:g_camera3D.posV.z -= fMove; break;	// ��
	case DIRECTION_LEFT		:g_camera3D.posV.x += fMove; break;	// ��
	case DIRECTION_RIGHT	:g_camera3D.posV.x -= fMove; break;	// �E
	}

	// �����_�̈ʒu��ݒ�
	g_camera3D.posR.x = g_camera3D.posV.x + (sinf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posR.y = g_camera3D.posV.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posR.z = g_camera3D.posV.z + (cosf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//========================================
// AxisRotationCamera3D�֐� - �J����(3D)�̎���]���� -
// Author:RIKU NISHIMURA
//========================================
void AxisRotationCamera3D(DIRECTION drct, float fRot) 
{
	// �����ɉ����Č����̈ړ��ʂ��X�V
	switch (drct)
	{
	case DIRECTION_UP		:g_camera3D.fVerticalMove	+= fRot; break;
	case DIRECTION_DOWN		:g_camera3D.fVerticalMove	-= fRot; break;
	case DIRECTION_LEFT		:g_camera3D.spin.y			-= fRot; break;
	case DIRECTION_RIGHT	:g_camera3D.spin.y			+= fRot; break;
	}
}

//========================================
// RotationCamera3D�֐� - �J����(3D)�̉�]���� -
// Author:RIKU NISHIMURA
//========================================
void RotationCamera3D(DIRECTION drct, float fRot)
{
	// �����ɉ����Č������X�V
	switch (drct)
	{
	case DIRECTION_UP		:g_camera3D.fHeight	+= fRot; break;
	case DIRECTION_DOWN		:g_camera3D.fHeight	-= fRot; break;
	case DIRECTION_LEFT		:g_camera3D.rot.y	-= fRot; break;
	case DIRECTION_RIGHT	:g_camera3D.rot.y	+= fRot; break;
	}

	// �����𐧌�
	RotControl(&g_camera3D.rot);

	// �����𐧌�
	FloatControl(&g_camera3D.fHeight, CAMERA3D_INIT_HEIGHT_MAX, CAMERA3D_INIT_HEIGHT_MIN);

	// �����_�̈ʒu��ݒ�
	g_camera3D.posR.x = g_camera3D.posV.x + (sinf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posR.y = g_camera3D.posV.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posR.z = g_camera3D.posV.z + (cosf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitCamera�֐� - �J�����̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitCamera(void)
{
	InitCamera2D();	// �J����(2D)�̏���������
	InitCamera3D();	// �J����(3D)�̏���������
}

//========================================
// UninitCamera�֐� - �J�����̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitCamera(void)
{
	UninitCamera2D();	// �J����(2D)�̏I������
	UninitCamera3D();	// �J����(3D)�̏I������
}

//========================================
// UpdateCamera�֐� - �J�����̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateCamera(void)
{
	UpdateCamera2D();	// �J����(2D)�̍X�V����
	UpdateCamera3D();	// �J����(3D)�̍X�V����
}

//========================================
// MatchCamera2DPosition�֐� - �`��ʒu���J����(2D)�ɍ��킹�� -
// Author:RIKU NISHIMURA
//========================================
void MatchCamera2DPosition(D3DXVECTOR3 *pDrawPos)
{
	// �`��ʒu���J�����ɍ��킹��
	MatchPosition(pDrawPos, g_camera2D.pos);

	// �`��ʒu�ɐU���ʒu���X�V����
	*pDrawPos += g_camera2D.vibrationPos;
}

//========================================
// SetCamera2DVibration�֐� - �J����(2D)�̐U����ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetCamera2DVibration(float fVibration)
{
	// ���[�U��]��^�ɂ���
	g_camera2D.aState[CAMERA_STATE_VIBRATION] = true;
	g_camera2D.aCounterState[CAMERA_STATE_VIBRATION] = 0;

	// �U���̋�����ݒ�
	g_camera2D.fVibration += fVibration;

	// �R���g���[���[�̐U����ݒ�
	SetVibration(
		fVibration / CONTROLLER_CAMERA2D_VIBRATION_MAX_VIBRATION, 
		fVibration * CONTROLLER_CAMERA2D_VIBRATION_TIME_DIAMETER);
}

//========================================
// InitParameterCamera3D�֐� - �J����(3D)�̃p�����[�^�[���������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterCamera3D(void)
{
	g_camera3D.posV				= CAMERA3D_INIT_POS_V;	// ���_
	g_camera3D.posR				= CAMERA3D_INIT_POS_R;	// �����_
	g_camera3D.posU				= CAMERA3D_INIT_POS_U;	// ������x�N�g��
	g_camera3D.move				= INITD3DXVECTOR3;		// �ړ���
	g_camera3D.rot				= CAMERA3D_INIT_ROT;	// ����
	g_camera3D.spin				= INITD3DXVECTOR3;		// ��]��
	g_camera3D.fLength			= CAMERA3D_INIT_LENGTH;	// ����
	g_camera3D.fHeight			= CAMERA3D_INIT_HEIGHT;	// ����
	g_camera3D.fVerticalMove	= 0.0f;					// �c�����̈ړ���
	g_camera3D.fVibration		= 0.0f;					// �U���̋���
	g_camera3D.nCounterVib		= 0;					// �U���J�E���^�[
	g_camera3D.vibrationPos		= INITD3DXVECTOR3;		// �U���ʒu
}

//========================================
// SetCamera3D�֐� - �J����(3D)�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetCamera3D(void) 
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏���������
	D3DXMatrixIdentity(&g_camera3D.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera3D.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 4000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera3D.mtxProjection);

	// �r���[�}�g���b�N�X�̐ݒ�
	D3DXMatrixIdentity(&g_camera3D.mtxView);

	{
		D3DXVECTOR3 posV = g_camera3D.posV;	// ���_
		D3DXVECTOR3 posR = g_camera3D.posR;	// �����_

		if (g_camera3D.vibrationPos != INITD3DXVECTOR3) 
		{// �U���ʒu�ɕϓ������鎞�A
			float fAngle = FindAngleLookDown(g_camera3D.posV, g_camera3D.posR) - (D3DX_PI * 0.5f);
			D3DXVECTOR3 addPos;	// ���Z�ʒu��U���ʒu�ɉ����Đݒ�
			addPos.x = sinf(fAngle) * g_camera3D.vibrationPos.x;
			addPos.z = cosf(fAngle) * g_camera3D.vibrationPos.x;
			addPos.y = g_camera3D.vibrationPos.y;

			// ���_/�����_�ɉ��Z�ʒu��K�p
			posV += addPos;
			posR += addPos;
		}
		
		// �r���[�}�g���b�N�X�̍쐬
		D3DXMatrixLookAtLH(&g_camera3D.mtxView, &posV, &posR, &g_camera3D.posU);
	}
	
	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera3D.mtxView);
}

//========================================
// SetCamera3DPosV�֐� - �J����(3D)�̎��_�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetCamera3DPosV(D3DXVECTOR3 pos) 
{
	// ���_�̈ʒu����
	g_camera3D.posV = pos;

	// �����_�̈ʒu��ݒ�
	g_camera3D.posR.x = g_camera3D.posV.x + (sinf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posR.y = g_camera3D.posV.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posR.z = g_camera3D.posV.z + (cosf(g_camera3D.rot.y) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//========================================
// SetCamera3DPosR�֐� - �J����(3D)�̒����_�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetCamera3DPosR(D3DXVECTOR3 pos)
{
	// �����_�̈ʒu����
	g_camera3D.posR = pos;

	// ���_�̈ʒu��ݒ�
	g_camera3D.posV.x = g_camera3D.posR.x + (sinf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
	g_camera3D.posV.y = g_camera3D.posR.y + (g_camera3D.fLength * g_camera3D.fHeight);
	g_camera3D.posV.z = g_camera3D.posR.z + (cosf(g_camera3D.rot.y + D3DX_PI) * (g_camera3D.fLength * (1.0f - fabsf(g_camera3D.fHeight))));
}

//========================================
// SetCamera3DVibration�֐� - �J����(3D)�̐U����ݒ� -
// Author:RIKU NISHIMURA
//========================================
void SetCamera3DVibration(float fVibration)
{
	// �U���̋�����ݒ�
	g_camera3D.fVibration += fVibration;

	// �U���J�E���^�[��������
	g_camera3D.nCounterVib = 0;

	// �R���g���[���[�̐U����ݒ�
	SetVibration(
		fVibration / CONTROLLER_CAMERA3D_VIBRATION_MAX_VIBRATION,
		fVibration * CONTROLLER_CAMERA3D_VIBRATION_TIME_DIAMETER);
}
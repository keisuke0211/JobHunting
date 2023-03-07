//========================================
// 
// �J�����̏���
// Author:���� ����
// 
//========================================
//  *** camera.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "input.h"

//****************************************
// �}�N����`
//****************************************

// �J�����̐U���ɂ����鎞��
// �J������1�x�̐U���ɂ����鎞��
// �J�����̐U���̌����{��
// �J�����̐U�����R���g���[���[�̐U���̎��Ԃɕϊ�����ۂ̔{��
// �J�����̐U�����R���g���[���[�̐U���̍ő�l�ɒB����l
#define CAMERA_VIBRATION_TIME				(20)
#define CAMERA_ONCE_VIBRATION_TIME			(2)
#define CAMERA_VIBRATION_DAMPING			(0.75f)
#define CONTROLLER_VIBRATION_TIME_DIAMETER	(5)
#define CONTROLLER_VIBRATION_MAX_VIBRATION	(16.0f)

//****************************************
// �v���g�^�C�v�錾
//****************************************
void StateProcessCamera(void);	// �J�����̏�ԏ���

//****************************************
// �O���[�o���ϐ�
//****************************************
Camera g_camera;	// �J�����̏��

//========== *** �J�����̏����擾 ***
Camera	*GetCamera(void) 
{
	return &g_camera;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StateProcessCamera�֐� - �J�����̏�ԏ��� -
//========================================
void StateProcessCamera(void)
{
	for (int nCntState = 0; nCntState < CAMERA_STATE_MAX; nCntState++)
	{
		if (g_camera.aState[nCntState])
		{// ���̏�Ԃ��^�̎��A
			switch (nCntState)
			{
				//========== *** �U�� ***
			case CAMERA_STATE_VIBRATION:

				if (g_camera.aCounterState[CAMERA_STATE_VIBRATION] < CAMERA_VIBRATION_TIME)
				{// �U���J�E���^�[���o���ɂ����鎞�Ԃ�������Ă��鎞�A
					// �U���J�E���^�[�����Z
					g_camera.aCounterState[CAMERA_STATE_VIBRATION]++;

					if ((g_camera.aCounterState[CAMERA_STATE_VIBRATION] % CAMERA_ONCE_VIBRATION_TIME) == 0)
					{// �U���J�E���^�[��1�x�̐U���ɂ����鎞�ԂɒB�������A
						float	fAngle	// �p�x�𗐐��Őݒ�
								= -D3DX_PI + fRand(D3DX_PI * 2);

						// �U���̖ڕW�ʒu��ݒ�
						g_camera.vibrationTargetPos.x = sinf(fAngle) * g_camera.fVibration;
						g_camera.vibrationTargetPos.y = cosf(fAngle) * g_camera.fVibration;

						// �U���̋���������������
						g_camera.fVibration *= CAMERA_VIBRATION_DAMPING;
					}
				}
				else
				{// �U���J�E���^�[���o���ɂ����鎞�ԂɒB�������A
					// �U���t���O���U�ɂ���
					g_camera.aState[CAMERA_STATE_VIBRATION] = false;
				}

				break;
			}
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitCamera�֐� - �J�����̏��������� -
//========================================
void InitCamera(void)
{
	
}

//========================================
// UninitCamera�֐� - �J�����̏I������ -
//========================================
void UninitCamera(void)
{
	
}

//========================================
// UpdateCamera�֐� - �J�����̍X�V���� -
//========================================
void UpdateCamera(void)
{
	// ��ԏ���
	StateProcessCamera();

	// �U���ʒu��ݒ�
	g_camera.vibrationPos = (g_camera.vibrationPos + g_camera.vibrationTargetPos) * 0.5f;
}

//========================================
// MatchCameraPosition�֐� - �`��ʒu���J�����ɍ��킹�� -
//========================================
void MatchCameraPosition(D3DXVECTOR3 *pDrawPos)
{
	// �`��ʒu���J�����ɍ��킹��
	MatchPosition(pDrawPos, g_camera.pos);

	// �`��ʒu�ɐU���ʒu���X�V����
	*pDrawPos += g_camera.vibrationPos;
}

//========================================
// SetCameraVibration�֐� - �J�����̐U����ݒ� -
//========================================
void SetCameraVibration(float fVibration)
{
	// ���[�U��]��^�ɂ���
	g_camera.aState[CAMERA_STATE_VIBRATION] = true;
	g_camera.aCounterState[CAMERA_STATE_VIBRATION] = 0;

	// �U���̋�����ݒ�
	g_camera.fVibration += fVibration;

	// �R���g���[���[�̐U����ݒ�
	SetVibration(
		fVibration / CONTROLLER_VIBRATION_MAX_VIBRATION, 
		fVibration * CONTROLLER_VIBRATION_TIME_DIAMETER);
}
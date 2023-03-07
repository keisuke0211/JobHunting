//========================================
// 
// ���̏���
// Author:RIKU NISHIMURA
// 
//========================================
//  *** light.cpp ***
//========================================
#include "chunk.h"
#include "color.h"
#include "main.h"
#include "physics.h"
#include "light.h"

//****************************************
// �}�N����`
//****************************************
// ��(2D)�̍ő吔
// ��(2D)�̖��邳�̍Œ�l
#define LIGHT2D_MAX				(64)
#define LIGHT2D_BRIGHTNESS_MIN	(25)

// ��(3D)�̍ő吔
#define LIGHT3D_MAX	(8)

//****************************************
// �\���̂̒�`
//****************************************
// ��(2D)�̗\����\����
typedef struct
{
	// �ʒu�֘A
	D3DXVECTOR3 pos;		// �ʒu
	float		fLength;	// ����

	// ��Ԋ֘A
	bool	bUse;		// �g�p����Ă��邩�t���O
	Color	col;		// �F
	float	fDensity;	// �Z��
}ReserveInfoLight2D;

// ��(3D)�̐ݒ���
typedef struct 
{
	// �ʒu�֘A
	D3DXVECTOR3	rot;			// ����
	float		fBrightness;	// ���邳
}Light3DSettingInfo;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ��(2D)�̏���������
// ��(2D)�̏I������
// ��(2D)�̍X�V����
void InitLight2D(void);
void UninitLight2D(void);
void UpdateLight2D(void);

// ��(2D)�̈ꊇ�ݒ菈��
void BulkSetLight2D(void);

// ��(3D)�̊Ǘ����̃p�����[�^�[����������
void InitParameterLight3D(void);

// ��(3D)�̏���������
// ��(3D)�̏I������
// ��(3D)�̍X�V����
void InitLight3D(void);
void UninitLight3D(void);
void UpdateLight3D(void);

//****************************************
// �O���[�o���ϐ�
//****************************************
Light2D				g_aLight2D				// ��(2D)�̏��
					[LIGHT2D_MAX];
ReserveInfoLight2D	g_aReserveInfoLight2D	// ��(2D)�̗\����
					[LIGHT2D_MAX];
D3DLIGHT9			g_light3D;				// ��(3D)�̏��
Light3DControl		g_light3DControl;		// ��(3D)�̊Ǘ����

// ��(3D)�̐ݒ���
const Light3DSettingInfo g_aLight3DSettingInfo[LIGHT3D_MAX]=
{
	{ D3DXVECTOR3( 0.0f , -0.5f , 0.5f),1.0f  },	// �O��
	{ D3DXVECTOR3(-0.5f , -0.5f , 0.5f),0.75f },	// ���O��
	{ D3DXVECTOR3( 0.5f , -0.5f , 0.5f),0.75f },	// �E�O��
	{ D3DXVECTOR3(-0.9f , -0.9f , 0.0f),0.5f  },	// ��
	{ D3DXVECTOR3( 0.9f , -0.9f , 0.0f),0.5f  },	// �E
	{ D3DXVECTOR3(-0.5f , -0.5f ,-0.5f),0.25f },	// ���㉺
	{ D3DXVECTOR3( 0.5f , -0.5f ,-0.5f),0.25f },	// �E�㉺
	{ D3DXVECTOR3( 0.0f , -0.5f ,-0.5f),0.25f },	// �㉺
};

//========== *** ��(2D)�̏����擾 ***
Light2D *GetLight2D(void) 
{
	return g_aLight2D;
}

//========== *** ��(3D)�̊Ǘ������擾 ***
Light3DControl *GetLight3DControl(void) 
{
	return &g_light3DControl;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitLight2D�֐� - ��(2D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitLight2D(void) 
{
	Light2D				*pSys			// ���̏��̃|�C���^
						= g_aLight2D;
	ReserveInfoLight2D	*pSysReserve	// ���̗\����̃|�C���^
						= g_aReserveInfoLight2D;

	for (int nCntSys = 0; nCntSys < LIGHT2D_MAX; nCntSys++, pSys++, pSysReserve++)
	{
		pSys->bUse			= false;	// �g�p����Ă��Ȃ���Ԃɂ���
		pSysReserve->bUse	= false;	// �\����
	}
}

//========================================
// UninitLight2D�֐� - ��(2D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitLight2D(void) 
{

}

//========================================
// UpdateLight2D�֐� - ��(2D)�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateLight2D(void)
{
	BulkSetLight2D();	// ���̈ꊇ�ݒ菈��
}

//========================================
// InitParameterLight3D�֐� - ��(3D)�̊Ǘ����̃p�����[�^�[���������� -
// Author:RIKU NISHIMURA
//========================================
void InitParameterLight3D(void)
{
	g_light3DControl.col			= INITCOLOR;	// �F
	g_light3DControl.fBrightness	= 1.0f;			// ���邳
}

//========================================
// InitLight3D�֐� - ��(3D)�̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitLight3D(void)
{
	// �Ǘ����̃p�����[�^�[����������
	InitParameterLight3D();

	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXVECTOR3			vecDir;	// �ݒ�p�����x�N�g��

	// ��(3D)�̏����N���A����
	ZeroMemory(&g_light3D, sizeof(D3DLIGHT9));

	// ��(3D)�̎�ނ�ݒ�
	g_light3D.Type = D3DLIGHT_DIRECTIONAL;

	for (int nCntLight3D = 0; nCntLight3D < LIGHT3D_MAX; nCntLight3D++)
	{
		// ��(3D)�̊g�U����ݒ�
		g_light3D.Diffuse = ColorToD3DXCOLOR(BrightnessToColor(g_light3DControl.col, g_aLight3DSettingInfo[nCntLight3D].fBrightness));

		// ��(3D)�̕�����ݒ�
		vecDir = g_aLight3DSettingInfo[nCntLight3D].rot;
		D3DXVec3Normalize(&vecDir, &vecDir);

		// �x�N�g���𐳋K������
		g_light3D.Direction = vecDir;

		// ��(3D)��ݒ肷��
		pDevice->SetLight(nCntLight3D, &g_light3D);

		// ��(3D)��L���ɂ���
		pDevice->LightEnable(nCntLight3D, TRUE);
	}
}

//========================================
// UninitLight3D�֐� - ��(3D)�̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitLight3D(void)
{

}

//========================================
// UpdateLight3D�֐� - ��(3D)�̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateLight3D(void)
{

}

//========================================
// BulkSetLight2D�֐� - ��(2D)�̈ꊇ�ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void BulkSetLight2D(void) 
{
	Light2D				*pSys			// ���̏��̃|�C���^
						= g_aLight2D;
	ReserveInfoLight2D	*pSysReserve	// ���̗\����̃|�C���^
						= g_aReserveInfoLight2D;

	for (int nCntSys = 0; nCntSys < LIGHT2D_MAX; nCntSys++, pSys++)
	{
		pSys->bUse = false;	// �g�p����Ă��Ȃ���Ԃɂ���

		if (!pSysReserve->bUse) 
		{// �J�E���g�̗\���񂪎g�p����Ă����ԂłȂ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		pSys->pos			= pSysReserve->pos;			// �ʒu����
		pSys->col			= pSysReserve->col;			// �F����
		pSys->fLength		= pSysReserve->fLength;		// ��������
		pSys->bUse			= true;						// �g�p����Ă����Ԃɂ���
		pSysReserve->bUse	= false;					// �\������g�p����Ă��Ȃ���Ԃɂ���
		pSysReserve++;									// �\����̃|�C���^��i�߂�
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitLight�֐� - ���̏��������� -
// Author:RIKU NISHIMURA
//========================================
void InitLight(void)
{
	InitLight2D();	// ��(2D)�̏���������
	InitLight3D();	// ��(3D)�̏���������
}

//========================================
// UninitLight�֐� - ���̏I������ -
// Author:RIKU NISHIMURA
//========================================
void UninitLight(void)
{
	UninitLight2D();	// ��(2D)�̏I������
	UninitLight3D();	// ��(3D)�̏I������
}

//========================================
// UpdateLight�֐� - ���̍X�V���� -
// Author:RIKU NISHIMURA
//========================================
void UpdateLight(void)
{
	UpdateLight2D();	// ��(2D)�̍X�V����
	UpdateLight3D();	// ��(3D)�̍X�V����
}

//========================================
// ReserveLight2D�֐� - ��(2D)�̗\�񏈗� -
// Author:RIKU NISHIMURA
//========================================
void ReserveLight2D(D3DXVECTOR3 pos, Color col, float fLength)
{
	ReserveInfoLight2D	*pSys	// ���̗\����̃|�C���^
						= g_aReserveInfoLight2D;

	for (int nCntSys = 0; nCntSys < LIGHT2D_MAX; nCntSys++, pSys++)
	{
		if (pSys->bUse) 
		{// �J�E���g�̗\���񂪎g�p����Ă����Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		pSys->pos		= pos;		// �ʒu����
		pSys->col		= col;		// �F����
		pSys->fLength	= fLength;	// ��������
		pSys->bUse		= true;		// �g�p����Ă����Ԃɂ���

		// �J��Ԃ������𔲂���
		break;
	}
}

//========================================
// GetColorForLight2D�֐� - ��(2D)�Ƃ̋�������F���擾 -
// Author:RIKU NISHIMURA
//========================================
Color GetColorForLight2D(D3DXVECTOR3 pos, Color myCol)
{
	Light2D	*pLight	// ���̏��̃|�C���^
			= g_aLight2D;
	Color	col		// �F
			= { LIGHT2D_BRIGHTNESS_MIN,LIGHT2D_BRIGHTNESS_MIN,LIGHT2D_BRIGHTNESS_MIN,255 };
	float	fBright	// ���邳
			= 1.0f;

	for (int nCntSys = 0; nCntSys < LIGHT2D_MAX; nCntSys++, pLight++)
	{
		float	fDistance	// ����
				= FindDistance(pos, pLight->pos);

		if ((pLight->bUse) && fDistance <= pLight->fLength)
		{// �g�p����Ă��邩�A���������̒��������̎��A
			// �F�������̊��������Z
			col.r += pLight->col.r * (1.0f - (fDistance / pLight->fLength)) * fBright;
			col.g += pLight->col.g * (1.0f - (fDistance / pLight->fLength)) * fBright;
			col.b += pLight->col.b * (1.0f - (fDistance / pLight->fLength)) * fBright;

			// �F�𐧌�
			IntControl(&col.r, 255, 0);
			IntControl(&col.g, 255, 0);
			IntControl(&col.b, 255, 0);
		}
	}

	// �����̐F��K�p
	col.r *= ((float)myCol.r / (float)(255));
	col.g *= ((float)myCol.g / (float)(255));
	col.b *= ((float)myCol.b / (float)(255));
	col.a *= ((float)myCol.a / (float)(255));
	
	// �F��Ԃ�
	return col;
}

//========================================
// SetBrightnessLight3D�֐� - ��(3D)�̖��邳�ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetBrightnessLight3D(float fBrightness)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXVECTOR3			vecDir;	// �ݒ�p�����x�N�g��

	// ��(3D)�̏����N���A����
	ZeroMemory(&g_light3D, sizeof(D3DLIGHT9));

	// ��(3D)�̎�ނ�ݒ�
	g_light3D.Type = D3DLIGHT_DIRECTIONAL;

	for (int nCntLight3D = 0; nCntLight3D < LIGHT3D_MAX; nCntLight3D++)
	{
		// ��(3D)�̊g�U����ݒ�
		g_light3D.Diffuse = ColorToD3DXCOLOR(BrightnessToColor(g_light3DControl.col, g_aLight3DSettingInfo[nCntLight3D].fBrightness)) * fBrightness;

		// ��(3D)�̕�����ݒ�
		vecDir = g_aLight3DSettingInfo[nCntLight3D].rot;
		D3DXVec3Normalize(&vecDir, &vecDir);

		// �x�N�g���𐳋K������
		g_light3D.Direction = vecDir;

		// ��(3D)��ݒ肷��
		pDevice->SetLight(nCntLight3D, &g_light3D);

		// ��(3D)��L���ɂ���
		pDevice->LightEnable(nCntLight3D, TRUE);
	}

	g_light3DControl.fBrightness = fBrightness;	// ���邳��ۑ�
}

//========================================
// SetColorLight3D�֐� - ��(3D)�̐F�ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetColorLight3D(Color col)
{
	LPDIRECT3DDEVICE9	pDevice	// �f�o�C�X�擾
						= GetDevice();
	D3DXVECTOR3			vecDir;	// �ݒ�p�����x�N�g��

	// ��(3D)�̏����N���A����
	ZeroMemory(&g_light3D, sizeof(D3DLIGHT9));

	// ��(3D)�̎�ނ�ݒ�
	g_light3D.Type = D3DLIGHT_DIRECTIONAL;

	// ��(3D)�̐F��ݒ�
	g_light3DControl.col = col;

	for (int nCntLight3D = 0; nCntLight3D < LIGHT3D_MAX; nCntLight3D++)
	{
		// ��(3D)�̊g�U����ݒ�
		g_light3D.Diffuse = ColorToD3DXCOLOR(BrightnessToColor(g_light3DControl.col, g_aLight3DSettingInfo[nCntLight3D].fBrightness)) * g_light3DControl.fBrightness;

		// ��(3D)�̕�����ݒ�
		vecDir = g_aLight3DSettingInfo[nCntLight3D].rot;
		D3DXVec3Normalize(&vecDir, &vecDir);

		// �x�N�g���𐳋K������
		g_light3D.Direction = vecDir;

		// ��(3D)��ݒ肷��
		pDevice->SetLight(nCntLight3D, &g_light3D);

		// ��(3D)��L���ɂ���
		pDevice->LightEnable(nCntLight3D, TRUE);
	}
}
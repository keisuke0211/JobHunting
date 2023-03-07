//========================================
// 
// �G[00] �̏���
// Author:���� ����
// 
//========================================
// *** chr_enemy_00.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "chunk.h"
#include "sound.h"
#include "physics.h"
#include "polygon.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "chr_enemy_00.h"
#include "eff_general_00.h"
#include "md_game_00.h"
#include "player.h"
#include "attack.h"
#include "fade.h"
#include "score.h"
#include "combo.h"
#include <stdio.h>
#include <float.h>

//****************************************
// �}�N����`
//****************************************
// �G[00] �̃_���[�W��Ԃ̎���
// �G[00] �̏o���ɂ����鎞��
// �G[00] �̏�����̂ɂ����鎞��
// �G[00] �̔����ϐ��ɂ����鎞��
// �G[00] �̖c��݂ɂ����鎞��
// �G[00] �̖c��݂ɂ�����{��
// �G[00] �̎�ޖ��̏��̃f�[�^�t�@�C���̑��΃p�X
// �G[00] �̋��O�i���̈ړ��ʂɂ�����{��
// �G[00] �̎�O�i���̈ړ��ʂɂ�����{��
// �G[00] �̃m�b�N�o�b�N��
// �G[00] �̏k����
#define CHR_ENEMY_00_DAMAGE_TIME					(20)
#define CHR_ENEMY_00_POP_TIME						(40)
#define CHR_ENEMY_00_CLEAR_TIME						(20)
#define CHR_ENEMY_00_EXPLOSION_RESISTANCE_TIME		(60)
#define CHR_ENEMY_00_SWELL_TIME						(8)
#define CHR_ENEMY_00_SWELL_DIAMETER					(1.4f)
#define CHR_ENEMY_00_TYPE_DATA_FILE_PATH			"data\\GAMEDATA\\CHR_ENEMY_00_TYPE_DATA.txt"
#define CHR_ENEMY_00_STRONG_MOVE_DIAMETER			(2.0f)
#define CHR_ENEMY_00_LIGHT_MOVE_DIAMETER			(0.1f)
#define CHR_ENEMY_00_KNOCK_BACK_POWER				(20.0f)
#define CHR_ENEMY_00_SCALE_DOWN						(0.01f)
#define CHR_ENEMY_00_SCORE_DIAMETER_ADDEND_CHAIN	(0.5f)

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �����񂩂�s����ǂݍ���
void StringLoadActLoad(char aString[TXT_MAX], CHR_ENEMY_00_ACT_LOAD *pActLoad);

// �p�����[�^�[�̏���������
void InitParameter(Chr_enemy_00 *pChr);

// �p�����[�^�[����ނɉ����Đݒ�
void SetParameter(Chr_enemy_00 *pChr);

// �F�̐ݒ菈��
void SetColor(VERTEX_2D *pVtx, Chr_enemy_00 *pChr);

// �s���ǂݍ��ݏ���
void ActLoading(Chr_enemy_00 *pChr);

// �s������
void ActProcess(Chr_enemy_00 *pChr);

// �T�E���h�ǂݍ��ݏ���
void SoundLoading(Chr_enemy_00 *pChr);

// �p�^�[��No.�ǂݍ��ݏ���
void PatternLoading(Chr_enemy_00 *pChr);

// �G�t�F�N�g�ǂݍ��ݏ���
void EffectLoading(Chr_enemy_00 *pChr);

// ���˓ǂݍ��ݏ���
void ShotLoading(Chr_enemy_00 *pChr);

// �����ǂݍ��ݏ���
void SummonLoading(Chr_enemy_00 *pChr);

// ��O��������
void SummonException(Chr_enemy_00 *pChr);

// ��������
void Summon(Chr_enemy_00 *pChr, int nCntSummon);

// ��ԏ���
void StateProcess(Chr_enemy_00 *pChr);

// �q�̈ړ�����
void ChildMoveProcess(Chr_enemy_00 *pChr, D3DXVECTOR3 posTemp, D3DXVECTOR3 rotTemp);

// �q�̔j�󏈗�
void ChildDestruction(Chr_enemy_00 *pChr);

// �q���_���[�W��Ԃɂ���
void ChildDamageState(Chr_enemy_00 *pChr);

// �@��[00] �Ƃ̓����蔻�菈��
void CollisionChr_fighter_00(Chr_enemy_00 *pChr);

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3DTEXTURE9		g_aTextureChr_enemy_00		// �e�N�X�`���ւ̃|�C���^
						[CHR_ENEMY_00_TYPE_MAX]		//
						= {};						//
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChr_enemy_00		// ���_�o�b�t�@�ւ̃|�C���^
						= NULL;						//
Chr_enemy_00			g_aChr_enemy_00				// �G[00] �̏��
						[CHR_ENEMY_00_MAX];			//
Chr_enemy_00Type		g_aChr_enemy_00Type			// �G[00] �̎�ޖ��̏��
						[CHR_ENEMY_00_TYPE_MAX];	//

// �G[00] �̍s���ǂݍ��݃e�L�X�g
char g_aActText[CHR_ENEMY_00_ACT_LOAD_MAX][TXT_MAX] =
{
	"�O�i�J�n",
	"�O�i�I��",
	"���O�i�J�n",
	"���O�i�I��",
	"��O�i�J�n",
	"��O�i�I��",
	"��i�J�n",
	"��i�I��",
	"����i�J�n",
	"����i�I��",
	"���i�J�n",
	"���i�I��",
	"���i",
	"���X���C�h�J�n",
	"���X���C�h�I��",
	"�E�X���C�h�J�n",
	"�E�X���C�h�I��",
	"�v���C���[�����]���J�n",
	"�v���C���[�����]���I��",
	"�v���C���[�ǔ��J�n",
	"�v���C���[�ǔ��I��",
	"�v���C���[�Î��J�n",
	"�v���C���[�Î��I��",
	"���S�Î��J�n",
	"���S�Î��I��",
	"���蔲���J�n",
	"���蔲���I��",
	"�e���蔲���J�n",
	"�e���蔲���I��",
	"���G�J�n",
	"���G�I��",
	"�m�b�N�o�b�N�J�n",
	"�m�b�N�o�b�N�I��",
	"�k���J�n",
	"�k���I��",
	"���S������",
	"�e����",
	"���o��",
	"��������",
	"������",
	"���[�v",
};

//========== *** �G[00] �̏����擾 ***
Chr_enemy_00 *GetChr_enemy_00(void)
{
	return g_aChr_enemy_00;
}

//========== *** �G[00] �̎�ޖ��̏����擾 ***
Chr_enemy_00Type *GetChr_enemy_00Type(void)
{
	return g_aChr_enemy_00Type;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// StringLoadActLoad�֐� - �����񂩂�s���̓ǂݍ��݂�ǂݍ��� -
//========================================
void StringLoadActLoad(char aString[TXT_MAX], CHR_ENEMY_00_ACT_LOAD *pActLoad)
{
	for (int nCntAct = 0; nCntAct < CHR_ENEMY_00_ACT_LOAD_MAX; nCntAct++)
	{
		if (strcmp(aString, g_aActText[nCntAct]) == 0)
		{// �s���̓ǂݍ��݃e�L�X�g�ƈ�v�������A
			// �s���̓ǂݍ��݂̎�ނ�ݒ�
			*pActLoad = (CHR_ENEMY_00_ACT_LOAD)nCntAct;
		}
	}
}

//========================================
// InitParameter�֐� - �p�����[�^�[�̏��������� -
//========================================
void InitParameter(Chr_enemy_00 *pChr) 
{
	pChr->pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	pChr->move					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
	pChr->fMove					= 0.0f;								// �ړ���
	pChr->rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	pChr->targetRot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ڕW����
	pChr->fWidth				= 0.0f;								// ��
	pChr->fHeight				= 0.0f;								// ����
	pChr->fHitTestWidth			= 0.0f;								// �����蔻��̕�
	pChr->fHitTestHeight		= 0.0f;								// �����蔻��̍���
	pChr->scale					= { 1.0f,1.0f };					// �傫���̔{��
	pChr->pTrackingPos			= NULL;								// �ǔ��ʒu
	pChr->pStarePos				= NULL;								// �^���ʒu
	pChr->nType					= 0;								// ���
	pChr->bUse					= false;							// �g�p����Ă��邩�t���O
	pChr->bBomb					= false;							// ���e�����t���O
	pChr->bReceivedExplosion	= false;							// �������󂯂��t���O
	pChr->nLife					= 0;								// �̗�

	for (int nCntState = 0; nCntState < CHR_ENEMY_00_STATE_MAX; nCntState++)
	{
		pChr->aState[nCntState]			= false;	// ��ԃt���O
		pChr->aCounterState[nCntState]	= 0;		// ��ԊǗ��J�E���^�[
	}

	pChr->nPattern		= 0;	// �p�^�[��No.
	pChr->nCounterAnim	= 0;	// �A�j���[�V�����̃J�E���^�[
	pChr->nCounterAct	= 0;	// �s���J�E���^�[

	for (int nCntAct = 0; nCntAct < CHR_ENEMY_00_ACT_MAX; nCntAct++) 
	{
		pChr->aAct[nCntAct] = false;	// �s���t���O
	}

	pChr->nCounterSound		= 0;	// �T�E���h�J�E���^�[
	pChr->nCounterPattern	= 0;	// �p�^�[��No.�J�E���^�[
	pChr->nCounterEffect	= 0;	// �G�t�F�N�g�J�E���^�[
	pChr->nCounterShot		= 0;	// ���˃J�E���^�[
	pChr->nCounterSummon	= 0;	// �����J�E���^�[
	pChr->nCntChain			= 0;	// �A���̃J�E���g
}

//========================================
// SetParameter�֐� - �p�����[�^�[����ނɉ����Đݒ� -
//========================================
void SetParameter(Chr_enemy_00 *pChr)
{
	pChr->fWidth			= g_aChr_enemy_00Type[pChr->nType].fWidth;			// ��
	pChr->fHeight			= g_aChr_enemy_00Type[pChr->nType].fHeight;			// ����
	pChr->fHitTestWidth		= g_aChr_enemy_00Type[pChr->nType].fHitTestWidth;	// �����蔻��̕�
	pChr->fHitTestHeight	= g_aChr_enemy_00Type[pChr->nType].fHitTestHeight;	// �����蔻��̍���
	pChr->fMove				= g_aChr_enemy_00Type[pChr->nType].fMove;			// �ړ���
	pChr->fRotation			= g_aChr_enemy_00Type[pChr->nType].fRotation;		// ��]��
	pChr->nLife				= g_aChr_enemy_00Type[pChr->nType].nLife;			// �̗�
}

//========================================
// SetColor�֐� - �F�̐ݒ菈�� -
//========================================
void SetColor(VERTEX_2D *pVtx, Chr_enemy_00 *pChr)
{
	float	fRed	// �s�ԓx
			= 1.0f;

	if (pChr->aState[CHR_ENEMY_00_STATE_DAMAGE])
	{// �_���[�W��Ԃ̎��A
		// �s�ԓx��ݒ�
		fRed = (float)pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] / (float)CHR_ENEMY_00_DAMAGE_TIME;
	}
	else if (pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
	{// ��������Ԃ̎��A
		// �s�ԓx��ݒ�
		fRed = (float)pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR] / (float)CHR_ENEMY_00_CLEAR_TIME;
	}

	// ���_�F�̐ݒ�
	SetVertexColor(pVtx,
	{
		(int)(255),
		(int)(255 * fRed),
		(int)(255 * fRed),
		(int)(255 * ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP] / (float)CHR_ENEMY_00_POP_TIME)
		* (1.0f - ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR] / (float)CHR_ENEMY_00_CLEAR_TIME)))
	});
}

//========================================
// ActLoading�֐� - �s���ǂݍ��ݏ��� -
//========================================
void ActLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT]))
	{// �c��ݏ��/�c��ݑ҂���Ԃ̂����ꂩ�̎��A
		// �������I������
		return;
	}

	for (int nCntAct = 0; nCntAct < g_aChr_enemy_00Type[pChr->nType].nActNum; nCntAct++)
	{
		if (pChr->nCounterAct == g_aChr_enemy_00Type[pChr->nType].aAct[nCntAct].nTime)
		{// �s���J�E���^�[�s�����鎞�Ԃƈ�v�������A
			switch (g_aChr_enemy_00Type[pChr->nType].aAct[nCntAct].type)
			{
				//========== *** �O�i�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_PROGRESS_START:

				// �O�i�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_PROGRESS] = true;

				break;
				//========== *** �O�i�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_PROGRESS_END:

				// �O�i�t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_PROGRESS] = false;

				break;
				//========== *** ���O�i�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_STRONG_PROGRESS_START:

				// ���O�i�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_STRONG_PROGRESS] = true;

				break;
				//========== *** ���O�i�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_STRONG_PROGRESS_END:

				// ���O�i�t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_STRONG_PROGRESS] = false;

				break;
				//========== *** ��O�i�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_LIGHT_PROGRESS_START:

				// ��O�i�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_LIGHT_PROGRESS] = true;

				break;
				//========== *** ��O�i�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_LIGHT_PROGRESS_END:

				// ��O�i�t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_LIGHT_PROGRESS] = false;

				break;
				//========== *** ��i�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_BACKWARD_START:

				// ��i�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_BACKWARD] = true;

				break;
				//========== *** ��i�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_BACKWARD_END:

				// ��i�t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_BACKWARD] = false;

				break;
				//========== *** ����i�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_STRONG_BACKWARD_START:

				// ����i�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_STRONG_BACKWARD] = true;

				break;
				//========== *** ����i�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_STRONG_BACKWARD_END:

				// ����i�t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_STRONG_BACKWARD] = false;

				break;
				//========== *** ���i�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_LIGHT_BACKWARD_START:

				// ���i�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_LIGHT_BACKWARD] = true;

				break;
				//========== *** ���i�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_LIGHT_BACKWARD_END:

				// ���i�t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_LIGHT_BACKWARD] = false;

				break;
				//========== *** ���i ***
			case CHR_ENEMY_00_ACT_LOAD_DRIVING:

				// �ړ��ʂ��X�V
				pChr->move.x += sinf(pChr->rot.z) * pChr->fMove;
				pChr->move.y += cosf(pChr->rot.z) * pChr->fMove;

				break;
				//========== *** ���X���C�h�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_SLIDE_LEFT_START:

				// ���X���C�h�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_SLIDE_LEFT] = true;

				break;
				//========== *** ���X���C�h�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_SLIDE_LEFT_END:

				// ���X���C�h�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_SLIDE_LEFT] = false;

				break;
				//========== *** �E�X���C�h�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_SLIDE_RIGHT_START:

				// �E�X���C�h�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_SLIDE_RIGHT] = true;

				break;
				//========== *** �E�X���C�h�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_SLIDE_RIGHT_END:

				// �E�X���C�h�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_SLIDE_RIGHT] = false;

				break;
				//========== *** �v���C���[�����]���J�n ***
			case CHR_ENEMY_00_ACT_LOAD_TURN_AROUND_TO_PLAYER_START:

				// �����]���ʒu��ݒ�
				pChr->pTurnAroundPos = &GetPlayer()->pos;

				// �����]���t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_TURN_AROUND] = true;

				break;
				//========== *** �v���C���[�����]���I�� ***
			case CHR_ENEMY_00_ACT_LOAD_TURN_AROUND_TO_PLAYER_END:

				// �����]���t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_TURN_AROUND] = false;

				break;
				//========== *** �v���C���[�ǔ��J�n ***
			case CHR_ENEMY_00_ACT_LOAD_TRACKING_PLAYER_START:

				// �ǔ��ʒu��ݒ�
				pChr->pTrackingPos = &GetPlayer()->pos;

				// �ǔ��t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_TRACKING] = true;

				break;
				//========== *** �v���C���[�ǔ��I�� ***
			case CHR_ENEMY_00_ACT_LOAD_TRACKING_PLAYER_END:

				// �ǔ��t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_TRACKING] = false;

				break;
				//========== *** �v���C���[�Î��J�n ***
			case CHR_ENEMY_00_ACT_LOAD_STARE_PLAYER_START:

				// �Î��ʒu��ݒ�
				// ��

				// �Î��t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_STARE] = true;

				break;
				//========== *** �v���C���[�Î��I�� ***
			case CHR_ENEMY_00_ACT_LOAD_STARE_PLAYER_END:

				// �Î��t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_STARE] = false;

				break;
				//========== *** ���S�Î��J�n ***
			case CHR_ENEMY_00_ACT_LOAD_STARE_CENTER_START:

				// �Î��ʒu��ݒ�
				// ��

				// �Î��t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_STARE] = true;

				break;
				//========== *** ���S�Î��I�� ***
			case CHR_ENEMY_00_ACT_LOAD_STARE_CENTER_END:

				// �Î��t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_STARE] = false;

				break;
				//========== *** ���蔲���J�n ***
			case CHR_ENEMY_00_ACT_LOAD_GHOST_START:

				// ���蔲���t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_GHOST] = true;

				break;
				//========== *** ���蔲���I�� ***
			case CHR_ENEMY_00_ACT_LOAD_GHOST_END:

				// ���蔲���t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_GHOST] = false;

				break;
				//========== *** �e���蔲���J�n ***
			case CHR_ENEMY_00_ACT_LOAD_BULLET_GHOST_START:

				// �e���蔲���t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_BULLET_GHOST] = true;

				break;
				//========== *** �e���蔲���I�� ***
			case CHR_ENEMY_00_ACT_LOAD_BULLET_GHOST_END:

				// �e���蔲���t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_BULLET_GHOST] = false;

				break;
				//========== *** ���G�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_INVINCIBLE_START:

				// ���G�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE] = true;

				break;
				//========== *** ���G�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_INVINCIBLE_END:

				// ���G�t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE] = false;

				break;
				//========== *** �m�b�N�o�b�N�J�n ***
			case CHR_ENEMY_00_ACT_LOAD_KNOCK_BACK_START:

				// �m�b�N�o�b�N�t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_KNOCK_BACK] = true;

				break;
				//========== *** �m�b�N�o�b�N�I�� ***
			case CHR_ENEMY_00_ACT_LOAD_KNOCK_BACK_END:

				// �m�b�N�o�b�N�t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_KNOCK_BACK] = false;

				break;
				//========== *** �k���J�n ***
			case CHR_ENEMY_00_ACT_LOAD_SCALE_DOWN_START:

				// �k���t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_SCALE_DOWN] = true;

				break;
				//========== *** �k���I�� ***
			case CHR_ENEMY_00_ACT_LOAD_SCALE_DOWN_END:

				// �k���t���O���U�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_SCALE_DOWN] = false;

				break;
				//========== *** ���S������ ***
			case CHR_ENEMY_00_ACT_LOAD_SUMMONING_ON_DIED:

				// ���S�������t���O��^�ɂ���
				pChr->aAct[CHR_ENEMY_00_ACT_SUMMONING_ON_DIED] = true;

				break;
				//========== *** �e���� ***
			case CHR_ENEMY_00_ACT_LOAD_CHILD_RELEASE:

				// �q�ԍ���-1�ɂ���
				pChr->nChildNum = -1;

				break;
				//========== *** ���o�� ***
			case CHR_ENEMY_00_ACT_LOAD_IMMEDIATE_POP:

				// �o�����ɂ���
				pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP] = CHR_ENEMY_00_POP_TIME;
				pChr->aState[CHR_ENEMY_00_STATE_IN_POP] = false;

				break;
				//========== *** �������� ***
			case CHR_ENEMY_00_ACT_LOAD_IMMEDIATE_CLEAR:

				// �g�p����Ă��Ȃ���Ԃɂ���
				pChr->bUse = false;

				break;
				//========== *** ������ ***
			case CHR_ENEMY_00_ACT_LOAD_CLEAR:

				if (!pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]) 
				{// �������łȂ����A
					// �������ɂ���
					pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]			= true;
					pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR]	= 0;
				}

				break;
				//========== *** ���[�v ***
			case CHR_ENEMY_00_ACT_LOAD_LOOP:

				// �s���̎��Ԃ�����������
				pChr->nCounterAct = -1;

				break;
			}
		}
	}

	// �s���J�E���^�[�����Z
	pChr->nCounterAct++;

	// �s������
	ActProcess(pChr);
}

//========================================
// ActProcess�֐� - �s������ -
//========================================
void ActProcess(Chr_enemy_00 *pChr)
{
	for (int nCntAct = 0; nCntAct < CHR_ENEMY_00_ACT_MAX; nCntAct++)
	{
		if (pChr->aAct[nCntAct])
		{// ���̏�Ԃ��^�̎�

			switch (nCntAct)
			{
				//========== *** �O�i ***
			case CHR_ENEMY_00_ACT_PROGRESS:

				// �ʒu���X�V
				pChr->pos.x += sinf(pChr->rot.z) * pChr->fMove;
				pChr->pos.y += cosf(pChr->rot.z) * pChr->fMove;

				break;
				//========== *** ���O�i ***
			case CHR_ENEMY_00_ACT_STRONG_PROGRESS:

				// �ʒu���X�V
				pChr->pos.x += sinf(pChr->rot.z) * pChr->fMove * CHR_ENEMY_00_STRONG_MOVE_DIAMETER;
				pChr->pos.y += cosf(pChr->rot.z) * pChr->fMove * CHR_ENEMY_00_STRONG_MOVE_DIAMETER;

				break;
				//========== *** ��O�i ***
			case CHR_ENEMY_00_ACT_LIGHT_PROGRESS:

				// �ʒu���X�V
				pChr->pos.x += sinf(pChr->rot.z) * pChr->fMove * CHR_ENEMY_00_LIGHT_MOVE_DIAMETER;
				pChr->pos.y += cosf(pChr->rot.z) * pChr->fMove * CHR_ENEMY_00_LIGHT_MOVE_DIAMETER;

				break;
				//========== *** ��i ***
			case CHR_ENEMY_00_ACT_BACKWARD:

				// �ʒu���X�V
				pChr->pos.x += sinf(pChr->rot.z + D3DX_PI) * pChr->fMove;
				pChr->pos.y += cosf(pChr->rot.z + D3DX_PI) * pChr->fMove;

				break;
				//========== *** ����i ***
			case CHR_ENEMY_00_ACT_STRONG_BACKWARD:

				// �ʒu���X�V
				pChr->pos.x += sinf(pChr->rot.z + D3DX_PI) * pChr->fMove * CHR_ENEMY_00_STRONG_MOVE_DIAMETER;
				pChr->pos.y += cosf(pChr->rot.z + D3DX_PI) * pChr->fMove * CHR_ENEMY_00_STRONG_MOVE_DIAMETER;

				break;
				//========== *** ���i ***
			case CHR_ENEMY_00_ACT_LIGHT_BACKWARD:

				// �ʒu���X�V
				pChr->pos.x += sinf(pChr->rot.z + D3DX_PI) * pChr->fMove * CHR_ENEMY_00_LIGHT_MOVE_DIAMETER;
				pChr->pos.y += cosf(pChr->rot.z + D3DX_PI) * pChr->fMove * CHR_ENEMY_00_LIGHT_MOVE_DIAMETER;

				break;
				//========== *** ���X���C�h ***
			case CHR_ENEMY_00_ACT_SLIDE_LEFT:

				// �ʒu���X�V
				pChr->pos.x += sinf(D3DX_PI * -0.5f) * pChr->fMove;
				pChr->pos.y += cosf(D3DX_PI * -0.5f) * pChr->fMove;

				break;
				//========== *** �E�X���C�h ***
			case CHR_ENEMY_00_ACT_SLIDE_RIGHT:

				// �ʒu���X�V
				pChr->pos.x += sinf(pChr->rot.z + (D3DX_PI * -0.5f)) * pChr->fMove;
				pChr->pos.y += cosf(pChr->rot.z + (D3DX_PI * -0.5f)) * pChr->fMove;

				break;
				//========== *** �����]�� ***
			case CHR_ENEMY_00_ACT_TURN_AROUND:

				// �����]������
				TurnAround(
					&pChr->rot,
					pChr->pos,
					*pChr->pTurnAroundPos,
					pChr->fRotation);

				break;
				//========== *** �ǔ� ***
			case CHR_ENEMY_00_ACT_TRACKING:

				// �ʒu���X�V
				pChr->pos.x += sinf(FindAngle(pChr->pos, *pChr->pTrackingPos)) * pChr->fMove;
				pChr->pos.y += cosf(FindAngle(pChr->pos, *pChr->pTrackingPos)) * pChr->fMove;

				break;
				//========== *** �Î� ***
			case CHR_ENEMY_00_ACT_STARE:

				// �������X�V
				pChr->rot.z = FindAngle(pChr->pos, *pChr->pStarePos);

				break;
				//========== *** �k�� ***
			case CHR_ENEMY_00_ACT_SCALE_DOWN:

				// ���ƍ����̔{�����X�V
				pChr->scale.fWidth	-= CHR_ENEMY_00_SCALE_DOWN;
				pChr->scale.fHeight	-= CHR_ENEMY_00_SCALE_DOWN;

				if ((pChr->scale.fWidth <= 0.0f)
					|| (pChr->scale.fHeight <= 0.0f)) 
				{// ����������0�ȉ��ɂȂ������A
					// �g�p���Ă��Ȃ���Ԃɂ���
					pChr->bUse = false;
				}

				break;
			}
		}
	}
}

//========================================
// SoundLoading�֐� - �T�E���h�ǂݍ��ݏ��� -
//========================================
void SoundLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		|| 
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// �c��ݏ��/�c��ݑ҂����/�������̂����ꂩ�̎��A
		// �������I������
		return;
	}

	for (int nCntSound = 0; nCntSound < g_aChr_enemy_00Type[pChr->nType].nSoundNum; nCntSound++)
	{
		if (pChr->nCounterSound == g_aChr_enemy_00Type[pChr->nType].aSound[nCntSound].nTime)
		{// �T�E���h�J�E���^�[���T�E���h��炷���Ԃƈ�v�������A
			// �T�E���h���Đ�
			PlaySound(g_aChr_enemy_00Type[pChr->nType].aSound[nCntSound].soundLabel);
		}
	}

	if ((pChr->nCounterSound >= g_aChr_enemy_00Type[pChr->nType].nSoundLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nSoundLoopTime != -1))
	{// �T�E���h�J�E���^�[���T�E���h�̃��[�v���ԂɒB�������A���T�E���h�̃��[�v���Ԃ�-1�łȂ����A
		// �T�E���h�J�E���^�[��������
		pChr->nCounterSound = -1;
	}

	// �T�E���h�J�E���^�[�����Z
	pChr->nCounterSound++;
}

//========================================
// PatternLoading�֐� - �p�^�[��No.�ǂݍ��ݏ��� -
//========================================
void PatternLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// �c��ݏ��/�c��ݑ҂����/�������̂����ꂩ�̎��A
		// �������I������
		return;
	}

	for (int nCntPattern = 0; nCntPattern < g_aChr_enemy_00Type[pChr->nType].nPatternNum; nCntPattern++)
	{
		if (pChr->nCounterPattern == g_aChr_enemy_00Type[pChr->nType].aPattern[nCntPattern].nTime)
		{// �p�^�[��No.�J�E���^�[���p�^�[��No.��ύX���鎞�Ԃƈ�v�������A
			// �p�^�[��No.���X�V
			pChr->nPattern = g_aChr_enemy_00Type[pChr->nType].aPattern[nCntPattern].nPattern;
		}
	}

	if ((pChr->nCounterPattern >= g_aChr_enemy_00Type[pChr->nType].nPatternLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nPatternLoopTime != -1))
	{// �p�^�[��No.�J�E���^�[���p�^�[��No.�̃��[�v���ԂɒB�������A���p�^�[��No.�̃��[�v���Ԃ�-1�łȂ����A
		// �p�^�[��No.�J�E���^�[��������
		pChr->nCounterPattern = -1;
	}

	// �p�^�[��No.�J�E���^�[�����Z
	pChr->nCounterPattern++;
}

//========================================
// EffectLoading�֐� - �G�t�F�N�g�ǂݍ��ݏ��� -
//========================================
void EffectLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// �c��ݏ��/�c��ݑ҂����/�������̂����ꂩ�̎��A
		// �������I������
		return;
	}

	for (int nCntEffect = 0; nCntEffect < g_aChr_enemy_00Type[pChr->nType].nEffectNum; nCntEffect++)
	{
		if (pChr->nCounterEffect == g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].nTime)
		{// �G�t�F�N�g�J�E���^�[���G�t�F�N�g��z�u���鎞�Ԃƈ�v�������A
			D3DXVECTOR3 setPos			//
						= pChr->pos;	// �ݒ�ʒu

			// �ݒ�ʒu��ݒ�
			setPos.x += sinf(pChr->rot.z - D3DX_PI * 0.5f)
				* g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].pos.x;
			setPos.y += cosf(pChr->rot.z - D3DX_PI * 0.5f)
				* g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].pos.x;
			setPos.x += sinf(pChr->rot.z + D3DX_PI)
				* g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].pos.y;
			setPos.y += cosf(pChr->rot.z + D3DX_PI)
				* g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].pos.y;

			// �G�t�F�N�g�S��[00] �̐ݒ菈��
			SetEff_general_00(
				setPos,
				D3DXVECTOR3(
					0.0f,
					0.0f,
					pChr->rot.z + g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].fAngle),
				g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].nEffectType,
				g_aChr_enemy_00Type[pChr->nType].aEffect[nCntEffect].bChild,
				&pChr->pos,
				&pChr->rot);
		}
	}

	if ((pChr->nCounterEffect >= g_aChr_enemy_00Type[pChr->nType].nEffectLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nEffectLoopTime != -1))
	{// �G�t�F�N�g�J�E���^�[���G�t�F�N�g�̃��[�v���ԂɒB�������A���G�t�F�N�g�̃��[�v���Ԃ�-1�łȂ����A
		// �G�t�F�N�g�J�E���^�[��������
		pChr->nCounterEffect = -1;
	}

	// �G�t�F�N�g�J�E���^�[�����Z
	pChr->nCounterEffect++;
}

//========================================
// ShotLoading�֐� - ���˓ǂݍ��ݏ��� -
//========================================
void ShotLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// �c��ݏ��/�c��ݑ҂����/�������̂����ꂩ�̎��A
		// �������I������
		return;
	}

	for (int nCntShot = 0; nCntShot < g_aChr_enemy_00Type[pChr->nType].nShotNum; nCntShot++)
	{
		if (pChr->nCounterShot == g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].nTime)
		{// ���˃J�E���^�[�����˂��鎞�Ԃƈ�v�������A
			D3DXVECTOR3 setPos	// �ݒ�ʒu
						= pChr->pos;

			// �ݒ�ʒu��ݒ�
			setPos.x	+= sinf(pChr->rot.z - D3DX_PI * 0.5f) 
						* g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].pos.x;
			setPos.y	+= cosf(pChr->rot.z - D3DX_PI * 0.5f)
						* g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].pos.x;
			setPos.x	+= sinf(pChr->rot.z + D3DX_PI)
						* g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].pos.y;
			setPos.y	+= cosf(pChr->rot.z + D3DX_PI)
						* g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].pos.y;

			// �e[00] �̐ݒ菈��
			SetAtk_bullet_00(
				setPos,
				D3DXVECTOR3(
					0.0f,
					0.0f,
					pChr->rot.z + g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].fAngle),
				g_aChr_enemy_00Type[pChr->nType].aShot[nCntShot].nBulletType,
				ATK_PARENT_TYPE_ENEMY);
		}
	}

	if ((pChr->nCounterShot >= g_aChr_enemy_00Type[pChr->nType].nShotLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nShotLoopTime != -1))
	{// ���˃J�E���^�[�����˂̃��[�v���ԂɒB�������A�����˂̃��[�v���Ԃ�-1�łȂ����A
		// ���˃J�E���^�[��������
		pChr->nCounterShot = -1;
	}

	// ���˃J�E���^�[�����Z
	pChr->nCounterShot++;
}

//========================================
// SummonLoading�֐� - �����ǂݍ��ݏ��� -
//========================================
void SummonLoading(Chr_enemy_00 *pChr)
{
	if ((pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// �c��ݏ��/�c��ݑ҂����/�������̂����ꂩ�̎��A
		// �������I������
		return;
	}

	for (int nCntSummon = 0; nCntSummon < g_aChr_enemy_00Type[pChr->nType].nSummonNum; nCntSummon++)
	{
		if (pChr->nCounterSummon == g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nTime)
		{// �����J�E���^�[���������鎞�Ԃƈ�v�������A
			// ��������
			Summon(pChr, nCntSummon);
		}
	}

	if ((pChr->nCounterSummon >= g_aChr_enemy_00Type[pChr->nType].nSummonLoopTime)
		&& (g_aChr_enemy_00Type[pChr->nType].nSummonLoopTime != -1))
	{// �����J�E���^�[�������̃��[�v���ԂɒB�������A�������̃��[�v���Ԃ�-1�łȂ����A
		// �����J�E���^�[��������
		pChr->nCounterSummon = -1;
	}

	// �����J�E���^�[�����Z
	pChr->nCounterSummon++;
}

//========================================
// SummonException�֐� - ��O�������� -
//========================================
void SummonException(Chr_enemy_00 *pChr) 
{
	for (int nCntSummon = 0; nCntSummon < g_aChr_enemy_00Type[pChr->nType].nSummonNum; nCntSummon++)
	{
		int n = 0;

		if (g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nTime == -1)
		{// �������鎞�Ԃ�-1�̎��A
			// ��������
			Summon(pChr, nCntSummon);
		}
	}
}

//========================================
// Summon�֐� - �������� -
//========================================
void Summon(Chr_enemy_00 *pChr, int nCntSummon) 
{
	D3DXVECTOR3 setPos	// �ݒ�ʒu
				= pChr->pos;

	// �ݒ�ʒu��ݒ�
	setPos.x += sinf(pChr->rot.z - D3DX_PI * 0.5f)
		* g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].pos.x;
	setPos.y += cosf(pChr->rot.z - D3DX_PI * 0.5f)
		* g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].pos.x;
	setPos.x += sinf(pChr->rot.z + D3DX_PI)
		* g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].pos.y;
	setPos.y += cosf(pChr->rot.z + D3DX_PI)
		* g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].pos.y;

	// �G[00] �̐ݒ菈��
	SetChr_enemy_00(
		setPos,
		D3DXVECTOR3(
			0.0f,
			0.0f,
			pChr->rot.z + g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].fAngle),
		g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nEnemyType,
		false, 
		false,
		g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nParentNum, 
		g_aChr_enemy_00Type[pChr->nType].aSummon[nCntSummon].nChildNum);
}

//========================================
// StateProcess�֐� - ��ԏ��� -
//========================================
void StateProcess(Chr_enemy_00 *pChr)
{
	for (int nCntState = 0; nCntState < CHR_ENEMY_00_STATE_MAX; nCntState++)
	{
		if (pChr->aState[nCntState])
		{// ���̏�Ԃ��^�̎�

			switch (nCntState)
			{
				//========== *** �o���� ***
			case CHR_ENEMY_00_STATE_IN_POP:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP] < CHR_ENEMY_00_POP_TIME)
				{// ��ԊǗ��J�E���^�[[�o����] ���o���ɂ����鎞�Ԃ�������Ă��鎞�A
					// ��ԊǗ��J�E���^�[[�o����] �����Z
					pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP]++;
				}
				else
				{// ��ԊǗ��J�E���^�[[�o����] ���o���ɂ����鎞�ԂɒB�������A
					// ���[�o����]���U�ɂ���
					pChr->aState[CHR_ENEMY_00_STATE_IN_POP] = false;
				}

				break;
				//========== *** ������ ***
			case CHR_ENEMY_00_STATE_IN_CLEAR:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR] < CHR_ENEMY_00_CLEAR_TIME)
				{// ��ԊǗ��J�E���^�[[������] ��������̂ɂ����鎞�Ԃ�������Ă��鎞�A
					// ��ԊǗ��J�E���^�[[������] �����Z
					pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR]++;
				}
				else
				{// ��ԊǗ��J�E���^�[[������] ��������̂ɂ����鎞�ԂɒB�������A
					// ���[������]���U�ɂ���
					pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR] = false;

					// �g�p���Ă��Ȃ���Ԃɂ���
					pChr->bUse = false;

					// �X�R�A�����Z
					*GetScore() += g_aChr_enemy_00Type[pChr->nType].nScore;
				}

				break;
				//========== *** �_���[�W ***
			case CHR_ENEMY_00_STATE_DAMAGE:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] < CHR_ENEMY_00_DAMAGE_TIME)
				{// ��ԊǗ��J�E���^�[[�_���[�W] �ɂ����鎞�Ԃ�������Ă��鎞�A
					// ��ԊǗ��J�E���^�[[�_���[�W] �����Z
					pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE]++;
				}
				else
				{// ��ԊǗ��J�E���^�[[�_���[�W] ��������̂ɂ����鎞�ԂɒB�������A
					// ���[�_���[�W]���U�ɂ���
					pChr->aState[CHR_ENEMY_00_STATE_DAMAGE] = false;
				}

				break;
				//========== *** �����ϐ� ***
			case CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE] < CHR_ENEMY_00_EXPLOSION_RESISTANCE_TIME)
				{// ��ԊǗ��J�E���^�[[�����ϐ�] �ɂ����鎞�Ԃ�������Ă��鎞�A
					// ��ԊǗ��J�E���^�[[�����ϐ�] �����Z
					pChr->aCounterState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE]++;
				}
				else
				{// ��ԊǗ��J�E���^�[[�����ϐ�] ��������̂ɂ����鎞�ԂɒB�������A
					// ���[�����ϐ�]���U�ɂ���
					pChr->aState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE] = false;
				}

				break;
				//========== *** �c��� ***
			case CHR_ENEMY_00_STATE_SWELL:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] < CHR_ENEMY_00_SWELL_TIME)
				{// ��ԊǗ��J�E���^�[[�c���] �ɂ����鎞�Ԃ�������Ă��鎞�A
					// ��ԊǗ��J�E���^�[[�c���] �����Z
					pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL]++;
				}
				else
				{// ��ԊǗ��J�E���^�[[�c���] ��������̂ɂ����鎞�ԂɒB�������A
					// ���[�c���]���U�ɂ���
					pChr->aState[CHR_ENEMY_00_STATE_SWELL] = false;

					// �G[00] �̔j�󏈗�
					DestructionChr_enemy_00(pChr);
				}

				break;
				//========== *** �c��ݑ҂� ***
			case CHR_ENEMY_00_STATE_SWELL_WAIT:

				if (pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL_WAIT] < pChr->nSwellWaitTime)
				{// ��ԊǗ��J�E���^�[[�c��ݑ҂�] �ɂ����鎞�Ԃ�������Ă��鎞�A
					// ��ԊǗ��J�E���^�[[�c��ݑ҂�] �����Z
					pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL_WAIT]++;
				}
				else
				{// ��ԊǗ��J�E���^�[[�c��ݑ҂�] ��������̂ɂ����鎞�ԂɒB�������A
					// ���[�c��ݑ҂�]���U�ɂ���
					pChr->aState[CHR_ENEMY_00_STATE_SWELL_WAIT] = false;

					// �c��ݏ�Ԃɂ���
					pChr->aState[CHR_ENEMY_00_STATE_SWELL] = true;
					pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] = 0;
				}

				break;
			}
		}
	}
}

//========================================
// ChildMoveProcess�֐�- �q�̈ړ����� -
//========================================
void ChildMoveProcess(Chr_enemy_00 *pChr, D3DXVECTOR3 posTemp, D3DXVECTOR3 rotTemp)
{
	if (pChr->nParentNum == -1) 
	{// �e�ԍ���-1�̎��A
		// �������I������
		return;
	}

	D3DXVECTOR3		move					// �ړ���
					= pChr->pos - posTemp;	//
	Chr_enemy_00	*pChildChr				// �G[00] �̏��̃|�C���^(�q)
					= g_aChr_enemy_00;		//

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((pChildChr->bUse)
			&& (pChr->nParentNum == pChildChr->nChildNum))
		{// �g�p����Ă��āA���e�ԍ��Ǝq�ԍ�����v���Ă��鎞�A
			// �ʒu���X�V
			pChildChr->pos += move;
			
			D3DXVECTOR3 rot;										// �e����q�ւ̌���
			float		fDistance									// 
						= FindDistance(pChr->pos, pChildChr->pos);	// �e�Ǝq�̋���

			// �e����q�ւ̌�����ݒ�
			rot.z = FindAngle(pChr->pos, pChildChr->pos) + (pChr->rot.z - rotTemp.z);		

			// �q�Ɍ����̍�����K�p
			pChildChr->rot.z += (pChr->rot.z - rotTemp.z);

			// �ʒu��e�̏��ɖ߂�����A
			// �������p�x�ɋ��������Z����
			pChildChr->pos = pChr->pos;
			pChildChr->pos.x += sinf(rot.z) * fDistance;
			pChildChr->pos.y += cosf(rot.z) * fDistance;
		}
	}
}

//========================================
// ChildDestruction�֐� - �q�̔j�󏈗� -
//========================================
void ChildDestruction(Chr_enemy_00 *pChr) 
{
	if (pChr->nParentNum == -1)
	{// �e�ԍ���-1�̎��A
		// �������I������
		return;
	}

	Chr_enemy_00	*pChildChr			// �G[00] �̏��̃|�C���^(�q)
					= g_aChr_enemy_00;	//

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((pChildChr->bUse)
			&& (pChr->nParentNum == pChildChr->nChildNum))
		{// �g�p����Ă��āA���e�ԍ��Ǝq�ԍ�����v���Ă��鎞�A
			// �A���̃J�E���g�����Z
			pChildChr->nCntChain++;

			// �j�󏈗�
			DestructionChr_enemy_00(pChildChr);
		}
	}
}

//========================================
// ChildDamageState�֐� - �q���_���[�W��Ԃɂ��� -
//========================================
void ChildDamageState(Chr_enemy_00 *pChr)
{
	if (pChr->nParentNum == -1)
	{// �e�ԍ���-1�̎��A
		// �������I������
		return;
	}

	Chr_enemy_00	*pChildChr			// 
					= g_aChr_enemy_00;	// �G[00] �̏��̃|�C���^(�q)

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChildChr++)
	{
		if ((pChildChr->bUse)
			&& (pChr->nParentNum == pChildChr->nChildNum))
		{// �g�p����Ă��āA���e�ԍ��Ǝq�ԍ�����v���Ă��鎞�A
			// ���_���[�W��Ԃɂ���
			pChildChr->aState[CHR_ENEMY_00_STATE_DAMAGE] = true;
			pChildChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] = CHR_ENEMY_00_DAMAGE_TIME * 0.5f;
		}
	}
}

//========================================
// CollisionChr_fighter_00�֐� - �@��[00]�Ƃ̓����蔻�菈�� -
//========================================
void CollisionChr_fighter_00(Chr_enemy_00 *pChr)
{
	if (!GetPlayer()->bDisp
		||
		GetPlayer()->bTurn
		||
		GetPlayer()->nDamageCounter > 0)
	{// �@��[00] ���g�p����Ă��Ȃ���Ԃ����G��ԁA�������͓G[00] ���o�������������̎��A
		// �������I������
		return;
	}

	if (CheckHit(
		pChr->pos,
		pChr->rot,
		pChr->fHitTestWidth * pChr->scale.fWidth,
		pChr->fHitTestHeight * pChr->scale.fHeight,
		HIT_TEST_TYPE_CIRCLE,
		GetPlayer()->pos,
		GetPlayer()->rot,
		PLAYER_HIT_TEST_RADIUS,
		0.0f,
		HIT_TEST_TYPE_CIRCLE))
	{// �G[00] �̍��W���@��[00] �̓����蔻����ɓ��������A

		// �v���C���[�̃_���[�W�J�E���^�[��������
		GetPlayer()->nDamageCounter = DAMAGE_TIME;

		PlaySound(SOUND_LABEL_SE_DAMAGE_000);

		if (--GetPlayer()->nLife <= 0) 
		{// ���Z�����̗͂��s�������A
			// ����ł���
			GetPlayer()->bDisp = false;

			SetFade(MODE_RESULT_00);
		}
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// LoadChr_enemy_00�֐� - �G[00] �̓ǂݍ��ݏ��� -
//========================================
void LoadChr_enemy_00(void)
{
	FILE *pFile;				// �t�@�C���|�C���^
	char aDataSearch[TXT_MAX];	// �f�[�^�����p

	Chr_enemy_00Type	*pChrType	// �G[00] �̎�ޖ��̏��
						= g_aChr_enemy_00Type;

	// ��ޖ��̏��̃f�[�^�t�@�C�����J��
	pFile = fopen(CHR_ENEMY_00_TYPE_DATA_FILE_PATH, "r");

	if (pFile == NULL)
	{// ��ޖ��̏��̃f�[�^�t�@�C�����J���Ȃ������ꍇ�A
		//�������I������
		return;
	}

	// �������ʎq��������܂œǂݍ��݂��J��Ԃ�
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// ����

		if (strcmp(aDataSearch, "END") == 0)
		{// �t�@�C�������
			fclose(pFile);
			break;
		}

		if (aDataSearch[0] == '#')
		{// �R�����g��ƈ�v�������A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (strcmp(aDataSearch, "CHR_ENEMY_00_TYPE") == 0)
		{// �G[00] �̎�ޖ��̏��̓ǂݍ��݂��J�n

			// �s����������������
			pChrType->nActNum = 0;

			// ���ː�������������
			pChrType->nShotNum = 0;

			// ������������������
			pChrType->nSummonNum = 0;

			while (1)
			{
				fscanf(pFile, "%s", aDataSearch); // ����

				if (strcmp(aDataSearch, "CHR_ENEMY_00_TYPE_END") == 0)
				{// �G[00] �̎�ޖ��̏��̓ǂݍ��݂��I��
					pChrType++;
					break;
				}//====================//
				else if (strcmp(aDataSearch, "�ǂݍ��ݖ�:") == 0)
				{// �ǂݍ��ݖ���ǂݍ���
					fscanf(pFile, "%s", pChrType->aLoadName);
				}//====================//
				else if (strcmp(aDataSearch, "�e�N�X�`���̑��΃p�X:") == 0)
				{// �e�N�X�`���̑��΃p�X��ǂݍ���
					fscanf(pFile, "%s", pChrType->aTexturPath);
				}//====================//
				else if (strcmp(aDataSearch, "�p�^�[���̏��X:") == 0)
				{// �p�^�[�����X��ǂݍ���
					fscanf(pFile, "%d", &pChrType->nPtnMaxX);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���p�^�[���̉���:") == 0)
				{// �A�j���p�^�[���̉�����ǂݍ���
					fscanf(pFile, "%d", &pChrType->nAnimPtnMin);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���p�^�[���̏��:") == 0)
				{// �A�j���p�^�[���̏����ǂݍ���
					fscanf(pFile, "%d", &pChrType->nAnimPtnMax);
				}//====================//
				else if (strcmp(aDataSearch, "�p�^�[���̏��Y:") == 0)
				{// �p�^�[�����Y��ǂݍ���
					fscanf(pFile, "%d", &pChrType->nPtnMaxY);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���[�V�����̎���:") == 0)
				{// �A�j���[�V�����ɂ����鎞�Ԃ�ǂݍ���
					fscanf(pFile, "%d", &pChrType->nAnimTime);
				}//====================//
				else if (strcmp(aDataSearch, "�A�j���[�V�����̎��:") == 0)
				{// �A�j���[�V�����̎�ނ�ǂݍ���
					fscanf(pFile, "%s", aDataSearch); // ����

					// �A�j���[�V�����̎�ނ𕶎��񂩂�ǂݍ���
					StringLoadAnimType(aDataSearch, &pChrType->animType);
				}//====================//
				else if (strcmp(aDataSearch, "��:") == 0)
				{// ����ǂݍ���
					fscanf(pFile, "%f", &pChrType->fWidth);

					// �����s�N�Z���P�ʂŏC��

					pChrType->fWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "����:") == 0)
				{// ������ǂݍ���
					fscanf(pFile, "%f", &pChrType->fHeight);

					// �������s�N�Z���P�ʂŏC��
					pChrType->fHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "�����̎��:") == 0)
				{// �����̎�ނ�ǂݍ���
					fscanf(pFile, "%s", aDataSearch); // ����

					// �����̎�ނ𕶎��񂩂�ǂݍ���
					StringLoadAngleType(aDataSearch, &pChrType->angleType);
				}//====================//
				else if (strcmp(aDataSearch, "�����蔻��̕�:") == 0)
				{// �����蔻��̕���ǂݍ���
					fscanf(pFile, "%f", &pChrType->fHitTestWidth);

					// �����蔻����s�N�Z���P�ʂŏC��
					pChrType->fHitTestWidth *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "�����蔻��̍���:") == 0)
				{// �����蔻��̕���ǂݍ���
					fscanf(pFile, "%f", &pChrType->fHitTestHeight);

					// �����蔻����s�N�Z���P�ʂŏC��
					pChrType->fHitTestHeight *= PIXEL;
				}//====================//
				else if (strcmp(aDataSearch, "�̗�:") == 0)
				{// �̗͂�ǂݍ���
					fscanf(pFile, "%d", &pChrType->nLife);
				}//====================//
				else if (strcmp(aDataSearch, "�ړ���:") == 0)
				{// �ړ��͂�ǂݍ���
					fscanf(pFile, "%f", &pChrType->fMove);
				}//====================//
				else if (strcmp(aDataSearch, "��]��:") == 0)
				{// ��]�͂�ǂݍ���
					fscanf(pFile, "%f", &pChrType->fRotation);
				}//====================//
				else if (strcmp(aDataSearch, "�Փ˃_���[�W:") == 0)
				{// �Փ˃_���[�W��ǂݍ���
					fscanf(pFile, "%d", &pChrType->nHitDamage);
				}//====================//
				else if (strcmp(aDataSearch, "�Փˎ��̔j��:") == 0)
				{// �Փˎ��̔j�󏈗���ǂݍ���
					fscanf(pFile, "%d", &pChrType->bHitBreak);
				}//====================//
				else if (strcmp(aDataSearch, "�����^�C�v:") == 0)
				{// �����̎�ނ�ǂݍ���
					fscanf(pFile, "%d", &pChrType->explosionType);
				}//====================//
				else if (strcmp(aDataSearch, "�_���[�W�T�E���h:") == 0)
				{// �_���[�W�T�E���h�̎�ނ�ǂݍ���
					fscanf(pFile, "%s", aDataSearch); // ����

					// �_���[�W�T�E���h�̎�ނ𕶎��񂩂�ǂݍ���
					StringLoadSound(aDataSearch, &pChrType->damageSound);
				}//====================//
				else if (strcmp(aDataSearch, "�{�[�i�X�T�E���h:") == 0)
				{// �{�[�i�X�T�E���h�t���O��ǂݍ���
					fscanf(pFile, "%d", &pChrType->bBonusSound);
				}//====================//
				else if (strcmp(aDataSearch, "�X�R�A:") == 0)
				{// �e�̔��˂ɂ����鎞�Ԃ�ǂݍ���
					fscanf(pFile, "%d", &pChrType->nScore);
				}//====================//
				else if (strcmp(aDataSearch, "ACT") == 0)
				{// �s���̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "ACT_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}
						else if (strcmp(aDataSearch, "�s��:") == 0)
						{// �s�����鎞�Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->aAct[pChrType->nActNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // ����

							// �����񂩂�s���̓ǂݍ��݂�ǂݍ���
							StringLoadActLoad(aDataSearch, &pChrType->aAct[pChrType->nActNum].type);

							// �s���������Z
							pChrType->nActNum++;
						}
					}
				}//====================//
				else if (strcmp(aDataSearch, "SHOT") == 0)
				{// ���˂̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "SHOT_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}//====================//
						else if (strcmp(aDataSearch, "���[�v����:") == 0) 
						{// ���˂̃��[�v���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->nShotLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "����:") == 0)
						{// ���˂��鎞�Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->aShot[pChrType->nShotNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // ����

							if (strcmp(aDataSearch, "[") == 0) 
							{
								while (1)
								{
									fscanf(pFile, "%s", aDataSearch); // ����

									if (strcmp(aDataSearch, "]") == 0)
									{// �J��Ԃ������𔲂���
										break;
									}//====================//
									else if (strcmp(aDataSearch, "���:") == 0) 
									{// �e�̎�ނ�ǂݍ���
										fscanf(pFile, "%s", aDataSearch); // ����

										// �e[00] �̎�ނ𕶎��񂩂�ǂݍ���
										StringLoadAtk_bullet_00(aDataSearch, &pChrType->aShot[pChrType->nShotNum].nBulletType);
									}//====================//
									else if (strcmp(aDataSearch, "X:") == 0)
									{// ���˂���ʒuX��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aShot[pChrType->nShotNum].pos.x);
									}//====================//
									else if (strcmp(aDataSearch, "Y:") == 0)
									{// ���˂���ʒuY��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aShot[pChrType->nShotNum].pos.y);
									}//====================//
									else if (strcmp(aDataSearch, "�p�x:") == 0)
									{// ���˂���p�x��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aShot[pChrType->nShotNum].fAngle);

										// �p�x�𐳂����l�ɕϊ�
										pChrType->aShot[pChrType->nShotNum].fAngle *= D3DX_PI;
									}//====================//
								}
							}

							// ���ː������Z
							pChrType->nShotNum++;
						}
					}
				}//====================//
				else if (strcmp(aDataSearch, "SOUND") == 0)
				{// �T�E���h�̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "SOUND_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}//====================//
						else if (strcmp(aDataSearch, "���[�v����:") == 0)
						{// �T�E���h�̃��[�v���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->nSoundLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "�T�E���h:") == 0)
						{// �T�E���h��炷���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->aSound[pChrType->nSoundNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // ����

							// �T�E���h�̎�ނ𕶎��񂩂�ǂݍ���
							StringLoadSound(aDataSearch, &pChrType->aSound[pChrType->nSoundNum].soundLabel);

							// �T�E���h�������Z
							pChrType->nSoundNum++;
						}//====================//
					}
				}//====================//
				else if (strcmp(aDataSearch, "PATTERN") == 0)
				{// �p�^�[��No.�̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "PATTERN_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}//====================//
						else if (strcmp(aDataSearch, "���[�v����:") == 0)
						{// �p�^�[��No.�̃��[�v���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->nPatternLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "�p�^�[��:") == 0)
						{// �p�^�[��No.��ύX���鎞�Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->aPattern[pChrType->nPatternNum].nTime);

							// �p�^�[��No.��ǂݍ���
							fscanf(pFile, "%d", &pChrType->aPattern[pChrType->nPatternNum].nPattern);

							// �p�^�[��No.�������Z
							pChrType->nPatternNum++;
						}//====================//
					}
				}//====================//
				else if (strcmp(aDataSearch, "EFFECT") == 0)
				{// �G�t�F�N�g�̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "EFFECT_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}//====================//
						else if (strcmp(aDataSearch, "���[�v����:") == 0)
						{// �G�t�F�N�g�̃��[�v���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->nEffectLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "�G�t�F�N�g:") == 0)
						{// �G�t�F�N�g���鎞�Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->aEffect[pChrType->nEffectNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // ����

							if (strcmp(aDataSearch, "[") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", aDataSearch); // ����

									if (strcmp(aDataSearch, "]") == 0)
									{// �J��Ԃ������𔲂���
										break;
									}//====================//
									else if (strcmp(aDataSearch, "���:") == 0)
									{// �e�̎�ނ�ǂݍ���
										fscanf(pFile, "%s", aDataSearch); // ����

										// �G�t�F�N�g�S��[00] �̎�ނ𕶎��񂩂�ǂݍ���
										StringLoadEff_general_00Type(aDataSearch, &pChrType->aEffect[pChrType->nEffectNum].nEffectType);
									}//====================//
									else if (strcmp(aDataSearch, "X:") == 0)
									{// �G�t�F�N�g��z�u����ʒuX��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aEffect[pChrType->nEffectNum].pos.x);
									}//====================//
									else if (strcmp(aDataSearch, "Y:") == 0)
									{// �G�t�F�N�g��z�u����ʒuY��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aEffect[pChrType->nEffectNum].pos.y);
									}//====================//
									else if (strcmp(aDataSearch, "�p�x:") == 0)
									{// �G�t�F�N�g��z�u����p�x��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aEffect[pChrType->nEffectNum].fAngle);

										// �p�x�𐳂����l�ɕϊ�
										pChrType->aEffect[pChrType->nEffectNum].fAngle *= D3DX_PI;
									}//====================//
									else if (strcmp(aDataSearch, "�e�q�֌W:") == 0)
									{// �q�t���O��ǂݍ���
										fscanf(pFile, "%d", &pChrType->aEffect[pChrType->nEffectNum].bChild);
									}//====================//
								}
							}

							// �G�t�F�N�g�������Z
							pChrType->nEffectNum++;
						}
					}
				}//====================//
				else if (strcmp(aDataSearch, "SUMMON") == 0)
				{// ���˂̏��̓ǂݍ��݂��J�n����
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch); // ����

						if (strcmp(aDataSearch, "SUMMON_END") == 0)
						{// �J��Ԃ������𔲂���
							break;
						}//====================//
						else if (strcmp(aDataSearch, "���[�v����:") == 0)
						{// �����̃��[�v���Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->nSummonLoopTime);
						}//====================//
						else if (strcmp(aDataSearch, "����:") == 0)
						{// �������鎞�Ԃ�ǂݍ���
							fscanf(pFile, "%d", &pChrType->aSummon[pChrType->nSummonNum].nTime);

							fscanf(pFile, "%s", aDataSearch); // ����

							if (strcmp(aDataSearch, "[") == 0)
							{
								// ��������e�ԍ��Ǝq�ԍ���������
								pChrType->aSummon[pChrType->nSummonNum].nParentNum	= -1;
								pChrType->aSummon[pChrType->nSummonNum].nChildNum	= -1;

								while (1)
								{
									fscanf(pFile, "%s", aDataSearch); // ����

									if (strcmp(aDataSearch, "]") == 0)
									{// �J��Ԃ������𔲂���
										break;
									}//====================//
									else if (strcmp(aDataSearch, "���:") == 0)
									{// �G�̎�ނ�ǂݍ���
										fscanf(pFile, "%s", aDataSearch); // ����

										// �G�̎�ނ𕶎��񂩂�ǂݍ���
										StringLoadChr_enemy_00Type(aDataSearch, &pChrType->aSummon[pChrType->nSummonNum].nEnemyType);
									}//====================//
									else if (strcmp(aDataSearch, "X:") == 0)
									{// ��������ʒuX��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aSummon[pChrType->nSummonNum].pos.x);
									}//====================//
									else if (strcmp(aDataSearch, "Y:") == 0)
									{// ��������ʒuY��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aSummon[pChrType->nSummonNum].pos.y);
									}//====================//
									else if (strcmp(aDataSearch, "�p�x:") == 0)
									{// ��������p�x��ǂݍ���
										fscanf(pFile, "%f", &pChrType->aSummon[pChrType->nSummonNum].fAngle);

										// �p�x�𐳂����l�ɕϊ�
										pChrType->aSummon[pChrType->nSummonNum].fAngle *= D3DX_PI;
									}//====================//
									else if (strcmp(aDataSearch, "�e:") == 0)
									{// �e�ԍ���ǂݍ���
										fscanf(pFile, "%d", &pChrType->aSummon[pChrType->nSummonNum].nParentNum);
									}//====================//
									else if (strcmp(aDataSearch, "�q:") == 0)
									{// �q���ԍ���ǂݍ���
										fscanf(pFile, "%d", &pChrType->aSummon[pChrType->nSummonNum].nChildNum);
									}//====================//
								}
							}

							// �����������Z
							pChrType->nSummonNum+=1;
						}
					}
				}//====================//
			}
		}
	}
}

//========================================
// StringLoadChr_enemy_00Type�֐� - �G[00] �̎�ނ𕶎��񂩂�ǂݍ��� -
//========================================
void StringLoadChr_enemy_00Type(char aString[TXT_MAX], int *pType)
{
	Chr_enemy_00Type	*pChrType	// �G[00] �̎�ޖ��̏��
						= GetChr_enemy_00Type();

	for (int nCntChrType = 0; nCntChrType < CHR_ENEMY_00_TYPE_MAX; nCntChrType++, pChrType++)
	{
		if (strcmp(aString, pChrType->aLoadName) == 0)
		{// �ǂݍ��ݖ�����v�������A
			// �G�̎�ނ���
			*pType = nCntChrType;
			break;
		}
	}
}

//========================================
// InitChr_enemy_00�֐� - �G[00] �̏��������� -
//========================================
void InitChr_enemy_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	for (int nCntChr_enemy_00Type = 0; nCntChr_enemy_00Type < CHR_ENEMY_00_TYPE_MAX; nCntChr_enemy_00Type++) 
	{
		D3DXCreateTextureFromFile(pDevice, g_aChr_enemy_00Type[nCntChr_enemy_00Type].aTexturPath, &g_aTextureChr_enemy_00[nCntChr_enemy_00Type]);
	}
	
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CHR_ENEMY_00_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChr_enemy_00,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00	*pChr	// �G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pVtx+=4, pChr++)
	{
		// �p�����[�^�[�����������A��ނɉ����Đݒ�
		InitParameter(pChr);
		SetParameter(pChr);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pChr->pos,
			pChr->rot,
			false,
			pChr->fWidth,
			pChr->fHeight,
			g_aChr_enemy_00Type[pChr->nType].angleType);

		// rhw�̐ݒ�
		SetRHW(pVtx);

		// ���_�J���[�̐ݒ�
		SetVertexColor(pVtx, { 255,255,255,255 });

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			pChr->nPattern,
			g_aChr_enemy_00Type[pChr->nType].nPtnMaxX,
			g_aChr_enemy_00Type[pChr->nType].nPtnMaxY,
			NULL);
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// UninitChr_enemy_00�֐� - �G[00] �̏I������ -
//========================================
void UninitChr_enemy_00(void)
{
	// �e�N�X�`���̔j��
	for (int nCntChr_enemy_00 = 0; nCntChr_enemy_00 < CHR_ENEMY_00_TYPE_MAX; nCntChr_enemy_00++)
	{
		if (g_aTextureChr_enemy_00[nCntChr_enemy_00] != NULL)
		{
			g_aTextureChr_enemy_00[nCntChr_enemy_00]->Release();
			g_aTextureChr_enemy_00[nCntChr_enemy_00] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffChr_enemy_00 != NULL) 
	{
		g_pVtxBuffChr_enemy_00->Release();
		g_pVtxBuffChr_enemy_00 = NULL;
	}
}

//========================================
// UpdateChr_enemy_00�֐� - �G[00] �̍X�V���� -
//========================================
void UpdateChr_enemy_00(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00	*pChr	// �G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++, pVtx+=4)
	{
		if (!pChr->bUse)
		{// �g�p����Ă��Ȃ����A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		D3DXVECTOR3 posTemp			// �ʒu��ۑ�
					= pChr->pos;	//
		D3DXVECTOR3 rotTemp			// ������ۑ�
					= pChr->rot;	//

		// �s���ǂݍ��ݏ���
		ActLoading(pChr);

		// �T�E���h�ǂݍ��ݏ���
		SoundLoading(pChr);

		// �p�^�[��No.�ǂݍ��ݏ���
		PatternLoading(pChr);

		// �G�t�F�N�g�ǂݍ��ݏ���
		EffectLoading(pChr);

		// ���˓ǂݍ��ݏ���
		ShotLoading(pChr);

		// �����ǂݍ��ݏ���
		SummonLoading(pChr);

		// �ʒu���X�V
		PositionUpdate(&pChr->pos, &pChr->move);

		// �q�̈ړ�����
		ChildMoveProcess(pChr, posTemp, rotTemp);

		// �@��[00] �Ƃ̓����蔻�菈��
		CollisionChr_fighter_00(pChr);

		// ��ԏ���
		StateProcess(pChr);

		// �e�N�X�`�����W�̐ݒ�
		SetTexturePosition(pVtx,
			AnimationCount(
				&pChr->nPattern,
				g_aChr_enemy_00Type[pChr->nType].nAnimPtnMin,
				g_aChr_enemy_00Type[pChr->nType].nAnimPtnMax,
				&pChr->nCounterAnim,
				g_aChr_enemy_00Type[pChr->nType].nAnimTime,
				g_aChr_enemy_00Type[pChr->nType].animType),
			g_aChr_enemy_00Type[pChr->nType].nPtnMaxX,
			g_aChr_enemy_00Type[pChr->nType].nPtnMaxY,
			NULL);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pChr->pos,
			pChr->rot,
			false,
			pChr->fWidth * (1.0f + ((CHR_ENEMY_00_SWELL_DIAMETER - 1.0f)
				* ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] / (float)CHR_ENEMY_00_SWELL_TIME)))
			* pChr->scale.fWidth,
			pChr->fHeight * (1.0f + ((CHR_ENEMY_00_SWELL_DIAMETER - 1.0f)
				* ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] / (float)CHR_ENEMY_00_SWELL_TIME)))
			* pChr->scale.fHeight,
			g_aChr_enemy_00Type[pChr->nType].angleType);

		// �F�̐ݒ�
		SetColor(pVtx, pChr);
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// DrawChr_enemy_00�֐� - �G[00] �̕`�揈�� -
//========================================
void DrawChr_enemy_00(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffChr_enemy_00, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	Chr_enemy_00	*pChr	// �G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (pChr->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_aTextureChr_enemy_00[pChr->nType]);

			// �G[00] �̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntChr * 4, 2);
		}
	}
}

//========================================
// SetChr_enemy_00�֐� - �G[00] �̐ݒ菈�� -
//========================================
void SetChr_enemy_00(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, bool bAreaBoss, bool bBomb, int nParentNum, int nChildNum)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffChr_enemy_00->Lock(0, 0, (void**)&pVtx, 0);

	Chr_enemy_00	*pChr	// �G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pVtx += 4, pChr++)
	{
		if (pChr->bUse)
		{// �g�p����Ă����Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// �p�����[�^������������
		InitParameter(pChr);

		pChr->pos			= pos;			// �ʒu����
		pChr->rot			= rot;			// �p�x����
		pChr->nType			= nType;		// ��ނ���
		pChr->bBomb			= bBomb;		// ���e�����t���O����
		pChr->nParentNum	= nParentNum;	// �e�ԍ�
		pChr->nChildNum		= nChildNum;	// �q���ԍ�

		// �p�����[�^����ނɉ����Đݒ�
		SetParameter(pChr);

		// ���_���W�̐ݒ�
		SetVertexPosition(pVtx,
			pChr->pos,
			pChr->rot,
			true,
			pChr->fWidth * (1.0f + ((CHR_ENEMY_00_SWELL_DIAMETER - 1.0f)
				* ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] / (float)CHR_ENEMY_00_SWELL_TIME))),
			pChr->fHeight * (1.0f + ((CHR_ENEMY_00_SWELL_DIAMETER - 1.0f)
				* ((float)pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] / (float)CHR_ENEMY_00_SWELL_TIME))),
			g_aChr_enemy_00Type[pChr->nType].angleType);

		// �F�̐ݒ�
		SetColor(pVtx, pChr);

		// �o�����̏�Ԃɂ���
		pChr->aState[CHR_ENEMY_00_STATE_IN_POP] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_IN_POP] = 0;

		// �g�p���Ă����Ԃɂ���
		pChr->bUse = true;

		break;
	}

	// ���_���W���A�����b�N����
	g_pVtxBuffChr_enemy_00->Unlock();
}

//========================================
// HitChr_enemy_00�֐� - �G[00] �̃q�b�g���� -
//========================================
void HitChr_enemy_00(Chr_enemy_00 *pChr, int nDamage)
{
	if ((pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE])
		||
		(pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]))
	{// ���G��Ԃ̎��A
		if (pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE])
		{
			// ����SE
			PlaySound(SOUND_LABEL_SE_METAL_000);
		}

		// �������I������
		return;
	}

	// �̗͂���_���[�W�����Z
	pChr->nLife -= nDamage;

	SetCombo(pChr->pos, D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR((float)245 / (float)255, (float)191 / (float)255, (float)63 / (float)255, 1.0f), 60);

	PlaySound(SOUND_LABEL_SE_DAMAGE_000);

	if (pChr->nLife <= 0)
	{// �̗͂�0�ȉ��̎��A
		// �̗͂�0�ɂ���
		pChr->nLife = 0;
		
		// ��������Ԃɂ���
		pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR] = 0;
	}
	else if (nDamage > 0)
	{// �̗͂�0�������Ă��鎞�A
		if (pChr->bReceivedExplosion)
		{// �������󂯂��t���O���^�̎��A
			// �����ϐ���Ԃɂ���
			pChr->aState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE] = true;
			pChr->aCounterState[CHR_ENEMY_00_STATE_EXPLOSION_RESISTANCE] = 0;
		}

		// �������󂯂��t���O���U�ɂ���
		pChr->bReceivedExplosion = false;

		// �_���[�W��Ԃɂ���
		pChr->aState[CHR_ENEMY_00_STATE_DAMAGE] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] = 0;

		// �q���_���[�W��Ԃɂ���
		ChildDamageState(pChr);

		// �_���[�W�T�E���h��炷
		PlaySound(g_aChr_enemy_00Type[pChr->nType].damageSound);
	}
}

//========================================
// HitChr_enemy_00ForAtk_explosion_00�֐� - ����[00] �p�̓G[00] �̃q�b�g���� -
//========================================
bool HitChr_enemy_00ForAtk_explosion_00(Chr_enemy_00 *pChr, int nDamage, int nCntChain)
{
	if ((pChr->aAct[CHR_ENEMY_00_ACT_INVINCIBLE])
		|| (nDamage <= 0))
	{// ���G��Ԃ̎��A�������̓_���[�W��0�ȉ��̎��A
		// �������I������
		return false;
	}

	// �̗͂���_���[�W�����Z
	pChr->nLife -= nDamage;

	if (pChr->nLife <= 0)
	{// �̗͂�0�ȉ��̎��A
		// �̗͂�0�ɂ���
		pChr->nLife = 0;

		// �c��ݏ�Ԃɂ���
		pChr->aState[CHR_ENEMY_00_STATE_SWELL] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_SWELL] = 0;

		// �A���̃J�E���g����
		pChr->nCntChain = nCntChain;

		return true;
	}
	else if (nDamage > 0)
	{// �̗͂�0�������Ă��鎞�A
		// �_���[�W��Ԃɂ���
		pChr->aState[CHR_ENEMY_00_STATE_DAMAGE] = true;
		pChr->aCounterState[CHR_ENEMY_00_STATE_DAMAGE] = 0;

		// �q���_���[�W��Ԃɂ���
		ChildDamageState(pChr);

		// �_���[�W�T�E���h��炷
		PlaySound(g_aChr_enemy_00Type[pChr->nType].damageSound);
	}

	return false;
}

//========================================
// DestructionChr_enemy_00�֐� - �G[00] �̔j�󏈗� -
//========================================
void DestructionChr_enemy_00(Chr_enemy_00 *pChr)
{
	// �q�̔j�󏈗�
	ChildDestruction(pChr);

	if (g_aChr_enemy_00Type[pChr->nType].nScore > 0)
	{// �X�R�A��0�������Ă��鎞�A
		int nScore	// �X�R�A�\��
			= g_aChr_enemy_00Type[pChr->nType].nScore
			* (1.0f + (CHR_ENEMY_00_SCORE_DIAMETER_ADDEND_CHAIN * pChr->nCntChain));

		// �X�R�A�����Z
		//GetChr_fighter_00()->nScore += nScore;
		// ��

		// �X�R�A[00] �̐ݒ菈��
		//SetUi_score_00(pChr->pos, nScore);
		// ��
	}

	if (pChr->aAct[CHR_ENEMY_00_ACT_SUMMONING_ON_DIED])
	{// ���S�������t���O���^�̎��A
		// ��O��������
		SummonException(pChr);
	}

	// �G[00]���g�p���Ă��Ȃ���Ԃɂ���
	pChr->bUse = false;

	// ����[00] �̐ݒ菈��
	SetAtk_explosion_00(pChr->pos, pChr->rot, g_aChr_enemy_00Type[pChr->nType].explosionType, pChr->nCntChain + 1);
	
	if (g_aChr_enemy_00Type[pChr->nType].bBonusSound)
	{// �{�[�i�X�T�E���h�t���O���^�̎��A
		
	}
}

//========================================
// RemoveAllChr_enemy_00�֐� - �G[00] ��S�ď������� -
//========================================
void RemoveAllChr_enemy_00(void) 
{
	Chr_enemy_00	*pChr	// �G[00] �̏��̃|�C���^
					= g_aChr_enemy_00;

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++)
	{
		if (!pChr->bUse)
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		// �o�����łȂ�����
		pChr->aState[CHR_ENEMY_00_STATE_IN_POP] = false;

		if (!pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR])
		{// �������łȂ����A
			// �������ɂ���
			pChr->aState[CHR_ENEMY_00_STATE_IN_CLEAR]			= true;
			pChr->aCounterState[CHR_ENEMY_00_STATE_IN_CLEAR]	= 0;
		}

		if (pChr->aState[CHR_ENEMY_00_STATE_SWELL])
		{// �c��ݏ�Ԃ̎��A
			pChr->aState[CHR_ENEMY_00_STATE_SWELL] = false;
			// �j�󏈗�
			DestructionChr_enemy_00(pChr);
		}
	}
}
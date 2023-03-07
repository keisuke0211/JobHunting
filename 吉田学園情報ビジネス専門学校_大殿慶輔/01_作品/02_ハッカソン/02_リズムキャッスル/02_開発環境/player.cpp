//==========================================
//
//�v���C���[�֌W�v���O����[player.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "polygon2D.h"
#include "chr_enemy_00.h"
#include "text.h"
#include "sound.h"
#include "ui_move-text_00.h"
#include "obj_castle_00.h"
#include "eff_particle_00.h"

//�}�N��
#define PLAYER_TEXTURE_PATH		"data\\TEXTURE\\Player\\Player-Motion.png"
#define LINE_TEXTURE_PATH		"data\\TEXTURE\\Player\\Player_JudgeLinekari.png"
#define LINE_SIZE_WIDTH			(64)	//���C���̃e�N�X�`���T�C�Y��
#define LINE_SIZE_HEIGHT		(64)	//���C���̃e�N�X�`���T�C�Y����
#define PLAYER_SIZE_WIDTH		(200)	//�v���C���[�e�N�X�`���̕�
#define PLAYER_SIZE_HEIGHT		(200)	//�v���C���[�e�N�X�`���̍���
#define PLAYER_PATTERN_WIDTH	(6)		//�v���C���[�e�N�X�`���̃p�^�[����
#define PLAYER_PATTERN_HEIGHT	(1)		//�v���C���[�e�N�X�`���̃p�^�[������
#define PLAYER_ANIM_COUNT		(30)	//�v���C���[�̕\���؂�ւ�����
#define PLAYER_ATK_CENTER		(300)	//����̒��S�ʒu
#define PLAYER_TEXNUM			(2)		//�e�N�X�`����
#define LINE_POS				D3DXVECTOR3(380.0f, 600.0f, 0.0f)

//����
#define JUDGE_MOVE_SPEED		(3.0f)	//����\���̈ړ���

//�X�R�A�֌W
#define SCORE_DIGIT				(8)		//�X�R�A��
#define SCORE_BASE				(100)	//����100%�̎��̃X�R�A
#define SCORE_TEXT_NUM			(6 + SCORE_DIGIT + 1)	//�X�R�A�\���Ɏg���e�L�X�g�̕\���������i"SCORE " + �X�R�A�� + \n�j
#define SCORE_POS				D3DXVECTOR3(SCREEN_WIDTH - 40.0f, 40.0f, 0.0f)	//�X�R�A�\���n�_

//�R���{�֌W
#define COMBO_MOVETIME			(30)	//�R���{�\��������̈ʒu�Ɉړ�����܂ł̎���
#define COMBO_DIGIT				(3)		//�R���{��
#define COMBO_TEXT_NUM			(6 + COMBO_DIGIT + 1)	//�R���{�\���Ɏg���e�L�X�g�̕\���������i"COMBO " + �R���{�� + \n�j
#define COMBO_TEXT_LENGTH		(40.0f * COMBO_TEXT_NUM)
#define COMBO_POS				SCORE_POS + D3DXVECTOR3(COMBO_TEXT_LENGTH, 40.0f, 0.0f)	//�R���{�\���n�_�̖ڕW�ʒu
#define COMBO_FLASH_PULSE		(4)		//�_�ł̊Ԋu
#define COMBO_LIGHTING_TIME		(120)	//�_�ł���_���ɕς�鎞��

//�F�֌W
#define COLOR_WHITE				{255,255,255,255}	//��
#define COLOR_YELLOW			{255,255,0,255}		//���F

//�A�j���[�V�����p�^�[����
typedef enum
{
	ANIMPATT_NONE = 0,
	ANIMPATT_ATK_A,
	ANIMPATT_ATK_B,
	ANIMPATT_ATK_X,
	ANIMPATT_ATK_Y,
	ANIMPATT_CLEAR,
	ANIMPATT_MAX
} ANIMPATT;

//�v���g
void AttackPlayer(Chr_enemy_00 *pEnemy);

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffPlayer;	//���_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayer[PLAYER_TEXNUM];		//�e�N�X�`���|�C���^
Player g_Player;
LPD3DXFONT g_pFontPlayer = NULL;			//�t�H���g�ւ̃|�C���^
int g_nCounterAppearTime = 0;				//�o�����̓_�Ŏ���
int g_nCounterFlashTime = 0;				//�_�Ŏ���

//����͈͂ƃX�R�A����
const int c_aJudgeWidth[JUDGE_MAX] = { 10, 20, 30 };				//����͈́i���肫���Ƃ���邢�Ƃ��������炱���������āj
const float c_aJudgeScoreRatio[JUDGE_MAX] = { 1.0f, 0.5f, 0.0f };	//����ɑ΂���X�R�A�����i1.0f = 100%�j
const int c_aAnimPattern[ANIMPATT_MAX] = { 0,3,4,1,2,5 };			//�A�j���[�V�����p�^�[���ԍ�

//========================
//�v���C���[����������
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		PLAYER_TEXTURE_PATH,
		&g_pTexturePlayer[0]);

	D3DXCreateTextureFromFile(pDevice,
		LINE_TEXTURE_PATH,
		&g_pTexturePlayer[1]);

	//���_�o�b�t�@�̐����i���_��*�v���C���[�Ɛ��j
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffPlayer,
		NULL);

	//�v���[���[����
	SetPlayer();
}

//========================
//�v���C���[�I������
//========================
void UninitPlayer(void)
{
	for (int nCntUninit = 0; nCntUninit < PLAYER_TEXNUM; nCntUninit++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturePlayer[nCntUninit] != NULL)
		{
			g_pTexturePlayer[nCntUninit]->Release();
			g_pTexturePlayer[nCntUninit] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxbuffPlayer != NULL)
	{
		g_pVtxbuffPlayer->Release();
		g_pVtxbuffPlayer = NULL;
	}
}

//========================
//�v���C���[�X�V����
//========================
void UpdatePlayer(void)
{
	Chr_enemy_00 *pEnemy = GetChr_enemy_00();
	char aDispScoreText[SCORE_TEXT_NUM] = "SCORE ";
	char aDispComboText[COMBO_TEXT_NUM] = "COMBO ";
	char aScoreNumText[SCORE_DIGIT];
	char aComboNumText[COMBO_DIGIT];
	bool bPushSameButton = false;

	//�A�j���[�V�����J�E���^���₷
	g_Player.nCounterAnim++;

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//�J�E���^�����̒l�ɒB������\����߂�
	if (g_Player.nCounterAnim >= PLAYER_ANIM_COUNT)
	{//�߂�
		//�e�N�X�`���ݒ�
		SetTexturePos2D(
			pVtx,
			c_aAnimPattern[ANIMPATT_NONE],
			PLAYER_PATTERN_WIDTH,
			PLAYER_PATTERN_HEIGHT,
			NULL);
	}

	for (int nCntEnemy = 0; nCntEnemy < CHR_ENEMY_00_MAX; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->nCounterMove >= PLAYER_ATK_CENTER - c_aJudgeWidth[JUDGE_MISS] && pEnemy->nCounterMove <= PLAYER_ATK_CENTER + c_aJudgeWidth[JUDGE_MISS])
			{//�v���C���[�̍U���͈͓��ɓG������i�~�X����͈̔́j
				Chr_enemy_00Type *pEnemyType = &GetChr_enemy_00Type()[pEnemy->nType];

				//�{�^���iABXY�j�̏��擾
				bPushSameButton = GetButtonTrigger(BUTTON_A + pEnemyType->elem);	//A�{�^������_�Ƃ��đ����ƑΉ�����{�^�����擾

				//�����ƑΉ�����{�^���������ꂽ��|��
				if (bPushSameButton == true)
				{//�|������
					AttackPlayer(pEnemy);	//�U���i����j����

					//�e�N�X�`���ݒ�
					SetTexturePos2D(
						pVtx,
						c_aAnimPattern[ANIMPATT_ATK_A + pEnemyType->elem],
						PLAYER_PATTERN_WIDTH,
						PLAYER_PATTERN_HEIGHT,
						NULL);

					//�J�E���^�[���Z�b�g
					g_Player.nCounterAnim = 0;
					g_Player.nCounterComboMoveTime = 0;

					//�_�Ŏ��ԃ��Z�b�g
					g_nCounterFlashTime = 0;
				}
			}
		}
	}

	//�X�R�A�e�L�X�g�ݒ�
	snprintf(&aScoreNumText[0], SCORE_DIGIT * 4, "%d", g_Player.nScore);
	strcat(&aDispScoreText[0], &aScoreNumText[0]);
	SetText2D(&aDispScoreText[0], 0, DISPLAY_RIGHT, D3DXVECTOR3(SCREEN_WIDTH - 40.0f, 40.0f, 0.0f), { 255,255,255,255 }, 1.0f, 1.0f);

	//�R���{�e�L�X�g�ݒ�
	if (g_Player.nCombo > 0)
	{//1�R���{�ȏ�Ȃ瓮����
		g_Player.nCounterComboMoveTime++;	//�J�E���g���₷
		g_nCounterFlashTime++;				//�_�ŃJ�E���g���₷

		float fRate = (float)g_Player.nCounterComboMoveTime / COMBO_MOVETIME;	//�����v�Z
		if (fRate > 1.0f)
		{//����̈ʒu�𒴂��Ȃ��悤�ɏC��
			fRate = 1.0f;
		}

		//�\��
		snprintf(&aComboNumText[0], COMBO_DIGIT * 4, "%d", g_Player.nCombo);
		strcat(&aDispComboText[0], &aComboNumText[0]);

		//�F����
		Color color = COLOR_WHITE;
		if (g_nCounterFlashTime / COMBO_FLASH_PULSE % 2 == 0 || g_nCounterFlashTime >= COMBO_LIGHTING_TIME)	//2�t���[����1��܂���60�t���[����
		{
			color = COLOR_YELLOW;
		}

		//�F������ɕ����\��
		SetText2D(&aDispComboText[0], 0, DISPLAY_RIGHT, COMBO_POS + D3DXVECTOR3(-COMBO_TEXT_LENGTH * fRate, 0.0f, 0.0f), color, 1.0f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffPlayer->Unlock();
}

//========================
//�v���C���[�`�揈��
//========================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxbuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���ݒ�ƕ`��
	for (int nCntDraw = 0; nCntDraw < PLAYER_TEXNUM; nCntDraw++)
	{
		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer[nCntDraw]);

		if (g_Player.bDisp)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntDraw, 2);
		}
	}
}

//========================
//�v���C���[���擾
//========================
Player *GetPlayer(void)
{
	return &g_Player;
}

//========================
//�v���C���[��������
//========================
void SetPlayer(void)
{
	//�A�j���[�V�����֌W������
	g_Player.nCounterAnim = 0;		//�J�E���^������
	g_Player.nPatternAnim = 0;		//�p�^�[��No.������

	//�ړ��֌W������
	g_Player.pos = D3DXVECTOR3(340.0f, 520.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//��]�֌W������
	//����������
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//��ԏ�����
	g_nCounterAppearTime = 0;

	//�̗́E�X�R�A������
	g_Player.nLife = 5;
	g_Player.nScore = 0;
	g_Player.nCombo = 0;

	//��ԃJ�E���^������
	g_Player.nCounterAttack = 0;

	//�����Ă��邱�Ƃɂ���
	g_Player.bDisp = true;

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//�v���C���[�̏��ݒ�
	//���_���W�̐ݒ�
	SetVertexPos2D(
		pVtx,
		g_Player.pos,
		INITD3DXVECTOR3,
		false,
		PLAYER_SIZE_WIDTH,
		PLAYER_SIZE_HEIGHT,
		ANGLE_TYPE_FIXED);

	//���W�ϊ��p�W���ݒ�
	SetRHW2D(pVtx);

	//���_�J���[
	Color playerColor = { 255,255,255,255 };
	SetVertexColor2D(pVtx, playerColor);

	//�e�N�X�`���ݒ�
	SetTexturePos2D(
		pVtx,
		ANIMPATT_NONE,
		PLAYER_PATTERN_WIDTH,
		PLAYER_PATTERN_HEIGHT,
		NULL);

	//���̏��ݒ�
	pVtx += 4;
	//���_���W�̐ݒ�
	SetVertexPos2D(
		pVtx,
		LINE_POS,
		INITD3DXVECTOR3,
		false,
		LINE_SIZE_WIDTH,
		LINE_SIZE_HEIGHT,
		ANGLE_TYPE_FIXED);

	//���W�ϊ��p�W���ݒ�
	SetRHW2D(pVtx);

	//���_�J���[
	SetVertexColor2D(pVtx, playerColor);

	//�e�N�X�`���ݒ�
	SetTexturePos2D(
		pVtx,
		0,
		1,
		1,
		NULL);

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffPlayer->Unlock();

	//�O���[�o���̓_�Ŏ��ԃ��Z�b�g
	g_nCounterFlashTime = 0;
}

//========================
//�v���C���[�U�������i���Ƃ�����ƃX�R�A���Z�E�_���[�W�j
//========================
void AttackPlayer(Chr_enemy_00 *pEnemy)
{
	PlaySound(5);

	int nDistEnemy = pEnemy->nCounterMove - PLAYER_ATK_CENTER;

	if (abs(nDistEnemy) <= c_aJudgeWidth[JUDGE_EXELENT])
	{//EXELENT�̎��̏���
		g_Player.nScore += SCORE_BASE * c_aJudgeScoreRatio[JUDGE_EXELENT];	//�X�R�A���Z
		g_Player.nCombo++;
		SetUi_moveText_00("EXELENT!", 1, g_Player.pos, D3DXVECTOR3(0.0f, -3.0f, 0.0f), { 0,0,0,255 }, 60);	//���������ݒ�
		SetEff_particle_00(pEnemy->pos, INITD3DXVECTOR3, (EFF_PARTICLE_00_TYPE)pEnemy->nType);
	}
	else if (abs(nDistEnemy) <= c_aJudgeWidth[JUDGE_GOOD])
	{//GOOD�̎��̏���
		g_Player.nScore += SCORE_BASE * c_aJudgeScoreRatio[JUDGE_GOOD];		//�X�R�A���Z
		g_Player.nCombo++;
		SetUi_moveText_00("GOOD", 1, g_Player.pos, D3DXVECTOR3(0.0f, -3.0f, 0.0f), { 0,0,0,255 }, 60);		//���������ݒ�
		SetEff_particle_00(pEnemy->pos, INITD3DXVECTOR3, (EFF_PARTICLE_00_TYPE)pEnemy->nType);
	}
	else
	{//MISS�̎��̏���
		g_Player.nLife--;		//�̗͌��炷
		g_Player.nCombo = 0;	//�R���{���Z�b�g
		g_Player.nCounterComboMoveTime = 0;
		SetUi_moveText_00("MISS...", 1, g_Player.pos, D3DXVECTOR3(0.0f, -3.0f, 0.0f), { 0,0,0,255 }, 60);	//���������ݒ�
		SetDamageObj_castle_00();	// ��̃_���[�W�ݒ�
	}
	pEnemy->bUse = false;
}
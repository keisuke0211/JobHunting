//========================================
// 
// �Q�[�����[00]�̏���
// Author:���� ����
// 
//========================================
// *** md_game_00.cpp ***
//========================================
#include "main.h"
#include "chunk.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "text.h"
#include "chr_enemy_00.h"
#include "atk_bullet_00.h"
#include "atk_explosion_00.h"
#include "md_game_00.h"
#include "bg.h"
#include "life.h"
#include "ground.h"
#include "player.h"
#include "score.h"
#include "cloud.h"
#include "combo.h"
#include <stdio.h>

//****************************************
// �}�N����` 
//****************************************

// �B���R�}���h�̗L��
#define SECRET_COMMAND	(true)

// �o���҂��̎���
// ���S��̗]�C�̎���
#define POP_WAIT_TIME		(120)
#define DIED_AFTERGLOW_TIME	(120)

// �E�F�[�u�҂��̎���
// �E�F�[�u�҂��̗]�C
// �E�F�[�u�e�L�X�g�̒��Ԃ܂ł̈ړ��ɂ����鎞��
// �E�F�[�u�e�L�X�g�̏I���܂ł̈ړ����J�n���鎞��
// �E�F�[�u�e�L�X�g�̏����̈ʒu
// �E�F�[�u�e�L�X�g�̒��Ԃ̈ʒu
// �E�F�[�u�e�L�X�g�̍Ō�̈ʒu
// �E�F�[�u�e�L�X�g�̑傫���ɂ�����{��
// �E�F�[�u�e�L�X�g�̐F
#define WAVE_WAIT_TIME						(60)
#define WAVE_WAIT_AFTERGLOW					(20)
#define WAVE_TEXT_INTERMEDIATE_MOVE_TIME	(20)
#define WAVE_TEXT_END_MOVE_START_TIME		(40)
#define WAVE_TEXT_FIRST_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.4f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_INTERMEDIATE_POS			(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_LAST_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.6f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define WAVE_TEXT_SCALE						(ScaleDiameter{1.6f, 1.6f})
#define WAVE_TEXT_COLOR						(Color{255, 255, 255, 255})

// �X�^�[�g�҂��̎���
// �X�^�[�g�e�L�X�g�̈ʒu
// �X�^�[�g�e�L�X�g�̒��Ԃ܂łɂ����鎞��
// �X�^�[�g�e�L�X�g�̏I���܂ł̐��ڂ��J�n���鎞��
// �X�^�[�g�e�L�X�g�̏����̑傫���ɂ�����{��
// �X�^�[�g�e�L�X�g�̒��Ԃ̑傫���ɂ�����{��
// �X�^�[�g�e�L�X�g�̍Ō�̑傫���ɂ�����{��
// �X�^�[�g�e�L�X�g�̐F
#define START_WAIT_TIME					(60)
#define START_TEXT_POS					(D3DXVECTOR3((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT * 0.5f, 0.0f))
#define START_TEXT_INTERMEDIATE_TIME	(20)
#define START_TEXT_END_START_TIME		(40)
#define START_TEXT_FIRST_SCALE			(ScaleDiameter{8.0f, 8.0f})
#define START_TEXT_INTERMEDIATE_SCALE	(ScaleDiameter{2.0f, 2.0f})
#define START_TEXT_LAST_SCALE			(ScaleDiameter{8.0f, 0.0f})
#define START_TEXT_COLOR				(Color{243, 191, 63, 255})

// ���U���g�X�R�A�̉��Z�ɂ����鎞��
// ���U���g�X�R�A�̉��Z��
// ���U���g�X�R�A�̕\���ɂ����鎞��
// ���U���g�X�R�A�̕\���������̐F
#define RESULT_SCORE_ADD_TIME			(3)
#define RESULT_SCORE_ADD_NUM			(17)
#define RESULT_SCORE_SHOW_TIME			(8)
#define RESULT_SCORE_SHOW_COMP_COLOR	(Color{243, 191, 63, 255})

// �����L���O�e�L�X�g�̊J�n�ʒu
// �����L���O�e�L�X�g�̏I���ʒu
// �����L���O�e�L�X�g�̈ړ��ɂ����鎞��
// �����L���O�e�L�X�g�̔z�u�ԊuX
// �����L���O�e�L�X�g�̓_�ŐF
// �����L���O�e�L�X�g�̓_�łɂ����鎞��
// �����L���O�t���[���̈ʒu
#define RANKING_TEXT_FIRST_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.4f) - (PIXEL * 8 * 9), PIXEL * 140, 0.0f))
#define RANKING_TEXT_LAST_POS		(D3DXVECTOR3((SCREEN_WIDTH * 0.5f) - (PIXEL * 8 * 9), PIXEL * 140, 0.0f))
#define RANKING_TEXT_MOVE_TIME		(10)
#define RANKING_TEXT_SPACE_Y		(PIXEL * 16)
#define RANKING_TEXT_BRINK_COLOR	(Color{243, 191, 63, 255})
#define RANKING_TEXT_BRINK_TIME		(4)
#define RANKING_FRAME_POS			(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, (SCREEN_HEIGHT * 0.5f) + (PIXEL * 16), 0.0f))

// �|�[�Y�e�L�X�g�̓_�łɂ����鎞��
#define PAUSE_TEXT_BRINK_TIME	(16)

// �|�[�Y���j���[�̈ʒu
// ���U���g���j���[�̈ʒu
#define MD_GAME_00_PAUSE_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 80, 0.0f))
#define MD_GAME_00_RESULT_MENU_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT - PIXEL * 64, 0.0f))

// �R���e�B�j���[�̃J�E���g��
// �R���e�B�j���[�̃J�E���g�ɂ����鎞��
// �R���e�B�j���[�̗]�C�ɂ����鎞��
// �R���e�B�j���[�����Ñ��̓_�łɂ����鎞��
#define CONTINUE_COUNT_NUM			(10)
#define CONTINUE_COUNT_TIME			(60)
#define CONTINUE_AFTERGLOW_TIME		(60)
#define PUSH_CONTINUE_BRINK_TIME	(30)

// �Q�[���N���A�ɂ����鎞��
// �Q�[���I�[�o�[�ɂ����鎞��
// �Q�[���N���A�̃e�L�X�g�ɂ����鎞��
// �Q�[���I�[�o�[�̃e�L�X�g�ɂ����鎞��
#define GAME_CLEAR_TIME				(300)
#define GAME_OVER_TIME				(300)
#define GAME_CLEAR_TEXT_TIME		(12)
#define GAME_OVER_TEXT_TIME			(12)

// �m�[�_���[�W�{�[�i�X�X�R�A
// �m�[�_���[�W�{�[�i�X�X�R�A�̔�e���̌�����
#define NO_DAMAGE_BONUS_SCORE			(100000)
#define NO_DAMAGE_BONUS_SCORE_DECREASE	(20000)

// ���̑��Έʒu
#define ALLOW_RELATIVE_POSITION	(D3DXVECTOR3(PIXEL * 8 * 6.5f,PIXEL * 0,0.0f))

// �G�̐ݒ���̍ő�
#define ENEMY_SET_MAX	(256)
#define TIME			(90)

//****************************************
// �񋓌^�̒�`
//****************************************

// �^�C�g�����[00] �̃��U���g���j���[
typedef enum
{
	MD_GAME_00_RESULT_MENU_RETRY = 0,		// ���g���C
	MD_GAME_00_RESULT_MENU_BACK_TO_TITLE,	// �^�C�g���ɖ߂�
	MD_GAME_00_RESULT_MENU_MAX,
}MD_GAME_00_RESULT_MENU;

// �^�C�g�����[00] �̃|�[�Y���j���[
typedef enum
{
	MD_GAME_00_PAUSE_MENU_RESUME = 0,		// �ĊJ
	MD_GAME_00_PAUSE_MENU_RETRY,			// ���g���C
	MD_GAME_00_PAUSE_MENU_SETTING,			// �ݒ�
	MD_GAME_00_PAUSE_MENU_BACK_TO_TITLE,	// �^�C�g���ɖ߂�
	MD_GAME_00_PAUSE_MENU_MAX,
}MD_GAME_00_PAUSE_MENU;

//****************************************
// �\���̂̒�`
//****************************************

// ���}�e�L�X�g�̏��\����
typedef struct 
{
	char			aString				// 
					[TXT_MAX];			// ������
	D3DXVECTOR3		pos;				// �ʒu
	ScaleDiameter	scaleDiameter;		// �傫���ɂ�����{��
	Color			col;				// �F
}SignalText;

// �����L���O�e�L�X�g�̏��\����
typedef struct 
{
	D3DXVECTOR3 pos;			// �ʒu
	int			nCounterMove;	// �ړ��J�E���^�[
	Color		col;			// �F
}RankingText;

//****************************************
// �v���g�^�C�v�錾
//****************************************

// �e�L�X�g���e�̍X�V����
void UpdateTxtMd_game_00(void);

// �Q�[���̏�Ԃɉ������J�n����
void StartGameState(MD_GAME_00_STATE state);

// �Q�[���̏�Ԃɉ������I������
void EndGameState(MD_GAME_00_STATE state);

// �Q�[���̏�Ԃɉ������X�V����
void UpdateGameState(void);

//****************************************
// �O���[�o���錾
//****************************************
MD_GAME_00_STATE	g_gameState				//
					= MD_GAME_00_STATE_N1;	// �Q�[���̏��
int					g_nStateCounter;		// ��Ԃ̃J�E���^�[
SignalText			g_signalText;			// ���}�e�L�X�g�̏��
bool				g_bResultMenu;			// ���U���g���j���[�̏o���t���O
int					g_nEnemyNum = 0;		// �G�̐�

EnemySet	g_aEnemySet[ENEMY_SET_MAX];	// �G�̐ݒ���
int			g_nCountertime;				// ���Ԃ̃J�E���^�[
int			g_nTime;					// ����
int			g_nScore;					// �X�R�A

// �Q�[�����[00] �̃e�L�X�g�̏��
Text g_aMd_game_00Text[MD_GAME_00_TEXT_MAX] =
{
	{ ""			, D3DXVECTOR3(0.0f, 0.0f, 0.0f)									, {255, 255, 255, 255} , FONT_001 },	// ���}
	{ "TIME  :%d"	, D3DXVECTOR3(PIXEL * 8 * 24, PIXEL * 8 * 2.5f, 0.0f)			, {255, 255, 255, 255} , FONT_000 },	// ����
	{ "SCORE :%08d"	, D3DXVECTOR3(PIXEL * 8 * 24, PIXEL * 8 * 1.5f, 0.0f)			, {255, 255, 255, 255} , FONT_000 },	// �X�R�A
	{ "STAGE CLEAR"	, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)	, {243, 191, 63 , 255} , FONT_001 },	// �Q�[���N���A
	{ "GAME OVER"	, D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)	, {88 , 248, 152, 255} , FONT_001 },	// �Q�[���I�[�o�[
};

// �X�R�A���擾
int *GetScore(void) 
{
	return &g_nScore;
}

//========== *** �Q�[�����[00]�̃e�L�X�g�̏����擾 ***
Text GetMd_game_00Text(MD_GAME_00_TEXT text)
{
	return g_aMd_game_00Text[text];
}

//========== *** �Q�[���̏�Ԃ̎擾	***
MD_GAME_00_STATE GetGameState(void)
{
	return g_gameState;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// UpdateTxtMd_game_00�֐� - �e�L�X�g���e�̍X�V���� -
//========================================
void UpdateTxtMd_game_00(void)
{
	char			aString					//
					[TXT_MAX];				// ���������p
		
	//========== *** �e�L�X�g[�X�R�A] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].aDisplay,g_nScore);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_SCORE,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SCORE].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�^�C��] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_TIME].aDisplay, g_nTime);
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_TIME,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_TIME].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_TIME].col,
			TEXT_MODE_LEFT,
			g_aMd_game_00Text[MD_GAME_00_TEXT_TIME].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[���}] ***
	{
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_SIGNAL,
			g_signalText.aString,
			g_signalText.pos,
			g_signalText.col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_SIGNAL].font,
			g_signalText.scaleDiameter.fWidth,
			g_signalText.scaleDiameter.fHeight);
	}
	//========== *** �e�L�X�g[�Q�[���N���A] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay);
		int nText = (g_nStateCounter / GAME_CLEAR_TEXT_TIME) + 1;
		IntControl(&nText, strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay), 0);
		aString[(g_nStateCounter / GAME_CLEAR_TEXT_TIME) + 1] = '\0';
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_GAME_CLEAR,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].font,
			1.0f,
			1.0f);
	}
	//========== *** �e�L�X�g[�Q�[���I�[�o�[] ***
	{
		sprintf(aString, g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay);
		int nText = (g_nStateCounter / GAME_OVER_TEXT_TIME) + 1;
		IntControl(&nText, strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay), 0);
		aString[(g_nStateCounter / GAME_OVER_TEXT_TIME) + 1] = '\0';
		UpdateTxt_00(
			TEXT_FIELD_UI,
			MD_GAME_00_TEXT_GAME_OVER,
			aString,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].pos,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].col,
			TEXT_MODE_CENTER,
			g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].font,
			1.0f,
			1.0f);
	}
}

//========================================
// StartGameState�֐� - �Q�[���̏�Ԃɉ������J�n���� -
//========================================
void StartGameState(MD_GAME_00_STATE state)
{
	// ��Ԃ̃J�E���^�[������������
	g_nStateCounter = 0;

	// �J�n����
	switch (g_gameState)
	{
		//========== *** �|�[�Y ***
	case MD_GAME_00_STATE_PAUSE:
	{
		
	}
	break;
		//========== *** �X�^�[�g�҂� ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// ���}�e�L�X�g�̐F��ݒ�
		g_signalText.col = START_TEXT_COLOR;

		// �����x��0�ɂ���
		g_signalText.col.nA = 0;

		// �ʒu��ݒ�
		g_signalText.pos = START_TEXT_POS;

		// �傫���ɂ�����{����ݒ�
		g_signalText.scaleDiameter = START_TEXT_FIRST_SCALE;

		// �������ݒ肷��
		sprintf(g_signalText.aString, "START");
	}
	break;
		//========== *** �ʏ� ***
	case MD_GAME_00_STATE_NORMAL:
	{

	}
	break;
		//========== *** �Q�[���N���A ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		// �e�L�X�g[�Q�[���N���A]	��\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_CLEAR, true);

		// �Q�[���N���ASE[000] ���Đ�
		PlaySound(SOUND_LABEL_SE_GAME_CLEAR_000);
	}
	break;
		//========== *** �Q�[���I�[�o�[ ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		// �e�L�X�g[�Q�[���I�[�o�[]	��\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_OVER, true);

		// �Q�[���I�[�o�[SE[000] ���Đ�
		PlaySound(SOUND_LABEL_SE_GAME_OVER_000);
	}
		break;
		//========== *** ���S��̗]�C ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{
		
	}
	break;
		//========= *** �I�� ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
	}
}

//========================================
// EndGameState�֐� - �Q�[���̏�Ԃɉ������I������ -
//========================================
void EndGameState(MD_GAME_00_STATE state)
{
	switch (g_gameState)
	{
		//========== *** �|�[�Y ***
	case MD_GAME_00_STATE_PAUSE:
	{
		
	}
	break;
		//========== *** �X�^�[�g�҂� ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// �e�L�X�g[���}] ���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SIGNAL, false);
	}
	break;
		//========== *** �ʏ� ***
	case MD_GAME_00_STATE_NORMAL:
	{

	}
	break;
		//========== *** �Q�[���N���A ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		// �e�L�X�g[�Q�[���N���A]	���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_CLEAR, false);
	}
	break;
		//========== *** �Q�[���I�[�o�[ ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		// �e�L�X�g[�Q�[���I�[�o�[]	���\��
		ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_GAME_OVER, false);
	}
	break;
		//========== *** �I�� ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
		//========== *** ���S��̗]�C ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{

	}
	break;
	}
}

//========================================
// UpdateGameState�֐� - �Q�[���̏�Ԃɉ������X�V���� -
//========================================
void UpdateGameState(void)
{
	switch (g_gameState)
	{
		//========== *** �|�[�Y ***
	case MD_GAME_00_STATE_PAUSE:
	{
		
	}
	break;
		//========== *** �ʏ� ***
	case MD_GAME_00_STATE_NORMAL:
	{
		
	}
	break;
		//========== *** �X�^�[�g�҂� ***
	case MD_GAME_00_STATE_START_WAIT:
	{
		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;

		if (g_nStateCounter <= START_TEXT_INTERMEDIATE_TIME)
		{// ��Ԃ̃J�E���^�[�����Ԃ܂łɂ����鎞�Ԉȉ��̎��A
			// �傫���ɂ�����{�����X�V
			g_signalText.scaleDiameter.fWidth
				= (START_TEXT_FIRST_SCALE.fWidth * (1.0f - ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME)))
				+ (START_TEXT_INTERMEDIATE_SCALE.fWidth * ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME));
			g_signalText.scaleDiameter.fHeight
				= (START_TEXT_FIRST_SCALE.fHeight * (1.0f - ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME)))
				+ (START_TEXT_INTERMEDIATE_SCALE.fHeight * ((float)g_nStateCounter / (float)START_TEXT_INTERMEDIATE_TIME));

			// �����x���X�V
			g_signalText.col.nA = 255
				* ((float)g_nStateCounter / (float)WAVE_TEXT_INTERMEDIATE_MOVE_TIME);
		}

		if (g_nStateCounter >= START_TEXT_END_START_TIME)
		{// ��Ԃ̃J�E���^�[���I���܂ł̈ړ����J�n���鎞�Ԉȏ�̎��A
			// �傫���ɂ�����{�����X�V
			g_signalText.scaleDiameter.fWidth
				= (START_TEXT_INTERMEDIATE_SCALE.fWidth
					* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME))))
				+ (START_TEXT_LAST_SCALE.fWidth * ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));
			g_signalText.scaleDiameter.fHeight
				= (START_TEXT_INTERMEDIATE_SCALE.fHeight
					* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME))))
				+ (START_TEXT_LAST_SCALE.fHeight * ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));

			// �����x���X�V
			g_signalText.col.nA = 255
				* (1.0f - ((float)(g_nStateCounter - START_TEXT_END_START_TIME) / (float)(START_WAIT_TIME - START_TEXT_END_START_TIME)));
		}

		if ((g_nStateCounter >= START_WAIT_TIME))
		{//	��Ԃ̃J�E���^�[���X�^�[�g�҂��̎��ԂɒB�������A
			// �Q�[���̏�Ԃ�ʏ�Ɉڍs
			SetMd_game_00State(MD_GAME_00_STATE_NORMAL);
		}
	}
	break;
		//========== *** �Q�[���N���A ***
	case MD_GAME_00_STATE_GAME_CLEAR:
	{
		if (g_nStateCounter >= GAME_CLEAR_TIME) 
		{// ��Ԃ̃J�E���^�[���Q�[���N���A�ɂ����鎞�ԂɒB�������A
			// ��Ԃ����U���g�Ɉڍs
			
		}
		else if ((g_nStateCounter % GAME_CLEAR_TEXT_TIME == 0)
			&&((g_nStateCounter / GAME_CLEAR_TEXT_TIME) < strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_CLEAR].aDisplay)))
		{// ��Ԃ̃J�E���^�[���Q�[���N���A�̃e�L�X�g�ɂ����鎞�ԂŊ���؂�鎞�A
			
		}

		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;
	}
	break;
		//========== *** �Q�[���I�[�o�[ ***
	case MD_GAME_00_STATE_GAME_OVER:
	{
		if (g_nStateCounter >= GAME_OVER_TIME)
		{// ��Ԃ̃J�E���^�[���Q�[���I�[�o�[�ɂ����鎞�ԂɒB�������A
			// ��Ԃ����U���g�Ɉڍs
			
		}
		else if ((g_nStateCounter % GAME_OVER_TEXT_TIME == 0)
			&& ((g_nStateCounter / GAME_OVER_TEXT_TIME) < strlen(g_aMd_game_00Text[MD_GAME_00_TEXT_GAME_OVER].aDisplay)))
		{// ��Ԃ̃J�E���^�[���Q�[���I�[�o�[�̃e�L�X�g�ɂ����鎞�ԂŊ���؂�鎞�A
			
		}

		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;
	}
	break;
		//========== *** ���S��̗]�C ***
	case MD_GAME_00_STATE_DIED_AFTERGLOW:
	{
		// ��Ԃ̃J�E���^�[�����Z
		g_nStateCounter++;

		if (g_nStateCounter >= DIED_AFTERGLOW_TIME)
		{// ��Ԃ̃J�E���^�[�����S��̗]�C�ɂ����鎞�ԂɒB�������A
			
		}
	}
	break;
		//========== *** �I�� ***
	case MD_GAME_00_STATE_END:
	{

	}
	break;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// InitMd_game_00�֐� - �Q�[�����[00]�̏��������� -
//========================================
void InitMd_game_00(void)
{
	g_gameState		= MD_GAME_00_STATE_N1;	// �Q�[���̏�Ԃ�������
	g_nCountertime	= 0;						// ���Ԃ̃J�E���^�[��������
	g_nTime			= TIME;						// ���Ԃ�������
	g_nScore		= 0;						// �X�R�A�̏�����

	// ����������
	InitTxt_00();			// �e�L�X�g
	InitChr_enemy_00();		// CHR:�G					[00]
	InitGround();			// �w�i
	InitPlayer();			// �v���C���[
	InitAtk_bullet_00();	// �e
	InitAtk_explosion_00();	// ����
	InitBg();				// �w�i
	InitLife();				// �̗�
	InitCloud();			// �_
	InitCombo();			// �R���{

	// �Q�[���̏�Ԃ��J�n�҂��ɐݒ�
	SetMd_game_00State(MD_GAME_00_STATE_START_WAIT);

	// �e�L�X�g[�X�R�A]	��\��
	ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_SCORE, true);
	ShowTxt_00(TEXT_FIELD_UI, MD_GAME_00_TEXT_TIME, true);

	PlaySound(SOUND_LABEL_BGM_STAGE_000);

	SetCloud2();		// �_�ݒ�
}

//========================================
// UninitMd_game_00�֐� - �Q�[�����[00]�̏I������ -
//========================================
void UninitMd_game_00(void)
{
	// �I������
	UninitTxt_00();				// �e�L�X�g
	UninitChr_enemy_00();		// CHR:�G	[00]
	UninitGround();				// �w�i
	UninitPlayer();				// �v���C���[
	UninitAtk_bullet_00();		// �e
	UninitAtk_explosion_00();	// ����
	UninitBg();					// �w�i
	UninitLife();				// �̗�
	UninitCloud();			// �_
	UninitCombo();			// �R���{

}

//========================================
// UpdateMd_game_00�֐� - �Q�[�����[00]�̍X�V���� -
//========================================
void UpdateMd_game_00(void)
{
	if (++g_nCountertime % 60 == 0
		&&
		GetFade() == FADE_NONE)
	{// ���ԃJ�E���^�[��60�ɒB�������A
		if (--g_nTime <= 0)
		{// ���Ԃ����Z��������0�ȉ��ɂȂ������A
			// ���U���g�Ɉȍ~
			SetFade(MODE_RESULT_00);
		}
		else 
		{
			for (int nCntSetEnemy = 0; nCntSetEnemy < g_nEnemyNum; nCntSetEnemy++) 
			{
				if (TIME - g_aEnemySet[nCntSetEnemy].in_time == g_nTime + 1)
				{// �o�����Ԃ���v�������A
					// �ݒ�
					SetChr_enemy_00(
						g_aEnemySet[nCntSetEnemy].pos,
						D3DXVECTOR3(0.0f, 0.0f, g_aEnemySet[nCntSetEnemy].nAngle),
						g_aEnemySet[nCntSetEnemy].nType,
						false,
						false,
						-1, 
						-1);
				}
			}
		}
	}

	if ((g_gameState != MD_GAME_00_STATE_PAUSE)
		&& (g_gameState != MD_GAME_00_STATE_N1))
	{// ��Ԃ��|�[�Y/����/�ݒ�łȂ����A
		// �X�V����
		UpdateChr_enemy_00();	// CHR:�G	[00]
		UpdateGround();			// �w�i
		UpdatePlayer();			// �v���C���[
		UpdateAtk_bullet_00();	// �e
		UpdateAtk_explosion_00();	// ����
		UpdateBg();					// �w�i
		UpdateLife();				// �̗�
		UpdateBg();					// �w�i
		UpdateLife();				// �̗�
		UpdateCloud();			// �_
		UpdateCombo();			// �R���{

	}

	// �Q�[���̏�Ԃɉ������X�V����
	UpdateGameState();

	// �e�L�X�g���e�̍X�V����
	UpdateTxtMd_game_00();
}

//========================================
// DrawMd_game_00�֐� - �Q�[�����[00]�̕`�揈�� -
//========================================
void DrawMd_game_00(void)
{
	// �`�揈��
	DrawCloud();			// �_
	DrawBg();				// �w�i
	DrawChr_enemy_00();		// CHR:�G	[00]
	DrawPlayer();			// �v���C���[
	DrawTxt_00();
	DrawAtk_bullet_00();	// �e
	DrawAtk_explosion_00();	// ����
	DrawGround();			// �w�i

	DrawLife();				// �̗�
	DrawCombo();			// �R���{
}

//========================================
// SetGameState�֐� - �Q�[���̏�Ԃ̐ݒ� -
//========================================
void SetMd_game_00State(MD_GAME_00_STATE state) 
{
	MD_GAME_00_STATE	stateTemp		// 
						= g_gameState;	// ���̏�Ԃ��ꎞ�I�Ɋi�[

	// �Q�[���̏�Ԃɉ������I������
	EndGameState(state);

	// �Q�[���̏�Ԃ𔽉f
	g_gameState = state;

	// �Q�[���̏�Ԃɉ������J�n����
	StartGameState(stateTemp);
}

//��====================================��
//�b�@�@�@ �G�l�~�[�f�[�^�ǂݍ��݁@�@ �@�b
//��====================================��
void LoadEnemy(void)
{
	int c = 0;	   //1�������m�F����ϐ�
	int column = 1;//�񐔂𐔂���ϐ�
	int line = 0;//�s���𐔂���ϐ�
	int nCntEnemy; //�J�E���g�ϐ�
	char aData[100];//�Ȃ��镶����
	FILE *pFile;
	memset(aData, 0, sizeof(aData));
	//�t�@�C�����J��
	pFile = fopen("data/csv/enemydata.csv", "r");
	//�t�@�C������P�������ǂݍ���
	while (fgetc(pFile) != '\n');
	while (c != EOF)
	{
		//�P�Z�����̕���������
		while (1)
		{
			c = fgetc(pFile);
			//�����Ȃ烋�[�v�𔲂���
			if (c == EOF)
				break;
			//�J���}�����s�łȂ���΁A�����Ƃ��ĂȂ���
			if (c != ',' && c != '\n')
				strcat(aData, (const char*)&c);
			//�J���}�����s�Ȃ烋�[�v������
			else
				break;
		}
		switch (column)
		{
			//atoi�֐��Ő��l�Ƃ��đ��
		case 1:	g_aEnemySet[line].nType = atoi(aData);				break;	//�P��ځF�G�E���
		case 2:	g_aEnemySet[line].in_time = atoi(aData);			break;	//�@��ځF�o������
		case 3:	g_aEnemySet[line].pos.x = (float)(atoi(aData));	break;	//�@��ځFX���W
		case 4:	g_aEnemySet[line].pos.y = (float)(atoi(aData));	break;	//�@��ځFY���W
		case 5:	g_aEnemySet[line].nAngle = (float)(atoi(aData));	break;	//�@��ځF�p�x
		}
		//�o�b�t�@��������
		memset(aData, 0, sizeof(aData));
		//�񐔂𑫂�
		column++;
		//�����ǂݍ��񂾕��������s��������񐔂����������čs���𑝂₷
		if (c == '\n')
		{
			column = 1;
			line++;
			g_nEnemyNum++;
		}
	}
	//�t�@�C�������
	fclose(pFile);
}
//==========================================
//
//�v���C���[�֌W�v���O����[player.cpp]
//Author:�Ό��D�n
//
//==========================================

//==========================================
//
//����
//�E�e�N�X�`���ǂݍ��ݕ����� "�ق��ق�" ���g���e�N�X�`���ɒ���
//�EANIM_PATT_JUMP��ANIM_PATT_ROTATE�̔ԍ����e�N�X�`���ɉ����Ē���
//
//==========================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "chr_enemy_00.h"
#include "physics.h"
#include "sound.h"

//�}�N��
//�^���֌W
#define ACCELERATION_GRAVITY	(20.8f)				//�d�͉����x
#define INITIAL_VELOCITY		(12)				//����
#define ROT_SPEED				(D3DX_PI * 6.0f)	//��]���x
#define TURN_TIME				(45)				//��]���ԁi�t���[���j
#define ATTACK_HIT_TEST_RADIUS	(32.0f)				//�U���̓����蔻��̔��a
#define INVINCIBLE_TIME			(60)				//���G����

//�A�j���[�V�����֌W
#define PLAYER_PATTERN_WIDTH	(4)					//�p�^�[����
#define PLAYER_PATTERN_HEIGHT	(2)					//�p�^�[������
#define ANIM_SPEED				(8)					//�A�j���[�V�����p�^�[���؂�ւ�����
#define ANIM_PATT_JUMP			(1)					//�W�����v�p�^�[���ԍ�
#define ANIM_PATT_ROTATE		(4)					//��]�p�^�[���ԍ�

//�{�^��
#define PUSH_KEY				DIK_RETURN			//�����{�^��

//�N�[���^�C��
#define CT_TIME					(0)				//�N�[���^�C�����ԁi�t���[��)

// �G[00] �Ƃ̓����蔻��
bool CollisionChr_enemy_00(void);

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxbuffPlayer;	//���_�o�b�t�@�|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayer;		//�e�N�X�`���|�C���^
Player g_Player;
LPD3DXFONT g_pFontPlayer = NULL;			//�t�H���g�ւ̃|�C���^

//========================
//�v���C���[����������
//========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^

	pDevice = GetDevice();	//�f�o�C�X�̎擾

#if 1
	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\chr_knight_000.png",
		&g_pTexturePlayer);
#endif

	//�ړ��֌W������
	g_Player.pos = D3DXVECTOR3(100.0f, 656.0f, 0.0f);

	//��]�֌W������
	//����������
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	//�Ίp���̒����Z�o
	g_Player.fLength = sqrtf(PLAYER_SIZE_WIDTH * PLAYER_SIZE_WIDTH + PLAYER_SIZE_HEIGHT * PLAYER_SIZE_HEIGHT) * 0.5f;

	//�Ίp���̊p�x�Z�o
	g_Player.fAngle = atan2f(PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT);

	//�J�X���ԏ�����
	g_Player.nCounterTurn = 0;

	//�T�C�Y������
	g_Player.fRotSpeed = 0.0f;

	//�̗͏�����
	g_Player.nLife = DEFAULT_LIFE;

	//�_���[�W�J�E���^�[������
	g_Player.nDamageCounter = 0;

	//�A�j���[�V�����֌W������
	g_Player.nCounterAnim = 0;		//�J�E���^������
	g_Player.nPatternAnim = 0;		//�p�^�[��No.������

	//��ԏ�����
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.bTurn = false;

	//�^���֌W������
	g_Player.fSpeed = 0.0f;
	g_Player.dwExecSkyLastTime = timeGetTime();

	//�N�[���^�C��������
	g_Player.nCoolTime = 0;

	//�����Ă��邱�Ƃɂ���
	g_Player.bDisp = true;

	//���n�t���O������
	g_Player.bLanding = false;
	g_Player.bChainJump = false;

	//�f�o�b�O�\���p�t�H���g����
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFontPlayer);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxbuffPlayer,
		NULL);

	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	//���W�ϊ��p�W���ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxbuffPlayer->Unlock();
}

//========================
//�v���C���[�I������
//========================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
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
	DWORD dwCurrentTime = timeGetTime();

	//�J�E���^���Z
	g_Player.nCounterAnim++;
	g_Player.nCoolTime--;
	if (g_Player.nDamageCounter > 0) {
		g_Player.nDamageCounter--;
	}


	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:	//�ʏ펞
		if (GetKeyboardTrigger(PUSH_KEY) == true)
		{//�X�y�[�X�L�[�������ꂽ
			g_Player.nPatternAnim = ANIM_PATT_JUMP;		//�A�j���[�V�����p�^�[�����W�����v�p�̂��̂ɂ���
			g_Player.state = PLAYERSTATE_JUMP;			//�W�����v���Ă����Ԃɂ���
			g_Player.dwExecSkyLastTime = dwCurrentTime;	//�W�����v�J�n�̎��Ԏ擾
			g_Player.bLanding = false;					//���n�t���O���U
			PlaySound(SOUND_LABEL_SE_JUMP_000);
		}

		//�A�j���[�V����
		if ((g_Player.nCounterAnim % ANIM_SPEED) == 0)
		{
			g_Player.nCounterAnim = 0;	//�J�E���^�����l�ɖ߂�

			//�p�^�[��No�X�V
			g_Player.nPatternAnim = (g_Player.nPatternAnim + 1) % 4;
		}

		//���R����
		g_Player.fSpeed = -ACCELERATION_GRAVITY * (dwCurrentTime - g_Player.dwExecSkyLastTime) / 1000;
		break;
	case PLAYERSTATE_DAMAGE:	//�_���[�W��
		break;
	case PLAYERSTATE_JUMP:
		//���x�ݒ�
		g_Player.fSpeed = -ACCELERATION_GRAVITY * (dwCurrentTime - g_Player.dwExecSkyLastTime) / 1000 + INITIAL_VELOCITY;

		if (GetKeyboardTrigger(PUSH_KEY) == true &&
			!g_Player.bTurn &&
			(g_Player.nCoolTime <= 0
				||
				g_Player.bChainJump))
		{//�X�y�[�X�L�[�������ꂽ
			g_Player.nPatternAnim = ANIM_PATT_ROTATE;		//�A�j���[�V�����p�^�[������]�p�̂��̂ɂ���
			g_Player.bTurn = true;							//��]���Ă����Ԃɂ���
			g_Player.bChainJump = false;					//2�x�ڈȍ~�̃W�����v�t���O
			PlaySound(SOUND_LABEL_SE_SLASH_000);
		}

		if (g_Player.fSpeed <= 0.0f)
		{//���x��0�ɂȂ���
			g_Player.state = PLAYERSTATE_FREEFALL;			//�������Ă����Ԃɂ���
			g_Player.dwExecSkyLastTime = dwCurrentTime;		//�����J�n�̎��Ԏ擾
		}

		break;
	case PLAYERSTATE_FREEFALL:
		if (GetKeyboardTrigger(PUSH_KEY) == true && 
			!g_Player.bTurn && 
			(g_Player.nCoolTime <= 0
				||
				g_Player.bChainJump))
		{//�X�y�[�X�L�[�������ꂽ
			g_Player.nPatternAnim = ANIM_PATT_ROTATE;		//�A�j���[�V�����p�^�[������]�p�̂��̂ɂ���
			g_Player.bTurn = true;							//��]���Ă����Ԃɂ���
			g_Player.bChainJump = false;					//2�x�ڈȍ~�̃W�����v�t���O
			PlaySound(SOUND_LABEL_SE_SLASH_000);
		}

		//���R����
		g_Player.fSpeed = -ACCELERATION_GRAVITY * (dwCurrentTime - g_Player.dwExecSkyLastTime) / 1000;
		break;
	}

	//��]
	if (g_Player.bTurn)
	{
		g_Player.nCounterTurn++;
		//��]
		g_Player.rot.z -= ROT_SPEED / TURN_TIME;

		if (g_Player.nCounterTurn >= TURN_TIME
			||
			g_Player.bLanding)
		{//���Ԍo��
			g_Player.rot.z = 0.0f;
			g_Player.bTurn = false;
			g_Player.nCounterTurn = 0;
			g_Player.nCoolTime = CT_TIME;
			g_Player.bChainJump = false;
		}

		if (CollisionChr_enemy_00()) 
		{// �G�Ƃ̓����蔻�肪�^�̎��A
			g_Player.state = PLAYERSTATE_JUMP;			//�W�����v���Ă����Ԃɂ���
			g_Player.dwExecSkyLastTime = dwCurrentTime;	//�W�����v�J�n�̎��Ԏ擾
			g_Player.bChainJump = true;					//2�x�ڈȍ~�̃W�����v�t���O
		}
	}

	//�����蔻��
	if (g_Player.pos.y + ((PLAYER_SIZE_HEIGHT / 2 - PLAYER_SIZE_HEIGHT / 2) * fabsf(cosf(g_Player.rot.z))) + PLAYER_SIZE_HEIGHT / 2 >= SCREEN_HEIGHT - GROUND_HEIGHT
		&& g_Player.state != PLAYERSTATE_JUMP)
	{//�n�ʂɒ��n
		g_Player.fSpeed = 0.0f;					//���x0�ɂ���
		g_Player.state = PLAYERSTATE_NORMAL;	//�ʏ��Ԃɖ߂�

		if (!g_Player.bLanding) 
		{// ���n�t���O���U�̎��A
			// ���n��^�ɂ���
			g_Player.bLanding = true;

			//�p�^�[���ʏ�p�ɖ߂�
			g_Player.nPatternAnim = 0;
		}

		//�ʒu��n�ʂɍ��킹��
		g_Player.pos.y = SCREEN_HEIGHT - GROUND_HEIGHT - ((PLAYER_SIZE_HEIGHT / 2 - PLAYER_SIZE_HEIGHT / 2) * fabsf(cosf(g_Player.rot.z)) + PLAYER_SIZE_HEIGHT / 2);
	}

	//�ʒu�X�V
	g_Player.pos.y -= g_Player.fSpeed;

	//��]����
	g_Player.rot.z = (float)fmod(g_Player.rot.z + D3DX_PI + (D3DX_PI * 2) + g_Player.fRotSpeed, D3DX_PI * 2) - D3DX_PI;

#if 0
	//�g�k
	if (GetKeyboardPress(DIK_UP) == true)
	{
		g_Player.move.z += LENGTH_AD;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{
		g_Player.move.z -= LENGTH_AD;
	}

	//�g�k���f
	g_Player.fLength += g_Player.move.z;

	//�T�C�Y����
	g_Player.move.z += (0 - g_Player.move.z) * DUMP_COEF;
#endif

	//�e�N�X�`�����W�̍X�V
	VERTEX_2D *pVtx;	//�ݒ�p�|�C���^

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	g_pVtxbuffPlayer->Lock(0, 0, (void **)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (-D3DX_PI + g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0 + g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((float)(g_Player.nPatternAnim % PLAYER_PATTERN_WIDTH) / PLAYER_PATTERN_WIDTH,
		(float)(g_Player.nPatternAnim / PLAYER_PATTERN_WIDTH) / PLAYER_PATTERN_HEIGHT);
	pVtx[1].tex = D3DXVECTOR2((float)(g_Player.nPatternAnim % PLAYER_PATTERN_WIDTH + 1) / PLAYER_PATTERN_WIDTH,
		(float)(g_Player.nPatternAnim / PLAYER_PATTERN_WIDTH) / PLAYER_PATTERN_HEIGHT);
	pVtx[2].tex = D3DXVECTOR2((float)(g_Player.nPatternAnim % PLAYER_PATTERN_WIDTH) / PLAYER_PATTERN_WIDTH,
		(float)(g_Player.nPatternAnim / PLAYER_PATTERN_WIDTH + 1) / PLAYER_PATTERN_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2((float)(g_Player.nPatternAnim % PLAYER_PATTERN_WIDTH + 1) / PLAYER_PATTERN_WIDTH,
		(float)(g_Player.nPatternAnim / PLAYER_PATTERN_WIDTH + 1) / PLAYER_PATTERN_HEIGHT);

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f - ((float)g_Player.nDamageCounter / (float)DAMAGE_TIME), 1.0f);

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

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);	//g_pTexturePlayer

	if (g_Player.bDisp)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//========================
// �G[00] �Ƃ̓����蔻��
//========================
bool CollisionChr_enemy_00(void) 
{
	Chr_enemy_00 *pChr = GetChr_enemy_00();	// �G[00] �̏��̎擾

	for (int nCntChr = 0; nCntChr < CHR_ENEMY_00_MAX; nCntChr++, pChr++) 
	{
		if (!pChr->bUse) 
		{// �g�p����Ă��Ȃ���Ԃ̎��A
			// �J��Ԃ�������܂�Ԃ�
			continue;
		}

		if (CheckHit(
			pChr->pos,
			pChr->rot,
			pChr->fHitTestWidth * pChr->scale.fWidth,
			pChr->fHitTestHeight * pChr->scale.fHeight,
			HIT_TEST_TYPE_CIRCLE,
			GetPlayer()->pos,
			GetPlayer()->rot,
			ATTACK_HIT_TEST_RADIUS,
			0.0f,
			HIT_TEST_TYPE_CIRCLE))
		{// �����蔻����ɓ��������A
			// �_���[�W��^����
			HitChr_enemy_00(pChr, 1);

			return true;
		}
	}
	
	return false;
}

//========================
//�v���C���[���擾
//========================
Player *GetPlayer(void)
{
	return &g_Player;
}
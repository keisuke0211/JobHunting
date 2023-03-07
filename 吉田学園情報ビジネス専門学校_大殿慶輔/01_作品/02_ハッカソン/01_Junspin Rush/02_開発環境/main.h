//========================================
// *** main.h ***
//========================================
#ifndef _MAIN_H_	// ���̃}�N����`������Ă��Ȃ�������
#define _MAIN_H_	// ��d�C���N���[�h�h�~�̃}�N�����`����

#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)	// �r���h���̌x���Ώ��p�}�N��
#include "dinput.h"						// ���͏����ɕK�v
#include "Xinput.h"						// ���͏����ɕK�v
#include "xaudio2.h"					// �T�E���h�����ɕK�v

//****************************************
// ���C�u�����̃����N
//****************************************
#pragma comment(lib,"d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	// DirectX�R���|�[�l���g���g�p����̂ɕK�v
#pragma comment(lib,"dsound.lib")	// DirectSound���g�p����̂ɕK�v
#pragma comment(lib,"winmm.lib")	// �V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	// ���͏����ɕK�v
#pragma comment(lib,"xinput.lib")	// ���͏����ɕK�v
#pragma warning(disable:4091)		// �x��:C4091�𖳎�����
#pragma warning(disable:4244)		// �x��:C4244�𖳎�����

//****************************************
// �}�N����`
//****************************************
#define CLASS_NAME			"WindowClass"								// �E�C���h�E�N���X
#define SCREEN_WIDTH		(1280)										// �E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)										// �E�C���h�E�̍���
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	// ���_�t�H�[�}�b�g(2D)
#define TXT_MAX				(256)										// ������̍ő吔(�ėp)
#define PIXEL				(4.0f)										// 1�h�b�g�̑傫��
#define SCORE_PATT_OFFSET	(15)	//�e�N�X�`���p�^�[���̂��꒲��
#define PATT_NUM_SPACE		(8 * 0 + 0)	//�X�y�[�X�̏ꏊ
#define PATT_NUM_C			(8 * 4 + 2)	//C�̏ꏊ
#define PATT_NUM_D			(8 * 4 + 3)	//C�̏ꏊ
#define PATT_NUM_E			(8 * 4 + 4)	//E�̏ꏊ
#define PATT_NUM_H			(8 * 4 + 7)	//H�̏ꏊ
#define PATT_NUM_N			(8 * 5 + 5)	//N�̏ꏊ
#define PATT_NUM_O			(8 * 5 + 6)	//O�̏ꏊ
#define PATT_NUM_R			(8 * 6 + 1)	//R�̏ꏊ
#define PATT_NUM_S			(8 * 6 + 2)	//S�̏ꏊ
#define PATT_NUM_T			(8 * 6 + 3)	//T�̏ꏊ
#define PATT_NUM_U			(8 * 6 + 4)	//U�̏ꏊ
#define PATT_NUM_Y			(8 * 7 + 0)	//Y�̏ꏊ
#define PATT_NUM_COLON		(8 * 3 + 1)	//�R�����̏ꏊ
#define PATT_NUM_B				(8 * 4 + 1)	//B�̏ꏊ
#define PATT_NUM_M				(8 * 5 + 4)	//M�̏ꏊ

//****************************************
// �񋓌^�̒�`
//****************************************

// ���(���[�h)�̎��
typedef enum
{
	MODE_TITLE_N1 = 0,	// ����
	MODE_TITLE_00,			// �^�C�g�����[00]
	MODE_GAME_00,			// �Q�[�����[00]
	MODE_RESULT_00,			// ���U���g���[00]
	MODE_MAX
}MODE;

//****************************************
// �\���̂̒�`
//****************************************

// ���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float rhw;			// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_2D;

// �傫���̔{���\����
typedef struct
{
	float fWidth;	// ��
	float fHeight;	// ����
}ScaleDiameter;

// �F�\����
typedef struct 
{
	int nR;
	int nG;
	int nB;
	int nA;
}Color;

//****************************************
// �v���g�^�C�v�錾
//****************************************
LPDIRECT3DDEVICE9 GetDevice	(void);											// �f�o�C�X���擾
HWND *GetWindowHandle		(void);											// �E�C���h�E�n���h�����擾
D3DXVECTOR3 GetWindowPos	(void);											// �E�C���h�E�̈ʒu���擾
void SetMode				(MODE mode);									// ���[�h�̐ݒ�
MODE GetMode				(void);											// ���[�h�̎擾
void WindowModeChange		(bool bFullScreen);								// �E�C���h�E�̃��[�h��؂�ւ���
bool Lottery				(float fProb);									// ���I����
float fRand					(float fMax);									// �����̗�����Ԃ�
void MatchPosition			(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos);	// �`��ʒu����ɍ��킹��
int PrusMinus				(float fNum);									// �^����ꂽ�l�ɉ�����1��-1��Ԃ�
void strinit				(char *pData, int nNum);						// �󔒖���

#endif
//========================================
// 
// ���C���̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
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
// �E�C���h�E�N���X
// �E�C���h�E�̕�
// �E�C���h�E�̍���
// ���_�t�H�[�}�b�g(2D)
// ���_�t�H�[�}�b�g(3D)
// ������̍ő吔(�ėp)
// �f�o�b�O�R�����g�g�p�t���O
#define CLASS_NAME		"WindowClass"
#define SCREEN_WIDTH	(1280)	// NORMAL=1280 4:3=960
#define SCREEN_HEIGHT	(720)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define TXT_MAX			(256)
#define _DEBUG_COMMENT	(false)

// 1�s�N�Z���̑傫��
// 1�s�N�Z���̑傫��(3D)
#define PIXEL	(3.0f)
#define PIXEL3D	(0.4f)

// �����X�N���[���̍��[
// �����X�N���[���̉E�[
#define INSIDE_SCREEN_LEFTMOST	(160)
#define INSIDE_SCREEN_RIGHTMOST	(1120)

// D3DXVECTOR3�̏������l
// Color�̏������l
#define INITD3DXVECTOR3	D3DXVECTOR3(0.0f,0.0f,0.0f)
#define INITCOLOR		Color{255,255,255,255}

//****************************************
// �񋓌^�̒�`
//****************************************
// ���(���[�h)�̎��
typedef enum
{
	MODE_NONE,			// ����
	MODE_GAME_00,		// �Q�[�����			[00]
	MODE_TITLE_00,		// �^�C�g�����			[00]
	MODE_RESULT_00,		// ���U���g���			[00]
	MODE_TUTORIAL_00,	// �`���[�g���A�����	[00]
	MODE_MAX
}MODE;

//****************************************
// �\���̂̒�`
//****************************************
// ���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float		rhw;	// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_2D;

// ���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_3D;

// �g��{���\����
typedef struct 
{
	float fWidth;	// ��
	float fHeight;	// ����
	float fDepth;	// ���s��
}Scale;

// �F�\����
typedef struct 
{
	int r;
	int g;
	int b;
	int a;
}Color;

// ���C�������̏��\����
typedef struct 
{
	LPDIRECT3D9			pD3D		// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
						= NULL;
	LPDIRECT3DDEVICE9	pD3DDevice	// Direct3D�f�o�C�X�ւ̃|�C���^
						= NULL;
	HWND				hWnd;		// �E�C���h�E�n���h��(���ʎq)
	MODE				mode		// ���݂̃��[�h
						= MODE_NONE;
	D3DXVECTOR3			windowPos;	// �E�C���h�E�̈ʒu
	bool				bPause		// �|�[�Y�t���O
						= false;
	Color				bgCol;		// �w�i�F
#if _DEBUG_COMMENT
	LPD3DXFONT			pFont		// �t�H���g�ւ̃|�C���^
						= NULL;
	int					nCountFPS	// FPS�̃J�E���^�[
						= 0;
	bool				bDebug		// �f�o�b�O���[�h��ON/OFF
						= true;
	int					nCommentY;	// �R�����g��Y���W
	Color				commentCol;	// �R�����g�F
#endif
}Main;

//****************************************
// �v���g�^�C�v�錾
//****************************************
// ���C�������̏��̎擾
// �f�o�C�X���擾
// �E�C���h�E�n���h�����擾
// �E�C���h�E�̈ʒu���擾
// ���[�h�̎擾
Main *GetMain(void);
LPDIRECT3DDEVICE9 GetDevice(void);
HWND *GetWindowHandle(void);
D3DXVECTOR3 GetWindowPos(void);
MODE GetMode(void);

// ���[�h�̐ݒ�
void SetMode(MODE mode);

// �E�C���h�E�̃��[�h��؂�ւ���
void WindowModeChange(bool bFullScreen);

#if _DEBUG_COMMENT
// �R�����g�̕`�揈��
// �R�����g�̐F�ύX����
void DrawComment(char *pString);
void CommentColorChange(Color col);
#endif

// ���b�Z�[�W����
void Message(char *pText, char *pCaption);

#endif
//========================================
// 
// ���C������
// Author:RIKU NISHIMURA
// 
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "camera.h"
#include "color.h"
#include "fade.h"
#include "input.h"
#include "light.h"
#include "text.h"
#include "sound.h"
#include "attack.h"			// ATTACK
#include "back-ground.h"	// BACKGROUND
#include "character.h"		// CHARACTER
#include "effect.h"			// EFFECT
#include "item.h"			// ITEM
#include "mode.h"			// MODE
#include "system.h"			// SYSTEM
#include "object.h"			// OBJECT
#include "user-interface.h"	// USERINTERFACE
#include "md_game_00.h"		// MD :�Q�[��			[00]
#include "md_result_00.h"	// MD :���U���g			[00]
#include "md_title_00.h"	// MD :�^�C�g��			[00]
#include "md_tutorial_00.h"	// MD :�`���[�g���A��	[00]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// �}�N����`
//****************************************
// �E�C���h�E�̖��O
#define WINDOW_NAME	""

// �����̃��[�h
#define INIT_MODE	(MODE_TITLE_00)

// �|�[�Y�L�[
// �f�o�b�O�L�[
#define PAUSE_KEY	(DIK_P)
#define DEBUG_KEY	(DIK_F1)

//****************************************
// �v���g�^�C�v�錾
//****************************************
// �E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ����������
// �I������
// �X�V����
// �`�揈��
// �ǂݍ��ݏ���
// �������ݏ���
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void Load(void);
void Save(void);

// ���[�h�ɉ���������������
// ���[�h�ɉ������I������
// ���[�h�ɉ������X�V����
// ���[�h�ɉ������`�揈��
void ModeInit(MODE mode);
void ModeUninit(MODE mode);
void ModeUpdate(MODE mode);
void ModeDraw(MODE mode);

// �f�o�b�O�R�����g�̕`�揈��
void DrawDebugComment(void);

// �E�C���h�E�̕\��/��\����؂�ւ���
void ShowTaskBar(bool bShow);

//****************************************
// �O���[�o���錾
//****************************************
Main	g_main;	// ���C�������̏��\����

//========== *** ���C�������̏��̎擾 ***
Main *GetMain(void) 
{
	return &g_main;
}

//========== *** �f�o�C�X�̎擾 ***
LPDIRECT3DDEVICE9 GetDevice(void) 
{
	return g_main.pD3DDevice;
}

//========== *** �E�C���h�E�n���h�����擾 ***
HWND *GetWindowHandle(void) 
{
	return &g_main.hWnd;
}

//========== *** �E�C���h�E�̈ʒu���擾 ***
D3DXVECTOR3 GetWindowPos(void)
{
	return g_main.windowPos;
}

//========== *** ���[�h�̎擾 ***
MODE GetMode(void)
{
	return g_main.mode;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| cpp�t�@�C���̐�L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// WinMain�֐�
// Author:RIKU NISHIMURA
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	MSG msg;				// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExecLastTime;	// �Ō�ɏ�����������
#if _DEBUG_COMMENT
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS���v����������
#endif

	srand((unsigned int)time(0)); // �����̎���擾

	// �E�C���h�E�N���X�̍\����
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						// �E�C���h�E�̃X�^�C��
		WindowProc,						// �E�C���h�E�v���V�[�W��
		0,								// 0�ɂ���
		0,								// 0�ɂ���
		hInstance,						// �C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),		// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		// �N���C�A���g�̈�̔w�i�F
		NULL,							// ���j���[�o�[
		CLASS_NAME,						// �E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	// �t�@�C���̃A�C�R��
	};

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�C���h�E�𐶐�
	g_main.hWnd = CreateWindowEx(
		0,								// �g���E�C���h�E�X�^�C��
		CLASS_NAME,						// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,					// �E�C���h�E�̖��O
		WS_SYSMENU | WS_MINIMIZEBOX,	// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,					// �E�C���h�E�̍���X���W
		CW_USEDEFAULT,					// �E�C���h�E�̍���Y���W
		SCREEN_WIDTH,					// �E�C���h�E�̕�
		SCREEN_HEIGHT,					// �E�C���h�E�̍���
		NULL,							// �e�E�C���h�E�̃n���h��
		NULL,							// ���j���[�n���h���܂���
		hInstance,						// �C���X�^���X�n���h��
		NULL);							// �E�C���h�E�쐬�f�[�^

	// ����������
	if (FAILED(Init(hInstance, g_main.hWnd, TRUE)))
	{// ���������������s�����ꍇ

		return -1;
	}

	{
		bool bFull = true;	// �t���X�N�t���O

#if _DEBUG
		bFull = false;
#endif
		// �E�C���h�E�̕\��
		WindowModeChange(bFull);
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0; // ���ݎ���������������
	dwExecLastTime = timeGetTime(); // ���ݎ������擾(�ۑ�)

#if _DEBUG_COMMENT
	dwFrameCount = 0;	// �t���[���J�E���g������������
	dwFPSLastTime = timeGetTime();	// ���ݎ������擾(�ۑ�)
#endif

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���

				break;
			}
			else
			{
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime(); // ���ݎ������擾

#if _DEBUG_COMMENT
			if ((dwCurrentTime - dwFPSLastTime) >= 500) 
			{// 0.5�b�o��
				// FPS���v��
				g_main.nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime; // FPS�𑪒肵��������ۑ�
				dwFrameCount = 0; // �t���[���J�E���g���N���A
			}
#endif

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) 
			{// 60/1�b�o��
				dwExecLastTime = dwCurrentTime; // �����J�n�̎���[���ݎ���]��ۑ�

				{
					RECT rec;	// �傫�����󂯎��RECT�\����

					// �E�C���h�E�̏����擾
					GetWindowRect(g_main.hWnd, &rec);

					// �E�C���h�E�̈ʒu��ݒ�
					g_main.windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);
				}

				// �X�V����
				Update();

				// �`�揈��
				Draw();

#if _DEBUG_COMMENT
				dwFrameCount++; // �t���[���J�E���g�����Z
#endif
			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//========================================
// ShowTaskBar�֐� - �^�X�N�o�[�̕\��/��\����؂�ւ��� -
// Author:RIKU NISHIMURA
// �Q�l: https://dixq.net/forum/viewtopic.php?t=12505
//========================================
void ShowTaskBar(bool bShow)
{
	HWND hTask;	// �^�X�N�o�[�̃n���h��

	// �^�X�N�o�[�̃n���h�����擾
	hTask = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (bShow)
	{// �\��
		// �^�X�N�o�[��\��
		ShowWindow(hTask, SW_SHOW);

		// �E�C���h�E���A�N�e�B�u�ɂ���
		SetFocus(g_main.hWnd);
	}
	else 
	{// ��\��
		// �^�X�N�o�[���\��
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================================
// WindowProc�֐�
// Author:RIKU NISHIMURA
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;	// �Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY: // �E�C���h�E�j���̃��b�Z�[�W

		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�𑗂�

		break;
	case WM_KEYDOWN: // �L�[�����̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE: // [ESC]�L�[�������ꂽ

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO); // �E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)

			if (nID == IDYES)
			{
				DestroyWindow(hWnd); // �E�C���h�E��j������
			}

			break;
		}

		break;
	case WM_CLOSE: // �I���{�^���������ꂽ��

		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO); // �E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)

		if (nID == IDYES)
		{
			DestroyWindow(hWnd); // �E�C���h�E��j������
		}
		else
		{
			return 0; // (��0��Ԃ��Ȃ��ƏI�����Ă��܂�)
		}

		break;
	case WM_LBUTTONDOWN: // �}�E�X�N���b�N�̃��b�Z�[�W

		// �ΏۃE�C���h�E�Ƀt�H�[�J�X�����킹��
		SetFocus(hWnd); // �}�E�X�����N���b�N�����E�C���h�E���A�N�e�B�u�ɂ���
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // ����̏�����Ԃ�
}

//========================================
// Init�֐� - ���������� -
// Author:RIKU NISHIMURA
//========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �w�i�F��������
	g_main.bgCol = INITCOLOR;

	// Direct3D�I�u�W�F�N�g�̐���
	g_main.pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_main.pD3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;	// �f�B�X�v���C���[�h

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_main.pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth				= SCREEN_WIDTH;					// �Q�[���̉�ʃT�C�Y(��)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;				// �Q�[���̉�ʃT�C�Y(����)
	d3dpp.BackBufferFormat				= d3ddm.Format;					// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount				= 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// �_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil		= TRUE;							// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed						= bWindow;						// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���
	if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_main.pD3DDevice)))
	{
		if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_main.pD3DDevice)))
		{
			if (FAILED(g_main.pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_main.pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

#if _DEBUG_COMMENT
	// �t�H���g�̐���
	D3DXCreateFont(g_main.pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_main.pFont);
#endif

	// �����_�[�X�e�[�g�̐ݒ�
	g_main.pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_main.pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_main.pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_main.pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�ݒ�
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_main.pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_main.pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �A���t�@�u�����h�̐ݒ�
	g_main.pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_main.pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_main.pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �ǂݍ��ݏ���
	Load();

	// ����������
	InitCamera();			// �J����
	InitSound(hWnd);		// �T�E���h
	InitLight();			// ��
	InitText();				// �e�L�X�g
	InitFade(INIT_MODE);	// �t�F�[�h

	// ���͊֘A�̏���������
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// Uninit�֐� - �I������ -
// Author:RIKU NISHIMURA
//========================================
void Uninit(void)
{
	// �������ݏ���
	Save();

	// �I������
	UninitInput();			// ���͊֘A
	UninitLight();			// ��
	UninitText();			// �e�L�X�g
	UninitSound();			// �T�E���h
	UninitFade();			// �t�F�[�h

#if _DEBUG_COMMENT
	// �t�H���g�̔j��
	if (g_main.pFont != NULL)
	{
		g_main.pFont->Release();
		g_main.pFont = NULL;
	}
#endif

	// Direct3D�f�o�C�X�̔j��
	if (g_main.pD3DDevice != NULL)
	{
		g_main.pD3DDevice->Release();
		g_main.pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_main.pD3D != NULL)
	{
		g_main.pD3D->Release();
		g_main.pD3D = NULL;
	}

	// �^�X�N�o�[��\��
	ShowTaskBar(true);
}

//========================================
// Update�֐� - �X�V���� - 
// Author:RIKU NISHIMURA
//========================================
void Update(void)
{
	// �X�V����
	UpdateInput(g_main.bPause);	// ���͊֘A
	ModeUpdate(g_main.mode);	// ���[�h
	UpdateCamera();				// �J����
	UpdateLight();				// ��
	UpdateFade();				// �t�F�[�h

#if _DEBUG_COMMENT
	if (GetKeyboardTrigger(DEBUG_KEY))
	{// �f�o�b�O�L�[�����������A
		// �f�o�b�O�\����ON/OFF��؂�ւ���
		g_main.bDebug ^= 1;
	}
	if (GetKeyboardTrigger(PAUSE_KEY))
	{// �|�[�Y�L�[�����������A
		// �|�[�Y�t���O��ON/OFF��؂�ւ���
		g_main.bPause ^= 1;
	}
#endif
}

//========================================
// Draw�֐� - �`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void Draw(void)
{
	// ��ʃN���A
	g_main.pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), ColorToD3DCOLOR(g_main.bgCol), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(g_main.pD3DDevice->BeginScene()))
	{// �`��J�n�����������ꍇ�A
		SetCamera3D();			// �J����(3D)�̐ݒ菈��
		ModeDraw(g_main.mode);	// ���[�h
		DrawFade();				// �t�F�[�h

#if _DEBUG_COMMENT
		if (g_main.bDebug)
		{// �f�o�b�O�\����ON�̎��A
			// �f�o�b�O�R�����g�̕`�揈��
			DrawDebugComment();
		}
#endif

		// �`��I��
		g_main.pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�̓���ւ�
	g_main.pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// Load�֐� - �ǂݍ��ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void Load(void)
{
	LoadSound();			// �T�E���h
	LoadFont();				// �t�H���g
	LoadAttack();			// ATTACK
	LoadBackGround();		// BACKGROUND
	LoadCharacter();		// CHARACTER
	LoadEffect();			// EFFECT
	LoadSystem();			// SYSTEM
	LoadItem();				// ITEM
	LoadMode();				// MODE
	LoadObject();			// OBJECT
	LoadUserInterface();	// USERINTERFACE
}

//========================================
// Save�֐� - �������ݏ��� -
// Author:RIKU NISHIMURA
//========================================
void Save(void)
{
	SaveSystem();	// SYSTEM
}

//========================================
// ModeInit�֐� - ���[�h�ɉ��������������� -
// Author:RIKU NISHIMURA
//========================================
void ModeInit(MODE mode) 
{
	switch (mode)
	{
	case MODE_GAME_00		:InitMd_game_00()		; break;	// MD:�Q�[�����			[00]
	case MODE_TITLE_00		:InitMd_title_00()		; break;	// MD:�^�C�g�����			[00]
	case MODE_RESULT_00		:InitMd_result_00()		; break;	// MD:���U���g���			[00]
	case MODE_TUTORIAL_00	:InitMd_tutorial_00()	; break;	// MD:�`���[�g���A�����	[00]
	}
}

//========================================
// ModeUninit�֐� - ���[�h�ɉ������I������ -
// Author:RIKU NISHIMURA
//========================================
void ModeUninit(MODE mode)
{
	switch (mode)
	{
	case MODE_GAME_00		:UninitMd_game_00()		; break;	// MD:�Q�[�����			[00]
	case MODE_TITLE_00		:UninitMd_title_00()	; break;	// MD:�^�C�g�����			[00]
	case MODE_RESULT_00		:UninitMd_result_00()	; break;	// MD:���U���g���			[00]
	case MODE_TUTORIAL_00	:UninitMd_tutorial_00()	; break;	// MD:�`���[�g���A�����	[00]
	}
}

//========================================
// ModeUpdate�֐� - ���[�h�ɉ������X�V���� -
// Author:RIKU NISHIMURA
//========================================
void ModeUpdate(MODE mode)
{
	switch (mode)
	{
	case MODE_GAME_00		:UpdateMd_game_00()		; break;	// MD:�Q�[�����			[00]
	case MODE_TITLE_00		:UpdateMd_title_00()	; break;	// MD:�^�C�g�����			[00]
	case MODE_RESULT_00		:UpdateMd_result_00()	; break;	// MD:���U���g���			[00]
	case MODE_TUTORIAL_00	:UpdateMd_tutorial_00()	; break;	// MD:�`���[�g���A�����	[00]
	}
}

//========================================
// ModeDraw�֐� - ���[�h�ɉ������`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void ModeDraw(MODE mode)
{
	switch (mode)
	{
	case MODE_GAME_00		:DrawMd_game_00()		; break;	// MD:�Q�[�����			[00]
	case MODE_TITLE_00		:DrawMd_title_00()		; break;	// MD:�^�C�g�����			[00]
	case MODE_RESULT_00		:DrawMd_result_00()		; break;	// MD:���U���g���			[00]
	case MODE_TUTORIAL_00	:DrawMd_tutorial_00()	; break;	// MD:�`���[�g���A�����	[00]
	}
}

#if _DEBUG_COMMENT
//========================================
// DrawDebugComment�֐� - �f�o�b�O�R�����g�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawDebugComment(void)
{
	char	aString	// ������
			[TXT_MAX];

	// �R�����g��Y���W��������
	g_main.nCommentY = 0;

	CommentColorChange({ 0,235,219 });	// �R�����g�F�ݒ�
	sprintf(aString, "FPS�F%d", g_main.nCountFPS); DrawComment(aString);
	sprintf(aString, " �f�o�b�O�\����ON/OFF�@:[ F1 ]"); DrawComment(aString);
	sprintf(aString, " "); DrawComment(aString);
	sprintf(aString, " ���_����@�@�@�@�@�@�@:[ �����L�[ ] or [ �E�{�^�� + �J�[�\���ړ� ] or [ ���X�e�B�b�N ]"); DrawComment(aString);
	sprintf(aString, " �v���C���[����@�@�@�@:[ WASD�L�[ ] or [ �E�X�e�B�b�N ]"); DrawComment(aString);
	sprintf(aString, " �A�C�e���I���@�@�@�@�@:[ Q / E�L�[ ]"); DrawComment(aString);
	sprintf(aString, " �A�C�e���g�p�@�@�@�@�@:[ SPACE�L�[ ]"); DrawComment(aString);

	// �R�����g�`�揈��
	DrawCommentChr_player_00();	// CHR:�v���C���[	[00]
}
#endif

//================================================================================
//----------|---------------------------------------------------------------------
//==========| h�t�@�C���̋��L�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// SetMode�֐� - ���[�h�̐ݒ菈�� -
// Author:RIKU NISHIMURA
//========================================
void SetMode(MODE mode) 
{
	// ���݂̃��[�h�̏I������
	ModeUninit(g_main.mode);

	// ���݂̉��(���[�h)��؂�ւ���
	g_main.mode = mode;

	// �V�������[�h�̏���������
	ModeInit(mode);
}

//========================================
// WindowModeChange�֐� - �E�C���h�E�̃��[�h��؂�ւ��� -
// Author:RIKU NISHIMURA
//========================================
void WindowModeChange(bool bFullScreen)
{
	HDC hDC			// �f�X�N�g�b�v�̃n���h��
		= GetDC(GetDesktopWindow());
	int nScrSizeX	// �f�X�N�g�b�v�̉�ʃT�C�YX
		= GetDeviceCaps(hDC, HORZRES);
	int nScrSizeY	// �f�X�N�g�b�v�̉�ʃT�C�YY
		= GetDeviceCaps(hDC, VERTRES);

	// �f�X�N�g�b�v�̃n���h���������
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bFullScreen)
	{// �S��ʃt���O���^�̎��A
		// �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// �S��ʃt���O���U�̎��A
		// �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			g_main.hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(g_main.hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	}

	// �E�C���h�E�̕\����Ԃ�ݒ�
	ShowWindow(g_main.hWnd, SW_NORMAL);

	// �N���C�A���g�̈���X�V
	UpdateWindow(g_main.hWnd);

	// �^�X�N�o�[��\��/��\��
	ShowTaskBar(bFullScreen ^ 1);
}

#if _DEBUG_COMMENT
//========================================
// DrawComment�֐� - �R�����g�̕`�揈�� -
// Author:RIKU NISHIMURA
//========================================
void DrawComment(char *pString)
{
	RECT	rect;	// �����ݒ肷��l�p�`

	// �e�L�X�g�̕`��
	rect = { 0, g_main.nCommentY , SCREEN_WIDTH, SCREEN_HEIGHT };
	g_main.nCommentY += g_main.pFont->DrawText(NULL, pString, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(g_main.commentCol.r, g_main.commentCol.g, g_main.commentCol.b, 255));
}
#endif

#if _DEBUG_COMMENT
//========================================
// DrawComment�֐� - �R�����g�̐F�ύX���� -
// Author:RIKU NISHIMURA
//========================================
void CommentColorChange(Color col) 
{
	// �R�����g�F����
	g_main.commentCol = col;
}
#endif

//========================================
// Message�֐� - ���b�Z�[�W���� -
// Author:RIKU NISHIMURA
//========================================
void Message(char *pText, char *pCaption)
{
	static int	bMessage	// ���b�Z�[�W�\�������t���O(�������[�v�h�~)
				= false;

	if (bMessage) 
	{// ���b�Z�[�W�\�������t���O
		bMessage = false;	// ���b�Z�[�W�\�������t���O���U�ɂ���
		return;				// �������I������
	}

	if (MessageBox(*GetWindowHandle(), pText, pCaption, MB_OK) == IDOK)
	{// ���b�Z�[�W�{�b�N�X�̕Ԃ�l��OK�̎��A
		// ���b�Z�[�W�\�������t���O��^�ɂ���
		bMessage = true;
	}
}
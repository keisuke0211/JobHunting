//========================================
// 
// ���C������
// Author:���� ����
//  
//========================================
// *** main.cpp ***
//========================================
#include "main.h"
#include "title.h"
#include "camera.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "bg_color.h"
#include "md_game_00.h"
#include "md_game_00.h"
#include "chr_enemy_00.h"
#include "atk_bullet_00.h"
#include "ranking.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//****************************************
// �}�N����`
//****************************************
#define WINDOW_NAME		"Jumspin Rush"		// �E�C���h�E�̖��O
#define INIT_MODE		(MODE_TITLE_00)

//****************************************
// �v���g�^�C�v�錾
//****************************************
LRESULT CALLBACK WindowProc	(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�C���h�E�v���V�[�W��
HRESULT Init				(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);			// ����������
void Uninit					(void);													// �I������
void Update					(void);													// �X�V����
void Draw					(void);													// �`�揈��
void Load					(void);													// �ǂݍ��ݏ���
void Save					(void);													// �������ݏ���
void DrawDebugComment		(void);													// �f�o�b�O�R�����g�̕`�揈��
void ShowTaskBar			(bool bShow);											// �E�C���h�E�̕\��/��\����؂�ւ���

//****************************************
// �O���[�o���錾
//****************************************
LPDIRECT3D9 g_pD3D = NULL;				// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3D�f�o�C�X�ւ̃|�C���^
HWND g_hWnd;							// �E�C���h�E�n���h��(���ʎq)
LPD3DXFONT g_pFont = NULL;				// �t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;					// FPS�̃J�E���^�[
bool g_bDebug = true;					// �f�o�b�O�\����ON/OFF
MODE g_mode = MODE_TITLE_N1;			// ���݂̃��[�h
D3DXVECTOR3 g_windowPos;				// �E�C���h�E�̈ʒu
bool g_bPause = false;					// �|�[�Y�t���O

//========== *** �f�o�C�X�̎擾 ***
LPDIRECT3DDEVICE9 GetDevice(void) 
{
	return g_pD3DDevice;
}

//========== *** �E�C���h�E�n���h�����擾 ***
HWND *GetWindowHandle(void) 
{
	return &g_hWnd;
}

//========== *** �E�C���h�E�̈ʒu���擾 ***
D3DXVECTOR3 GetWindowPos(void)
{
	return g_windowPos;
}

//========== *** ���[�h�̎擾 ***
MODE GetMode(void)
{
	return g_mode;
}

//========================================
// WinMain�֐�
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	MSG msg;				// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExecLastTime;	// �Ō�ɏ�����������
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS���v����������

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
	g_hWnd = CreateWindowEx(
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
	if (FAILED(Init(hInstance, g_hWnd, TRUE)))
	{// ���������������s�����ꍇ

		return -1;
	}

	// �E�C���h�E�̕\��
	WindowModeChange(false);

	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0; // ���ݎ���������������
	dwExecLastTime = timeGetTime(); // ���ݎ������擾(�ۑ�)

	dwFrameCount = 0;	// �t���[���J�E���g������������
	dwFPSLastTime = timeGetTime();	// ���ݎ������擾(�ۑ�)

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

			if ((dwCurrentTime - dwFPSLastTime) >= 500) 
			{// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime; // FPS�𑪒肵��������ۑ�
				dwFrameCount = 0; // �t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)) 
			{// 60/1�b�o��
				dwExecLastTime = dwCurrentTime; // �����J�n�̎���[���ݎ���]��ۑ�

				{
					RECT rec;	// �傫�����󂯎��RECT�\����

					// �E�C���h�E�̏����擾
					GetWindowRect(g_hWnd, &rec);

					// �E�C���h�E�̈ʒu��ݒ�
					g_windowPos = D3DXVECTOR3(rec.left, rec.top, 0.0f);
				}

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++; // �t���[���J�E���g�����Z
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
// WindowModeChange�֐� - �E�C���h�E�̃��[�h��؂�ւ��� -
//========================================
void WindowModeChange(bool bFullScreen) 
{
	HDC hDC								// 
		= GetDC(GetDesktopWindow());	// �f�X�N�g�b�v�̃n���h��
	int nScrSizeX						// 
		= GetDeviceCaps(hDC, HORZRES);	// �f�X�N�g�b�v�̉�ʃT�C�YX
	int nScrSizeY						// 
		= GetDeviceCaps(hDC, VERTRES);	// �f�X�N�g�b�v�̉�ʃT�C�YY

	// �f�X�N�g�b�v�̃n���h���������
	ReleaseDC(GetDesktopWindow(), hDC);

	if (bFullScreen) 
	{// �S��ʃt���O���^�̎��A
		// �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			g_hWnd,
			HWND_TOPMOST,
			0,
			0,
			nScrSizeX,
			nScrSizeY,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(g_hWnd, GWL_STYLE, WS_POPUP);
	}
	else
	{// �S��ʃt���O���U�̎��A
		// �E�C���h�E�̈ʒu��ݒ�
		SetWindowPos(
			g_hWnd,
			HWND_TOP,
			(nScrSizeX * 0.5f) - (SCREEN_WIDTH * 0.5f),
			(nScrSizeY * 0.5f) - (SCREEN_HEIGHT * 0.5f),
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SWP_SHOWWINDOW);

		// �E�C���h�E�X�^�C����ύX
		SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	// �E�C���h�E�̕\����Ԃ�ݒ�
	ShowWindow(g_hWnd, SW_NORMAL);

	// �N���C�A���g�̈���X�V
	UpdateWindow(g_hWnd);

	// �^�X�N�o�[��\��/��\��
}

//========================================
// ShowTaskBar�֐� - �^�X�N�o�[�̕\��/��\����؂�ւ��� -
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
		SetFocus(g_hWnd);
	}
	else 
	{// ��\��
		// �^�X�N�o�[���\��
		ShowWindow(hTask, SW_HIDE);
	}
}

//========================================
// WindowProc�֐�
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
//========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;	// �f�B�X�v���C���[�h

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
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
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �ǂݍ��ݏ���
	Load();

	// ����������
	InitCamera();		// �J����
	InitBg_color();		// BG :�w�i�F
	InitSound(hWnd);	// �T�E���h

	ResetRanking();

	// �t�F�[�h�̐ݒ�
	InitFade(INIT_MODE);

	// �L�[�{�[�h�̏���������
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================================
// Uninit�֐� - �I������ -
//========================================
void Uninit(void)
{
	// �������ݏ���
	Save();

	// �I������
	UninitBg_color();	// BG :�w�i�F
	UninitMd_game_00();	// MD :�Q�[�����	[00]
	UninitFade();		// �t�F�[�h
	UninitInput();		// �L�[�{�[�h

	// �T�E���h�̒�~
	StopSound();
	
	// �T�E���h�̏I������
	UninitSound();

	// �t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// �^�X�N�o�[��\��
	ShowTaskBar(true);
}

//========================================
// Update�֐� - �X�V���� - 
//========================================
void Update(void)
{
	UpdateKeyboard();		// �L�[�{�[�h
	UpdateButton();			// �{�^��

	if (!g_bPause)
	{// �|�[�Y�t���O���U�̎��A
		// �X�V����
		UpdateCamera();			// �J����
		UpdateCursor();			// �J�[�\��
		UpdateFade();			// �t�F�[�h
		UpdateMouse();			// �}�E�X
		UpdateStick();			// �X�e�B�b�N
		UpdateVibration();		// �R���g���[���[�̐U��
		UpdateBg_color();		// BG :�w�i�F

		// ���݂̉��(���[�h)�̍X�V����
		switch (g_mode)
		{
			//========== *** �^�C�g�����[00] ***
		case MODE_TITLE_00:
		{
			
			UpdateTitle();

		}
		break;
			//========== *** �Q�[�����[00] ***
		case MODE_GAME_00:
		{
			UpdateMd_game_00();
		}
		break;
			//========== *** ���U���g���[00] ***
		case MODE_RESULT_00:

			UpdateResult();

			break;
		}
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F1))
	{// F1�L�[�����������A
		// �f�o�b�O�\����ON/OFF��؂�ւ���
		g_bDebug ^= 1;
	}
	if (GetKeyboardTrigger(DIK_F2)
		||
		GetButtonTrigger(BUTTON_BACK))
	{// F2�L�[�����������A
		// �|�[�Y�t���O��ON/OFF��؂�ւ���
		g_bPause ^= 1;
	}
#endif
}

//========================================
// Draw�֐� - �`�揈�� -
//========================================
void Draw(void)
{
	{
		Color	bgCol					//
				= GetBg_color()->col;	// �w�i�F

		// ��ʃN���A
		g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(bgCol.nR, bgCol.nG, bgCol.nB, bgCol.nA), 1.0f, 0);
	}

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// �`��J�n�����������ꍇ�A
		// ���݂̉��(���[�h)�̕`�揈��
		switch (g_mode)
		{
			//========== *** �^�C�g�����[00] ***
		case MODE_TITLE_00:

			DrawTitle();

			break;
			//========== *** �Q�[�����[00] ***
		case MODE_GAME_00:

			DrawMd_game_00();

			break;
			//========== *** ���U���g���[00] ***
		case MODE_RESULT_00:

			DrawResult();

			break;
		}

		// �`�揈��
		DrawFade();			// �t�F�[�h

#ifdef _DEBUG
		if (g_bDebug)
		{// �f�o�b�O�\����ON�̎��A
			// �f�o�b�O�R�����g�̕`�揈��
			DrawDebugComment();
		}
#endif

		// �`��I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@���t�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// Load�֐� - �ǂݍ��ݏ��� -
//========================================
void Load(void)
{
	// �ǂݍ��ݏ���
	LoadAtk_bullet_00();
	LoadChr_enemy_00();		// CHR:�G				[00]
	
	// �G�̔z�u���̓ǂݍ���
	LoadEnemy();
}

//========================================
// Save�֐� - �������ݏ��� -
//========================================
void Save(void)
{

}

//========================================
// DrawComment�֐� - �f�o�b�O�R�����g�̕`�揈�� -
//========================================
void DrawDebugComment(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	char aString[TXT_MAX];
	
	sprintf(aString, "FPS�F%d\n", g_nCountFPS);

	g_pFont->DrawText(NULL, aString, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
// SetMode�֐� - ���[�h�̐ݒ� -
//========================================
void SetMode(MODE mode) 
{
	// ���݂̉��(���[�h)�̏I������
	switch (g_mode)
	{
		//========== *** �^�C�g�����[00] ***
	case MODE_TITLE_00:

		UninitTitle();
		
		break;
		//========== *** �Q�[�����[00] ***
	case MODE_GAME_00:
		
		UninitMd_game_00();
		
		break;
		//========== *** ���U���g���[00] ***
	case MODE_RESULT_00:

		UninitResult();

		break;
	}

	// �V�������(���[�h)�̏���������
	switch (mode)
	{
		//========== *** �^�C�g�����[00] ***
	case MODE_TITLE_00:

		InitTitle();

		break;
		//========== *** �Q�[�����[00] ***
	case MODE_GAME_00:

		InitMd_game_00();

		break;
		//========== *** ���U���g���[00] ***
	case MODE_RESULT_00:

		InitResult();

		break;
	}

	// ���݂̉��(���[�h)��؂�ւ���
	g_mode = mode;
}

//========================================
// Lottery�֐� - ���I���� -
//========================================
bool Lottery(float fProb)
{
	if (fProb > (0.1f * (rand() % 1000)))
	{// ���I�������A
		// �^��Ԃ�
		return true;
	}
	else 
	{// ���I���Ȃ��������A
		// �U��Ԃ�
		return false;
	}
}

//========================================
// fRand�֐� - �����̗�����Ԃ� -
//========================================
float fRand(float fMax) 
{
	float fNum;	// �����̒l

	fMax *= 100;

	fNum = rand() % (int)fMax;

	return fNum * 0.01f;
}

//========================================
// MatchPosition�֐� - �`��ʒu����ɍ��킹�� -
//========================================
void MatchPosition(D3DXVECTOR3 *pDrawPos, D3DXVECTOR3 basePos)
{
	// �ʒu���v���C���[�̈ʒu�ɍ��킹��
	pDrawPos->x = pDrawPos->x - basePos.x + (SCREEN_WIDTH * 0.5f);
	pDrawPos->y = pDrawPos->y - basePos.y + (SCREEN_HEIGHT * 0.5f);
}

//========================================
// PrusMinus�֐� - �^����ꂽ�l�ɉ�����1�𕄍��t���ŕԂ� -
//========================================
int PrusMinus(float fNum)
{
	if (fNum == 0) 
	{// ������0���������A
		// 0��Ԃ�
		return 0;
	}

	// 1�𕄍��t���ŕԂ�
	return (fabsf(fNum) / fNum);
}

//========================================
// strinit�֐� - �󔒖��� -
//========================================
void strinit(char *pData, int nNum) 
{
	for (int nCnt = 0; nCnt < nNum; nCnt++) 
	{
		pData[nCnt] = '\0';
	}
}
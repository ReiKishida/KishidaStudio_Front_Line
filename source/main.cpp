//=============================================================================
//
// ���C������ [main.cpp]
// Author : Takuto Ishida
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "debugProc.h"
#include "resource.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"		// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"FrontLine"	// �E�C���h�E�̃L���v�V������
#define WINDOW_MODE		(false)			// �E�B���h�E���[�h(false�Ńt���X�N���[��)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// ���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	// ���������[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// �A�C�R��
	HICON hIcon;
	hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	HICON hIconSml;
	hIconSml = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEX�\���̂̃T�C�Y
		CS_CLASSDC,						// �E�C���h�E�X�^�C��
		WndProc,						// �E�B���h�E�v���V�[�W���̃A�h���X
		0,								// 0�Œ�
		0,								// 0�Œ�
		hInstance,						// WinMain()�̃C���X�^���X�n���h��
		hIcon,							// �g�p����A�C�R��
		LoadCursor(NULL, IDC_ARROW),	// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F
		"MENUID",						// ���j���[
		CLASS_NAME,						// Window�N���X�̖��O
		hIconSml						// �g�p���鏬�����A�C�R��
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// �}�E�X�J�[�\�����B��
	ShowCursor(false);

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	CManager *pManager = NULL;

	pManager = new CManager;

	if (NULL != pManager)
	{
		// ����������
		if (FAILED(pManager->Init(hInstance, hWnd, WINDOW_MODE)))
		{
			return -1;
		}
	}
	else
	{
		MessageBox(0, "�}�l�[�W���[�N���X�̃��������m�ۂł��܂���ł���", "���s", MB_OK);
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �t���[���J�E���g������
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// FPS��
	static int nCntFPS = 0;

	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();	// ���݂̎��Ԃ��擾
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b���ƂɎ��s
#ifdef _DEBUG
				nCntFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60�b�o��
				dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

				if (NULL != pManager)
				{
					// FPS���Z�o
					CDebugProc::Print("FPS�@�@�@�@�@:�@%d\n", nCntFPS);

					// �}�l�[�W���[�̍X�V����
					pManager->Update();

					// �}�l�[�W���[�̕`�揈��
					pManager->Draw();
				}
				else
				{
					MessageBox(0, "���������m�ۂł��Ă��܂���", "���s", MB_OK);
				}

				dwFrameCount++;
			}
		}
	}

	if (pManager != NULL)
	{// �}�l�[�W���[�̔j��
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ����\��߂�
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);	// �E�B���h�E��j������悤�w������
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

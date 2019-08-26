#include	"main.h"
#include	"manager.h"

//#include	"RawInput.h"
//#include "RawInputListeners.h"

#ifdef _DEBUG
// ���������[�N�̂��߂̃C���N���[�h
#include	<crtdbg.h>
#endif // _DEBUG

//static const char* CLASS_NAME = "DX11AppClass";
//static const char* WINDOW_NAME = "Front Core";

static const wchar_t* CLASS_NAME = L"DX11AppClass";
static const wchar_t* WINDOW_NAME = L"Front Line";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HWND g_Window;
static HINSTANCE g_hInstance;

HWND GetWindow()
{
	return g_Window;
}

HINSTANCE GethInstance()
{
	return g_hInstance;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	g_hInstance = hInstance;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		g_hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	g_Window = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
		//WS_POPUPWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		g_hInstance,
		NULL);

	// �E�C���h�E�̕\��(�����������̌�ɍs��)
	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if (false == CManager::Init()) return -1;

	//�t���[���J�E���g������
	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	// ���b�Z�[�W���[�v
	MSG msg;
	while(CManager::Get_GameEnd())
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
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// �X�V����
				CManager::Update();

				// �`�揈��
				CManager::Draw();
			}
		}
	}

	timeEndPeriod(1);				// ����\��߂�

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	CManager::Uninit();

	return (int)msg.wParam;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch(uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					DestroyWindow(hWnd);
					break;
			}
			break;

		default:
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}







///////////////////////////////////////////////////////////////////////////////////////////////

#include <random>

const int Get_Random(int min, int max)
{
	std::random_device rnd;									// �񌈒�I�ȗ���������𐶐�
	std::mt19937_64 mt(rnd());								//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
	std::uniform_int_distribution<> rand(min, max);			// [min, max] �͈͂̈�l����

	return rand(mt);
}

bool Rand_Bool(const double probability) {
	static std::random_device rnd;				// �񌈒�I�ȗ���������𐶐�
	static std::mt19937_64 mt(rnd());			//  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l

	std::bernoulli_distribution uid(probability);
	return uid(mt);
}


float Lerp(const float x1, const float y1, const float x2, const float y2, const float tx)
{
	float dx, dy;

	dx = x2 - x1;
	dy = y2 - y1;

	return ( y1 + (dy / dx) * (tx - x1) );
}

short Loop_Plus(short tx, short max)
{
	return (tx % max);
}

short Loop_Minus(short tx, short max)
{
	return ((tx + max) % max);
}
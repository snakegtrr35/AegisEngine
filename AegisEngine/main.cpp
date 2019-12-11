#include	"main.h"
#include	"manager.h"
#include	"resource.h"

#include	"Timer.h"

#ifdef _DEBUG
// メモリリークのためのインクルード
#include	<crtdbg.h>

#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif // _DEBUG


#ifdef UNICODE
static const wchar_t* CLASS_NAME = L"DX11AppClass";
static const wchar_t* WINDOW_NAME = L"AegisEngine";
#else
static const char* CLASS_NAME = "DX11AppClass";
static const char* WINDOW_NAME = "AegisEngine";
#endif // !UNICODE

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	g_Window = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		//WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
		WS_POPUPWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		//(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		SCREEN_WIDTH,
		//(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// ウインドウの表示(初期化処理の後に行う)
	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	// 初期化処理(ウィンドウを作成してから行う)
	if (false == CManager::Init()) return -1;

	//フレームカウント初期化
	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	// メッセージループ
	MSG msg;
	ZeroMemory(&msg, sizeof msg);

	while(CManager::Get_GameEnd())
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 240))
			{
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				CManager::Update();

				// 描画処理
				CManager::Draw();
			}
		}
	}

	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	CManager::Uninit();

	return (int)msg.wParam;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// ウインドウプロシージャ
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
					//DestroyWindow(hWnd);
					break;
			}
			break;

		default:
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
#include <conio.h>
#include <corecrt_wstdio.h>
#include <tchar.h>
#include <Windows.h>
#include <windowsx.h>

#include "DesktopDefense.h"
#include "DesktopDefenseGameInstance.h"
#include "Audio.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND TryCreateWindow(HINSTANCE _hInstance);

bool MainMessageLoopTick(MSG* _outMsg);

GameInstanceBase* g_GameInstance;
Map* g_Map;
GameObject* g_Player;
Audio* _audio;

bool g_IsClicked = false; // Test Code
bool g_IsDoubleClicked = false; // Test Code

int g_k = 0;
int g_y = 0;



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPWSTR lpCmdLine,
                       _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 윈도우를 생성하고 화면에 표시합니다.
	const HWND _hWnd = TryCreateWindow(hInstance);

	if (_hWnd == nullptr)
		return -1;

	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);

	_audio = Audio_Create(512);
	g_GameInstance = new DesktopDefenseGameInstance(_hWnd);

	MSG _msg;

	while (TRUE)
	{
		if (g_GameInstance->Tick() == true)
			break;

		if (MainMessageLoopTick(&_msg) == true)
			break;
	}

	delete g_GameInstance;

	return static_cast<int>(_msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_GameInstance != nullptr && g_GameInstance->OnWndProc(hWnd, message, wParam, lParam) == true)
		return 0;

	switch (message)
	{
		case WM_CREATE:
		{
			ShowCursor(false);
			// 전역변수로 관리되는 맵 생성 (데스크탑 캡쳐, 블루스크린 및 깨짐 연출용 마젠타 이미지 로드)
			g_Map = new Map(hWnd, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			srand(time(NULL));
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND TryCreateWindow(HINSTANCE _hInstance)
{
	const TCHAR _title[] = _T("Desktop Defense");

	// 윈도우 클래스를 등록합니다.
	WNDCLASS wcex;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(_hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _title;

	RegisterClass(&wcex);

	// 윈도우를 생성합니다.
	/*HWND _hWnd = CreateWindow(_title, _title,
	                          WS_OVERLAPPEDWINDOW, 100, 100, 1280, 1040,
	                          NULL, NULL, _hInstance, NULL);*/

	// 게임 플레이 화면 생성 ..!
	HWND _hWnd = CreateWindow(
		_title,
		_title,
		// 팝업 윈도우 생성
		// (전체 창 모드를 하기 위해 설정)
		WS_POPUP,
		// Window 생성 위치 (x, y)
		// (전체 창 모드를 하기 위해 설정)
		0, 0,
		// GetSystemMetrics : 매개변수로 전달된 값에 해당하는 시스템 설정 정보를 반환하는 함수
		// SM_CXSCREEN : 현재 화면 해상도의 x축 크기 (pixel 단위)를 얻습니다.
		// SM_CYSCREEN : 현재 화면 해상도의 y축 크기 (pixel 단위)를 얻습니다.
		// (전체 창 모드를 하기 위해 설정)
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL, NULL, _hInstance, NULL);

	return _hWnd;
}

bool MainMessageLoopTick(MSG* _outMsg)
{
	// Message
	if (PeekMessage(_outMsg, NULL, 0, 0, PM_REMOVE))
	{
		if (_outMsg->message == WM_QUIT)
			return true;

		DispatchMessage(_outMsg);
	}

	return false;
}
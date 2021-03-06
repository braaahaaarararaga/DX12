
#include "main.h"
#include "renderer.h"


const char* CLASS_NAME = "DX12AppClass";
const char* WINDOW_NAME = "DX12";



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	// ウィンドウクラスの鞠�h
	RegisterClassEx(&wcex);

	// ウィンドウの恬撹
	g_Window = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);


	// 兜豚晒�I尖(ウィンドウを恬撹してから佩う)


	// ウインドウの燕幣(兜豚晒�I尖の瘁に佩う)
	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	// レンダラ�`兜豚晒
	CRenderer renderer;


	//フレ�`ムカウント兜豚晒
	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;


	// メッセ�`ジル�`プ
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が柵ばれたらル�`プ�K阻
				break;
			}
			else
			{
				// メッセ�`ジの鍬�Uとディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				// 厚仟�I尖
				renderer.Update();
				// 宙鮫�I尖
				renderer.Draw();
			}
		}
	}

	timeEndPeriod(1);				// 蛍盾嬬を��す

	// ウィンドウクラスの鞠�hを盾茅
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �K阻�I尖

	return (int)msg.wParam;
}


//=============================================================================
// ウインドウプロシ�`ジャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
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


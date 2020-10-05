
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

	// ������ɥ����饹�ε��h
	RegisterClassEx(&wcex);

	// ������ɥ�������
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


	// ���ڻ��I��(������ɥ������ɤ��Ƥ����Ф�)


	// ������ɥ��α�ʾ(���ڻ��I�������Ф�)
	ShowWindow(g_Window, nCmdShow);
	UpdateWindow(g_Window);

	// ������`���ڻ�
	CRenderer renderer;


	//�ե�`�५����ȳ��ڻ�
	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;


	// ��å��`����`��
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()�����Ф줿���`�׽K��
				break;
			}
			else
			{
				// ��å��`���η��U�ȥǥ����ѥå�
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

				// ���I��
				renderer.Update();
				// �軭�I��
				renderer.Draw();
			}
		}
	}

	timeEndPeriod(1);				// �ֽ��ܤ����

	// ������ɥ����饹�ε��h����
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �K�˄I��

	return (int)msg.wParam;
}


//=============================================================================
// ������ɥ��ץ��`����
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


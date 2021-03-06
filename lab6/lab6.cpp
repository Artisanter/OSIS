#include "stdafx.h"
#include "lab6.h"
#include <vector>
#include <Windows.h>
#include <windowsx.h>
#include <strsafe.h>

#define STR L"THE CAKE IS A LIE"
#define MAX_LOADSTRING 100
#define STARTBUTTON 1001
#define STOPBUTTON 1002
#define TIMERid 42
#define MIN_Y 10

// Глобальные переменные:
HCURSOR cursor;
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HFONT hFont;

void Draw(HDC);
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDC_FINALLAB, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FINALLAB));
	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = cursor;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;
	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
	HWND hWnd = CreateWindowW(szWindowClass, L"Aperture Laboratories", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr); 
	HMENU hMainMenu = CreateMenu();
	HMENU hPopMenu = CreatePopupMenu();
	AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenu, L"Menu");
	AppendMenu(hPopMenu, MF_STRING, STARTBUTTON, L"Start");
	AppendMenu(hPopMenu, MF_STRING, STOPBUTTON, L"Stop");
	SetMenu(hWnd, hMainMenu);
	SetMenu(hWnd, hPopMenu);
	if (!hWnd)
	{
		return FALSE;
	}

	LOGFONT lf;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	StringCchCopy(lf.lfFaceName, LF_FACESIZE, L"Times New Roman");
	lf.lfHeight = 30;
	lf.lfWidth = 10;
	lf.lfWeight = 20;
	lf.lfEscapement = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfItalic = 0;
	
	hFont = CreateFontIndirect(&lf);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd); 
	cursor = LoadCursorFromFileA("portal_gun.cur");
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int max_y;
	static int x;
	static int y = MIN_Y;
	static int offset = 5;

	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case STARTBUTTON:
		{
			SetTimer(hWnd, TIMERid, 20, NULL);
		}
		break;
		case STOPBUTTON:
		{
			KillTimer(hWnd, TIMERid);
		}
		break;
		}
	}
	break;
	case WM_SETCURSOR:
	{
		SetCursor(cursor);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
	{
		static HDC hdc = GetDC(hWnd);
		SelectObject(hdc, hFont);
		TextOut(hdc, x - wcslen(STR) * 5 - 20, y, STR, wcslen(STR));
		if (y < MIN_Y || y > max_y)
			offset *= -1;
		y += offset;
	}
	break;
	case WM_SIZE:
	{
		x = LOWORD(lParam) / 2; //ширина
		max_y = HIWORD(lParam) - 50; //высота
	}
	break;
	case WM_DESTROY:
	{
		KillTimer(hWnd, TIMERid);
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
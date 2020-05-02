#include <Windows.h>


LRESULT CALLBACK WndProc
(
	HWND hWnd,                                           // указатель окна
	UINT uMsg,                                           // идентификатор сообщения
	WPARAM wParam,                                       // первый параметр сообщения
	LPARAM lParam                                        // второй параметр сообщения
)
{
	static HWND hAddButton, hClearButton, hToRightButton, hDeleteButton;
	static HWND hEdit;
	static HWND hLeftListBox, hRightListBox;
	static const int addId = 1, clearId = 2, toRightId = 3, deleteId = 4, editId = 5, leftListId = 6, rightListId = 7;

	HINSTANCE hInstance;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		RECT rect;
		GetWindowRect(hWnd, &rect);
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		auto halfWidth = (rect.right - rect.left) / 2;
		auto halfHeight = (rect.bottom - rect.top) / 2;

		if (!(hLeftListBox = CreateWindow
		(
			"LISTBOX",
			NULL,
			WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_BORDER | LBS_HASSTRINGS,
			0,
			0,
			halfWidth,
			halfHeight,
			hWnd,
			(HMENU)leftListId,
			hInstance,
			NULL
		)))
		{
			exit(-1);
		}
		ShowWindow(hLeftListBox, SW_SHOWNORMAL);

		if (!(hRightListBox = CreateWindow
		(
			"LISTBOX",
			NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD | LBS_HASSTRINGS,
			halfWidth,
			0,
			halfWidth,
			halfHeight,
			hWnd,
			(HMENU)rightListId,
			hInstance,
			NULL
		)))
		{
			exit(-1);
		}
		ShowWindow(hRightListBox, SW_SHOWNORMAL);

		if (!(hEdit = CreateWindow
		(
			"EDIT",
			NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
			0,
			halfHeight,
			halfWidth * 2,
			halfHeight/4,
			hWnd,
			(HMENU)editId,
			hInstance,
			NULL
		)))
		{
			exit(-1);
		}
		ShowWindow(hEdit, SW_SHOWNORMAL);

		if (!(hAddButton = CreateWindow
		(
			"BUTTON",
			"Add",
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
			0,
			halfHeight/4 * 5,
			halfWidth,
			halfHeight/4,
			hWnd,
			(HMENU)addId,
			hInstance,
			NULL
		)))
		{
			exit(-1);
		}
		ShowWindow(hAddButton, SW_SHOWNORMAL);

		if (!(hClearButton = CreateWindow
		(
			"BUTTON",
			"Clear",
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
			0,
			halfHeight/4 * 6,
			halfWidth,
			halfHeight/4,
			hWnd,
			(HMENU)clearId,
			hInstance,
			NULL
		)))
		{
			exit(-1);
		}
		ShowWindow(hClearButton, SW_SHOWNORMAL);

		if (!(hToRightButton = CreateWindow
		(
			"BUTTON",
			"To Right",
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
			halfWidth,
			halfHeight/4 * 5,
			halfWidth,
			halfHeight/4,
			hWnd,
			(HMENU)toRightId,
			hInstance,
			NULL
		)))
		{
			exit(-1);
		}
		ShowWindow(hToRightButton, SW_SHOWNORMAL);

		if (!(hDeleteButton = CreateWindow
		(
			"BUTTON",
			"Delete",
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON,
			halfWidth,
			halfHeight/4 * 6,
			halfWidth,
			halfHeight/4,
			hWnd,
			(HMENU)deleteId,
			hInstance,
			NULL
		)))
		{
			exit(-1);
		}
		ShowWindow(hDeleteButton, SW_SHOWNORMAL);
	}
	break;

	case WM_COMMAND:
		if (lParam == (LPARAM)hAddButton)
		{
			CHAR str[MAX_PATH];
			GetWindowText(hEdit, str, MAX_PATH);
			if (str[0] != 0 && SendMessage(hLeftListBox, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)str) == LB_ERR)
			{
				SendMessage(hEdit, EM_SETSEL, NULL, -1);
				SendMessage(hEdit, WM_CLEAR, NULL, NULL);
				SendMessage(hLeftListBox, LB_ADDSTRING, NULL, (LPARAM)str);
				UpdateWindow(hLeftListBox);
			}
		}

		if (lParam == (LPARAM)hClearButton)
		{
			SendMessage(hLeftListBox, LB_RESETCONTENT, 0, 0);
			SendMessage(hRightListBox, LB_RESETCONTENT, 0, 0);
			UpdateWindow(hLeftListBox);
			UpdateWindow(hRightListBox);
		}

		if (lParam == (LPARAM)hToRightButton)
		{
			LRESULT index;
			if ((index = SendMessage(hLeftListBox, LB_GETCURSEL, NULL, NULL)) != LB_ERR)
			{
				CHAR str[MAX_PATH];
				SendMessage(hLeftListBox, LB_GETTEXT, index, (LPARAM)str);
				if (SendMessage(hRightListBox, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)str) == LB_ERR)
				{
					SendMessage(hRightListBox, LB_ADDSTRING, NULL, (LPARAM)str);
					UpdateWindow(hRightListBox);
				}
			}
		}

		if (lParam == (LPARAM)hDeleteButton)
		{
			LRESULT index;
			if ((index = SendMessage(hLeftListBox, LB_GETCURSEL, NULL, NULL)) != LB_ERR)
			{
				SendMessage(hLeftListBox, LB_DELETESTRING, index, 0);
				UpdateWindow(hLeftListBox);
			}
			if ((index = SendMessage(hRightListBox, LB_GETCURSEL, NULL, NULL)) != LB_ERR)
			{
				SendMessage(hRightListBox, LB_DELETESTRING, index, 0);
				UpdateWindow(hRightListBox);
			}
		}
		break;
	case WM_PAINT:

		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}


int WINAPI WinMain
(
	HINSTANCE hInstance,		                        // дескриптор текущего экземпляра окна
	HINSTANCE hPrevInstance,	                        // дескриптор предыдущего экземпляра окна 
	LPSTR lpCmdLine,			                        // указатель на командную строку
	int nCmdShow
)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);	                        // WNDCLASSEX size in bytes
	wcex.style = CS_HREDRAW | CS_VREDRAW;		        // Window class styles
	wcex.lpszClassName = TEXT("MAIN_WINDOW");	        // Window class name
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// Window background brush color.
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);    // Window cursor
	wcex.lpfnWndProc = WndProc;		                    // Window procedure associated to this window class.
	wcex.hInstance = hInstance;	                        // The application instance.

	// Register window and ensure registration success.
	if (!RegisterClassEx(&wcex))
		return 1;

	// Setup window initialization attributes.
	CREATESTRUCT cs = { 0 };
	cs.x = CW_USEDEFAULT;                               // Window X position
	cs.y = 0;	                                        // Window Y position
	cs.cx = 640;	                                    // Window width
	cs.cy = 480;	                                    // Window height
	cs.hInstance = hInstance;                           // Window instance.
	cs.lpszClass = wcex.lpszClassName;		            // Window class name
	cs.lpszName = TEXT("SP_Lab7");       	            // Window title
	cs.style = WS_OVERLAPPEDWINDOW;		                // Window style

	// Create the window.
	HWND hWnd = CreateWindowEx
	(
		cs.dwExStyle,
		cs.lpszClass,
		cs.lpszName,
		cs.style,
		cs.x,
		cs.y,
		cs.cx,
		cs.cy,
		cs.hwndParent,
		cs.hMenu,
		cs.hInstance,
		cs.lpCreateParams
	);

	// Validate window.
	if (!hWnd)
		return 1;

	// Display the window.
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// Main message loop.
	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Unregister window class, freeing the memory that was
	// previously allocated for this window.
	UnregisterClass(wcex.lpszClassName, hInstance);

	return (int)msg.wParam;
}

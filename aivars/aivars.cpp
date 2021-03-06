#include "stdafx.h"
#include "aivars.h"
#include <windows.h>
#include <commctrl.h>
//#include <iostream>
//#include <fstream>

#pragma comment(lib, "comctl32.lib")

HWND hTool;
HINSTANCE hInst;
HWND hStatus;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HIMAGELIST g_hImageList = NULL;

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

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AIVARS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AIVARS));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AIVARS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(RGB(249, 247, 229)));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_AIVARS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{  
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE: 
		{
			const int ImageListID = 0;
			const int numButtons = 3;
			const int bitmapSize = 16;
			const DWORD buttonStyles = BTNS_AUTOSIZE;
			int statwidths[] = { 100, -1 };

			InitCommonControls();

			hTool = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0, hWnd, NULL, GetModuleHandle(NULL), NULL);
			hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hWnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);

			if (hTool == NULL)
				return NULL;

			g_hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR16 | ILC_MASK, numButtons, 0);

			SendMessage(hTool, TB_SETIMAGELIST, (WPARAM)ImageListID, (LPARAM)g_hImageList);
			SendMessage(hTool, TB_LOADIMAGES, (WPARAM)IDB_STD_SMALL_COLOR, (LPARAM)HINST_COMMCTRL);
			SendMessage(hTool, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
			SendMessage(hStatus, SB_SETPARTS, sizeof(statwidths)/sizeof(int), (LPARAM)statwidths);
			SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)L"ok this is epic");
			SendMessage(hStatus, SB_SETTEXT, 1, (LPARAM)L"ok this is epic 2");

			TBBUTTON tbButtons[numButtons] = {
				{ MAKELONG(STD_FILENEW, ImageListID), TB_NEW, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0, (INT_PTR)L"New" },
				{ MAKELONG(STD_FILEOPEN, ImageListID), TB_OPEN, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0, (INT_PTR)L"Open"},
				{ MAKELONG(STD_FIND, ImageListID), TB_SEARCH, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0, (INT_PTR)L"Search" }
			};

			SendMessage(hTool, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
			SendMessage(hTool, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);
			SendMessage(hTool, TB_AUTOSIZE, 0, 0);
			ShowWindow(hTool, TRUE);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case TB_NEW:
				MessageBox(NULL, L"New entry", L"New", MB_OK | MB_ICONINFORMATION);
				break;
			case TB_SEARCH:
				MessageBox(NULL, L"Search entries", L"Search", MB_OK | MB_ICONINFORMATION);
				break;
			case TB_OPEN:
				MessageBox(NULL, L"Open entry", L"Open", MB_OK | MB_ICONINFORMATION);
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_SIZE:
		{
			HWND hTool;
			HWND hStatus;
			RECT rcStatus;
			RECT rcTool;
			int iToolHeight;
			int iStatusHeight;

			hTool = GetDlgItem(hWnd, IDC_MAIN_TOOL);
			SendMessage(hTool, TB_AUTOSIZE, 0, 0);
			GetWindowRect(hTool, &rcTool);
			iToolHeight = rcTool.bottom - rcTool.top;

			hStatus = GetDlgItem(hWnd, IDC_MAIN_STATUS);
			SendMessage(hStatus, TB_AUTOSIZE, 0, 0);
			GetWindowRect(hStatus, &rcStatus);
			iStatusHeight = rcStatus.bottom - rcStatus.top;
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
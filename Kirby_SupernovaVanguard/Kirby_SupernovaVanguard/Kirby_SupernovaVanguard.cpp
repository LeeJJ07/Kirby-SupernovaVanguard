#include "Socket.h"
#include "Kirby_SupernovaVanguard.h"
#include "UserData.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void DoubleBuffering(HDC , std::vector<Player*> );

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					 _In_opt_ HINSTANCE hPrevInstance,
					 _In_ LPWSTR    lpCmdLine,
					 _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_KIRBYSUPERNOVAVANGUARD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KIRBYSUPERNOVAVANGUARD));

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
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KIRBYSUPERNOVAVANGUARD));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_KIRBYSUPERNOVAVANGUARD);
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
	static SOCKET socket;
	static TCHAR str[200];
	static std::vector<Player*> client(4);
	static UserData uData;
	static int myID;

	switch (message)
	{
	case WM_CREATE:
	{
		SetTimer(hWnd, 1, 1, NULL);

		if (InitClient(hWnd, socket))
		{
			ReadInitMessage(socket, uData);
			myID = uData.id;

			client[myID] = new Player();

			//SetUserData(uData, client);
			SetPlayer(client, uData);
		}
		break;
	}
	case WM_TIMER:
		InvalidateRgn(hWnd, NULL, FALSE);
		break;
	case WM_ASYNC:
		switch (lParam)
		{
		case FD_READ:
			ReadMessage(socket, client, uData);
			break;
		}
		break;
	
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			DoubleBuffering(hdc, client);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		CloseClient(socket, client, myID);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	DWORD newTime = GetTickCount64();
	static DWORD oldTime = newTime;

	if (newTime - oldTime < 5)
		return 0;

	oldTime = newTime;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		client[myID]->SetPos({ client[myID]->GetPos().x - 1, client[myID]->GetPos().y });
		SetUserData(uData, client[myID]);
		send(socket, (char*)&uData, sizeof(UserData), NULL);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		client[myID]->SetPos({ client[myID]->GetPos().x + 1, client[myID]->GetPos().y });
		SetUserData(uData, client[myID]);
		send(socket, (char*)&uData, sizeof(UserData), NULL);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		
	}
	return 0;
}

void DoubleBuffering(HDC hdc, std::vector<Player*> client)
{
	HDC memdc;
	static HBITMAP  hBit, oldBit;

	memdc = CreateCompatibleDC(hdc);
	hBit = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);

	SelectObject(memdc, hBit);
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	FillRect(memdc, &rect, hBrush);

	DrawPlayer(memdc, client);

	BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memdc, 0, 0, SRCCOPY);

	SelectObject(memdc, oldBit);
	DeleteDC(memdc);
	DeleteObject(hBrush);
}
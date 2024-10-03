#include "Socket.h"
#include "Kirby_SupernovaVanguard.h"
#include "UserData.h"
#include "ActionData.h"
#include "StartScene.h"

#define MAX_LOADSTRING 100
#define TIMER_START 1
#define TIMER_START 1
#define TIMER_START 1

enum SceneState { START, SELECT, GAME };
RECT        rectView;

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

   int width = GetSystemMetrics(SM_CXSCREEN);
   int height = GetSystemMetrics(SM_CYSCREEN);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

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
	static SceneState curScene;

	static SOCKET socket;
	static TCHAR str[200];
	static std::vector<Player*> client(8);
	static UserData uData, myData;
	static short myID;

	static StartScene startScene;

	switch (message)
	{
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rectView);

		curScene = START;

		SetTimer(hWnd, TIMER_START, 20, NULL);

		if (InitClient(hWnd, socket))
		{
			ReadInitMessage(socket, myData);
			myID = myData.id;

			client[myID] = new Player();

			//SetUserData(uData, client);
			SetPlayer(client, myData);
		}
		break;
	}
	case WM_TIMER:
		switch(wParam)
		{
		case TIMER_START:
			InvalidateRgn(hWnd, NULL, FALSE);
			break;
		}
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

			if (curScene == START)
				startScene.DrawBitmapDoubleBuffering(hWnd, hdc, rectView);
			//DoubleBuffering(hdc, client);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, TIMER_START);
		CloseClient(socket, client, myID);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	static int x, y;
	static bool isChange = false;

	static ActionData aD;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		x -= 3;
		isChange = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		x += 3;
		isChange = true;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		y += 3;
		isChange = true;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		y -= 3;
		isChange = true;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{

	}

	DWORD newTime = GetTickCount64();
	static DWORD oldTime = newTime;

	if (newTime - oldTime < 20 || !isChange)
		return 0;

	oldTime = newTime;
	
	aD.id = myID;
	aD.playerMove = { x,y };
	aD.cursorMove = { 0,0 };

	send(socket, (char*)&aD, sizeof(ActionData), NULL);

	isChange = false;
	x = 0, y = 0;
	aD.playerMove = { x,y };
	
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
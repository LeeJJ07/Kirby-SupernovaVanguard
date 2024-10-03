#include "Kirby_SupernovaVanguard.h"
#include "UserData.h"
#include "ActionData.h"
<<<<<<< HEAD
#include "StartScene.h"
=======
#include "Camera.h"
#include "Socket.h"
#include "Map.h"
<<<<<<< HEAD
#include "Multithread.h"
=======
>>>>>>> 4b7570e4bb408ab224c4e5da3e92f5cba9f20b1d
>>>>>>> 2f600fb41d3c2a2cc7f77c32df44745cb38a6793

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

void DoubleBuffering(HDC , std::vector<Player*>);
void DrawCamera(HDC, RECT);
void InitObjArr();
void Paint(void*);

TCHAR str[200];
std::vector<Player*> client(4);
UserData uData, myData;
short myID;

DWORD dwThID1, dwThID2;
HANDLE hThreads[2];

unsigned long ulStackSize = 0;

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
<<<<<<< HEAD
	   CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);
=======
	  0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, nullptr, nullptr, hInstance, nullptr);
>>>>>>> 4b7570e4bb408ab224c4e5da3e92f5cba9f20b1d

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

	static StartScene startScene;

	switch (message)
	{
	case WM_CREATE:
	{
<<<<<<< HEAD
		InitializeCriticalSection(&cs);
=======
<<<<<<< HEAD
		GetClientRect(hWnd, &rectView);

		curScene = START;

		SetTimer(hWnd, TIMER_START, 20, NULL);
=======
		SetTimer(hWnd, 1, 1, NULL);
>>>>>>> 2f600fb41d3c2a2cc7f77c32df44745cb38a6793

		InitObjArr();
>>>>>>> 4b7570e4bb408ab224c4e5da3e92f5cba9f20b1d

		if (InitClient(hWnd, socket))
		{
			ReadInitMessage(socket, myData);
			myID = myData.id;

			client[myID] = new Player();

			SetPlayer(client, myData);

			Create(client[myID]);

			camera.SetTargetObject(client[myID]);
		}

		hThreads[0] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Paint, &hWnd, 0, (unsigned*)&dwThID1);
		hThreads[1] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Paint, hWnd, 0, (unsigned*)&dwThID2);

		if (hThreads[0])
			ResumeThread(hThreads[0]);
		if (hThreads[1])
			ResumeThread(hThreads[1]);

		WaitForSingleObject(hThreads, INFINITE); 

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
			InvalidateRgn(hWnd, NULL, FALSE);
			break;
		}
		break;
<<<<<<< HEAD
=======
	
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			if (curScene == START)
				startScene.DrawBitmapDoubleBuffering(hWnd, hdc, rectView);
			//DoubleBuffering(hdc, client);

			CString t;

			t.Format(_T("%d"), client[myID]->GetPos().x);
			TextOut(hdc, 0, 0, t, t.GetLength());
			t.Format(_T("%d"), client[myID]->GetPos().y);
			TextOut(hdc, 50, 0, t, t.GetLength());

			EndPaint(hWnd, &ps);
		}
		break;
>>>>>>> 2f600fb41d3c2a2cc7f77c32df44745cb38a6793
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
		x -= 1;
		isChange = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		x += 1;
		isChange = true;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		y += 1;
		isChange = true;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		y -= 1;
		isChange = true;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{

	}

	DWORD newTime = GetTickCount64();
	static DWORD oldTime = newTime;

	if (newTime - oldTime < 5 || !isChange)
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
	static HBITMAP  hBit, mapBit, oldBit;

	camera.Resize();
	camera.Update();

	int cTop = camera.GetCameraPos().y - CAMERA_HEIGHT / 2;
	int cBottom = camera.GetCameraPos().y + CAMERA_HEIGHT / 2;
	int cLeft = camera.GetCameraPos().x - CAMERA_WIDTH / 2;
	int cRight = camera.GetCameraPos().x + CAMERA_WIDTH / 2;

	memdc = CreateCompatibleDC(hdc);
	hBit = CreateCompatibleBitmap(hdc, CAMERA_WIDTH, CAMERA_HEIGHT);
	mapBit = CreateCompatibleBitmap(memdc, MAX_MAP_SIZE_X, MAX_MAP_SIZE_Y);

	oldBit = (HBITMAP)SelectObject(memdc, mapBit);
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	RECT rect = { 0, 0, MAX_MAP_SIZE_X, MAX_MAP_SIZE_Y };

	RECT cameraFrame = { cLeft ,cTop, cRight, cBottom };

	cameraFrame.top;
	cameraFrame.left;
	cameraFrame.right;
	cameraFrame.bottom;

	FillRect(memdc, &rect, hBrush);

	DrawCamera(memdc, cameraFrame);

	BitBlt(hdc, 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, memdc, cLeft, cTop, SRCCOPY);

	SelectObject(memdc, oldBit);
	DeleteDC(memdc);
	DeleteObject(hBit);
	DeleteObject(mapBit);
	//DeleteObject(hBrush);
}

void DrawCamera(HDC hdc, RECT rect)
{
	for (int i = 0; i < objnum; i++)
	{
		switch (objArr[i]->GetType())
		{
		case TERRAIN:
			break;
		case PLAYER:
			DrawPlayer(hdc, (Player*)objArr[i]);
			break;
		case ENEMY:
			break;
		case PMISSILE:
			break;
		case EMISSILE:
			break;
		default:
			continue;
		}
	}
}

void InitObjArr()
{
	objArr = new Collider2D*[1000];
}

void Paint(void* pParam)
{
	HWND hWnd = (HWND)pParam;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	DoubleBuffering(hdc, client);

	CString t;

	t.Format(_T("%d"), client[myID]->GetPos().x);
	TextOut(hdc, 0, 0, t, t.GetLength());
	t.Format(_T("%d"), client[myID]->GetPos().y);
	TextOut(hdc, 50, 0, t, t.GetLength());

	EndPaint(hWnd, &ps);
}
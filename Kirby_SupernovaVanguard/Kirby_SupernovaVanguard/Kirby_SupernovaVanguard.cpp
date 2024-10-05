#include "Kirby_SupernovaVanguard.h"
#include "UserData.h"
#include "ActionData.h"
#include "StartScene.h"
#include "Camera.h"
#include "Socket.h"
#include "Map.h"
#include "Multithread.h"

#define MAX_LOADSTRING 100
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
unsigned __stdcall Paint(HWND );
unsigned __stdcall Send();
unsigned __stdcall Read();

TCHAR str[200];
std::vector<Player*> client(4);
UserData uData, myData;
short myID;
static SOCKET cSocket;

std::mutex mtx;  // 스레드 간 상호 배제를 위한 뮤텍스
std::condition_variable cv;  // 스레드 간 동기화를 위한 조건 변수
int turn;  // A(0), B(1), C(1)
LPARAM temp;

DWORD dwThID1, dwThID2, dwThID3;
HANDLE hThreads[3];

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
	  0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int x, y;
bool isChange = false;

ActionData aD;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	temp = lParam;

	static SceneState curScene;

	static StartScene startScene;

	switch (message)
	{
	case WM_CREATE:
	{
		InitializeCriticalSection(&cs);
		GetClientRect(hWnd, &rectView);

		curScene = START;

		SetTimer(hWnd, TIMER_START, 1, NULL);

		InitObjArr();
		if (InitClient(hWnd, cSocket))
		{
			ReadInitMessage(cSocket, myData);
			myID = myData.id;

			client[myID] = new Player();

			SetPlayer(client, myData);

			Create(client[myID]);

			camera.SetTargetObject(client[myID]);
		}

		hThreads[0] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Paint, hWnd, 0, (unsigned*)&dwThID1);
		hThreads[1] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Send, NULL, 0, (unsigned*)&dwThID2);
		hThreads[2] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Read, NULL, 0, (unsigned*)&dwThID3);

		if (hThreads[0])
			ResumeThread(hThreads[0]);
		if (hThreads[1])
			ResumeThread(hThreads[1]);
		if (hThreads[2])
			ResumeThread(hThreads[2]);

		break;
	}
	case WM_TIMER:
		switch(wParam)
		{
		case TIMER_START:
			//InvalidateRgn(hWnd, NULL, FALSE);
			break;
		}
		break;
	case WM_ASYNC:
		switch (lParam)
		{
		case FD_READ:
			ReadMessage(cSocket, client, uData);
			break;
		}
		break;
	case WM_DESTROY:
		if (hThreads[0])CloseHandle(hThreads[0]);
		if (hThreads[1])CloseHandle(hThreads[1]);
		if (hThreads[2])CloseHandle(hThreads[2]);

		DeleteCriticalSection(&cs);

		KillTimer(hWnd, TIMER_START);
		CloseClient(cSocket, client, myID);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

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

unsigned __stdcall Send()
{
	while (TRUE)
	{
		std::unique_lock<std::mutex> lock(mtx);  // 뮤텍스 잠금
		cv.wait(lock, [] { return turn == 0; });  // A 함수의 차례가 될 때까지 대기

		if (isChange)
		{
			aD.id = myID;
			aD.playerMove = { x,y };
			aD.cursorMove = { 0,0 };

			send(cSocket, (char*)&aD, sizeof(ActionData), NULL);

			isChange = false;
			x = 0, y = 0;
			aD.playerMove = { x,y };

			turn = 1;  // B 함수에게 차례를 넘김
			cv.notify_all();  // B 함수를 깨움
		}
	}
}

unsigned __stdcall Read()
{
	while (TRUE)
	{
		std::unique_lock<std::mutex> lock(mtx);  // 뮤텍스 잠금
		cv.wait(lock, [] { return turn == 1; });  // B 함수의 차례가 될 때까지 대기

		if(temp == FD_READ)
		{
			ReadMessage(cSocket, client, uData);

			turn = 0;  // C 함수에게 차례를 넘김
			cv.notify_all();  // C 함수를 깨움
		}
	}
}

unsigned __stdcall Paint(HWND pParam)
{
	while (TRUE)
	{
		//std::unique_lock<std::mutex> lock(mtx);  // 뮤텍스 잠금
		//cv.wait(lock, [] { return turn == 2; });  // C 함수의 차례가 될 때까지 대기
		
		EnterCriticalSection(&cs);

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(pParam, &ps);

		/*if (curScene == START)
			startScene.DrawBitmapDoubleBuffering(hWnd, hdc, rectView);*/

		DoubleBuffering(hdc, client);

		CString t;

		t.Format(_T("%d"), client[myID]->GetPos().x);
		TextOut(hdc, 0, 0, t, t.GetLength());
		t.Format(_T("%d"), client[myID]->GetPos().y);
		TextOut(hdc, 50, 0, t, t.GetLength());

		InvalidateRgn(pParam, NULL, FALSE);

		EndPaint(pParam, &ps);

		LeaveCriticalSection(&cs);

		//turn = 0;  // A 함수에게 차례를 넘김
		//cv.notify_all();  // A 함수를 깨움
	}
}
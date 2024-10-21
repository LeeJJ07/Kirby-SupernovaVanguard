#include "Kirby_SupernovaVanguard.h"
#include "PlayerData.h"
#include "ActionData.h"
#include "StartScene.h"
#include "SelectScene.h"
#include "AllSkill.h"
#include "Camera.h"
#include "Socket.h"
#include "Map.h"
#include "Multithread.h"

#define MAX_LOADSTRING 100
#define TIMER_START 1
#define TIMER_SELECT 2

enum SceneState { START, SELECT, GAME };
RECT        rectView;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void DoubleBuffering(HDC, std::vector<Object*>);
void DrawCamera(HDC);
void DrawCollider(HDC&);
void InitObjArr();
unsigned __stdcall Paint(HWND);
unsigned __stdcall Send();
unsigned __stdcall Read();

std::vector<Object*> vClient(PLAYERNUM);
std::vector<Object*> vMonster(MONSTERNUM);
std::vector<Object*> vSkill(SKILLNUM);
TOTALDATA uData;
Object** objArr;
static SOCKET cSocket;

// >> : Thread
CRITICAL_SECTION cs;
// <<

// >> : Map
HBITMAP hImage;
static HDC memdc;
static HBITMAP mapBit, oldMemBitmap;

static HDC bufferdc;
static HBITMAP oldBufferBitmap, bufferBitmap;
// <<

// >> : move
int x, y;
int cursorX, cursorY;

ActionData aD;

static std::chrono::high_resolution_clock::time_point t1_move;
static std::chrono::high_resolution_clock::time_point t2_move;
static std::chrono::duration<double> timeSpan_move;

bool canGoToNext;
SceneState curScene;
StartScene startScene;
SelectScene selectScene;

void DrawMousePosition(HDC);
void Update();
void UpdateSelect();
// <<

// >> : fps
static std::chrono::high_resolution_clock::time_point t1_fps;
static std::chrono::high_resolution_clock::time_point t2_fps;
static std::chrono::duration<double> timeSpan_fps;

static int renderingCount = 0;
static int textRenderingCount = 0;

void CountFPS();
void DrawFPS(HDC);
// <<

// >> : Rendering
static std::chrono::high_resolution_clock::time_point t1_render;
static std::chrono::high_resolution_clock::time_point t2_render;
static std::chrono::duration<double> timeSpan_render;

bool isDrawCollider;
// <<

// >> : ReadCount
static std::chrono::high_resolution_clock::time_point t1_read;
static std::chrono::high_resolution_clock::time_point t2_read;
static std::chrono::duration<double> timeSpan_read;

std::chrono::high_resolution_clock::time_point t1_readCount;
std::chrono::high_resolution_clock::time_point t2_readCount;
std::chrono::duration<double> timeSpan_readCount;

int readCount;

void DrawReadNum(HDC);
// <<

// >> : Send
static std::chrono::high_resolution_clock::time_point t1_send;
static std::chrono::high_resolution_clock::time_point t2_send;
static std::chrono::duration<double> timeSpan_send;

static std::chrono::high_resolution_clock::time_point t1_sendCount;
static std::chrono::high_resolution_clock::time_point t2_sendCount;
static std::chrono::duration<double> timeSpan_sendCount;

static int sendCount = 0;
static int textsendCount = 0;

void CountSendNum();
void DrawSendNum(HDC);
// <<

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

	if (!InitInstance(hInstance, nCmdShow))
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

		if (curScene == SELECT)
			UpdateSelect();
		if (curScene == GAME)
			Update();
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KIRBYSUPERNOVAVANGUARD));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KIRBYSUPERNOVAVANGUARD);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	hImage = (HBITMAP)LoadImage(NULL, TEXT("Images/Backgrounds/spacebackground.bmp"), IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		InitializeCriticalSection(&cs);
		GetClientRect(hWnd, &rectView);

		curScene = START;

		SetTimer(hWnd, TIMER_START, 1, NULL);
		SetTimer(hWnd, TIMER_SELECT, 1, NULL);

		InitObjArr();

		if (hThreads[0])
			ResumeThread(hThreads[0]);
		if (hThreads[1])
			ResumeThread(hThreads[1]);
		if (hThreads[2])
			ResumeThread(hThreads[2]);

	}
	break;
	case WM_CHAR:
		if (wParam == VK_RETURN/* && canGoToNext*/)
		{
			switch (curScene)
			{
			case START:
				if (InitClient(hWnd, cSocket))
				{
					if (!ReadInitMessage(cSocket, uData))
						break;

					vClient[myID] = new Player();

					vClient[myID]->ObjectUpdate(uData, myID);
					vClient[myID]->GetCollider()->MovePosition(vClient[myID]->GetPosition());

					CreateObject((Player*)vClient[myID], myID);

					camera.SetTargetObject(vClient[myID]);
					t1_fps = std::chrono::high_resolution_clock::now();
					t1_render = std::chrono::high_resolution_clock::now();
					t1_send = std::chrono::high_resolution_clock::now();
					t1_read = std::chrono::high_resolution_clock::now();
					t1_move = std::chrono::high_resolution_clock::now();
					t1_sendCount = std::chrono::high_resolution_clock::now();
					t1_readCount = std::chrono::high_resolution_clock::now();

					hThreads[0] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Paint, hWnd, 0, (unsigned*)&dwThID1);
					hThreads[1] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Send, NULL, 0, (unsigned*)&dwThID2);
					hThreads[2] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Read, NULL, 0, (unsigned*)&dwThID3);

					curScene = SELECT;
				}
				break;
			case SELECT:
				if (!aD.isReady)
					aD.isReady = true;
				break;
			}
		}
		break;
	case WM_LBUTTONDOWN:
	{
		cursorX = LOWORD(lParam);
		cursorY = HIWORD(lParam);
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (curScene == GAME)
		{
			cursorX = LOWORD(lParam);
			cursorY = HIWORD(lParam);
		}
	}
		break;
	case WM_PAINT:
		if (curScene == START)
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			startScene.DrawBitmapDoubleBuffering(hWnd, hdc, rectView, canGoToNext);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_START:
			Update();
			InvalidateRgn(hWnd, NULL, FALSE);
			break;
		case TIMER_SELECT:
			if (curScene != SELECT || !aD.isReady)
			{
				InvalidateRgn(hWnd, NULL, FALSE);
				break;
			}
			{
				int i;
				for (i = 0; i < vClient.size(); i++)
				{
					if (vClient[i] != NULL && !vClient[i]->GetIsInGame())
						break;
				}
				if (i == vClient.size())
					curScene = GAME;
			}
		}
		break;
	case WM_DESTROY:
		if (hThreads[0])
			CloseHandle(hThreads[0]);
		if (hThreads[1])
			CloseHandle(hThreads[1]);
		if (hThreads[2])
			CloseHandle(hThreads[2]);

		DeleteCriticalSection(&cs);

		KillTimer(hWnd, TIMER_START);
		KillTimer(hWnd, TIMER_SELECT);
		CloseClient(cSocket, vClient, myID);

		SelectObject(memdc, oldMemBitmap);
		DeleteDC(memdc);
		SelectObject(bufferdc, oldBufferBitmap);
		DeleteDC(bufferdc);

		DeleteObject(mapBit);
		DeleteObject(bufferBitmap);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void DoubleBuffering(HDC hdc)
{
	int cTop = camera.GetCameraPos().y - CAMERA_HEIGHT / 2;
	int cLeft = camera.GetCameraPos().x - CAMERA_WIDTH / 2;

	if (mapBit == NULL)
	{
		memdc = CreateCompatibleDC(hdc);
		mapBit = CreateCompatibleBitmap(hdc, MAX_MAP_SIZE_X, MAX_MAP_SIZE_Y);
		oldMemBitmap = (HBITMAP)SelectObject(memdc, mapBit);

		SelectObject(memdc, hImage);
		BitBlt(memdc, 0, 0, MAX_MAP_SIZE_X, MAX_MAP_SIZE_Y, memdc, 0, 0, SRCCOPY);

		bufferdc = CreateCompatibleDC(hdc);
		bufferBitmap = CreateCompatibleBitmap(hdc, MAX_MAP_SIZE_X, MAX_MAP_SIZE_Y);
		oldBufferBitmap = (HBITMAP)SelectObject(bufferdc, bufferBitmap);
	}

	BitBlt(bufferdc, cLeft, cTop, CAMERA_WIDTH, CAMERA_HEIGHT, memdc, cLeft, cTop, SRCCOPY);

	CString t;

	t.Format(_T("Render fps : %d %d"), cLeft, cTop);
	TextOut(bufferdc, 200, 200, t, t.GetLength());

	DrawCamera(bufferdc);

	if (isDrawCollider)
		DrawCollider(bufferdc);

	BitBlt(hdc, 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, bufferdc, cLeft, cTop, SRCCOPY);
}

void DrawCamera(HDC hdc)
{
	for (int i = 0; i < FINALINDEX; i++)
	{
		if (objArr[i] == nullptr)
			continue;

		if (objArr[i]->GetPosition().x < vClient[myID]->GetPosition().x - 1000
			|| objArr[i]->GetPosition().x > vClient[myID]->GetPosition().x + 1000
			|| objArr[i]->GetPosition().y < vClient[myID]->GetPosition().y - 650
			|| objArr[i]->GetPosition().y > vClient[myID]->GetPosition().y + 650)
			continue;

		switch (objArr[i]->GetCollider()->GetType())
		{
		case TERRAIN:
			break;
		case PLAYER:
			((Player*)objArr[i])->DrawPlayer(hdc);
			break;
		case MONSTER:
			((Monster*)objArr[i])->Draw(hdc);
			break;
		case PMISSILE:
			DrawSkill(hdc, (Skill*)objArr[i]);
			break;
		case EMISSILE:
			break;
		default:
			continue;
		}
	}
}

void DrawCollider(HDC& hdc)
{
	for (int i = 0; i < OBJECTNUM; i++)
	{
		if (objArr[i] != nullptr)
			objArr[i]->GetCollider()->DrawCollider(hdc);
	}
}

void InitObjArr()
{
	objArr = new Object * [OBJECTNUM];
	for (int i = 0; i < OBJECTNUM; i++)
	{
		objArr[i] = nullptr;
	}
}

unsigned __stdcall Send()
{
	while (TRUE)
	{
		if (timeSpan_send.count() >= 0.01 && cs.DebugInfo != NULL)
		{
			aD.id = myID;
			aD.playerMove = { x,y };
			aD.cursorMove = { cursorX, cursorY };
			aD.charactertype = dynamic_cast<Player*>(vClient[myID])->GetCharacterType();

			send(cSocket, (char*)&aD, sizeof(ActionData), NULL);

			sendCount++;

			x = 0, y = 0;
			aD.playerMove = { x,y };

			if (timeSpan_sendCount.count() >= 1)
			{
				CountSendNum();
			}

			t1_send = std::chrono::high_resolution_clock::now();
			timeSpan_send = std::chrono::duration_cast<std::chrono::duration<double>>(t2_send - t1_send);
		}
		Sleep(0);
	}
}

unsigned __stdcall Read()
{
	while (TRUE)
	{
		if (timeSpan_read.count() >= 0.005)
		{
			ReadMessage(cSocket, vClient, uData);
		}
		Sleep(0);
	}
}

unsigned __stdcall Paint(HWND pParam)
{
	while (TRUE)
	{
		PAINTSTRUCT ps;
		if (curScene == START)
		{
			Sleep(0);
			continue;
		}
		if (timeSpan_render.count() >= 0.0075 && cs.DebugInfo != NULL)
		{
			EnterCriticalSection(&cs);

			HDC hdc = BeginPaint(pParam, &ps);

			switch (curScene)
			{
			case SELECT:
			{
				{
					selectScene.DrawBitmapDoubleBuffering(pParam, hdc, rectView, vClient);

					t1_render = std::chrono::high_resolution_clock::now();
					timeSpan_render = std::chrono::duration_cast<std::chrono::duration<double>>(t2_render - t1_render);
				}
			}
			break;
			case GAME:
				{
					DoubleBuffering(hdc);

					renderingCount++;

					if (timeSpan_fps.count() >= 1)
					{
						CountFPS();
					}

					DrawMousePosition(hdc);
					DrawFPS(hdc);
					DrawSendNum(hdc);
					DrawReadNum(hdc);

					t1_render = std::chrono::high_resolution_clock::now();
					timeSpan_render = std::chrono::duration_cast<std::chrono::duration<double>>(t2_render - t1_render);
				}
				break;
			}

			EndPaint(pParam, &ps);

			LeaveCriticalSection(&cs);
		}
		Sleep(0);
	}
}

void CountFPS()
{
	textRenderingCount = renderingCount;

	renderingCount = 0;

	t1_fps = std::chrono::high_resolution_clock::now();
}

void DrawFPS(HDC hdc)
{
	CString t;

	t.Format(_T("Render fps : %d"), textRenderingCount);
	TextOut(hdc, 100, 0, t, t.GetLength());
}

void CountSendNum()
{
	textsendCount = sendCount;

	sendCount = 0;

	t1_sendCount = std::chrono::high_resolution_clock::now();
}

void DrawSendNum(HDC hdc)
{
	CString t;

	t.Format(_T("Send fps : %d"), textsendCount);
	TextOut(hdc, 250, 0, t, t.GetLength());
}

void DrawReadNum(HDC hdc)
{
	CString t;

	t.Format(_T("Read fps : %d"), textreadCount);
	TextOut(hdc, 350, 0, t, t.GetLength());

	t.Format(_T("L : %d, R : %d"), cursorX, cursorY);
	TextOut(hdc, 350, 200, t, t.GetLength());
}

void DrawMousePosition(HDC hdc)
{
	CString t;

	t.Format(_T("%d"), vClient[myID]->GetPosition().x);
	TextOut(hdc, 0, 0, t, t.GetLength());
	t.Format(_T("%d"), vClient[myID]->GetPosition().y);
	TextOut(hdc, 50, 0, t, t.GetLength());
}

void Update()
{
	t2_fps = std::chrono::high_resolution_clock::now();
	t2_render = std::chrono::high_resolution_clock::now();
	t2_send = std::chrono::high_resolution_clock::now();
	t2_move = std::chrono::high_resolution_clock::now();
	t2_read = std::chrono::high_resolution_clock::now();
	t2_sendCount = std::chrono::high_resolution_clock::now();
	t2_readCount = std::chrono::high_resolution_clock::now();

	timeSpan_fps = std::chrono::duration_cast<std::chrono::duration<double>>(t2_fps - t1_fps);
	timeSpan_render = std::chrono::duration_cast<std::chrono::duration<double>>(t2_render - t1_render);
	timeSpan_send = std::chrono::duration_cast<std::chrono::duration<double>>(t2_send - t1_send);
	timeSpan_move = std::chrono::duration_cast<std::chrono::duration<double>>(t2_move - t1_move);
	timeSpan_read = std::chrono::duration_cast<std::chrono::duration<double>>(t2_move - t1_move);
	timeSpan_sendCount = std::chrono::duration_cast<std::chrono::duration<double>>(t2_sendCount - t1_sendCount);
	timeSpan_readCount = std::chrono::duration_cast<std::chrono::duration<double>>(t2_readCount - t1_readCount);

	if (timeSpan_move.count() >= 0.005)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			x -= 1;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			x += 1;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			y += 1;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			y -= 1;
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
		}
		t1_move = std::chrono::high_resolution_clock::now();
	}
	if (GetAsyncKeyState('C') & 0x8000)
	{
		isDrawCollider = true;
	}
	else
		isDrawCollider = false;
}

void UpdateSelect()
{
	t2_render = std::chrono::high_resolution_clock::now();
	timeSpan_render = std::chrono::duration_cast<std::chrono::duration<double>>(t2_render - t1_render);
}
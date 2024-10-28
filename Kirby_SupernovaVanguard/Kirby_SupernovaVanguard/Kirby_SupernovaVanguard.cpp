#define _CRT_SECURE_NO_WARNINGS
#include "Kirby_SupernovaVanguard.h"
#include "PlayerData.h"
#include "ActionData.h"
#include "StartScene.h"
#include "SelectScene.h"
#include "skill.h"
#include "MonsterSkill.h"
#include "Camera.h"
#include "Socket.h"
#include "Multithread.h"
#include "Exp.h"
#include "Hp.h"
#include "Animation.h"
#include "SkillSelector.h"

#define MAX_LOADSTRING 100
#define TIMER_START 1
#define TIMER_SELECT 2

// >> : animation
std::map<ObjectImage, Animation*> imageDatas;
std::map<ESKILLTYPE, std::pair< HBITMAP, BITMAP>> imDatas;
void LoadImages();
void LoadSkillImage();
void CleanUpImageDatas();
// <<

// >> : font
void LoadCustomFont();
HFONT CreateCustomFont(int fontSize, LPCWSTR fontName);
void UnloadCustomFont();
// <<

enum SceneState { START, SELECT, GAME };
RECT        rectView;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

void DoubleBuffering(HDC, std::vector<Object*>);
void DrawPlayerHp(HDC&);
void DrawMonsterHp(HDC&);
void DrawCamera(HDC hdc, int cLeft, int cTop);
void DrawEXP(HDC&,int&,int&);
void DrawTime(HDC& hdc, int& cameraLeft, int& cameraTop);
void DrawLevelUp(HDC& hdc, int& cameraLeft, int& cameraTop);
void DrawCollider(HDC&);
void InitObjArr();
unsigned __stdcall Paint(HWND);
unsigned __stdcall Send();
unsigned __stdcall Read();

std::vector<Object*> vClient(PLAYERNUM);
std::vector<Object*> vMonster(MONSTERNUM);
std::vector<Object*> vSkill(SKILLNUM);
std::vector<Object*> vMonsterSkill(MONSTERSKILLNUM);
TOTALDATA uData;
Object** objArr;
static SOCKET cSocket;

// >> : Thread
DWORD dwThID1, dwThID2, dwThID3;
HANDLE hThreads[3];
CRITICAL_SECTION cs;

bool threadEnd_Read;
bool threadEnd_Send;
bool threadEnd_Paint;
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
bool isLockOn;
SceneState curScene;
StartScene startScene;
SelectScene selectScene;

void DrawPlayerPosition(HDC);
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

// >> : LevelUp
bool isChoiceSkill;
bool isSetSkill;
SkillSelector* skillSelector[3];

static HBITMAP hSkillSelectTitleImage;
static BITMAP m_skillSelectTitleBitInfo;
// <<

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

	hSkillSelectTitleImage = (HBITMAP)LoadImage(NULL, TEXT("Images/Backgrounds/levelUpTitle.bmp"), IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hSkillSelectTitleImage == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
	}
	else
		GetObject(hSkillSelectTitleImage, sizeof(BITMAP), &m_skillSelectTitleBitInfo);

	LoadCustomFont();

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return FALSE;

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

		LoadImages(); // 이미지 로드
		LoadSkillImage();

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
			break;
		}
		if (wParam == 'c' || wParam == 'C')
		{
			isDrawCollider = !isDrawCollider;
			break;
		}
		if (wParam == 'z' || wParam == 'Z')
		{
			isLockOn = !isLockOn;
			aD.isLockOn = isLockOn;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	{
		cursorX = LOWORD(lParam);
		cursorY = HIWORD(lParam);
		// 이부분 세 개가 스킬 선택 부분이네
		if (uData.publicdata.islevelUp && !isChoiceSkill)
		{
			int curskill;
			for (curskill = 0; curskill < 3; curskill++)
			{
				if (cursorX > (SCREEN_SIZE_X / 2 + 450 * (curskill - 1) - SKILL_SELECTOR_WIDTH / 2)
					&& cursorX < (SCREEN_SIZE_X / 2 + 450 * (curskill - 1) + SKILL_SELECTOR_WIDTH / 2)
					&& cursorY >(SCREEN_SIZE_Y / 2 - SKILL_SELECTOR_HEIGHT / 2)
					&& cursorY < (SCREEN_SIZE_Y / 2 + SKILL_SELECTOR_HEIGHT / 2))
					break;
			}
			if (curskill == 3)
				break;
			aD.newskill = uData.udata[myID].levelUpSkillIndex[curskill].first;
			isChoiceSkill = true;
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
				{
					curScene = GAME;
					for (int j = 0; j < PLAYERNUM; j++)
					{
						if (vClient[j] == NULL)
							break;
						dynamic_cast<Player*>(vClient[j])->SetPlayerAni();
					}
				}
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

		threadEnd_Read = true;
		threadEnd_Send = true;
		threadEnd_Paint = true;

		Sleep(0);

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

		for (int i = 0; i < 3; i++)
			delete skillSelector[i];

		DeleteObject(hImage);
		DeleteObject(hSkillSelectTitleImage);

		UnloadCustomFont();

		CleanUpImageDatas();

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

	DrawCamera(bufferdc, cLeft, cTop);

	DrawPlayerHp(bufferdc);
	DrawMonsterHp(bufferdc);

	DrawEXP(bufferdc, cTop, cLeft);

	if (isDrawCollider)
		DrawCollider(bufferdc);

	DrawTime(bufferdc, cLeft, cTop);
	// <<

	DrawLevelUp(bufferdc, cLeft, cTop);

	BitBlt(hdc, 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, bufferdc, cLeft, cTop, SRCCOPY);

}

void DrawCamera(HDC hdc, int cLeft, int cTop)
{
	for (int i = MONSTERINDEX; i < FINALINDEX; i++)
	{
		if (objArr[i] == nullptr)
			continue;

		if (objArr[i]->GetPosition().x < cLeft
			|| objArr[i]->GetPosition().x > cLeft + CAMERA_WIDTH
			|| objArr[i]->GetPosition().y < cTop
			|| objArr[i]->GetPosition().y > cTop + CAMERA_HEIGHT)
			continue;

		switch (objArr[i]->GetCollider()->GetColliderType())
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
			((Skill*)objArr[i])->DrawSkill(hdc);
			break;
		case EMISSILE:
			((MonsterSkill*)objArr[i])->DrawMonsterSkill(hdc);
			break;
		default:
			continue;
		}
	}

	for (int i = PLAYERINDEX; i < MONSTERINDEX; i++)
	{
		if (objArr[i] == nullptr)
			continue;

		if (objArr[i]->GetPosition().x < cLeft
			|| objArr[i]->GetPosition().x > cLeft + CAMERA_WIDTH
			|| objArr[i]->GetPosition().y < cTop
			|| objArr[i]->GetPosition().y > cTop + CAMERA_HEIGHT)
			continue;

		switch (objArr[i]->GetCollider()->GetColliderType())
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
			((Skill*)objArr[i])->DrawSkill(hdc);
			break;
		case EMISSILE:
			((MonsterSkill*)objArr[i])->DrawMonsterSkill(hdc);
			break;
		default:
			continue;
		}
	}
}

void DrawPlayerHp(HDC& hdc)
{
	for (int i = 0; i < vClient.size(); i++)
	{
		if (vClient[i] == nullptr)
			continue;

		int offsetY = 20;

		RECT MaxHpBar;

		MaxHpBar.left = vClient[i]->GetPosition().x - MAXHP_WIDTH;
		MaxHpBar.right = vClient[i]->GetPosition().x + MAXHP_WIDTH;
		MaxHpBar.top = vClient[i]->GetPosition().y - MAXHP_HEIGHT + offsetY;
		MaxHpBar.bottom = vClient[i]->GetPosition().y + MAXHP_HEIGHT + offsetY;
		Rectangle(hdc, MaxHpBar.left, MaxHpBar.top, MaxHpBar.right, MaxHpBar.bottom);

		HBRUSH brush;
		brush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

		float hpPer = ((float)uData.udata[i].curHealth / uData.udata[i].maxHealth);

		RECT HpBar;

		HpBar.left = vClient[i]->GetPosition().x - CURHP_WIDTH;
		HpBar.right = HpBar.left + CURHP_WIDTH * 2 * hpPer;
		HpBar.top = vClient[i]->GetPosition().y - CURHP_HEIGHT + offsetY;
		HpBar.bottom = vClient[i]->GetPosition().y + CURHP_HEIGHT + offsetY;
		Rectangle(hdc, HpBar.left, HpBar.top, HpBar.right, HpBar.bottom);

		SelectObject(hdc, oldBrush);
		DeleteObject(brush);
	}
}

void DrawMonsterHp(HDC& hdc)
{
	for (int i = 0; i < vMonster.size(); i++)
	{
		if (vMonster[i] == nullptr)
			continue;

		int offsetY = 20;

		RECT MaxHpBar;

		MaxHpBar.left = vMonster[i]->GetPosition().x - MAXHP_WIDTH;
		MaxHpBar.right = vMonster[i]->GetPosition().x + MAXHP_WIDTH;
		MaxHpBar.top = vMonster[i]->GetPosition().y - MAXHP_HEIGHT + offsetY;
		MaxHpBar.bottom = vMonster[i]->GetPosition().y + MAXHP_HEIGHT + offsetY;
		Rectangle(hdc, MaxHpBar.left, MaxHpBar.top, MaxHpBar.right, MaxHpBar.bottom);

		HBRUSH brush;
		brush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

		float hpPer = ((float)uData.mdata[i].curHealth / uData.mdata[i].maxHealth);

		RECT HpBar;

		HpBar.left = vMonster[i]->GetPosition().x - CURHP_WIDTH;
		HpBar.right = HpBar.left + CURHP_WIDTH * 2 * hpPer;
		HpBar.top = vMonster[i]->GetPosition().y - CURHP_HEIGHT + offsetY;
		HpBar.bottom = vMonster[i]->GetPosition().y + CURHP_HEIGHT + offsetY;
		Rectangle(hdc, HpBar.left, HpBar.top, HpBar.right, HpBar.bottom);

		SelectObject(hdc, oldBrush);
		DeleteObject(brush);
	}
}

void DrawEXP(HDC& hdc, int& cameraTop, int& cameraLeft)
{
	RECT MaxExpBar;

	MaxExpBar.left = cameraLeft + MAXEXP_OFFSET_LEFT;
	MaxExpBar.right = cameraLeft + CAMERA_WIDTH - MAXEXP_OFFSET_RIGHT;
	MaxExpBar.top = cameraTop + CAMERA_HEIGHT - MAXEXP_OFFSET_TOP;
	MaxExpBar.bottom = cameraTop + CAMERA_HEIGHT - MAXEXP_OFFSET_BOTTOM;
	Rectangle(hdc, MaxExpBar.left, MaxExpBar.top, MaxExpBar.right, MaxExpBar.bottom);

	HBRUSH brush;
	brush = CreateSolidBrush(RGB(255, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

	if (uData.publicdata.exp < 0 || uData.publicdata.exp > uData.publicdata.maxExp)
		return;

	// 경험치 비율을 0에서 1 사이로 제한
	double expPer = (double)uData.publicdata.exp / uData.publicdata.maxExp;
	if (expPer >= 1.0f)
		expPer = 0.0f;

	RECT ExpBar;

	ExpBar.left = cameraLeft + EXP_OFFSET_LEFT;
	ExpBar.right = ExpBar.left + (int)((CAMERA_WIDTH - EXP_OFFSET_RIGHT) * expPer);
	ExpBar.top = cameraTop + CAMERA_HEIGHT - EXP_OFFSET_TOP;
	ExpBar.bottom = cameraTop + CAMERA_HEIGHT - EXP_OFFSET_BOTTOM;
	Rectangle(hdc, ExpBar.left, ExpBar.top, ExpBar.right, ExpBar.bottom);

	SelectObject(hdc, oldBrush);
	DeleteObject(brush);
}

void DrawTime(HDC& bufferdc, int& cameraLeft, int& cameraTop) {
	int minutes = static_cast<int>(uData.publicdata.currentTime) / 60;
	int seconds = static_cast<int>(uData.publicdata.currentTime) % 60;

	CString t;

	t.Format(_T("%02d : %02d"), minutes, seconds);  // 두 자리의 분과 초로 출력

	// 글꼴 생성 (예: Arial, 크기 50으로 설정)
	HFONT hFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	// 기존 글꼴 저장
	HFONT oldFont = (HFONT)SelectObject(bufferdc, hFont);

	// 텍스트 색상을 흰색으로 설정
	SetTextColor(bufferdc, RGB(255, 255, 255));

	// 텍스트 배경색을 투명으로 설정
	SetBkMode(bufferdc, TRANSPARENT);

	// 시간의 텍스트 크기를 얻어 화면 상단 중앙에 배치
	SIZE textSize;
	GetTextExtentPoint32(bufferdc, t, t.GetLength(), &textSize);
	int centerX = cameraLeft + (CAMERA_WIDTH - textSize.cx) / 2;  // 카메라 좌표 기준 중앙 X
	TextOut(bufferdc, centerX, cameraTop + 60, t, t.GetLength());  // Y 좌표는 상단에 10픽셀 여백

	// 기존 글꼴 및 색상 복원
	SelectObject(bufferdc, oldFont);
	DeleteObject(hFont);  // 새로 만든 글꼴 삭제
}

void DrawLevelUp(HDC& bufferdc, int& cameraLeft, int& cameraTop) {
	if (uData.publicdata.isAllPlayerChoice)
		return;

	BLENDFUNCTION blend = { AC_SRC_OVER, 0, 220, 0 };
	HDC hScreenDC = GetDC(NULL);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, CAMERA_WIDTH, CAMERA_HEIGHT);
	SelectObject(hMemoryDC, hBitmap);

	RECT rect = { 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT };
	HBRUSH hBrush = CreateSolidBrush(RGB(50, 50, 50));
	FillRect(hMemoryDC, &rect, hBrush);
	DeleteObject(hBrush);

	AlphaBlend(bufferdc, cameraLeft, cameraTop, CAMERA_WIDTH, CAMERA_HEIGHT, hMemoryDC, 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, blend);

	DeleteObject(hBitmap);
	DeleteDC(hMemoryDC);
	ReleaseDC(NULL, hScreenDC);

	HDC hMemDC = CreateCompatibleDC(bufferdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hSkillSelectTitleImage);

	TransparentBlt(
		bufferdc, cameraLeft + CAMERA_WIDTH / 2 - SKILL_SELECTOR_TITLE_WIDTH / 2, cameraTop + 100, SKILL_SELECTOR_TITLE_WIDTH , SKILL_SELECTOR_TITLE_HEIGHT,
		hMemDC, 0, 0, m_skillSelectTitleBitInfo.bmWidth, m_skillSelectTitleBitInfo.bmHeight,
		RGB(255, 255, 255)
	);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);

	// 큰 텍스트 출력 설정
	HFONT hFontLarge = CreateCustomFont(52, L"Y 최애 TTF Regular"); // 큰 폰트 설정
	HFONT hOldFont1 = (HFONT)SelectObject(bufferdc, hFontLarge);

	// 텍스트 색상 설정 (예: 흰색)
	SetTextColor(bufferdc, RGB(255, 255, 255));
	SetBkMode(bufferdc, TRANSPARENT);

	// 텍스트 위치 조정
	int textX = cameraLeft + CAMERA_WIDTH / 2;
	int textY = cameraTop + 95 + SKILL_SELECTOR_TITLE_HEIGHT / 2;

	// 텍스트 중앙 정렬 설정
	SetTextAlign(bufferdc, TA_CENTER | TA_TOP);

	// "SELECT SKILL" 텍스트 출력
	TextOut(bufferdc, textX, textY, L"SELECT  SKILL", lstrlen(L"SELECT  SKILL"));

	// 작은 텍스트 출력 설정
	HFONT hFontSmall = CreateCustomFont(24, L"Y 최애 TTF Regular"); // 작은 폰트 설정
	HFONT hOldFont2 = (HFONT)SelectObject(bufferdc, hFontSmall);

	// 아래쪽에 "배울 스킬을 선택하시오" 추가
	int subTextY = textY + 650; // 추가된 텍스트의 Y 위치 (조정 가능)
	TextOut(bufferdc, textX, subTextY, L"배울 스킬을 선택하시오", lstrlen(L"배울 스킬을 선택하시오"));

	// 폰트 해제 및 정리
	SelectObject(bufferdc, hOldFont1);
	SelectObject(bufferdc, hOldFont2);
	DeleteObject(hFontLarge);
	DeleteObject(hFontSmall);

	
	hFontLarge = CreateCustomFont(28, L"Y 최애 TTF Regular");
	hFontSmall = CreateCustomFont(20, L"Y 최애 TTF Regular");

	// 스킬 선택창 및 텍스트 그리기
	for (int i = 0; i < 3; i++)
	{
		skillSelector[i]->Draw(bufferdc, cameraLeft, cameraTop, hFontLarge, hFontSmall, imDatas);
	}

	// 폰트 삭제 및 정리
	DeleteObject(hFontLarge);
	DeleteObject(hFontSmall);

}

void DrawCollider(HDC& hdc)
{
	for (int i = 0; i < OBJECTNUM; i++)
	{
		if (objArr[i] != nullptr)
		{
			int id = objArr[i]->Getid();
			objArr[i]->GetCollider()->DrawCollider(hdc, id);
		}
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
		if (timeSpan_send.count() >= 0.0025 && cs.DebugInfo != NULL)
		{
			if (threadEnd_Send)
				return 0;
			if (curScene == GAME)
			{
				POINT cursorPos;
				GetCursorPos(&cursorPos);

				cursorX = cursorPos.x;
				cursorY = cursorPos.y;
			}

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
		if (timeSpan_read.count() >= 0.01)
		{
			if (threadEnd_Read)
				return 0;
			EnterCriticalSection(&cs);

			ReadMessage(cSocket, vClient, uData);

			if (uData.publicdata.islevelUp && isChoiceSkill)
			{
				aD.isChoice = true;
				isSetSkill = false;
			}
			if (!uData.publicdata.islevelUp)
			{
				aD.isChoice = false;
				isChoiceSkill = false;
			}
			if (uData.publicdata.islevelUp && !isChoiceSkill && !isSetSkill)
			{
				for (int i = 0; i < 3; i++)
					skillSelector[i]->SetInfo(uData.udata[myID].levelUpSkillIndex[i]);
				isSetSkill = true;
			}
			t1_read = std::chrono::high_resolution_clock::now();

			LeaveCriticalSection(&cs);
		}
		Sleep(0);
	}
}

unsigned __stdcall Paint(HWND pParam)
{
	while (TRUE)
	{
		if (threadEnd_Paint)
			return 0;
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

					DrawPlayerPosition(hdc);
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

void DrawPlayerPosition(HDC hdc)
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
	timeSpan_read = std::chrono::duration_cast<std::chrono::duration<double>>(t2_read - t1_read);
	timeSpan_sendCount = std::chrono::duration_cast<std::chrono::duration<double>>(t2_sendCount - t1_sendCount);
	timeSpan_readCount = std::chrono::duration_cast<std::chrono::duration<double>>(t2_readCount - t1_readCount);

	if (timeSpan_move.count() >= 0.005)
	{
		if (GetAsyncKeyState('A') & 0x8000)
		{
			x -= 1;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			x += 1;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			y += 1;
		}
		if (GetAsyncKeyState('W') & 0x8000)
		{
			y -= 1;
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
		}
		t1_move = std::chrono::high_resolution_clock::now();
	}
}

void UpdateSelect()
{
	t2_render = std::chrono::high_resolution_clock::now();
	timeSpan_render = std::chrono::duration_cast<std::chrono::duration<double>>(t2_render - t1_render);
}
void LoadImages()
{
	std::ifstream file("Datas/imageDatas.txt");
	if (!file.is_open())
	{
		MessageBox(NULL, _T("이미지 데이터 파일을 열 수 없습니다."), _T("에러"), MB_OK);
		return;
	}

	std::string line;
	int i = 0;
	while (std::getline(file, line))
	{
		std::stringstream ss(line);

		// 파일 경로 읽기
		std::string filePath;
		ss >> filePath;

		// cnt와 spacingX 값 읽기
		int cnt, spacingX, R, G, B, height;
		ss >> cnt >> spacingX >> R >> G >> B;

		std::vector<POINT> frames(cnt);
		std::vector<int> lengths(cnt);

		// 프레임 좌표 및 길이 읽기
		for (int j = 0; j < cnt; j++)
		{
			int x, y;
			ss >> x >> y;
			frames[j] = { x, y };
		}
		for (int j = 0; j < cnt; j++)
		{
			int length;
			ss >> length;
			lengths[j] = length;
		}
		ss >> height;

		// Animation 객체 생성
		Animation* ani = new Animation(cnt, spacingX, R, G, B, frames, lengths, height, filePath);
		ani->Load();

		// Enum에 맞춰 맵에 저장
		imageDatas.insert({ (ObjectImage)i, ani });
		i++;
	}

	file.close();

	// >> : 스킬 셀렉터 설정
	for (int i = 0; i < 3; i++)
	{
		skillSelector[i] = new SkillSelector({ SCREEN_SIZE_X / 2 + (i - 1) * 450, SCREEN_SIZE_Y / 2 });
		skillSelector[i]->Load();
	}

	// << :
}

void CleanUpImageDatas()
{
	for (auto& pair : imageDatas)
	{
		delete pair.second;  // 각 Animation 객체를 delete
		pair.second = nullptr;  // 안전하게 포인터를 nullptr로 설정
	}
	imageDatas.clear();  // map을 비움
}


// >> : 폰트
void LoadCustomFont()
{
	// 폰트 파일 경로를 지정합니다
	LPCWSTR fontPath = L"Fonts/YOnepick-Regular.ttf";

	// AddFontResourceEx로 폰트를 메모리에 로드합니다
	AddFontResourceEx(fontPath, FR_PRIVATE, 0);
}
HFONT CreateCustomFont(int fontSize, LPCWSTR fontName)
{
	HFONT hFont = CreateFont(
		fontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName);
	return hFont;
}

void UnloadCustomFont()
{
	LPCWSTR fontPath = L"Fonts/YOnepick-Regular.ttf";
	RemoveFontResourceEx(fontPath, FR_PRIVATE, 0);
}

// <<

void LoadSkillImage()
{
	imDatas[HEAL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/healSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[HEAL].first, sizeof(BITMAP), &imDatas[HEAL].second);

	imDatas[KIRBYSKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/kirSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[KIRBYSKILL].first, sizeof(BITMAP), &imDatas[KIRBYSKILL].second);

	imDatas[DEDEDESKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/ddSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[DEDEDESKILL].first, sizeof(BITMAP), &imDatas[DEDEDESKILL].second);

	imDatas[METAKNIGHTSKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/metaSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[METAKNIGHTSKILL].first, sizeof(BITMAP), &imDatas[METAKNIGHTSKILL].second);

	imDatas[MABEROASKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/maboSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[MABEROASKILL].first, sizeof(BITMAP), &imDatas[MABEROASKILL].second);

	imDatas[ELECTRICFIELDSKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/elecSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[ELECTRICFIELDSKILL].first, sizeof(BITMAP), &imDatas[ELECTRICFIELDSKILL].second);

	imDatas[KUNAISKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/kuSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[KUNAISKILL].first, sizeof(BITMAP), &imDatas[KUNAISKILL].second);

	imDatas[MAGICARROWSKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/marSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[MAGICARROWSKILL].first, sizeof(BITMAP), &imDatas[MAGICARROWSKILL].second);

	imDatas[TORNADOSKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/torSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[TORNADOSKILL].first, sizeof(BITMAP), &imDatas[TORNADOSKILL].second);

	imDatas[TRUCKSKILL].first = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/trkSk.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(imDatas[TRUCKSKILL].first, sizeof(BITMAP), &imDatas[TRUCKSKILL].second);
}
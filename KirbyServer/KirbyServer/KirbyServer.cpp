// KirbyServer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "KirbyServer.h"
#include <WinSock2.h>
#include "Object.h"
#include "TotalData.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

enum State { RECEIVE, SEND };
enum Direction { UP, RIGHT, DOWN, LEFT };

std::vector<Monster*> monsterArr(MONSTERNUM);

typedef struct sendData
{

}SendData;

// 클라이언트 ActionData와 형식 같음
typedef struct receiveData
{
	short id;
	POINT playerMove;
	POINT cursorMove;
	bool isReady;
}ReceiveData;

#define MAX_LOADSTRING 100
#define WM_ASYNC WM_USER + 1
#define TIMER_01 1
#define TIMER_GENERATEMONSTER 2

static int readyclientnum = 0;
static bool isGenerateMonster = false;

int InitServer(HWND hWnd);
int CloseServer();
SOCKET AcceptSocket(HWND hWnd, SOCKET s, SOCKADDR_IN& c_addr, short userID);
void SendToAll();
void ReadData();
void UpdateMonster();
void SetMonsterData(MONSTERDATA& mData, Monster*& m);
void GenerateMonster(int playerIdx);
void CloseClient(SOCKET socket);
void InitMonsterData(MONSTERDATA& mData, Monster*& m, int playerIdx);
bool IsValidSpawnPos(int playerIdx, POINT pos);
POINT SetRandomSpawnPos(int playerIdx, EMonsterType mType);
void InitUserData(PLAYERDATA& userData, int id);
void SetUserData(PLAYERDATA& uData, ReceiveData rData);
void SetTarget(MONSTERDATA& mData, TOTALDATA& tData, int monsterIdx);

WSADATA wsaData;
SOCKET s, cs;

vector<SOCKET> socketList;
TOTALDATA totalData;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_KIRBYSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KIRBYSERVER));

	MSG msg;

	// 기본 메시지 루프입니다:
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
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KIRBYSERVER));
	wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_KIRBYSERVER);
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
	static short userID = 0;

	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_01:
			UpdateMonster();

			SendToAll();
			break;
		case TIMER_GENERATEMONSTER:
		{
			if (isGenerateMonster)
			{
				for (int pIdx = 0; pIdx < PLAYERNUM; pIdx++)
				{
					if (totalData.udata[pIdx].dataType == 0)
						break;
					GenerateMonster(pIdx);
				}
				SendToAll();
			}
		}
			break;
		} 
		break;
	case WM_CREATE:
		SetTimer(hWnd, TIMER_01, 1, NULL);
		SetTimer(hWnd, TIMER_GENERATEMONSTER, 1000, NULL);

		return InitServer(hWnd);
		break;
	case WM_ASYNC:
		switch (lParam)
		{
		case FD_ACCEPT:
			AcceptSocket(hWnd, s, c_addr, userID++);
			break;
		case FD_READ:
			ReadData();
			break;
		case FD_CLOSE:
			CloseClient(wParam);
			break;
		}
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

int InitServer(HWND hWnd)
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	s = socket(AF_INET, SOCK_STREAM, 0);
	
	int sendBufSize = 88256 * 2;  // 송신 버퍼 크기 (예: 8KB)
	int recvBufSize = 88256 * 2;  // 수신 버퍼 크기 (예: 8KB)

	if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufSize, sizeof(sendBufSize)) == SOCKET_ERROR) {
		std::cerr << "Setting send buffer size failed.\n";
		closesocket(s);
		WSACleanup();
		return 1;
	}

	// 수신 버퍼 크기 설정
	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufSize, sizeof(recvBufSize)) == SOCKET_ERROR) {
		std::cerr << "Setting recv buffer size failed.\n";
		closesocket(s);
		WSACleanup();
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = 12346;

	addr.sin_addr.S_un.S_addr = inet_addr("172.30.1.14");

	bind(s, (LPSOCKADDR)&addr, sizeof(addr));

	if (listen(s, 10) == SOCKET_ERROR)
		return 0;

	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_ACCEPT);
	return 0;
}

int CloseServer()
{
	closesocket(s);
	WSACleanup();
	return 0;
}

SOCKET AcceptSocket(HWND hWnd, SOCKET s, SOCKADDR_IN& c_addr, short userID)
{
	SOCKET cs;
	int _size = sizeof(c_addr);
	cs = accept(s, (LPSOCKADDR)&c_addr, &_size);
	WSAAsyncSelect(cs, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);

	PLAYERDATA userData;
	InitUserData(userData, userID);

	socketList.push_back(cs);
	totalData.udata[userID] = userData;

	SendToAll();//{ cs , userData }

	return cs;
}

void ReadData()
{
	readyclientnum = 0;
	for (int i = 0; i < socketList.size(); i++) {
		ReceiveData temp = {};
		int dataLen = recv(socketList[i], (char*)&temp, sizeof(ReceiveData), 0);
		if(dataLen > 0)
		{
			SetUserData(totalData.udata[temp.id], temp);
			if (totalData.udata[temp.id].inGameStart)
				readyclientnum++;
		}
	}	

	if (socketList.size() == readyclientnum)
		isGenerateMonster = true;
}

// 모든 유저들에게 업데이트 된 정보를 전달
void SendToAll()//pair<SOCKET, UserData> cs
{
	for (int i = 0; i < PLAYERNUM; i++)
	{
		if (totalData.udata[i].dataType == NULL)
			break;
		send(socketList[i], (char*)&totalData, sizeof(TOTALDATA), 0);
	}
}

void CloseClient(SOCKET socket)
{
	for (int i = 0; i < socketList.size(); i++) {
		if (socketList[i] == socket) {
			closesocket(socketList[i]);
			//totalData.udata.erase(totalData.udata.begin() + i);
			socketList.erase(socketList.begin() + i);
			break;
		}
	}
}

void InitUserData(PLAYERDATA& userData, int id)
{
	userData.dataType = PLAYERTYPE;
	userData.id = id;
	userData.pos = { 50 * (id + 1), 50 * (id + 1)};
	userData.lookingDir = { 1.0, 1.0 };
	userData.mousePos = { 0, 0 };
	userData.offset = { 0, 0 };
	userData.radius = 10;
	userData.inGameStart = false;
}

void SetUserData(PLAYERDATA& uData, ReceiveData rData)
{
	uData.pos.x += rData.playerMove.x;
	uData.pos.y += rData.playerMove.y;

	uData.mousePos.x = rData.cursorMove.x;
	uData.mousePos.y = rData.cursorMove.y;

	uData.inGameStart = rData.isReady;
}

void SetTarget(MONSTERDATA& mData, TOTALDATA& tData, int monsterIdx)
{
	int distance = pow(mData.pos.x - tData.udata[0].pos.x, 2) + pow(mData.pos.y - tData.udata[0].pos.y, 2);

	mData.targetnum = 0;
	monsterArr[monsterIdx]->SetTargetPos(tData.udata[0].pos);

	for (int i = 1; i < PLAYERNUM; i++)
	{
		if (tData.udata[i].dataType == 0)
			continue;

		int newdistance = pow(mData.pos.x - tData.udata[i].pos.x, 2) + pow(mData.pos.y - tData.udata[i].pos.y, 2);
		if (newdistance < distance)
		{
			distance = newdistance;

			mData.targetnum = i;
			monsterArr[monsterIdx]->SetTargetPos(tData.udata[mData.targetnum].pos);
		}
	}
}

void InitMonsterData(MONSTERDATA& mData, Monster*& m, int playerIdx)
{
	EMonsterType mType = (EMonsterType)(rand() % NORMAL_MONSTER_TYPE_COUNT);
	POINT generatePos = SetRandomSpawnPos(playerIdx, mType);

	if (!IsValidSpawnPos(playerIdx, generatePos))
		return;

	switch (mType)
	{
	case RUNNER:
		m = new RunnerMonster(generatePos, mType, CHASE, {0, 0},
			RUNNER_BASE_DAMAGE, RUNNER_BASE_HEALTH, RUNNER_BASE_SPEED, TRUE);
		break;
	case SPEAR:
		m = new SpearMonster(generatePos, mType, CHASE, { 0, 0 },
			SPEAR_BASE_DAMAGE, SPEAR_BASE_HEALTH, SPEAR_BASE_SPEED, SPEAR_BASE_RANGE, TRUE);
		break;
	case WINGBUG:
		m = new WingBugMonster(generatePos, mType, CHASE, { 0, 0 },
			WINGBUG_BASE_DAMAGE, WINGBUG_BASE_HEALTH, WINGBUG_BASE_SPEED, TRUE);
		if (generatePos.x < totalData.udata[playerIdx].pos.x) m->SetLookingDir({ 1, 0 });
		else m->SetLookingDir({ -1, 0 });
		break;
	case FIREMAN:
		m = new FireManMonster(generatePos, mType, CHASE, { 0, 0 },
			FIREMAN_BASE_DAMAGE, FIREMAN_BASE_HEALTH, FIREMAN_BASE_SPEED, TRUE);
		break;
	case LANDMINE:
		m = new LandMineMonster(generatePos, mType, CHASE, { 0, 0 },
			LANDMINE_BASE_DAMAGE, LANDMINE_BASE_HEALTH, LANDMINE_BASE_SPEED, TRUE);
		break;
	}
	
	monsterCount++;

	mData.dataType = MONSTERTYPE;
	mData.pos = m->GetPosition();
	mData.monsterType = mType;
}

bool IsValidSpawnPos(int playerIdx, POINT pos)
{
	for (int i = 0; i < PLAYERNUM; i++)
	{
		if (playerIdx == i || totalData.udata[i].dataType == 0)
			continue;

		int distance = pow(totalData.udata[i].pos.x - pos.x, 2) + pow(totalData.udata[i].pos.y - pos.y, 2);

		if (distance < SCREEN_SIZE_Y / 4 * SCREEN_SIZE_Y / 4)
			return false;
	}
	return true;
}

POINT SetRandomSpawnPos(int playerIdx, EMonsterType mType)
{
	POINT generatePos = { totalData.udata[playerIdx].pos.x, totalData.udata[playerIdx].pos.y };

	switch (mType)
	{
	case RUNNER:
	case FIREMAN:
	case SPEAR:
	{
		Direction spawnDir = (Direction)(rand() % 4);
		int randX = 0, randY = 0;

		switch (spawnDir)
		{
		case UP:
			randX = rand() % SCREEN_SIZE_X;
			randY = -(SCREEN_SIZE_Y + DEFAULT_SPAWN_SIZE_Y);
			break;
		case DOWN:
			randX = rand() % SCREEN_SIZE_X;
			randY = (SCREEN_SIZE_Y + DEFAULT_SPAWN_SIZE_Y);
			break;
		case RIGHT:
			randX = (SCREEN_SIZE_X + DEFAULT_SPAWN_SIZE_X);
			randY = rand() % SCREEN_SIZE_Y;
			break;
		case LEFT:
			randX = -(SCREEN_SIZE_X + DEFAULT_SPAWN_SIZE_X);
			randY = rand() % SCREEN_SIZE_Y;
			break;
		}
		generatePos.x += randX;
		generatePos.y += randY;
	}
		break;
	case WINGBUG:
	{
		int randomNum = rand() % 2;
		if (randomNum)generatePos.x += (SCREEN_SIZE_X + DEFAULT_SPAWN_SIZE_X);
		else generatePos.x += -(SCREEN_SIZE_X + DEFAULT_SPAWN_SIZE_X);
	}
		break;
	case LANDMINE:
		//#####################################
		//위치 다시 세팅
		break;
	}
	return generatePos;
}

void GenerateMonster(int playerIdx)
{
	for (int i = 0; i < MONSTERNUM; i++)
	{
		if (totalData.mdata[i].dataType == 0)
		{
			InitMonsterData(totalData.mdata[i], monsterArr[i], playerIdx);
			return;
		}
	}
}

void UpdateMonster()
{
	for (int i = 0; i < MONSTERNUM; i++)
	{
		if (totalData.mdata[i].dataType == 0)
			continue;

		totalData.mdata[i].t2_targeting = std::chrono::high_resolution_clock::now();

		totalData.mdata[i].timeSpan_targeting = std::chrono::duration_cast<std::chrono::duration<double>>(totalData.mdata[i].t2_targeting - totalData.mdata[i].t1_targeting);

		if(totalData.mdata[i].timeSpan_targeting.count() > RETARGETINGTIME)
		{
			SetTarget(totalData.mdata[i], totalData, i);
			totalData.mdata[i].t1_targeting = std::chrono::high_resolution_clock::now();
		}
		monsterArr[i]->Update();
		if (!monsterArr[i]->GetEnabled())
		{
			totalData.mdata[i].dataType = 0;
			delete monsterArr[i];
			continue;
		}

		SetMonsterData(totalData.mdata[i], monsterArr[i]);
	}
}

void SetMonsterData(MONSTERDATA& mData, Monster*& m)
{
	
	mData.pos = m->GetPosition();
	mData.lookingDir = m->GetLookingDir();
	mData.curState = m->GetMonsterState();
}
﻿#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "KirbyServer.h"
#include <WinSock2.h>
#include "Object.h"
#include "TotalData.h"
#include "ReadData.h"
#include "TotalSkill.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

using namespace std;

enum State { RECEIVE, SEND };
enum Direction { UP, RIGHT, DOWN, LEFT };

int curplayerindex = PLAYERINDEX;
int curmonsterindex = MONSTERINDEX;
int curskillindex = SKILLINDEX;

std::vector<Monster*> monsterArr(MONSTERNUM);

// >> : multithread
DWORD dwThID1, dwThID2;
HANDLE hThreads[2];
CRITICAL_SECTION criticalsection;
unsigned long ulStackSize = 0;
bool threadEnd_Update;
bool threadEnd_Send;

unsigned __stdcall Update();
unsigned __stdcall Send();
void UpdateTimer();
void UpdateThread();
// <<

// >> : send
static std::chrono::high_resolution_clock::time_point t1_send;
static std::chrono::high_resolution_clock::time_point t2_send;
static std::chrono::duration<double> timeSpan_send;
// <<

// >> : Update
static std::chrono::high_resolution_clock::time_point t1_update;
static std::chrono::high_resolution_clock::time_point t2_update;
static std::chrono::duration<double> timeSpan_update;
// <<

// >> : skill
vector<Skill*> vSkill(SKILLNUM);
void GenerateSkill();
void UpdateSkill();
void UpdateUi();
void SetBasisSkillData(int&);
void SetSkillData(int&, int);
void SetSkillToDatasheet();
// <<

// >> : player
std::vector<Player*> vClient;
bool isAllPlayerChoice = true;
// <<

// >> : UI
static std::chrono::high_resolution_clock::time_point t1_UI;
static std::chrono::high_resolution_clock::time_point t2_UI;
static std::chrono::duration<double> timeSpan_UI;
// <<

#define MAX_LOADSTRING 100
#define WM_ASYNC WM_USER + 1
#define TIMER_01 1
#define TIMER_GENERATEMONSTER 2
#define TIMER_UPDATESKILL 3
#define TIMER_GENERATESKILL 4

static int readyclientnum = 0;
static bool isAllclientReady = false;
static bool isGameStart = false;
static int skillnum;

int InitServer(HWND hWnd);
int CloseServer();
SOCKET AcceptSocket(HWND hWnd, SOCKET s, SOCKADDR_IN& c_addr, short userID);
void SendToAll();
void ReadData();
void UpdateMonster();
void SetMonsterData(MONSTERDATA& mData, Monster*& m);
void GenerateMonster(int playerIdx);
void CloseClient(SOCKET socket);
void InitMonsterData(MONSTERDATA& mData, Monster*& m, int playerIdx, int ID);
bool IsValidSpawnPos(int playerIdx, POINT pos);
POINT SetRandomSpawnPos(int playerIdx, EMonsterType mType);
void InitUserData(PLAYERDATA& userData, int id);
void SetUserData(PLAYERDATA& uData, ReceiveData rData);
void SetTarget(MONSTERDATA& mData, TOTALDATA& tData, int monsterIdx);

WSADATA wsaData;
SOCKET s, cs;

vector<SOCKET> socketList;

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
		UpdateThread();
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
		case TIMER_GENERATEMONSTER:
		{
			if (isAllclientReady)
			{
				for (int pIdx = 0; pIdx < PLAYERNUM; pIdx++)
				{
					if (totalData.udata[pIdx].dataType == 0)
						break;
					GenerateMonster(pIdx);
				}
				if(!isGameStart)
				{
					for (int i = 0; i < socketList.size(); i++)
					{
						SetBasisSkillData(i);
					}

					hThreads[0] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Update, NULL, 0, (unsigned*)&dwThID1);

					t1_update = std::chrono::high_resolution_clock::now();

					if (hThreads[0])
						ResumeThread(hThreads[0]);
					isGameStart = true;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		}
		break;
	case WM_CREATE:
	{
		InitializeCriticalSection(&criticalsection);

		SetTimer(hWnd, TIMER_GENERATEMONSTER, 1000, NULL);
		hThreads[1] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Send, NULL, 0, (unsigned*)&dwThID2);

		t1_send = std::chrono::high_resolution_clock::now();

		if (hThreads[1])
			ResumeThread(hThreads[1]);

		totalData.publicdata.maxExp = 100;
		totalData.publicdata.exp = 0;

		return InitServer(hWnd);
	}
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
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			wchar_t buffer[50];
			swprintf(buffer, 50, L"%f", totalData.publicdata.currentTime);

			// 화면에 float 값 출력
			TextOut(hdc, 500, 500, buffer, wcslen(buffer));

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		if (hThreads[0])
			CloseHandle(hThreads[0]);
		if (hThreads[1])
			CloseHandle(hThreads[1]);

		threadEnd_Update = true;
		threadEnd_Send = true;

		Sleep(0);

		//delete kirbyskill;

		DeleteCriticalSection(&criticalsection);

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
	
	int sendBufSize = sizeof(TOTALDATA) + 1;  // 송신 버퍼 크기 (예: 8KB)
	int recvBufSize = sizeof(TOTALDATA) + 1;  // 수신 버퍼 크기 (예: 8KB)

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

	addr.sin_addr.S_un.S_addr = inet_addr("172.30.1.94");

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

	Player* player = new Player(userID);
	vClient.push_back(player);

	SendToAll(); // { cs , userData }

	return cs;
}

void ReadData()
{
	readyclientnum = 0;
	int choiceClientNum = 0;
	static int a[4];
	for (int i = 0; i < socketList.size(); i++) {
		ReceiveData temp = {};
		int dataLen = recv(socketList[i], (char*)&temp, sizeof(ReceiveData), 0);
		if(dataLen > 0)
		{
			if (isAllPlayerChoice)
			{
				SetUserData(totalData.udata[temp.id], temp);

				if (totalData.udata[temp.id].inGameStart)
				{
					readyclientnum++;
				}
			}
			else if (temp.isChoice && a[i] == 0)
			{
				choiceClientNum++;
				totalData.publicdata.islevelUp = false;
				a[i] = temp.newskill;
				SetSkillData(i, a[i]);
			}
		}
	}
	if (socketList.size() == choiceClientNum)
	{
		isAllPlayerChoice = true;
		for (int i = 0; i < 4; i++)
			a[i] = 0;
	}

	if (socketList.size() == readyclientnum)
	{
		isAllclientReady = true;
	}
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

unsigned __stdcall Update()
{
	while (TRUE)
	{
		if (timeSpan_update.count() >= 0.02)
		{
			if (threadEnd_Update)
				return 0;
			EnterCriticalSection(&criticalsection);

			if (isAllPlayerChoice)
			{
				UpdateTimer();
				UpdateMonster();
				GenerateSkill();
				UpdateSkill();
				UpdateUi();
				SetSkillToDatasheet();
			}

			t1_update = std::chrono::high_resolution_clock::now();
			timeSpan_update = std::chrono::duration_cast<std::chrono::duration<double>>(t2_update - t1_update);

			LeaveCriticalSection(&criticalsection);
		}
		Sleep(0);
	}
}

unsigned __stdcall Send()
{
	while (TRUE)
	{
		if (timeSpan_send.count() >= 0.0001)
		{
			if (threadEnd_Send)
				return 0;
			EnterCriticalSection(&criticalsection);

			for (int i = 0; i < PLAYERNUM; i++)
			{
				if (totalData.udata[i].dataType == NULL)
					break;
				send(socketList[i], (char*)&totalData, sizeof(TOTALDATA), 0);
			}

			t1_send = std::chrono::high_resolution_clock::now();
			timeSpan_send = std::chrono::duration_cast<std::chrono::duration<double>>(t2_send - t1_send);

			LeaveCriticalSection(&criticalsection);
		}
		Sleep(0);
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
	uData.charactertype = rData.charactertype;

	// >> : SetLookingdir
		// >> : SetPlayerPosition
		int PlayerX, PlayerY;

		if (uData.pos.x >= SCREEN_SIZE_X / 2)
			PlayerX = SCREEN_SIZE_X / 2;
		else if (uData.pos.x >= (MAX_MAP_SIZE_X - SCREEN_SIZE_X / 2))
			PlayerX = uData.pos.x - (MAX_MAP_SIZE_X - SCREEN_SIZE_X);
		else
			PlayerX = uData.pos.x;

		if (uData.pos.y >= SCREEN_SIZE_Y / 2)
			PlayerY = SCREEN_SIZE_Y / 2;
		else if (uData.pos.y >= (MAX_MAP_SIZE_Y - SCREEN_SIZE_Y / 2))
			PlayerY = uData.pos.y - (MAX_MAP_SIZE_Y - SCREEN_SIZE_Y);
		else
			PlayerY = uData.pos.y;
		// <<
	
		PAIR lookingdir = { (PlayerX - uData.mousePos.x), (PlayerY - uData.mousePos.y)};
		double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
		lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

		uData.lookingDir.first = -lookingdir.first;
		uData.lookingDir.second = -lookingdir.second;
	// <<
}

void SetBasisSkillData(int& playerIndex)
{
	vClient[playerIndex]->SetCharacterType(totalData.udata[playerIndex].charactertype);
	Skill* basisSkill = nullptr;
	switch (vClient[playerIndex]->GetCharacterType())
	{
	case ECharacterType::KIRBY:
		basisSkill = new KirbySkill(playerIndex, 0);
		break;
	case ECharacterType::DEDEDE:
		basisSkill = new DededeSkill(playerIndex, 0);
		break;
	case ECharacterType::METAKNIGHT:
		basisSkill = new MetaknightSkill(playerIndex, 0);
		break;
	case ECharacterType::MABOROA:
		basisSkill = new MaberoaSkill(playerIndex, 0);
		break;
	}
	SkillManager* skillmanager = new SkillManager(basisSkill->Getskilltype(), basisSkill->Getcooltime());

	std::vector<SkillManager*> sm = vClient[playerIndex]->GetSkillManager();
	sm.push_back(skillmanager);
	vClient[playerIndex]->SetSkillManager(sm);
}


void SetSkillData(int& playerIndex, int skillnum)
{
	Skill* basisSkill = nullptr;
	switch (skillnum)
	{
	case SKILLTYPE::ELECTRICFIELDSKILL:
		basisSkill = new ElectricfieldSkill(playerIndex, 0);
		break;
	case SKILLTYPE::KUNAISKILL:
		basisSkill = new KunaiSkill(playerIndex, 0);
		break;
	case SKILLTYPE::MAGICARROWSKILL :
		basisSkill = new MagicArrowSkill(playerIndex, 0);
		break;
	case SKILLTYPE::TORNADOSKILL:
		basisSkill = new TornadoSkill(playerIndex, 0);
		break;
	case SKILLTYPE::TRUCKSKILL:
		basisSkill = new TruckSkill(playerIndex, 0);
		break;
	}
	SkillManager* skillmanager = new SkillManager(basisSkill->Getskilltype(), basisSkill->Getcooltime());

	std::vector<SkillManager*> sm = vClient[playerIndex]->GetSkillManager();
	sm.push_back(skillmanager);
	vClient[playerIndex]->SetSkillManager(sm);
}

void GenerateSkill()
{
	for (int i = 0; i < socketList.size(); i++)
	{
		std::vector<SkillManager*> temp = vClient[i]->GetSkillManager();
		for (int j = 0; j < temp.size(); j++)
		{
			temp[j]->Settime_2();

			double skillcooltime = std::chrono::duration_cast<std::chrono::duration<double>>(temp[j]->Gettime_2() - temp[j]->Gettime_1()).count();

			if (skillcooltime > temp[j]->Getcooltime())
			{
				int s;
				bool isGenerateSkill = false;
				for (s = SKILLINDEX; s < FINALINDEX; s++)
				{
					if (!OBJECTIDARR[s])
					{
						switch (temp[j]->Gettype())
						{
						case SKILLTYPE::KIRBYSKILL:
						{
							KirbySkill* kirbySkill = new KirbySkill(i, 0);
							kirbySkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							kirbySkill->Settime_1();
							kirbySkill->Settime_2();
							kirbySkill->Setisactivate(true);
							kirbySkill->SetID(s);
							kirbySkill->Setoffset({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							kirbySkill->Setposition({ totalData.udata[i].pos.x + kirbySkill->Getoffset().x, totalData.udata[i].pos.y + kirbySkill->Getoffset().y });
							kirbySkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = kirbySkill;
						}
						break;
						case SKILLTYPE::DEDEDESKILL:
						{
							DededeSkill* dededeSkill = new DededeSkill(i, 0);
							dededeSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							dededeSkill->Settime_1();
							dededeSkill->Settime_2();
							dededeSkill->Setisactivate(true);
							dededeSkill->SetID(s);
							dededeSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first * 10, (long)totalData.udata[i].lookingDir.second * 10 });
							dededeSkill->Setposition({ totalData.udata[i].pos.x + dededeSkill->Getoffset().x, totalData.udata[i].pos.y + dededeSkill->Getoffset().y });
							dededeSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = dededeSkill;
						}
						break;
						case SKILLTYPE::METAKNIGHTSKILL:
						{
							MetaknightSkill* metaknightSkill = new MetaknightSkill(i, 0);
							metaknightSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							metaknightSkill->Settime_1();
							metaknightSkill->Settime_2();
							metaknightSkill->Setisactivate(true);
							metaknightSkill->SetID(s);
							metaknightSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first * metaknightSkill->Getsize() / OFFSETADJUST, (long)totalData.udata[i].lookingDir.second * metaknightSkill->Getsize() / OFFSETADJUST });
							metaknightSkill->Setposition({ totalData.udata[i].pos.x + metaknightSkill->Getoffset().x, totalData.udata[i].pos.y + metaknightSkill->Getoffset().y });
							metaknightSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = metaknightSkill;
						}
						break;
						case SKILLTYPE::MABEROASKILL:
						{
							MaberoaSkill* maberoaskill = new MaberoaSkill(i, 0);
							maberoaskill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							maberoaskill->Settime_1();
							maberoaskill->Settime_2();
							maberoaskill->Setisactivate(true);
							maberoaskill->SetID(s);
							maberoaskill->Setoffset({ 0, -10 });
							maberoaskill->Setposition({ totalData.udata[i].pos.x + maberoaskill->Getoffset().x, totalData.udata[i].pos.y + maberoaskill->Getoffset().y });
							maberoaskill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = maberoaskill;
						}
						break;
						case SKILLTYPE::ELECTRICFIELDSKILL:
						{
							ElectricfieldSkill* electricfieldskill = new ElectricfieldSkill(i, 0);
							electricfieldskill->Setdirection({ 0,0 });
							electricfieldskill->Settime_1();
							electricfieldskill->Settime_2();
							electricfieldskill->Setisactivate(true);
							electricfieldskill->SetID(s);
							electricfieldskill->Setoffset({ 0,0 });
							electricfieldskill->Setposition({ totalData.udata[i].pos.x + electricfieldskill->Getoffset().x, totalData.udata[i].pos.y + electricfieldskill->Getoffset().y });
							electricfieldskill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = electricfieldskill;
						}
							break;
						case SKILLTYPE::KUNAISKILL:
						{
							KunaiSkill* kunaiSkill = new KunaiSkill(i, 0);
							kunaiSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							kunaiSkill->Settime_1();
							kunaiSkill->Settime_2();
							kunaiSkill->Setisactivate(true);
							kunaiSkill->SetID(s);
							kunaiSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first * kunaiSkill->Getsize() / OFFSETADJUST, (long)totalData.udata[i].lookingDir.second * kunaiSkill->Getsize() / OFFSETADJUST });
							kunaiSkill->Setposition({ totalData.udata[i].pos.x + kunaiSkill->Getoffset().x, totalData.udata[i].pos.y + kunaiSkill->Getoffset().y });
							kunaiSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = kunaiSkill;
						}
						break;
						case SKILLTYPE::MAGICARROWSKILL:
						{
							MagicArrowSkill* magicarrowSkill = new MagicArrowSkill(i, 0);
							magicarrowSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							magicarrowSkill->Settime_1();
							magicarrowSkill->Settime_2();
							magicarrowSkill->Setisactivate(true);
							magicarrowSkill->SetID(s);
							magicarrowSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first , (long)totalData.udata[i].lookingDir.second });
							magicarrowSkill->Setposition({ totalData.udata[i].pos.x + magicarrowSkill->Getoffset().x, totalData.udata[i].pos.y + magicarrowSkill->Getoffset().y });
							magicarrowSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = magicarrowSkill;
						}
						break;
						case SKILLTYPE::TORNADOSKILL:
						{
							TornadoSkill* tornadoSkill = new TornadoSkill(i, 0);
							tornadoSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							tornadoSkill->Settime_1();
							tornadoSkill->Settime_2();
							tornadoSkill->Setisactivate(true);
							tornadoSkill->SetID(s);
							tornadoSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first , (long)totalData.udata[i].lookingDir.second });
							tornadoSkill->Setposition({ totalData.udata[i].pos.x + tornadoSkill->Getoffset().x, totalData.udata[i].pos.y + tornadoSkill->Getoffset().y });
							tornadoSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = tornadoSkill;
						}
						break;
						case SKILLTYPE::TRUCKSKILL:
						{
							TruckSkill* truckSkill = new TruckSkill(i, 0);
							truckSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							truckSkill->Settime_1();
							truckSkill->Settime_2();
							truckSkill->Setisactivate(true);
							truckSkill->SetID(s);
							truckSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first , (long)totalData.udata[i].lookingDir.second });
							truckSkill->Setposition({ totalData.udata[i].pos.x + truckSkill->Getoffset().x, totalData.udata[i].pos.y + truckSkill->Getoffset().y });
							truckSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = truckSkill;
						}
						break;
						}
						skillnum++;
						temp[j]->Settime_1();

						OBJECTIDARR[s] = true;
						isGenerateSkill = true;

						break;
					}
				}
				if (s == FINALINDEX && !isGenerateSkill)
				{
					return;
				}
			}
		}
	}
}

void SetSkillToDatasheet()
{
	int i = 0;
	for (auto it = vSkill.begin(); it != vSkill.end(); ++it)
	{
		Skill* skill = *it;

		if (skill == nullptr)
			continue;

		int ID = skill->GetID() - SKILLINDEX;
		switch (skill->Getskilltype())
		{
		case SKILLTYPE::KIRBYSKILL:
			SetKirbySkillInDatasheet(skill, ID);
			break;
		case SKILLTYPE::DEDEDESKILL:
			SetDededeSkillInDatasheet(skill, ID);
			break;
		case SKILLTYPE::METAKNIGHTSKILL:
			SetMetaknightSkillInDatasheet(skill, ID);
			break;
		case SKILLTYPE::MABEROASKILL:
			SetMaberoaSkillInDatasheet(skill, ID);
			break;
		case SKILLTYPE::ELECTRICFIELDSKILL:
			SetElectricfieldSkillInDatasheet(skill, ID);
			break;
		case SKILLTYPE::KUNAISKILL:
			SetKunaiSkillInDatasheet(skill, ID);
			break;
		case SKILLTYPE::MAGICARROWSKILL:
			SetMagicArrowSkillInDatasheet(skill, ID);
			break;
		case SKILLTYPE::TORNADOSKILL:
			SetTornadoSkillInDatasheet(skill, ID);
			break;
		case SKILLTYPE::TRUCKSKILL:
			SetTruckSkillInDatasheet(skill, ID);
			break;
		}
	}
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

void InitMonsterData(MONSTERDATA& mData, Monster*& m, int playerIdx, int ID)
{
	EMonsterType mType = (EMonsterType)(rand() % NORMAL_MONSTER_TYPE_COUNT);
	POINT generatePos = SetRandomSpawnPos(playerIdx, mType);

	if (!IsValidSpawnPos(playerIdx, generatePos))
		return;

	switch (mType)
	{
	case RUNNER:
		m = new RunnerMonster(generatePos, mType, CHASE, { 0, 0 },
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
	mData.id = ID;                                                                                                                   
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
	for (int i = MONSTERINDEX; i < SKILLINDEX; i++)
	{
		if (!OBJECTIDARR[i])
		{
			InitMonsterData(totalData.mdata[i - MONSTERINDEX], monsterArr[i - MONSTERINDEX], playerIdx, i);
			OBJECTIDARR[i] = true;
			return;
		}
	}
}

void UpdateSkill()
{
	for (auto skill : vSkill)
	{
		if (skill == nullptr)
			continue;
		switch (skill->Getskilltype())
		{
		case SKILLTYPE::KIRBYSKILL:
			UpdateKirbySkill(skill);
			break;
		case SKILLTYPE::DEDEDESKILL:
			UpdateDededeSkill(skill);
			break;
		case SKILLTYPE::METAKNIGHTSKILL:
			UpdateMetaknightSkill(skill);
			break;
		case SKILLTYPE::MABEROASKILL:
			UpdateMaberoaSkill(skill);
			break;
		case SKILLTYPE::ELECTRICFIELDSKILL:
			UpdateElectricfieldSkill(skill);
			break;
		case SKILLTYPE::KUNAISKILL:
			UpdateKunaiSkill(skill);
			break;
		case SKILLTYPE::MAGICARROWSKILL:
			UpdateMagicArrowSkill(skill);
			break;
		case SKILLTYPE::TORNADOSKILL:
			UpdateTornadoSkill(skill);
			break;
		case SKILLTYPE::TRUCKSKILL:
			UpdateTruckSkill(skill);
			break;
		}
	}
}

void UpdateMonster()
{
	for (int i = 0; i < MONSTERNUM; i++)
	{
		if (totalData.mdata[i].dataType == 0)
			continue;

		monsterArr[i]->Sett2_targeting();

		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(monsterArr[i]->Gett2_targeting() - monsterArr[i]->Gett1_targeting());

		if(time_span.count() > RETARGETINGTIME)
		{
			SetTarget(totalData.mdata[i], totalData, i);
			monsterArr[i]->Sett1_targeting();
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

void UpdateUi()
{
	totalData.publicdata.exp++;
	if (totalData.publicdata.exp >= totalData.publicdata.maxExp)
	{
		totalData.publicdata.maxExp *= 100;
		totalData.publicdata.exp = 0;
		totalData.publicdata.islevelUp = true;
		isAllPlayerChoice = false;
	}
	if (timeSpan_UI.count() >= 1)
	{
		totalData.publicdata.currentTime += 1;
		t1_UI = std::chrono::high_resolution_clock::now();
		timeSpan_UI = std::chrono::duration_cast<std::chrono::duration<double>>(t2_UI - t1_UI);
	}
}

void SetMonsterData(MONSTERDATA& mData, Monster*& m)
{
	mData.pos = m->GetPosition();
	mData.lookingDir = m->GetLookingDir();
	mData.curState = m->GetMonsterState();
}


void UpdateTimer()
{
	t2_UI = std::chrono::high_resolution_clock::now();
	timeSpan_UI = std::chrono::duration_cast<std::chrono::duration<double>>(t2_UI - t1_UI);
}

void UpdateThread()
{
	t2_update = std::chrono::high_resolution_clock::now();
	timeSpan_update = std::chrono::duration_cast<std::chrono::duration<double>>(t2_update - t1_update);

	t2_send = std::chrono::high_resolution_clock::now();

	timeSpan_send = std::chrono::duration_cast<std::chrono::duration<double>>(t2_send - t1_send);
}
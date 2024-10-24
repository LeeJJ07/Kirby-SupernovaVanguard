#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "KirbyServer.h"
#include <WinSock2.h>
#include "Object.h"
#include "TotalData.h"
#include "ReadData.h"
#include "TotalSkill.h"
#include "TotalMonsterSkill.h"

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
vector<MonsterSkill*> vMonsterSkill(MONSTERSKILLNUM);
void GenerateSkill();
void GenerateMonsterSkill();
void UpdateSkill();
void UpdateMonsterSkill();
void UpdateUi();
void SetBasisSkillData(int&);
void SetSkillData(int&, int);
void SetSkillToDatasheet();
void SetMonsterSkillToDatasheet();
float UpdateAngle(PAIR&);
// <<

// >> : player
std::vector<Player*> vClient;
bool isAllPlayerChoice = true;
bool isLockOn = true;
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
void GenerateKungFuMan();
void GenerateGaoGao();
void GenerateBoss();
void GenerateLandMine(int cx, int cy, int r);
void InitLandMine(MONSTERDATA& mData, Monster*& m, int ID, POINT generatePos);
void InitKFM(MONSTERDATA& mData, Monster*& m, int ID, POINT generatePos);
void InitGaoGao(MONSTERDATA& mData, Monster*& m, int ID, POINT generatePos);
void InitBoss();
void CloseClient(SOCKET socket);
void InitMonsterData(MONSTERDATA& mData, Monster*& m, int playerIdx, int ID);
bool IsValidSpawnPos(int playerIdx, POINT pos);
POINT SetRandomSpawnPos(int playerIdx, EMonsterType mType);
void InitUserData(PLAYERDATA& userData, int id);
void SetUserData(PLAYERDATA& uData, ReceiveData rData);
void SetTarget(MONSTERDATA& mData, TOTALDATA& tData, int monsterIdx);

bool testLandMine = false;
bool init_miniboss1 = false;
bool init_miniboss2 = false;
bool init_boss = false;

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
				/*if (!testLandMine)
				{
					testLandMine = true;
					GenerateLandMine(2000, 1000, 600);
				}*/
				/*if (!init_miniboss1 && totalData.publicdata.currentTime > FIRST_BOSS_INIT_TIME)
				{
					init_miniboss1 = true;
					for (int i = 0; i < PLAYERNUM; i++)
					{
						if (!totalData.udata[i].dataType) continue;
						GenerateKungFuMan();
					}
				}*/
				if (!init_miniboss2 && totalData.publicdata.currentTime > SECOND_BOSS_INIT_TIME)
				{
					init_miniboss2 = true;
					for (int i = 0; i < vClient.size(); i++)
					{
						if (!totalData.udata[i].dataType) continue;
						GenerateGaoGao();
					}
				}
				if (!init_boss && totalData.publicdata.currentTime > THIRD_BOSS_INIT_TIME)
				{
					init_boss = true;
				}
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
		srand(NULL);
		InitializeCriticalSection(&criticalsection);

		SetTimer(hWnd, TIMER_GENERATEMONSTER, 1000, NULL);
		hThreads[1] = (HANDLE)_beginthreadex(NULL, ulStackSize, (unsigned(__stdcall*)(void*))Send, NULL, 0, (unsigned*)&dwThID2);

		t1_send = std::chrono::high_resolution_clock::now();

		if (hThreads[1])
			ResumeThread(hThreads[1]);

		totalData.publicdata.maxExp = 10000;
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
			EnterCriticalSection(&criticalsection);
			ReadData();
			LeaveCriticalSection(&criticalsection);
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
	static int choiceClientNum = 0;
	static int a[4];
	for (int i = 0; i < socketList.size(); i++) {
		ReceiveData temp = {};
		int dataLen = recv(socketList[i], (char*)&temp, sizeof(ReceiveData), 0);
		if(dataLen > 0)
		{
			if (isAllPlayerChoice)
			{
				SetUserData(totalData.udata[temp.id], temp);
				vClient[temp.id]->SetisLockOn(temp.isLockOn);

				if (totalData.udata[temp.id].inGameStart)
				{
					readyclientnum++;
				}
			}
			else if (temp.isChoice && a[i] == 0)
			{
				choiceClientNum++;
				a[i] = temp.newskill;
				SetSkillData(i, a[i]);
			}
		}
	}
	if (socketList.size() == choiceClientNum)
	{
		isAllPlayerChoice = true;
		totalData.publicdata.islevelUp = false;
		for (int i = 0; i < 4; i++)
			a[i] = 0;
		choiceClientNum = 0;
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
				GenerateMonsterSkill();
				UpdateMonsterSkill();
				UpdateUi();
				SetSkillToDatasheet();
				SetMonsterSkillToDatasheet();
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

		if (uData.pos.x >= (MAX_MAP_SIZE_X - SCREEN_SIZE_X / 2))
			PlayerX = uData.pos.x - (MAX_MAP_SIZE_X - SCREEN_SIZE_X);
		else if (uData.pos.x >= SCREEN_SIZE_X / 2)
			PlayerX = SCREEN_SIZE_X / 2;
		else
			PlayerX = uData.pos.x;

		if (uData.pos.y >= (MAX_MAP_SIZE_Y - SCREEN_SIZE_Y / 2))
			PlayerY = uData.pos.y - (MAX_MAP_SIZE_Y - SCREEN_SIZE_Y);
		else if (uData.pos.y >= SCREEN_SIZE_Y / 2)
			PlayerY = SCREEN_SIZE_Y / 2;
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
							int monsterIndex = FindCloseMonster(totalData.udata[i].pos);

							KirbySkill* kirbySkill = new KirbySkill(i, monsterIndex);
							if (!(vClient[i]->GetisLockOn()))
								kirbySkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							else
							{
								PAIR lookingdir = { (kirbySkill->Getposition().x - totalData.mdata[monsterIndex].pos.x), (kirbySkill->Getposition().y - totalData.mdata[monsterIndex].pos.y) };
								double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
								lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

								kirbySkill->Setdirection({ (long)(-lookingdir.first),(long)(-lookingdir.second) });
							}
							kirbySkill->Settime_1();
							kirbySkill->Settime_2();
							kirbySkill->Setisactivate(true);
							kirbySkill->SetID(s);
							kirbySkill->Setoffset({ (long)totalData.udata[i].lookingDir.first * (long)kirbySkill->Getsize() / OFFSETADJUST / 2, (long)totalData.udata[i].lookingDir.second * (long)kirbySkill->Getsize() / OFFSETADJUST / 2 });
							kirbySkill->Setposition({ totalData.udata[i].pos.x + kirbySkill->Getoffset().x, totalData.udata[i].pos.y + kirbySkill->Getoffset().y });
							kirbySkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = kirbySkill;
						}
						break;
						case SKILLTYPE::DEDEDESKILL:
						{
							int monsterIndex = FindCloseMonster(totalData.udata[i].pos);

							DededeSkill* dededeSkill = new DededeSkill(i, monsterIndex);
							if (!(vClient[i]->GetisLockOn()))
								dededeSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							else
							{
								PAIR lookingdir = { (dededeSkill->Getposition().x - totalData.mdata[monsterIndex].pos.x), (dededeSkill->Getposition().y - totalData.mdata[monsterIndex].pos.y) };
								double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
								lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

								dededeSkill->Setdirection({ (long)(-lookingdir.first),(long)(-lookingdir.second) });
							}
							dededeSkill->Settime_1();
							dededeSkill->Settime_2();
							dededeSkill->Setisactivate(true);
							dededeSkill->SetID(s);
							dededeSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first * (long)dededeSkill->Getsize() / OFFSETADJUST / 2, (long)totalData.udata[i].lookingDir.second * (long)dededeSkill->Getsize() / OFFSETADJUST / 2 });
							dededeSkill->Setposition({ totalData.udata[i].pos.x + dededeSkill->Getoffset().x, totalData.udata[i].pos.y + dededeSkill->Getoffset().y });
							dededeSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = dededeSkill;
						}
						break;
						case SKILLTYPE::METAKNIGHTSKILL:
						{
							int monsterIndex = FindCloseMonster(totalData.udata[i].pos);

							MetaknightSkill* metaknightSkill = new MetaknightSkill(i, monsterIndex);
							PAIR lookingdir;
							if (!(vClient[i]->GetisLockOn()))
							{
								lookingdir = { (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second };
								metaknightSkill->Setdirection({ (long)lookingdir.first, (long)lookingdir.second });
							}
							else
							{
								lookingdir = { (metaknightSkill->Getposition().x - totalData.mdata[monsterIndex].pos.x ), (metaknightSkill->Getposition().y - totalData.mdata[monsterIndex].pos.y) };
								
								if (lookingdir.first == 0)
									lookingdir.first = skilloffsetX;
								if (lookingdir.second == 0)
									lookingdir.second = skilloffsetX;

								double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
								lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;
								lookingdir.first = -lookingdir.first; lookingdir.second = -lookingdir.second;

								metaknightSkill->Setdirection({ (long)lookingdir.first,(long)lookingdir.second });
							}
							metaknightSkill->Setangle(UpdateAngle(lookingdir));
							metaknightSkill->Settime_1();
							metaknightSkill->Settime_2();
							metaknightSkill->Setisactivate(true);
							metaknightSkill->SetID(s);
							metaknightSkill->Setoffset({ (long)lookingdir.first * (long)metaknightSkill->Getsize() / OFFSETADJUST / 2, (long)lookingdir.second * (long)metaknightSkill->Getsize() / OFFSETADJUST / 2 });
							metaknightSkill->Setposition({ totalData.udata[i].pos.x + metaknightSkill->Getoffset().x, totalData.udata[i].pos.y + metaknightSkill->Getoffset().y });
							metaknightSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = metaknightSkill;
						}
						break;
						case SKILLTYPE::MABEROASKILL:
						{
							int monsterIndex = FindCloseMonster(totalData.udata[i].pos);

							MaberoaSkill* maberoaSkill = new MaberoaSkill(i, monsterIndex);
							if (!(vClient[i]->GetisLockOn()))
								maberoaSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							else
							{
								PAIR lookingdir = { (maberoaSkill->Getposition().x - totalData.mdata[monsterIndex].pos.x), (maberoaSkill->Getposition().y - totalData.mdata[monsterIndex].pos.y) };
								double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
								lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

								maberoaSkill->Setdirection({ (long)(-lookingdir.first),(long)(-lookingdir.second) });
							}
							maberoaSkill->Settime_1();
							maberoaSkill->Settime_2();
							maberoaSkill->Setisactivate(true);
							maberoaSkill->SetID(s);
							maberoaSkill->Setoffset({ 0, -10 });
							maberoaSkill->Setposition({ totalData.udata[i].pos.x + maberoaSkill->Getoffset().x, totalData.udata[i].pos.y + maberoaSkill->Getoffset().y });
							maberoaSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = maberoaSkill;
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
							int monsterIndex = FindCloseMonster(totalData.udata[i].pos);

							KunaiSkill* kunaiSkill = new KunaiSkill(i, monsterIndex);
							PAIR lookingdir;
							if (!(vClient[i]->GetisLockOn()))
							{
								lookingdir = { (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second };
								kunaiSkill->Setdirection({ (long)lookingdir.first, (long)lookingdir.second });
							}
							else
							{
								lookingdir = { (kunaiSkill->Getposition().x - totalData.mdata[monsterIndex].pos.x), (kunaiSkill->Getposition().y - totalData.mdata[monsterIndex].pos.y) };
								
								if (lookingdir.first == 0)
									lookingdir.first = skilloffsetX;
								if (lookingdir.second == 0)
									lookingdir.second = skilloffsetX;

								double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
								lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;
								lookingdir.first = -lookingdir.first; lookingdir.second = -lookingdir.second;

								kunaiSkill->Setdirection({ (long)lookingdir.first,(long)lookingdir.second });
							}
							kunaiSkill->Setangle(UpdateAngle(lookingdir));
							kunaiSkill->Settime_1();
							kunaiSkill->Settime_2();
							kunaiSkill->Setisactivate(true);
							kunaiSkill->SetID(s);
							kunaiSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first * (long)kunaiSkill->Getsize() / OFFSETADJUST / 2, (long)totalData.udata[i].lookingDir.second * (long)kunaiSkill->Getsize() / OFFSETADJUST / 2 });
							kunaiSkill->Setposition({ totalData.udata[i].pos.x + kunaiSkill->Getoffset().x, totalData.udata[i].pos.y + kunaiSkill->Getoffset().y });
							kunaiSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = kunaiSkill;
						}
						break;
						case SKILLTYPE::MAGICARROWSKILL:
						{
							int monsterIndex = FindCloseMonster(totalData.udata[i].pos);

							MagicArrowSkill* magicarrowSkill = new MagicArrowSkill(i, 0);

							PAIR lookingdir = { (magicarrowSkill->Getposition().x - totalData.mdata[monsterIndex].pos.x), (magicarrowSkill->Getposition().y - totalData.mdata[monsterIndex].pos.y) };
							
							if (lookingdir.first == 0)
								lookingdir.first = skilloffsetX;
							if (lookingdir.second == 0)
								lookingdir.second = skilloffsetX;

							double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
							
							lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

							magicarrowSkill->Setdirection({ (long)(-lookingdir.first),(long)(-lookingdir.second) });
							magicarrowSkill->Setangle(UpdateAngle(lookingdir));
							
							magicarrowSkill->Settime_1();
							magicarrowSkill->Settime_2();
							magicarrowSkill->Setisactivate(true);
							magicarrowSkill->SetID(s);
							magicarrowSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first * (long)magicarrowSkill->Getsize() / OFFSETADJUST / 2, (long)totalData.udata[i].lookingDir.second * (long)magicarrowSkill->Getsize() / OFFSETADJUST / 2 });
							magicarrowSkill->Setposition({ totalData.udata[i].pos.x + magicarrowSkill->Getoffset().x, totalData.udata[i].pos.y + magicarrowSkill->Getoffset().y });
							magicarrowSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = magicarrowSkill;
						}
						break;
						case SKILLTYPE::TORNADOSKILL:
						{
							int monsterIndex = FindCloseMonster(totalData.udata[i].pos);
							TornadoSkill* tornadoSkill = new TornadoSkill(i, monsterIndex);
							tornadoSkill->Setdirection({ (long)totalData.udata[i].lookingDir.first, (long)totalData.udata[i].lookingDir.second });
							tornadoSkill->Settime_1();
							tornadoSkill->Settime_2();
							tornadoSkill->Setisactivate(true);
							tornadoSkill->SetID(s);
							tornadoSkill->Setoffset({ (long)totalData.udata[i].lookingDir.first , (long)totalData.udata[i].lookingDir.second });
							tornadoSkill->Setposition({ totalData.udata[i].pos.x + tornadoSkill->Getoffset().x, totalData.udata[i].pos.y + tornadoSkill->Getoffset().y });
							tornadoSkill->Setmasternum(i);
							tornadoSkill->Settargetnum(monsterIndex);
							vSkill[s - SKILLINDEX] = tornadoSkill;
						}
						break;
						case SKILLTYPE::TRUCKSKILL:
						{
							int monsterIndex = FindCloseMonster(totalData.udata[i].pos);
							
							TruckSkill* truckSkill = new TruckSkill(i, monsterIndex);
							truckSkill->Setoffset({ (rand() % SCREEN_SIZE_X) / 2 , (rand() % SCREEN_SIZE_Y) / 2 });
							truckSkill->Setposition({ totalData.udata[i].pos.x + truckSkill->Getoffset().x, totalData.udata[i].pos.y + truckSkill->Getoffset().y });

							PAIR lookingdir = { (truckSkill->Getposition().x - totalData.mdata[monsterIndex].pos.x), (truckSkill->Getposition().y - totalData.mdata[monsterIndex].pos.y) };
							double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
							lookingdir.first /= -temp / OFFSETADJUST; lookingdir.second /= -temp / OFFSETADJUST;

							truckSkill->Setdirection({ (long)(lookingdir.first),(long)(lookingdir.second) });
							truckSkill->Setangle(UpdateAngle(lookingdir));

							truckSkill->Settime_1();
							truckSkill->Settime_2();
							truckSkill->Setisactivate(true);
							truckSkill->SetID(s);
							truckSkill->Setmasternum(i);
							vSkill[s - SKILLINDEX] = truckSkill;
						}
						break;
						}

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

void GenerateMonsterSkill()
{
	for (int i = 0; i < MONSTERNUM; i++)
	{
		if (monsterArr[i] != 0 &&monsterArr[i]->GetMonsterState() == EMonsterState::ATTACK)
		{
			std::vector<SkillManager*> skillmanager = monsterArr[i]->GetSkillManager();
			for (int j = 0; j < skillmanager.size(); j++)
			{
				skillmanager[j]->Settime_2();

				double skillcooltime = std::chrono::duration_cast<std::chrono::duration<double>>(skillmanager[j]->Gettime_2() - skillmanager[j]->Gettime_1()).count();

				if (skillcooltime > skillmanager[j]->Getcooltime())
				{
					int s;
					bool isGenerateSkill = false;
					for (s = MONSTERSKILLINDEX; s < FINALINDEX; s++)
					{
						if (!OBJECTIDARR[s])
						{
							switch (skillmanager[j]->Gettype())
							{
							case MONSTERSKILLTYPE::SPEARSKILL:
							{
								int playerIndex = FindClosePlayer(totalData.mdata[i].pos);

								SpearSkill* spearSkill = new SpearSkill(i, playerIndex);

								PAIR lookingdir = { (spearSkill->Getposition().x - totalData.udata[playerIndex].pos.x), (spearSkill->Getposition().y - totalData.udata[playerIndex].pos.y) };
								double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
								lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

								spearSkill->Setdirection({ (long)(-lookingdir.first),(long)(-lookingdir.second) });
								spearSkill->Setangle(UpdateAngle(lookingdir));

								spearSkill->Settime_1();
								spearSkill->Settime_2();
								spearSkill->Setisactivate(true);
								spearSkill->SetID(s);
								spearSkill->Setoffset({ (long)totalData.mdata[i].lookingDir.first * (long)spearSkill->Getsize() / OFFSETADJUST / 2, (long)totalData.mdata[i].lookingDir.second * (long)spearSkill->Getsize() / OFFSETADJUST / 2 });
								spearSkill->Setposition({ totalData.mdata[i].pos.x + spearSkill->Getoffset().x, totalData.mdata[i].pos.y + spearSkill->Getoffset().y });
								spearSkill->Setmasternum(i);
								vMonsterSkill[s - MONSTERSKILLINDEX] = spearSkill;
							}
							break;
							case MONSTERSKILLTYPE::FIREMANSKILL:
							{
								int playerIndex = FindClosePlayer(totalData.mdata[i].pos);

								FiremanSkill* firemanSkill = new FiremanSkill(i, playerIndex);

								PAIR lookingdir = { (firemanSkill->Getposition().x - totalData.udata[playerIndex].pos.x), (firemanSkill->Getposition().y - totalData.udata[playerIndex].pos.y) };
								double temp = sqrt(pow(lookingdir.first, 2) + pow(lookingdir.second, 2));
								lookingdir.first /= temp / OFFSETADJUST; lookingdir.second /= temp / OFFSETADJUST;

								firemanSkill->Setdirection({ (long)(-lookingdir.first),(long)(-lookingdir.second) });
								firemanSkill->Setangle(UpdateAngle(lookingdir));

								firemanSkill->Settime_1();
								firemanSkill->Settime_2();
								firemanSkill->Setisactivate(true);
								firemanSkill->SetID(s);
								firemanSkill->Setoffset({ (long)totalData.mdata[i].lookingDir.first * (long)firemanSkill->Getsize() / OFFSETADJUST / 2, (long)totalData.mdata[i].lookingDir.second * (long)firemanSkill->Getsize() / OFFSETADJUST / 2 });
								firemanSkill->Setposition({ totalData.mdata[i].pos.x + firemanSkill->Getoffset().x, totalData.mdata[i].pos.y + firemanSkill->Getoffset().y });
								firemanSkill->Setmasternum(i);
								vMonsterSkill[s - MONSTERSKILLINDEX] = firemanSkill;
							}
							break;
							}

							skillmanager[j]->Settime_1();

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

void SetMonsterSkillToDatasheet()
{
	int i = 0;
	for (auto it = vMonsterSkill.begin(); it != vMonsterSkill.end(); ++it)
	{
		MonsterSkill* monsterSkill = *it;

		if (monsterSkill == nullptr)
			continue;

		int ID = monsterSkill->GetID() - MONSTERSKILLINDEX;
		switch (monsterSkill->Getskilltype())
		{
		case MONSTERSKILLTYPE::SPEARSKILL:
			SetSpearSkillInDatasheet(monsterSkill, ID);
			break;
		case MONSTERSKILLTYPE::FIREMANSKILL:
			SetFiremanSkillInDatasheet(monsterSkill, ID);
			break;
		}
	}
}

int FindCloseMonster(POINT& myposition)
{
	int x = myposition.x;
	int y = myposition.y;

	int monsternum = -1;
	int min = 99999999;

	for (int i = 0; i < MONSTERNUM; i++)
	{
		if (totalData.mdata[i].dataType == 0)
			continue;

		int value = pow(x - totalData.mdata[i].pos.x, 2) + pow(y - totalData.mdata[i].pos.y, 2);

		if (value < min)
		{
			min = value;
			monsternum = i;
		}
	}
	return monsternum;
}

int FindClosePlayer(POINT& myposition)
{
	int x = myposition.x;
	int y = myposition.y;

	int playernum = -1;
	int min = 99999999;

	for (int i = 0; i < PLAYERNUM; i++)
	{
		if (totalData.udata[i].dataType != 'p')
			continue;

		int value = pow(x - totalData.udata[i].pos.x, 2) + pow(y - totalData.udata[i].pos.y, 2);

		if (value < min)
		{
			min = value;
			playernum = i;
		}
	}
	return playernum;
}

POINT GetNormalizationRange(POINT& skill, POINT& monster)
{
	int newX = monster.x - skill.x;
	int newY = monster.y - skill.y;
	POINT newPos;

	double temp = sqrt(pow(newX, 2) + pow(newY, 2));
	newPos.x = newX / temp * OFFSETADJUST;
	newPos.y = newY / temp * OFFSETADJUST;
	if (newPos.x <= -100 && newPos.y <= -100)
		return { 0,0 };

	if (temp <= 10)
		return { 0,0 };

	return newPos;
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
		}
	}
	monsterArr[monsterIdx]->SetTargetPos(tData.udata[mData.targetnum].pos);
}

void InitMonsterData(MONSTERDATA& mData, Monster*& m, int playerIdx, int ID)
{
	//EMonsterType mType = (EMonsterType)(rand() % NORMAL_MONSTER_TYPE_COUNT);
	EMonsterType mType = (EMonsterType)FIREMAN;
	POINT generatePos = SetRandomSpawnPos(playerIdx, mType);

	if (!IsValidSpawnPos(playerIdx, generatePos))
		return;

	MonsterSkill* monsterSkill = nullptr;

	switch (mType)
	{
	case RUNNER:
		m = new RunnerMonster(generatePos, mType, CHASE, { 0, 0 },
			RUNNER_BASE_DAMAGE, RUNNER_BASE_HEALTH, RUNNER_BASE_SPEED, TRUE);
		break;
	case SPEAR:
		m = new SpearMonster(generatePos, mType, CHASE, { 0, 0 },
			SPEAR_BASE_DAMAGE, SPEAR_BASE_HEALTH, SPEAR_BASE_SPEED, SPEAR_BASE_RANGE, TRUE);

		monsterSkill = new SpearSkill(ID, playerIdx);
		break;
	case WINGBUG:
		m = new WingBugMonster(generatePos, mType, CHASE, { 0, 0 },
			WINGBUG_BASE_DAMAGE, WINGBUG_BASE_HEALTH, WINGBUG_BASE_SPEED, TRUE);
		if (generatePos.x < totalData.udata[playerIdx].pos.x) m->SetLookingDir({ -1, 0 });
		else m->SetLookingDir({ 1, 0 });
		break;
	case FIREMAN:
		m = new FireManMonster(generatePos, mType, CHASE, { 0, 0 },
			FIREMAN_BASE_DAMAGE, FIREMAN_BASE_HEALTH, FIREMAN_BASE_SPEED, TRUE);

		monsterSkill = new FiremanSkill(ID, playerIdx);
		break;
	}

	monsterArr[ID - MONSTERINDEX] = m;

	SkillManager* skillmanager = new SkillManager(monsterSkill->Getskilltype(), monsterSkill->Getcooltime());

	std::vector<SkillManager*> sm = monsterArr[ID - MONSTERINDEX]->GetSkillManager();
	sm.push_back(skillmanager);
	monsterArr[ID - MONSTERINDEX]->SetSkillManager(sm);

	monsterCount++;

	mData.dataType = MONSTERTYPE;
	mData.pos = m->GetPosition();
	mData.monsterType = mType;
	mData.id = ID;                                                                                                                   
}
void InitKFM(MONSTERDATA& mData, Monster*& m, int ID, POINT generatePos)
{
	m = new KungFuMan(generatePos, KUNGFUMAN, CHASE, { 0, 0 },
		KUNGFUMAN_BASE_DAMAGE, KUNGFUMAN_BASE_HEALTH, KUNGFUMAN_BASE_SPEED, KUNGFUMAN_BASE_ATTACK_SPEED, TRUE);

	monsterCount++;

	mData.dataType = MONSTERTYPE;
	mData.pos = m->GetPosition();
	mData.monsterType = KUNGFUMAN;
	mData.id = ID;
}
void InitGaoGao(MONSTERDATA& mData, Monster*& m, int ID, POINT generatePos)
{
	m = new GaoGao({ -100, -100}, GAOGAO, ATTACK, { -50, -50 },
		GAOGAO_BASE_DAMAGE, GAOGAO_BASE_HEALTH, GAOGAO_BASE_ATTACK_SPEED, TRUE);

	monsterCount++;

	mData.dataType = MONSTERTYPE;
	mData.pos = m->GetPosition();
	mData.monsterType = GAOGAO;
	mData.id = ID;
}
void InitBoss()
{

}
void InitLandMine(MONSTERDATA& mData, Monster*& m, int ID, POINT generatePos)
{
	m = new LandMineMonster(generatePos, LANDMINE, CHASE, { 0, 0 },
		LANDMINE_BASE_DAMAGE, LANDMINE_BASE_HEALTH, LANDMINE_BASE_SPEED, TRUE);

	monsterCount++;

	mData.dataType = MONSTERTYPE;
	mData.pos = m->GetPosition();
	mData.monsterType = LANDMINE;
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
void GenerateLandMine(int cx, int cy, int r)
{
	int mineCount = 160;
	double pi = 3.141592;
	double radian = 360 / mineCount * 180 / pi;

	for (int i = 0; i < PLAYERNUM; i++)
	{
		if (totalData.udata[i].dataType == 0) continue;
		totalData.udata[i].pos.x = cx - 100 + 200 * i;
		totalData.udata[i].pos.y = cy + 250;

		vClient[i]->SetPosition(totalData.udata[i].pos);
	}

	for (int i = 0; i < mineCount; i++)
	{
		POINT generatePos = { cx + cos(radian * i) * r, cy + sin(radian * i) * r };

		for (int i = MONSTERINDEX; i < SKILLINDEX; i++)
		{
			if (!OBJECTIDARR[i])
			{
				InitLandMine(totalData.mdata[i - MONSTERINDEX], monsterArr[i - MONSTERINDEX], i, generatePos);
				OBJECTIDARR[i] = true;
				break;
			}
		}
	}
}

void GenerateKungFuMan()
{
	//for (int i = 0; i < PLAYERNUM; i++)
	//{
	//	if (totalData.udata[i].dataType == 0) continue;
	//	totalData.udata[i].pos.x = cx - 100 + 200 * i;
	//	totalData.udata[i].pos.y = cy + 250;

	//	vClient[i]->SetPosition(totalData.udata[i].pos);
	//}

	POINT generatePos = { 500, 500 };

	for (int i = MONSTERINDEX; i < SKILLINDEX; i++)
	{
		if (!OBJECTIDARR[i])
		{
			InitKFM(totalData.mdata[i - MONSTERINDEX], monsterArr[i - MONSTERINDEX], i, generatePos);
			OBJECTIDARR[i] = true;
			break;
		}
	}
}
void GenerateGaoGao()
{
	POINT generatePos = { 500, 500 };

	for (int i = MONSTERINDEX; i < SKILLINDEX; i++)
	{
		if (!OBJECTIDARR[i])
		{
			InitGaoGao(totalData.mdata[i - MONSTERINDEX], monsterArr[i - MONSTERINDEX], i, generatePos);
			OBJECTIDARR[i] = true;
			break;
		}
	}
}

void GenerateBoss()
{

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

void UpdateMonsterSkill()
{
	for (auto skill : vMonsterSkill)
	{
		if (skill == nullptr)
			continue;
		switch (skill->Getskilltype())
		{
		case MONSTERSKILLTYPE::SPEARSKILL:
			UpdateSpearSkill(skill);
			break;
		case MONSTERSKILLTYPE::FIREMANSKILL:
			UpdateFiremanSkill(skill);
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

		if ((monsterArr[i]->GetMonsterType() == KUNGFUMAN || monsterArr[i]->GetMonsterType() == GAOGAO) &&  time_span.count() > 0.1)
		{
			SetTarget(totalData.mdata[i], totalData, i);
			monsterArr[i]->Sett1_targeting();
		}
		else if(time_span.count() > RETARGETINGTIME)
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

float UpdateAngle(PAIR& lookingdir)
{
	double angleRadians = atan2(lookingdir.second, lookingdir.first);

	// 라디안 값을 도(degree)로 변환하려면
 	double angleDegrees = angleRadians * 180.0 / 3.14;

	if (angleDegrees < 0)
	{
		angleDegrees = 180 + (180 - abs(angleDegrees));
	}

	return angleDegrees;
}
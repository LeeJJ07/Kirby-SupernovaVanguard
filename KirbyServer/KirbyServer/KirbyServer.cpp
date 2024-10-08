﻿// KirbyServer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "KirbyServer.h"
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum State { RECEIVE, SEND };

typedef struct userData
{
	bool inGameStart;

	pair<double, double> lookingDir;
	POINT center;
	POINT pos;
	POINT mousePos;
	int radius;
	int moveDir;
	short id;
}UserData;

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

int InitServer(HWND hWnd);
int CloseServer();
SOCKET AcceptSocket(HWND hWnd, SOCKET s, SOCKADDR_IN& c_addr, short userID);

void SendToClient(pair<SOCKET, UserData> cs);
void SendToAll();
void ReadData();
void CloseClient(SOCKET socket);
void InitUserData(UserData& userData, int id);
void SetUserData(UserData& uData, ReceiveData rData);

WSADATA wsaData;
SOCKET s, cs;
SOCKADDR_IN addr = { 0 }, c_addr = { 0 };

vector<SOCKET> socketList;
vector<UserData> userList;

TCHAR msg[200] = { 0 };
char buffer[100];

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

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
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
			SendToAll();
			break;
		}
		break;
	case WM_CREATE:
		SetTimer(hWnd, TIMER_01, 5, NULL);
		return InitServer(hWnd);
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

	UserData userData;
	InitUserData(userData, userID);

	SendToClient({ cs, userData });

	socketList.push_back(cs);
	userList.push_back(userData);

	SendToAll();//{ cs , userData }

	return cs;
}

void ReadData()
{
	for (int i = 0; i < socketList.size(); i++) {
		ReceiveData temp;
		int dataLen = recv(socketList[i], (char*)&temp, sizeof(ReceiveData), 0);
		if(dataLen > 0)
			SetUserData(userList[temp.id], temp);
	}
}

// 현재 연결된 유저한테 정보를 알려줌
void SendToClient(pair<SOCKET, UserData> cs)
{
	send(cs.first, (char*)&cs.second, sizeof(UserData), 0);
}

// 모든 유저들에게 업데이트 된 정보를 전달
void SendToAll()//pair<SOCKET, UserData> cs
{
	for (int i = 0; i < socketList.size(); i++)
	{
		for (int j = 0; j < userList.size(); j++)
		{
			send(socketList[i], (char*)&userList[j], sizeof(UserData), 0);
		}
	}
}

void CloseClient(SOCKET socket)
{
	for (int i = 0; i < socketList.size(); i++) {
		if (socketList[i] == socket) {
			closesocket(socketList[i]);
			userList.erase(userList.begin() + i);
			socketList.erase(socketList.begin() + i);
			break;
		}
	}
}

void InitUserData(UserData& userData, int id)
{
	userData.id = id;
	userData.center = { 50 * (id + 1), 50 * (id + 1)};
	userData.lookingDir = { 1.0, 1.0 };
	userData.moveDir = 0;
	userData.mousePos = { 0,0 };
	userData.pos = { 50 * (id + 1), 50 * (id + 1) };
	userData.radius = 10;
	userData.inGameStart = false;
}

void SetUserData(UserData& uData, ReceiveData rData)
{
	uData.pos.x += rData.playerMove.x;
	uData.pos.y += rData.playerMove.y;
	//마우스 위치도 설정 필요
	uData.mousePos.x = rData.cursorMove.x;
	uData.mousePos.y = rData.cursorMove.y;

	uData.inGameStart = rData.isReady;
}
﻿// KirbyServer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "KirbyServer.h"
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <list>

using namespace std;

#define MAX_LOADSTRING 100
#define WM_ASYNC WM_USER + 1

int InitServer(HWND hWnd);
int CloseServer();
SOCKET AcceptSocket(HWND hWnd, SOCKET s, SOCKADDR_IN& c_addr);

void SendMessageToClient(char* buffer);
void ReadMessage(TCHAR* msg, char* buffer);
void CloseClient(SOCKET socket);

WSADATA wsaData;
SOCKET s, cs;
SOCKADDR_IN addr = { 0 }, c_addr = { 0 };

list<SOCKET>  socketList;

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
    switch (message)
    {
    case WM_COMMAND:
        break;
    case WM_CREATE:
        return InitServer(hWnd);
    case WM_ASYNC:
        switch (lParam)
        {
        case FD_ACCEPT:
            AcceptSocket(hWnd, s, c_addr);
            break;
        case FD_READ:
            ReadMessage(msg, buffer);
            if (_tcscmp(msg, _T("")))
            {
                //뭔가 서버 화면에 띄우는 코드
            }
            break;
        case FD_CLOSE:
            CloseClient(wParam);
            break;
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
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
    addr.sin_addr.S_un.S_addr = inet_addr("172.30.1.14");

    bind(s, (LPSOCKADDR)&addr, sizeof(addr));

    if (listen(s, 5) == SOCKET_ERROR)
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

SOCKET AcceptSocket(HWND hWnd, SOCKET s, SOCKADDR_IN& c_addr)
{
    SOCKET cs;
    int _size = sizeof(c_addr);
    cs = accept(s, (LPSOCKADDR)&c_addr, &_size);
    WSAAsyncSelect(cs, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);

    socketList.push_back(cs);

    return cs;
}

void ReadMessage(TCHAR* msg, char* buffer)
{
    for (list<SOCKET>::iterator it = socketList.begin(); it != socketList.end(); it++)
    {
        SOCKET cs = (*it);
        int msgLen = recv(cs, buffer, 100, 0);
        if (msgLen > 0)
        {
            buffer[msgLen] == NULL;
#ifdef _UNICODE
            msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
            MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
            msg[msgLen] = NULL;
#else
            strcpy_s(msg, buffer);
#endif
            SendMessageToClient(buffer);
        }
    }
}

void SendMessageToClient(char* buffer)
{
    for (list<SOCKET>::iterator it = socketList.begin(); it != socketList.end(); it++)
    {
        SOCKET cs = (*it);
        send(cs, (LPSTR)buffer, strlen(buffer) + 1, 0);
    }
}

void CloseClient(SOCKET socket)
{
    for (list<SOCKET>::iterator it = socketList.begin(); it != socketList.end(); it++)
    {
        SOCKET cs = (*it);
        if (cs == socket)
        {
            closesocket(cs);
            it = socketList.erase(it);
            break;
        }
    }
}
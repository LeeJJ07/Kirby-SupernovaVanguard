#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include "framework.h"

#pragma comment(lib, "ws2_32.lib")

#define WM_ASYNC WM_USER + 1

extern int readCount;
extern int textreadCount;

struct PLAYERDATA;

class Player;

struct USER
{
	char* ip;
	char* port;
	SOCKET socket;
};

// >> :
static WSADATA wsadata;
static SOCKADDR_IN addr = { 0 }, c_addr = { 0 };
static TCHAR msg[200];
static char buffer[100];
static int msgLen;
static int msgCount;

int InitClient(HWND hWnd, SOCKET&);
int SendMessageToServer(SOCKET &, TCHAR*);
void ReadMessage(SOCKET&, std::vector<Player*>&, PLAYERDATA&);
void CloseClient(SOCKET&, std::vector<Player*>&, int);
void ReadInitMessage(SOCKET&, PLAYERDATA&);
extern void CountReadNum();
// <<
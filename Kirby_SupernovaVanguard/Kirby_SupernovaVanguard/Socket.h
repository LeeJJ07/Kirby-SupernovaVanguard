#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include "framework.h"

#pragma comment(lib, "ws2_32.lib")

#define WM_ASYNC WM_USER + 1

extern int readCount;
extern int textreadCount;

struct TOTALDATA;

class Object;

// >> :
static WSADATA wsadata;
static SOCKADDR_IN addr = { 0 };

int InitClient(HWND hWnd, SOCKET&);
void ReadMessage(SOCKET&, std::vector<Object*>&);
void CloseClient(SOCKET&, std::vector<Object*>&, int);
bool ReadInitMessage(SOCKET&);
extern std::vector<Object*> vMonster;
extern std::vector<Object*> vSkill;
extern std::vector<Object*> vMonsterSkill;
extern void CountReadNum();
// <<
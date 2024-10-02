#include "Socket.h"
#include "UserData.h"
#include "ActionData.h"

int InitClient(HWND hWnd, SOCKET &s)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);

	if (s == INVALID_SOCKET)
	{
		MessageBox(NULL, _T("socket() failed"), _T("Error"), MB_OK);
		return 0;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = 12346;
	addr.sin_addr.S_un.S_addr = inet_addr("172.30.1.14");

	if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("connect() failed"), _T("Error"), MB_OK);
		return 0;
	}

	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);
	return 1;
}

int SendMessageToServer(SOCKET &s, TCHAR* str)
{
	if (s == INVALID_SOCKET)return 0;

#ifdef _UNICODE
	msgLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, msgLen, NULL, NULL);
	msg[msgLen] = NULL;
#else
	strcpy_s(buffer, str);
	msgLen = strlen(buffer);
#endif

	send(s, (LPSTR)buffer, msgLen + 1, 0);
	msgCount = 0;

	return 1;
}

void ReadMessage(SOCKET &s, std::vector<Player*>& p, UserData &uD)
{
	int bytesReceived = recv(s, (char*)&uD, sizeof(UserData), 0);

	if (bytesReceived == sizeof(UserData))
	{
		SetPlayer(p, uD);
	}
	else if (bytesReceived == sizeof(ActionData))
	{
		SetPlayer(p[uD.id], uD);
	}
}

void ReadInitMessage(SOCKET& s, UserData& uD)
{
	int bytesReceived;
	while ((bytesReceived = recv(s, (char*)&uD, sizeof(UserData), 0)) == -1);

	if (bytesReceived != sizeof(UserData))
	{
		MessageBox(NULL, _T("receive() failed"), _T("Error"), MB_OK);
		return;
	}
}

void CloseClient(SOCKET& s, std::vector<Player*>& p, int id)
{
	p[id] = NULL;

	closesocket(s);
	WSACleanup();
}
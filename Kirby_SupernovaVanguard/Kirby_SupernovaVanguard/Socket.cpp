#include "Socket.h"
#include "PlayerData.h"
#include "ActionData.h"
#include "Multithread.h"
#include "Camera.h"

short myID;
int textreadCount;

enum DATATYPE {
	PLAYERTYPE = 'p',
	MONSTERTYPE = 'm',
	ITEMTYPE = 'i'
};

extern std::chrono::duration<double> timeSpan_readCount;
extern std::chrono::high_resolution_clock::time_point t1_readCount;
extern std::chrono::high_resolution_clock::time_point t2_readCount;

int InitClient(HWND hWnd, SOCKET &s)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);

	int sendBufSize = 81920 * 2;  // �۽� ���� ũ�� (��: 8KB)
	int recvBufSize = 81920 * 2;  // ���� ���� ũ�� (��: 8KB)
	
	if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufSize, sizeof(sendBufSize)) == SOCKET_ERROR) {
		std::cerr << "Setting send buffer size failed.\n";
		closesocket(s);
		WSACleanup();
		return 1;
	}

	// ���� ���� ũ�� ����
	if (setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufSize, sizeof(recvBufSize)) == SOCKET_ERROR) {
		std::cerr << "Setting recv buffer size failed.\n";
		closesocket(s);
		WSACleanup();
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = 12346;
	addr.sin_addr.S_un.S_addr = inet_addr("172.30.1.94");

	if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		MessageBox(NULL, _T("connect() failed"), _T("Error"), MB_OK);
		return 0;
	}

	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);
	return 1;
}

void ReadMessage(SOCKET &s, std::vector<Object*>& p, TOTALDATA& pD)
{
	EnterCriticalSection(&cs);

	int bytesReceived = recv(s, (char*)&pD, sizeof(TOTALDATA), 0);

	if (bytesReceived > 0)
	{
		readCount++;

		for (int i = 0; i < PLAYERNUM; i++)
		{
			if (pD.udata[i].dataType == 0)
				break;

			Player* pData = dynamic_cast<Player*>(p[i]);
			if (!pData)
			{
				pData = new Player();
				CreateObject(pData);
			}
			pData->ObjectUpdate(pD, i);
			pData->GetCollider()->MovePosition(pData->GetPosition());

			p[i] = pData;

			camera.PositionUpdate();
		}

		for (int i = 0; i < MONSTERNUM; i++)
		{
			if (pD.mdata[i].dataType == 0)
				continue;

			if (!vMonster[i])
			{
				vMonster[i] = new Monster;
				CreateObject((Monster*)vMonster[i]);
			}

			vMonster[i]->ObjectUpdate(pD, i);
			vMonster[i]->GetCollider()->MovePosition(vMonster[i]->GetPosition());
		}

		if (timeSpan_readCount.count() >= 1)
		{
			CountReadNum();
		}
	}

	LeaveCriticalSection(&cs);
}

bool ReadInitMessage(SOCKET& s, TOTALDATA& uD)
{
	int totalBytesReceived = 0; // �� ������ ����Ʈ ��
	int bytesToReceive = sizeof(TOTALDATA); // ������ ������ ũ��
	int bytesReceived;

	Sleep(1);
	// ��� �����͸� ���� ������ �ݺ�
	while (totalBytesReceived < bytesToReceive)
	{
		bytesReceived = recv(s, (char*)&uD + totalBytesReceived, bytesToReceive - totalBytesReceived, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			MessageBox(NULL, _T("receive() failed"), _T("Error"), MB_OK);
			return false;
		}
		if (bytesReceived == 0)
		{
			// ������ ����� ���
			MessageBox(NULL, _T("Connection closed"), _T("Error"), MB_OK);
			return false;
		}
		totalBytesReceived += bytesReceived; // ������ ����Ʈ ���� ������Ʈ
	}

	// uD���� ������ ó��
	short num = -1;
	for (int i = 0; i < PLAYERNUM; i++)
	{
		if (uD.udata[i].dataType == 0)
			break;
		num++;
	}

	myID = num;

	return true;
}

void CloseClient(SOCKET& s, std::vector<Object*>& p, int id)
{
	p[id] = NULL;

	closesocket(s);
	WSACleanup();
}

void CountReadNum()
{

	textreadCount = readCount;

	readCount = 0;

	t1_readCount = std::chrono::high_resolution_clock::now();
}
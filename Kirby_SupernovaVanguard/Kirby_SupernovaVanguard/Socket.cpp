#include "Socket.h"
#include "PlayerData.h"
#include "ActionData.h"
#include "Multithread.h"
#include "Camera.h"
#include "Skill.h"
#include "MonsterSkill.h"

short myID;
int textreadCount;

enum DATATYPE {
	PLAYERTYPE = 'p',
	MONSTERTYPE = 'm',
	SKILLTYPE = 's',
	ITEMTYPE = 'i'
};

extern std::chrono::duration<double> timeSpan_readCount;
extern std::chrono::high_resolution_clock::time_point t1_readCount;
extern std::chrono::high_resolution_clock::time_point t2_readCount;

int InitClient(HWND hWnd, SOCKET &s)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);

	int sendBufSize = sizeof(TOTALDATA) + 1;  // 송신 버퍼 크기 (예: 8KB)
	int recvBufSize = sizeof(TOTALDATA) + 1;  // 수신 버퍼 크기 (예: 8KB)
	
	if (setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufSize, sizeof(sendBufSize)) == SOCKET_ERROR) {
		std::cerr << "Setting send buffer size failed.\n";
		closesocket(s);
		WSACleanup();
		return 1;
	}

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

	if (bytesReceived >= (int)sizeof(TOTALDATA))
	{
		readCount++;

		// >> : playerdata
		for (int i = 0; i < PLAYERNUM; i++)
		{
			if (pD.udata[i].dataType != PLAYERTYPE)
			{
				if (i == 0)
				{
					LeaveCriticalSection(&cs);
					return;
				}
				break;;
			}

			if (!p[i])
			{
				p[i] = new Player();
				CreateObject(p[i], i + PLAYERINDEX);
				p[i]->Setid(i + PLAYERINDEX);
			}
			p[i]->ObjectUpdate(pD, i);
			p[i]->GetCollider()->MovePosition(p[i]->GetPosition());

			camera.PositionUpdate();
		}
		// <<

		// >> : monsterdata
		for (int i = 0; i < MONSTERNUM; i++)
		{
			if (pD.mdata[i].dataType != MONSTERTYPE)
			{
				objArr[i + MONSTERINDEX] = nullptr;
				continue;
			}

			if (vMonster[i] == nullptr)
			{
				vMonster[i] = new Monster(pD.mdata[i].monsterType);
				CreateObject((Monster*)vMonster[i], i + MONSTERINDEX);
				vMonster[i]->Setid(i);
			}

			vMonster[i]->ObjectUpdate(pD, i);
			vMonster[i]->GetCollider()->MovePosition(vMonster[i]->GetPosition());
		}
		// <<

		// >> : skilldata
		for (int i = 0; i < SKILLNUM; i++)
		{
			if (pD.sdata[i].dataType != SKILLTYPE || !pD.sdata[i].isActivate)
			{
				objArr[i + SKILLINDEX] = nullptr;
				vSkill[i] = nullptr;
				continue;
			}

			if(vSkill[i] == nullptr)
			{
				vSkill[i] = new Skill((ESKILLTYPE)pD.sdata[i].skillType);
				CreateObject((Skill*)vSkill[i], i + SKILLINDEX);
				vSkill[i]->Setid(pD.sdata[i].targetNum);
			}

			vSkill[i]->ObjectUpdate(pD, i);
			vSkill[i]->GetCollider()->MovePosition(vSkill[i]->GetPosition());
		}
		// <<

		// >> : monsterskilldata
		for (int i = 0; i < MONSTERSKILLNUM; i++)
		{
			if (pD.msdata[i].dataType != SKILLTYPE || !pD.msdata[i].isActivate)
			{
				objArr[i + MONSTERSKILLINDEX] = nullptr;
				vMonsterSkill[i] = nullptr;
				continue;
			}

			if (vMonsterSkill[i] == nullptr || vMonsterSkill[i]->GetCollider()->GetColliderShape() != pD.msdata[i].colliderShape)
			{
				vMonsterSkill[i] = new MonsterSkill((EMONSTERSKILLTYPE)pD.msdata[i].skillType);
				CreateObject((MonsterSkill*)vMonsterSkill[i], i + MONSTERSKILLINDEX);
				vMonsterSkill[i]->Setid(pD.msdata[i].targetNum);
			}

			vMonsterSkill[i]->ObjectUpdate(pD, i);
			vMonsterSkill[i]->GetCollider()->MovePosition(vMonsterSkill[i]->GetPosition());
		}
		// <<

		// >> publicdata
		{
			//pD.publicdata.Exp;
		}
		// <<

		if (timeSpan_readCount.count() >= 1)
		{
			CountReadNum();
		}
	}
	LeaveCriticalSection(&cs);
}

bool ReadInitMessage(SOCKET& s, TOTALDATA& uD)
{
	int totalBytesReceived = 0;
	int bytesToReceive = sizeof(TOTALDATA);
	int bytesReceived;

	Sleep(1);

	while (totalBytesReceived < bytesToReceive)
	{
		bytesReceived = recv(s, (char*)&uD + totalBytesReceived, bytesToReceive - totalBytesReceived, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			continue;
		}
		if (bytesReceived == 0)
		{
			continue;
		}
		totalBytesReceived += bytesReceived;
	}

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
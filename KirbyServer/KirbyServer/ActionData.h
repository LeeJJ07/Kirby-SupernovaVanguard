#pragma once

struct ActionData
{
	short id;		// �÷��̾� 1,2,3,4p
	POINT playerMove;		// �÷��̾� �̵� ��ǥ
	POINT cursorMove;		// Ŀ�� �̵� ��ǥ
	bool isReady;
};
#pragma once

struct ActionData
{
	ECharacterState curState;
	POINT	playerMove;		// �÷��̾� �̵� ��ǥ
	POINT	cursorMove;		// Ŀ�� �̵� ��ǥ
	short	id;		// �÷��̾� 1,2,3,4p
	short	charactertype;
	short	newskill;
	bool	isReady;
	bool	isChoice;
	bool	isLockOn;
};
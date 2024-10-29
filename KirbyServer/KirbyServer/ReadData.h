#pragma once


typedef struct receiveData
{
	ECharacterState curState;
	POINT	playerMove;
	POINT	cursorMove;
	short	id;
	short	charactertype;
	short	newskill;
	bool	isReady;
	bool	isChoice;
	bool	isLockOn;
	bool	send;
}ReceiveData;
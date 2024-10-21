#pragma once

typedef struct receiveData
{
	POINT	playerMove;
	POINT	cursorMove;
	short	id;
	short	charactertype;
	short	newskill;
	bool	isReady;
	bool	isChoice;
}ReceiveData;
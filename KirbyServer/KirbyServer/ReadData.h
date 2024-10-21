#pragma once

typedef struct receiveData
{
	short	id;
	POINT	playerMove;
	POINT	cursorMove;
	short	charactertype;
	bool	isReady;
	bool	isChoice;
}ReceiveData;
#pragma once

struct ActionData
{
	ECharacterState curState;
	POINT	playerMove;		// 플레이어 이동 좌표
	POINT	cursorMove;		// 커서 이동 좌표
	short	id;		// 플레이어 1,2,3,4p
	short	charactertype;
	short	newskill;
	bool	isReady;
	bool	isChoice;
	bool	isLockOn;
};
#pragma once

struct ActionData
{
	short id;		// 플레이어 1,2,3,4p
	POINT playerMove;		// 플레이어 이동 좌표
	POINT cursorMove;		// 커서 이동 좌표
	bool isReady;
};
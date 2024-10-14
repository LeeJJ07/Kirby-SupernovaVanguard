#include "Player.h"

void Player::ObjectUpdate(TOTALDATA pData, int i)
{
	SetPosition(pData.udata[i].pos);
	GetCollider()->SetOffset(pData.udata[i].offset);
	SetLookingDir(pData.udata[i].lookingDir);
	SetIsInGame(pData.udata[i].inGameStart);
	SetMousePosition(pData.udata[i].mousePos);
}

void Player::DrawPlayer(HDC&)
{

}
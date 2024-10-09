#include "Player.h"

void Player::ObjectUpdate(PlayerData pD)
{
	SetPosition(pD.pos);
	GetCollider()->SetOffset(pD.offset);
	SetLookingDir(pD.lookingDir);
	SetIsInGame(pD.inGameStart);
	SetMousePosition(pD.mousePos);
}

void Player::DrawPlayer(HDC&)
{

}
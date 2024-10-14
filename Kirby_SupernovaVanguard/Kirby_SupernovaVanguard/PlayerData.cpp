#include "PlayerData.h"

//void SetObjectData(TOTALDATA*& ud, Object* p)
//{
//	TOTALDATA* pData = (TOTALDATA*)ud;
//	Player* player = (Player*)p;
//
//	pData->pos = player->GetPosition();
//	pData->offset = player->GetCollider()->GetOffset();
//	pData->lookingDir = player->GetLookingDir();
//	pData->mousePos = player->GetMousePosition();
//
//	int nRadius = ((Circle2D*)((Player*)p->GetCollider()))->GetRadius();
//
//	if (nRadius > 0)
//		pData->radius = nRadius;
//
//	ud = pData;
//	p = player;
//}
//
//void SetObject(Object* &p, TOTALDATA*& ud)
//{
//	PlayerData* pData = (PlayerData*)ud;
//	Player* player = (Player*)p;
//
//	if (!player)
//		player = new Player();
//
//	player->SetPosition(pData->pos);
//	player->SetLookingDir(pData->lookingDir);
//	player->SetMousePosition(pData->mousePos);
//	player->GetCollider()->SetOffset(pData->offset);
//	player->SetIsInGame(pData->inGameStart);
//
//	ud = pData;
//	p = player;
//}
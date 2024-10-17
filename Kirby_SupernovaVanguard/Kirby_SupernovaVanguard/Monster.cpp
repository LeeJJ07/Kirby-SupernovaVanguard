#include "Monster.h"

void Monster::DrawMonster(HDC&)
{

}

void Monster::ObjectUpdate(TOTALDATA totalData, int i)
{
	SetPosition(totalData.mdata[i].pos);
	GetCollider()->SetOffset(totalData.mdata[i].offset);
}
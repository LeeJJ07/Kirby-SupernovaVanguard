#include "Monster.h"

void Monster::DrawMonster(HDC&)
{

}

void Monster::ObjectUpdate(TOTALDATA mData, int i)
{
	SetPosition(mData.mdata[i].pos);
	GetCollider()->SetOffset(mData.mdata[i].offset);
}

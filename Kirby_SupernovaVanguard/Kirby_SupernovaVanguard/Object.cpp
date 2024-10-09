#include "Object.h"

void DrawPlayer(HDC hdc, Object* p)
{
	int left = p->GetPos().x - p->GetRadius();
	int right = p->GetPos().x + p->GetRadius();
	int top = p->GetPos().y - p->GetRadius();
	int bottom = p->GetPos().y + p->GetRadius();
	Ellipse(hdc, left, top, right, bottom);
}
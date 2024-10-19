#include "Rectangle2D.h"

void Rectangle2D::DrawCollider(HDC& hdc)
{
	int left = GetPosition().x - size;
	int right = GetPosition().x + size;
	int top = GetPosition().y - size;
	int bottom = GetPosition().y + size;
	Rectangle(hdc, left, top, right, bottom);
}
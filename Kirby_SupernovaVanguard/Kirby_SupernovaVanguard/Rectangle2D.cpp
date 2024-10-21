#include "Rectangle2D.h"

void Rectangle2D::DrawCollider(HDC& hdc)
{
	int left = GetPosition().x - width;
	int right = GetPosition().x + width;
	int top = GetPosition().y - height;
	int bottom = GetPosition().y + height;
	Rectangle(hdc, left, top, right, bottom);
}
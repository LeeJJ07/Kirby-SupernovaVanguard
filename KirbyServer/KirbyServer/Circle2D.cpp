#include "Circle2D.h"

void Circle2D::DrawCollider(HDC& hdc)
{
	int left = GetPosition().x - radius;
	int right = GetPosition().x + radius;
	int top = GetPosition().y - radius;
	int bottom = GetPosition().y + radius;
	Ellipse(hdc, left, top, right, bottom);
}
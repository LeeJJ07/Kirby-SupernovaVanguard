#include "Rectangle2D.h"

void Rectangle2D::DrawCollider(HDC& hdc, int& ID)
{
	int left = GetPosition().x - width;
	int right = GetPosition().x + width;
	int top = GetPosition().y - height;
	int bottom = GetPosition().y + height;
	Rectangle(hdc, left, top, right, bottom);

	CString t;

	t.Format(_T("%d"), ID);

	SetTextColor(hdc, RGB(0, 0, 0));

	TextOut(hdc, GetPosition().x, GetPosition().y, t, t.GetLength());
}
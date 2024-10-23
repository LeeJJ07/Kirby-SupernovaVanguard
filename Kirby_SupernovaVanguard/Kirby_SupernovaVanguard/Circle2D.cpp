#include "Circle2D.h"

void Circle2D::DrawCollider(HDC& hdc, int& ID)
{
	int left = GetPosition().x - radius;
	int right = GetPosition().x + radius;
	int top = GetPosition().y - radius;
	int bottom = GetPosition().y + radius;
	Ellipse(hdc, left, top, right, bottom);

	CString t;

	t.Format(_T("%d"), ID);

	SetTextColor(hdc, RGB(0, 0, 0));

	TextOut(hdc, GetPosition().x, GetPosition().y, t, t.GetLength());
}

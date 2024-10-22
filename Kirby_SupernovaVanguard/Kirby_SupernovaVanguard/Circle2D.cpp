#include "Circle2D.h"

void Circle2D::DrawCollider(HDC& hdc, int& ID)
{
	int left = GetPosition().x - radius * 3;
	int right = GetPosition().x + radius * 3;
	int top = GetPosition().y - radius * 3;
	int bottom = GetPosition().y + radius * 3;
	Ellipse(hdc, left, top, right, bottom);

	CString t;

	t.Format(_T("%d"), ID);

	SetTextColor(hdc, RGB(0, 0, 0));

	TextOut(hdc, GetPosition().x, GetPosition().y, t, t.GetLength());
}

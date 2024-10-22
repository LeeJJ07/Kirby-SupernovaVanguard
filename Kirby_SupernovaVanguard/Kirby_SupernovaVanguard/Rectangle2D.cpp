#include "Rectangle2D.h"

void Rectangle2D::DrawCollider(HDC& hdc, int& ID)
{
	POINT point[4];
	point[0] = { GetPosition().x - width ,GetPosition().y - height };
	point[1] = { GetPosition().x + width ,GetPosition().y - height };
	point[2] = { GetPosition().x + width ,GetPosition().y + height };
	point[3] = { GetPosition().x - width ,GetPosition().y + height };
	/*int left = GetPosition().x - width;
	int right = GetPosition().x + width;
	int top = GetPosition().y - height;
	int bottom = GetPosition().y + height;
	Rectangle(hdc, left, top, right, bottom);*/

	float fRad = 3.14 / 180 * angle;

	for (int i = 0; i < 4; i++)
	{
		point[i].x = point[i].x * cos(angle) - point[i].y * sin(angle);
		point[i].y = point[i].x * sin(angle) + point[i].y * cos(angle);
		point[i].x += GetPosition().x;
		point[i].y += GetPosition().y;
	}

	Polygon(hdc, point, 4);

	CString t;

	t.Format(_T("%d"), ID);

	SetTextColor(hdc, RGB(0, 0, 0));

	TextOut(hdc, GetPosition().x, GetPosition().y, t, t.GetLength());
}
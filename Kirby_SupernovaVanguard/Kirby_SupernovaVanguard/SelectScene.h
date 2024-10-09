#pragma once

#include <vector>
#include "framework.h"
#include "Object.h"

#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
using namespace std;

class SelectScene
{
private:
	HBITMAP hBackImage;
	BITMAP bitBack;

	vector<Image*> pImg;
	vector<int> w, h;

	ULONG_PTR g_GdiPlusToken;
	GdiplusStartupInput gpsi;

public:
	SelectScene();
	~SelectScene();

	void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc, RECT& rectView, vector<Object*>& clients);
	void DeleteBitmap();

	int SelectCharacter(Object* client, RECT& rectView);
};


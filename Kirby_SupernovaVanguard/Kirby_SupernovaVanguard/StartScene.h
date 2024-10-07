#pragma once
#include <vector>
#include "framework.h"

#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
using namespace std;

class StartScene
{
private:
	HBITMAP hBackImage;
	BITMAP bitBack;

	int objectNum, idx;
	vector<vector<POINT>> objects;

	vector<Image*> pImg;
	vector<int> w, h;

	ULONG_PTR g_GdiPlusToken;
	GdiplusStartupInput gpsi;

public:
	StartScene();
	~StartScene();

	void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc, RECT& rectView, bool& canGoToNext);
	void DeleteBitmap();
};


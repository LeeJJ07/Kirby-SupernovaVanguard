#pragma once
#include <vector>
#include "framework.h"

#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
using namespace std;

enum EStartState { LOADING, WAITING };
class StartScene
{
private:
	HBITMAP hBackImage;
	BITMAP bitBack;

	int objectNum, idx;
	vector<vector<POINT>> objects;

	Image* pImg;
	int w, h;
public:
	StartScene();
	~StartScene();

	void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc, RECT& rect);
	void DeleteBitmap();

	int GetObjectNum() { return objectNum; }
};


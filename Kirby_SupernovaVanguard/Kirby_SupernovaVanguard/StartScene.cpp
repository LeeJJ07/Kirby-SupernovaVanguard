#define _CRT_SECURE_NO_WARNINGS
#include "StartScene.h"



ULONG_PTR g_GdiPlusToken;

StartScene::StartScene()
{
	objects.resize(6);
	FILE* file = fopen("Datas/PosData.txt", "r");
	if (file != NULL)
	{
		int x, y;
		while (fscanf_s(file, "%d %d", &x, &y) != EOF)
		{
			if (x == -1 && y == -1)
			{
				objectNum++;
				continue;
			}
			objects[objectNum].push_back({ x,y });
		}
	}
	fclose(file);

	// >> : background image
	hBackImage = (HBITMAP)LoadImage(NULL, TEXT("Images/Backgrounds/Start.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBackImage == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("back 이미지 로드 에러"), _T("에러"), MB_OK);
	}
	else
		GetObject(hBackImage, sizeof(BITMAP), &bitBack);

	idx = 0;
	objectNum = 0;

	static GdiplusStartupInput gpsi;
	GdiplusStartup(&g_GdiPlusToken, &gpsi, NULL);

	pImg = nullptr;
	pImg = Image::FromFile((WCHAR*)L"Images/Backgrounds/ShootingStar.png");

	w = pImg->GetWidth();
	h = pImg->GetHeight();
}

StartScene::~StartScene()
{}

void StartScene::DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc, RECT &rectView)
{
	HDC hDoubleBufferDC;
	HBITMAP hDoubleBufferImage = nullptr, hOldDoubleBufferBitmap;


	hDoubleBufferDC = CreateCompatibleDC(hdc);
	if (hDoubleBufferImage == NULL)
		hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
	hOldDoubleBufferBitmap = (HBITMAP)SelectObject(hDoubleBufferDC, hDoubleBufferImage);

	Graphics graphics(hDoubleBufferDC);
	
	if (objectNum == 0)
	{
		static int cccc = 0;
		if (cccc < 3)
		{
			// 왼쪽 절반을 그립니다.
			Rect destRect(objects[objectNum][idx].x, objects[objectNum][idx].y, w / 2, h);
			Rect srcRect(0, 0, w / 2, h); // 이미지의 왼쪽 절반 소스 영역
			graphics.DrawImage(pImg, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}
		else
		{
			if (cccc > 6) cccc = 0;
			// 오른쪽 절반을 그립니다.
			Rect destRect(objects[objectNum][idx].x, objects[objectNum][idx].y, w / 2, h);
			Rect srcRect(w / 2, 0, w / 2, h); // 이미지의 오른쪽 절반 소스 영역
			graphics.DrawImage(pImg, destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}
		cccc++;
		
		/*if (idx & 1)
			graphics.DrawImage(pImg, objects[objectNum][idx].x, objects[objectNum][idx].y, w, h);
		else
			graphics.DrawImage(pImg, objects[objectNum][idx].x + w / 2, objects[objectNum][idx].y, w, h);*/
	}
	else if (objectNum < 6)
	{

	}
	else
	{
		HDC hMemDC;
		HBITMAP hOldBitmap;

		{
			hMemDC = CreateCompatibleDC(hDoubleBufferDC);
			hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);

			BitBlt(hDoubleBufferDC, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);

			SelectObject(hMemDC, hOldBitmap);
			DeleteDC(hMemDC);
		}
	}

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hDoubleBufferDC, 0, 0, SRCCOPY);

	SelectObject(hDoubleBufferDC, hOldDoubleBufferBitmap);
	DeleteDC(hDoubleBufferDC);

	if (hDoubleBufferImage != nullptr)
		DeleteObject(hDoubleBufferImage);  // 자원 해제

	if (objectNum >= 6) 
		return;
	idx++;
	if (idx == objects[objectNum].size())
	{
		objectNum++;
		idx = 0;
	}
}

void StartScene::DeleteBitmap()
{
	DeleteObject(hBackImage);
	GdiplusShutdown(g_GdiPlusToken);
}

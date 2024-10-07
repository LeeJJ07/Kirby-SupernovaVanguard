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

	pImg.resize(8);
	w.resize(8), h.resize(8);

	pImg[0] = Image::FromFile((WCHAR*)L"Images/Backgrounds/ShootingStar.png");
	pImg[1] = Image::FromFile((WCHAR*)L"Images/Backgrounds/k.png");
	pImg[2] = Image::FromFile((WCHAR*)L"Images/Backgrounds/i.png");
	pImg[3] = Image::FromFile((WCHAR*)L"Images/Backgrounds/r.png");
	pImg[4] = Image::FromFile((WCHAR*)L"Images/Backgrounds/b.png");
	pImg[5] = Image::FromFile((WCHAR*)L"Images/Backgrounds/y.png");
	pImg[6] = Image::FromFile((WCHAR*)L"Images/Backgrounds/subTitle.png");
	pImg[7] = Image::FromFile((WCHAR*)L"Images/Backgrounds/PressButton.png");

	for (int i = 0; i < 8; i++)
	{
		w[i] = pImg[i]->GetWidth();
		h[i] = pImg[i]->GetHeight();
	}
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
		Rect destRect(objects[objectNum][idx].x, objects[objectNum][idx].y - 150, w[objectNum] / 2, h[objectNum]);
		if ((idx / 20) % 2)
		{
			Rect srcRect(0, 0, w[objectNum] / 2, h[objectNum]); // 이미지의 왼쪽 절반 소스 영역
			graphics.DrawImage(pImg[objectNum], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}
		else
		{
			Rect srcRect(w[objectNum] / 2, 0, w[objectNum] / 2, h[objectNum]); // 이미지의 오른쪽 절반 소스 영역
			graphics.DrawImage(pImg[objectNum], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}
	}
	else if (objectNum < 6)
	{		
		for (int i = 1; i <= objectNum; i++)
		{
			if (objectNum == i)
			{
				Rect destRect(objects[i][idx].x - w[i] / 2, objects[i][idx].y - h[i] / 2, w[i], h[i]);
				Rect srcRect(0, 0, w[i], h[i]);
				graphics.DrawImage(pImg[i], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
			}
			else
			{
				Rect destRect(objects[i][objects[i].size() - 1].x - w[i] / 2, objects[i][objects[i].size() - 1].y - h[i] / 2, w[i], h[i]);
				Rect srcRect(0, 0, w[i], h[i]);
				graphics.DrawImage(pImg[i], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
			}
		}
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
		for (int i = 1; i < objectNum; i++)
		{
			Rect destRect(objects[i][objects[i].size() - 1].x - w[i] / 2, objects[i][objects[i].size() - 1].y - h[i] / 2, w[i], h[i]);
			Rect srcRect(0, 0, w[i], h[i]);
			graphics.DrawImage(pImg[i], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}
		Rect destRect(rectView.right / 2 - w[6] / 2, rectView.bottom - 250, w[6], h[6]);
		Rect srcRect(0, 0, w[6], h[6]);
		graphics.DrawImage(pImg[6], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);

		static int tempidx = 0;
		tempidx++;
		if ((tempidx / 50) % 2)
		{
			Rect destRect(rectView.right / 2 - w[7] / 2, - 200, w[7], h[7]);
			Rect srcRect(0, 0, w[7], h[7]);
			graphics.DrawImage(pImg[7], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}
		if (tempidx > 499) tempidx = 0;
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

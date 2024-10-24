#define _CRT_SECURE_NO_WARNINGS
#include "SelectScene.h"

SelectScene::SelectScene()
{
	// >> : background image
	hBackImage = (HBITMAP)LoadImage(NULL, TEXT("Images/Backgrounds/Select.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBackImage == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("back 이미지 로드 에러"), _T("에러"), MB_OK);
	}
	else
		GetObject(hBackImage, sizeof(BITMAP), &bitBack);

	GdiplusStartup(&g_GdiPlusToken, &gpsi, NULL);

	pImg.resize(10);
	w.resize(10), h.resize(10);

	pImg[0] = Image::FromFile((WCHAR*)L"Images/Backgrounds/kirby.png");
	pImg[1] = Image::FromFile((WCHAR*)L"Images/Backgrounds/ddd.png");
	pImg[2] = Image::FromFile((WCHAR*)L"Images/Backgrounds/metanight.png");
	pImg[3] = Image::FromFile((WCHAR*)L"Images/Backgrounds/maboroa.png");

	pImg[4] = Image::FromFile((WCHAR*)L"Images/Backgrounds/playerText.png");
	pImg[5] = Image::FromFile((WCHAR*)L"Images/Backgrounds/selectText.png");

	pImg[6] = Image::FromFile((WCHAR*)L"Images/Backgrounds/kirbySelected.png");
	pImg[7] = Image::FromFile((WCHAR*)L"Images/Backgrounds/dddSelected.png");
	pImg[8] = Image::FromFile((WCHAR*)L"Images/Backgrounds/metanightSelected.png");
	pImg[9] = Image::FromFile((WCHAR*)L"Images/Backgrounds/maboroaSelected.png");


	for (int i = 0; i < 10; i++)
	{
		w[i] = pImg[i]->GetWidth();
		h[i] = pImg[i]->GetHeight();
	}
}

SelectScene::~SelectScene()
{
	DeleteBitmap();
}

void SelectScene::DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc, RECT& rectView, vector<Object*>& clients)
{
	HDC hDoubleBufferDC;
	HBITMAP hDoubleBufferImage = nullptr, hOldDoubleBufferBitmap;


	hDoubleBufferDC = CreateCompatibleDC(hdc);
	if (hDoubleBufferImage == NULL)
		hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
	hOldDoubleBufferBitmap = (HBITMAP)SelectObject(hDoubleBufferDC, hDoubleBufferImage);

	Graphics graphics(hDoubleBufferDC);

	HDC hMemDC;
	HBITMAP hOldBitmap;
	{
		hMemDC = CreateCompatibleDC(hDoubleBufferDC);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);

		BitBlt(hDoubleBufferDC, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}

	HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hOldBrush;

	hOldBrush = (HBRUSH)SelectObject(hDoubleBufferDC, blackBrush);

	// 가운데 캐릭터 선택 박스 및 해당 캐릭터
	for (int i = 0; i < 4; i++)
	{
		int centerX = rectView.right / 4 * ((i % 2) * 2 + 1) + (i % 2 - 0.5) * -250;
		int centerY = rectView.bottom / 4 * ((i / 2) * 2 + 1) - (i / 2) * 150;

		Rectangle(hDoubleBufferDC, centerX - 300, centerY - 250, centerX + 300, centerY + 150);

		Rect destRect(centerX - w[i] / 2, centerY - h[i] / 2 - 50, w[i], h[i]);
		Rect srcRect(0, 0, w[i], h[i]);
		graphics.DrawImage(pImg[i], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
	}

	// select scene 맨 아래 네모 박스 및 캐릭터
	for (int i = 0; i < 4; i++)
	{
		if ((Player*)(clients[i]) == NULL) continue;

		int centerX = rectView.right / 5 * (i + 1);
		int centerY = rectView.bottom - 170;

		Rectangle(hDoubleBufferDC, centerX - 100, centerY - 100, centerX + 100, centerY + 100);

		int index = SelectCharacter((Player*)(clients[i]), rectView);
		Rect destRect(centerX - w[index] / 2, centerY - h[index] / 2, w[index], h[index]);
		Rect srcRect(0, 0, w[index], h[index]);
		graphics.DrawImage(pImg[index],
			destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
	}

	SelectObject(hDoubleBufferDC, hOldBrush);
	DeleteObject(blackBrush);

	// 양 옆 PLAYER & SELECT
	for (int i = 4; i < 6; i++)
	{
		int centerX = (i - 4) *1600 + 150;
		int centerY = rectView.bottom / 2;

		Rect destRect(centerX - w[i] / 2, centerY - h[i] / 2 - 150, w[i], h[i]);
		Rect srcRect(0, 0, w[i], h[i]);
		graphics.DrawImage(pImg[i], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
	}

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hDoubleBufferDC, 0, 0, SRCCOPY);

	SelectObject(hDoubleBufferDC, hOldDoubleBufferBitmap);
	DeleteDC(hDoubleBufferDC);


	if (hDoubleBufferImage != nullptr)
		DeleteObject(hDoubleBufferImage);  // 자원 해제
}

void SelectScene::DeleteBitmap()
{
	DeleteObject(hBackImage);
	GdiplusShutdown(g_GdiPlusToken);
}

int SelectScene::SelectCharacter(Player* client, RECT& rectView)
{
	for (int i = 0; i < 4; i++)
	{
		int centerX = rectView.right / 4 * ((i % 2) * 2 + 1) + (i % 2 - 0.5) * -250;
		int centerY = rectView.bottom / 4 * ((i / 2) * 2 + 1) - (i / 2) * 150;

		if (client->GetMousePosition().x > centerX - 300
			&& client->GetMousePosition().x < centerX + 300
			&& client->GetMousePosition().y > centerY - 250
			&& client->GetMousePosition().y < centerY + 150)
		{
			client->SetCharacterType((ECharacterType)(i + 1));
			return i + 6;
		}
	}
	return client->GetCharacterType() + 6;
}
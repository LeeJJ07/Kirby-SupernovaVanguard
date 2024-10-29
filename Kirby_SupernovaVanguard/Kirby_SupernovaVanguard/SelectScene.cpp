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

	pImg[4] = Image::FromFile((WCHAR*)L"Images/Backgrounds/SelectPlayer.png");

	pImg[5] = Image::FromFile((WCHAR*)L"Images/Backgrounds/kirbySelected.png");
	pImg[6] = Image::FromFile((WCHAR*)L"Images/Backgrounds/dddSelected.png");
	pImg[7] = Image::FromFile((WCHAR*)L"Images/Backgrounds/metanightSelected.png");
	pImg[8] = Image::FromFile((WCHAR*)L"Images/Backgrounds/maboroaSelected.png");


	for (int i = 0; i < 9; i++)
	{
		w[i] = pImg[i]->GetWidth();
		h[i] = pImg[i]->GetHeight();
	}
}

SelectScene::~SelectScene()
{
	DeleteBitmap();
}

void SelectScene::DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc, RECT& rectView, vector<Object*>& clients, POINT curMousePos)
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
	// 가운데 캐릭터 선택 박스 및 해당 캐릭터
	for (int i = 0; i < 4; i++)
	{
		int centerX = rectView.right / 2 + (float)(i - 1.5) * 400;
		int centerY = rectView.bottom / 2;

		if (curMousePos.x > centerX - 250
			&& curMousePos.x < centerX + 250
			&& curMousePos.y > centerY - 250
			&& curMousePos.y < centerY + 250)
		{
			Rect destRect(centerX - w[i + 5] / 2, centerY - h[i + 5] / 2, w[i + 5], h[i + 5]);
			Rect srcRect(0, 0, w[i + 5], h[i + 5]);
			graphics.DrawImage(pImg[i + 5], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
			continue;
		}

		Rect destRect(centerX - w[i] / 2, centerY - h[i] / 2, w[i], h[i]);
		Rect srcRect(0, 0, w[i], h[i]);
		graphics.DrawImage(pImg[i], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
	}

	// select scene 맨 아래 네모 박스 및 캐릭터
	for (int i = 0; i < 4; i++)
	{
		if ((Player*)(clients[i]) == NULL) continue;

		int centerX = rectView.right / 5 * (i + 1);
		int centerY = rectView.bottom - 170;

		int index = SelectCharacter((Player*)(clients[i]), rectView);
		Rect destRect(centerX - w[index] / 2, centerY - h[index] / 2, w[index], h[index]);
		Rect srcRect(0, 0, w[index], h[index]);
		graphics.DrawImage(pImg[index],
			destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
	}


	// 텍스트 문구
	{
		int centerX = rectView.right / 2;
		int centerY = rectView.bottom / 2 - 400;

		Rect destRect(centerX - w[4] / 2, centerY - h[4] / 2, w[4], h[4]);
		Rect srcRect(0, 0, w[4], h[4]);
		graphics.DrawImage(pImg[4], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
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
		int centerX = rectView.right / 2 + (float)(i - 1.5) * 400;
		int centerY = rectView.bottom / 2;

		if (client->GetMousePosition().x > centerX - 250
			&& client->GetMousePosition().x < centerX + 250
			&& client->GetMousePosition().y > centerY - 250
			&& client->GetMousePosition().y < centerY + 250)
		{
			client->SetCharacterType((ECharacterType)(i + 1));
			return i + 5;
		}
	}
	return client->GetCharacterType() + 5;
}
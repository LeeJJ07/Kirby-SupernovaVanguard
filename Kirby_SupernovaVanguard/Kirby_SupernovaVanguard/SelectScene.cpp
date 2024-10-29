#define _CRT_SECURE_NO_WARNINGS
#include "SelectScene.h"

float clientWidth[5] = { 0, 0, 0.5, 1, 1.5 };
int charIndex[4] = {0, 0, 0, 0};

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

	pImg.resize(17);
	w.resize(17), h.resize(17);

	pImg[0] = Image::FromFile((WCHAR*)L"Images/Backgrounds/kirby.png");
	pImg[1] = Image::FromFile((WCHAR*)L"Images/Backgrounds/ddd.png");
	pImg[2] = Image::FromFile((WCHAR*)L"Images/Backgrounds/metanight.png");
	pImg[3] = Image::FromFile((WCHAR*)L"Images/Backgrounds/maboroa.png");

	pImg[4] = Image::FromFile((WCHAR*)L"Images/Backgrounds/SelectPlayer.png");

	pImg[5] = Image::FromFile((WCHAR*)L"Images/Backgrounds/kirbySelected.png");
	pImg[6] = Image::FromFile((WCHAR*)L"Images/Backgrounds/dddSelected.png");
	pImg[7] = Image::FromFile((WCHAR*)L"Images/Backgrounds/metanightSelected.png");
	pImg[8] = Image::FromFile((WCHAR*)L"Images/Backgrounds/maboroaSelected.png");

	pImg[9] = Image::FromFile((WCHAR*)L"Images/Backgrounds/player1.png");
	pImg[10] = Image::FromFile((WCHAR*)L"Images/Backgrounds/player2.png");
	pImg[11] = Image::FromFile((WCHAR*)L"Images/Backgrounds/player3.png");
	pImg[12] = Image::FromFile((WCHAR*)L"Images/Backgrounds/player4.png");

	pImg[13] = Image::FromFile((WCHAR*)L"Images/Backgrounds/player1select.png");
	pImg[14] = Image::FromFile((WCHAR*)L"Images/Backgrounds/player2select.png");
	pImg[15] = Image::FromFile((WCHAR*)L"Images/Backgrounds/player3select.png");
	pImg[16] = Image::FromFile((WCHAR*)L"Images/Backgrounds/player4select.png");


	for (int i = 0; i < 17; i++)
	{
		w[i] = pImg[i]->GetWidth();
		h[i] = pImg[i]->GetHeight();
	}
}

SelectScene::~SelectScene()
{
	DeleteBitmap();
}

void SelectScene::DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc, RECT& rectView, vector<Object*>& clients, POINT curMousePos, TOTALDATA& tData)
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

	int clientLen = 0;
	while (clientLen < PLAYERNUM && (Player*)clients[clientLen] != NULL) clientLen++;

	for (int i = 0; i < clientLen; i++)
	{
		if ((Player*)(clients[i]) == NULL) break;
		int centerX = rectView.right / 2 + (float)(i - clientWidth[clientLen]) * 350;
		int centerY = rectView.bottom / 2 + 300;
		int radius = 130;

		// 반투명한 색상 생성 (ARGB: Alpha, Red, Green, Blue)
		Color semiTransparentColor(200, 0, 0, 0); // 알파값이 128로 반투명한 파란색
		SolidBrush semiTransparentBrush(semiTransparentColor);

		// 반투명 원 그리기
		graphics.FillEllipse(&semiTransparentBrush, centerX - radius, centerY - radius, radius * 2, radius * 2);
		{
			if(!tData.udata[i].inGameStart)
				charIndex[i] = SelectCharacter((Player*)(clients[i]), rectView);
			Rect destRect(centerX - 150 / 2, centerY - 150 / 2, 150, 150);
			Rect srcRect(0, 0, w[charIndex[i]], h[charIndex[i]]);
			graphics.DrawImage(pImg[charIndex[i]],
				destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}

		if(!tData.udata[i].inGameStart)
		{
			Rect destRect(centerX - w[i + 9] * 1.5f, centerY - 160, w[i + 9] * 3, h[i + 9] * 3);
			Rect srcRect(0, 0, w[i + 9], h[i + 9]);
			graphics.DrawImage(pImg[i + 9], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}
		else
		{
			Rect destRect(centerX - w[i + 13] / 2 * 4, centerY - h[i+13] / 2 * 4, w[i + 13] * 4, h[i + 13] * 4);
			Rect srcRect(0, 0, w[i + 13], h[i + 13]);
			graphics.DrawImage(pImg[i + 13], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
		}
	}

	// 가운데 캐릭터 선택 박스 및 해당 캐릭터
	for (int i = 0; i < 4; i++)
	{
		int centerX = rectView.right / 2 + (float)(i - 1.5) * 400;
		int centerY = rectView.bottom / 2 - 100;

		Color semiTransparentColor(128, 0, 0, 0);
		SolidBrush semiTransparentBrush(semiTransparentColor);

		// 반투명 사각형 그리기
		graphics.FillRectangle(&semiTransparentBrush, centerX - 180, centerY - 220, 360, 440);

		if (curMousePos.x > centerX - 200
			&& curMousePos.x < centerX + 200
			&& curMousePos.y > centerY - 250
			&& curMousePos.y < centerY + 250)
		{
			Rect destRect(centerX - 320 / 2, centerY - 320 / 2, 320, 320);
			Rect srcRect(0, 0, w[i + 5], h[i + 5]);
			graphics.DrawImage(pImg[i + 5], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
			continue;
		}

		Rect destRect(centerX - 180 / 2, centerY - 180 / 2, 180, 180);
		Rect srcRect(0, 0, w[i], h[i]);
		graphics.DrawImage(pImg[i], destRect, srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height, UnitPixel);
	}

	// 텍스트 문구
	{
		int centerX = rectView.right / 2;
		int centerY = rectView.bottom / 2 - 450;

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
		int centerY = rectView.bottom / 2 - 100;

		if (client->GetMousePosition().x > centerX - 200
			&& client->GetMousePosition().x < centerX + 200
			&& client->GetMousePosition().y > centerY - 250
			&& client->GetMousePosition().y < centerY + 250)
		{
			client->SetCharacterType((ECharacterType)(i + 1));
			return i + 5;
		}
	}
	return client->GetCharacterType() + 4;
}
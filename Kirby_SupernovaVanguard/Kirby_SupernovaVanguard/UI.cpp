#include "UI.h"

void UI::SetUIAni()
{
    Animation* tempAni[1] = { nullptr };

    switch (uiType)
    {
    case GAMEOVER:
        tempAni[0] = imageDatas[gameover];
        break;
    case RESTART:
        tempAni[0] = imageDatas[ObjectImage::Restart];
        break;
    }
    for (int i = 0; i < 1; i++)
    {
        Animation* temp = new Animation(tempAni[i]->GetCnt(), tempAni[i]->GetSpacingX(),
            tempAni[i]->GetR(), tempAni[i]->GetG(), tempAni[i]->GetB(), tempAni[i]->GetCog(),
            tempAni[i]->GetLengths(), tempAni[i]->Height(), tempAni[i]->GetFilePath());
        temp->Load();

        ani.insert({ (EUITYPE)uiType, temp });
    }
}

void UI::DrawUI(HDC& hdc, Camera& camera)
{
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, ani[uiType]->GetBitmap());

    ani[uiType]->IncreaseIdx();

    int width = ani[uiType]->GetCurWidth() - 1;
    int height = ani[uiType]->GetHeight() - 1;
    int left = camera.GetCameraPos().x - ani[uiType]->GetCurCog().x + ani[uiType]->GetPrevWidth();
    int top = camera.GetCameraPos().y * 1.2f - ani[uiType]->GetCurCog().y;

    HDC hTempDC = CreateCompatibleDC(hdc);
    HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hTempDC, hTempBitmap);

    StretchBlt(
        hTempDC, 0, 0, width, height,
        hMemDC, ani[uiType]->GetPrevWidth(), 0, width, height,
        SRCCOPY
    );

    TransparentBlt(
        hdc, left, top, width, height,
        hTempDC, 0, 0, width, height,
        RGB(ani[uiType]->GetR(), ani[uiType]->GetG(), ani[uiType]->GetB())
    );

    SelectObject(hTempDC, hOldBitmap);
    DeleteDC(hTempDC);
    DeleteObject(hTempBitmap);

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}
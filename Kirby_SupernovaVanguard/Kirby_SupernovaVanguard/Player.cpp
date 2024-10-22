#include "Player.h"

void Player::ObjectUpdate(TOTALDATA pData, int i)
{
	SetPosition(pData.udata[i].pos);
	GetCollider()->SetOffset(pData.udata[i].offset);
	SetLookingDir(pData.udata[i].lookingDir);
	SetIsInGame(pData.udata[i].inGameStart);
	SetMousePosition(pData.udata[i].mousePos);
}

void Player::DrawPlayer(HDC& hdc)
{
    characterState = ATTACK;

    Animation* tempAni = nullptr;
    
    switch (characterType)
    {
    case KIRBY:
        if (characterState == IDLE)  tempAni = imageDatas[kirby_Idle];
        else if(characterState == WALK) tempAni = imageDatas[kirby_Walk];
        else if(characterState == ATTACK) tempAni = imageDatas[kirby_Attack];
        break;
    case DDD:
        if (characterState == IDLE)  tempAni = imageDatas[ddd_Idle];
        else if (characterState == WALK) tempAni = imageDatas[ddd_Walk];
        else if (characterState == ATTACK) tempAni = imageDatas[ddd_Attack];
        break;
    case METANIHGT:
        if (characterState == IDLE)  tempAni = imageDatas[meta_Idle];
        else if (characterState == WALK) tempAni = imageDatas[meta_Walk];
        else if (characterState == ATTACK) tempAni = imageDatas[meta_Attack];
        break;
    case MABOROA:
        if (characterState == IDLE)  tempAni = imageDatas[maboroa_Idle];
        else if (characterState == WALK) tempAni = imageDatas[maboroa_Walk];
        else if (characterState == ATTACK) tempAni = imageDatas[maboroa_Attack];
        break;
    }

    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, tempAni->GetBitmap());

    tempAni->IncreaseIdx();

    int width = tempAni->GetCurWidth() + tempAni->GetSpacingX();
    int height = tempAni->GetHeight() - 1;
    int left = GetPosition().x - tempAni->GetCurCog().x + tempAni->GetPrevWidth();
    int top = GetPosition().y - tempAni->GetCurCog().y;

    HDC hTempDC = CreateCompatibleDC(hdc);
    HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hTempDC, hTempBitmap);

    if (mousePosition.x > position.x)
    {
        StretchBlt(
            hTempDC, 0, 0, width, height,
            hMemDC, tempAni->GetPrevWidth(), 0, width, height,
            SRCCOPY
        );
    }
    else
    {
        StretchBlt(
            hTempDC, width, 0, -width, height,
            hMemDC, tempAni->GetPrevWidth(), 0, width, height,
            SRCCOPY
        );
    }

    TransparentBlt(
        hdc, left, top, width, height,        
        hTempDC, 0, 0, width, height,         
        RGB(tempAni->GetR(), tempAni->GetG(), tempAni->GetB()) 
    );

    SelectObject(hTempDC, hOldBitmap);
    DeleteDC(hTempDC);
    DeleteObject(hTempBitmap);

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}
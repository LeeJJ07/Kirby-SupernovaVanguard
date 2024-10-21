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

    int left = GetPosition().x - tempAni->GetCurCog().x + tempAni->GetPrevWidth();
    int top = GetPosition().y - tempAni->GetCurCog().y;
    TransparentBlt(
        hdc, left, top,
        tempAni->GetCurWidth() + tempAni->GetSpacingX(),      //크기
        tempAni->GetHeight() - 1,        //크기
        hMemDC,
        tempAni->GetPrevWidth(), 0, tempAni->GetCurWidth() + tempAni->GetSpacingX(), tempAni->Height() - 1,
        RGB(tempAni->GetR(), tempAni->GetG(), tempAni->GetB())
    );

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}
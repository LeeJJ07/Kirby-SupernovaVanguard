#include "Player.h"
#include "Camera.h"

void Player::ObjectUpdate(TOTALDATA& pData, int i)
{
	SetPosition(pData.udata[i].pos);
	GetCollider()->SetOffset(pData.udata[i].offset);
	SetLookingDir(pData.udata[i].lookingDir);
	SetIsInGame(pData.udata[i].inGameStart);
	SetMousePosition(pData.udata[i].mousePos);
    SetmaxHealth(pData.udata[i].maxHealth);
    SetcurHealth(pData.udata[i].curHealth);
    SetplayerSize(pData.udata[i].radius);
    SetCharacterState(pData.udata[i].curState);
    
    switch (GetCollider()->GetColliderShape())
    {
    case CIRCLE:
        ((Circle2D*)GetCollider())->SetRadius(pData.udata[i].radius);
        break;
    case RECTANGLE:

        break;
    }
}

void Player::SetPlayerAni()
{
    Animation* tempAni[3] = { nullptr };

    switch (characterType)
    {
    case KIRBY:
        tempAni[0] = imageDatas[kirby_Idle];
        tempAni[1] = imageDatas[kirby_Walk];
        tempAni[2] = imageDatas[kirby_Attack];
        break;
    case DDD:
        tempAni[0] = imageDatas[ddd_Idle];
        tempAni[1] = imageDatas[ddd_Walk];
        tempAni[2] = imageDatas[ddd_Attack];
        break;
    case METANIHGT:
        tempAni[0] = imageDatas[meta_Idle];
        tempAni[1] = imageDatas[meta_Walk];
        tempAni[2] = imageDatas[meta_Attack];
        break;
    case MABOROA:
        tempAni[0] = imageDatas[maboroa_Idle];
        tempAni[1] = imageDatas[maboroa_Walk];
        tempAni[2] = imageDatas[maboroa_Attack];
        break;
    }
    for (int i = 0; i < 3; i++)
    {
        Animation* temp = new Animation(tempAni[i]->GetCnt(), tempAni[i]->GetSpacingX(), 
            tempAni[i]->GetR(), tempAni[i]->GetG(), tempAni[i]->GetB(), tempAni[i]->GetCog(), 
            tempAni[i]->GetLengths(), tempAni[i]->Height(), tempAni[i]->GetFilePath());
        temp->Load();

        ani.insert({ (ECharacterState)i, temp });
    }
}

void Player::DrawPlayer(HDC& hdc, ActionData& aD)
{
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, ani[characterState]->GetBitmap());

    ani[characterState]->IncreaseIdx();

    int width = ani[characterState]->GetCurWidth();
    int height = ani[characterState]->GetHeight() - 1;
    int left = GetPosition().x - ani[characterState]->GetCurCog().x + ani[characterState]->GetPrevWidth();
    int top = GetPosition().y - ani[characterState]->GetCurCog().y;

    HDC hTempDC = CreateCompatibleDC(hdc);
    HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hTempDC, hTempBitmap);

    if (mousePosition.x + (camera.GetCameraPos().x - SCREEN_SIZE_X / 2) >  position.x)
    {
        StretchBlt(
            hTempDC, 0, 0, width, height,
            hMemDC, ani[characterState]->GetPrevWidth(), 0, width, height,
            SRCCOPY
        );
    }
    else
    {
        StretchBlt(
            hTempDC, width, 0, -width, height,
            hMemDC, ani[characterState]->GetPrevWidth(), 0, width, height,
            SRCCOPY
        );
    }

    TransparentBlt(
        hdc, left, top, width, height,        
        hTempDC, 0, 0, width, height,         
        RGB(ani[characterState]->GetR(), ani[characterState]->GetG(), ani[characterState]->GetB())
    );

    SelectObject(hTempDC, hOldBitmap);
    DeleteDC(hTempDC);
    DeleteObject(hTempBitmap);

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
}
#include "Monster.h"

void Monster::SetMonsterAni()
{
    Animation* tempAni[6] = { nullptr };
    switch (monsterType)
    {
    case RUNNER:
        tempAni[0] = imageDatas[runner_Walk];
        tempAni[1] = nullptr;
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = nullptr;
        tempAni[5] = nullptr;
        break;
    case SPEAR:
        tempAni[0] = imageDatas[spear_Walk];
        tempAni[1] = imageDatas[spear_Attack];
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = nullptr;
        tempAni[5] = nullptr;
        break;
    case WINGBUG:
        tempAni[0] = imageDatas[wingbug_Walk];
        tempAni[1] = nullptr;
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = nullptr;
        tempAni[5] = nullptr;
        break;
    case FIREMAN:
        tempAni[0] = imageDatas[fireman_Walk];
        tempAni[1] = imageDatas[fireman_Attack];
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = nullptr;
        tempAni[5] = nullptr;
        break;
    case LANDMINE:
        tempAni[0] = imageDatas[landmine_Idle];
        tempAni[1] = nullptr;
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = nullptr;
        tempAni[5] = nullptr;
        break;
    case KUNGFUMAN:
        tempAni[0] = imageDatas[KFM_Walk];
        tempAni[1] = imageDatas[KFM_Attack];
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = imageDatas[KFM_Detect];
        tempAni[5] = nullptr;
        break;
    case GAOGAO:
        tempAni[0] = nullptr;
        tempAni[1] = imageDatas[gaogao_Attack];
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = imageDatas[gaogao_Detect];
        tempAni[5] = imageDatas[gaogao_Stun];
        break;
    case BOSS:
        tempAni[0] = nullptr;
        tempAni[1] = nullptr;
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = nullptr;
        tempAni[5] = nullptr;
        break;
    }

    for (int i = 0; i < 6; i++)
    {
        if (tempAni[i] == nullptr) 
            continue;
        Animation* temp = new Animation(tempAni[i]->GetCnt(), tempAni[i]->GetSpacingX(),
            tempAni[i]->GetR(), tempAni[i]->GetG(), tempAni[i]->GetB(), tempAni[i]->GetCog(),
            tempAni[i]->GetLengths(), tempAni[i]->Height(), tempAni[i]->GetFilePath());
        temp->Load();

        ani.insert({ (EMonsterState)i, temp });
    }
}

void Monster::Draw(HDC& hdc)
{
    if (ani[curState] == nullptr)
    {
        return;
    }
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, ani[curState]->GetBitmap());

    ani[curState]->IncreaseIdx();

    int width = ani[curState]->GetCurWidth();
    int height = ani[curState]->GetHeight() - 1;
    int left = GetPosition().x - ani[curState]->GetCurCog().x + ani[curState]->GetPrevWidth();
    int top = GetPosition().y - ani[curState]->GetCurCog().y;

    HDC hTempDC = CreateCompatibleDC(hdc);
    HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hTempDC, hTempBitmap);

    if (lookingDirection.first > 0)
    {
        StretchBlt(
            hTempDC, 0, 0, width, height,
            hMemDC, ani[curState]->GetPrevWidth(), 0, width, height,
            SRCCOPY
        );
    }
    else
    {
        StretchBlt(
            hTempDC, width, 0, - (width + 1), height,
            hMemDC, ani[curState]->GetPrevWidth(), 0, width, height,
            SRCCOPY
        );
    }

    TransparentBlt(
        hdc, left, top, width, height,
        hTempDC, 0, 0, width, height,
        RGB(ani[curState]->GetR(), ani[curState]->GetG(), ani[curState]->GetB())
    );

    SelectObject(hTempDC, hOldBitmap);
    DeleteDC(hTempDC);
    DeleteObject(hTempBitmap);

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

}

void Monster::ObjectUpdate(TOTALDATA& totalData, int i)
{
	SetPosition(totalData.mdata[i].pos);
    SetLookingDir(totalData.mdata[i].lookingDir);
	GetCollider()->SetOffset(totalData.mdata[i].offset);
    
    SetMonsterType(totalData.mdata[i].monsterType);
    
    if (curState != totalData.mdata[i].curState)
    {
        SetMonsterState(totalData.mdata[i].curState);
    }
}

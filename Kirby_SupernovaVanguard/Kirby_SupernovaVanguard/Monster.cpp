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
        tempAni[1] = nullptr;
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
        tempAni[0] = imageDatas[boss_mode1];
        tempAni[1] = imageDatas[boss_mode2];
        tempAni[2] = nullptr;
        tempAni[3] = nullptr;
        tempAni[4] = nullptr;
        tempAni[5] = imageDatas[boss_eye];
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
    if (monsterType == BOSS)
    {
        BossDraw(hdc);
        return;
    }
    if (ani[curState] == nullptr)
        return;

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
void Monster::BossDraw(HDC& hdc)
{
    HDC hMemDC = CreateCompatibleDC(hdc);

    // 현재 상태에 따른 보스 애니메이션 비트맵 선택 및 그리기
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, ani[curState]->GetBitmap());
    ani[curState]->IncreaseIdx();

    // 보스 애니메이션 크기를 2배로 설정
    int width = ani[curState]->GetCurWidth() * 2;
    int height = ani[curState]->GetHeight() * 2;
    int left = GetPosition().x - ani[curState]->GetCurCog().x * 2 + ani[curState]->GetPrevWidth() * 2;
    int top = GetPosition().y - ani[curState]->GetCurCog().y * 2;

    TransparentBlt(
        hdc, left, top, width, height,
        hMemDC, ani[curState]->GetPrevWidth(), 0, ani[curState]->GetCurWidth(), ani[curState]->GetHeight(),
        RGB(ani[curState]->GetR(), ani[curState]->GetG(), ani[curState]->GetB())
    );

    // 보스 애니메이션 그린 후, hMemDC에서 비트맵 선택 해제
    SelectObject(hMemDC, hOldBitmap);

    // 눈 애니메이션을 그리기 위해 hMemDC에 눈 비트맵 선택
    HBITMAP hEyeBitmap = (HBITMAP)SelectObject(hMemDC, ani[(EMonsterState)5]->GetBitmap());

    // 눈 애니메이션 크기를 2배로 설정
    int eyeWidth = (ani[(EMonsterState)5]->GetCurWidth() - 1) * 1.5f;
    int eyeHeight = ani[(EMonsterState)5]->GetHeight() * 1.5f;
    int eyeLeft = GetPosition().x - ani[(EMonsterState)5]->GetCurCog().x * 1.5f + lookingDirection.first * 5;
    int eyeTop = GetPosition().y - ani[(EMonsterState)5]->GetCurCog().y * 1.5f + lookingDirection.second * 5;

    TransparentBlt(
        hdc, eyeLeft, eyeTop, eyeWidth, eyeHeight,
        hMemDC, 0, 0, ani[(EMonsterState)5]->GetCurWidth() - 1, ani[(EMonsterState)5]->GetHeight(),
        RGB(ani[(EMonsterState)5]->GetR(), ani[(EMonsterState)5]->GetG(), ani[(EMonsterState)5]->GetB())
    );

    // 리소스 해제
    SelectObject(hMemDC, hEyeBitmap);
    DeleteDC(hMemDC);
}

void Monster::ObjectUpdate(int i, TOTALDATA& uData)
{
	SetPosition(uData.mdata[i].pos);
    SetLookingDir(uData.mdata[i].lookingDir);
	GetCollider()->SetOffset(uData.mdata[i].offset);
    SetmaxHealth(uData.mdata[i].maxHealth);
    SetcurHealth(uData.mdata[i].curHealth);
    
    SetMonsterType(uData.mdata[i].monsterType);
    
    if (curState != uData.mdata[i].curState)
    {
        SetMonsterState(uData.mdata[i].curState);
    }
}

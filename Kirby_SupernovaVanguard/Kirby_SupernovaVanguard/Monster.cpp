#include "Monster.h"

void Monster::Draw(HDC& hdc)
{
    // 색상 설정을 위한 브러시 핸들
    HBRUSH brush;

    switch (monsterType)
    {
    case RUNNER:
        brush = CreateSolidBrush(RGB(255, 0, 0)); // 빨강
        break;
    case SPEAR:
        brush = CreateSolidBrush(RGB(255, 165, 0)); // 주황
        break;
    case WINGBUG:
        brush = CreateSolidBrush(RGB(255, 255, 0)); // 노랑
        break;
    case FIREMAN:
        brush = CreateSolidBrush(RGB(0, 255, 0)); // 초록
        break;
    case LANDMINE:
        brush = CreateSolidBrush(RGB(0, 0, 255)); // 파랑
        break;
    default:
        brush = CreateSolidBrush(RGB(255, 255, 255)); // 기본 색상 (흰색)
        break;
    }

    // 브러시 선택
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    // 원 그리기
    int left = GetPosition().x - 10;
    int right = GetPosition().x + 10;
    int top = GetPosition().y - 10;
    int bottom = GetPosition().y + 10;
    Ellipse(hdc, left, top, right, bottom);

    // 이전 브러시 복원 및 브러시 삭제
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);

    // 추가 코드 index 관리
    // drawIndex = (drawIndex + 1)% (현재 애니메이션의 크기)
}

void Monster::ObjectUpdate(TOTALDATA mData, int i)
{
	SetPosition(mData.mdata[i].pos);
	GetCollider()->SetOffset(mData.mdata[i].offset);
    
    SetMonsterType(mData.mdata[i].monsterType);
    
    if (curState != mData.mdata[i].curState)
    {
        drawIndex = 0;
        SetMonsterState(mData.mdata[i].curState);
    }
}

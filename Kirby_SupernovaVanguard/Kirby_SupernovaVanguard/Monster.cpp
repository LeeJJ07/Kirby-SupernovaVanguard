#include "Monster.h"

void Monster::Draw(HDC& hdc)
{
    HBRUSH brush;

    switch (monsterType)
    {
    case RUNNER:
        brush = CreateSolidBrush(RGB(255, 0, 0));
        break;
    case SPEAR:
        brush = CreateSolidBrush(RGB(255, 165, 0));
        break;
    case WINGBUG:
        brush = CreateSolidBrush(RGB(255, 255, 0));
        break;
    case FIREMAN:
        brush = CreateSolidBrush(RGB(0, 255, 0));
        break;
    case LANDMINE:
        brush = CreateSolidBrush(RGB(0, 0, 255));
        break;
    default:
        brush = CreateSolidBrush(RGB(255, 255, 255));
        break;
    }

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    int left = GetPosition().x - 10;
    int right = GetPosition().x + 10;
    int top = GetPosition().y - 10;
    int bottom = GetPosition().y + 10;
    Ellipse(hdc, left, top, right, bottom);

    SelectObject(hdc, oldBrush);
    DeleteObject(brush);

    // �߰� �ڵ� index ����
    // drawIndex = (drawIndex + 1)% (���� �ִϸ��̼��� ũ��)
}

void Monster::ObjectUpdate(TOTALDATA totalData, int i)
{
	SetPosition(totalData.mdata[i].pos);
	GetCollider()->SetOffset(totalData.mdata[i].offset);
    
    SetMonsterType(totalData.mdata[i].monsterType);
    
    if (curState != totalData.mdata[i].curState)
    {
        drawIndex = 0;
        SetMonsterState(totalData.mdata[i].curState);
    }
}

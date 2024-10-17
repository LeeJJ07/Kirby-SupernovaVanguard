#include "Monster.h"

void Monster::Draw(HDC& hdc)
{
    // ���� ������ ���� �귯�� �ڵ�
    HBRUSH brush;

    switch (monsterType)
    {
    case RUNNER:
        brush = CreateSolidBrush(RGB(255, 0, 0)); // ����
        break;
    case SPEAR:
        brush = CreateSolidBrush(RGB(255, 165, 0)); // ��Ȳ
        break;
    case WINGBUG:
        brush = CreateSolidBrush(RGB(255, 255, 0)); // ���
        break;
    case FIREMAN:
        brush = CreateSolidBrush(RGB(0, 255, 0)); // �ʷ�
        break;
    case LANDMINE:
        brush = CreateSolidBrush(RGB(0, 0, 255)); // �Ķ�
        break;
    default:
        brush = CreateSolidBrush(RGB(255, 255, 255)); // �⺻ ���� (���)
        break;
    }

    // �귯�� ����
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

    // �� �׸���
    int left = GetPosition().x - 10;
    int right = GetPosition().x + 10;
    int top = GetPosition().y - 10;
    int bottom = GetPosition().y + 10;
    Ellipse(hdc, left, top, right, bottom);

    // ���� �귯�� ���� �� �귯�� ����
    SelectObject(hdc, oldBrush);
    DeleteObject(brush);

    // �߰� �ڵ� index ����
    // drawIndex = (drawIndex + 1)% (���� �ִϸ��̼��� ũ��)
}

void Monster::ObjectUpdate(TOTALDATA totalData, int i)
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

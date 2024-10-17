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
    if (position.x < 1000)
    {
        int xxxxx = 10000;
        return;
    }
    // ���� ������ ���� �귯�� �ڵ�
    HBRUSH brush;
    brush = CreateSolidBrush(RGB(255, 255, 255)); // �⺻ ���� (���)

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
}
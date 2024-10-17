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
    // 색상 설정을 위한 브러시 핸들
    HBRUSH brush;
    brush = CreateSolidBrush(RGB(255, 255, 255)); // 기본 색상 (흰색)

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
}
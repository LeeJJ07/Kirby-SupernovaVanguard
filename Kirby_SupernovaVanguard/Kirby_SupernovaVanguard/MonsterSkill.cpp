#include "MonsterSkill.h"

void MonsterSkill::ObjectUpdate(TOTALDATA& totalData, int i)
{
	switch (totalData.msdata[i].colliderShape)
	{
	case CIRCLE:
		Circle2D* skillcirclecollider;
		skillcirclecollider = dynamic_cast<Circle2D*>(this->GetCollider());
		skillcirclecollider->SetRadius(totalData.msdata[i].colliderSize);
		break;
	case RECTANGLE:
		Rectangle2D* skillrectanglecollider;
		skillrectanglecollider = dynamic_cast<Rectangle2D*>(this->GetCollider());
		skillrectanglecollider->SetWidth(totalData.msdata[i].colliderSize);
		skillrectanglecollider->SetHeight(totalData.msdata[i].colliderSize2);
		skillrectanglecollider->Setangle(totalData.msdata[i].angle);
		break;
	}
	SetPosition(totalData.msdata[i].position);
	GetCollider()->MovePosition(totalData.msdata[i].colliderPosition);
	Setsize(totalData.msdata[i].size);
	Setsize2(totalData.msdata[i].size2);
}

void MonsterSkill::SetMonsterSkillAni()
{
	Animation* tempAni[2] = { nullptr };
	switch (monsterSkillType)
	{
	case SPEARSKILL:
		tempAni[0] = imageDatas[spearSkill_Attack];
		tempAni[1] = imageDatas[spearSkill_Attack];
		break;
	case FIREMANSKILL:
		tempAni[0] = imageDatas[fireman_Attack];
		tempAni[1] = nullptr;
		break;
	}

	for (int i = 0; i < 2; i++)
	{
		if (tempAni[i] == nullptr)
			continue;
		Animation* temp = new Animation(tempAni[i]->GetCnt(), tempAni[i]->GetSpacingX(),
			tempAni[i]->GetR(), tempAni[i]->GetG(), tempAni[i]->GetB(), tempAni[i]->GetCog(),
			tempAni[i]->GetLengths(), tempAni[i]->Height(), tempAni[i]->GetFilePath());
		temp->Load();

		monsterani.insert({ (EMONSTERSKILLSTATE)i, temp });
	}
}

void MonsterSkill::DrawMonsterSkill(HDC& hdc)
{
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, monsterani[eMonsterSkillState]->GetBitmap());

	monsterani[eMonsterSkillState]->IncreaseIdx();
	int idx = monsterani[eMonsterSkillState]->GetIndex();
	int width = monsterani[eMonsterSkillState]->GetCurWidth();
	int height = monsterani[eMonsterSkillState]->GetHeight() - 1;

	// GetPosition()���� ���� ��ǥ�� �߽����� ���
	int centerX = GetPosition().x;
	int centerY = GetPosition().y;

	int WchangeValue;
	int HchangeValue;

	Circle2D* collider2D = dynamic_cast<Circle2D*>(this->GetCollider());
	if (collider2D != 0)
	{
		WchangeValue = collider2D->GetRadius() * 2.f;
		HchangeValue = collider2D->GetRadius() * 2.f;
	}
	else
	{
		Rectangle2D* collider2D = dynamic_cast<Rectangle2D*>(this->GetCollider());
		WchangeValue = collider2D->GetWidth();
		HchangeValue = collider2D->GetHeight();
	}

	HDC hTempDC = CreateCompatibleDC(hdc);
	HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, WchangeValue, HchangeValue);
	HBITMAP hOldTempBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);

	// ũ�� ��ȯ
	StretchBlt(hTempDC, 0, 0, WchangeValue, HchangeValue, hMemDC, monsterani[eMonsterSkillState]->GetPrevWidth(), 0, width, height, SRCCOPY);

	// ���ο� ��ǥ: �߽��� �������� ��ȯ�� �̹����� �»�� ��ġ�� ����
	int newLeft = centerX - WchangeValue / 2;
	int newTop = centerY - HchangeValue / 2;

	// ����̽� ���ؽ�Ʈ���� ��� �׷��� ��� ����
	SetGraphicsMode(hdc, GM_ADVANCED);

	float radian = this->GetCollider()->Getangle() * 3.14f / 180;

	// ȸ�� ��ȯ ��Ʈ���� ����
	XFORM xForm;
	xForm.eM11 = (FLOAT)cos(radian);
	xForm.eM12 = (FLOAT)sin(radian);
	xForm.eM21 = (FLOAT)-sin(radian);
	xForm.eM22 = (FLOAT)cos(radian);
	// �߽����� �������� ȸ����Ű�� ���� �̵� ����
	xForm.eDx = (FLOAT)(centerX - (centerX * cos(radian) - centerY * sin(radian)));
	xForm.eDy = (FLOAT)(centerY - (centerX * sin(radian) + centerY * cos(radian)));

	// ��ȯ ����
	SetWorldTransform(hdc, &xForm);

	// ���� ���� ����Ͽ� �̹��� �׸���
	TransparentBlt(
		hdc, newLeft, newTop, WchangeValue, HchangeValue, // ������ ��ǥ
		hTempDC, 0, 0, WchangeValue, HchangeValue,
		RGB(monsterani[eMonsterSkillState]->GetR(), monsterani[eMonsterSkillState]->GetG(), monsterani[eMonsterSkillState]->GetB())
	);

	// ��ȯ�� ���� ���·� ����
	ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);

	SelectObject(hTempDC, hOldTempBitmap);
	DeleteDC(hTempDC);
	DeleteObject(hTempBitmap);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}
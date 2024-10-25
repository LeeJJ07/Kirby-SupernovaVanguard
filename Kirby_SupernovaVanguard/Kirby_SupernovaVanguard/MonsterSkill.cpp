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

	// GetPosition()으로 얻은 좌표를 중심으로 계산
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

	// 크기 변환
	StretchBlt(hTempDC, 0, 0, WchangeValue, HchangeValue, hMemDC, monsterani[eMonsterSkillState]->GetPrevWidth(), 0, width, height, SRCCOPY);

	// 새로운 좌표: 중심을 기준으로 변환된 이미지의 좌상단 위치를 구함
	int newLeft = centerX - WchangeValue / 2;
	int newTop = centerY - HchangeValue / 2;

	// 디바이스 컨텍스트에서 고급 그래픽 모드 설정
	SetGraphicsMode(hdc, GM_ADVANCED);

	float radian = this->GetCollider()->Getangle() * 3.14f / 180;

	// 회전 변환 매트릭스 설정
	XFORM xForm;
	xForm.eM11 = (FLOAT)cos(radian);
	xForm.eM12 = (FLOAT)sin(radian);
	xForm.eM21 = (FLOAT)-sin(radian);
	xForm.eM22 = (FLOAT)cos(radian);
	// 중심점을 기준으로 회전시키기 위해 이동 설정
	xForm.eDx = (FLOAT)(centerX - (centerX * cos(radian) - centerY * sin(radian)));
	xForm.eDy = (FLOAT)(centerY - (centerX * sin(radian) + centerY * cos(radian)));

	// 변환 적용
	SetWorldTransform(hdc, &xForm);

	// 투명 블릿을 사용하여 이미지 그리기
	TransparentBlt(
		hdc, newLeft, newTop, WchangeValue, HchangeValue, // 수정된 좌표
		hTempDC, 0, 0, WchangeValue, HchangeValue,
		RGB(monsterani[eMonsterSkillState]->GetR(), monsterani[eMonsterSkillState]->GetG(), monsterani[eMonsterSkillState]->GetB())
	);

	// 변환을 원래 상태로 복원
	ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);

	SelectObject(hTempDC, hOldTempBitmap);
	DeleteDC(hTempDC);
	DeleteObject(hTempBitmap);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}
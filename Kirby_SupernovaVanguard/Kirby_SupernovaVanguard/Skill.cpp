#include "Skill.h"

void Skill::ObjectUpdate(TOTALDATA& totalData, int i)
{
	switch (totalData.sdata[i].colliderShape)
	{
	case CIRCLE:
		Circle2D* skillcirclecollider;
		skillcirclecollider = dynamic_cast<Circle2D*>(this->GetCollider());
		skillcirclecollider->SetRadius(totalData.sdata[i].colliderSize);
		break;
	case RECTANGLE:
		Rectangle2D* skillrectanglecollider;
		skillrectanglecollider = dynamic_cast<Rectangle2D*>(this->GetCollider());
		skillrectanglecollider->SetWidth(totalData.sdata[i].colliderSize);
		skillrectanglecollider->SetHeight(totalData.sdata[i].colliderSize2);
		skillrectanglecollider->Setangle(totalData.sdata[i].angle);
		break;
	}
	SetPosition(totalData.sdata[i].position);
	GetCollider()->MovePosition(totalData.sdata[i].colliderPosition);
	Setsize(totalData.sdata[i].size);
	Setsize2(totalData.sdata[i].size2);
}

void Skill::SetSkillAni()
{
	Animation* tempAni[2] = { nullptr };
	switch (skillType)
	{
	case KIRBYSKILL:
		tempAni[0] = imageDatas[kirbySkill_Attack];
		tempAni[1] = nullptr;
		break;
	case DEDEDESKILL:
		tempAni[0] = imageDatas[dededeSkill_Attack];
		tempAni[1] = nullptr;
		break;
	case METAKNIGHTSKILL:
		tempAni[0] = imageDatas[metaknightSkill_Attack];
		tempAni[1] = nullptr;
		break;
	case MABEROASKILL:
		tempAni[0] = imageDatas[maberoaSkill_Attack];
		tempAni[1] = nullptr;
		break;
	case ELECTRICFIELDSKILL:
		tempAni[0] = imageDatas[electricfieldSkill_Attack];
		tempAni[1] = nullptr;
		break;
	case KUNAISKILL:
		tempAni[0] = imageDatas[kunaiSkill_Attack];
		tempAni[1] = nullptr;
		break;
	case MAGICARROWSKILL:
		tempAni[0] = imageDatas[magicarrowSkill_Attack];
		tempAni[1] = nullptr;
		break;
	case TORNADOSKILL:
		tempAni[0] = imageDatas[tornadoSkill_Attack];
		tempAni[1] = nullptr;
		break;
	case TRUCKSKILL:
		tempAni[0] = imageDatas[truckSkill_Attack];
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

		ani.insert({ (ESKILLSTATE)i, temp });
	}
}

void Skill::DrawSkill(HDC& hdc)
{
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, ani[eSkillState]->GetBitmap());

	ani[eSkillState]->IncreaseIdx();
	int idx = ani[eSkillState]->GetIndex();
	int width = ani[eSkillState]->GetCurWidth();
	int height = ani[eSkillState]->GetHeight() - 1;

	// GetPosition()으로 얻은 좌표를 중심으로 계산
	int centerX = GetPosition().x;
	int centerY = GetPosition().y;

	int WchangeValue;
	int HchangeValue;
	int newLeft;
	int newTop;

	Circle2D* collider2D = dynamic_cast<Circle2D*>(this->GetCollider());
	if (collider2D != 0)
	{
		WchangeValue = collider2D->GetRadius() * 2.f;
		HchangeValue = WchangeValue;

		// 새로운 좌표: 중심을 기준으로 변환된 이미지의 좌상단 위치를 구함
		newLeft = centerX - WchangeValue / 2;
		newTop = centerY - HchangeValue / 2;
	}
	else
	{
		Rectangle2D* collider2D = dynamic_cast<Rectangle2D*>(this->GetCollider());
		WchangeValue = collider2D->GetWidth();
		HchangeValue = collider2D->GetHeight();

		// 새로운 좌표: 중심을 기준으로 변환된 이미지의 좌상단 위치를 구함
		newLeft = centerX - WchangeValue / 2;
		newTop = centerY - HchangeValue / 2;
	}

	HDC hTempDC = CreateCompatibleDC(hdc);
	HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, WchangeValue, HchangeValue);
	HBITMAP hOldTempBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);

	// 크기 변환
	StretchBlt(hTempDC, 0, 0, WchangeValue, HchangeValue, hMemDC, ani[eSkillState]->GetPrevWidth(), 0, width, height, SRCCOPY);

	// 디바이스 컨텍스트에서 고급 그래픽 모드 설정
	SetGraphicsMode(hdc, GM_ADVANCED);

	if (collider2D == 0)
	{
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
	}

	// 투명 블릿을 사용하여 이미지 그리기
	TransparentBlt(
		hdc, newLeft, newTop, WchangeValue, HchangeValue, // 수정된 좌표
		hTempDC, 0, 0, WchangeValue, HchangeValue,
		RGB(ani[eSkillState]->GetR(), ani[eSkillState]->GetG(), ani[eSkillState]->GetB())
	);

	// 변환을 원래 상태로 복원
	ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);

	SelectObject(hTempDC, hOldTempBitmap);
	DeleteDC(hTempDC);
	DeleteObject(hTempBitmap);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}
#include "SkillSelector.h"
#include "Skill.h"

SkillSelector::SkillSelector(POINT pos)
	:m_hBit(0)
	, m_bitInfo{}
{
	this->pos = pos;
	this->width = SKILL_SELECTOR_WIDTH;
	this->height = SKILL_SELECTOR_HEIGHT;
	this->skillIdx = -1;

	this->curSkillLevel = -1;
	this->skillName = L"";
	this->skillDescript = L"";
}

SkillSelector::~SkillSelector()
{
	DeleteObject(m_hBit);
}

void SkillSelector::Load()
{
	m_hBit = (HBITMAP)LoadImage(nullptr, L"Images/Backgrounds/selectSkill.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (m_hBit == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
	}
	else GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}

void SkillSelector::Draw(HDC& hdc, int& cameraLeft, int& cameraTop, HFONT& hLargeFont, HFONT& hSmallFont, std::map<ESKILLTYPE, std::pair< HBITMAP, BITMAP>>& imDatas)
{
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, m_hBit);

    TransparentBlt(
        hdc, pos.x - width / 2 + cameraLeft, pos.y - height / 2 + cameraTop, width, height,
		hMemDC, 0, 0, Width(), Height(),
        RGB(0, 0, 0)
    );

    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

	HDC hSkillDC = CreateCompatibleDC(hdc);
	HBITMAP hOldSkillBitmap = (HBITMAP)SelectObject(hSkillDC, imDatas[(ESKILLTYPE)skillIdx].first);

	// 이미지의 중앙 위치 계산
	int imageX = pos.x + cameraLeft - 45;
	int imageY = pos.y + cameraTop - 45;

	// TransparentBlt로 중앙에 그리기 (필터링 색상 (0, 0, 0))
	TransparentBlt(
		hdc, imageX, imageY, 90, 90,
		hSkillDC, 0, 0, imDatas[(ESKILLTYPE)skillIdx].second.bmWidth, imDatas[(ESKILLTYPE)skillIdx].second.bmHeight,
		RGB(0, 0, 0)
	);

	SelectObject(hSkillDC, hOldSkillBitmap);
	DeleteDC(hSkillDC);

	// 큰 폰트로 스킬 이름 출력
	HFONT hOldFont = (HFONT)SelectObject(hdc, hLargeFont);
	SetTextColor(hdc, RGB(255, 255, 0)); // 노란색
	TextOut(hdc, pos.x + cameraLeft, pos.y + cameraTop + height / 2 - 50, skillName.c_str(), skillName.length());
	SelectObject(hdc, hOldFont);

	// 작은 폰트로 스킬 설명 출력
	hOldFont = (HFONT)SelectObject(hdc, hSmallFont);
	SetTextColor(hdc, RGB(255, 255, 255)); // 흰색
	TextOut(hdc, pos.x + cameraLeft, pos.y + cameraTop + height / 2 - 10, skillDescript.c_str(), skillDescript.length());
	SelectObject(hdc, hOldFont);
}

void SkillSelector::SetInfo(std::pair<short, int>& skillInfo)
{
	skillIdx = skillInfo.first;
	curSkillLevel = skillInfo.second;

	skillDescript = L"해당 스킬이 강화됩니다!";
	switch (skillInfo.first)
	{
	case KIRBYSKILL:
		skillName = L"12년산 입냄새";
		break;
	case DEDEDESKILL:
		skillName = L"두더쥐 잡기 경력 15년";
		break;
	case METAKNIGHTSKILL:
		skillName = L"뽑았으면 무라도 썰자";
		break;
	case MABEROASKILL:
		skillName = L"만세 만세 만만세";
		break;
	case ELECTRICFIELDSKILL:
		skillName = L"어디서 타는 냄새가..";
		if (skillInfo.second == 0)
			skillDescript = L"장판으로 주변 적을 공격합니다.";
		break;
	case KUNAISKILL:
		skillName = L"슈슉 슈슈슉";
		if (skillInfo.second == 0)
			skillDescript = L"마우스 위치로 공격합니다.";
		break;
	case MAGICARROWSKILL:
		skillName = L"슉 슉";
		if (skillInfo.second == 0)
			skillDescript = L"관통하는 화살 공격을 합니다.";
		break;
	case TORNADOSKILL:
		skillName = L"슈우우우웅";
		if (skillInfo.second == 0)
			skillDescript = L"가까운 적을 지속 공격합니다.";
		break;
	case TRUCKSKILL:
		skillName = L"옥지 말고 빵 빵";
		if (skillInfo.second == 0)
			skillDescript = L"트럭이 적을 공격합니다.";
		break;
	default:
		skillName = L"회복 물약";
		skillDescript = L"현재 체력을 회복합니다!";
		return;
	}
	if (skillInfo.second == 0)
		skillName += L"   New!!";
}
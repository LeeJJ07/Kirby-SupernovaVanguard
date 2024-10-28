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
		MessageBox(NULL, _T("�̹��� �ε� ����"), _T("����"), MB_OK);
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

	// �̹����� �߾� ��ġ ���
	int imageX = pos.x + cameraLeft - 45;
	int imageY = pos.y + cameraTop - 45;

	// TransparentBlt�� �߾ӿ� �׸��� (���͸� ���� (0, 0, 0))
	TransparentBlt(
		hdc, imageX, imageY, 90, 90,
		hSkillDC, 0, 0, imDatas[(ESKILLTYPE)skillIdx].second.bmWidth, imDatas[(ESKILLTYPE)skillIdx].second.bmHeight,
		RGB(0, 0, 0)
	);

	SelectObject(hSkillDC, hOldSkillBitmap);
	DeleteDC(hSkillDC);

	// ū ��Ʈ�� ��ų �̸� ���
	HFONT hOldFont = (HFONT)SelectObject(hdc, hLargeFont);
	SetTextColor(hdc, RGB(255, 255, 0)); // �����
	TextOut(hdc, pos.x + cameraLeft, pos.y + cameraTop + height / 2 - 50, skillName.c_str(), skillName.length());
	SelectObject(hdc, hOldFont);

	// ���� ��Ʈ�� ��ų ���� ���
	hOldFont = (HFONT)SelectObject(hdc, hSmallFont);
	SetTextColor(hdc, RGB(255, 255, 255)); // ���
	TextOut(hdc, pos.x + cameraLeft, pos.y + cameraTop + height / 2 - 10, skillDescript.c_str(), skillDescript.length());
	SelectObject(hdc, hOldFont);
}

void SkillSelector::SetInfo(std::pair<short, int>& skillInfo)
{
	skillIdx = skillInfo.first;
	curSkillLevel = skillInfo.second;

	skillDescript = L"�ش� ��ų�� ��ȭ�˴ϴ�!";
	switch (skillInfo.first)
	{
	case KIRBYSKILL:
		skillName = L"12��� �Գ���";
		break;
	case DEDEDESKILL:
		skillName = L"�δ��� ��� ��� 15��";
		break;
	case METAKNIGHTSKILL:
		skillName = L"�̾����� ���� ����";
		break;
	case MABEROASKILL:
		skillName = L"���� ���� ������";
		break;
	case ELECTRICFIELDSKILL:
		skillName = L"��� Ÿ�� ������..";
		if (skillInfo.second == 0)
			skillDescript = L"�������� �ֺ� ���� �����մϴ�.";
		break;
	case KUNAISKILL:
		skillName = L"���� ������";
		if (skillInfo.second == 0)
			skillDescript = L"���콺 ��ġ�� �����մϴ�.";
		break;
	case MAGICARROWSKILL:
		skillName = L"�� ��";
		if (skillInfo.second == 0)
			skillDescript = L"�����ϴ� ȭ�� ������ �մϴ�.";
		break;
	case TORNADOSKILL:
		skillName = L"�������";
		if (skillInfo.second == 0)
			skillDescript = L"����� ���� ���� �����մϴ�.";
		break;
	case TRUCKSKILL:
		skillName = L"���� ���� �� ��";
		if (skillInfo.second == 0)
			skillDescript = L"Ʈ���� ���� �����մϴ�.";
		break;
	default:
		skillName = L"ȸ�� ����";
		skillDescript = L"���� ü���� ȸ���մϴ�!";
		return;
	}
	if (skillInfo.second == 0)
		skillName += L"   New!!";
}
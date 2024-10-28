#include "SkillSelector.h"

SkillSelector::SkillSelector(POINT pos)
	:m_hBit(0)
	, m_bitInfo{}
{
	this->pos = pos;
	this->width = SKILL_SELECTOR_WIDTH;
	this->height = SKILL_SELECTOR_HEIGHT;
	this->skillIdx = -1;
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

void SkillSelector::Draw(HDC& hdc, int& cameraLeft, int& cameraTop)
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
}
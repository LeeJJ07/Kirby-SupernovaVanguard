#pragma once
#include "framework.h"
#include "Skill.h"

class Skill;

class SkillSelector
{
private:
	HBITMAP m_hBit;
	BITMAP m_bitInfo;
	HBITMAP m_hBit_blank;
	BITMAP m_bitInfo_blank;
	HBITMAP m_hBit_full;
	BITMAP m_bitInfo_full;

	POINT pos;
	int width, height;
	int skillIdx;
	int curSkillLevel;

	std::wstring skillName;
	std::wstring skillDescript;

public:
	void Load();
	UINT Width() { return m_bitInfo.bmWidth; }
	UINT Height() { return m_bitInfo.bmHeight; }

	HBITMAP GetBitmap() { return m_hBit; }

	void Draw(HDC& hdc, int& cameraLeft, int& cameraTop, HFONT& hLargeFont, HFONT& hSmallFont, std::map<ESKILLTYPE, std::pair< HBITMAP, BITMAP>>& imDatas);

public:
	SkillSelector(POINT pos);
	~SkillSelector();

	void SetSkillIdx(int idx) { this->skillIdx = idx; }
	void SetSkillLevel(int level) { this->curSkillLevel = level; }

	void SetInfo(std::pair<short, int>& skillInfo);
};


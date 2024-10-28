#pragma once
#include "framework.h"

class SkillSelector
{
private:
	HBITMAP m_hBit;
	BITMAP m_bitInfo;

	POINT pos;
	int width, height;
	int skillIdx;

public:
	void Load();
	UINT Width() { return m_bitInfo.bmWidth; }
	UINT Height() { return m_bitInfo.bmHeight; }

	HBITMAP GetBitmap() { return m_hBit; }

	void Draw(HDC& hdc, int& cameraLeft, int& cameraTop);

public:
	SkillSelector(POINT pos);
	~SkillSelector();
};


#pragma once

#include "framework.h"

class Animation
{
public:
	HBITMAP			m_hBit;
	BITMAP			m_bitInfo;

	int curIdx;
	int increaseIdx;

	int cnt;
	int spacingX;
	int R, G, B;
	std::vector<POINT> m;
	std::vector<int> lengths;
	int height;
	
public:
	void Load(const std::wstring& _strFilePath);

	UINT Width() { return m_bitInfo.bmWidth; }
	UINT Height() { return m_bitInfo.bmHeight; }

	HBITMAP GetBitmap() { return m_hBit; }

	int GetIndex() { return curIdx; }
	void SetIndex(int idx) { curIdx = idx; }
	void IncreaseIdx() 
	{ 
		increaseIdx++;
		if (increaseIdx >= (cnt * 10))
			increaseIdx = 0;
		curIdx = increaseIdx /  10;
	}

	POINT GetCurCog() { return m[curIdx]; }
	int GetCurWidth() { return lengths[curIdx]; }
	int GetPrevWidth()
	{
		int sumWidth = 0;
		for (int i = 0; i < curIdx; i++)
			sumWidth += lengths[i] + spacingX - 1;
		return sumWidth - spacingX;
	}
	int GetHeight() { return height; }
	int GetSpacingX() { return spacingX; }
	int GetR() { return R; }
	int GetG() { return G; }
	int GetB() { return B; }

public: 
	Animation(int cnt, int spacingX, int R, int G, int B, std::vector<POINT>& m, std::vector<int>& lengths, int height);
	~Animation();
};


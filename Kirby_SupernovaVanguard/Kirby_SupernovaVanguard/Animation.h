#pragma once

#include "framework.h"

class Animation
{
private:
	HBITMAP			m_hBit;
	BITMAP			m_bitInfo;
	std::string		filePath;

	int curIdx;
	int increaseIdx;

	int cnt;
	int spacingX;
	int R, G, B;
	std::vector<POINT> m;
	std::vector<int> lengths;
	int height;
	
public:
	void Load();

	UINT Width() { return m_bitInfo.bmWidth; }
	UINT Height() { return m_bitInfo.bmHeight; }

	HBITMAP GetBitmap() { return m_hBit; }

	int GetCnt() { return cnt; }
	int GetIndex() { return curIdx; }
	void SetIndex(int idx) { curIdx = idx; }
	bool IncreaseIdx() 
	{
		increaseIdx++;
		if (increaseIdx >= (cnt * 10))
		{
			increaseIdx = 0;
			return true;
		}
		curIdx = increaseIdx /  10;
		return false;
	}

	POINT GetCurCog() { return m[curIdx]; }
	int GetCurWidth() { return lengths[curIdx]; }
	int GetPrevWidth()
	{
		int sumWidth = 0;
		for (int i = 0; i < curIdx; i++)
			sumWidth += lengths[i] + spacingX;
		return sumWidth;
	}
	int GetHeight() { return height; }
	int GetSpacingX() { return spacingX; }
	int GetR() { return R; }
	int GetG() { return G; }
	int GetB() { return B; }

	std::vector<POINT> GetCog() { return m; }
	std::vector<int> GetLengths() { return lengths; }
	std::string GetFilePath() { return filePath; }

	void SetincreaseIdx(int increaseIdx) { this->increaseIdx = increaseIdx; }

public: 
	Animation(int cnt, int spacingX, int R, int G, int B, std::vector<POINT> m, std::vector<int> lengths, int height, std::string filePath);
	~Animation();
};

extern std::map<ObjectImage, Animation*> imageDatas;
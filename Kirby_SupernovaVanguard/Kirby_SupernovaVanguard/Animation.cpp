

#include "Animation.h"



Animation::Animation(int cnt, int spacingX, int R, int G, int B, std::vector<POINT> m, std::vector<int> lengths, int height, std::string filePath)
	:m_hBit(0)
	, m_bitInfo{}
{
	curIdx = 0;
	increaseIdx = 0;

	this->cnt = cnt;
	this->spacingX = spacingX;
	this->R = R;
	this->G = G;
	this->B = B;
	
	this->m.resize(this->cnt);
	this->lengths.resize(this->cnt);

	for (int i = 0; i < this->cnt; i++)
	{
		this->m[i] = m[i];
		this->lengths[i] = lengths[i];
	}
	this->height = height;
	this->filePath = filePath;
}

Animation::~Animation()
{
	DeleteObject(m_hBit);
}
void Animation::Load()
{
	m_hBit = (HBITMAP)LoadImage(nullptr, std::wstring(filePath.begin(), filePath.end()).c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (m_hBit == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("이미지 로드 에러"), _T("에러"), MB_OK);
	}
	else GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}
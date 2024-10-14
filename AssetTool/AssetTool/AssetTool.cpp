
// AssetTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _CRT_SECURE_NO_WARNINGS 

#include "framework.h"
#include "AssetTool.h"
#include <fstream>
#include <commdlg.h>
#include <string>
#include <vector>
#include <CommCtrl.h>
#include <sstream>
#pragma comment(lib, "msimg32.lib")
//#include <objidl.h>
//#include <gdiplus.h>
//#pragma comment(lib, "Gdiplus.lib")
//using namespace Gdiplus;

using namespace std;

#define MAX_LOADSTRING 100
#define MAX_FILENAME_SIZE 100   
#define TIMER_ANI 1

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;

TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0비트맵 파일\0*.bmp";

RECT rectView;

HDC g_hTextImgDC;
HBITMAP g_hTextImgBMP;
int g_TextImgWidth;
int g_TextImgHeight;
int g_StartImgX;
int g_StartImgY;

bool g_bNowDrow = false;
POINT g_MarginPos = { 0, 0 };
POINT g_CharPos = { 0, 0 };

int startCenterX, startCenterY;
int height, length, spacingX;
int R, G, B;
vector<POINT> pos;
vector<int> widths;

HWND hDlg = NULL;

COLORREF GetPixelColorAtMouseClick();
void DrawArea(HDC hdc);
void DrawAnimation(HDC hdc);
void GameInit(TCHAR filename[]);
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
void SetDlgItemTextFromAnsi(HWND hDlg, int nIDDlgItem, const std::string& str);
string TCHARToString(const TCHAR* tcharStr);
void ParseWidthsFromEditControl(HWND hDlg, int length);

void GameEnd();
void SaveImageDataToFile(const TCHAR* imageName);
void SaveBitmap(HWND hWnd, LPCTSTR filePath);
void SaveBitmap(HWND hWnd, LPCTSTR filePath, RECT rectToSave);

bool isDrawRect;
bool isPlayAnimation;
int curFrame;

bool isWhiteBox;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASSETTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASSETTOOL));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (hDlg == NULL || !IsDialogMessage(hDlg, &msg))  // 다이얼로그 메시지 처리
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASSETTOOL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASSETTOOL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      200, 100, 1500, 1000, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
COLORREF GetPixelColorAtMouseClick()
{
    // 마우스 커서 위치 가져오기
    POINT mousePos;
    if (GetCursorPos(&mousePos))
    {
        // 화면의 DC(디바이스 컨텍스트) 가져오기
        HDC hdcScreen = GetDC(NULL);
        if (hdcScreen)
        {
            // 픽셀 색상 가져오기
            COLORREF color = GetPixel(hdcScreen, mousePos.x, mousePos.y);
            // DC 해제
            ReleaseDC(NULL, hdcScreen);
            return color;
        }
    }
    return RGB(0, 0, 0); // 오류 발생 시 기본값
}

void DrawArea(HDC hdc)
{
    if (widths.empty() || !height || !length || length != widths.size()) 
        return;

    int tempWidth = 0;
    for (int i = 0; i < length; i++)
        tempWidth += widths[i] + spacingX;
    tempWidth -= spacingX;
    tempWidth = tempWidth - widths[0];
    
    startCenterX = (rectView.right - tempWidth) / 2;
    startCenterY = rectView.bottom / 2;

    // 빨간색 테두리 설정
    HPEN hPen;
    if (isWhiteBox)
        hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    else
        hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);  // 투명한 내부를 위한 브러시

    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);  // 기존 펜 저장 및 새 펜 선택
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);  // 기존 브러시 저장 및 새 브러시 선택

    int stackWidth = 0;
    for (int idx = 0; idx < length; idx++)
    {
        int left = startCenterX + spacingX * idx - widths[idx] / 2 + stackWidth;
        int right = startCenterX + spacingX * idx + widths[idx] / 2 + stackWidth;
        int top = startCenterY - height / 2;
        int bottom = startCenterY + height / 2;
        if (idx != length - 1)
            stackWidth += widths[idx] / 2 + widths[idx + 1] / 2;
        // 사각형 그리기
        Rectangle(hdc, left, top, right, bottom);
    }

    // 원래 펜과 브러시 복원
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);

    // 리소스 해제
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
void DrawAnimation(HDC hdc)
{
    if (length != pos.size())
        return;

    int aniStartX = g_StartImgX;
    int aniStartY = g_StartImgY;

    // 메모리 DC 생성
    HDC hMemDC = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, g_hTextImgBMP);

    // 현재 프레임에 따라 비트맵의 시작 위치 계산
    int xStart = curFrame + spacingX;
    for (int i = 0; i < curFrame; i++)
        xStart += widths[i];
    int yStart = 0;

    int diffX = xStart + widths[curFrame] / 2 - pos[curFrame].x;
    int diffY = yStart + height / 2 - pos[curFrame].y;

    // 비트맵을 창에 그리기
    // TransparentBlt를 사용하여 투명한 색상으로 비트맵 그리기
    TransparentBlt(hdc, rectView.right/2 + diffX, 200 + diffY, widths[curFrame], height,
        g_hTextImgDC, aniStartX + xStart, aniStartY + yStart, widths[curFrame], height, RGB(R, G, B));

    // 메모리 DC 정리
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

    // 현재 프레임 업데이트
    curFrame = (curFrame + 1) % length;
}

void GameInit(TCHAR filename[])
{
    HDC hdc = GetDC(g_hWnd);

    g_hTextImgDC = CreateCompatibleDC(hdc);
    g_hTextImgBMP = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(g_hTextImgDC, g_hTextImgBMP);
    ReleaseDC(g_hWnd, hdc);

    BITMAP bm;
    GetObject(g_hTextImgBMP, sizeof(BITMAP), &bm);
    g_TextImgWidth = bm.bmWidth;
    g_TextImgHeight = bm.bmHeight;

}
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
    HDC hDoubleBufferDC = CreateCompatibleDC(hdc);
    HBITMAP hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
    HBITMAP hOldDoubleBufferBitmap = (HBITMAP)SelectObject(hDoubleBufferDC, hDoubleBufferImage);

    // 비트맵을 그리기 위한 메모리 DC 생성
    HDC hMemDC = CreateCompatibleDC(hDoubleBufferDC);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, g_hTextImgBMP);

    // 비트맵 이동 처리
    if (g_bNowDrow)
    {
        POINT MPos;
        GetCursorPos(&MPos);
        ScreenToClient(hWnd, &MPos);

        g_CharPos.x = MPos.x - g_MarginPos.x;
        g_CharPos.y = MPos.y - g_MarginPos.y;
    }

    // g_CharPos를 중심으로 반지름 3인 원 그리기
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 브러시
    SelectObject(hDoubleBufferDC, hBrush);
    int radius = 3;
    Ellipse(hDoubleBufferDC, g_CharPos.x - radius, g_CharPos.y - radius, g_CharPos.x + radius, g_CharPos.y + radius);
    DeleteObject(hBrush); // 브러시 자원 해제

    // 비트맵을 더블 버퍼에 그리기
    if (g_hTextImgDC != NULL)
    {
        BitBlt(hDoubleBufferDC, g_CharPos.x, g_CharPos.y, g_TextImgWidth, g_TextImgHeight, g_hTextImgDC, g_StartImgX, g_StartImgY, SRCCOPY);
    }
    if (pos.size())
    {
        // 텍스트 색상 및 브러시 설정
        SetBkMode(hDoubleBufferDC, TRANSPARENT);
        SetTextColor(hDoubleBufferDC, RGB(255, 255, 255)); // 흰색 텍스트
        HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255)); // 흰색 펜
        SelectObject(hDoubleBufferDC, hPen);

        int offsetY = 0; // Y축 오프셋 초기화

        for (const POINT& p : pos)
        {
            // 점 찍기
            SetPixel(hDoubleBufferDC, p.x, p.y, RGB(255, 255, 255)); // 흰색 점

            // 좌표 텍스트 생성
            string text = "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";

            // 텍스트 위치를 오른쪽 위에 맞추기 위해 좌표 조정
            int textWidth = text.length() * 8; // 대략적인 텍스트 폭 계산
            int textX = rectView.right - textWidth - 5; // 오른쪽 여백 설정
            int textY = 5 + offsetY; // 위쪽 여백 설정

            // 텍스트 출력
            TextOutA(hDoubleBufferDC, textX, textY, text.c_str(), text.length());  // 오른쪽 위에 출력
            offsetY += 20; // 다음 텍스트의 Y축 위치를 아래로 이동
        }

        DeleteObject(hPen);  // 펜 자원 해제
    }

    // 사각형 그리기
    if(isDrawRect)
        DrawArea(hDoubleBufferDC);

    // 애니메이션
    if (isPlayAnimation)
        DrawAnimation(hDoubleBufferDC);

    // 박스의 크기와 위치 설정 (오른쪽 중간에 위치하도록)
    int boxWidth = 100;
    int boxHeight = 100;
    int boxX = rectView.right - boxWidth - 20; // 오른쪽으로 20 픽셀 여백
    int boxY = (rectView.bottom / 2) - (boxHeight / 2); // 세로 중간에 배치

    // 브러시 설정 후 사각형 그리기
    HBRUSH hBoxBrush = CreateSolidBrush(RGB(R, G, B));
    SelectObject(hDoubleBufferDC, hBoxBrush);
    Rectangle(hDoubleBufferDC, boxX, boxY, boxX + boxWidth, boxY + boxHeight);
    DeleteObject(hBoxBrush);

    // 더블 버퍼의 내용을 실제 화면에 복사
    BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hDoubleBufferDC, 0, 0, SRCCOPY);

    // 자원 해제
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);

    SelectObject(hDoubleBufferDC, hOldDoubleBufferBitmap);
    DeleteObject(hDoubleBufferImage);  // 자원 해제
    DeleteDC(hDoubleBufferDC);
}


void GameEnd()
{
    if (g_hTextImgBMP != NULL)
    {
        DeleteObject(g_hTextImgBMP);
        g_hTextImgBMP = NULL;
    }
    if (g_hTextImgDC != NULL)
    {
        DeleteDC(g_hTextImgDC);
        g_hTextImgDC = NULL;
    }
}

void SaveImageDataToFile(const TCHAR* imageName)
{
    // 텍스트 파일 열기
    ofstream outFile("imageDatas.txt", std::ios::app); // append 모드로 열기
    if (!outFile)
    {
        MessageBox(NULL, _T("파일을 열 수 없습니다."), _T("오류"), MB_OK);
        return;
    }

    // TCHAR를 std::string으로 변환
    string imageNameStr = TCHARToString(imageName);

    // 이미지 이름과 전역 변수 기록
    outFile << imageNameStr << " "      // 이미지 이름
            << length << " "             // length
            << spacingX << " "          // spacingX
            << R << " "                 // R
            << G << " "                 // G
            << B << " ";                // B

    // pos 벡터의 좌표 저장
    for (const POINT& p : pos)
    {
        outFile << p.x << " " << p.y << " "; // x, y 좌표 저장
    }
    for(const int& i : widths)
        outFile << i << " "; // x, y 좌표 저장
    outFile << height;
    outFile << std::endl; // 줄 바꿈

    // 파일 닫기
    outFile.close();
}

void SaveBitmap(HWND hWnd, LPCTSTR filePath, RECT rectToSave)
{
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hBitmap = NULL;
    BITMAP bitmap;
    BITMAPFILEHEADER bmpFileHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    HANDLE hFile;
    DWORD dwBytesWritten = 0;
    BYTE* lpBitmapData = NULL;

    // 윈도우 DC와 메모리 DC 생성
    hdcWindow = GetDC(hWnd);
    hdcMemDC = CreateCompatibleDC(hdcWindow);

    // 저장할 영역의 크기 계산
    int width = rectToSave.right - rectToSave.left;
    int height = rectToSave.bottom - rectToSave.top;

    // 비트맵 생성
    hBitmap = CreateCompatibleBitmap(hdcWindow, width, height);
    SelectObject(hdcMemDC, hBitmap);

    // 지정된 영역을 메모리 DC로 복사
    BitBlt(hdcMemDC, 0, 0, width, height, hdcWindow, rectToSave.left, rectToSave.top, SRCCOPY);

    // 비트맵 정보 가져오기
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // 비트맵 데이터 저장을 위한 BITMAPINFO 구조체 설정
    BITMAPINFO bmpInfo;
    memset(&bmpInfo, 0, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = width; // 저장할 비트맵의 너비
    bmpInfo.bmiHeader.biHeight = height; // 저장할 비트맵의 높이
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24; // 32비트 비트맵
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    // 비트맵 데이터 크기 계산
    int bitmapDataSize = bitmap.bmWidthBytes * bitmap.bmHeight;
    lpBitmapData = (BYTE*)malloc(bitmapDataSize);

    // 비트맵 데이터를 가져옴
    GetDIBits(hdcMemDC, hBitmap, 0, (UINT)height, lpBitmapData, &bmpInfo, DIB_RGB_COLORS);

    // 비트맵 파일 헤더 설정
    bmpFileHeader.bfType = 0x4D42;  // "BM"
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bitmapDataSize;
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;

    // 비트맵 정보 헤더 설정
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 24;  // 32비트
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = bitmapDataSize;
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    // 파일 열기
    hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(hWnd, _T("파일을 저장할 수 없습니다."), _T("오류"), MB_OK);
        goto Cleanup;
    }

    // 파일에 비트맵 파일 헤더 쓰기
    WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);

    // 파일에 비트맵 정보 헤더 쓰기
    WriteFile(hFile, &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);

    // 파일에 비트맵 데이터 쓰기
    WriteFile(hFile, lpBitmapData, bitmapDataSize, &dwBytesWritten, NULL);

    // 파일 닫기
    CloseHandle(hFile);

    MessageBox(hWnd, _T("비트맵 저장 완료"), _T("성공"), MB_OK);

Cleanup:
    // 메모리 정리
    if (lpBitmapData)
        free(lpBitmapData);
    DeleteObject(hBitmap);
    DeleteDC(hdcMemDC);
    ReleaseDC(hWnd, hdcWindow);
}


void SaveBitmap(HWND hWnd, LPCTSTR filePath)
{
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hBitmap = NULL;
    BITMAP bitmap;
    BITMAPFILEHEADER bmpFileHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    HANDLE hFile;
    DWORD dwBytesWritten = 0;
    BYTE* lpBitmapData = NULL;

    // 윈도우 DC와 메모리 DC 생성
    hdcWindow = GetDC(hWnd);
    hdcMemDC = CreateCompatibleDC(hdcWindow);

    // 클라이언트 영역 크기 가져오기
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    int width = rcClient.right - rcClient.left;
    int height = rcClient.bottom - rcClient.top;

    // 비트맵 생성
    hBitmap = CreateCompatibleBitmap(hdcWindow, width, height);
    SelectObject(hdcMemDC, hBitmap);

    // 윈도우의 내용을 메모리 DC로 복사
    BitBlt(hdcMemDC, 0, 0, width, height, hdcWindow, 0, 0, SRCCOPY);

    // 비트맵 정보 가져오기
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // 비트맵 데이터 저장을 위한 BITMAPINFO 구조체 설정
    BITMAPINFO bmpInfo;
    memset(&bmpInfo, 0, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = bitmap.bmWidth;
    bmpInfo.bmiHeader.biHeight = bitmap.bmHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32; // 32비트 비트맵
    bmpInfo.bmiHeader.biCompression = BI_RGB;

    // 비트맵 데이터 크기 계산
    int bitmapDataSize = bitmap.bmWidthBytes * bitmap.bmHeight;
    lpBitmapData = (BYTE*)malloc(bitmapDataSize);

    // 비트맵 데이터를 가져옴
    GetDIBits(hdcMemDC, hBitmap, 0, (UINT)bitmap.bmHeight, lpBitmapData, &bmpInfo, DIB_RGB_COLORS);

    // 비트맵 파일 헤더 설정
    bmpFileHeader.bfType = 0x4D42;  // "BM"
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + bitmapDataSize;
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;

    // 비트맵 정보 헤더 설정
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = bitmap.bmWidth;
    bmpInfoHeader.biHeight = bitmap.bmHeight;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 32;  // 32비트
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = bitmapDataSize;
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    // 파일 열기
    hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(hWnd, _T("파일을 저장할 수 없습니다."), _T("오류"), MB_OK);
        goto Cleanup;
    }

    // 파일에 비트맵 파일 헤더 쓰기
    WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);

    // 파일에 비트맵 정보 헤더 쓰기
    WriteFile(hFile, &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);

    // 파일에 비트맵 데이터 쓰기
    WriteFile(hFile, lpBitmapData, bitmapDataSize, &dwBytesWritten, NULL);

    // 파일 닫기
    CloseHandle(hFile);

    MessageBox(hWnd, _T("비트맵 저장 완료"), _T("성공"), MB_OK);

Cleanup:
    // 메모리 정리
    if (lpBitmapData)
        free(lpBitmapData);
    DeleteObject(hBitmap);
    DeleteDC(hdcMemDC);
    ReleaseDC(hWnd, hdcWindow);
}

BOOL CALLBACK Dialog1_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OPENFILENAME sfn;
    static HWND targetHwnd = FindWindow(NULL, L"AssetTool");

    TCHAR sFilePathName[MAX_FILENAME_SIZE] = _T("");
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        string strR = to_string(R);
        SetDlgItemTextFromAnsi(hDlg, IDC_EDIT_readR, strR.c_str());

        // G 값을 에디트 컨트롤에 설정
        string strG = to_string(G);
        SetDlgItemTextFromAnsi(hDlg, IDC_EDIT_readG, strG.c_str());

        // B 값을 에디트 컨트롤에 설정
        string strB = to_string(B);
        SetDlgItemTextFromAnsi(hDlg, IDC_EDIT_readB, strB.c_str());
    }
        return TRUE; // 초기화 완료
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDC_BUTTON_check:
        {
            TCHAR buffer[128];  
            GetDlgItemText(hDlg, IDC_EDIT_height, buffer, 128);
            height = _ttoi(buffer);
            GetDlgItemText(hDlg, IDC_EDIT_length, buffer, 128);
            length = _ttoi(buffer);  
            GetDlgItemText(hDlg, IDC_EDIT_spacingX, buffer, 128);
            spacingX = _ttoi(buffer); 

            GetDlgItemText(hDlg, IDC_EDIT_width, buffer, 128);
            ParseWidthsFromEditControl(hDlg, length);

            if (targetHwnd)
                InvalidateRect(targetHwnd, NULL, TRUE);
        }
            break;
        case IDC_BUTTON_cut:
        {
            g_TextImgWidth = 0;
            for (int i = 0; i < length; i++)
                g_TextImgWidth += widths[i] + spacingX;
            g_TextImgWidth -= spacingX;
            g_TextImgHeight = height;

            g_StartImgX = rectView.right / 2 - g_CharPos.x - g_TextImgWidth / 2;
            g_StartImgY = rectView.bottom / 2 - g_CharPos.y - height / 2;

            g_CharPos.x = rectView.right / 2 - g_TextImgWidth / 2;
            g_CharPos.y = rectView.bottom / 2 - g_TextImgHeight / 2;

            if (targetHwnd)
                InvalidateRect(targetHwnd, NULL, TRUE);
        }   
            break;
        case IDC_BUTTON_pickPosAgain:
            pos.clear();
            if (targetHwnd)
                InvalidateRect(targetHwnd, NULL, TRUE);
            break;
        case IDC_BUTTON_rectOn:
            isDrawRect = true;
            if (targetHwnd)
                InvalidateRect(targetHwnd, NULL, TRUE);
            break;
        case IDC_BUTTON_rectOff:
            isDrawRect = false;
            if (targetHwnd)
                InvalidateRect(targetHwnd, NULL, TRUE);
            break;
        case IDC_BUTTON_AniOn:
            curFrame = 0;
            isPlayAnimation = true;
            break;
        case IDC_BUTTON_AniOff:
            isPlayAnimation = false;
            if (targetHwnd)
                InvalidateRect(targetHwnd, NULL, TRUE);
            break;
        case IDC_BUTTON_changeBoxColor:
            isWhiteBox = !isWhiteBox;
            if (targetHwnd)
                InvalidateRect(targetHwnd, NULL, TRUE);
            break;
        case IDC_BUTTON_save:
        {
            memset(&sfn, 0, sizeof(OPENFILENAME));
            sfn.lStructSize = sizeof(OPENFILENAME);
            sfn.hwndOwner = targetHwnd;
            sfn.lpstrFilter = filter;
            sfn.lpstrFile = sFilePathName;
            sfn.nMaxFile = MAX_FILENAME_SIZE;
            sfn.lpstrInitialDir = _T(".");
            if (GetSaveFileName(&sfn) != 0)
            {
                // 사용자에게 파일 저장 여부 확인 메시지
                TCHAR str[MAX_PATH];
                _stprintf_s(str, _T("%s 파일로 저장하겠습니까?"), sfn.lpstrFile);
                MessageBox(targetHwnd, str, _T("저장하기 선택"), MB_OK);

                // 더블 버퍼에 그려진 부분을 비트맵 파일로 저장하는 함수 호출
                int left = startCenterX - widths[0] / 2 + 1;
                int right = startCenterX;
                for (int i = 0; i < length - 1; i++)
                    right += (widths[i] + widths[i + 1]) / 2 + spacingX;
                right += widths[length - 1] / 2;

                int top = startCenterY - height / 2 + 1;
                int bottom = startCenterY + height / 2 - 1 ;
                RECT rectToSave = { left, top, right, bottom }; // 저장할 영역 설정
                SaveBitmap(targetHwnd, sfn.lpstrFile, rectToSave);
                SaveImageDataToFile(sfn.lpstrFile);
            }
        }
            break;
        }
    }
    break;
    case WM_CLOSE:  // 'X' 버튼을 눌렀을 때 처리
        EndDialog(hDlg, 0);  // 다이얼로그를 종료하고 0을 반환
        return TRUE;

    default:
        return FALSE;  // 기본 메시지 처리
    }
    return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int sx = 0;
    static int sy = 0;

    OPENFILENAME ofn;
    TCHAR sFilePathName[MAX_FILENAME_SIZE] = _T("");
    
    switch (message)
    {
    case WM_CREATE:
        isDrawRect = true;
        isPlayAnimation = false;
        isWhiteBox = false;
        GetClientRect(hWnd, &rectView);
        SetTimer(hWnd, TIMER_ANI, 100, NULL);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_OpenFile:
                memset(&ofn, 0, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = filter;
                ofn.lpstrFile = sFilePathName;
                ofn.nMaxFile = MAX_FILENAME_SIZE;
                ofn.lpstrFile[0] = '\0'; // 파일 경로 버퍼 초기화
                ofn.lpstrInitialDir = L"C:\\Users\\inha\\OneDrive\\문서\\Kirby-SupernovaVanguard\\AssetTool\\AssetTool\\Images\\bmp"; // 초기 디렉토리 설정
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

                // OFN_ENABLEHOOK 플래그를 사용하지 않도록 단순화
                if (GetOpenFileName(&ofn) != 0)
                {
                    GameEnd();
                    GameInit(sFilePathName);
                    InvalidateRect(hWnd, NULL, FALSE);
                }

                g_StartImgX = 0;
                g_StartImgY = 0;
                g_CharPos.x = rectView.right / 2 - g_TextImgWidth / 2;
                g_CharPos.y = rectView.bottom / 2 - g_TextImgHeight / 2;

                pos.clear();

                break;
            case ID_MENU_Set:
                hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_Setup), hWnd, Dialog1_Proc);
                ShowWindow(hDlg, SW_SHOW);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_TIMER:
        if (wParam == TIMER_ANI && isPlayAnimation)
        {
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case WM_LBUTTONDOWN:
    {
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);

        if (g_CharPos.x <= sx && sx <= g_CharPos.x + g_TextImgWidth
            && g_CharPos.y <= sy && sy <= g_CharPos.y + g_TextImgHeight)
        {
            g_MarginPos.x = sx - g_CharPos.x;
            g_MarginPos.y = sy - g_CharPos.y;

            g_bNowDrow = true;
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }
    break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            g_CharPos.x--;
            break;
        case VK_RIGHT:
            g_CharPos.x++;
            break;
        case VK_UP:
            g_CharPos.y--;
            break;
        case VK_DOWN:
            g_CharPos.y++;
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_MOUSEMOVE:
    {
        if (g_bNowDrow)
        {
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }
        break;
    case WM_LBUTTONUP:
    {
        g_bNowDrow = false;
    }
    break;
    case WM_RBUTTONDOWN:
    {
        sx = LOWORD(lParam);
        sy = HIWORD(lParam);
        
        sx = g_TextImgWidth / 2 - (rectView.right / 2 - sx);
        sy = g_TextImgHeight / 2 - rectView.bottom / 2 + sy;
        if (pos.size() < length)
        {
            pos.push_back({ sx, sy });
            InvalidateRect(hWnd, NULL, FALSE);
        }
    }
        break;
    case WM_MOUSEWHEEL:
        if ((SHORT)HIWORD(wParam) > 0) //마우스휠을 올릴 경우
        {
            COLORREF color = GetPixelColorAtMouseClick();
            R = GetRValue(color);
            G = GetGValue(color);
            B = GetBValue(color);

            if (hDlg != NULL)
            {
                string strR = to_string(R);
                SetDlgItemTextFromAnsi(hDlg, IDC_EDIT_readR, strR.c_str());

                // G 값을 에디트 컨트롤에 설정
                string strG = to_string(G);
                SetDlgItemTextFromAnsi(hDlg, IDC_EDIT_readG, strG.c_str());

                // B 값을 에디트 컨트롤에 설정
                string strB = to_string(B);
                SetDlgItemTextFromAnsi(hDlg, IDC_EDIT_readB, strB.c_str());
            }
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다..
            DrawBitmapDoubleBuffering(hWnd, hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        GameEnd();
        KillTimer(hWnd, TIMER_ANI);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
void SetDlgItemTextFromAnsi(HWND hDlg, int nIDDlgItem, const std::string& str)
{
    wchar_t wBuffer[256];  // 유니코드 문자열 버퍼
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wBuffer, 256);  // ANSI -> 유니코드 변환
    SetDlgItemText(hDlg, nIDDlgItem, wBuffer);  // 변환된 유니코드 문자열로 텍스트 설정
}

string TCHARToString(const TCHAR* tcharStr)
{
#ifdef UNICODE
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, NULL, 0, NULL, NULL);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, tcharStr, -1, &str[0], size_needed, NULL, NULL);
    return str;
#else
    return std::string(tcharStr);
#endif
}
void ParseWidthsFromEditControl(HWND hDlg, int length)
{
    TCHAR buffer[128];
    GetDlgItemText(hDlg, IDC_EDIT_width, buffer, sizeof(buffer) / sizeof(TCHAR));

    // 문자열을 std::wstring으로 변환
    wstring wstr(buffer);

    // std::wistringstream을 사용하여 공백으로 구분된 정수 파싱
    wistringstream wss(wstr);
    widths.clear(); // 벡터 초기화

    int value;
    while (wss >> value)
    {
        widths.push_back(value);
    }
}


// AssetTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _CRT_SECURE_NO_WARNINGS 

#include "framework.h"
#include "AssetTool.h"
#include <fstream>
#include <commdlg.h>
#include <string>
#include <vector>

using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;

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
int width, height, length, spacingX;
int R, G, B;
vector<POINT> pos;

HWND hDlg = NULL;

COLORREF GetPixelColorAtMouseClick();
void SaveCroppedBitmap(HDC hdc, HWND hWnd, const TCHAR* filename);
void SaveBitmapToFile(HDC hdc, HBITMAP hBitmap, int width, int height, const TCHAR* filename);
void DrawArea(HDC hdc);
void GameInit(TCHAR filename[]);
void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
void SetDlgItemTextFromAnsi(HWND hDlg, int nIDDlgItem, const std::string& str);
void GameEnd();

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
    if (!width || !height || !length) 
        return;

    startCenterX = (rectView.right - (length - 1) * (width + spacingX)) / 2;
    startCenterY = rectView.bottom / 2;

    // 빨간색 테두리 설정
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));  // 빨간색 테두리 펜 생성
    HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);  // 투명한 내부를 위한 브러시

    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);  // 기존 펜 저장 및 새 펜 선택
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);  // 기존 브러시 저장 및 새 브러시 선택

    for (int idx = 0; idx < length; idx++)
    {
        int left = startCenterX + (width + spacingX) * idx - width / 2;
        int right = startCenterX + (width + spacingX) * idx + width / 2;
        int top = startCenterY - height / 2;
        int bottom = startCenterY + height / 2;

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

    if (g_hTextImgBMP != NULL)
    {
        DeleteObject(g_hTextImgBMP);
        g_hTextImgBMP = NULL;   
    }
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
            std::string text = "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";

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


    //사각형 그리기
    DrawArea(hDoubleBufferDC);

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
    if (g_hTextImgDC != NULL)
    {
        DeleteDC(g_hTextImgDC);
        g_hTextImgDC = NULL;
    }
}

void SaveBitmapToFile(HBITMAP hBitmap, const TCHAR* filePath)
{
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp); // 비트맵 정보 가져오기

    // BMP 파일 헤더 크기
    DWORD bmpFileHeaderSize = sizeof(BITMAPFILEHEADER);
    DWORD bmpInfoHeaderSize = sizeof(BITMAPINFOHEADER);
    DWORD bmpSize = bmp.bmWidthBytes * bmp.bmHeight; // 비트맵 데이터 크기

    // BMP 파일 헤더
    BITMAPFILEHEADER bfh;
    bfh.bfType = 0x4D42; // 'BM'
    bfh.bfSize = bmpFileHeaderSize + bmpInfoHeaderSize + bmpSize; // 전체 파일 크기
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = bmpFileHeaderSize + bmpInfoHeaderSize; // 비트맵 데이터 시작 위치

    // BMP 정보 헤더
    BITMAPINFOHEADER bih;
    bih.biSize = bmpInfoHeaderSize;
    bih.biWidth = bmp.bmWidth;
    bih.biHeight = bmp.bmHeight;
    bih.biPlanes = 1;
    bih.biBitCount = 32; // RGBA 형식
    bih.biCompression = BI_RGB;
    bih.biSizeImage = 0; // 비트맵 데이터 크기 (0으로 설정하면 크기를 자동으로 계산)
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    // 파일 열기
    HANDLE hFile = CreateFile(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, L"파일을 열 수 없습니다.", L"오류", MB_OK);
        return;
    }

    DWORD written;

    // BMP 파일 헤더 쓰기
    WriteFile(hFile, &bfh, bmpFileHeaderSize, &written, NULL);
    // BMP 정보 헤더 쓰기
    WriteFile(hFile, &bih, bmpInfoHeaderSize, &written, NULL);

    // 비트맵 데이터 가져오기
    HDC hdc = CreateCompatibleDC(NULL);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hBitmap);

    // 비트맵 데이터를 파일에 쓰기
    BYTE* pBits = new BYTE[bmpSize];
    GetDIBits(hdc, hBitmap, 0, bmp.bmHeight, pBits, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

    // 비트맵 데이터는 32비트로 저장하므로 패딩을 고려해야 합니다.
    for (int i = 0; i < bmp.bmHeight; i++)
    {
        WriteFile(hFile, pBits + (bmp.bmHeight - 1 - i) * bmp.bmWidthBytes, bmp.bmWidthBytes, &written, NULL);
    }

    // 자원 해제
    delete[] pBits;
    SelectObject(hdc, hOldBitmap);
    DeleteDC(hdc);
    CloseHandle(hFile);
}

BOOL CALLBACK Dialog1_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND targetHwnd = FindWindow(NULL, L"AssetTool");
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
            TCHAR buffer[64];  

            GetDlgItemText(hDlg, IDC_EDIT_width, buffer, 64);
            width = _ttoi(buffer); 
            GetDlgItemText(hDlg, IDC_EDIT_height, buffer, 64);
            height = _ttoi(buffer);
            GetDlgItemText(hDlg, IDC_EDIT_length, buffer, 64);
            length = _ttoi(buffer);  
            GetDlgItemText(hDlg, IDC_EDIT_spacingX, buffer, 64);
            spacingX = _ttoi(buffer); 

            if (targetHwnd)
                InvalidateRect(targetHwnd, NULL, TRUE);
        }
            break;
        case IDC_BUTTON_cut:
        {
            g_TextImgWidth = width * length + spacingX * (length - 1);
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
        case IDC_BUTTON_save:
        {
            SaveBitmapToFile(g_hTextImgBMP, L"output.bmp");
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

#define MAX_FILENAME_SIZE 100   
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int sx = 0;
    static int sy = 0;

    OPENFILENAME Ofn;
    TCHAR sFilePathName[MAX_FILENAME_SIZE] = _T("");
    static TCHAR sFilter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0비트맵 파일\0*.bmp";

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case ID_OpenFile:
                memset(&Ofn, 0, sizeof(OPENFILENAME));
                Ofn.lStructSize = sizeof(OPENFILENAME);
                Ofn.hwndOwner = hWnd;
                Ofn.lpstrFilter = sFilter;
                Ofn.lpstrFile = sFilePathName;
                Ofn.nMaxFile = MAX_FILENAME_SIZE;
                Ofn.lpstrFile[0] = '\0'; // 파일 경로 버퍼 초기화
                Ofn.lpstrInitialDir = L"C:\\Users\\inha\\OneDrive\\문서\\Kirby-SupernovaVanguard\\AssetTool\\AssetTool\\Images\\bmp"; // 초기 디렉토리 설정
                Ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

                // OFN_ENABLEHOOK 플래그를 사용하지 않도록 단순화
                if (GetOpenFileName(&Ofn) != 0)
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
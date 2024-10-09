
// AssetTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "AssetTool.h"
#include <fstream>
#include <commdlg.h>

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

bool g_bNowDrow = false;
POINT g_MarginPos = { 0, 0 };
POINT g_CharPos = { 0, 0 };

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
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

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
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
        BitBlt(hDoubleBufferDC, g_CharPos.x, g_CharPos.y, g_TextImgWidth, g_TextImgHeight, g_hTextImgDC, 0, 0, SRCCOPY);
    }

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

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
#define MAX_FILENAME_SIZE 100   
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
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
                }
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
        static int sx = 0;
        static int sy = 0;
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

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

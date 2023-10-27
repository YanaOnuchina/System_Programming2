#ifndef UNICODE 
#define UNICODE 
#endif

#include <Windows.h> 
#include<strsafe.h> 
#include <vector> 
#include <string> 
#include <fstream> 

using namespace std;
HWND hWnd;
int fontHeight = 30;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitDC(HWND hWnd, int windowWidth, int windowHeight);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex;
    MSG msg;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_VREDRAW | CS_HREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
    HBRUSH hBkgrndBrush = CreateSolidBrush(RGB(127, 255, 212));
    wcex.hbrBackground = hBkgrndBrush;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Lab2";
    wcex.hIconSm = NULL;

    if (!RegisterClassEx(&wcex))
    {
        return 0;
    }

    hWnd = CreateWindowEx(0, L"Lab2", L"SP2", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL); 

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int width, height;

    switch (uMsg)
    {
    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        break;
    case WM_PAINT:
        InitDC(hWnd, width, height);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void InitDC(HWND hWnd, int windowWidth, int windowHeight) {
    RECT rect; HDC memDC;
    HBITMAP hBmp, hOldBmp; PAINTSTRUCT ps;
    HDC currDC = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rect);

    memDC = CreateCompatibleDC(currDC); hBmp = CreateCompatibleBitmap(currDC, rect.right - rect.left, rect.bottom - rect.top);
    hOldBmp = (HBITMAP)SelectObject(memDC, hBmp);
    HBRUSH hBkgrndBrush = CreateSolidBrush(RGB(127, 255, 212));
    FillRect(currDC, &rect, hBkgrndBrush);
    DeleteObject(hBkgrndBrush);
    SetBkMode(currDC, TRANSPARENT);
    HFONT hFont = CreateFont(fontHeight, 0, 0, 0, FW_BOLD, false, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, NULL);
    SelectObject(currDC, hFont);
    DeleteDC(memDC);
    EndPaint(hWnd, &ps);
}
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
int fontSize = 30;
const int COL_NUMBER = 8;
const  int ROW_NUMBER = 9;
const int CELL_NUMBER = COL_NUMBER * ROW_NUMBER;
string textPieces[CELL_NUMBER];

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

    ifstream in("C:/Users/Yana/source/repos/SystemProgramming2/x64/Debug/sample1.txt");
    string buffer;
    int i = 0;
    if (in.is_open()) {
        while (!in.eof()) {
            char symbol = in.get();
            if (symbol != ' ')
                buffer += symbol;
            else
            {
                textPieces[i % CELL_NUMBER] += buffer;
                i++;
                buffer = "";
            }

        }
        if (in.eof()) {
            buffer.pop_back();
            textPieces[i % CELL_NUMBER] += buffer;
        }
    }

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

void DrawVerticalLines(HDC hDC, int windowWidth, int windowHeight)
{
    int columnWidth = windowWidth / COL_NUMBER;

    for (int i = 0; i < COL_NUMBER; i++)
    {
        MoveToEx(hDC, i * columnWidth, 0, NULL);
        LineTo(hDC, i * columnWidth, windowHeight);
    }
    MoveToEx(hDC, windowWidth - 1, 0, NULL);
    LineTo(hDC, windowWidth - 1, windowHeight);
}

void DrawHorizontalLines(HDC hDC, int windowWidth, int rowHeight)
{
    MoveToEx(hDC, 0, rowHeight, NULL);
    LineTo(hDC, windowWidth, rowHeight);
}


void DrawTable(HDC currDC, int windowWidth, int windowHeight) {
    int columnWidth = windowWidth / COL_NUMBER;
    DrawHorizontalLines(currDC, windowWidth, 1);
    int i = 0;
    int editInd = 0; int textHeight = 0;
    int fontSize = 20; RECT rect = { 0, 0, 0, windowHeight };
    while (i < CELL_NUMBER) {
        rect.top = textHeight;

        int maxTextHeight = 0;  
        for (int j = 0; (j < COL_NUMBER) && (i < CELL_NUMBER); j++)
        {
            const char* str = textPieces[i].c_str();
            rect.left = j * columnWidth + 3;
            rect.right = (j + 1) * columnWidth;
            int currentTextHeight = DrawTextA(currDC, str, strlen(str), &rect, DT_EDITCONTROL | DT_WORDBREAK);
            if (maxTextHeight < currentTextHeight) {
                maxTextHeight = currentTextHeight;
            }
            i++;
        }

        textHeight += maxTextHeight;
        DrawHorizontalLines(currDC, windowWidth, textHeight);
    }
    DrawVerticalLines(currDC, windowWidth, textHeight);
}

void InitDC(HWND hWnd, int windowWidth, int windowHeight) {
    RECT rect; HDC memDC;
    HBITMAP hBmp, hOldBmp; PAINTSTRUCT ps;
    HDC currDC = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rect);

    memDC = CreateCompatibleDC(currDC); 
    hBmp = CreateCompatibleBitmap(currDC, rect.right - rect.left, rect.bottom - rect.top);
    hOldBmp = (HBITMAP)SelectObject(memDC, hBmp);
    HBRUSH hBkgrndBrush = CreateSolidBrush(RGB(127, 255, 212));
    FillRect(currDC, &rect, hBkgrndBrush);
    DeleteObject(hBkgrndBrush);
    SetBkMode(currDC, TRANSPARENT);
    DrawTable(currDC, rect.right - rect.left, rect.bottom - rect.top);
    HFONT hFont = CreateFont(fontSize, 0, 0, 0, FW_BOLD, false, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, NULL);
    SelectObject(currDC, hFont);
    DeleteDC(memDC);
    EndPaint(hWnd, &ps);
}
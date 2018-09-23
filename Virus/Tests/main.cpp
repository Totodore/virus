#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <SFML/Graphics.hpp> 
#include <SFML/System.hpp>
#include <SFML/Config.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

int main()
{
    char *lpBits;
    TcpSocket sock;
    cout << "listen" << endl;
    Socket::Status status;
    TcpListener listenFd;
    Packet data;

    listenFd.listen(4443);
    if (status != Socket::Done)
        return 0;
    listenFd.accept(sock);

    int x = 800, y = 600;
    sock.receive(data);
    data >> lpBits;
    Image txt;
    txt.loadFromMemory((void*)lpBits, sizeof(lpBits));
    txt.saveToFile("/home/theodore/");
    return 0;

}
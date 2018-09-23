#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
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
#include "Keyboard.hpp"

using namespace std;
using namespace sf;

TcpSocket sock;
RenderWindow window;

void sendFunc();
void recvFunc();

int main()
{
    cout << "listen" << endl;
    Socket::Status status;
    TcpListener listenFd;
    Packet data;

    listenFd.listen(6545);
    if (status != Socket::Done)
        return 0;
    listenFd.accept(sock);
    cout << "thread started" << endl;
    // int x = 800, y = 600;
    // sock.receive(data);
    // data >> x;
    // data.clear();
    // sock.receive(data);
    // data >> y;
    window.create(VideoMode(1920, 1080), "ScreenLead", Style::Close);

    Thread sendThread(&sendFunc);
    // Thread recvThread(&recvFunc);
    cout << "you can enter commands like -shutdown; -active-mouse; -unactive-mouse; -unactive-keyboard; -active-keyboard." << endl;
    //recvThread.launch();
    sendThread.launch();

    sendThread.wait();
    sendThread.terminate();
    listenFd.close();
    sock.disconnect();
    return 0;
}

void sendFunc()
{
    Packet packSend;
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

    Vector2i pos;
    Vector2i lastPos;
    while(1)
    {
        if (KeyboardLog::getKeyPressed() != 0x0)
        {
            blockSend.key = KeyboardLog::getKeyPressed();          
        }
        if (Mouse::getPosition() != lastPos)
        {
            blockSend.rightClick = 0;
            blockSend.leftClick = 0;
            pos = Mouse::getPosition();
            blockSend.x = pos.x;
            blockSend.y = pos.y;
            cout << blockSend.x << " " << blockSend.y << endl;
            lastPos.x = pos.x; lastPos.y = pos.y;
        }
        if (Mouse::isButtonPressed(Mouse::Button::Left) && blockSend.leftClick == 0)
        {
            blockSend.leftClick = true;
            cout << "left button pressed" << endl;
        }
        if (Mouse::isButtonPressed(Mouse::Button::Right) && blockSend.rightClick == 0)
        {
            blockSend.rightClick = true;
            cout << "right button pressed" << endl;
        }
        packSend.clear();
        packSend.append((char *)&blockSend, sizeof(blockSend));
        sock.send(packSend);
    }

}

void recvFunc()
{
    Packet data;
    Image ImgScreen;
    Texture Tscreen;
    Sprite screen;
    while(1)
    {
        data.clear();
        sock.receive(data);
        window.clear();
        if (!ImgScreen.loadFromMemory(data.getData(), data.getDataSize()))
            return;
        Tscreen.loadFromImage(ImgScreen);
        screen.setTexture(Tscreen);
        window.draw(screen);
    }
}
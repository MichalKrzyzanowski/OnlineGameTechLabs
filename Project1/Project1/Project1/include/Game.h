#include <SDL.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <SDL_net.h>
#include "Circle.h"

class Game
{
public:

    Game();
    ~Game();
    void run();

private:

    void processEvents(SDL_Event e);
    void update();
    void render();

    void communication();
    void cleanUp();

    bool m_gameIsRunning;

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    Circle m_player1;
    Circle m_player2;

    // network
    char m_host[50] = "127.0.0.1";
    int m_port{ 1234 };
    UDPsocket m_sock;
    IPaddress m_serverAddress;
    UDPpacket* m_packet;
};
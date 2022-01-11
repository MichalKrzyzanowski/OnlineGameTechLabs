#include <SDL.h>
#include <iostream>
#include <string>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT 8888   

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

    void cleanUp();

    bool m_gameIsRunning;

    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

};
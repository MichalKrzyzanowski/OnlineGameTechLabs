#ifndef GAME_H
#define GAME_H

#include <vector>
#include <stdio.h>
#include <SDL.h>
#include <sstream>

class Client;

class Game
{
private:
	bool m_gameIsRunning{};
	int m_speed{ 2 };

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	std::vector<SDL_Rect> m_shapes{};
	Client* m_client;

private:
	void update();
	void processEvents(SDL_Event e);
	void render(SDL_Renderer* renderer);

public:
	Game(Client* client);
	~Game();
	void run();
	std::vector<std::string> splitString(const std::string& s, char delimiter);
};

#include "Client.h"

#endif // !GAME_H
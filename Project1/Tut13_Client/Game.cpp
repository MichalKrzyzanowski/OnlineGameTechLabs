#include "Game.h"

Game::Game(Client* client) :
	m_client{ client }
{
	SDL_Init(SDL_INIT_VIDEO);

	m_window = SDL_CreateWindow("Online Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	m_shapes.push_back(SDL_Rect{ 100, 100, 20, 20 });
	m_shapes.push_back(SDL_Rect{ 200, 100, 20, 20 });
	m_shapes.push_back(SDL_Rect{ 300, 100, 20, 20 });

	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

Game::~Game()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	m_renderer = nullptr;

	SDL_Quit();
}

void Game::run()
{
	m_gameIsRunning = true;
	SDL_Event e{};

	while (m_gameIsRunning)
	{
		processEvents(e);
		update();
		render(m_renderer);
	}
}

void Game::update()
{
	printf("%d\n", m_client->m_id);

	std::string positionData = std::to_string(m_shapes[m_client->m_id].x) +
		"," + std::to_string(m_shapes[m_client->m_id].y);

	m_client->SendString(positionData); //Send string to server

	std::vector<std::string> data = splitString(m_client->m_message, ',');

	if (data.size() == 3)
	{
		if (atoi(data[0].c_str()) == 0)
		{
			m_shapes[0].x = atoi(data[1].c_str());
			m_shapes[0].y = atoi(data[2].c_str());
		}
		else if (atoi(data[0].c_str()) == 1)
		{
			m_shapes[1].x = atoi(data[1].c_str());
			m_shapes[1].y = atoi(data[2].c_str());
		}
		else if (atoi(data[0].c_str()) == 2)
		{
			m_shapes[2].x = atoi(data[1].c_str());
			m_shapes[2].y = atoi(data[2].c_str());
		}
	}
}

void Game::processEvents(SDL_Event e)
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			m_gameIsRunning = false;
		}

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
		{
			m_gameIsRunning = false;
		}

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w)
		{
			m_shapes[0].y -= m_speed;
		}

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s)
		{
			m_shapes[0].y += m_speed;
		}

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a)
		{
			m_shapes[0].x -= m_speed;
		}

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d)
		{
			m_shapes[0].x += m_speed;
		}
	}
}

void Game::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_renderer);

	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0x00, 0xFF, 0xFF);

	for (auto const& shape : m_shapes)
	{
		SDL_RenderDrawRect(m_renderer, &shape);
	}

	SDL_RenderPresent(m_renderer);
}

std::vector<std::string> Game::splitString(const std::string& s, char delimiter)
{
	std::vector<std::string> splits;
	std::string split;
	std::istringstream ss(s);
	while (std::getline(ss, split, delimiter))
	{
		splits.push_back(split);
	}
	return splits;
}

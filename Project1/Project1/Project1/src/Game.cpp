#include "../include/Game.h"

Game::Game() :
	m_player1{ Vector2f{100.0f, 100.0f}, 32 },
	m_player2{ Vector2f{150.0f, 150.0f}, 32 },
	m_gameIsRunning{ false }
{
	SDL_Init(SDL_INIT_VIDEO);

	m_window = SDL_CreateWindow("SDL game loop", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	if (SDLNet_Init() < 0)
	{
		printf("SDLNet failed to init: %s\n", SDLNet_GetError());
	}

	if (!(m_sock = SDLNet_UDP_Open(0)))
	{
		printf("SDLNet failed to open socket: %s\n", SDLNet_GetError());
	}

	if (SDLNet_ResolveHost(&m_serverAddress, m_host, m_port))
	{
		printf("SDLNet SDLNet_ResolveHost(%s %d): %s\n", m_host, m_port, SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (!(m_packet = SDLNet_AllocPacket(512)))
	{
		printf("SDLNet falied to allocate packet: %s\n", SDLNet_GetError());
	}
}

Game::~Game()
{
	cleanUp();
}

void Game::run()
{
	m_gameIsRunning = true;
	SDL_Event e{};

	while (m_gameIsRunning)
	{
		communication();
		//processEvents(e);
		//update();
		//render();
	}

	cleanUp();
}

void Game::processEvents(SDL_Event e)
{
	// simple event loop
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			m_gameIsRunning = false;
		}

		// checks if the escape key is pressed down
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
		{
			m_gameIsRunning = false;
		}

		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d)
		{
			m_player1.move(2, 0);
		}
	}

}

void Game::update()
{
}

void Game::render()
{
	SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_renderer);

	m_player1.render(m_renderer);
	m_player2.render(m_renderer);

	SDL_RenderPresent(m_renderer);
}

void Game::communication()
{
	printf("Fill the buffer\n");
	scanf_s("%d", m_packet->data);

	m_packet->address.host = m_serverAddress.host;
	m_packet->address.port = m_serverAddress.port;
	m_packet->len = strlen((char*)m_packet->data) + 1;
	SDLNet_UDP_Send(m_sock, -1, m_packet);

	if (!strcmp((char*)m_packet->data, "quit")) m_gameIsRunning = false;
}

void Game::cleanUp()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	m_window = nullptr;
	m_renderer = nullptr;

	SDLNet_FreePacket(m_packet);
	SDLNet_Quit();

	SDL_Quit();
}
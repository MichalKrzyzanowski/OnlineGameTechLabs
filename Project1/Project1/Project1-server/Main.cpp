#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_net.h>

int main(int argc, char** argv)
{
	UDPsocket socketDescriptor;
	UDPpacket* packet;
	bool successfullSetup{ true };

	if (SDLNet_Init() < 0)
	{
		printf("SDLNet init failed: %s\n", SDLNet_GetError());
		successfullSetup = false;
	}

	if (!(socketDescriptor = SDLNet_UDP_Open(2000)))
	{
		printf("SDLNet failed to open socket: %s\n", SDLNet_GetError());
		successfullSetup = false;
	}

	if (!(packet = SDLNet_AllocPacket(512)))
	{
		printf("SDLNet falied to allocate packet: %s\n", SDLNet_GetError());
		successfullSetup = false;
	}

	if (successfullSetup)
	{
		printf("Server is running\n");
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	bool quit{ false };

	while (!quit)
	{
		if (SDLNet_UDP_Recv(socketDescriptor, packet))
		{
			printf("Packet incoming\n");
			printf("\tChan: %d\n", packet->channel);
			printf("\tData: %s\n", (char*)packet->data);
			printf("\Maxlen: %d\n", packet->maxlen);
			printf("\Status: %d\n", packet->status);
			printf("\Address: %x %x\n", packet->address.host, packet->address.port);

			if (!strcmp((char*)packet->data, "quit")) quit = true;
		}
	}

	SDLNet_FreePacket(packet);
	SDLNet_Quit();

	return 0;
}
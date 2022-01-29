/*
Tutorial 13 Client Solution
Author: Pindrought
Date: 02/26/2017
*/
#include "Client.h"
#include <iostream>
#include "Game.h"

int main(int argc, char** argv)
{
	Client myClient("127.0.0.1",1111); //Create client to connect to server 127.0.0.1 [localhost] on port 1111
	if (!myClient.Connect()) //If client fails to connect...
	{
		std::cout << "Failed to connect to server..." << std::endl;
		system("pause");
		return -1;
	}

	Game game{ &myClient };

	game.run();

	//std::string userinput; //holds the user's chat message
	//while (true)
	//{
	//	std::getline(std::cin,userinput); //Get line if user presses enter and fill the buffer
	//	myClient.SendString(userinput); //Send string to server
	//}

	std::cin.get();
	return 0;
}


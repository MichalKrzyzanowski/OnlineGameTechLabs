/*
Tutorial 13 Server Solution
Author: Pindrought
Date: 02/26/2017
*/
#include "Server.h"

int main()
{
	Server MyServer(1111, false); //Create server on port 1111, false=do not loopback to localhost (others can connect)
	while (true)
	{
		MyServer.ListenForNewConnection(); //Accept new connection (if someones trying to connect)
	}
	return 0;
}
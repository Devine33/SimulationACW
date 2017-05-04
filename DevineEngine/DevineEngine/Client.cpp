#include "Client.h"
#include <iostream>


Client::Client()
{
}


Client::~Client()
{
}

void Client::InitializeSockets() const
{
	WORD wVersionRequested = MAKEWORD(2, 0);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData)) {
		std::cerr << "Socket initialisation failed" << std::endl;
	}
}

void Client::CreateSocketData() const
{

}

bool Client::CheckSocket(SOCKET s)
{
	if (s == INVALID_SOCKET)
	{
		std::cerr << "Create socket failed" << std::endl;
		return false;
	}
	return true;
}

bool Client::CheckBindAndListen(SOCKET S)
{
	if (bind(S, reinterpret_cast<sockaddr *>(&Peer), sizeof(Peer)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with " << WSAGetLastError() << std::endl;
		/*return false;*/
	}
	else if (listen(S, 5) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

void Client::Run()
{
		InitializeSockets();
	
		SOCKET S = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
		SetupPeer();
		//checks for invalid
		CheckSocket(S);
	
		if (connect(S, reinterpret_cast<sockaddr *>(&Peer), sizeof(Peer)) == SOCKET_ERROR)
		{
			SetupHost();
			SOCKET Listener = socket(AF_INET, SOCK_STREAM, 0);
			//Checks for Failing Bind and listen
			if (CheckBindAndListen(Listener) == false)
			{
				std::cout << "BIND OR LISTEN FAILED";
			}
			else
			{
				std::cout << "Succesful So Far";
				SOCKET Transfer = socket(AF_INET, SOCK_STREAM, 0);
			}
		}
}

void Client::SetupHost()
{
		Host.sin_family = AF_INET;
		Host.sin_port = htons(9171);	// port 9171
		Host.sin_addr.S_un.S_addr = ADDR_ANY;
}

void Client::SetupPeer()
{
		Peer.sin_family = AF_INET;
		Peer.sin_port = htons(9171);	// port 9171
		Peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
}

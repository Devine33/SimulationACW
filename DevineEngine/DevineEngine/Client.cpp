#include "Client.h"
#include <iostream>
#include <thread>

//startup sockets
//check if host exists
//if not become host
//
//
//Send/receive
Client::Client(): m_Host(false), S(0), Host_Socket(0), Peer_Socket(0), m_PeerConnected(false)
{

	//HANDLE process = GetCurrentProcess(); 
	//DWORD_PTR processAffinityMask = 0x04; 
	//// processor 3
	//SetProcessAffinityMask(process, processAffinityMask);
	//SetThreadAffinityMask(GetCurrentThread(), 4);

	WORD wVersionRequested = MAKEWORD(2, 0);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData)) {
		std::cerr << "Socket initialisation failed" << std::endl;
	}

	S = socket(AF_INET, SOCK_STREAM, 0);
	SetupPeer();


	//checks for invalid
	if (S == INVALID_SOCKET)
	{
		std::cout << "socket failed";
	}
	else if (connect(S, reinterpret_cast<sockaddr*>(&Peer), sizeof Peer) == SOCKET_ERROR)
	{
		SetupHost();
		m_Host = true;
		std::cout << "your now the host \n";
		Host_Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (bind(Host_Socket,(sockaddr *)(&Host), sizeof(Host)) == SOCKET_ERROR)
		{
			std::cout << " \n Bind Failed \n" << WSAGetLastError();
		}
		else
		{
			std::cout << "Bind Successful\n";
		}
	}
	else
	{
		//your not the host your a peer
		std::cout << "YOURE THE PEER";
		m_Host == false;
		//setup peer socket
		Peer_Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (Peer_Socket == INVALID_SOCKET) 
		{
			std::cout << "Create socket failed" << std::endl;
		}
		m_PeerConnected = true;
	}

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

bool Client::CheckSocket(SOCKET s)
{
	if (s == INVALID_SOCKET)
	{
		std::cerr << "Create socket failed" << std::endl;
		return false;
	}
	return true;
}

bool Client::CheckBind(SOCKET S)
{
	if (bind(S, reinterpret_cast<sockaddr *>(&Peer), sizeof(Peer)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

bool Client::CheckListener(SOCKET S)
{
	if (listen(S,5) == SOCKET_ERROR)
	{
		std::cout << "Listen Failed" << WSAGetLastError() << std::endl;
		return false;
	}
	else
	{
		return true;
	}


}

bool Client::CheckReceive(SOCKET S)
{
	/*auto Length = (int)strlen(&buffer);
	if (recv(S,&buffer,Length,0) == SOCKET_ERROR)
	{
		std::cout << "Receive Failed" << WSAGetLastError() << std::endl;
		return false;
	}*/
	return true;
}

void Client::Run()
{
	char buffer[300];
	auto Length = (int)strlen(buffer);

	if (m_Host == true)
	{

		//host has been bound now listen
		if (listen(Host_Socket,5) == SOCKET_ERROR)
		{
			std::cout << "Listen Failed" << WSAGetLastError();
		}
		else
		{
			//then accept
			SOCKET s1 = accept(Host_Socket, NULL, NULL);
			if (s1 == INVALID_SOCKET)
			{
				std::cout << "Accept Failed With" << WSAGetLastError() << std::endl;
			}
			else
			{
				m_PeerConnected = true;
				const std::string buffer;
				buffer == "Hello";
				auto Length = (int)strlen(buffer.c_str());
				std::cout << "Accepted" << std::endl;
				std::cout << ReturnPeerConnected();
				/*send(s1, buffer.c_str(), Length,0);*/
			}			
		}
	}
	//if your not the host 
	else
	{

	}
	std::thread Rec(Receive, this);
	while (true)
	{
		m_Timer.SetFixedTimeStep(true);
		m_Timer.SetTargetElapsedSeconds(1.0f / 60.f);	
		m_Timer.Tick([&]()
		{
			Send();
		});		
	}
}

void Client::SetupHost()
{
		Host.sin_family = AF_INET;
		Host.sin_port = htons(9171);	// port 9171
		Host.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
}

void Client::SetupPeer()
{
		Peer.sin_family = AF_INET;
		Peer.sin_port = htons(9171);	// port 9171
		Peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
}

bool Client::ReturnPeerConnected()
{
	return m_PeerConnected;
}

void Client::Send()
{
	char buffer[300];
	char b[4];
	auto Length = (int)strlen(buffer);
	SOCKET Type;
	
	if (m_Host == true)
		Type = Host_Socket;
	else
		Type = Peer_Socket;
	send(Type, buffer, Length, 0);
	//std::cout << "Sending " << std::endl;
}

void Client::Receive(Client* c)
{
	while (true)
	{
		char buffer[300];
		auto Length = (int)strlen(buffer);
		
		SOCKET Type;

		if (c->m_Host == true)
			Type = c->Host_Socket;
		else
			Type = c->Peer_Socket;
		recv(Type, buffer, Length, 0);

		/*std::cout << "Received " << Length << std::endl;*/
	}
	
}


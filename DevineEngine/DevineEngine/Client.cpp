#include "Client.h"
#include <iostream>
#include <thread>


Client::Client(): m_Host(false), S(0), Listener(0), Transfer(0)
{
	HANDLE process = GetCurrentProcess(); 
	DWORD_PTR processAffinityMask = 0x04; 
	// processor 3
	SetProcessAffinityMask(process, processAffinityMask);
	SetThreadAffinityMask(GetCurrentThread(), 4);

	InitializeSockets();

	S = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SetupPeer();
	//checks for invalid
	CheckSocket(S);

	if (connect(S, reinterpret_cast<sockaddr *>(&Peer), sizeof Peer) == SOCKET_ERROR)
	{
		SetupHost();
		m_Host = true;
		std::cout << "your now the host";
		if (CheckBind(Listener) == false)
		{
			std::cout << "BIND OR LISTEN FAILED";
		}
		
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
		/*return false;*/
	}
	return true;
}

void Client::Run()
{
		//server should listen and receive
		if (connect(S, reinterpret_cast<sockaddr *>(&Peer), sizeof(Peer)) == SOCKET_ERROR)
		{
			SetupHost();
			m_Host = true;
			std::cout << "your now the host";
			Listener = socket(AF_INET, SOCK_STREAM, 0);
			//Checks for Failing Bind and listen
			//just bind
			if (CheckBind(Listener) == false)
			{
				std::cout << "BIND OR LISTEN FAILED";
			}
			else
			{
				std::cout << "Succesful So Far";
				Transfer = accept(Listener, nullptr, nullptr);
				char buffer[4096];
				if(CheckSocket(Transfer) == false)
				{
					
				}
				else if(recv(Transfer, buffer, 1, 0) == SOCKET_ERROR)
				{
					
				}
				else
				{
					std::cout << "Message=" << buffer << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Connected";
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

void Client::Send(SOCKET S)
{

	std::thread Tsend([&]()
	{
		
	});
}

void Client::Receive(SOCKET S)
{
	std::thread Treceive([&]()
	{
	/*	S = recv();*/
	});
}

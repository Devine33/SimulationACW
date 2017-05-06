#include "Networking.h"
#include <iostream>
#include <thread>



Networking::Networking(GameEngine *G): m_Host(false), m_PeerConnected(false), Host_Socket(0), Peer_Socket(0),Game(G)
{
	//HANDLE process = GetCurrentProcess(); 
	//DWORD_PTR processAffinityMask = 0x02; 
	////// processor 2
	//SetProcessAffinityMask(process, processAffinityMask);
	//SetThreadAffinityMask(GetCurrentThread(), 4);

	InitializeSockets();

	Peer.sin_family = AF_INET;
	Peer.sin_port = htons(9171);	// port 9171
	Peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	Peer_Socket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (Peer_Socket == INVALID_SOCKET)
	{
		std::cout << "socket failed";
	}

	else if (connect(Peer_Socket, reinterpret_cast<sockaddr*>(&Peer), sizeof Peer) == SOCKET_ERROR)
	{
		Host.sin_family = AF_INET;
		Host.sin_port = htons(9171);	// port 9171
		Host.sin_addr.S_un.S_addr = htonl(ADDR_ANY);

		m_Host = true;
		std::cout << " \n your now the host \n";
		Host_Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (::bind(Host_Socket,(sockaddr *)(&Host), sizeof(Host)) == SOCKET_ERROR)
		{
			std::cout << " \n Bind Failed \n" << WSAGetLastError();
			assert(0);
		}
		else
		{
			std::cout << "Bind Successful\n";
		}
	}
	else
	{		
		
		if (Peer_Socket == INVALID_SOCKET) 
		{
			std::cout << "Create socket failed" << WSAGetLastError() <<  std::endl;
			assert(0);
		}	
		m_PeerConnected = true;
		CreateGravityWell();
	}
}

Networking::~Networking()
{
}

void Networking::InitializeSockets() const
{
	WORD wVersionRequested = MAKEWORD(2, 0);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData)) {
		std::cerr << "Socket initialisation failed" << std::endl;
	}
}

bool Networking::CheckSocket(SOCKET s)
{
	if (s == INVALID_SOCKET)
	{
		std::cerr << "Create socket failed" << std::endl;
		return false;
	}
	return true;
}

bool Networking::CheckBind(SOCKET S)
{
	if (::bind(S, reinterpret_cast<sockaddr *>(&Peer), sizeof(Peer)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed with " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

bool Networking::CheckListener(SOCKET S)
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

bool Networking::CheckReceive(SOCKET S)
{
	/*auto Length = (int)strlen(&buffer);
	if (recv(S,&buffer,Length,0) == SOCKET_ERROR)
	{
		std::cout << "Receive Failed" << WSAGetLastError() << std::endl;
		return false;
	}*/
	return true;
}

void Networking::GetMessageLength()
{
}

void Networking::Run()
{
	if (m_Host == true)
	{
		//host has been bound now listen
		if (listen(Host_Socket,5) == SOCKET_ERROR)
		{
			std::cout << "Listen Failed" << WSAGetLastError();
			assert(0);
		}
		else
		{
			//then accept
			s1 = accept(Host_Socket, nullptr, nullptr);
			if (s1 == INVALID_SOCKET)
			{
				std::cout << "Accept Failed With" << WSAGetLastError() << std::endl;
				assert(0);
			}
			else
			{		
				CreateGravityWell();
			}			
		}
	}
	else
	{	
		CreateGravityWell();
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

void Networking::SetupHost()
{
		Host.sin_family = AF_INET;
		Host.sin_port = htons(9171);	// port 9171
		Host.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
		//htonl(ADDR_ANY);
}

void Networking::SetupPeer()
{
	Peer.sin_family = AF_INET;
	Peer.sin_port = htons(9171);	// port 9171
	Peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
}

bool Networking::ReturnPeerConnected()
{
	return m_PeerConnected;
}

void Networking::CreateGravityWell()
{
	if (m_PeerConnected == true)
	{
		
	}
	else
	{
		Game->CreateGravityWell();
	}
}

void Networking::Send()
{

	string ToSend = std::to_string(Well_Pos) + '-' + to_string(Game->GetWellPosition().x) + ',' + to_string(Game->GetWellPosition().y) + "," + to_string(Game->GetWellPosition().z);
	ToSend = to_string(ToSend.size()) + "_" + ToSend;
	//std::cout << ToSend;	
	//L  T X Y Z
	//30_1-0,3,5
	SOCKET Type;
	auto Length = ToSend.size();
	if (m_Host == true)
	{
		Type = s1;
	}
	else
	{
		Type = Peer_Socket;
	}

	send(Type, ToSend.c_str(), Length, 0);
	//std::cout << "Sending " << ToSend << std::endl;
}

void Networking::Receive(Networking* c)
{
	while (true)
	{
		SOCKET Type;
		//get message length
		char TempBuffer[34];
		//string length
		string lengthBuffer;
		//length counter
		int length = 0;
		//bool for length received is full message
		bool LengthReceived = false;
		lengthBuffer = "";

		if (c->m_Host == true)
		{
			Type = c->s1;
		}
		else
			//Peer_Socet = 0;
		{
			Type = c->Peer_Socket;
		}
		//while its not fully received
		
		while (LengthReceived == false)
		{
			int num = recv(Type,TempBuffer, 1, 0);
			if (num == -1)
			{
				//cerr << "received failed" << WSAGetLastError();
			
				//std::cout << WSAGetLastError();
				assert(0);
			}
			//1-0,3,5
			 if (TempBuffer[0] == '_')
			{
				//when it is received
				LengthReceived = true;
				length = std::stoi(lengthBuffer);
			}
			else
			{
				lengthBuffer = lengthBuffer + TempBuffer[0];
			}				
		}
		char buffer[300];	
		int rl = recv(Type, buffer, length, 0);	
		
		auto delimiter ='-';
		//store chars in here
		string token;
		//1-0,3,5				
			int pos = 0;
		for (auto element : buffer)
		{
			//std::cout << element;
			//FIRST ONE IS ALWAYS GONNA BE THE ONE BEFORE THE DELIMITER
			token = element;
			int identifer = stoi(token);
			//if element != delimiter token = token + element;
			std::cout << token;
			if (element == delimiter)
			{
				
			}
		}

	/*	if (last == Well_Pos)
		{
			
		}*/
			c->Game->UpdateWellPositions();
	}
}



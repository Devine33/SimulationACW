#include "Networking.h"
#include <iostream>
#include <thread>



Networking::Networking(GameEngine *G): m_Host(false), m_PeerConnected(false), Host_Socket(0), Peer_Socket(0), s1(0), Game(G)
{
	//HANDLE process = GetCurrentProcess(); 
	//DWORD_PTR processAffinityMask = 0x02; 
	////// processor 2
	//SetProcessAffinityMask(process, processAffinityMask);
	//SetThreadAffinityMask(GetCurrentThread(), 4);

	WORD wVersionRequested = MAKEWORD(2, 0);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData))
	{
		std::cerr << "Socket initialisation failed" << std::endl;
	}

	Peer.sin_family = AF_INET;
	Peer.sin_port = htons(9171); // port 9171
	Peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	Peer_Socket = socket(AF_INET, SOCK_STREAM, 0);

	if (Peer_Socket == INVALID_SOCKET)
	{
		std::cout << "socket failed";
	}

	else if (connect(Peer_Socket, reinterpret_cast<sockaddr*>(&Peer), sizeof Peer) == SOCKET_ERROR)
	{
		Host.sin_family = AF_INET;
		Host.sin_port = htons(9171); // port 9171
		Host.sin_addr.S_un.S_addr = htonl(ADDR_ANY);

		m_Host = true;
		std::cout << " \n your now the host \n";
		Host_Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (::bind(Host_Socket, (sockaddr *)(&Host), sizeof(Host)) == SOCKET_ERROR)
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
	}
}

Networking::~Networking()
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
				if (Peer_Socket == INVALID_SOCKET)
				{
					std::cout << "Create socket failed" << WSAGetLastError() << std::endl;
					assert(0);
				}
				Game->CreateGravityWell();
			}			
		}
	}
	else
	{			
		Game->CreateGravityWell();
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

void Networking::Send()
{
	SOCKET Type;
	string ToSend = std::to_string(Well_Pos) + '!' + to_string(Game->GetWellPosition().x) + ',' + to_string(Game->GetWellPosition().y) + ',' + to_string(Game->GetWellPosition().z);
	ToSend = to_string(ToSend.size()) + "_" + ToSend;

	string Ball_To_Send = "";
	for (auto element : Game->GetSphere())
	{
		if (m_Host == true)
		{
			Type = s1;
		}
		else
		{
			Type = Peer_Socket;
		}

		Ball_To_Send = std::to_string(Ball_Pos) + '!' + to_string(element->ReturnObjectID()) + ','  + to_string(element->GetPos().x) + ',' + to_string(element->GetPos().y) + ',' + to_string(element->GetPos().z);
		Ball_To_Send = to_string(Ball_To_Send.size()) + "_" + Ball_To_Send;
		auto BALLLENGTH = Ball_To_Send.size();
		std::cout << Ball_To_Send;
		send(Type, Ball_To_Send.c_str(), BALLLENGTH, 0);
	}
	/*string Ball_To_Send = std::to_string(Ball_Pos) + '!' + to_string(Game->GetSphere()->ReturnObjectID());
	Ball_To_Send = to_string(Ball_To_Send.size()) + "_" + Ball_To_Send;*/

	//L  T X Y Z
	//30_1-0,3,5

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
		{
			Type = c->Peer_Socket;
		}
		//while its not fully received
		
		while (LengthReceived == false)
		{
			int num = recv(Type,TempBuffer, 1, 0);
			if (num == -1)
			{
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
		char buffer[30];	
		int rl = recv(Type, buffer, length, 0);
		if (rl == -1)
		{
			std::cout <<  "RECEIVE ERROR" << WSAGetLastError();
		}
		auto delimiter ='!';
		auto nDelim = ',';
		int count = 0;

		//store chars in here
		string token;
		string token2;
		string token3;

		string identifer;
		float Xnum;
		float Ynum;
		float Znum;
		//1!0,3,5				
		//gets 1
			//std::cout << buffer[0];
			for (auto i = 0; i < length; i++)
			{
				count++;
				if (buffer[i] == delimiter)
				{
					
					//std::cout << identifer;
					count++;
					break;
				}
				identifer += buffer[i];
				int num = stoi(identifer);
				count++;
				switch (num)
				{
					case 1:
					{
							for (auto i = count; i < length; i++)
							{
								if (buffer[i] == nDelim)
								{								
									//std::cout << token << endl;
									Xnum = stof(token);
									count++;
									break;
								}
								token += buffer[i];
								count++;
							}
							for (auto i = count; i < length; i++)
							{
								if (buffer[i] == nDelim)
								{
									//std::cout << token2 << endl;
									Ynum = stof(token2);
									count++;
									break;
								}
								token2 += buffer[i];
								count++;
							}
							for (auto i = count; i < length; i++)
							{
								if (buffer[i] == nDelim)
								{
									/*std::cout << token3 << endl;*/
									count++;
									break;
								}
								token3 += buffer[i];
								count++;
							}
							Znum = stof(token3);
							//switch on token for action
							c->Game->UpdateWellPositions(Xnum, Ynum, Znum);
							break;
					}
					case 2:
						{
						if (buffer[i] == nDelim)
						{
							//std::cout << token << endl;
							Xnum = stof(token);
							count++;
							break;
						}
							token += buffer[i];
							count++;
						}
					//parse sphere message
				}

			}

		
	}
}



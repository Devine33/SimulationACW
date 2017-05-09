#include "Networking.h"
#include <iostream>
#include <thread>
#include <WS2tcpip.h>
Networking::Networking(GameEngine *G) : m_Host(false), m_PeerConnected(false), Host_Socket(0), Peer_Socket(0), s1(0), Game(G), Network_Freq(60), Port(0)
{
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000010);

	std::thread Init(&Networking::InitializeSockets,this);
	Init.join();

	Peer.sin_family = AF_INET;
	std::string ip_from_config = "150.237.93.28";
	inet_pton(AF_INET, "150.237.93.28", &Peer.sin_addr);
	Peer.sin_port = htons(9171);
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

void Networking::InitializeSockets() const
{
	HANDLE process = GetCurrentProcess();
	//// processor 3
	SetProcessAffinityMask(process, 0b00000010);

	WORD wVersionRequested = MAKEWORD(2, 0);

	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData))
	{
		std::cerr << "Socket initialisation failed" << std::endl;
	}
}

void Networking::Run()
{
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000010);

	if (m_Host == true)
	{
		//host has been bound now listen
		if (listen(Host_Socket, 5) == SOCKET_ERROR)
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
				//assert(0);
			}
			else
			{
				if (Peer_Socket == INVALID_SOCKET)
				{
					std::cout << "Create socket failed" << WSAGetLastError() << std::endl;
					//assert(0);
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
	//Receive_t();
	while (true)
	{
		m_Timer.SetFixedTimeStep(true);
												//replace with variable and put in anttweak
												//replace with 60 if errors occur
		m_Timer.SetTargetElapsedSeconds(1.0f / Network_Freq);
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

//union byte32
//{
//	int iVal;
//	float fVal;
//	char bytes[sizeof(int)];
//};
//
//struct spheretonetwork
//{
//	int id;
//	float x;
//	float y;
//	float z;
//	bool owned;
//};

void Networking::Send()
{
	SOCKET Type;
	string ToSend = std::to_string(Well_Pos) + '!' + to_string(Game->GetWellPosition().x) + ',' + to_string(Game->GetWellPosition().y) + ',' + to_string(Game->GetWellPosition().z);
	ToSend = to_string(ToSend.size()) + "_" + ToSend;

	string Ball_To_Send = "";
	for (auto element : Game->GetSphere())
	{
		element->ReturnOwnership();
		if (element->ReturnOwnership() == false)
		{
			Ball_To_Send = to_string(Ball_Pos) + '!' + to_string(element->GetNewPos().x) + ',' + to_string(element->GetNewPos().y) + ',' + to_string(element->GetNewPos().z) + ',' + to_string(element->GetNewVel().x) + ',' + to_string(element->GetNewVel().y) + ',' + to_string(element->GetNewVel().z) + ',' + to_string(element->GetMass());
			//Ball_To_Send = to_string(Ball_To_Send.size()) + "_" + Ball_To_Send;
			//auto BALLLENGTH = Ball_To_Send.size();
			//std::cout << Ball_To_Send;
			ToSend += to_string(Ball_To_Send.size()) + "_" + Ball_To_Send;
			//std::cout << Ball_To_Send;
		}		
	}
	//int ballsCounted = 0;
	//for (auto element : Game->ReturnGravityWells())
	//{
	//	if (element->GetID() == 1 )
	//	{
	//		for (auto balls : Game->GetSphere())
	//		{
	//			if (balls->ReturnOwnership() ==true)
	//			{
	//				Ball_To_Send = to_string(Ball_Pos) + '!' + to_string(element->GetNewPos().x) + ',' + to_string(element->GetNewPos().y) + ',' + to_string(element->GetNewPos().z);
	//				ToSend += to_string(Ball_To_Send.size()) + "_" + Ball_To_Send;
	//				ballsCounted++;
	//			}
	//		}
	//	}
	//}
	//std::cout << ballsCounted;
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

void Networking::CloseSockets()
{
	closesocket(Host_Socket);
	closesocket(Peer_Socket);
	closesocket(s1);
	WSACleanup();
}

void Networking::SetIPAddress(string IP_s)
{
	IP = IP_s;
}

void Networking::SetupPort(int p)
{
	Port = p;
}

int Networking::GetPort()
{
	return Port;
}

string Networking::GetIP()
{
 	return IP;
}

void Networking::SetNetworkFrequency(float Freq)
{
	Network_Freq = Freq;
}

void Networking::Receive(Networking* c)
{
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000010);

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
			int num = recv(Type, TempBuffer, 1, 0);
			if (num == -1)
			{
				//assert(0);
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
		char buffer[1584];
		int rl = recv(Type, buffer, length, 0);
		if (rl == -1)
		{
			std::cout << "RECEIVE ERROR" << WSAGetLastError();
		}
		auto delimiter = '!';
		auto nDelim = ',';
		int count = 0;

		//store chars in here
		string token;
		string token2;
		string token3;

		string token4;
		string token5;
		string token6;

		string token7;
		string token8;
		string token9;
		string token10;
	

		string identifer;
		float Xnum;
		float Ynum;
		float Znum;

		float X_SpherePos;
		float X_SphereVel;
		float Y_SpherePos;
		float Y_SphereVel;
		float Z_SpherePos;
		float Z_SphereVel;
		float mass;
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
				
				//SEND OVER VISIBILITY DATA WHICH IS IKE IT BEING NOTED TO OTHER PEER
				for (auto i = count; i < length; i++)
				{
					if (buffer[i] == nDelim)
					{
						//std::cout << token << endl;
						X_SpherePos = stof(token4);
						count++;
						break;
					}
					token4 += buffer[i];
					count++;
				}
				for (auto i = count; i < length; i++)
				{
					if (buffer[i] == nDelim)
					{
						//std::cout << token << endl;
						Y_SpherePos = stof(token5);
						count++;
						break;
					}
					token5 += buffer[i];
					count++;
				}

				for (auto i = count; i < length; i++)
				{
					if (buffer[i] == nDelim)
					{
						//std::cout << token << endl;
						Z_SpherePos = stof(token6);
						count++;
						break;
					}
					token6 += buffer[i];
					count++;
				}
				for (auto i = count; i < length; i++)
				{
					if (buffer[i] == nDelim)
					{
						//std::cout << token << endl;
						Z_SpherePos = stof(token6);
						count++;
						break;
					}
					token6 += buffer[i];
					count++;
				}
				for (auto i = count; i < length; i++)
				{
					if (buffer[i] == nDelim)
					{
						//std::cout << token << endl;
						X_SphereVel = stof(token7);
						count++;
						break;
					}
					token7 += buffer[i];
					count++;
				}
				for (auto i = count; i < length; i++)
				{
					if (buffer[i] == nDelim)
					{
						//std::cout << token << endl;
						Y_SphereVel = stof(token8);
						count++;
						break;
					}
					token8 += buffer[i];
					count++;
				}
				for (auto i = count; i < length; i++)
				{
					if (buffer[i] == nDelim)
					{
						//std::cout << token << endl;
						Z_SphereVel = stof(token9);
						count++;
						break;
					}
					token9 += buffer[i];
					count++;
				}
				for (auto i = count; i < length; i++)
				{
					if (buffer[i] == nDelim)
					{
						//std::cout << token << endl;
						mass = stof(token10);
						count++;
						break;
					}
					token10 += buffer[i];
					count++;
				}
					//switch on token for action
				c->Game->BallsWithinRegion(X_SpherePos,Y_SpherePos,Z_SpherePos,X_SphereVel,Y_SphereVel,Z_SphereVel,mass);
				//c->Game->NotifySpheres(X_SpherePos, Y_SpherePos, Z_SpherePos);
				break;
			}
			//parse sphere message
			}

		}


	}
}




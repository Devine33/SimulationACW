#pragma once
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
// TCP CLIENT
class Client
{
public:
	Client();
	~Client();
	void InitializeSockets() const;
	void CreateSocketData() const;
	bool CheckSocket(SOCKET s);
	bool CheckBindAndListen(SOCKET S);
	void Run();
	void SetupHost();
	void SetupPeer();
private:
	//if its true your the host
	bool m_Host;
	////Addresses for Host & Peers
	sockaddr_in Peer;
	sockaddr_in Host;
};


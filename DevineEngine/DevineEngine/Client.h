#pragma once
#include <winsock2.h>
#include "Timer\StepTime.h"
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
	bool CheckBind(SOCKET S);
	bool CheckListener(SOCKET S);
	bool CheckReceive(SOCKET S);
	void Run();
	void SetupHost();
	void SetupPeer();
	bool ReturnPeerConnected();
	 void Send();
	static void Receive(Client* c);
private:
	//if its true your the host
	bool m_Host;
	////Addresses for Host & Peers
	sockaddr_in Peer;
	sockaddr_in Host;
	bool m_PeerConnected;
	//
	SOCKET S;
	SOCKET Host_Socket;
	SOCKET Peer_Socket;
	DX::StepTime m_Timer;
};


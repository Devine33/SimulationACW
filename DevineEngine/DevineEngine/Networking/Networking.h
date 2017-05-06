#pragma once
#include <winsock2.h>
#include "../Timer/StepTime.h"
#include "../GameEngine/GameEngine.h"
#include <sstream>
#pragma comment(lib, "ws2_32.lib")

// TCP CLIENT
class Networking
{
public:
	Networking(GameEngine *G);
	~Networking();
	void InitializeSockets() const;
	void CreateSocketData() const;
	bool CheckSocket(SOCKET s);
	bool CheckBind(SOCKET S);
	bool CheckListener(SOCKET S);
	bool CheckReceive(SOCKET S);
	Vector3 GetVector();
	void GetMessageLength();
	void Run();
	void SetupHost();
	void SetupPeer();
	bool ReturnPeerConnected();
	void CreateGravityWell();
	void Send();
	static void Receive(Networking* c);

private:
	const int Well_Pos = 1;
	//if its true your the host
	bool m_Host;
	////Addresses for Host & Peers
	sockaddr_in Peer;
	sockaddr_in Host;
	bool m_PeerConnected;
	//If you not a peer your a host
	SOCKET Host_Socket;
	//if your not the host your a peer
	SOCKET Peer_Socket;
	SOCKET s1;
	DX::StepTime m_Timer;
	GameEngine* Game;
};


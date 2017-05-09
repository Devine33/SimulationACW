#include "SystemEngine.h"
#include <thread>


SystemEngine::SystemEngine(): G(nullptr), N(nullptr), m_NetworkFrequency(0), Port(0)
{
}


SystemEngine::~SystemEngine()
{
	delete G;
	//delete N;
}

void SystemEngine::RunSystem(int cmd,WNDPROC wnd)
{
	G = new GameEngine;
	G->InitializeComponents(cmd, wnd);
	this->GetIP();
	this->GetPort();
	std::thread L(&SystemEngine::StartNetworking,this);
	
	G->GameLoop();
	N->CloseSockets();
	L.join();
}

void SystemEngine::CreateGravityWell()
{
	G->CreateGravityWell();
}

void SystemEngine::StartNetworking(SystemEngine* S)
{
	//Networking
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000010);

		S->N = new Networking(S->G);
		S->N->SetIPAddress(S->returnIP());
		S->N->SetupPort(S->ReturnPort());
		S->m_NetworkFrequency = 60;
		TwAddVarRW(S->G->ReturnAntTweak(), "Network_Freq", TW_TYPE_FLOAT,&S->m_NetworkFrequency,"");
		S->N->Run();

}

void SystemEngine::IncreaseNetworkFrequency()
{
	m_NetworkFrequency += 0.1;
}

void SystemEngine::DecreaseNetworkFrequency()
{
	m_NetworkFrequency -= 0.1;
}

string SystemEngine::returnIP()
{
	return IP;
}

int SystemEngine::ReturnPort()
{
	return Port;
}

void SystemEngine::GetIP()
{
	IP = G->GetIP();
}

void SystemEngine::GetPort()
{
	Port = G->ReturnPort();
}

GameEngine* SystemEngine::GetEngine()
{
	return G;
}

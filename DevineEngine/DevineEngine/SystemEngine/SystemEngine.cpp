#include "SystemEngine.h"
#include <thread>


SystemEngine::SystemEngine(): G(nullptr), N(nullptr)
{
}


SystemEngine::~SystemEngine()
{
}

void SystemEngine::RunSystem(int cmd,WNDPROC wnd)
{
	G = new GameEngine;
	G->InitializeComponents(cmd, wnd);
	std::thread L(StartNetworking,this);
	G->GameLoop();
}

void SystemEngine::CreateGravityWell()
{
	G->CreateGravityWell();
}

void SystemEngine::StartNetworking(SystemEngine* S)
{
		S->N = new Networking(S->G);
		S->N->Run();
}

GameEngine* SystemEngine::GetEngine()
{
	return G;
}

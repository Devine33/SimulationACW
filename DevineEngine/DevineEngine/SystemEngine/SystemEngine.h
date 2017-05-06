#pragma once
#include "../Networking/Networking.h"
#include "../GameEngine/GameEngine.h"
class SystemEngine
{
public:
	SystemEngine();
	~SystemEngine();
	//Will Start Game Engine Loop ETC..
	void RunSystem(int,WNDPROC);
	void CreateGravityWell();
	static void StartNetworking(SystemEngine*);
	GameEngine* GetEngine();
private:
	GameEngine* G;
	Networking* N;
};


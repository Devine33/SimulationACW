#include <windows.h>
#include "Time.h"
#include <memory>
#include "GameEngine/GameEngine.h"
#include "Tracer/Trace.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	/*t.StartQuery();*/
	std::unique_ptr<GameEngine> G(new GameEngine);

	/*X->StartWindowing(nCmdShow);*/
	G->InitializeComponents(nCmdShow);
	G->RenderLoop();
	/*t.EndQuery();*/

	/*t.Elapsed();*/
	/*X->BeginScene();*/
	TRACE(L"WinMain \n");
	//std::unique_ptr<Windowing> wind(new Windowing);	
	//wind->WindowCreation(nCmdShow);
	//wind->Run();
}


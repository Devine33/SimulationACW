#include <windows.h>  
#include "DirectX.h"
#include "Time.h"
#include <memory>
#include "Trace.hpp"
#include "GameEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{


	/*t.StartQuery();*/
	std::unique_ptr<GameEngine> G(new GameEngine);

	/*X->StartWindowing(nCmdShow);*/
	G->InitializeComponents(nCmdShow);
	G->RenderLoop();
	/*t.EndQuery();*/

	/*t.GetElapsed();*/
	/*X->BeginScene();*/
	TRACE(L"WinMain \n");
	//std::unique_ptr<Windowing> wind(new Windowing);	
	//wind->WindowCreation(nCmdShow);
	//wind->Run();
}


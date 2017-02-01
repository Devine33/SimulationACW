#include <windows.h>  
#include "DirectX.h"
#include "Time.h"
#include <memory>
#include "Trace.hpp"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	Time t;

	t.StartQuery();
	std::unique_ptr<DirectX> X(new DirectX);

	X->StartWindowing(nCmdShow);


	t.EndQuery();

	/*t.GetElapsed();*/
	X->BeginScene();
	TRACE(L"WinMain \n");
	//std::unique_ptr<Windowing> wind(new Windowing);	
	//wind->WindowCreation(nCmdShow);
	//wind->Run();
}


#include <memory>
#include "GameEngine/GameEngine.h"
#include "Tracer/Trace.hpp"
#include <iostream>
#include <Mouse.h>
#include "../packages/AntTweakBar.1.16.3/build/native/include/AntTweakBar.h"
std::unique_ptr<GameEngine> G(new GameEngine);
int i = 0;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (TwEventWin(hwnd, msg, wParam, lParam))
		return 0;
	//TRACE(L"Windowing::WndProc \n");
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		//TRACE(L"Windowing::WndProc::WM_KEYDOWN \n");
		std::cout << wParam << "\n";
		if (wParam == 27)
		{
			PostQuitMessage(0);
		}
		if (wParam == 82)
		{
			//TRACE(L"System Reset\n");
			
		}
		if (wParam == 87)
		{
			G->MoveUp();
		}
		if(wParam == 38)
		{
			//zoom in
		}
		if(wParam == 83)
		{
			//move down
			G->MoveDown();
		}
		if (wParam == 65)
		{
			G->MoveLeft();
		}
		if (wParam == 68)
		{
			G->MoveRight();
		}
		if (wParam == 80)
		{
			G->PauseSimulation();
		}
		//make use of input handler here send over WPARAM MATCH MVOEMENTS TO CAMERA AND CALL METHOD
		break;
	case WM_ACTIVATEAPP:
		Mouse::ProcessMessage(msg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
		Mouse::ProcessMessage(msg, wParam, lParam);
		/*std::cout << "Moving \n";*/
		G->GetMousePosition();
		G->MoveGravityWell();
		break;
	case WM_LBUTTONDOWN:
		Mouse::ProcessMessage(msg, wParam, lParam);
		G->ApplyAttractor();
		//std::cout << "Applying Attractor /n";	
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		Mouse::ProcessMessage(msg, wParam, lParam);
		G->ApplyRetractor();
		break;
	case WM_RBUTTONUP:
		break;
	case WM_MBUTTONDOWN:
		break;
	case WM_MBUTTONUP:
		break;
	case WM_MOUSEWHEEL:
		break;
	case WM_XBUTTONDOWN:
		break;
	case WM_XBUTTONUP:
		break;
	case WM_MOUSEHOVER:	
		break;
	default:
		return  DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	G->InitializeComponents(nCmdShow,WndProc);
}


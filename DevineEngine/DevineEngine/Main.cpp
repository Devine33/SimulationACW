#include <memory>
#include <iostream>
#include "../packages/AntTweakBar.1.16.3/build/native/include/AntTweakBar.h"
#include "SystemEngine/SystemEngine.h"

std::shared_ptr<SystemEngine> S(new SystemEngine);
int i = 0;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (TwEventWin(hwnd, msg, wParam, lParam))
		return 0;
	/*TRACE(L"Windowing::WndProc \n");*/
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		//if time is left use input handler + command pattern for inputs + mouse
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
			S->GetEngine()->MoveUp();
		}
		if(wParam == 38)
		{
			//zoom in
		}
		if(wParam == 83)
		{
			//move down
			S->GetEngine()->MoveDown();
		}
		if (wParam == 65)
		{
			S->GetEngine()->MoveLeft();
		}
		if (wParam == 68)
		{
			S->GetEngine()->MoveRight();
		}
		if (wParam == 80)
		{
			S->GetEngine()->PauseSimulation();
		}
		if (wParam == 77)
		{
			S->GetEngine()->MoveGravityWellUp();
		}
		if (wParam == 78)
		{
			S->GetEngine()->MoveGravityWellDown();
		}
		if (wParam == 79)
		{
			S->GetEngine()->IncreasePhysics();
		}
		if (wParam == 76)
		{
			S->GetEngine()->DecreasePhysics();
		}
		if (wParam == 85)
		{
			S->IncreaseNetworkFrequency();
		}
		if (wParam == 74)
		{
			S->DecreaseNetworkFrequency();
		}
		if (wParam == 38)
		{
			S->GetEngine()->MoveForward();
		}
		if (wParam ==40)
		{
			S->GetEngine()->MoveBackward();
		}
		if (wParam ==73)
		{
			S->GetEngine()->IncreaseGraphics();
		}
		if (wParam == 75)
		{
			S->GetEngine()->DecreaseGraphics();
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
		S->GetEngine()->GetMousePosition();
		S->GetEngine()->MoveGravityWell();
		break;
	case WM_LBUTTONDOWN:
		Mouse::ProcessMessage(msg, wParam, lParam);
		S->GetEngine()->ApplyAttractor();
		//std::cout << "Applying Attractor /n";	
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		Mouse::ProcessMessage(msg, wParam, lParam);
		S->GetEngine()->ApplyRetractor();		
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
	/*G->InitializeComponents(nCmdShow,WndProc);*/
	S->RunSystem(nCmdShow, WndProc);
}


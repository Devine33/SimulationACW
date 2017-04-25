#include <windows.h>
#include "Time.h"
#include <memory>
#include "GameEngine/GameEngine.h"
#include "Tracer/Trace.hpp"
#include <iostream>
#include <Mouse.h>
std::unique_ptr<GameEngine> G(new GameEngine);
std::unique_ptr<Mouse> mouse(new Mouse);
//mouse = std::make_unique<Mouse>();
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	XMFLOAT2 mousePosInPixels(float(mouse.get()->GetState().x), float(mouse.get()->GetState().y));
	mouse->SetWindow(hwnd);
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
			TRACE(L"System Reset\n");
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
		//make use of input handler here send over WPARAM MATCH MVOEMENTS TO CAMERA AND CALL METHOD
		break;
	case WM_ACTIVATEAPP:
	case WM_INPUT:
	case WM_MOUSEMOVE:
		/*std::cout << "Moving \n";*/
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		/*Mouse::ProcessMessage(msg, wParam, lParam);
		std::cout << mousePosInPixels.x << "\n";
		std::cout << mousePosInPixels.y << "\n";*/
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


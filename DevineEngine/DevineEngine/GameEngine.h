#pragma once
#include <Windows.h>
#include "DirectX.h"
class GameEngine
{
public:
	GameEngine();
	~GameEngine();
	/*void Initialize();
	void Run();*/
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


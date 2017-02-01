#pragma once
#include <Windows.h>
#include "Windowing.h"
#include "DirectX.h"
#include "Time.h"
class GameEngine
{
public:
	GameEngine();
	~GameEngine();
	void InitializeComponents(int cmd);
	void RenderLoop();
	void Draw();
private:
	bool m_Done;
#pragma region Command Pattern (Inputs)
	Input* m_Input;
	KeyDownCommand* m_KeyDown;
	InputHandler* m_Handler;
#pragma endregion 
	DirectX m_DirectX;
	Time m_Timer;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


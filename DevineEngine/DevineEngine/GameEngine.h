#pragma once
#include <Windows.h>
#include "Windowing.h"
#include "Direct_X.h"
#include "Time.h"
#include "Camera.h"
#include "Triangle.h"
#include "ColourShader.h"

class GameEngine
{
public:
	GameEngine();
	~GameEngine();
	void InitializeComponents(int cmd) const;
	void RenderLoop();
	void Draw() const;
private:
	bool m_Done;
#pragma region Command Pattern (Inputs)
	Input* m_Input;
	KeyDownCommand* m_KeyDown;
	InputHandler* m_Handler;
#pragma endregion 
	Direct_X* m_DirectX;
	Time* m_Timer;
	Camera* m_Camera;
	Triangle* m_Triangle;
	ColourShader* m_ColourShader;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


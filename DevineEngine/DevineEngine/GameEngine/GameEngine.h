#pragma once
#include <Windows.h>
#include "../Input/Input.h"
#include "../Input/InputHandler.h"
#include "../DirectX/Direct_X.h"
#include "../Timer/Time.h"
#include "../Camera/Camera.h"
#include "../Graphics/Primitives/Triangle/Triangle.h"
#include "../Graphics/Shaders/ColourShader/ColourShader.h"

class KeyDownCommand;

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

	//covers overall running times and such
	Time* m_OverallTimer;
	Camera* m_Camera;
	Triangle* m_Triangle;
	ColourShader* m_ColourShader;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


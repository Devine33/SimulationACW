#pragma once
#include <Windows.h>
#include "../Input/Input.h"
#include "../Input/InputHandler.h"
#include "../DirectX/Direct_X.h"
#include "../Timer/Time.h"
#include "../Camera/Camera.h"
#include "../Graphics/Primitives/Triangle/Triangle.h"
#include "../Graphics/Primitives/Sphere/Sphere.h"
#include "../Graphics/Shaders/Colour/ColourShader.h"
#include "../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"
#include "../GUI/UI.h"
#include "../Texture.h"
class KeyDownCommand;

class GameEngine
{
public:
	GameEngine();
	~GameEngine();
	void InitializeComponents(int cmd);
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
	////covers overall running times and such
	Time* m_OverallTimer;
	Camera* m_Camera;
	Triangle* m_Triangle;
	std::unique_ptr<GeometricPrimitive> m_shape;
	Sphere* m_Sphere;
	ColourShader* m_ColourShader;
	//get UI CLASS WORKING.. LATERR
	UI* m_Ui;
	Texture* m_Texture;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


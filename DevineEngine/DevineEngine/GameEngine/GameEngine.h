#pragma once
#include <Windows.h>

#include "../Input/Input.h"
#include "../Input/KeyDownCommand.h"
//#include "../Input/InputHandler.h"
#include "../DirectX/Direct_X.h"
#include "../Timer/Time.h"
#include "../Camera/Camera.h"
#include "../Graphics/Primitives/Sphere/Sphere.h"
#include "../Graphics/Primitives/Arena/Cylinder.h"
#include "../Graphics/Shaders/Colour/ColourShader.h"
#include "../GUI/UI.h"
#include "../Graphics/Texture/Texture.h"
#include "../Timer/StepTime.h"
#include "../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"
#include "../ContactManifold.h"
#include  <SimpleMath.h>
//class KeyDownCommand;


class GameEngine
{
public:
	GameEngine();
	~GameEngine();
	void InitializeComponents(int cmd);
	void GameLoop();
	void Draw() const;
	void Update(DX::StepTime const& timer);
	void DynamicCollisionResponse();
	void DynamicCollisionDetection();
	void CalculateObjectPhysics(float dt);
	void UpdateObjectPhysics();
private:
	bool m_Done;
#pragma region Command Pattern (Inputs)
	Input* m_Input;
	KeyDownCommand* m_KeyDown;
	/*InputHandler* m_Handler;*/
#pragma endregion 
	Direct_X* m_DirectX;

	Time* m_Timer;
	////covers overall running times and such
	Time* m_OverallTimer;
	Camera* m_Camera;
	Sphere* m_Sphere;
	Cylinder* m_Cylinder;
	ColourShader* m_ColourShader;
	vector<Sphere*> m_SphereList;
	ContactManifold* m_CM;

	Vector3 Pos = { 0,4,0 };
	//get UI CLASS WORKING.. LATERR
	UI* m_Ui;
	Texture* m_Texture;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


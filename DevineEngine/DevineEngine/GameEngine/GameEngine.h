#pragma once
#include <Windows.h>
#include "../DirectX/Direct_X.h"
#include "../Camera/Camera.h"
#include "../Graphics/Primitives/Sphere/Sphere.h"
#include "../Graphics/Primitives/Arena/Cylinder.h"
#include "../Graphics/Shaders/Colour/ColourShader.h"
#include "../GUI/UI.h"
#include "../Graphics/Texture/Texture.h"
#include "../Timer/StepTime.h"
#include "../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"
#include "../Physics/ContactManifold.h"
#include  <SimpleMath.h>
#include "../GravityWell.h"
#include "../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/Mouse.h"

class GameEngine
{
public:
	GameEngine();
	GameEngine(const GameEngine&) = default;
	~GameEngine();
	void InitializeComponents(int cmd, WNDPROC Wndproc);
	void GameLoop();
	void Draw();
	void Update(DX::StepTime const& timer);
	void DynamicCollisionResponse();
	void DynamicCollisionDetection();
	void CalculateObjectPhysics(float dt);
	void Restart();
	void UpdateObjectPhysics();
	void MoveUp() const;
	void MoveDown() const;
	void MoveLeft() const;
	void MoveRight() const;
	void MoveGravityWell(Vector3 vec);
private:
	bool m_Done;
	Direct_X* m_DirectX;
	Camera* m_Camera;
	Sphere* m_Sphere;
	GravityWell* m_GravityWell;
	Cylinder* m_Cylinder;
	ColourShader* m_ColourShader;
	vector<Sphere*> m_SphereList;
	ContactManifold* m_CM;
	UI* m_Ui;
	Texture* m_Texture;
	TwBar *bar;
	double DeltaTime;
	double* DT;
	int* m_NumBalls;
	int balls = 0;
	std::unique_ptr<Mouse> mouse;
};

//static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//GameEngine* ApplicationHandle = nullptr;

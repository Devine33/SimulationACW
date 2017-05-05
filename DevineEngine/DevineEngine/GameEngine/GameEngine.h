#pragma once
#include "../Client.h"
#include "../DirectX/Direct_X.h"
#include "../Camera/Camera.h"
#include "../Graphics/Primitives/Sphere/Sphere.h"
#include "../Graphics/Primitives/Arena/Cylinder.h"
#include "../Graphics/Shaders/Colour/ColourShader.h"
#include "../GUI/UI.h"
#include "../Graphics/Texture/Texture.h"
#include "../Timer/StepTime.h"
#include "../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"
#include "../Physics/GravityWell/GravityWell.h"
#include "../Physics/ContactManifold/ContactManifold.h"
#include  <SimpleMath.h>
#include "../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/Mouse.h"
#include <memory>



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
	void MoveGravityWell();
	std::shared_ptr<Mouse>  GetMouse();
	void GetMousePosition();
	void PauseSimulation();
	void ApplyAttractor();
	double ReturnDelta();
	void ApplyRetractor();
	static void StartRun(GameEngine* G);
	
private:
	bool m_Done;
	Direct_X* m_DirectX;
	Camera* m_Camera;
	Sphere* m_Sphere;
	GravityWell* m_GravityWell;
	Cylinder* m_Cylinder;
	ColourShader* m_ColourShader;
	vector<Sphere*> m_SphereList;
	vector<GravityWell*> m_GravityWellList;
	ContactManifold* m_CM;
	UI* m_Ui;
	Texture* m_Texture;
	Texture* m_MarbleTexture;
	Texture* m_BowlingBall;
	TwBar *bar;
	double DeltaTime;
	double* DT;
	int* m_NumBalls;
	int balls = 0;
	std::shared_ptr<Mouse> mouse;
	Vector3 mousePosInPixels;
	Vector3 Movement;
	float Elasticity;
	bool Simulation_Paused;
	Client* m_Client;
};

//static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
//GameEngine* ApplicationHandle = nullptr;

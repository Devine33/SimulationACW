#include "GameEngine.h"
#include <AntTweakBar.h>
#include <iostream>
#include <thread>
#include "../Tracer/Trace.hpp"
DX::StepTime s_Timer;
GameEngine::GameEngine(): m_Done(false), m_Sphere(nullptr), m_GravityWell(nullptr), m_Cylinder(nullptr), m_Ui(nullptr), bar(nullptr), DeltaTime(0), DT(nullptr), m_NumBalls(nullptr), Elasticity(0), Simulation_Paused(false), m_Client(nullptr)
{
	m_DirectX = new Direct_X;
	m_Camera = new Camera;
	m_ColourShader = new ColourShader;
	//m_Ui = new UI("mybar",m_DirectX->GetDevice());
	m_Texture = new Texture;
	m_MarbleTexture = new Texture;
	m_BowlingBall = new Texture;
	m_CM = new ContactManifold;

}

GameEngine::~GameEngine()
{
	delete m_DirectX;
	delete m_Camera;
	delete m_ColourShader;
	delete m_Cylinder;
	//delete m_Ui;
	delete m_Texture;
	delete m_CM;
}
//Initializes Everything In The Application
void GameEngine::InitializeComponents(int cmd, WNDPROC Wndproc)
{
	TRACE(L"Start");
	//Sphere Variables
	Vector3 CPOS{ 0.0f,0.0f,0.0f };

#pragma region DirectX
	m_DirectX->StartWindowing(cmd,Wndproc);
#pragma endregion 
#pragma region Camera
	m_Camera->CameraSetup(m_DirectX->GetScreenHeight(), m_DirectX->GetScreenWidth());
	m_Camera->SetPosition(0.0f, 5.0f, -14);
	m_Camera->SetRotation(25.0f, 0.0f, 0.0f);
#pragma endregion 
#pragma region Cylinder
	m_Cylinder = new Cylinder(m_DirectX->GetDeviceContext(),10,50);
	m_Cylinder->GetFloorHeight();
	m_Cylinder->SetPosition(CPOS);
#pragma endregion 
#pragma region Sphere
	
	ifstream fin("Configuration/input.txt");
	char buffer[256], ch;
	fin.get(buffer, sizeof(buffer), '=');
	fin >> ch >> balls;
	fin.get(buffer, sizeof(buffer), '=');
	fin >> ch >> Elasticity;
	fin.close();
	if (!TwInit(TW_DIRECT3D11, m_DirectX->GetDevice()))
	{
		//MessageBoxA(.GetHandle(), TwGetLastError(), "AntTweakBar initialization failed", MB_OK | MB_ICONERROR);
	}
	bar = TwNewBar("TweakBar");
	//TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help
	/*int barSize[2] = { m_DirectX->GetScreenWidth(), m_DirectX->GetScreenHeight() };*/

	m_NumBalls = &balls;
	
	TwAddVarRW(bar, "Number Of Balls", TW_TYPE_INT16, m_NumBalls, "");

	for (int i = 0; i < balls ; i++)
	{		
		m_Sphere = new Sphere(m_DirectX->GetDeviceContext(),0.5);
		m_Sphere->SetElasticity(Elasticity);
		/*m_Sphere->SetVelocity(Velo);*/
		m_SphereList.push_back(m_Sphere);
		/*Velo.x = -Velo.x;*/
	}

	m_Sphere->ArrangeGrid(m_SphereList,m_SphereList.size());
#pragma endregion 
	Vector3 gwellpos = { 0,-3,-5 };
	m_GravityWell = new GravityWell(m_DirectX->GetDeviceContext(), 5);
	m_GravityWell->SetPos(gwellpos);
	float elasticity = 0.85;
	fin.get(buffer, sizeof(buffer), '=');

	fin.close();
	m_GravityWell->SetElasticity(elasticity);
	m_Sphere->SetElasticity(elasticity);
#pragma region ColourShader
	m_ColourShader->Initialize(m_DirectX->GetDevice());
#pragma endregion 
#pragma region UI
	////m_Ui->Initialize(m_DirectX->GetDevice());
	//m_Ui->GetWindowSize(800,800);
	//m_Ui->AddIntVariable("var", m_numballs);
	//TwAddVarRW(m_Ui->getBar(), "var",TW_TYPE_INT16 ,&m_numballs, "");
#pragma endregion 
#pragma region Texture
	m_Texture->Initialize(m_DirectX->GetDevice(), L"../DevineEngine/data/Stone01.dds");
	m_MarbleTexture->Initialize(m_DirectX->GetDevice(), L"../DevineEngine/data/Marble.dds");
	m_BowlingBall->Initialize(m_DirectX->GetDevice(), L"../DevineEngine/data/Bowling.dds");
#pragma endregion 

	mouse = std::make_shared<Mouse>(); 
	mouse->SetWindow(m_DirectX->GetHandle());
	mouse->SetMode(Mouse::MODE_RELATIVE);

	std::thread L(StartRun, this);
	//TRACE(L"Initialized");
	GameLoop();
}
//MainLoop
void GameEngine::GameLoop()
{
		MSG m_Msg;
		ZeroMemory(&m_Msg, sizeof(MSG));
		m_Done = false;
		
		/*auto state = mouse->GetState();*/
		while (!m_Done)
		{
			while (PeekMessage(&m_Msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&m_Msg);
				DispatchMessage(&m_Msg);
				
			}
			if (m_Msg.message == WM_QUIT) m_Done = true;
			else
			{
				
				if (Simulation_Paused == false)
				{
					s_Timer.Tick([&]()
					{
						Update(s_Timer);
						//allows Y,H to increase/decrease the time scale, minimum 0.001, maximum 1.0 (globally)
					});
					Draw();
					TwAddVarRW(bar, "Delta", TW_TYPE_DOUBLE, DT, "");
				}
				else
				{
				}
				
			}			
		}
}

//Renders Everything In The Scene
void GameEngine::Draw()
{
	auto Red = Colors::Turquoise;;
	Red.f[3] = 0.3;
	//Matrixes For Camera
	XMMATRIX worldMatrix, view, projection;
	m_DirectX->BeginScene();	
#pragma region Camera
	m_Camera->Render();
	m_Camera->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(view);
	m_Camera->GetProjectionMatrix(projection);
#pragma endregion 
	//Shapes World Matrix
	Matrix world = worldMatrix;
	for (auto element : m_SphereList)
	{			

		//world *= Get elements rotation x,y,z
		world *= DirectX::XMMatrixTranslation(element->GetPos().x, element->GetPos().y, element->GetPos().z);
		
		//element->GetPrim()->Draw(world, view, projection, Colors::White, m_Texture->GetTexture());
		if (element->GetMass() == 1.0f)
		{
			element->GetPrim()->Draw(world, view, projection, Colors::White, m_Texture->GetTexture());
		}
		else if (element ->GetMass() == 5.0f)
		{
			element->GetPrim()->Draw(world, view, projection, Colors::White,m_MarbleTexture->GetTexture());
		}
		else
		{
			element->GetPrim()->Draw(world, view, projection, Colors::White,m_BowlingBall->GetTexture());
		}
		world = worldMatrix;
	}
	world *= DirectX::XMMatrixTranslation(m_Cylinder->GetPosition().x, m_Cylinder->GetPosition().y, m_Cylinder->GetPosition().z);
	m_Cylinder->GetPrim()->Draw(world, view, projection);
	world = worldMatrix;
	world *= DirectX::XMMatrixTranslation(m_GravityWell->GetPos().x, m_GravityWell->GetPos().y, m_GravityWell->GetPos().z);
	
	m_GravityWell->GetPrim()->Draw(world, view, projection,Red);
	m_GravityWell->GetCPrim()->Draw(world, view, projection,Colors::AliceBlue);
	world = worldMatrix;

	TwDraw();
	m_DirectX->EndScene();
	
}
//Physics Loop Controls All Movement And Collision
void GameEngine::Update(DX::StepTime const& timer)
{
	DeltaTime = timer.GetElapsedSeconds();
	DT = &DeltaTime;
	auto t = DT;
	CalculateObjectPhysics(*t);
	m_CM->Clear();
	DynamicCollisionDetection();
	DynamicCollisionResponse();	
	UpdateObjectPhysics();
}
//If Objects Have Collided Get The Collision Point And Respond Appropriately
void GameEngine::DynamicCollisionResponse()
{
	for (int collision = 0; collision < m_CM->GetNumPoints(); ++collision)
	{
		ManifoldPoint &point = m_CM->GetPoint(collision);
		point.contactID1->CollisionResponseWithSphere(point);
	}
}

//Detect If Objects Have Collided With One Another
void GameEngine::DynamicCollisionDetection()
{
	for (auto element : m_SphereList)
	{
		for (auto element2 : m_SphereList)
		{
			
			if (element == element2)
			{
			}
			else
				element->CollisionWithSphere(element2, m_CM);
		}
	}

	for (auto element : m_SphereList)
	{
		for (auto element2 : m_SphereList)
		{

			if (element == element2)
			{
			}
			else
				element->CollisionWithGround(m_Cylinder, m_CM);
		}
	}

	for (auto element : m_SphereList)
	{
		for (auto element2 : m_SphereList)
		{

			if (element == element2)
			{
			}
			else
				element->CollisionWithWalls(m_Cylinder, m_CM);
		}
	}

	for (auto element : m_SphereList)
	{
		m_GravityWell->SpheresInWell(element);
	}

}
//Move All The Spheres By The Appropriate Forces
void GameEngine::CalculateObjectPhysics(float dt)
{
	for (auto element : m_SphereList)
	{
		State S;
		S.v = element->GetVel();
		S.x = element->GetPos();	
		element->Integrate(S,dt);
	}
	
}
//Should Restart
void GameEngine::Restart()
{
	

}

//Update All The Spheres
void GameEngine::UpdateObjectPhysics()
{
	for (auto element : m_SphereList)
	{
		element->Update();
	}
}

void GameEngine::MoveUp() const
{
	m_Camera->MoveUp();
}

void GameEngine::MoveDown() const
{
	m_Camera->MoveDown();
}

void GameEngine::MoveLeft() const
{
	m_Camera->MoveLeft();
}

void GameEngine::MoveRight() const
{
	m_Camera->MoveRight();
}

void GameEngine::MoveGravityWell()
{
	
	m_GravityWell->Move(Movement);
}

std::shared_ptr<Mouse> GameEngine::GetMouse()
{
	return mouse;
}
//gets mouse location on window
void GameEngine::GetMousePosition()
{
	auto state = mouse->GetState();
	XMFLOAT3 MP(float(state.x), float(state.y),-20);
	mousePosInPixels = MP;
	//std::cout <<  mousePosInPixels.x << "\n";
	//std::cout << mousePosInPixels.y << "\n";

	if (state.positionMode == Mouse::MODE_RELATIVE)
	{ //state.x and state.y are relative values; system cursor is not visible
		Movement = Vector3(mousePosInPixels.x * 0.1, 0, -mousePosInPixels.y * 0.1);
	}
	else
	{
		// state.x and state.y are absolute pixel values; system cursor is visible
	}


}

void GameEngine::PauseSimulation()
{
	if (Simulation_Paused)
	{
		Simulation_Paused = false;
	}
	else
	{
		Simulation_Paused = true;
	}
}

void GameEngine::ApplyAttractor()
{
	Mouse::ButtonStateTracker tracker;
	using ButtonState = Mouse::ButtonStateTracker::ButtonState;
	auto state = mouse->GetState();

	Vector3 DeltaTimeV = Vector3(ReturnDelta(), ReturnDelta(), ReturnDelta());
	tracker.Update(state);
	//TwAddVarRW(bar, "Attract Value", TW_TYPE_FLOAT, &DeltaTimeV.x);
	if (tracker.leftButton == ButtonState::PRESSED)
	{	
		DeltaTimeV += Vector3(0.5, 0.5, 0.5);
		std::cout << DeltaTimeV.x << "\n";

		m_GravityWell->ApplyAttractor(DeltaTimeV);
	}
}

double GameEngine::ReturnDelta()
{
	return DeltaTime;
}

void GameEngine::ApplyRetractor()
{
	Mouse::ButtonStateTracker tracker;
	using ButtonState = Mouse::ButtonStateTracker::ButtonState;
	auto state = mouse->GetState();

	Vector3 DeltaTimeV = Vector3(ReturnDelta(), ReturnDelta(), ReturnDelta());
	tracker.Update(state);
	if (tracker.rightButton == ButtonState::PRESSED)
	{
		DeltaTimeV -= Vector3(0.5, 0.5, 0.5);
		std::cout << DeltaTimeV.x;
		m_GravityWell->ApplyRepellor(DeltaTimeV);
	}
	
}

void GameEngine::StartRun(GameEngine* G)
{
	G->m_Client = new Client;
	G->m_Client->Run();
}

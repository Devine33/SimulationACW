#include "GameEngine.h"
#include <AntTweakBar.h>
#include <iostream>
#include <thread>
#include "../Tracer/Trace.hpp"
#include <algorithm>
DX::StepTime s_Timer;
GameEngine::GameEngine(): m_Done(false), m_Sphere(nullptr), m_GravityWell(nullptr), m_Cylinder(nullptr), m_Ui(nullptr), bar(nullptr), DeltaTime(0), DT(nullptr), m_NumBalls(nullptr), Elasticity(0), Simulation_Paused(false), PhysicsFrequency(0)
//, m_Client(nullptr)
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
	m_Cylinder = new Cylinder(m_DirectX->GetDeviceContext(),10,200);
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
	fin.get(buffer, sizeof(buffer), '=');
	fin >> ch >> Friction;
	fin.get(buffer, sizeof(buffer), '=');
	fin >> ch >> IPAddress;
	IPAddress.c_str();
	fin.get(buffer, sizeof(buffer), '=');
	fin >> ch >> Port;
	fin.close();
	std::cout << IPAddress;
	if (!TwInit(TW_DIRECT3D11, m_DirectX->GetDevice()))
	{
		//MessageBoxA(.GetHandle(), TwGetLastError(), "AntTweakBar initialization failed", MB_OK | MB_ICONERROR);
	}
	bar = TwNewBar("TweakBar");
	//TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help
	/*int barSize[2] = { m_DirectX->GetScreenWidth(), m_DirectX->GetScreenHeight() };*/

	
	gwellpos = Vector3(0, -4, -5);
	m_GravityWell = new GravityWell(m_DirectX->GetDeviceContext(), 5);
	m_GravityWell->SetPos(gwellpos);
	m_GravityWellList.push_back(m_GravityWell);
	
	//may need to be a float
	GWellforce = m_GravityWell->GetGravityWellForce();

	TwAddVarRW(bar, "Number Of Balls", TW_TYPE_INT16, &balls, "");

	TwAddVarRW(bar, "Elasticity", TW_TYPE_FLOAT, &Elasticity, "");

	TwAddVarRW(bar, "Friction", TW_TYPE_FLOAT, &Friction, "");

	TwAddVarRW(bar, "XForce", TW_TYPE_FLOAT, &GWellforce.x, "");
	//for (auto element : m_GravityWellList)
	//{
	//	if (element->GetID() == 0)
	//	{
	//		for (int i = 0; i < balls; i++)
	//		{
	//			m_Sphere = new Sphere(m_DirectX->GetDeviceContext(), 0.5);
	//			m_Sphere->SetElasticity(Elasticity);
	//			m_Sphere->IsOwned();
	//			/*m_Sphere->SetVelocity(Velo);*/
	//			m_SphereList.push_back(m_Sphere);
	//			/*Velo.x = -Velo.x;*/
	//		}
	//	}
	//	if (element->GetID() == 1)
	//	{
	//	}
	//}
	//std::cout << m_SphereList.size();

	std::thread T(&GameEngine::CreateSpheres,this);
	T.join();

	m_Sphere->ArrangeGrid(m_SphereList,m_SphereList.size());
#pragma endregion 




	PhysicsFrequency = 60.0f;
	TwAddVarRW(bar, "Physics_Frequency", TW_TYPE_FLOAT, &PhysicsFrequency, "");

	m_GravityWell->SetElasticity(Elasticity);
	m_Sphere->SetElasticity(Elasticity);
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
	//TRACE(L"Initialized");
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
					//replace with variable and put in antweak
					s_Timer.SetFixedTimeStep(true);
					s_Timer.SetTargetElapsedSeconds(1.0f / PhysicsFrequency);
					s_Timer.Tick([&]()
					{
						Update(s_Timer);
						//allows Y,H to increase/decrease the time scale, minimum 0.001, maximum 1.0 (globally)
					});
					Draw();
					TwAddVarRW(bar, "Delta", TW_TYPE_DOUBLE, &DeltaTime, "");
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
	auto Red = Colors::Turquoise;
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
		if (element->GetVisibility() == true)
		{
			world = Matrix::CreateFromYawPitchRoll(element->GetRotation().y * ReturnDelta(), element->GetRotation().x * ReturnDelta(), 0.0f);
			world *= DirectX::XMMatrixTranslation(element->GetPos().x, element->GetPos().y, element->GetPos().z);

			//element->GetPrim()->Draw(world, view, projection, Colors::White, m_Texture->GetTexture());
			if (element->GetMass() == 1.0f)
			{
				element->GetPrim()->Draw(world, view, projection, Colors::White, m_Texture->GetTexture());
				
			}
			else if (element->GetMass() == 5.0f)
			{
				element->GetPrim()->Draw(world, view, projection, Colors::White, m_MarbleTexture->GetTexture());
				
			}
			else
			{
				element->GetPrim()->Draw(world, view, projection, Colors::White, m_BowlingBall->GetTexture());
				
			}
			
		}
		//world *= Get elements rotation x,y,z
		
		world = worldMatrix;
	}
	world *= DirectX::XMMatrixTranslation(m_Cylinder->GetPosition().x, m_Cylinder->GetPosition().y, m_Cylinder->GetPosition().z);
	m_Cylinder->GetPrim()->Draw(world, view, projection);
	world = worldMatrix;
	

	
	//m_GravityWell->GetPrim()->Draw(world, view, projection,Red);
	//m_GravityWell->GetCPrim()->Draw(world, view, projection,Colors::AliceBlue);

	for (auto element : m_GravityWellList)
	{
		world *= DirectX::XMMatrixTranslation(element->GetPos().x, element->GetPos().y, element->GetPos().z);
		if (element->GetID() == 0)
		{
			
			element->GetPrim()->Draw(world, view, projection, Red);
			element->GetCPrim()->Draw(world, view, projection, Colors::AliceBlue);
			element->GetRegion()->Draw(world, view, projection, Colors::Crimson * 0.4);
		}
		if (element->GetID() == 1)
		{
			element->GetPrim()->Draw(world, view, projection, Colors::Cyan * 0.5);
			element->GetCPrim()->Draw(world, view, projection, Colors::AliceBlue);
			element->GetRegion()->Draw(world, view, projection, Colors::DarkGoldenrod * 0.4);
		}
		
		world = worldMatrix;
	}
	world = worldMatrix;

	TwDraw();
	m_DirectX->EndScene();
	
}
//Physics Loop Controls All Movement And Collision
void GameEngine::Update(DX::StepTime const& timer)
{
	std::vector<std::thread> Threads;
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000100);
	//SetThreadAffinityMask(GetCurrentThread(), 4);

	DeltaTime = timer.GetElapsedSeconds();
	/*CalculateObjectPhysics(DeltaTime);*/
	std::thread T(&GameEngine::CalculateObjectPhysics, this, DeltaTime);
	T.join();
	m_CM->Clear();
	/*for (unsigned i = 0; i < 2; ++i)
	{
		Threads.push_back(std::thread(&GameEngine::DynamicCollisionDetection, this));
	}
	std::for_each(Threads.begin(), Threads.end(),
		std::mem_fn(&std::thread::join));*/
	//DynamicCollisionDetection();
	std::thread D(&GameEngine::DynamicCollisionDetection,this);
	D.join();

	std::thread R(&GameEngine::DynamicCollisionResponse, this);
	R.join();
	//DynamicCollisionResponse();	
	//std::thread R(&GameEngine::DynamicCollisionResponse, this);
	std::thread U(&GameEngine::UpdateObjectPhysics, this);
	//UpdateObjectPhysics();
	U.join();
	
	//R.join();
}
//If Objects Have Collided Get The Collision Point And Respond Appropriately
void GameEngine::DynamicCollisionResponse()
{
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000100);
	/*SetThreadAffinityMask(GetCurrentThread(), 4);*/

	for (int collision = 0; collision < m_CM->GetNumPoints(); ++collision)
	{
		ManifoldPoint &point = m_CM->GetPoint(collision);
		point.contactID1->CollisionResponseWithSphere(point);
	}
}

//Detect If Objects Have Collided With One Another
void GameEngine::DynamicCollisionDetection()
{
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000100);
	//SetThreadAffinityMask(GetCurrentThread(), 4);

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
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000100);
	SetThreadAffinityMask(GetCurrentThread(), 4);

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
	HANDLE process = GetCurrentProcess();
	DWORD_PTR processAffinityMask = 0x03;
	//// processor 3
	SetProcessAffinityMask(process, 0b00000100);
	SetThreadAffinityMask(GetCurrentThread(), 4);

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

void GameEngine::BallsWithinRegion(float x, float y, float z)
{
	Vector3 T(x, y, z);
	//IF PEER IS 1 THEN PASS THE POSITION AND VELOCITY TO THE HOST 
		//for each ball in the list
		//for (auto balls : m_SphereList)
		//{
		//		//if the ball is in my sphere then it is noted i can not it with visibility 
		//	if (balls->GetVisibility() == true)
		//	{
		//		m_RegionSphereList.push_back(balls);
		//		//std::cout << m_RegionSphereList.size() << "\n";
		//	}
		//	/*else if (balls->GetVisibility() == false)
		//	{
		//		auto t = std::find(m_RegionSphereList.begin(), m_RegionSphereList.end(), balls);
		//		if (t != m_RegionSphereList.end())
		//		{
		//			m_RegionSphereList.erase(t);
		//		}
		//		
		//	}*/
		//	else { m_RegionSphereList.clear(); }
		//}
		//
	for (auto element : m_GravityWellList)
	{
		for (auto ball : m_SphereList)
		{
			if (element->GetID() == 0)
			{
				ball->IsOwned();
				ball->SetPos(T);		
			}
			if (element->GetID() == 1)
			{
				ball->IsOwned();
				ball->SetNewPos(T);			
			}
		}
	}
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

vector<GravityWell*> GameEngine::ReturnGravityWells()
{
	return m_GravityWellList;
}


void GameEngine::NotifySpheres(float x,float y,float z)
{
	Vector3 T = Vector3(x, y, z);
}

Vector3 GameEngine::GetWellPosition()
{

	for (auto element : m_GravityWellList)
	{
		if (element->GetID() == 0)
		{
			return element->GetPos();
		}
		if(element->GetID() == 1)
		{
			return element->GetPos();
		}
	}
}

void GameEngine::UpdateWellPositions(float x,float y, float z)
{
	Vector3 t = Vector3(x, y, z);
	for (auto element : m_GravityWellList)
	{

		 if (element->GetID() == 1)
		{
			element->SetPos(t);
		}

	}
}

vector<Sphere*> GameEngine::GetSphere()
{
	return m_SphereList;
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

void GameEngine::CreateGravityWell()
{
	GravityWell* m_PeerWell = new GravityWell(m_DirectX->GetDeviceContext(), 5);
	m_PeerWell->SetPos(gwellpos);
	//1 WILL BE PEER
	//0 WILL BE HOST
	m_GravityWellList.push_back(m_PeerWell);
	int totalballs = 0;
	int Balls = 0;
	for (auto element : m_GravityWellList)
	{
		for (auto t : m_SphereList)
		{
			if (element->GetID() == 0)
			{
				t->IsOwned();
				totalballs++;				
				//std::cout << totalballs;
				element->SetTotalBalls(totalballs);
				TwAddVarRW(bar, "Delta", TW_TYPE_INT32, &totalballs, "");
			}

			if (element->GetID() == 1)
			{
				t->NotOwned();
				if (t->NotOwned())
				{
					balls++;
				}
			}
		}
	}

	std::cout << totalballs << "\n TOTAL BALLS OWNED \n";
	std::cout << Balls << "\n TOTAL BALLS NOT OWNED \n";

}

void GameEngine::MoveGravityWellUp()
{
	for (auto element : m_GravityWellList)
	{
		element->MoveUp();
	}
}

void GameEngine::IncreasePhysics()
{
	PhysicsFrequency += 0.1f;
}

void GameEngine::DecreasePhysics()
{
	PhysicsFrequency -= 0.1f;
}

string GameEngine::GetIP()
{
	return IPAddress;
}

int GameEngine::ReturnPort()
{
	return Port;
}

void GameEngine::Calc_Physics()
{
	
}

TwBar* GameEngine::ReturnAntTweak()
{
	return bar;
}

void GameEngine::MoveGravityWellDown()
{
	for (auto element : m_GravityWellList)
	{
		element->MoveDown();
	}
}

void GameEngine::CreateSpheres()
{
	for (int i = 0; i < balls; i++)
	{
		m_Sphere = new Sphere(m_DirectX->GetDeviceContext(), 0.5);
		m_Sphere->SetElasticity(Elasticity);
		//m_Sphere->IsOwned();
		//m_Sphere->SetVelocity(Velo);
		m_SphereList.push_back(m_Sphere);
		//Velo.x = -Velo.x;
	}
}

void GameEngine::MoveForward()
{
	m_Camera->MoveForward();
}

void GameEngine::MoveBackward()
{
	m_Camera->MoveBack();
}

#include "GameEngine.h"
#include "../Tracer/Trace.hpp"
#include "GeometricPrimitive.h"
#include <string>
#include <iostream>
#include <thread>
#include <fstream>
DX::StepTime s_Timer;

GameEngine::GameEngine(): m_Done(false), m_Input(nullptr), m_KeyDown(nullptr)
{
	m_DirectX = new Direct_X;
	m_Timer = new Time;	
	//m_OverallTimer = new Time;
	//SORT THIS LATER 
	m_Camera = new Camera;
	if(!m_Camera)
	{
		TRACE(L"NO Camera");
	}
	m_Input = new Input;
	m_KeyDown = new KeyDownCommand(m_Input);
	/*m_Handler = new InputHandler;*/
	m_ColourShader = new ColourShader;
	m_Cylinder = new Cylinder;
	m_Ui = new UI;
	m_Texture = new Texture;
	m_CM = new ContactManifold;
}


GameEngine::~GameEngine()
{
	delete m_DirectX;
	//delete m_Timer;
	delete m_Camera;
	delete m_Input;
	delete m_KeyDown;
	//delete m_Handler;
}

void GameEngine::InitializeComponents(int cmd)
{
	SimpleMath::Vector3 Velo(3, 0, 0);
	bool result = true;
	m_DirectX->StartWindowing(cmd);
	m_Camera->CameraSetup(m_DirectX->GetScreenHeight(), m_DirectX->GetScreenWidth());
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_Cylinder->Initialize(m_DirectX->GetDeviceContext());
	//m_Sphere = new Sphere(m_DirectX->GetDeviceContext());
	//ifstream fin("Configuration/input.txt");
	//fin >> *m_Sphere;
	for (int i = 0; i <6 ; i++)
	{
		m_Sphere = new Sphere(m_DirectX->GetDeviceContext(),0.5);
		m_Sphere->SetVelocity(Velo);
		m_Sphere->SetPos(Pos *= {0,0.1f,0});
		m_Sphere->SetMass(10.0f);
		m_SphereList.push_back(m_Sphere);
	}
	
	// auto count = std::count(m_SphereList.begin(), m_SphereList.end(), 1);
	
	result = m_ColourShader->Initialize(m_DirectX->GetDevice());
	if (!result)
	{
		TRACE(L"SHADER FAILED");
	}

	m_Ui->Initialize(m_DirectX->GetDevice());
	m_Ui->GetWindowSize(m_DirectX->GetScreenWidth(), m_DirectX->GetScreenHeight());
	result = m_Texture->Initialize(m_DirectX->GetDevice(), L"../DevineEngine/data/Stone01.dds");
	if (!result)
	{
		TRACE(L"SHADER FAILED");
	}

	TRACE(L"Initialized");


}

void GameEngine::GameLoop()
{
		MSG m_Msg;
		ZeroMemory(&m_Msg, sizeof(MSG));
		/*std::wstring message = std::to_wstring(m_Msg.wParam).c_str();
		std::wstring newl = message.append(L"\n").c_str();*/
		m_Done = false;
		//m_OverallTimer->StartTime();
		while (!m_Done)
		{
			/*m_OverallTimer->CalcFrameRate();
			m_OverallTimer->TotalRunningTime();*/
			// Handle the windows messages.
			//inut handler moved to window;
			if (PeekMessage(&m_Msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&m_Msg);
				DispatchMessage(&m_Msg);

				if (m_Msg.message == WM_QUIT) m_Done = true;
			}
			else
			{
				s_Timer.Tick([&]()
				{
					Update(s_Timer);
				});
				Draw();
			}
				
		}
}

void GameEngine::Draw() const
{	
	


	bool result;
	XMMATRIX worldMatrix, view, projection;
	m_DirectX->BeginScene();
	/*TRACE(L"GameEngine::Draw() \n");*/	
	m_Camera->Render();
	m_Camera->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(view);
	m_Camera->GetProjectionMatrix(projection);

	XMMATRIX world = XMMatrixTranslation(-1,0,0);
	
	for (auto element : m_SphereList)
	{
		//element->SetPos(Pos);
		world *= XMMatrixTranslation(element->GetPos().x, element->GetPos().y, element->GetPos().z);
		//element.(world += XMMatrixTranslation(-0.5, 0, 0), view, projection, m_Texture->GetTexture());
		element->GetPrim()->Draw(world, view, projection,Colors::White, m_Texture->GetTexture());
		world *= XMMatrixTranslation(0.6,0,0);
	}

	m_Cylinder->Draw(worldMatrix, view, projection);
	//result = m_ColourShader->Render(m_DirectX->GetDeviceContext(), m_Triangle->GetIndexCount(), worldMatrix, view, projection);

	//if (!result)
	//{
	//	TRACE(L"FAILED SHADER");
	//}
	TwDraw();
	//shader
	m_DirectX->EndScene();
}

void GameEngine::Update(DX::StepTime const& timer)
{
	float delta = float(timer.GetElapsedSeconds());

	CalculateObjectPhysics(delta);
	m_CM->Clear();
	DynamicCollisionDetection();

	DynamicCollisionResponse();
	//
	UpdateObjectPhysics();

}

void GameEngine::DynamicCollisionResponse()
{
	for (int collision = 0; collision < m_CM->GetNumPoints(); ++collision)
	{
		ManifoldPoint &point = m_CM->GetPoint(collision);
		point.contactID1->CollisionResponseWithSphere(point);
	}
}

void GameEngine::DynamicCollisionDetection()
{
	for (auto element : m_SphereList)
	{
		for (auto element2 : m_SphereList)
		{
			/*if (element == element2)
			{
				continue;
			}
			else
				element->CollisionWithSphere(element2, m_CM);*/
		}
	}
}

void GameEngine::CalculateObjectPhysics(float dt)
{
	for (auto element : m_SphereList)
	{
		element->CalculatePhysics(dt);
	}
}

void GameEngine::UpdateObjectPhysics()
{
	for (auto element : m_SphereList)
	{
		element->Update();
	}
}

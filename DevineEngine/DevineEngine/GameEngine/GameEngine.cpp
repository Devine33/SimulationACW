#include "GameEngine.h"
#include "../Input/KeyDownCommand.h"
#include "../Tracer/Trace.hpp"
#include "GeometricPrimitive.h"
#include <string>
#include <iostream>


GameEngine::GameEngine(): m_Done(false), m_Input(nullptr), m_KeyDown(nullptr), m_Handler(nullptr)
{
	m_DirectX = new Direct_X;
	m_Timer = new Time;	
	m_OverallTimer = new Time;
	//SORT THIS LATER 
	m_Camera = new Camera;
	if(!m_Camera)
	{
		TRACE(L"NO Camera");
	}
	m_Input = new Input;
	m_KeyDown = new KeyDownCommand(m_Input);
	m_Handler = new InputHandler;
	m_Triangle = new Triangle;
	if (!m_Triangle)
	{
		TRACE(L"NO MODEL");
	}
	m_ColourShader = new ColourShader;
	m_Sphere = new Sphere;
	//ifstream myfile("input.txt");
	//if (myfile.is_open())
	//{
	//	myfile >> *m_Sphere;
	//	myfile.close();
	//}
	//else cout << "Unable to open file";
	m_Ui = new UI;
}


GameEngine::~GameEngine()
{
	delete m_DirectX;
	delete m_Timer;
	delete m_Camera;
	delete m_Input;
	delete m_KeyDown;
	delete m_Handler;
}

void GameEngine::InitializeComponents(int cmd)
{
	bool result = true;
	m_DirectX->StartWindowing(cmd);
	m_Camera->CameraSetup(m_DirectX->GetScreenHeight(), m_DirectX->GetScreenWidth());
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	result = m_Triangle->Initialize(m_DirectX->GetDevice());
	m_shape = GeometricPrimitive::CreateSphere(m_DirectX->GetDeviceContext());
	m_Sphere->Initialize(m_DirectX->GetDeviceContext());

	if(!result)
	{
		TRACE(L"TRIANGLE FAILED");
	}
	result = m_ColourShader->Initialize(m_DirectX->GetDevice());
	if (!result)
	{
		TRACE(L"SHADER FAILED");
	}

	m_Ui->Initialize(m_DirectX->GetDevice());
	m_Ui->GetWindowSize(m_DirectX->GetScreenWidth(), m_DirectX->GetScreenHeight());

	TRACE(L"Initialized");


}

void GameEngine::RenderLoop()
{
		MSG m_Msg;
		ZeroMemory(&m_Msg, sizeof(MSG));
		/*std::wstring message = std::to_wstring(m_Msg.wParam).c_str();
		std::wstring newl = message.append(L"\n").c_str();*/
		
		// Loop until there is a quit message from the window or the user.
		m_Done = false;
		m_OverallTimer->StartTime();
		while (!m_Done)
		{
			m_OverallTimer->CalcFrameRate();
			m_OverallTimer->TotalRunningTime();
			// Handle the windows messages.
			if (PeekMessage(&m_Msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&m_Msg);
				DispatchMessage(&m_Msg);
			}
				switch (m_Msg.message)
				{
				case WM_KEYDOWN:
					m_Handler->SetCommand(m_KeyDown);
					m_Handler->KeyPress();
					TRACE(std::to_wstring(m_Msg.wParam).append(L"\n").c_str());
					break;
				case WM_QUIT:
					m_OverallTimer->EndTime();
					m_OverallTimer->Elapsed();
					m_Done = true;
					break;	
				case WM_LBUTTONDOWN:
					std::cout << "button clicked \n";
					m_Handler->SetCommand(m_KeyDown);
					m_Handler->MouseClick();
				default:
					{		
						m_Timer->StartTime();
						Draw();
						m_Timer->EndTime();
						m_Timer->DeltaTime();
						
					}				
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
	
	XMMATRIX world = XMMatrixTranslation(0, 0, -m_OverallTimer->GetTotalRunningTime()) * XMMatrixRotationRollPitchYaw(0, 0,0 /*m_Timer->GetDeltaTime()*/);
	//m_Triangle->Render(m_DirectX->GetDeviceContext());
	//m_shape->Draw(world, viewMatrix, projectionMatrix);
	m_Sphere->Draw(world, view, projection);
	result = m_ColourShader->Render(m_DirectX->GetDeviceContext(), m_Triangle->GetIndexCount(), worldMatrix, view, projection);
	if (!result)
	{
		TRACE(L"FAILED SHADER");
	}
	TwDraw();
	//shader
	m_DirectX->EndScene();
}

#include "GameEngine.h"
#include <string>
#include "Trace.hpp"

GameEngine::GameEngine(): m_Done(false), m_Input(nullptr), m_KeyDown(nullptr), m_Handler(nullptr)
{
	m_DirectX = new Direct_X;
	m_Timer = new Time;
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

void GameEngine::InitializeComponents(int cmd) const
{
	bool result = true;
	m_DirectX->StartWindowing(cmd);
	m_Camera->CameraSetup(m_DirectX->GetScreenHeight(), m_DirectX->GetScreenWidth());
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	result = m_Triangle->Initialize(m_DirectX->GetDevice());
	if(!result)
	{
		TRACE(L"TRIANGLE FAILED");
	}
	result = m_ColourShader->Initialize(m_DirectX->GetDevice());
	if (!result)
	{
		TRACE(L"SHADER FAILED");
	}
	TRACE(L"Initialized");
}

void GameEngine::RenderLoop()
{
		MSG m_Msg;
		ZeroMemory(&m_Msg, sizeof(MSG));
		std::wstring message = std::to_wstring(m_Msg.wParam).c_str();
		std::wstring newl = message.append(L"\n").c_str();

		// Loop until there is a quit message from the window or the user.
		m_Done = false;
		m_Timer->StartTime();
		while (!m_Done)
		{
			m_Timer->GetTimeNow();
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
					m_Done = true;
					m_Timer->EndTime();
					m_Timer->GetElapsed();
				default:
					{
						Draw();
						
					}
			
				}		
		}
}

void GameEngine::Draw() const
{
	bool result;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_DirectX->BeginScene();
	TRACE(L"GameEngine::Draw() \n");	
	m_Camera->Render();
	m_Camera->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Camera->GetProjectionMatrix(projectionMatrix);

	m_Triangle->Render(m_DirectX->GetDeviceContext());

	result = m_ColourShader->Render(m_DirectX->GetDeviceContext(), m_Triangle->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		TRACE(L"FAILED SHADER");
	}
	//shader
	m_DirectX->EndScene();
}

#include "GameEngine.h"
#include <string>
#include "Trace.hpp"

GameEngine::GameEngine(): m_Done(false), m_Input(nullptr), m_KeyDown(nullptr), m_Handler(nullptr)
{
}


GameEngine::~GameEngine()
{
}

void GameEngine::InitializeComponents(int cmd)
{
	m_DirectX.StartWindowing(cmd);
	m_Input = new Input;
	m_KeyDown = new KeyDownCommand(m_Input);
	m_Handler = new InputHandler;

	//
	//RenderLoop
}

void GameEngine::RenderLoop()
{
		MSG m_Msg;
		ZeroMemory(&m_Msg, sizeof(MSG));
		std::wstring message = std::to_wstring(m_Msg.wParam).c_str();
		std::wstring newl = message.append(L"\n").c_str();

		// Loop until there is a quit message from the window or the user.
		m_Done = false;
		m_Timer.StartTime();
		while (!m_Done)
		{
			m_Timer.GetTimeNow();
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
				m_Timer.EndTime();
				m_Timer.GetElapsed();
				default:
				/*TRACE(L"Rendering \n");	*/
				Draw();
			}		
	}
}

void GameEngine::Draw()
{
	m_DirectX.BeginScene();

	m_DirectX.EndScene();
}

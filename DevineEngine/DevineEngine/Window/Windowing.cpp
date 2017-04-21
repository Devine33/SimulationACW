#include "Windowing.h"
#include <string>
#include "../Tracer/Trace.hpp"
#include <AntTweakBar.h>
#include <iostream>
// Step 4: the Window Procedure COULD  POSSIBLE MOVE THIS TO GAME ENGINE but works just fine here :D
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (TwEventWin(hwnd, msg, wParam, lParam))
		return 0;

	TRACE(L"Windowing::WndProc \n");
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		TRACE(L"Windowing::WndProc::WM_KEYDOWN \n");
		if(wParam == 27)
		{
			PostQuitMessage(0);
		}
		if(wParam == 82)
		{
			TRACE(L"System Reset\n");
		}
		break;
	case WM_LBUTTONDOWN:
		std::cout << "button clicked \n";
		/*
		ApplicationHandle->ReturnHandle()->SetCommand()*/
		/*m_Input->SetCommand(m_KeyDown);
		m_Handler->MouseClick();*/
		break;
	default:
		return  DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

Windowing::Windowing(): m_HInstance(nullptr), m_Done(false), m_Result(false), m_Hwnd(nullptr)//, m_Input(nullptr), m_KeyDown(nullptr), m_Handler(nullptr)
{
	m_InputHandler = new InputHandler;
	m_KeyDown = new KeyDownCommand(m_Input);
}

Windowing::~Windowing()
{
	TRACE(L"Window Destroyed \n");
}

bool Windowing::FillWindowStruct()
{
	TRACE(L"Windowing::FillWindowStruct \n");
	//Step 1: Registering the Window Class
	m_Window.cbSize = sizeof(WNDCLASSEX);
	m_Window.style = 0;
	m_Window.lpfnWndProc = WndProc;
	m_Window.cbClsExtra = 0;
	m_Window.cbWndExtra = 0;
	m_Window.hInstance = m_HInstance;
	m_Window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	m_Window.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_Window.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	m_Window.lpszMenuName = nullptr;
	m_Window.lpszClassName = m_ClassName;
	m_Window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassEx(&m_Window))
	{
		MessageBox(nullptr, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return false;
	}
	return true;
	
}

bool Windowing::HardwareHandle(int cmd)
{
	m_Hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		m_ClassName,
		L"DivineEngine",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,
		nullptr, nullptr, m_HInstance, nullptr);

	if (m_Hwnd == nullptr)
	{
		MessageBox(nullptr, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	ShowWindow(m_Hwnd, cmd);
	UpdateWindow(m_Hwnd);

	return true;
}

void Windowing::WindowCreation(int cmd)
{
	FillWindowStruct();
	HardwareHandle(cmd);
}

HWND const& Windowing::GetHandle() const
{
	return m_Hwnd;
}

int const& Windowing::GetScreenWidth() const
{
	return screenWidth;
}

int const& Windowing::GetScreenHeight() const
{
	return screenHeight;
}

KeyDownCommand* Windowing::ReturnKeyHandle()
{
	return m_KeyDown;
}

InputHandler* Windowing::ReturnInputHandle()
{
	return m_InputHandler;
}
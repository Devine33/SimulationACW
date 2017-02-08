#include "Windowing.h"
#include "Trace.hpp"
#include <string>
// Step 4: the Window Procedure COULD  POSSIBLE MOVE THIS TO GAME ENGINE but works just fine here :D
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
	default:
		return  DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

Windowing::Windowing(): m_HInstance(nullptr), m_Done(false), m_Result(false), m_Hwnd(nullptr)//, m_Input(nullptr), m_KeyDown(nullptr), m_Handler(nullptr)
{
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

//THIS WILL BE MOVING TO GAME ENGINE SEE WEBSITE ABOUT GAME LOOP AND WIN32
//void Windowing::Run()
//{
//	MSG m_Msg;
//	ZeroMemory(&m_Msg, sizeof(MSG));
//	std::wstring message = std::to_wstring(m_Msg.wParam).c_str();
//	std::wstring newl = message.append(L"\n").c_str();
//	// Loop until there is a quit message from the window or the user.
//	m_Done = false;
//	while (!m_Done)
//	{
//		// Handle the windows messages.
//		if (PeekMessage(&m_Msg, nullptr, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&m_Msg);
//			DispatchMessage(&m_Msg);
//		
//		}
//
//		switch (m_Msg.message)
//		{
//		case WM_KEYDOWN:
//			m_Handler->SetCommand(m_KeyDown);
//			m_Handler->KeyPress();
//			
//			TRACE(std::to_wstring(m_Msg.wParam).append(L"\n").c_str());
//			break;
//			case WM_QUIT:
//				m_Done = true;
//			/*default:
//				TRACE(L"Rendering \n");		*/			
//		}
//		// If windows signals to end the application then exit out.
//		//if (m_Msg.message == WM_QUIT)
//		//{
//		//	m_Done = true;
//		//}
//		//else
//		//{
//		//	// Otherwise do the  rendering
//		//	m_Done = false;
//		//}
//
//	}
//}

//LRESULT Windowing::MessageHandler(const HWND hwnd, const UINT umsg, const WPARAM wparam, const LPARAM lparam)
//{
//	switch (umsg)
//	{
//		// Check if a key has been pressed on the keyboard.
//	//case WM_KEYDOWN:
//	//{
//	//	TRACE(L"Key is Pressed \n");
//	//	switch (wparam)
//	//	{
//	//	case VK_ESCAPE:
//	//		PostQuitMessage(0);
//	//		::WndProc(hwnd, WM_CLOSE, wparam, lparam);
//	//	}
//	//}
//
//	// Check if a key has been released on the keyboard.
//	//case WM_KEYUP:
//	//{
//	//	return 0;
//	//}
//
//	// Any other messages send to the default message handler as our application won't make use of them.
//		default:
//		{
//			return TRACE(L"MessageHandler");
//		}
//	}
//}

#pragma once
//#include "../Input/InputHandler.h"
//#include "../Input/KeyDownCommand.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
class Windowing
{
public:
#pragma region Gang Of Five
	Windowing();
	Windowing(const Windowing& ) = default;
	Windowing& operator=(const Windowing&) =  default;
	Windowing(Windowing&&) = default;
	Windowing& operator=(Windowing&&) = default;
	~Windowing();
#pragma endregion 
	
	bool FillWindowStruct(WNDPROC WndProc);
	bool HardwareHandle(int cmd);
	void WindowCreation(int cmd, WNDPROC Wndproc);
	/*static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);*/
	HWND const& GetHandle() const;
	//This is the Messaging loop
	int const& GetScreenWidth() const;
	int const& GetScreenHeight() const;
	void Run();
	 LRESULT MessageHandler(const  HWND hwnd, const  UINT umsg, const  WPARAM wparam, const LPARAM lparam);
	/* InputHandler* ReturnInputHandle();
	 KeyDownCommand* ReturnKeyHandle();*/
private:
	const LPCWSTR m_ClassName = L"myWindowClass";
	HINSTANCE m_HInstance;
	WNDCLASSEX m_Window;	
	bool m_Done, m_Result;
	HWND m_Hwnd;
	//Input* m_Input;
	//InputHandler* m_InputHandler;
	//KeyDownCommand* m_KeyDown;
	/*int screenWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	int screenHeight = GetSystemMetrics(SM_CYSCREEN) / 2;*/
	
	int screenWidth = 800;
	int screenHeight = 600;
};

//static Windowing* ApplicationHandle = nullptr;




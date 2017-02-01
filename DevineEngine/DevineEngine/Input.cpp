#include "Input.h"
#include <iostream>
#include "Trace.hpp"
Input::Input(): m_keys{true}
{
}

void Input::Initialize()
{
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i<256; i++)
	{
		m_keys[i] = false;
	}
}

void Input::KeyDown(const unsigned int input)
{
	TRACE(L"Input::Keydown \n");
	// If a key is pressed then save that state in the key array.
	GetAsyncKeyState(input);
}

void Input::KeyUp(const unsigned int key)
{
	std::cout << "Input::KeyUp \n";
}

bool Input::IsKeyDown(const unsigned int key) const
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

Input::~Input()
{
}

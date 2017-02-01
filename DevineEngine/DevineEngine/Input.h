#pragma once
#include <Windows.h>
class Input
{
public:
	Input();
	~Input();

	void Initialize();
	void KeyDown(const unsigned int input);
	void KeyUp(const unsigned int key);
	bool IsKeyDown(const unsigned int key)const;
private:
	bool m_keys[256];

};


#pragma once
#include "Command.h"
//concrete
class InputHandler
{
public:
	InputHandler();
	~InputHandler();
	void SetCommand(Command* cmd);
	void KeyPress() const;
private:
	Command* m_Cmd;
};


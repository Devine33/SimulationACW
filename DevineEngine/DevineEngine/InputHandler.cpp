#include "InputHandler.h"
#include "Trace.hpp"


InputHandler::InputHandler(): m_Cmd(nullptr)
{
}

InputHandler::~InputHandler()
{
}

void InputHandler::SetCommand(Command* cmd)
{
	TRACE(L"COMMAND SET \n");
	m_Cmd = cmd;
}

void InputHandler::KeyPress() const
{
	TRACE(L"Key Pressed \n");
	m_Cmd->Execute();
}

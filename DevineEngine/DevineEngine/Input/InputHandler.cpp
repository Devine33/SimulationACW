#include "InputHandler.h"
#include "../Tracer/Trace.hpp"


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

void InputHandler::MouseClick() const
{
	TRACE(L"Clicking \n");
	m_Cmd->Execute();
}

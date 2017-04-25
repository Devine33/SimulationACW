#include "KeyDownCommand.h"
#include "../Tracer/Trace.hpp"

KeyDownCommand::KeyDownCommand(Input* input): m_Input(nullptr)
{
}

KeyDownCommand::~KeyDownCommand()
{
}

void KeyDownCommand::Execute()
{
	//TRACE(L"KeyDownCommand \n");
	/*m_Input->KeyDown();*/
}

//void KeyDownCommand::Execute(const unsigned key)
//{
//	
//}

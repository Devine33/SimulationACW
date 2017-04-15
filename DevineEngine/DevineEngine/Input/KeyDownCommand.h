#pragma once
#include "Command.h"
#include "Input.h"

class KeyDownCommand : public Command
{
public:
	explicit KeyDownCommand(Input* input);
	virtual ~KeyDownCommand();
	void Execute() override;
	/*void Execute(const unsigned) override;*/
private:
	Input *m_Input;
};


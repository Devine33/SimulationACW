#pragma once
class Command
{
public:
	virtual ~Command();
	virtual void Execute() = 0;
	/*virtual void Execute(const unsigned int) = 0;*/
};


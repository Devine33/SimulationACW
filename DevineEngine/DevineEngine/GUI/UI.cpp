#include "UI.h"
UI::UI()
{
}


UI::~UI()
{
}

void UI::Initialize(ID3D11Device* DEV)
{
	TwInit(TW_DIRECT3D11, DEV);
}

void UI::GetWindowSize(int width, int height)
{
	TwWindowSize(width, height);
}

void UI::CreateNewBar(const std::string const barname )
{
	
	bar =TwNewBar(barname.c_str());
}

TwBar* UI::getBar()
{
	return bar;
}

//void UI::AddFloatVariable(TwBar* myBar, std::string name,float* var)
//{
//	TwAddVarRW(myBar, name.c_str(), TW_TYPE_FLOAT, var);
//}

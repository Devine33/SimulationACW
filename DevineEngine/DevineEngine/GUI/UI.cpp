#include "UI.h"
UI::UI(std::string& barname, ID3D11Device* DEV): bar(nullptr)
{

	if (!TwInit(TW_DIRECT3D11, DEV))
	{
		//MessageBoxA(.GetHandle(), TwGetLastError(), "AntTweakBar initialization failed", MB_OK | MB_ICONERROR);
	}
	bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' ");
	/*TwInit(TW_DIRECT3D11, DEV);
	bar = TwNewBar(barname.c_str());*/
}


UI::~UI()
{
}

void UI::GetWindowSize(int width, int height)
{
	TwWindowSize(width, height);
}

void UI::AddIntVariable(std::string,int& num)
{
	TwAddVarRW(getBar(), "var", TW_TYPE_INT16, &num, "");
}

TwBar* UI::getBar()
{
	return bar;
}

//void UI::AddFloatVariable(TwBar* myBar, std::string name,float* var)
//{
//	TwAddVarRW(myBar, name.c_str(), TW_TYPE_FLOAT, var);
//}

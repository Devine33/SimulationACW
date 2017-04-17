#pragma once
#include <AntTweakBar.h>
#include <d3d11.h>
class UI
{
public:
	UI();
	~UI();
	void Initialize(ID3D11Device*);
	void GetWindowSize(int width, int height);
	void CreateNewBar(const std::string const);
	void AddFloatVariable(TwBar*,std::string,float,float&);
	TwBar* getBar();
private:
	TwBar* bar;
};


#pragma once
#include <AntTweakBar.h>
#include <d3d11.h>
class UI
{
public:
	UI(std::string&, ID3D11Device* DEV);
	~UI();
	void Initialize(ID3D11Device*);
	void GetWindowSize(int width, int height);
	//void CreateNewBar(const std::string const);
	void AddIntVariable(std::string, int& num);
	TwBar* getBar();
private:
	TwBar* bar;
};


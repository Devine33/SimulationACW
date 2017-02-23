#pragma once
//base for drawing objects 
class Renderer
{
	
public:
	virtual ~Renderer();
	virtual void Draw() = 0;
};


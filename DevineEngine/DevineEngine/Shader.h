#pragma once
#include <directxmath.h>

//base for shaders 
class Shader
{
	

public:
	Shader()
	{
	}

	virtual ~Shader()
	{
	}

	//will cleanup any varibales used by the shader 
	virtual void Cleanup() = 0;
	virtual void SetParameters() = 0;
	virtual void Render() = 0;
	//Will Initialize and create the shader 
	virtual void Create() = 0;
	//Will define the shaders input & ConstantBuffers
	virtual void DefineInputLayouts() = 0;

	enum ShaderType
	{
		VertexShader,
		HullShader,
		DomainShader,
		GeometryShader,
		PixelShader
	};

	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
};


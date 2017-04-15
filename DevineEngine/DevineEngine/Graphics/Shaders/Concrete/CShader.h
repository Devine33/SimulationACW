#pragma once
#include "../Base/Shader.h"
#include <d3d11.h>

class CShader : Shader
{
public:
	CShader();
	virtual ~CShader();
	void Cleanup() override;
	void SetParameters() override;
	void Render() override;
	void Create() override;
	void SetShader(ShaderType,ID3D11DeviceContext*) const;
	void DefineInputLayouts() override;
private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11HullShader* m_HullShader;
	ID3D11DomainShader* m_DomainShader;
	ID3D11GeometryShader* m_GeometryShader;

};



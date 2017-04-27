#include "CShader.h"



CShader::CShader(): m_VertexShader(nullptr), m_PixelShader(nullptr), m_HullShader(nullptr), m_DomainShader(nullptr), m_GeometryShader(nullptr)
{
}


CShader::~CShader()
{
}

void CShader::Cleanup()
{
}

void CShader::SetParameters()
{
}

void CShader::Render()
{
}

void CShader::Create()
{
}


/**
 * \brief Sets the ShaderType
 * \param st the shader type to be given ShaderType::VertexShader
 * \param deviceContext passes DeviceContext to allow the shader to be set
 */
void CShader::SetShader(ShaderType st,ID3D11DeviceContext* deviceContext) const
{
	switch(st)
	{
	case VertexShader:
		{
		deviceContext->VSSetShader(m_VertexShader, nullptr, 0);
		}
	case HullShader:
		{
		deviceContext->HSSetShader(m_HullShader, nullptr, 0);
		}
	case DomainShader:
		{
		deviceContext->DSSetShader(m_DomainShader, nullptr, 0);
		}
	case GeometryShader:
		{
		deviceContext->GSSetShader(m_GeometryShader, nullptr, 0);
		}
	case PixelShader:
		{
		deviceContext->PSSetShader(m_PixelShader, nullptr, 0);
		}
	}
}

void CShader::DefineInputLayouts()
{
}

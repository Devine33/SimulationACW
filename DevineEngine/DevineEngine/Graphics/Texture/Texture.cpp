#include "Texture.h"
#include "../include/DDSTextureLoader.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;


	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, filename, nullptr, &m_texture, 0, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

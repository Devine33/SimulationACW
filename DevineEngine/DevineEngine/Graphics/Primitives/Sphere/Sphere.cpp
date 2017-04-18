#include "Sphere.h"
Sphere::Sphere()
{
}


Sphere::~Sphere()
{
	/*m_shape.reset();*/
}
//deviceContext
void Sphere::Initialize(ID3D11DeviceContext* context)
{	
	m_shape = DirectX::GeometricPrimitive::CreateSphere(context);
}

void Sphere::Draw(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection, ID3D11ShaderResourceView* texture) const
{
	
	m_shape->Draw(world, view, projection,DirectX::Colors::White,texture);
}

void Sphere::Read(std::istream& in)
{
	char buffer[256], ch;

	in.get(buffer, sizeof(buffer), '=');
}
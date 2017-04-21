#include "Cylinder.h"



Cylinder::Cylinder()
{
}


Cylinder::~Cylinder()
{
}

void Cylinder::Initialize(ID3D11DeviceContext* context)
{
	m_shape = DirectX::GeometricPrimitive::CreateCylinder(context,10,10);
}
 
void Cylinder::Draw(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX projection) const
{
	
		m_shape->Draw(world, view, projection, DirectX::Colors::White);
	
	
}

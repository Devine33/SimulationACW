#include "Cylinder.h"
Cylinder::Cylinder(ID3D11DeviceContext* context,float height,float diameter): m_Height(0)
{
	m_shape = DirectX::GeometricPrimitive::CreateCylinder(context, height, diameter);
	m_FloorHeight = height / 2;
	m_Radius = diameter / 2;
}

Cylinder::~Cylinder()
{
}

void Cylinder::SetPosition(DirectX::SimpleMath::Vector3 POSIN)
{
	m_Position = POSIN;
}

void Cylinder::SetHeight(int Height)
{
	m_Height = Height;
}

std::shared_ptr<DirectX::GeometricPrimitive> Cylinder::GetPrim() const
{
	return m_shape;
}

int Cylinder::GetFloorHeight()
{
	return m_FloorHeight;
}

float Cylinder::GetRadius()
{
	return m_Radius;
}

DirectX::SimpleMath::Vector3 Cylinder::GetPosition() const
{
	return m_Position;
}

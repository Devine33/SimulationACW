#include "GravityWell.h"



GravityWell::GravityWell(ID3D11DeviceContext* context, float radius): Sphere(context,radius),m_Position(0,0,0)
{
	m_shape = DirectX::GeometricPrimitive::CreateSphere(context, radius);
}


GravityWell::~GravityWell()
{
}

Vector3 GravityWell::GetPos() const
{
	return m_Position;
}

void GravityWell::SetPos(Vector3 posin)
{
	m_Position = posin;
}

void GravityWell::Move(Vector3 Pos)
{
	m_Position += Pos;
}

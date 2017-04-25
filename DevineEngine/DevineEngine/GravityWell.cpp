#include "GravityWell.h"



GravityWell::GravityWell(ID3D11DeviceContext* context, float radius): Sphere(context,radius)
{
	m_shape = DirectX::GeometricPrimitive::CreateSphere(context, radius);
}


GravityWell::~GravityWell()
{
}

Vector3 GravityWell::GetPos()
{
	return m_Position;
}

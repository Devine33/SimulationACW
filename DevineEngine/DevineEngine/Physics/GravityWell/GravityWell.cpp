#include "GravityWell.h"
#include <iostream>

int GravityWell::id = 0;
GravityWell::GravityWell(ID3D11DeviceContext* context, float diameter): Sphere(context, diameter), m_Position(0, 0, 0), m_Radius(0)
{
	m_Well_ID = id;
	m_shape = DirectX::GeometricPrimitive::CreateSphere(context, diameter);
	m_Cshape = DirectX::GeometricPrimitive::CreateSphere(context, diameter / 5);
	m_Diameter = diameter;
	m_Force = Vector3(0, 0, 0);
	id++;
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

Vector3 GravityWell::ApplyAttractor(Vector3 DT)
{
	//std::cout << m_Force.x << "/n";
	return m_Force += DT;
}

Vector3 GravityWell::ApplyRepellor(Vector3 DT)
{
	return m_Force -= DT;
}

Vector3 GravityWell::CancelForces()
{
	return m_Force = Vector3(0, 0, 0);
}

int GravityWell::GetID() const
{
	return m_Well_ID;
}

void GravityWell::Move(Vector3 Pos)
{
	m_Position += Pos;
}

void GravityWell::SpheresInWell(Sphere* S)
{
	
	Vector3 Well = this->GetPos();
	Vector3 _Sphere = S->GetNewPos();
	auto Distance = Vector3::Distance(Well, _Sphere) - (this->GetRadius() + S->GetRadius() / 2);
	
	if(Distance < 0.0f)
	{
		S->SetVisibility();
		
		auto Direction = _Sphere - Well;
		m_Force = Direction;
		Direction.Normalize();

		S->SetNewVel(m_Force * Direction);
		std::cout << "In Sphere \n";
		/*std::cout << m_Force.x << "/n";*/
		/*m_Force +=  S->GetVel();
		S->SetNewVel(GetGravityWellForce()); 
		m_Force = Vector3(0, 0, 0);*/
	}
	else
	{
		//S->SetInvisible();
	}
}

float GravityWell::GetRadius() const
{
	return m_Diameter / 2;
}

Vector3 GravityWell::GetGravityWellForce() const
{
	return m_Force;
}

std::shared_ptr<DirectX::GeometricPrimitive> GravityWell::GetCPrim()
{
	return m_Cshape;
}

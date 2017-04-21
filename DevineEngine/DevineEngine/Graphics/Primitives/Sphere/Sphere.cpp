#include "Sphere.h"
#include "../../../ContactManifold.h"
//add in set 
int Sphere::countID = 0;
Sphere::Sphere(ID3D11DeviceContext* context,float radius): SphereNumber(0), m_Mass(0), m_Radius(radius)
{
	m_shape = DirectX::GeometricPrimitive::CreateSphere(context, radius);
	++countID;
}

Sphere::~Sphere()
{
	/*m_shape.reset();*/
}

void Sphere::CalculatePhysics(float dt)
{
	Vector3 Force(0.0f, -9.81f * m_Mass,0);
	Vector3 Acceleration = Force / m_Mass;
	
	m_NewVelocity = m_Velocity + (Acceleration * dt);
	m_NewPosition = m_Position + (m_Velocity * dt);

	if (m_NewPosition.y < -20.0f + m_Radius)
	{
		/*m_NewPosition.Set(m_newPos.GetX(), -20.0f + m_radius);
		m_newVelocity.Set(m_newVelocity.GetX(), 0.0f);*/
	}
}

void Sphere::CollisionWithSphere(Sphere* Sphere2, ContactManifold* contactManifold)
{
	Vector3 POS1 = this->GetNewPos();
	Vector3 POS2 = Sphere2->GetNewPos();
	float dist = Vector3::Distance(POS1,POS2) -  (this->GetRadius() + Sphere2->GetRadius());

	if (dist < 0.0f)
	{
		ManifoldPoint mp;
		mp.contactID1 = this;   
		mp.contactID2 = Sphere2;   
		Vector3 SUB = POS2 - POS1;
		SUB.Normalize();
		mp.contactNormal = SUB;
;		contactManifold->Add(mp);
	}
}

void Sphere::Update()
{
	m_Velocity = m_NewVelocity;
	m_Position = m_NewPosition;
}

void Sphere::CollisionResponseWithSphere(ManifoldPoint& point)
{
	Vector3 colNormal = point.contactNormal;

	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(-1.0f*colNormal*colNormal.Dot(point.contactID1->GetVel()));

	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(-1.0f*colNormal*colNormal.Dot(point.contactID2->GetVel()));
}

std::shared_ptr<DirectX::GeometricPrimitive> Sphere::GetPrim()
{
	return m_shape;
}

int Sphere::GetSphereCount() const
{
	return SphereNumber;
}

void Sphere::SetPos(Vector3 posin)
{
	m_Position = posin;
}

void Sphere::SetVelocity(Vector3 velin)
{
	m_Velocity = velin;
}

Vector3 Sphere::GetPos() const
{
	return m_Position;
}

Vector3 Sphere::GetVel()
{
	return m_Velocity;
}

void Sphere::SetNewPos(Vector3 pos)
{
	m_NewPosition = pos;
}

void Sphere::SetNewVel(Vector3 vel)
{
	m_NewVelocity = vel;
}

Vector3 Sphere::GetNewPos()
{
	return m_NewPosition;
}

Vector3 Sphere::GetNewVel()
{
	return  m_NewVelocity;
}

void Sphere::SetMass(float mass)
{
	m_Mass = mass;
}

float Sphere::GetMass()
{
	return m_Mass;
}

void Sphere::ResetPos()
{
	m_NewPosition = m_Position;
}

float Sphere::GetRadius()
{
	return m_Radius;
}

void Sphere::Read(std::istream& in)
{
	char buffer[256], ch;

	in.get(buffer, sizeof(buffer), '=');
	in >> ch >> SphereNumber;
}

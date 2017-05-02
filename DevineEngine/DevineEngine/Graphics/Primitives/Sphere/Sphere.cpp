#include "Sphere.h"
#include <iostream>
int Sphere::countID = 0;
Sphere::Sphere(ID3D11DeviceContext* context,float radius): SphereNumber(0), m_Mass(0), m_Radius(radius), Elasticity(0)
{
	m_objectID = countID;
	++countID;
	m_shape = DirectX::GeometricPrimitive::CreateSphere(context, radius);
}

Sphere::~Sphere()
{
}

void Sphere::CalculatePhysics(float dt)
{
	Vector3 Force(0.0f, -9.81f * m_Mass,0);
	Vector3 Acceleration = Force / m_Mass;	
	m_NewVelocity = m_Velocity + (Acceleration * dt);
	m_NewPosition = m_Position + (m_Velocity + m_NewVelocity /= 2) * dt;
}

void Sphere::CollisionWithSphere(Sphere* Sphere2, ContactManifold* contactManifold)
{
	Vector3 POS1 = this->GetNewPos();
	Vector3 POS2 = Sphere2->GetNewPos();
	float Distance = Vector3::Distance(POS1,POS2) -  ( this->GetRadius() / 2 + Sphere2->GetRadius() / 2);
	if (Distance <= 0.0f)
	{
		ManifoldPoint mp;
		mp.contactID1 = this;   
		mp.contactID2 = Sphere2;   
		Vector3 SUB = POS2 - POS1;
		SUB.Normalize();
		mp.contactNormal = SUB;
		contactManifold->Add(mp);
	}		
}

void Sphere::Update()
{
	m_Velocity = m_NewVelocity;
	m_Position = m_NewPosition;
}

Vector3 Sphere::CalculateAcceleration(const State& state)
{
	Vector3 Force(0.0f, -9.81f * m_Mass, 0);
	Vector3 Acceleration = Force / m_Mass;
	return Acceleration;
}

Derivative Sphere::Evaluate(const State &initial,float dt, const Derivative& d)
{
	State state;
	state.x = initial.x + d.dx*dt;
	state.v = initial.v + d.dv*dt;

	Derivative output;
	output.dx = state.v;
	output.dv = CalculateAcceleration(state);
	return output;
}

void Sphere::Integrate(State &state, float dt)
{
	Derivative a, b, c, d;

	a = Evaluate(state,0.0f, Derivative());
	b = Evaluate(state,dt*=0.5f, a);
	c = Evaluate(state,dt*=0.5f, b);
	d = Evaluate(state,dt, c);

	//velocity
	Vector3 dxdt = 1.0f / 6.0f *
		(a.dx + 2.0*(b.dx + c.dx) + d.dx);
	//acceleration
	Vector3 dvdt = 1.0f / 6.0f *
		(a.dv + 2.0f*(b.dv + c.dv) + d.dv);

	m_NewPosition = m_Position + dxdt * dt;
	m_NewVelocity = m_Velocity + dvdt * dt;
}

void Sphere::CollisionResponseWithSphere(ManifoldPoint& point)
{
	Vector3 colNormal = point.contactNormal;
	Vector3 V1 = point.contactID1->GetNewVel();
	Vector3 V2 = point.contactID2->GetNewVel();
	float m1 = point.contactID1->GetMass();
	float m2 = point.contactID2->GetMass();

	Vector3 t = (m1 - GetElasticity() * m2) * (V1.Dot(colNormal) * colNormal + (m2 + GetElasticity() * m2) * V2.Dot(colNormal) * colNormal) / (m1 + m2);
	Vector3 t2 = ((m1 + GetElasticity() * m1) * (V1.Dot(colNormal) * colNormal + (m2 - GetElasticity() * m1) * (V2.Dot(colNormal)) * colNormal)) / (m1 + m2);

	Vector3 e1 = V1 - (V1.Dot(colNormal)) * colNormal + t;
	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(e1 * 0.4);
	
	Vector3 e2 = V1 - (V2.Dot(colNormal)) * colNormal + t2;
	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(e2 * 0.4);
}

//NEED COLLISION RESPONSE FOR FLOOR
void Sphere::CollisionWithGround(Cylinder* Cylinder, ContactManifold* contactManifold)
{
	//Coefficient of restitution
	Vector3 POS1 = this->GetNewPos();
	int GROUND = Cylinder->GetFloorHeight() + this->GetRadius();
	Vector3 POS2(0, -GROUND, 0);
	POS2.Normalize();
	auto ColNormal = POS1 - POS2;
	ColNormal.Normalize();
	if (POS1.y < -GROUND + this->GetRadius())
	{
		//elasticity is 0
		//must be 0.85 or something else;
		auto t = GetElasticity();
		m_NewPosition = Vector3(m_NewPosition.x, -GROUND + m_Radius, m_NewPosition.z);
		Vector3 PN = Vector3(0, 1, 0);
		//elasticity isnt 
		Vector3 NV = m_Velocity - (1.0f + 0.85) * m_Velocity.Dot(PN) * PN;
		this->SetNewVel(NV);
	}
}

void Sphere::CollisionWithWalls(Cylinder* Cylinder, ContactManifold* contactManifold)
{
	float SpositionX = this->GetPos().x;
	float SpositionZ = this->GetPos().z;
	Vector2 SP = Vector2(SpositionX, SpositionZ);
	float CpositionX = Cylinder->GetPosition().x;
	float CpositionZ = Cylinder->GetPosition().z;
	Vector2 CP = Vector2(CpositionX, CpositionZ);
	Vector2 CN = CP - SP;
	//distance
	auto D = Vector2::Distance(SP, CP);
	float wall = Cylinder->GetRadius();
	if(wall < D)
	{
		/*Vector3 Reflected = 2 * CN *= (CN * this->GetNewVel());
		this->SetNewVel(Reflected *= Elasticity);*/
	}
}

void Sphere::ArrangeGrid(std::vector<Sphere*> S, int num)
{
	Vector3 Standard{ -4.0f,0.0f,0.0f };
	//Vector3 PS{-4.0,0.0,0.0 };
	//float spacevalue = 1.5;
	for (auto element : S)
	{		
		if (element->ReturnObjectID() % 8 == 0 && element->ReturnObjectID() != 0 )
		{
			Standard.z += 1.5;
			Standard.x -= 10.5;
			element->SetPos(Standard);
			element->SetMass(5.0);
		}
		else if(element->ReturnObjectID() % 3 == 0)
		{
			Standard.x += 1.5;
			element->SetPos(Standard);
			element->SetMass(9.0);
		}
		else
		{
			Standard.x += 1.5;
			element->SetPos(Standard);
			element->SetMass(1.0);
		}
	}
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

void Sphere::SetElasticity(float e)
{
	Elasticity = e;
}

float Sphere::GetElasticity()
{
	return Elasticity;
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

Vector3 Sphere::GetNewPos() const
{
	return m_NewPosition;
}

Vector3 Sphere::GetNewVel() const
{
	return m_NewVelocity;
}

int Sphere::ReturnObjectID()
{
	return m_objectID;
}

void Sphere::SetMass(float mass)
{
	m_Mass = mass;
}

float Sphere::GetMass() const
{
	return m_Mass;
}

void Sphere::ResetPos()
{
	m_NewPosition = m_Position;
}

float Sphere::GetRadius() const
{
	return m_Radius;
}

void Sphere::Read(std::istream& in)
{
	char buffer[256], ch;
	in.get(buffer, sizeof(buffer), '=');
	in >> ch >> SphereNumber;
}

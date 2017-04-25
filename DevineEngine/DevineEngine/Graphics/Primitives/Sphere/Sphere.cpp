#include "Sphere.h"
#include <iostream>
//add in set 
int Sphere::countID = 0;
Sphere::Sphere(ID3D11DeviceContext* context,float radius): SphereNumber(0), m_Mass(0), m_Radius(radius)
{
	m_objectID = countID;
	++countID;
	m_shape = DirectX::GeometricPrimitive::CreateSphere(context, radius);
	
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
	m_NewPosition = m_Position + (m_Velocity + m_NewVelocity /= 2) * dt;
}


//Vector3 Sphere::CalcAcceleration(float dt, Vector3 MVEL, Vector3 MPOS)
//{
//	const float k = 10;
//	const float b = 1;
//	return -k * MPOS - b * MVEL ;
//}
//
////might need to break up these functions
//Vector3 Sphere::CalcPosition(float dt, Vector3 MPOS)
//{
//	const float k = 10;
//	return -k * MPOS;
//}
//
//void Sphere::Integrate(float dt)
//{
//	m_Velocity = m_Velocity + m_NewVelocity * dt;
//	m_Position = m_Position = GetPos() + m_NewPosition * dt;
//
//	m_NewVelocity = m_Velocity;
//	Acceleration = CalcAcceleration(dt, m_Velocity, m_Position);
//}
//
//void Sphere::Evaluate(float dt)
//{
//
//	auto MVEL = EvaluateVelocity(dt);
//	auto MPOS = EvaluatePosition(dt);
//
//	m_NewVelocity = MVEL;
//	Acceleration = CalcAcceleration(dt, MVEL, MPOS);
//
//}
////state.v = initial.v + d.dv*dt;
//Vector3 Sphere::EvaluateVelocity(float dt)
//{
//	auto MVEL = m_Velocity = GetVel() + m_NewVelocity * dt;
//	return m_NewVelocity = MVEL;
//}
//
////state.x = initial.x + d.dx*dt;
//Vector3 Sphere::EvaluatePosition(float dt)
//{
//	auto MPOS = m_Position + m_NewPosition * dt;
//	return m_NewPosition = MPOS;
//}
//
//Vector3 Sphere::EvaluateSecond(float dt)
//{
//}
//
//void Sphere::RK4(float dt)
//{
//	// F= MA
//	//Vector3 Force(0.0f, -9.81f * m_Mass, 0);
//	//
//
//	//m_NewVelocity = m_Position / dt;
//	////A = F/M : RATE OF CHANGE IN VELOCITY
//	//Vector3 Acceleration = Force / m_Mass;
//	//auto a;
//	//auto b;
//	//auto c;
//	//auto d;
//	//DX/DT = V: VELOCITY IS THE RATE OF CHANGE OF POSITION OVER TIME
//
//	//VELOCITY = POSITION / DT
//	//m_Position
//	//m_Velocity
//
//	//m_NewVelocity
//	//Vector3 Acceleration
//
//	//WORK OUT ACCELERATION BEING APPLIED
//	//A = F / M
//	//V = A * T
//	//P = V *T
//}
//
//

void Sphere::CollisionWithSphere(Sphere* Sphere2, ContactManifold* contactManifold)
{
	Vector3 POS1 = this->GetNewPos();
	Vector3 POS2 = Sphere2->GetNewPos();
	float Distance = Vector3::Distance(POS1,POS2) -  ( this->GetRadius() / 2 + Sphere2->GetRadius() / 2);

	if (Distance < 0.0f)
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

void Sphere::CollisionResponseWithSphere(ManifoldPoint& point)
{
	Vector3 colNormal = point.contactNormal;

	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(-1.0f*colNormal*colNormal.Dot(point.contactID1->GetVel()));

	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(-1.0f*colNormal*colNormal.Dot(point.contactID2->GetVel()));
}
//NEED COLLISIONR ESPONSE FOR FLOOR
void Sphere::CollisionWithGround(Cylinder* Cylinder, ContactManifold* contactManifold)
{
	Vector3 POS1 = this->GetNewPos();
	int GROUND = Cylinder->GetFloorHeight();
	if (POS1.y < -GROUND + this->GetRadius())
	{
		Vector3 NP{ m_NewPosition.x, static_cast<float>(-Cylinder->GetFloorHeight() + this->GetRadius()), m_NewPosition.z };
		Vector3 NV{ 0,0.0f,0 };
		this->SetNewPos(NP);
		this->SetNewVel(NV);

		//A COLLISION HAS OCCURRED ADD A MANIFOLD POINT
		ManifoldPoint mp;
		mp.contactID1 = this;
	/*	mp.contactID2 = Cylinder;*/
	}
}

void Sphere::CollisionWithWalls(Cylinder* Cylinder, ContactManifold* contactManifold)
{
	Vector3 POS1 = this->GetNewPos();
	float wall = Cylinder->GetRadius();
	Vector3 CPOS = Cylinder->GetPosition();

	if(wall < Vector3::Distance(POS1,CPOS))
	{
		Vector3 NP{ 0.0f,0.0f,0.0f };
		Vector3 NV{ 0, 0.0f, 0 };
		this->SetNewPos(NP);
		this->SetNewVel(NV);
	}
}

void Sphere::ArrangeGrid(std::vector<Sphere*> S, int num)
{
	Vector3 Standard{ -4.0f,0.0f,0.0f };
	Vector3 PS{-4.0,0.0,0.0 };
	float spacevalue = 1.5;
	for (auto element : S)
	{
		
		if (element->ReturnObjectID() % 8 == 0 && element->ReturnObjectID() != 0 )
		{
			Standard.z += 1.5;
			Standard.x -= 10.5;
			element->SetPos(Standard);
			
		}
		else
			Standard.x += 1.5;
			element->SetPos(Standard);
			
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

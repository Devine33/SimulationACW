#include "Sphere.h"
#include <iostream>
int Sphere::countID = 0;
Sphere::Sphere(ID3D11DeviceContext* context,float radius): SphereNumber(0), m_Mass(0), m_Radius(radius)
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
	/*float Friction;
	float Elasticity;*/
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
		this->GetVel() * 0.888;
		Sphere2->GetVel() * 0.888;
		SUB.Normalize();
		mp.contactNormal = SUB;
		contactManifold->Add(mp);
	}
	/*if (Distance <= -1.0f)
	{
		std::cout << "STUCK";
	}*/
	
		
}

void Sphere::Update()
{
	m_Velocity = m_NewVelocity;
	m_Position = m_NewPosition;
}

Vector3 Sphere::CalculateAcceleration(const State& state, float t)
{
	Vector3 Force(0.0f, -9.81f * m_Mass, 0);
	Vector3 Acceleration = Force / m_Mass;
	return Acceleration;
}

Derivative Sphere::Evaluate(float t, float dt, const Derivative& d)
{
	State state;
	state.x = m_Position + d.dx*dt;
	state.v = m_Velocity + d.dv*dt;

	Derivative output;
	output.dx = m_Velocity;
	output.dv = CalculateAcceleration(state, t + dt);
	return output;
}

void Sphere::Integrate(float dt)
{
	Derivative a, b, c, d;
	float t = dt;
	a = Evaluate(t, 0.0f, Derivative());
	b = Evaluate(t, dt*=0.5f, a);
	c = Evaluate(t, dt*=0.5f, b);
	d = Evaluate(t, dt, c);

	//velocity
	Vector3 dxdt = 1.0f / 6.0f *
		(a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	//acceleration
	Vector3 dvdt = 1.0f / 6.0f *
		(a.dv + 2.0f*(b.dv + c.dv) + d.dv);

	m_NewPosition = m_Position + dxdt * dt;
	m_NewVelocity = m_Velocity + dvdt * dt;

	/*state.x = state.x + dxdt * dt;
	state.v = state.v + dvdt * dt;*/
}

void Sphere::CollisionResponseWithSphere(ManifoldPoint& point)
{


	Vector3 colNormal = point.contactNormal;
	Vector3 V1 = point.contactID1->GetVel();

	float X1 = colNormal.Dot(V1);

	

	float m1 = point.contactID1->GetMass();

	//colNormal = colNormal * -1;

	Vector3 V2 = point.contactID2->GetVel();

	float X2 = colNormal.Dot(V2);

	Vector3 V2X = colNormal * X2;

	Vector3 V2Y = V2 - V2X;

	float m2 = point.contactID2->GetMass();

	
	Vector3 E1 = -(point.contactID1->GetVel() - point.contactID2->GetVel());
	Vector3 E2 = (-point.contactID1->GetVel() - -point.contactID2->GetVel());
	Vector3 E = E1 / E2;

	Vector3 V1X = colNormal * X1;

	Vector3 V1Y = V1 - V1X;
	Vector3 P1 = V1X * (m1 - m2) / (m1+ m2);

	Vector3 P2 = V2X*(2 * m2) / (m1 + m2);
	Vector3 P3 = P2 += V1Y;

	/*P3.Normalize();*/
	Vector3 B1 = V1X * (2 * m1) / (m1 + m2);
	Vector3 B2 = V2X*(m2 - m1) / (m1 + m2);
	Vector3 B3 = B2 += V2Y;

	Vector3 t = (m1 - Elasticity * m2) * (V1.Dot(colNormal) * colNormal + (m2 + Elasticity * m2) * V2.Dot(colNormal) * colNormal) / (m1 + m2);

	Vector3 t2 = (m1 - Elasticity * m1) * (V1.Dot(colNormal) * colNormal + (m2 - Elasticity * m2) * V2.Dot(colNormal) * colNormal) / (m1 + m2);
	t * cos(45);
	/*B3.Normalize();*/
	/*point.contactID1->ResetPos();
	point.contactID1->SetNewVel(P3);
	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(B3);*/
	//may need to apply friction here
	point.contactID1->ResetPos();
	point.contactID1->SetNewVel(t);
	point.contactID2->ResetPos();
	point.contactID2->SetNewVel(t2);

	

}

//NEED COLLISION RESPONSE FOR FLOOR
void Sphere::CollisionWithGround(Cylinder* Cylinder, ContactManifold* contactManifold)
{
	//Coefficient of restitution
	 Elasticity = 0.4;
	Vector3 POS1 = this->GetNewPos();
	int GROUND = Cylinder->GetFloorHeight() + this->GetRadius();
	Vector3 POS2(0, -GROUND, 0);
	POS2.Normalize();
	auto ColNormal = POS1 - POS2;
	ColNormal.Normalize();
	if (POS1.y < -GROUND + this->GetRadius())
	{
		//angular
		/*float vDotN = ColNormal.Dot(ColNormal);
		Vector3 FT = -(this->GetVel() + (vDotN * ColNormal));
		FT *=  3.0+ 0.3;
		FT *= this->GetMass();
		Vector3 Centre = this->GetPos() - POS2;
		auto Torgue = Centre.Cross(FT);
		Torgue.Normalize();*/

		//av
		auto AV = this->GetPos().y - this->GetPos().x;
		//av * world xy
		auto AA = AV * 2;
		//linear 
		Vector3 SUB = POS2 - POS1;
		Vector3 V(1, 1, 1);
		SUB.Normalize();
		this->ResetPos();

		Vector3 Reflected = 2 * ColNormal * (ColNormal * this->GetVel());
		this->SetNewVel(-Reflected *= Elasticity);
	}
}

void Sphere::CollisionWithWalls(Cylinder* Cylinder, ContactManifold* contactManifold)
{
	Vector3 POS1 = this->GetNewPos();
	float wall = Cylinder->GetRadius();
	Vector3 CPOS = Cylinder->GetPosition();

	if(wall < Vector3::Distance(POS1,CPOS))
	{

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

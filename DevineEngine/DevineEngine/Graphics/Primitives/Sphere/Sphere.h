#pragma once
#include <d3d11.h>
#include <memory>
#include "../Arena/Cylinder.h"
#include "../../../Physics/ContactManifold.h"
#include "../../../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"
#include "SimpleMath.h"
#include <istream>
#include <list>

struct Derivative;
struct ManifoldPoint;
class ContactManifold;
using namespace DirectX::SimpleMath;
struct State
{
	Vector3 x;      // position
	Vector3 v;      // velocity
};

struct Derivative
{
	Vector3 dx;      // dx/dt = velocity
	Vector3 dv;      // dv/dt = acceleration
};
class Sphere
{
public:
	Sphere(ID3D11DeviceContext* context,float radius );
	~Sphere();
	void CalculatePhysics(float dt);
	void CollisionWithSphere(Sphere* Sphere, ContactManifold *contactManifold);
	void Update();
	Vector3 CalculateAcceleration(const State &state);
	Derivative Evaluate(const State &initial,float dt,const Derivative &d);
	void Integrate(State &state, float dt);
	void CollisionResponseWithSphere(ManifoldPoint &point);
	void CollisionWithGround(Cylinder* Cylinder, ContactManifold *contactManifold);
	void CollisionWithWalls(Cylinder* Cylinder, ContactManifold *contactManifold);
	void ArrangeGrid(std::vector<Sphere*>,int num);
	void SetPos(Vector3 posin);
	void SetVelocity(Vector3 velin);
	void SetElasticity(float e);
	float GetElasticity();
	void SetMass(float mass);
	void SetNewPos(Vector3 pos);
	void SetNewVel(Vector3 vel);
	Vector3 GetPos() const;
	Vector3 GetVel();	
	Vector3 GetNewPos() const;
	Vector3 GetNewVel() const;
	int ReturnObjectID();
	float GetRadius() const;
	float GetMass() const;
	std::shared_ptr<DirectX::GeometricPrimitive> GetPrim();
	int GetSphereCount() const;
	void ResetPos();	

	friend std::istream &operator >> (std::istream &in, Sphere &o) { o.Read(in); return in; }
private:
	std::shared_ptr<DirectX::GeometricPrimitive> m_shape;
	Vector4 color;
	int SphereNumber;
	Vector3 m_Position;
	Vector3 m_NewPosition;
	Vector3 m_NewVelocity;
	Vector3 m_Velocity;
	float m_Mass;
	float m_Radius;
	static int countID;
	int m_objectID;
	void Read(std::istream &in);
	Vector3 Acceleration;
	float Elasticity;


	
};




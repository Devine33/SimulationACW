#pragma once
#include <d3d11.h>
#include <memory>
#include "../../../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"
#include "SimpleMath.h"
#include <istream>
#include <list>

struct ManifoldPoint;
class ContactManifold;
using namespace DirectX::SimpleMath;

class Sphere
{
public:
	Sphere(ID3D11DeviceContext* context,float radius );
	~Sphere();
	//move all darrens methods in here but 3d

	void CalculatePhysics(float dt);
	void CollisionWithSphere(Sphere* Sphere, ContactManifold *contactManifold);
	void Update();
	void CollisionResponseWithSphere(ManifoldPoint &point);
	std::shared_ptr<DirectX::GeometricPrimitive> GetPrim();
	int GetSphereCount() const;
	void SetPos(Vector3 posin);
	void SetVelocity(Vector3 velin);
	friend std::istream &operator >> (std::istream &in, Sphere &o) { o.Read(in); return in; }
	Vector3 GetPos() const;
	Vector3 GetVel();
	void SetNewPos(Vector3 pos);
	void SetNewVel(Vector3 vel);
	Vector3 GetNewPos();
	Vector3 GetNewVel();
	void SetMass(float mass);
	float GetMass();
	void ResetPos();
	float GetRadius();
private:
	std::shared_ptr<DirectX::GeometricPrimitive> m_shape;
	void Read(std::istream &in);
	Vector4 color;
	int SphereNumber;
	Vector3 m_Position;
	Vector3 m_NewPosition;
	Vector3 m_NewVelocity;
	Vector3 m_Velocity;
	float m_Mass;
	float m_Radius;
	static int countID;
};




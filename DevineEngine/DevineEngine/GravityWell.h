#pragma once
#include "Graphics/Primitives/Sphere/Sphere.h"

class GravityWell : public Sphere
{
public:
	GravityWell(ID3D11DeviceContext* context, float diameter);
	~GravityWell();
	Vector3 GetPos() const;
	void SetPos(Vector3 posin);
	//LeftButton
	Vector3 ApplyAttractor();
	//RightButton
	Vector3 ApplyRepellor();
	//both at the same time
	Vector3 CancelForces();
	void Move(Vector3);
	void SpheresInWell(Sphere* S);
	float GetRadius() const;
	Vector3 GetGravityWellForce() const;
private:
	std::shared_ptr<DirectX::GeometricPrimitive> m_shape;
	float m_Diameter;
	Vector3 m_Position;
	float m_Radius;
	Vector3 m_Force;
};

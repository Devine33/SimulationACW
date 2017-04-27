#pragma once
#include "Graphics/Primitives/Sphere/Sphere.h"

class GravityWell : public Sphere
{
public:
	GravityWell(ID3D11DeviceContext* context, float radius);
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
	//std::shared_ptr<DirectX::GeometricPrimitive> GetPrim();
private:
	std::shared_ptr<DirectX::GeometricPrimitive> m_shape;
	Vector3 m_Position;
};


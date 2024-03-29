#pragma once
#include "../../Graphics/Primitives/Sphere/Sphere.h"

class GravityWell : public Sphere
{
public:
	GravityWell(ID3D11DeviceContext* context, float diameter);
	~GravityWell();
	Vector3 GetPos() const;
	void SetPos(Vector3 posin);
	//LeftButton
	Vector3 ApplyAttractor(Vector3 DT);
	//RightButton
	Vector3 ApplyRepellor(Vector3 DT);
	//both at the same time
	Vector3 CancelForces();
	int GetID() const;
	int GetBallsOwned();
	void Move(Vector3);
	void MoveUp();
	void MoveDown();
	void SpheresInWell(Sphere* S);
	float GetRadius() const;
	void SetTotalBalls(int balls);
	Vector3 GetGravityWellForce() const;
	std::shared_ptr<DirectX::GeometricPrimitive> GetCPrim();
	std::shared_ptr<DirectX::GeometricPrimitive> GetRegion();
private:
	std::shared_ptr<DirectX::GeometricPrimitive> m_shape;
	std::shared_ptr<DirectX::GeometricPrimitive> m_Cshape;
	std::shared_ptr<DirectX::GeometricPrimitive> m_Region;
	int m_ballsOwned;
	float m_Diameter;
	Vector3 m_Position;
	float m_Radius;
	Vector3 m_Force;
	int m_Well_ID;
	static int TotalBalls;
	static int id;
};

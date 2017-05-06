#pragma once
#include <d3d11.h>
#include <memory>
#include "../../../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"
#include <SimpleMath.h>


class Cylinder
{
public:
	Cylinder(ID3D11DeviceContext* context,float height, float diameter);
	~Cylinder();
	void SetPosition(DirectX::SimpleMath::Vector3 POSIN);
	void SetHeight(int Height);
	std::shared_ptr<DirectX::GeometricPrimitive> GetPrim() const;
	int GetFloorHeight();
	float GetRadius();
	DirectX::SimpleMath::Vector3 GetPosition() const;
private:
	DirectX::SimpleMath::Vector3 m_Position;
	int m_Height;
	int m_FloorHeight;;
	std::shared_ptr<DirectX::GeometricPrimitive> m_shape;
	float m_Radius;
	DirectX::XMFLOAT4 color;
};


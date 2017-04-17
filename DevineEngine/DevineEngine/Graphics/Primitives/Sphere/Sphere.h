#pragma once
#include <d3d11.h>
#include <memory>
#include "../../../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"
#include <istream>
namespace DirectX {
	struct XMMATRIX;
}

class Sphere
{
public:
	Sphere();
	~Sphere();
	void Initialize(ID3D11DeviceContext* context);
	void Draw(DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX) const;

	friend std::istream &operator >> (std::istream &in, Sphere &o) { o.Read(in); return in; }
private:
	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
	void Read(std::istream &in);
	DirectX::XMFLOAT4 color;
};




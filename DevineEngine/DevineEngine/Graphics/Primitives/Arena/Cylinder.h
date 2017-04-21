#pragma once
#include <d3d11.h>
#include <istream>
#include <memory>
#include "../../../../packages/directxtk_desktop_2015.2017.2.10.1/build/native/include/GeometricPrimitive.h"

namespace DirectX {
	struct XMMATRIX;
}

class Cylinder
{
public:
	Cylinder();
	~Cylinder();

	void Initialize(ID3D11DeviceContext* context);
	void Draw(DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX) const;

	friend std::istream &operator >> (std::istream &in, Cylinder &o) { o.Read(in); return in; }
private:
	std::unique_ptr<DirectX::GeometricPrimitive> m_shape;
	void Read(std::istream &in);
	DirectX::XMFLOAT4 color;
};


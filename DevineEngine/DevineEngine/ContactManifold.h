#pragma once
#include <directxmath.h>
#include <d3d11.h>
#include "SimpleMath.h"
class Sphere;

struct ManifoldPoint {
	Sphere *contactID1;
	Sphere *contactID2;
	DirectX::SimpleMath::Vector3 contactNormal;
};

class ContactManifold
{
public:
	ContactManifold();
	~ContactManifold();
	void Add(ManifoldPoint point);
	void Clear();
	int GetNumPoints() const;
	ManifoldPoint& GetPoint(int index);

private:
	ManifoldPoint m_points[1000];
	int m_numOfPoints;
};


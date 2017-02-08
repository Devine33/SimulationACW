#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <SimpleMath.h>
class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(const float x, const  float y, const  float z);
	void SetRotation(const float x, const  float y, const  float z);
	void CameraSetup(const float screenheight, const float screenwidth);
	void Render();
	void GetViewMatrix(DirectX::SimpleMath::Matrix&) const;
	void GetProjectionMatrix(DirectX::SimpleMath::Matrix&) const;
	void GetWorldMatrix(DirectX::SimpleMath::Matrix&) const;
	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetRotation() const;

private:
	float screenaspect;
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	DirectX::SimpleMath::Matrix m_projectionMatrix;
	DirectX::SimpleMath::Matrix m_worldMatrix;
	DirectX::SimpleMath::Matrix m_orthoMatrix;
	DirectX::SimpleMath::Matrix m_viewMatrix;
	float screenHeight;
	float ScreenWidth;
	DirectX::SimpleMath::Vector3 up, position, lookAt;
	float yaw, pitch, roll;
	DirectX::SimpleMath::Matrix rotationMatrix;

	float radians;
};


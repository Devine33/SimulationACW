#pragma once
#include <DirectXCollision.h>
namespace DirectX {
	struct XMMATRIX;
}

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(const float x, const  float y, const  float z);
	void SetRotation(const float x, const  float y, const  float z);
	void CameraSetup(const float screenheight, const float screenwidth);
	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&) const;
	void GetProjectionMatrix(DirectX::XMMATRIX&) const;
	void GetWorldMatrix(DirectX::XMMATRIX&) const;
	DirectX::XMVECTOR GetPosition() const;
	DirectX::XMVECTOR GetRotation() const;
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void MoveForward();
	void MoveBack();
private:
	float screenaspect;
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_orthoMatrix;
	DirectX::XMMATRIX m_viewMatrix;
	float screenHeight;
	float ScreenWidth;
	DirectX::XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;
	float fieldOfView;
	float radians;
};


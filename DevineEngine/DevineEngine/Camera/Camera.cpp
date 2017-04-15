#include "Camera.h"

Camera::Camera(): screenaspect(0), m_positionX(0), m_positionY(0), m_positionZ(0), m_rotationX(0), m_rotationY(0), m_rotationZ(0), screenHeight(0), ScreenWidth(0), yaw(0), pitch(0), roll(0), radians(0)
{
}


Camera::~Camera()
{
}

void Camera::SetPosition(const float x, const float y, const float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void Camera::SetRotation(const float x, const float y, const float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

void Camera::CameraSetup(const float screenheight, const float screenwidth)
{
	screenaspect = screenwidth / screenheight;
	screenHeight = screenheight;
	ScreenWidth = screenwidth;
}

void Camera::Render()
{
	DirectX::XMFLOAT3 UP = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	up = DirectX::XMLoadFloat3(&UP);


	DirectX::XMFLOAT3 POSITION = DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
	// Setup the position of the camera in the world.
	position = DirectX::XMLoadFloat3(&POSITION);

	radians = m_rotationY * 0.0174532925f;


	DirectX::XMFLOAT3 LOOKAT = DirectX::XMFLOAT3(sinf(radians) + m_positionX, m_positionY, 100.0f);
	// Setup where the camera is looking by default.
	lookAt = DirectX::XMLoadFloat3(&LOOKAT);

					  //1.0f

					  //////// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	////// Create the rotation matrix from the yaw, pitch, and roll values.
	//
	
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	//// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	float fieldOfView;
	// Translate the rotated camera position to the location of the viewer.

	fieldOfView = DirectX::XM_PI / 4.0f;
	/*screenaspect = height / width;*/
	// Create the projection matrix for 3D rendering.
	m_worldMatrix = DirectX::XMMatrixIdentity();


	m_orthoMatrix = DirectX::XMMatrixOrthographicLH(ScreenWidth, screenHeight, 1.0f, 1000.0f);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenaspect, 1.0f, 1000.0F);
}

void Camera::GetViewMatrix(DirectX::XMMATRIX& viewMatrix) const
{
	viewMatrix = m_viewMatrix;
}

void Camera::GetProjectionMatrix(DirectX::XMMATRIX& projection) const
{
	projection = m_projectionMatrix;
}

void Camera::GetWorldMatrix(DirectX::XMMATRIX& world) const
{
	world = m_worldMatrix;
}

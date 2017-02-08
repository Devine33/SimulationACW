#include "Camera.h"

Camera::Camera()
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
	screenaspect = screenheight / screenwidth;
	screenHeight = screenheight;
	ScreenWidth = screenwidth;
}

void Camera::Render()
{
	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;


	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	radians = m_rotationY * 0.0174532925f;

	// Setup where the camera is looking by default.
	lookAt.x = sinf(radians) + m_positionX;
	lookAt.y = m_positionY;
	lookAt.z = 100.0f;//cosf(radians) + m_positionZ;

					  //1.0f

					  //////// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	////// Create the rotation matrix from the yaw, pitch, and roll values.
	//
	rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(yaw, pitch, roll);
	//// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	float fieldOfView;
	// Translate the rotated camera position to the location of the viewer.

	fieldOfView = static_cast<float>(DirectX::XM_PI) / 4.0f;
	/*screenaspect = height / width;*/
	// Create the projection matrix for 3D rendering.
	m_worldMatrix = DirectX::XMMatrixIdentity();


	m_orthoMatrix = DirectX::XMMatrixOrthographicLH(ScreenWidth, screenHeight, 1.0f, 1000.0f);

	m_viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenaspect, 1.0f, 1000.0F);
}

void Camera::GetViewMatrix(DirectX::SimpleMath::Matrix& viewMatrix) const
{
	viewMatrix = m_viewMatrix;
}

void Camera::GetProjectionMatrix(DirectX::SimpleMath::Matrix& projection) const
{
	projection = m_projectionMatrix;
}

void Camera::GetWorldMatrix(DirectX::SimpleMath::Matrix& world) const
{
	world = m_worldMatrix;
}

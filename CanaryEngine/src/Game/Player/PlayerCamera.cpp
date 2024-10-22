#include "PlayerCamera.h"

PlayerCamera::PlayerCamera(glm::vec3 Position)
{
	FrontVector = glm::vec3(0.0f, 0.0f, -1.0f);
	CameraPosition = Position;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = -90.0f;
	Pitch = 0.0f;
	UpdateCameraVectors();
}

PlayerCamera::PlayerCamera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float InYaw, float InPitch)
{
	FrontVector = glm::vec3(0.0f, 0.0f, -1.0f);
	CameraPosition = glm::vec3(PosX, PosY, PosZ);
	WorldUp = glm::vec3(UpX, UpY, UpZ);
	Yaw = InYaw;
	Pitch = InPitch;
	UpdateCameraVectors();
}

glm::vec3 PlayerCamera::GetPosition()
{
	return glm::vec3();
}

glm::vec3 PlayerCamera::GetFrontVector()
{
	return glm::vec3();
}

glm::vec3 PlayerCamera::GetUpVector()
{
	return glm::vec3();
}

glm::vec3 PlayerCamera::GetRightVector()
{
	return glm::vec3();
}

glm::vec3 PlayerCamera::GetWorldUpVector()
{
	return glm::vec3();
}

glm::mat4 PlayerCamera::GetViewMatrix()
{
	return glm::lookAt(CameraPosition, CameraPosition + FrontVector, UpVector);
}

void PlayerCamera::ProcessKeyboardInput(ECameraMovement Direction, float DeltaTime)
{
	float Velocity = MovementSpeed * DeltaTime;

	if (Direction == Forward)
	{
		CameraPosition += FrontVector * Velocity;
	}
	if (Direction == Backward)
	{
		CameraPosition -= FrontVector * Velocity;
	}
	if (Direction == Left)
	{
		CameraPosition -= RightVector * Velocity;
	}
	if (Direction == Right)
	{
		CameraPosition += RightVector * Velocity;
	}
}

void PlayerCamera::ProcessMouseMovement(float XOffset, float YOffset, GLboolean ConstrainPitch)
{
	XOffset *= MouseSensitivity;
	YOffset *= MouseSensitivity;

	Yaw += XOffset;
	Pitch += YOffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (ConstrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

void PlayerCamera::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	FrontVector = glm::normalize(front);
	// also re-calculate the Right and Up vector
	RightVector = glm::normalize(glm::cross(FrontVector, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	UpVector = glm::normalize(glm::cross(RightVector, FrontVector));
}
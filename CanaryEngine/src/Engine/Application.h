#pragma once

#include "Game/Player/PlayerCamera.h"

struct GLFWwindow;

class Application
{
public:
	void Run();
	void ProcessInput(GLFWwindow* InWindow);

	// Camera
	PlayerCamera Camera = PlayerCamera(glm::vec3(0.0f, 0.0f, 3.0f));
	float LastX = 800 / 2.0f;
	float LastY = 600 / 2.0f;
	bool FirstMouseMove = true;

	// Time
	float DeltaTime = 0.0f;
	float LastFrame = 0.0f;

private:
	bool bWireframeMode = false;
};
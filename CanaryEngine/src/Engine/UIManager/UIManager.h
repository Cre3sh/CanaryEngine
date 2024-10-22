#pragma once

struct GLFWwindow;

class UIManager
{
public:
	// Initialise base window, as well as change ImGui style colors
	void Intialise(GLFWwindow* Window);

	void NewFrame();
	void Render();

	void Shutdown();

	void AddDebugWindow();
};
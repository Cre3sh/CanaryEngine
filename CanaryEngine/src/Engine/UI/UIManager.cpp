#include "UIManager.h"

#include <GLFW/glfw3.h>

#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>

void UIManager::Intialise(GLFWwindow* Window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& IO = ImGui::GetIO(); (void)IO;

    // Styling
    ImGui::StyleColorsDark();
    ImGuiStyle& Style = ImGui::GetStyle();

    Style.FrameRounding = 4.0f;    // Round the frames
    Style.WindowRounding = 6.0f;   // Round the windows
    Style.ScrollbarRounding = 4.0f; // Round the scrollbar
    Style.GrabRounding = 4.0f;     // Round grab handles
    Style.WindowTitleAlign = ImVec2(0.5f, 0.5f); // Center window title

    // Modify padding, borders, etc. for a sleek look
    Style.FramePadding = ImVec2(8, 4);
    Style.ItemSpacing = ImVec2(10, 8);

    // Set colors for a more modern, flat look
    ImVec4* colors = Style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);  // Dark background
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);  // Darker title bar
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);  // Flat blue button
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);  // Lighter blue when hovered
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);  // Darker blue when pressed
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);  // Flat gray background
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);  // Slightly lighter on hover
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

    ImGui_ImplGlfw_InitForOpenGL(Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UIManager::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::Render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::AddDebugWindow()
{
    ImGui::Begin("Debugger");
    if (ImGui::Button("Exit"))
    {
        // Add application exit here
    }

    ImGui::End();
}
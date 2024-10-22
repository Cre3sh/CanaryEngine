#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum ECameraMovement {
    Forward,
    Backward,
    Left,
    Right
};

class PlayerCamera
{

public:
    PlayerCamera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f));

    // constructor with scalar values
    PlayerCamera(float PosX, float PosY, float PosZ, float UpX, float UpY, float UpZ, float InYaw, float InPitch);

    // Returns the position of the camera
    glm::vec3 GetPosition();

    // Returns the camera's front vector
    glm::vec3 GetFrontVector();

    // Returns the camera's up vector
    glm::vec3 GetUpVector();

    // Returns the camera's right vector
    glm::vec3 GetRightVector();

    // Returns the up vector of the world
    glm::vec3 GetWorldUpVector();

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    void ProcessKeyboardInput(ECameraMovement Direction, float DeltaTime);

    void ProcessMouseMovement(float XOffset, float YOffset, GLboolean ConstrainPitch = true);

private:
    void UpdateCameraVectors();

    // camera Attributes
    glm::vec3 CameraPosition;
    glm::vec3 FrontVector;
    glm::vec3 UpVector;
    glm::vec3 RightVector;
    glm::vec3 WorldUp;

    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed = 2.5f;
    float MouseSensitivity = 0.1f;
};
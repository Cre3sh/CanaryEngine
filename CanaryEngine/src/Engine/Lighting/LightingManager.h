#pragma once

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

class ShaderProgram;

struct Light {
    glm::vec3 LightPosition; // 12 bytes
    glm::vec3 LightColor;    // 12 bytes
    float Intensity;         // 4 bytes
    int LightType;           // 4 bytes
    float LightRadius;       // 4 bytes
    glm::vec3 LightDirection; // 12 bytes
    float LightCutOff;        // 4 bytes
};

class LightManager 
{
public:
    // Add a light to the scene
    void AddLight(const Light& light);

    // Updates all existing light objects
    void UpdateLights(ShaderProgram& Shader);

    std::vector<Light> GetActiveLights() const;

private:
    const int MaxLights = 100;
    std::vector<Light> Lights;
};
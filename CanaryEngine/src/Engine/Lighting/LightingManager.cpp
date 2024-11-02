#include "LightingManager.h"

#include "Engine/Shader/ShaderProgram.h"

void LightManager::AddLight(const Light& InLight)
{
    if (Lights.size() < MaxLights) {
        Lights.push_back(InLight);
    }
}

void LightManager::UpdateLights(ShaderProgram& Shader)
{
    // Technically better to use UBOs for this, but I had a lot of trouble getting that setup working 
    // and so we'll have to make do with this less efficient solution for now

    // Set the number of active lights
    GLint numLightsLoc = glGetUniformLocation(Shader.ID, "numLights");
    glUniform1i(numLightsLoc, static_cast<int>(Lights.size()));

    for (size_t i = 0; i < Lights.size(); ++i) {
        std::string base = "lights[" + std::to_string(i) + "]";

        // Get uniform locations for each property of the light
        GLint positionLoc = glGetUniformLocation(Shader.ID, (base + ".LightPosition").c_str());
        GLint colorLoc = glGetUniformLocation(Shader.ID, (base + ".LightColor").c_str());
        GLint intensityLoc = glGetUniformLocation(Shader.ID, (base + ".Intensity").c_str());
        GLint TypeLoc = glGetUniformLocation(Shader.ID, (base + ".LightType").c_str());
        GLint RadLoc = glGetUniformLocation(Shader.ID, (base + ".LightRadius").c_str());

        // Set the light properties
        glUniform3fv(positionLoc, 1, &Lights[i].LightPosition[0]);
        glUniform3fv(colorLoc, 1, &Lights[i].LightColor[0]);
        glUniform1f(intensityLoc, Lights[i].Intensity);
        glUniform1i(TypeLoc, Lights[i].LightType);
        glUniform1f(RadLoc, Lights[i].LightRadius);
    }
}

std::vector<Light> LightManager::GetActiveLights() const
{
    return Lights;
}
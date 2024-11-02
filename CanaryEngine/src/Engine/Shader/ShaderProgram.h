#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

#include <glad/glad.h> // include glad to get all the required OpenGL headers

class ShaderProgram
{
public:
    // The program ID
    unsigned int ID;

    // Constructor reads and builds the shader
    ShaderProgram(const char* VertexPath, const char* FragmentPath);

    // Use/activate the shader
    void Use();

    // Utility uniform functions
    void SetBool(const std::string& Name, bool Value) const;
    void SetInt(const std::string& Name, int Value) const;
    void SetFloat(const std::string& Name, float Value) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec2(const std::string& name, float x, float y) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec3(const std::string& name, float x, float y, float z) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetVec4(const std::string& name, float x, float y, float z, float w) const;
    void SetMat2(const std::string& name, const glm::mat2& mat) const;
    void SetMat3(const std::string& name, const glm::mat3& mat) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
    void CheckCompileErrors(unsigned int Shader, std::string Type);
};

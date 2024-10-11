#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h> // include glad to get all the required OpenGL headers

class ShaderManger
{
public:
    // The program ID
    unsigned int ID;

    // Constructor reads and builds the shader
    ShaderManger(const char* VertexPath, const char* FragmentPath);

    // Use/activate the shader
    void Use();

    // Utility uniform functions
    void SetBool(const std::string& Name, bool Value) const;
    void SetInt(const std::string& Name, int Value) const;
    void SetFloat(const std::string& Name, float Value) const;

private:
    void CheckCompileErrors(unsigned int Shader, std::string Type);
};
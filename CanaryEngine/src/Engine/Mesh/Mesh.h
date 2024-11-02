#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

class ShaderProgram;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int ID;
    std::string Type;
    std::string Path;  // we store the path of the texture to compare with other textures
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> InVertices, std::vector<unsigned int> InIndices, std::vector<Texture> InTextures);

    void Draw(ShaderProgram& Shader);
private:
    void SetupMesh();

    // mesh data
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<Texture> Textures;

    //  render data
    unsigned int VAO, VBO, EBO;
};
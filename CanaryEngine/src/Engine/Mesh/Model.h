#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Engine/Shader/ShaderProgram.h"
#include "Mesh.h"

class Model
{
public:
	Model(std::string FilePath);

	void Draw(ShaderProgram& Shader);

private:
    void LoadModel(std::string FilePath);

    void ProcessNode(aiNode* Node, const aiScene* Scene);
    Mesh ProcessMesh(aiMesh* InMesh, const aiScene* Scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial* Material, aiTextureType Type, std::string TypeName);

    unsigned int TextureFromFile(const char* InFilePath, const std::string& InDirectory, bool bGamma = false);

    // model data
    std::vector<Mesh> Meshes;
    std::string Directory;

    std::vector<Texture> LoadedTextures;
};
#include "Model.h"

#include <stb/stb_image.h>

Model::Model(std::string FilePath)
{
	LoadModel(FilePath);
}

void Model::Draw(ShaderProgram& Shader)
{
	for (unsigned int i = 0; i < Meshes.size(); i++)
	{
		Meshes[i].Draw(Shader);
	}
}

void Model::LoadModel(std::string FilePath)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
		return;
	}
	Directory = FilePath.substr(0, FilePath.find_last_of('/'));

	ProcessNode(Scene->mRootNode, Scene);
}

void Model::ProcessNode(aiNode* Node, const aiScene* Scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		Meshes.push_back(ProcessMesh(mesh, Scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i], Scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* InMesh, const aiScene* Scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < InMesh->mNumVertices; i++)
	{
		Vertex vertex;
		// Process vertex positions, normals and texture coordinates

		// Positions
		glm::vec3 vector;
		vector.x = InMesh->mVertices[i].x;
		vector.y = InMesh->mVertices[i].y;
		vector.z = InMesh->mVertices[i].z;
		vertex.Position = vector;

		// Normals
		if (InMesh->HasNormals())
		{
			vector.x = InMesh->mNormals[i].x;
			vector.y = InMesh->mNormals[i].y;
			vector.z = InMesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if (InMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = InMesh->mTextureCoords[0][i].x;
			vec.y = InMesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else 
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < InMesh->mNumFaces; i++)
	{
		aiFace face = InMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (InMesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = Scene->mMaterials[InMesh->mMaterialIndex];

		std::vector<Texture> DiffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());

		std::vector<Texture> SpecularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), SpecularMaps.begin(), SpecularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* Material, aiTextureType Type, std::string TypeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < Material->GetTextureCount(Type); i++)
	{
		aiString str;
		Material->GetTexture(Type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < LoadedTextures.size(); j++)
		{
			if (std::strcmp(LoadedTextures[j].Path.data(), str.C_Str()) == 0)
			{
				textures.push_back(LoadedTextures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), Directory);
			texture.Type = TypeName;
			texture.Path = str.C_Str();
			textures.push_back(texture);
			LoadedTextures.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char* InFilePath, const std::string& InDirectory, bool bGamma)
{
	std::string Filename = std::string(InFilePath);
	Filename = InDirectory + '/' + Filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(Filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1) 
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << InFilePath << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
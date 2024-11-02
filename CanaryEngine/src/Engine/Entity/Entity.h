#pragma once

#include <glm/glm.hpp>

class Model;
class ShaderProgram;

// An 'Entity' is a representation of an object that exists within the game world that has a position and scale
class Entity
{
public:
	// Called every frame
	virtual void Tick(ShaderProgram& Shader, float InDeltaTime);

private:
	// The 3D model representing this entity (can be null)
	Model* EntityModel = nullptr;

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
};
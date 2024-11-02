#include "Entity.h"

#include "Engine/Mesh/Model.h"
#include "Engine/Shader/ShaderProgram.h"

void Entity::Tick(ShaderProgram& Shader, float InDeltaTime)
{
	assert(EntityModel != nullptr);

	EntityModel->Draw(Shader);
}
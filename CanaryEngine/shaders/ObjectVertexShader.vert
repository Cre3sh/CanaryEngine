#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;  // Position in world space
out vec3 Normal;   // Normal in world space
out vec2 TexCoords;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
    // Transform vertex position into world space
    FragPos = vec3(ModelMatrix * vec4(aPos, 1.0));

    // Transform the normal to world space
    Normal = mat3(transpose(inverse(ModelMatrix))) * aNormal;

    TexCoords = aTexCoords;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(aPos, 1.0);
}
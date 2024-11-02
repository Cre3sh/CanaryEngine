#include "Mesh.h"

#include <glad/glad.h> // Holds all OpenGL type declarations

#include "Engine/Shader/ShaderProgram.h"

Mesh::Mesh(std::vector<Vertex> InVertices, std::vector<unsigned int> InIndices, std::vector<Texture> InTextures)
{
	Vertices = InVertices;
	Indices = InIndices;
	Textures = InTextures;

    SetupMesh();
}

void Mesh::Draw(ShaderProgram& Shader)
{
    unsigned int DiffuseNum = 1;
    unsigned int SpecularNum = 1;
    for (unsigned int i = 0; i < Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string Number;
        std::string Name = Textures[i].Type;
        if (Name == "texture_diffuse")
            Number = std::to_string(DiffuseNum++);
        else if (Name == "texture_specular")
            Number = std::to_string(SpecularNum++);

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(Shader.ID, (Name + Number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, Textures[i].ID);
    }

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
    //////////////////////////////////////
    // VERTEX ARAY OBJECT (VBO)         //
    /////////////////////////////////////

    // a Vertex Array Object (VAO) encapsulates the entire "state" (In OpenGL, "state" refers to all the settings, configurations,
    // and data that OpenGL uses to manage how rendering is done) necessary for describing vertex data, so you don’t need to 
    // repeatedly bind buffers and set vertex attributes (position, color etc.) every 
    // time you draw something. Once you configure the VAO, you can bind 
    // it anytime and all the previously set configurations are remembered.

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // In OpenGL, binding means making an object (like a buffer or a texture) active
    // or current so that any operations you perform apply to that object.
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // In this case, we are specifying the target of the buffer (GL_ARRAY_BUFFER), the size of the data (in bytes),
    // The actual data (our array of vertices) and a usage hint telling OpenGL that the data will not change often
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

    // An EBO is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw.
    // If we want to draw a square using two triangles. Instead of defining each corner of the square multiple times,
    // we can define each corner once and then use indices to refer to these corners.
    // This makes our program more memory efficient as we don’t need to repeat vertex data for vertices that are shared between shapes.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
    
    // glVertexAttribPointer defines how OpenGL should interpret the vertex data stored in a Vertex Buffer Object (VBO).

    // 0: This is the attribute location. In our vertex shader, we wrote: layout(location = 0) in vec3 aPos;
    // This tells OpenGL that this vertex attribute corresponds to the vertex position and should be accessed using index 0.

    // 3: This parameter specifies the size of the vertex attribute, the attribute is a vec3 so it is composed of 3 values

    // GL_FLOAT: this specifies the type of the data (in this case float)

    // GL_FALSE: This specifies whether fixed-point data values should be normalized.
    // Since we are using floating-point data for positions, we set this to GL_FALSE

    // second last argument: This argument is known as the stride and tells us the space between consecutive vertex attributes.
    // https://stackoverflow.com/questions/22296510/what-does-stride-mean-in-opengles#:~:text=Amount%20of%20bytes%20from%20the,means%20they%20are%20tightly%20packed.

    // (void*)0): This parameter is the offset within the buffer where this attribute begins.

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
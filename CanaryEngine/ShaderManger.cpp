#include "ShaderManger.h"

ShaderManger::ShaderManger(const char* VertexPath, const char* FragmentPath)
{
    // 1. Retrieve the vertex/fragment source code from our filePath
    std::string VertexCode;
    std::string FragmentCode;
    std::ifstream VextexShaderFile;
    std::ifstream FragmentShaderFile;

    try
    {
        // Open files
        VextexShaderFile.open(VertexPath);
        FragmentShaderFile.open(FragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << VextexShaderFile.rdbuf();
        fShaderStream << FragmentShaderFile.rdbuf();
        // close file handlers
        VextexShaderFile.close();
        FragmentShaderFile.close();
        // convert stream into string
        VertexCode = vShaderStream.str();
        FragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    const char* VertexShaderCode = VertexCode.c_str();
    const char* FragmentShaderCode = FragmentCode.c_str();

    // 2. compile shaders
    unsigned int Vertex, Fragment;

    // vertex shader
    Vertex = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(Vertex, 1, &VertexShaderCode, NULL);
    glCompileShader(Vertex);
    CheckCompileErrors(Vertex, "VERTEX");

    // fragment Shader
    Fragment = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(Fragment, 1, &FragmentShaderCode, NULL);
    glCompileShader(Fragment);
    CheckCompileErrors(Fragment, "FRAGMENT");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, Vertex);
    glAttachShader(ID, Fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
}

void ShaderManger::Use()
{
    glUseProgram(ID);
}

void ShaderManger::SetBool(const std::string& Name, bool Value) const
{
    glUniform1i(glGetUniformLocation(ID, Name.c_str()), (int)Value);
}

void ShaderManger::SetInt(const std::string& Name, int Value) const
{
    glUniform1i(glGetUniformLocation(ID, Name.c_str()), Value);
}

void ShaderManger::SetFloat(const std::string& Name, float Value) const
{
    glUniform1f(glGetUniformLocation(ID, Name.c_str()), Value);
}

void ShaderManger::CheckCompileErrors(unsigned int Shader, std::string Type)
{
    int success;
    char infoLog[1024];
    if (Type != "PROGRAM")
    {
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(Shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << Type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(Shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(Shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << Type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

#include "../../pch.hpp"
#include "Shader.hpp"

Shader::Shader(pcstr filePath, GLenum type)
{
    spdlog::info("Reading and creating a shader: {}", filePath);
    this->shader = glCreateShader(type);

    this->shaderPath = filePath;
    this->shaderCode = ray::file_system::read_file(this->shaderPath);
}

Shader::~Shader()
{
    glDeleteShader(this->shader);
}

void Shader::Compile()
{
    spdlog::info("Compiling a shader: {}", this->shaderPath);
    char const* VertexSourcePointer = this->shaderCode.c_str();
    glShaderSource(this->shader, 1, &VertexSourcePointer, 0);
    glCompileShader(this->shader);

    GLint compile, info;

    glGetShaderiv(this->shader, GL_COMPILE_STATUS, &compile);
    glGetShaderiv(this->shader, GL_INFO_LOG_LENGTH, &info);

    if (info > NULL)
    {
        std::vector<char> ShaderErrorMessage(info + 1);
        glGetShaderInfoLog(this->shader, info, NULL, &ShaderErrorMessage[0]);
        spdlog::error(&ShaderErrorMessage[0]);
    }
}
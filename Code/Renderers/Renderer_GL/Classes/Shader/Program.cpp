#include "../../pch.hpp"
#include "Program.hpp"

Program::Program()
{
	spdlog::info("Creating a shader program");
	this->program = glCreateProgram();
}

Program::~Program()
{
	spdlog::info("Uninstall a shader program");
	glDeleteProgram(this->program);
}

void Program::Attach(Shader* shader)
{
	spdlog::info("Attaching a shader {} to a shader program", shader->shaderPath);
	glAttachShader(this->program, shader->shader);
}

void Program::Link()
{
	spdlog::info("Attaching a shader program");
	glLinkProgram(this->program);

	GLint compile, info;

	glGetShaderiv(this->program, GL_COMPILE_STATUS, &compile);
	glGetShaderiv(this->program, GL_INFO_LOG_LENGTH, &info);

	if (info > NULL)
	{
		std::vector<char> ProgramErrorMessage(info + 1);
		glGetShaderInfoLog(this->program, info, NULL, &ProgramErrorMessage[0]);
		spdlog::error(&ProgramErrorMessage[0]);
	}
}

void Program::Use()
{
	glUseProgram(this->program);
}

GLint Program::Attrib(const GLchar* name) const
{
	return glGetAttribLocation(this->program, name);
}

GLuint Program::GetUniform(pcstr name)
{
	return glGetUniformLocation(this->program, name);
}

void Program::SetUniform(const GLchar* name, const GLint& value)
{
    glUniform1i(GetUniform(name), value);
}

void Program::SetUniform(const GLchar* name, const GLuint& value)
{
    glUniform1ui(GetUniform(name), value);
}

void Program::SetUniform(const GLchar* name, const GLfloat& value)
{
    glUniform1f(GetUniform(name), value);
}

void Program::SetUniform(const GLchar* name, const glm::vec2& value)
{
    glUniform2f(GetUniform(name), value.x, value.y);
}

void Program::SetUniform(const GLchar* name, const glm::vec3& value)
{
    glUniform3f(GetUniform(name), value.x, value.y, value.z);
}

void Program::SetUniform(const GLchar* name, const glm::vec4& value)
{
    glUniform4f(GetUniform(name), value.x, value.y, value.z, value.w);
}

void Program::SetUniform(const GLchar* name, const glm::mat3& value)
{
    glUniformMatrix3fv(GetUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetUniform(const GLchar* name, const glm::mat4& value)
{
    glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}
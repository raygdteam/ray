#pragma once

class Shader
{
public:
	Shader(pcstr, GLenum);
	~Shader();

	void Compile();

	GLuint shader;
	std::string shaderCode;
	pcstr shaderPath;
};
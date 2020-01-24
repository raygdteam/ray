#pragma once

#include "Shader.hpp"

class Program
{
public:
	Program();
	~Program();

	void Attach(Shader*);
	void Link();
	void Use();

    GLint Attrib(const GLchar*) const;
    GLuint GetUniform(pcstr);

    void SetUniform(const GLchar*, const GLint&);
    void SetUniform(const GLchar*, const GLuint&);
    void SetUniform(const GLchar*, const GLfloat&);
    void SetUniform(const GLchar*, const glm::vec2&);
    void SetUniform(const GLchar*, const glm::vec3&);
    void SetUniform(const GLchar*, const glm::vec4&);
    void SetUniform(const GLchar*, const glm::mat3&);
    void SetUniform(const GLchar*, const glm::mat4&);

private:
	GLuint program;
};
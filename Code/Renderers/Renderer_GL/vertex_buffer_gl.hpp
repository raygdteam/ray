#pragma once
class VertexBuffer_GL
{
public:
	VertexBuffer_GL();

	void CreateBuffer(void* vertices, GLsizei size);
	void Bind();
	void Unbind();
	void Destroy();

private:
	GLuint m_VBO;

};


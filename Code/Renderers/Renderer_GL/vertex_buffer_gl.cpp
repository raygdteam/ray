#include "pch.hpp"
#include "vertex_buffer_gl.hpp"


VertexBuffer_GL::VertexBuffer_GL()
{
	glGenBuffers(1, &m_VBO);
}

void VertexBuffer_GL::CreateBuffer(void* vertices, GLsizei size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexBuffer_GL::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void VertexBuffer_GL::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer_GL::Destroy()
{
	glDeleteBuffers(1, &m_VBO);
}

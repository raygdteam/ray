#include "../../pch.hpp"
#include "vertex.hpp"

namespace ray::renderer::detail::opengl::classes
{
	vertex::vertex(const void* data)
	{
		glCreateBuffers(1, &_vertexBuffer);
		glNamedBufferData(_vertexBuffer, sizeof(data), data, GL_STATIC_DRAW);
		
		glCreateBuffers(1, &_vertexArray);
		glEnableVertexAttribArray(0);
		glBindVertexBuffer(0, _vertexBuffer, 0, 12);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	}

	vertex::~vertex()
	{
		// maybe this dont work

		glDeleteBuffers(1, &_vertexBuffer);
		glDeleteBuffers(1, &_vertexArray);
		glDisableVertexAttribArray(0);
	}

	void vertex::bind()
	{
		glBindVertexArray(_vertexArray);
	}
}
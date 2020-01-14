#pragma once

namespace ray::renderer::detail::opengl::classes
{
	class vertex
	{
	public:
		vertex(const void*);
		~vertex();

		void bind();

	private:
		GLuint _vertexArray;
		GLuint _vertexBuffer;
	};
}

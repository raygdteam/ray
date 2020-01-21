#pragma once

#include <assimp/mesh.h>

class Model
{
public:
	struct Mesh
	{
		enum buffers {
			vertex, texcoord, normal, index
		};
		GLuint _vao;
		GLuint _vbo[4];

		unsigned int elementsCount;

		Mesh(aiMesh* mesh);
		~Mesh();

		void Draw();
	};

	std::vector<Mesh*> meshEntries;

	Model(const char* filename);
	~Model(void);

	void Draw();
};
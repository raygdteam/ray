#pragma once

#include <assimp/mesh.h>

class Mesh
{
public:
	GLuint _vao;
	GLuint _vbo;

	unsigned int elementsCount;

	Mesh(aiMesh* mesh);
	~Mesh();

	void Draw();
};

extern std::vector<Mesh*> meshEntries;
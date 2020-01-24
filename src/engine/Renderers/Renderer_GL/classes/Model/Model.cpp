#include "../../pch.hpp"
#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

Model::Mesh::Mesh(aiMesh* mesh)
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	elementsCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions())
	{
		float* vertices = new float[mesh->mNumVertices * 3];
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &_vbo[vertex]);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[vertex]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete[] vertices;
	}


	if (mesh->HasTextureCoords(0))
	{
		float* texCoords = new float[mesh->mNumVertices * 2];
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &_vbo[texcoord]);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[texcoord]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete[] texCoords;
	}

	if (mesh->HasNormals())
	{
		float* normals = new float[mesh->mNumVertices * 3];
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &_vbo[normal]);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo[normal]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete[] normals;
	}

	if (mesh->HasFaces())
	{
		unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
		for (size_t i = 0; i < mesh->mNumFaces; ++i)
		{
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &_vbo[index]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);

		delete[] indices;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::Mesh::~Mesh()
{
	if (_vbo[vertex])
		glDeleteBuffers(1, &_vbo[vertex]);

	if (_vbo[texcoord])
		glDeleteBuffers(1, &_vbo[texcoord]);

	if (_vbo[normal])
		glDeleteBuffers(1, &_vbo[normal]);

	if (_vbo[index])
		glDeleteBuffers(1, &_vbo[index]);

	glDeleteVertexArrays(1, &_vao);
}

void Model::Mesh::Draw()
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, elementsCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

Model::Model(const char* filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, NULL);
	if (!scene)
		spdlog::error(importer.GetErrorString());

	for (size_t i = 0; i < scene->mNumMeshes; ++i)
		meshEntries.push_back(new Mesh(scene->mMeshes[i]));
}

Model::~Model(void)
{
	for (size_t i = 0; i < meshEntries.size(); ++i)
		delete meshEntries.at(i);

	meshEntries.clear();
}

void Model::Draw()
{
	for (size_t i = 0; i < meshEntries.size(); ++i)
		meshEntries.at(i)->Draw();
}
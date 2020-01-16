#include "../../pch.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

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

std::vector<Mesh*> meshEntries;
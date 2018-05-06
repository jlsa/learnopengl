#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>



unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
public:
	/* Functions */
	Model(std::string const &path, bool gamma = false)
		: gammaCorrection(gamma)
	{
		//std::cout << path << std::endl;
		loadModel(path);
	}
	~Model();

	void Draw(Shader shader);
	void DrawMesh(Shader shader);

private:
	/* Model Data*/
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	/* Functions */
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);

};


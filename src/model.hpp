#ifndef VOXELS_MODEL_HPP
#define VOXELS_MODEL_HPP

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "shader.hpp"

class Model {
public:
    Model(string path);
    void draw(Shader &shader);

private:
    vector<Mesh> meshes;
    vector<Texture2D> loadedTextures;
    string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture2D> loadMaterialTextures(aiMaterial *material, aiTextureType aiType, TextureType type);
};

#endif
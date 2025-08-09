#include <iostream>
#include <algorithm>

#include "model.hpp"

Model::Model(string path) {
    loadModel(path);
}

void Model::draw(Shader &shader) {
    for (Mesh &mesh : meshes) {
        mesh.draw(shader);
    }
}

void Model::loadModel(string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "Assimp error: " << importer.GetErrorString() << endl;
        exit(1);
    }

    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // Process all the node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // Then process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture2D> textures;

    // Gathering vertex data
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // Gathering vertex indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Gathering textures
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture2D> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
        vector<Texture2D> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

vector<Texture2D> Model::loadMaterialTextures(aiMaterial *material, aiTextureType aiType, TextureType type) {
    vector<Texture2D> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(aiType); i++) {
        aiString str;
        material->GetTexture(aiType, i, &str);
        string texturePath = directory + '/';
        texturePath.append(str.data);

        auto cachedTextureIt = std::find_if(loadedTextures.cbegin(), loadedTextures.cend(), [texturePath](const Texture2D &texture) -> bool {
            return texture.path == texturePath;
        });

        if (cachedTextureIt != loadedTextures.cend()) {
            textures.push_back(*cachedTextureIt);
        } else {
            Texture2D newTexture(texturePath, type, GL_TEXTURE0);
            textures.push_back(newTexture);
            loadedTextures.push_back(newTexture);
        }
    }
    return textures;
}

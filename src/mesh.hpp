#ifndef VOXELS_MESH_HPP
#define VOXELS_MESH_HPP

#include <vector>
#include <glm/glm.hpp>

#include "texture2D.hpp"
#include "shader.hpp"

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public: 
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture2D> textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture2D> textures);
    void draw(Shader &shader);

private:
    // Buffer objects needed for rendering
    GLuint VAO, VBO, EBO;
    void setupMesh();
};

#endif
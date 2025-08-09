#include "mesh.hpp"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture2D> textures)
        : vertices(vertices), indices(indices), textures(textures) {
    setupMesh();
}

void Mesh::draw(Shader &shader) {
    unsigned int diffuseNumber = 1;
    unsigned int specularNumber = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        Texture2D &texture = textures[i];
        string name, number;
        if (texture.type == TextureType::DIFFUSE) {
            name = "diffuse";
            number = std::to_string(diffuseNumber++);
        } else if (texture.type == TextureType::SPECULAR) {
            name = "specular";
            number = std::to_string(specularNumber++);
        }
        texture.bind(GL_TEXTURE0 + i);
        shader.setIntUniform(("material." + name + number).c_str(), i);
    }

    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind the vertex array so other code won't mess it up
}
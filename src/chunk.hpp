#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <vector>

#include "shader.hpp"
#include "math/vectors.hpp"
#include "block.hpp"

#define CHUNK_SIZE 32
#define CHUNK_HEIGHT 64

class Chunk {
    Vec2i chunkCoordinate;
    Block content[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];
    vector<float> mesh {};
    GLuint VAO = 0, VBO = 0;
    bool recomputeMeshPending = false;

    void recomputeMesh();

public:
    explicit Chunk(Vec2i chunkCoordinate);
    [[nodiscard]] Block getBlock(const Vec3i& pos) const;
    void setBlock(const Vec3i& pos, const Block& block);

    void draw(Shader &shader);
};

Vec2i blockPosToChunkPos(Vec3i blockPos);

#endif
#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "shader.hpp"
#include "math/vectors.hpp"

#define CHUNK_SIZE 32
#define CHUNK_HEIGHT 64

enum class Block {
    AIR, SOLID
};

class Chunk {
    Vec2i chunkCoordinate;
    Block content[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];

public:
    explicit Chunk(Vec2i chunkCoordinate);
    [[nodiscard]] Block getBlock(int x, int y, int z) const;
    void setBlock(int x, int y, int z, Block block);

    void draw(Shader &shader, GLuint cubeVAO) const;
};

Vec2i blockPosToChunkPos(Vec3i blockPos);

#endif